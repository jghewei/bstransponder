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
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "coreotn_loc.h"

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
** Public Functions
*/

/*******************************************************************************
* coreotn_ghao_db_chnl_rec_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate calendar slots to the list of tribslots in argument. The old
*   tribslots are deallocated, then the new tribslots are allocated. In the
*   case of tribslot increase, it is assumed that there available calendar
*   slots available.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   db_id                  - Database ID in COREOTN
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ho_chnl_id             - HO channel ID
*   lo_chnl_id             - LO channel ID
*   new_ts                 - Array with the tribslots to use
*
* OUTPUTS:
*   old_cal_mask           - Old calendar mask for LO channel
*   new_cal_mask           - New calendar mask for LO channel
*   cal_slots_to_add_remove_num  - Number of calendar slots to add or remove
*   cal_slots_to_add_remove      - List of ordered calendar slots to add/remove
*
* RETURNS:
*   PMC_ERROR           - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_ghao_db_chnl_rec_update(coreotn_handle_t *coreotn_handle,
                                                 util_gen_db_id_t db_id,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 ho_chnl_id,
                                                 UINT32 lo_chnl_id,
                                                 BOOL8  new_ts[ODTU_MUX_NUM_CHANL],
                                                 UINT32 old_cal_mask[3],
                                                 UINT32 new_cal_mask[3],
                                                 UINT32 *cal_slots_to_add_remove_num,
                                                 UINT32 cal_slots_to_add_remove[79])
{
    UINT32 db_handle;
    line_core_otn_db_key key;
    line_core_otn_db_rec new_rec;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    UINT32 num_of_recs;
    UINT32 db_rec_id;
    UINT32 ts_id;
    UINT32 i;
    UINT32 cal_slots_add_remove_cnt;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_TX_DB)
    {
        db_handle = coreotn_handle->cfg.tx_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        db_handle = coreotn_handle->cfg.rx_db_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_ghao_db_chnl_rec_update(): db_id=%d, mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d\n", db_id, mux_stage, ho_chnl_id, lo_chnl_id);
    UINT32 cal_index;
    PMC_LOG_TRACE("Tribslots: ");
    for (cal_index = 0; cal_index < ODTU_MUX_NUM_CHANL; ++cal_index)
    {
        if (new_ts[cal_index] == 1)
        {
            PMC_LOG_TRACE("%d ", cal_index);
        }
    }
    PMC_LOG_TRACE("\n");

    /* Initialize masks */
    for (i=0; i<3; ++i)
    {
        old_cal_mask[i] = 0;
        new_cal_mask[i] = 0;
    }

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        /* mo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = lo_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        /* lo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = lo_chnl_id;

    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(coreotn_handle->base),
                               db_id,
                               db_handle,
                               (void *)&key,
                               (void **)&q_result);

    if (num_of_recs == 0)
    {
        PMC_RETURN(COREOTN_ERR_DB_ENTRY_NOT_FOUND);
    }
    temp_result = *q_result;

    for (db_rec_id = 0; db_rec_id < num_of_recs; db_rec_id++)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Check old: ho_channel=%d, lo_channel=%d, num_of_recs=%d, db_rec_id=%d, calendar_entry=%d, trib_slot=%d, status=%d\n",
                  ho_chnl_id,
                  lo_chnl_id,
                  num_of_recs,
                  db_rec_id,
                  temp_result.p_recs[db_rec_id]->calendar_entry,
                  temp_result.p_recs[db_rec_id]->trib_slot,
                  temp_result.p_recs[db_rec_id]->cal_entry_status);

        /* Update DB */
        if (mux_stage == COREOTN_MUX_STAGE_ONE)
        {
            if ( temp_result.p_recs[db_rec_id]->mo_channel != ODTU_MUX_DB_ENTRY_UNKNOWN )
            {
                new_rec = *temp_result.p_recs[db_rec_id];
                new_rec.ho_channel = ho_chnl_id;
                new_rec.mo_channel = ODTU_MUX_DB_ENTRY_UNKNOWN;
                new_rec.lo_channel = ODTU_MUX_DB_ENTRY_UNKNOWN;
                new_rec.trib_slot = COREOTN_DB_ENTRY_UNKNOWN;
                new_rec.cal_entry_status = 0;

                PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Deallocate cal_slot: ho_chnl_id=%d, trib_slot=%d, mo_channel=%d, calendar_entry=%d, status=%d\n",
                          new_rec.ho_channel,
                          new_rec.trib_slot,
                          new_rec.mo_channel,
                          new_rec.calendar_entry,
                          new_rec.cal_entry_status);

                result = gen_db_update_record(&(coreotn_handle->base),
                                              db_id,
                                              db_handle,
                                              &new_rec);
                if (result != PMC_SUCCESS) {
                    PMC_RETURN(result);
                }

                /* Add to old calendar mask */
                old_cal_mask[temp_result.p_recs[db_rec_id]->calendar_entry/32] |= (1 << (temp_result.p_recs[db_rec_id]->calendar_entry%32));
            }
        }
        else if (mux_stage == COREOTN_MUX_STAGE_TWO)
        {
            if ( temp_result.p_recs[db_rec_id]->lo_channel != ODTU_MUX_DB_ENTRY_UNKNOWN )
            {
                new_rec = *temp_result.p_recs[db_rec_id];
                new_rec.calendar_entry = temp_result.p_recs[db_rec_id]->calendar_entry;
                new_rec.lo_channel = ODTU_MUX_DB_ENTRY_UNKNOWN;
                new_rec.ts_port_mo_lo = COREOTN_DB_ENTRY_UNKNOWN;
                new_rec.cal_entry_status = 0;

                PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Deallocate cal_slot: mo_chnl_id=%d, trib_slot=%d, lo_channel=%d, calendar_entry=%d, status=%d\n",
                          new_rec.mo_channel,
                          new_rec.trib_slot,
                          new_rec.lo_channel,
                          new_rec.calendar_entry,
                          new_rec.cal_entry_status);

                result = gen_db_update_record(&(coreotn_handle->base),
                                              db_id,
                                              db_handle,
                                              &new_rec);
                if (result != PMC_SUCCESS) {
                    PMC_RETURN(result);
                }

                /* Add to old calendar mask */
                old_cal_mask[new_rec.calendar_entry/32] |= (1 << (new_rec.calendar_entry%32));
            }
        }
    }


    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        /* query the record to update (only supports stage one) */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = ho_chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        /* query the record to update (only supports stage one) */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = ho_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(coreotn_handle->base),
                               db_id,
                               db_handle,
                               (void *)&key,
                               (void **)&q_result);
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
    }
    temp_result = *q_result;

    /* Update the database for the LO channel ID */
    /* ts could range from 0 - max num ts in HO */
    for (ts_id = 0; ts_id < num_of_recs; ++ts_id)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Check new: ho_channel=%d, lo_channel=%d, ts_id=%d, new_ts=%d, calendar_entry=%d, trib_slot=%d, status=%d\n",
                  ho_chnl_id,
                  lo_chnl_id,
                  ts_id,
                  new_ts[ts_id],
                  temp_result.p_recs[ts_id]->calendar_entry,
                  temp_result.p_recs[ts_id]->trib_slot,
                  temp_result.p_recs[ts_id]->cal_entry_status);

        if (new_ts[ts_id])
        {
            if (mux_stage == COREOTN_MUX_STAGE_ONE)
            {
                if ( temp_result.p_recs[ts_id]->mo_channel == ODTU_MUX_DB_ENTRY_UNKNOWN )
                {
                    new_rec = *temp_result.p_recs[ts_id];
                    /* calendar entry is not assigned to any tributary slot/LO channel */
                    /* update MO channel ID to 'calendar_entry' */
                    new_rec.calendar_entry = temp_result.p_recs[ts_id]->calendar_entry;
                    new_rec.mo_channel = lo_chnl_id;
                    new_rec.trib_slot = ts_id;
                    new_rec.cal_entry_status = 1;
                    /*new_rec.lo_channel = DO_NOT_USE_KEY;*/

                    PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Allocate cal_slot: ho_chnl_id=%d, trib_slot=%d, mo_channel=%d, calendar_entry=%d, status=%d\n",
                              new_rec.ho_channel,
                              new_rec.trib_slot,
                              new_rec.mo_channel,
                              new_rec.calendar_entry,
                              new_rec.cal_entry_status);

                    /* send back the result to the DB */
                    result = gen_db_update_record(&(coreotn_handle->base),
                                                  db_id,
                                                  db_handle,
                                                  &new_rec);
                    if (result != PMC_SUCCESS) {
                        PMC_RETURN(result);
                    }

                    /* Add to new calendar mask */
                    new_cal_mask[new_rec.calendar_entry/32] |= (1 << (new_rec.calendar_entry%32));
                }
            }
            else if (mux_stage == COREOTN_MUX_STAGE_TWO)
            {
                if ( temp_result.p_recs[ts_id]->lo_channel == ODTU_DMX_DB_ENTRY_UNKNOWN )
                {
                    new_rec = *temp_result.p_recs[ts_id];
                    /* calendar entry is not assigned to any tributary slot/LO channel */
                    /* update LO channel ID to 'chnl_id' */
                    new_rec.calendar_entry = temp_result.p_recs[ts_id]->calendar_entry;
                    new_rec.lo_channel = lo_chnl_id;
                    new_rec.ts_port_mo_lo = ts_id;
                    new_rec.cal_entry_status = 1;
                    /* new_rec.ts_port_mo_lo = 1;*/

                    PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): Allocate cal_slot: mo_chnl_id=%d, trib_slot=%d, lo_channel=%d, calendar_entry=%d, status=%d\n",
                              new_rec.mo_channel,
                              new_rec.trib_slot,
                              new_rec.lo_channel,
                              new_rec.calendar_entry,
                              new_rec.cal_entry_status);

                    /* send back the result to the DB */
                    result = gen_db_update_record(&(coreotn_handle->base),
                                                  db_id,
                                                  db_handle,
                                                  &new_rec);
                    if (result != PMC_SUCCESS) {
                        PMC_RETURN(result);
                    }

                    /* Add to new calendar mask */
                    new_cal_mask[new_rec.calendar_entry/32] |= (1 << (new_rec.calendar_entry%32));
                 }
            }
            else
            {
                PMC_RETURN(COREOTN_ERR_INVALID_ARG);
            }
        }
    }

    /* Generate the calendar slots to add or remove */
    cal_slots_add_remove_cnt = 0;
    for (i = 0; i < 79; ++i)
    {
        cal_slots_to_add_remove[i] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
    }
    for(i=0; i<96; ++i)
    {
        if( (((old_cal_mask[i/32] & (1 << (i%32))) ^ (new_cal_mask[i/32] & (1 << (i%32)))) & (1<<(i%32))) != 0 )
        {
            cal_slots_to_add_remove[cal_slots_add_remove_cnt] = i;
            cal_slots_add_remove_cnt++;
        }
    }

    if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_TX_DB)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): coreotn_tx_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): coreotn_tx_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): coreotn_rx_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): coreotn_rx_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }

    PMC_LOG_TRACE("coreotn_ghao_db_chnl_rec_update(): cal_slots_to_add_remove=%d,\n", cal_slots_add_remove_cnt);
    for (i = 0; i < cal_slots_add_remove_cnt; ++i)
    {
        PMC_LOG_TRACE("cal_slots_to_add_remove[%d]: %d\n", i, cal_slots_to_add_remove[i]);
    }

    *cal_slots_to_add_remove_num = cal_slots_add_remove_cnt;

    PMC_RETURN(result);

} /* coreotn_ghao_db_chnl_rec_update */

/*******************************************************************************
* coreotn_ghao_db_chnl_get_masks
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reserve calendar slots to the list of tribslots in argument. The old
*   tribslots are unreserved, then the new tribslots are resverved. In the
*   case of tribslot increase, it is assumed that there available calendar
*   slots available.
*   This function only generates the old and new calendar slot masks and
*   doesn't make any change to the database.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   db_id                  - Database ID in COREOTN
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ho_chnl_id             - HO channel ID
*   lo_chnl_id             - LO channel ID
*   new_ts                 - Array with the tribslots to use
*
* OUTPUTS:
*   old_cal_mask           - Old calendar mask for LO channel
*   new_cal_mask           - New calendar mask for LO channel
*   cal_slots_to_add_remove_num  - Number of calendar slots to add or remove
*   cal_slots_to_add_remove      - List of ordered calendar slots to add/remove
*
* RETURNS:
*   PMC_ERROR           - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_ghao_db_chnl_get_masks(coreotn_handle_t *coreotn_handle,
                                                util_gen_db_id_t db_id,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT32 ho_chnl_id,
                                                UINT32 lo_chnl_id,
                                                BOOL8  new_ts[ODTU_MUX_NUM_CHANL],
                                                UINT32 old_cal_mask[3],
                                                UINT32 new_cal_mask[3],
                                                UINT32 *cal_slots_to_add_remove_num,
                                                UINT32 cal_slots_to_add_remove[79])
{
    UINT32 db_handle;
    line_core_otn_db_key key;
    line_core_otn_db_query_res *q_result;
    UINT32 num_of_recs;
    UINT32 db_rec_id;
    UINT32 ts_id;
    UINT32 i;
    UINT32 cal_slots_add_remove_cnt;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_TX_DB)
    {
        db_handle = coreotn_handle->cfg.tx_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        db_handle = coreotn_handle->cfg.rx_db_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_ghao_db_chnl_get_masks(): db_id=%d, mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d\n", db_id, mux_stage, ho_chnl_id, lo_chnl_id);
    UINT32 cal_index;
    PMC_LOG_TRACE("Tribslots: ");
    for (cal_index = 0; cal_index < ODTU_MUX_NUM_CHANL; ++cal_index)
    {
        if (new_ts[cal_index] == 1)
        {
            PMC_LOG_TRACE("%d ", cal_index);
        }
    }
    PMC_LOG_TRACE("\n");

    /* Initialize masks */
    for (i=0; i<3; ++i)
    {
        old_cal_mask[i] = 0;
        new_cal_mask[i] = 0;
    }

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        /* mo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = lo_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        /* lo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = lo_chnl_id;

    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(coreotn_handle->base),
                               db_id,
                               db_handle,
                               (void *)&key,
                               (void **)&q_result);

    if (num_of_recs == 0)
    {
        PMC_RETURN(COREOTN_ERR_DB_ENTRY_NOT_FOUND);
    }

    for (db_rec_id = 0; db_rec_id < num_of_recs; db_rec_id++)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): Check old: ho_channel=%d, lo_channel=%d, num_of_recs=%d, db_rec_id=%d, new_ts=%d, calendar_entry=%d, trib_slot=%d, status=%d\n",
                  ho_chnl_id,
                  lo_chnl_id,
                  num_of_recs,
                  db_rec_id,
                  new_ts[q_result->p_recs[db_rec_id]->trib_slot],
                  q_result->p_recs[db_rec_id]->calendar_entry,
                  q_result->p_recs[db_rec_id]->trib_slot,
                  q_result->p_recs[db_rec_id]->cal_entry_status);

        /* Get old calendar mask */
        if (mux_stage == COREOTN_MUX_STAGE_ONE)
        {
            if ( q_result->p_recs[db_rec_id]->mo_channel != ODTU_MUX_DB_ENTRY_UNKNOWN )
            {
                /* Add to old calendar mask */
                old_cal_mask[q_result->p_recs[db_rec_id]->calendar_entry/32] |= (1 << (q_result->p_recs[db_rec_id]->calendar_entry%32));
                if (new_ts[q_result->p_recs[db_rec_id]->trib_slot] == TRUE)
                {
                    /* Add to new calendar mask */
                    new_cal_mask[q_result->p_recs[db_rec_id]->calendar_entry/32] |= (1 << (q_result->p_recs[db_rec_id]->calendar_entry%32));
                };
            }
        }
        else if (mux_stage == COREOTN_MUX_STAGE_TWO)
        {
            if ( q_result->p_recs[db_rec_id]->lo_channel != ODTU_MUX_DB_ENTRY_UNKNOWN )
            {
                /* Add to old calendar mask */
                old_cal_mask[q_result->p_recs[db_rec_id]->calendar_entry/32] |= (1 << (q_result->p_recs[db_rec_id]->calendar_entry%32));
                if (new_ts[q_result->p_recs[db_rec_id]->ts_port_mo_lo] == TRUE)
                {
                    /* Add to new calendar mask */
                    new_cal_mask[q_result->p_recs[db_rec_id]->calendar_entry/32] |= (1 << (q_result->p_recs[db_rec_id]->calendar_entry%32));
                };
            }
        }
    }



    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        /* query the record to update (only supports stage one) */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = ho_chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        /* query the record to update (only supports stage one) */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = ho_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(coreotn_handle->base),
                               db_id,
                               db_handle,
                               (void *)&key,
                               (void **)&q_result);
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
    }

    /* Update the database for the LO channel ID */
    /* ts could range from 0 - max num ts in HO */
    for (ts_id = 0; ts_id < num_of_recs; ++ts_id)
    {
        if (new_ts[ts_id] == TRUE)
        {
            PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): Reserve new: ho_channel=%d, lo_channel=%d, ts_id=%d, new_ts=%d, calendar_entry=%d, trib_slot=%d, status=%d\n",
                      ho_chnl_id,
                      lo_chnl_id,
                      ts_id,
                      new_ts[ts_id],
                      q_result->p_recs[ts_id]->calendar_entry,
                      q_result->p_recs[ts_id]->trib_slot,
                      q_result->p_recs[ts_id]->cal_entry_status);

            if (mux_stage == COREOTN_MUX_STAGE_ONE)
            {
                if ( q_result->p_recs[ts_id]->mo_channel == ODTU_MUX_DB_ENTRY_UNKNOWN )
                {
                    /* Add to new calendar mask */
                    new_cal_mask[q_result->p_recs[ts_id]->calendar_entry/32] |= (1 << (q_result->p_recs[ts_id]->calendar_entry%32));
                }
            }
            else if (mux_stage == COREOTN_MUX_STAGE_TWO)
            {
	             if ( q_result->p_recs[ts_id]->lo_channel == ODTU_DMX_DB_ENTRY_UNKNOWN )
	             {
                    /* Add to new calendar mask */
                    new_cal_mask[q_result->p_recs[ts_id]->calendar_entry/32] |= (1 << (q_result->p_recs[ts_id]->calendar_entry%32));
                }
            }
            else
            {
                PMC_RETURN(COREOTN_ERR_INVALID_ARG);
            }
        }
    }


    /* Generate the calendar slots to add or remove */
    cal_slots_add_remove_cnt = 0;
    for (i = 0; i < 79; ++i)
    {
        cal_slots_to_add_remove[i] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
    }
    for(i=0; i<96; ++i)
    {
        if( (((old_cal_mask[i/32] & (1 << (i%32)))  ^ (new_cal_mask[i/32] & (1 << (i%32)))) & (1<<(i%32))) != 0 )
        {
            cal_slots_to_add_remove[cal_slots_add_remove_cnt] = i;
            cal_slots_add_remove_cnt++;
        }
    }

    if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_TX_DB)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): coreotn_tx_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): coreotn_tx_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_LINE_CORE_OTN_RX_DB)
    {
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): coreotn_rx_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): coreotn_rx_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }

    PMC_LOG_TRACE("coreotn_ghao_db_chnl_get_masks(): cal_slots_to_add_remove=%d,\n", cal_slots_add_remove_cnt);
    for (i = 0; i < cal_slots_add_remove_cnt; ++i)
    {
        PMC_LOG_TRACE("cal_slots_to_add_remove[%d]: %d\n", i, cal_slots_to_add_remove[i]);
    }

    *cal_slots_to_add_remove_num = cal_slots_add_remove_cnt;

    PMC_RETURN(result);

} /* coreotn_ghao_db_chnl_get_masks */


/*******************************************************************************
*  coreotn_tpid_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Get the TPID value in TX and RX datapaths.
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU MUX stage instance in COREOTN
*   lo_chnl_id          - LO channel ID
*
* OUTPUTS:
*   tx_tpid             - TX TBID
*   rx_tpid             - TX TBID
*
* RETURNS:
*   PMC_ERROR           - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_tpid_get(coreotn_handle_t *coreotn_handle,
                                  coreotn_mux_stage_t mux_stage,
                                  UINT32 lo_chnl_id,
                                  UINT8 *tx_tpid,
                                  UINT8 *rx_tpid)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        *rx_tpid = coreotn_handle->var.rx_lo_chnl_ctxt[lo_chnl_id].trib_port;
        *tx_tpid = coreotn_handle->var.tx_lo_chnl_ctxt[lo_chnl_id].trib_port;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        *rx_tpid = coreotn_handle->var.rx_mo_chnl_ctxt[lo_chnl_id].trib_port;
        *tx_tpid = coreotn_handle->var.tx_mo_chnl_ctxt[lo_chnl_id].trib_port;
    }

    PMC_RETURN(result);
} /* coreotn_tpid_get */


/*******************************************************************************
*  coreotn_fo1_bwr_ind_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Set the register field BWR_IND_SEL according to muxing stage.
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU MUX stage instance in COREOTN
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR           - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_fo1_bwr_ind_sel_set(coreotn_handle_t *coreotn_handle,
                                             coreotn_mux_stage_t mux_stage)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        PMC_LOG_TRACE("Call coreotn_fo1_bwr_ind_sel_set(): mux_stage=%d, BWR_IND_SEL=%d\n", mux_stage, 1);
        coreotn_fo_field_BWR_IND_SEL_set(NULL, coreotn_handle, 0, 1);
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        PMC_LOG_TRACE("Call coreotn_fo1_bwr_ind_sel_set(): mux_stage=%d, BWR_IND_SEL=%d\n", mux_stage, 0);
        coreotn_fo_field_BWR_IND_SEL_set(NULL, coreotn_handle, 0, 0);
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_RETURN(result);
} /* coreotn_fo1_bwr_ind_sel_set */


/*******************************************************************************
* coreotn_oduksw_ghao_queue_resize_zone_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function is used for G.HAO to resize the CPB queue for a given channel
*   configured in ODUflex. The zone threshold is updated accordingly.
*
* INPUTS:
*   coreotn_handle              - handle to the COREOTN instance to be operated on
*   egress_mux_output_port      - master DPI port id
*   egress_mux_output_port_chnl - master DPI port channel id
*   halt_buffer                 - ODUk Switch FIFO buffer
*   oduflex_num_ts              - requested number of ODUflex tribslots
*   increase                    - 0: decrease; 1: increase
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
PUBLIC PMC_ERROR coreotn_oduksw_ghao_queue_resize_zone_update(coreotn_handle_t *coreotn_handle,
                                                              UINT32 egress_mux_output_port,
                                                              UINT32 egress_mux_output_port_chnl,
                                                              UINT32 halt_buffer,
                                                              UINT32 oduflex_num_ts,
                                                              BOOL increase)
{
    PMC_ERROR result = PMC_SUCCESS;
    cpb_handle_t *cpb_handle;

    PMC_ENTRY();

    cpb_handle = coreotn_handle->oduksw_handle;

    PMC_LOG_TRACE("Call coreotn_oduksw_ghao_queue_resize_zone_update(): egress_mux_output_port=%d, egress_mux_output_port_chnl=%d, halt_buffer=%d, oduflex_num_ts=%d\n", egress_mux_output_port, egress_mux_output_port_chnl, halt_buffer, oduflex_num_ts);

    if (result == PMC_SUCCESS)
    {
        result = cpb_ghao_queue_resize_zone_update( cpb_handle,
                                                    egress_mux_output_port,
                                                    egress_mux_output_port_chnl,
                                                    halt_buffer,
                                                    oduflex_num_ts,
                                                    increase);
    }

    PMC_RETURN(result);
} /* coreotn_oduksw_ghao_queue_resize_zone_update */

/*******************************************************************************
*  coreotn_mux_ghao_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize G.HAO state machines and HO_ID memory in ODTU_MUX
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*   ho_chnl_id             - High Order channel ID
*   lo_chnl_id             - Low Order channel ID
*   tpid                   - Tributary slot ID
*   cal_slots_add_rem      - Calendar slot mask of the added / removed TS
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_init(coreotn_handle_t *coreotn_handle,
                                       coreotn_mux_stage_t mux_stage,
                                       UINT8 ghao_id,
                                       UINT8 ho_chnl_id,
                                       UINT8 lo_chnl_id,
                                       UINT8 tpid,
                                       BOOL8 cal_slots_add_rem[ODTU_MUX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_init(): mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, ghao_id, ho_chnl_id, lo_chnl_id, tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_init(odtu_mux_handle,
                                    ho_chnl_id,
                                    lo_chnl_id,
                                    tpid,
                                    cal_slots_add_rem,
                                    (odtu_mux_stage_instance_t)mux_stage,
                                    ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_init */

/*******************************************************************************
*  coreotn_mux_ghao_send_add_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send ADD NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_add_nack(coreotn_handle_t *coreotn_handle,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_add_nack(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_add_nack(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_add_nack */


/*******************************************************************************
*  coreotn_mux_ghao_send_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send ADD ACK on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_add_ack(coreotn_handle_t *coreotn_handle,
                                               coreotn_mux_stage_t mux_stage,
                                               UINT8 ghao_id)

{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_add_ack(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_add_ack(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_add_ack */


/*******************************************************************************
*  coreotn_mux_ghao_send_remove_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send REM NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_remove_nack(coreotn_handle_t *coreotn_handle,
                                                   coreotn_mux_stage_t mux_stage,
                                                   UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_remove_nack(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_remove_nack(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_remove_nack */

/*******************************************************************************
*  coreotn_mux_ghao_send_remove_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send REM ACK on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_remove_ack(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mux_stage_t mux_stage,
                                                  UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_remove_ack(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_remove_ack(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_remove_ack */

/*******************************************************************************
*  coreotn_mux_ghao_send_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send TSCC on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*   tscc                   - TSCC value to be sent
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_tscc(coreotn_handle_t *coreotn_handle,
                                            coreotn_mux_stage_t mux_stage,
                                            UINT8 ghao_id,
                                            BOOL8 tscc)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_tscc(): mux_stage=%d, ghao_id=%d, tscc=%d\n", mux_stage, ghao_id, tscc);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_tscc(odtu_mux_handle, tscc, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_tscc */

/*******************************************************************************
*  coreotn_mux_ghao_send_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set RP bit in RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*   rp                     - RP value to be sent
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_rp(coreotn_handle_t *coreotn_handle,
                                          coreotn_mux_stage_t mux_stage,
                                          UINT8 ghao_id,
                                          BOOL8 rp)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_rp(): mux_stage=%d, ghao_id=%d, rp=%d\n", mux_stage, ghao_id, rp);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_rp(odtu_mux_handle, rp, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_rp */

/*******************************************************************************
*  coreotn_mux_ghao_send_norm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send NORM on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_norm(coreotn_handle_t *coreotn_handle,
                                            coreotn_mux_stage_t mux_stage,
                                            UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_norm(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_norm(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_norm */

/*******************************************************************************
*  coreotn_mux_ghao_send_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send IDLE NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_send_idle_nack(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_send_idle_nack(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_send_idle_nack(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_send_idle_nack */

/*******************************************************************************
*  coreotn_mux_ghao_update_stdby_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update standby configuration registers in currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_update_stdby_config(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_update_stdby_config(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_update_stdby_config(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);

} /* coreotn_mux_ghao_update_stdby_config */

/*******************************************************************************
*  coreotn_mux_ghao_wait_config_update_done
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Wait until configuration update is completed.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ho_chnl_id             - HO channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_wait_config_update_done(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 ho_chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_wait_config_update_done(): mux_stage=%d, ho_chnl_id=%d\n", mux_stage, ho_chnl_id);

#ifndef PMC_SW_SIMULATION
    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_config_update_poll(odtu_mux_handle, ho_chnl_id, COREOTN_POLL_COUNT_MAX);
        if (PMC_SUCCESS != result)
        {
            result = COREOTN_ERR_CHNL_INVALID_STATE;
        }
    }
#endif

    PMC_RETURN(result);

} /* coreotn_mux_ghao_wait_config_update_done */

/*******************************************************************************
*  coreotn_mux_ghao_resize_fifo
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize EXPAND_IBUF and TXJC_PROC CFC FIFOs in currently resized LO channel
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_resize_fifo(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_resize_fifo(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_resize_fifo(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_resize_fifo */

/*******************************************************************************
* coreotn_odtu_mux_xoff_thresh_change
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs per word adjustments on the xoff threshold in the ODTU_MUX
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_instance           - If 1 : mux in FMF1 is the muxing node
*                               2 : mux in FMF2 is the muxing node
*   chnl_id                - Channel id to perform the adjustment for
*   xoff_lvl               - XOFF threshold level
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR             - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_odtu_mux_xoff_thresh_change(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_instance,
                                                     UINT32 chnl_id,
                                                     UINT32 xoff_lvl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_LOG_TRACE("Call coreotn_odtu_mux_xoff_thresh_change(): mux_stage=%d, chnl_id=%d, xoff_lvl=%d\n", mux_instance, chnl_id, xoff_lvl);

    if(mux_instance == COREOTN_MUX_STAGE_ONE)
    {

            /*mux adjustments in FMF1 */
            /*expand fifo adjustment*/
           ret_val = odtu_mux_ghao_adjust_exp_xoff_lvl(coreotn_handle->odtu_mux1_handle,
                                                       chnl_id,
                                                       xoff_lvl);

    }
    else if(mux_instance == COREOTN_MUX_STAGE_TWO)
    {
            /*mux adjustments in FMF2 */
            /*expand fifo adjustment*/
           ret_val = odtu_mux_ghao_adjust_exp_xoff_lvl(coreotn_handle->odtu_mux2_handle,
                                                       chnl_id,
                                                       xoff_lvl);
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_RETURN(ret_val);

} /* coreotn_odtu_mux_xoff_thresh_change */


/*******************************************************************************
* coreotn_odtu_mux_xoff_thresh_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs per word adjustments on the xoff threshold in the ODTU_MUX. The
*   current threshold is incremented if ramping up or decremented if ramping
*   down.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_instance           - If 1 : mux in FMF1 is the muxing node
*                               2 : mux in FMF2 is the muxing node
*   chnl_id                - Channel id to perform the adjustment for
*   increase               - 0: Decrease; 1: Increase
*   xoff_lvl               - Current XOFF threshold level
*
* OUTPUTS:
*   xoff_lvl               - ConfiguredXOFF threshold level
*
* RETURNS:
*   PMC_ERROR              - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_odtu_mux_xoff_thresh_update(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_instance,
                                                     UINT32 chnl_id,
                                                     BOOL increase,
                                                     UINT32 *xoff_lvl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 xoff_thresh;

    PMC_ENTRY();

    xoff_thresh = *xoff_lvl;

    PMC_LOG_TRACE("Call coreotn_odtu_mux_xoff_thresh_update(): mux_stage=%d, chnl_id=%d, cur_xoff_lvl=%d\n", mux_instance, chnl_id, xoff_thresh);

    if (increase)
    {
        /* Increment threshold */
        xoff_thresh ++;
    }
    else
    {
        /* Decrement threshold */
        xoff_thresh --;
    }


    /* Reconfigure XOFF threshold in MUX(1-2) and/or MUX(2-3a)*/
    ret_val = coreotn_odtu_mux_xoff_thresh_change(coreotn_handle,
                                                  mux_instance,
                                                  chnl_id,
                                                  xoff_thresh);

    /* Return new threshold value */
    *xoff_lvl = xoff_thresh;


    PMC_RETURN(ret_val);

} /* coreotn_odtu_mux_xoff_thresh_update */


/*******************************************************************************
*  coreotn_mux_ghao_recfg_passthru
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Do passthru re-config to new TS count, but don't enable the 
*   additional calendars
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_recfg_passthru(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_recfg_passthru(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_recfg_passthru(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_recfg_passthru */

/*******************************************************************************
*  coreotn_mux_ghao_recfg_exp_lo_calen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Turn valid off/on on last to-be-removed Calendar
*   (by setting/clearing EXP.LO_FIRST_CYCLE)
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*   cal_slot               - LO calendar cycle that will be enabled / disabled
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_recfg_exp_lo_calen(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_stage,
                                                     UINT8 ghao_id,
                                                     UINT8 cal_slot)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_recfg_exp_lo_calen(): mux_stage=%d, ghao_id=%d, cal_slot=%d\n", mux_stage, ghao_id, cal_slot);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_recfg_exp_lo_calen(odtu_mux_handle, cal_slot, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_recfg_exp_lo_calen */

/*******************************************************************************
*  coreotn_mux_ghao_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear LCR and BWR context variables for G.HAO
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ghao_id                - G.HAO ramp ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_end(coreotn_handle_t *coreotn_handle,
                                      coreotn_mux_stage_t mux_stage,
                                      UINT8 ghao_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mux_ghao_end(): mux_stage=%d, ghao_id=%d\n", mux_stage, ghao_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_end(odtu_mux_handle, ghao_id);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_end */


/*******************************************************************************
*  coreotn_dmx_ghao_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   odtu_dmx_ghao_update_state:
*   Update the demux ghao context variable
*
*   odtu_dmx_ghao_enable:
*   Enable ghao mecanism which indicate the OHFS_REMOVE to capture rcoh and process it
*   - enable OHFS_REMOVE HO_RCOH_EN
*   - enable OHFS_REMOVE FLEX_RCOH_EN
*   For demux instance, only need to enable ho_rcoh_enable
*
*   odtu_dmx_ghao_isr_enable:
*   Enable ghao related events to be propagated to the interrupt pin
*   - enable RCOH_TRNSFR_SHDW_DNE_I event to interrupt pin
*   - enable HAO_STATUS_AVL_I(96) event to propagate to interrupt pin
*   - enable CONFIG_UPDATE_DNE(96) event to propagate to interrupt pin
*   - enable var context to process procedure based on isr
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   lo_chnl_id          - ghao channel id
*   tpid                - ghao tributary port id
*   trib_slot           - list of tributary slot affected by ghao protocol
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if error
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_init(coreotn_handle_t *coreotn_handle,
                                       coreotn_mux_stage_t mux_stage,
                                       UINT8 lo_chnl_id,
                                       UINT8 tpid,
                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    odtu_dmx_handle_t *odtu_dmx_handle;
    UINT32 ts;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_init(): mux_stage=%d, tpid=%d, lo_chnl_id=%d\n", mux_stage, tpid, lo_chnl_id);

    odtu_dmx_ghao_update_state(odtu_dmx_handle,
                               GHAO_LCR_STATE_IDLE,
                               GHAO_BWR_STATE_IDLE,
                               FALSE,
                               lo_chnl_id,
                               tpid,
                               trib_slot,
                               FALSE,
                               FALSE,
                               FALSE,
                               GHAO_LCR_CTRL_IDLE,
                               FALSE);


    /* In discovery mode, you don't know which TS will come from source,
      so enable all TS to detect new RCOH */
    for (ts = 0; ts < 96; ++ts)
    {
        odtu_dmx_ghao_enable(odtu_dmx_handle, ts, TRUE, FALSE);
    }


/* Skip to be in poll mode
    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_isr_enable(odtu_dmx_handle, TRUE);
    }
*/
    PMC_RETURN(PMC_SUCCESS);
} /* coreotn_dmx_ghao_init */


/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_add_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD NACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an ADD NACK message (rp =1, ctrl=ADD, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_add_nack(coreotn_handle_t *coreotn_handle,
                                                    coreotn_mux_stage_t mux_stage,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_add_nack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_add_nack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_add_nack */


/*******************************************************************************
*  coreotn_dmx_ghao_update_stdby_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configure the stanby configuration RAM as follow:
*   - calls odtu_dmx_ho_chnl_offline_update to write HO config info into stdby pages
*   - adjust memory context for reprovisioning
*   - calls odtu_dmx_lo_offline_deprov to remove old config from the database
*   - calls odtu_dmx_lo_chnl_prov to provision new lo channel configuration,
*     with ghao_reprov=1 which do not reprogram the mp_mgen IPT table
*   - calls sts_sw_cfg to write configuration into standby RAM
*   Trigger odtu_dmx_reconfig_status_update with GHAO mode (swich on knife edge)
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*   add                 - when 1, adding tributary slot, when 0, removing ts
*   old_trib_slot_mask  - current position of the tributary slot within ho container
*                         in a list of 3x32 bit format
*   trib_slots_add_rem  - Tribslots to add or remove in G.HAO
*   msi_data            - Tributary port ID (TPID) used in the added/removed TS
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_update_stdby_config(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      BOOL8 add,
                                                      UINT32 old_trib_slot_mask[3],
                                                      UINT8 trib_slots_add_rem[ODTU_DMX_NUM_CHANL],
                                                      UINT8 msi_data)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_update_stdby_config(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, add=%d, msi_data=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, add, msi_data);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_update_stdby_config(odtu_dmx_handle,
                                                   ho_chnl_id,
                                                   lo_chnl_id,
                                                   trib_slots_add_rem,
                                                   (odtu_dmx_stage_instance_t)mux_stage,
                                                   add,
                                                   old_trib_slot_mask,
                                                   msi_data);
    }

    PMC_RETURN(result);

} /* odtu_dmx_update_stdby_config */



/*******************************************************************************
*  coreotn_dmx_ghao_update_stdby_config_after_knife_edge
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configure the stanby configuration RAM as follow:
*   - calls odtu_dmx_ho_chnl_offline_update to write HO config info into stdby pages
*   - adjust memory context for reprovisioning
*   - calls odtu_dmx_lo_offline_deprov to remove old config from the database
*   - calls odtu_dmx_lo_chnl_prov to provision new lo channel configuration,
*     with ghao_reprov=1 which do not reprogram the mp_mgen IPT table
*   - calls sts_sw_cfg to write configuration into standby RAM
*   Trigger odtu_dmx_reconfig_status_update with GHAO mode (swich on knife edge)
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*   num_ts              - Number of TS after knife edge
*   new_trib_slot_mask  - current position of the tributary slot within ho container
*                         in a list of 3x32 bit format
*   msi_data            - Tributary port ID (TPID) used in the added/removed TS
*   add                 - When 1, adding tributary slots, when 0, removing ts
*   cal_to_add_remove_num  -Number of calendar slots to be added/removed
*   cal_slots_add_rem   - Calendar slots that have been added/removed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_update_stdby_config_after_knife_edge(coreotn_handle_t *coreotn_handle,
                                                                       coreotn_mux_stage_t mux_stage,
                                                                       UINT8 ho_chnl_id,
                                                                       UINT8 lo_chnl_id,
                                                                       UINT32 num_ts,
                                                                       UINT32 new_trib_slot_mask[3],
                                                                       UINT8 msi_data,
                                                                       BOOL8 add,
                                                                       UINT32 cal_to_add_remove_num,
                                                                       UINT32 cal_slots_add_rem[79])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_update_stdby_config_after_knife_edge(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, num_ts=%d, msi_data=%d, add=%d, cal_to_add_remove_num=%d\n", mux_stage, ho_chnl_id, lo_chnl_id, num_ts, msi_data, add, cal_to_add_remove_num);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_update_stdby_config_after_knife_edge(odtu_dmx_handle,
                                                                    (odtu_dmx_stage_instance_t)mux_stage,
                                                                    ho_chnl_id,
                                                                    lo_chnl_id,
                                                                    num_ts,
                                                                    new_trib_slot_mask,
                                                                    msi_data,
                                                                    add,
                                                                    cal_to_add_remove_num,
                                                                    cal_slots_add_rem);
    }

    PMC_RETURN(result);

} /* coreotn_dmx_ghao_update_stdby_config_after_knife_edge */



/*******************************************************************************
*  coreotn_dmx_ghao_wait_config_update_done
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Wait until configuration update is completed.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ho_chnl_id             - HO channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_config_update_done(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 ho_chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_config_update_done(): mux_stage=%d, ho_chnl_id=%d\n", mux_stage, ho_chnl_id);

#ifndef PMC_SW_SIMULATION
    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_config_update_done_poll(odtu_dmx_handle, ho_chnl_id,
                                                  1000);
        if (PMC_SUCCESS != result)
        {
            result = COREOTN_ERR_CHNL_INVALID_STATE;
        }
    }

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_config_update_done_int_clear(odtu_dmx_handle, ho_chnl_id);
    }
#endif

    PMC_RETURN(result);

} /* coreotn_dmx_ghao_wait_config_update_done */



/*******************************************************************************
* coreotn_dmx_ghao_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function that deprovisions the offline page
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 lo_chnl_id,
                                                 UINT32 ho_chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_lo_chnl_deprov(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d\n", mux_stage, ho_chnl_id, lo_chnl_id);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_lo_chnl_deprov(odtu_dmx_handle,
                                              (odtu_dmx_stage_instance_t)mux_stage,
                                              lo_chnl_id,
                                              ho_chnl_id);
    }



    PMC_RETURN(result);

} /* coreotn_dmx_ghao_update_stdby_config */


/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD ACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an ADD ACK message (rp =1, ctrl=ADD, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_add_ack(coreotn_handle_t *coreotn_handle,
                                                   coreotn_mux_stage_t mux_stage,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])

{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_add_ack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_add_ack(odtu_dmx_handle,
                                                ho_chnl_id,
                                                lo_chnl_id,
                                                tpid,
                                                trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_add_ack */


/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an NORM ACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not a NORM ACK message (rp =1, ctrl=NORM, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_norm_ack(coreotn_handle_t *coreotn_handle,
                                                    coreotn_mux_stage_t mux_stage,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_norm_ack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_norm_ack(odtu_dmx_handle,
                                                 ho_chnl_id,
                                                 lo_chnl_id,
                                                 tpid,
                                                 trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_norm_ack */

/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an IDLE NACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an IDLE NACK message (rp =1, ctrl=IDLE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_idle_nack(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_stage,
                                                     UINT8 *ho_chnl_id,
                                                     UINT8 *lo_chnl_id,
                                                     UINT8 *tpid,
                                                     BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])

{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_idle_nack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_idle_nack(odtu_dmx_handle,
                                                  ho_chnl_id,
                                                  lo_chnl_id,
                                                  tpid,
                                                  trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_idle_nack */

/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information, compare with expected TSCC message
*   - update internal context
*   - compare and update TSCC value in context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*   tscc                - expected value of tscc when discovery mode is FALSE
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH does not contain expected TSCC  message
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_tscc(coreotn_handle_t *coreotn_handle,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                UINT8 *tpid,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                BOOL8 tscc)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;
    BOOL8 tscc_tmp;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_tscc(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        tscc_tmp = tscc;
        result = odtu_dmx_ghao_wait_for_tscc(odtu_dmx_handle,
                                             ho_chnl_id,
                                             lo_chnl_id,
                                             tpid,
                                             trib_slot,
                                             &tscc_tmp);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_tscc */

/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare end of RP message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an end of RP message (rp =0)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_rp(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT8 *ho_chnl_id,
                                              UINT8 *lo_chnl_id,
                                              UINT8 *tpid,
                                              BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_rp(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_rp(odtu_dmx_handle,
                                           ho_chnl_id,
                                           lo_chnl_id,
                                           tpid,
                                           trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_rp */


/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_remove_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE NACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE NACK message (rp =1, ctrl=REMOVE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_remove_nack(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_remove_nack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_remove_nack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_remove_nack */

/*******************************************************************************
*  coreotn_dmx_ghao_wait_for_remove_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls coreotn_dmx_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE ACK message
*   - update internal context
*
* INPUTS:
*   coreotn_handle      - handle to the COREOTN instance to be operated on
*   mux_stage           - ODU DEMUX stage instance in COREOTN
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE ACK message (rp =1, ctrl=REMOVE, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_remove_ack(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      UINT8 *tpid,
                                                      BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_dmx_handle_t *odtu_dmx_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_dmx_handle = coreotn_handle->odtu_dmx2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_dmx_ghao_wait_for_remove_ack(): mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tpid=%d\n", mux_stage, *ho_chnl_id, *lo_chnl_id, *tpid);

    if (result == PMC_SUCCESS)
    {
        result = odtu_dmx_ghao_wait_for_remove_ack(odtu_dmx_handle,
                                                   ho_chnl_id,
                                                   lo_chnl_id,
                                                   tpid,
                                                   trib_slot);
    }

    PMC_RETURN(result);
} /* coreotn_dmx_ghao_wait_for_remove_ack */


/*******************************************************************************
*  coreotn_mpma_ghao_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots.
*
* INPUTS:
*   coreotn_handle    - handle to the COREOTN instance to be operated on
*   mpma_inst         - MPMA instance in COREOTN
*   chnl              - channel to resize
*   oduflex_num_tribslots  - final number of ODUflex tribslots
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpma_ghao_fifo_resize(coreotn_handle_t *coreotn_handle,
                                               coreotn_mpma_inst_t mpma_inst,
                                               UINT32 chnl,
                                               UINT32 oduflex_num_tribslots)
{
    PMC_ERROR result = PMC_SUCCESS;
    mpma_handle_t *mpma_handle;

    PMC_ENTRY();

    if (mpma_inst == COREOTN_MPMA_TO_COREOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo1_handle;
    }
    else if (mpma_inst == COREOTN_MPMA_FROM_MAPOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpma_ghao_fifo_resize(): mpma_inst=%d, chnl=%d, oduflex_num_tribslots=%d\n", mpma_inst, chnl, oduflex_num_tribslots);

    if (result == PMC_SUCCESS)
    {
        result = mpma_ghao_fifo_resize(mpma_handle,
                                       chnl,
                                       oduflex_num_tribslots);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpma_ghao_fifo_resize */


/*******************************************************************************
* coreotn_mpma_ghao_scbs3_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the scbs3 calendar for an ODUFLEX channel to the new number of
*   calendar slots for G.HAO ODUflex resizing.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   mpma_inst           - MPMA instance in COREOTN
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
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
PUBLIC PMC_ERROR coreotn_mpma_ghao_scbs3_reprov(coreotn_handle_t *coreotn_handle,
                                                coreotn_mpma_inst_t mpma_inst,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate)
{
    DOUBLE rate;
    PMC_ERROR result = PMC_SUCCESS;
    mpma_handle_t *mpma_handle;

    PMC_ENTRY();

    if (mpma_inst == COREOTN_MPMA_TO_COREOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo1_handle;
    }
    else if (mpma_inst == COREOTN_MPMA_FROM_MAPOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpma_ghao_scbs3_reprov(): mpma_inst=%d, chnl=%d, oduflex_gfp_rate=%d\n", mpma_inst, chnl, oduflex_gfp_rate);

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
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (result == PMC_SUCCESS)
    {
        result = mpma_schd_ch_reprov(mpma_handle, chnl, rate);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpma_ghao_scbs3_reprov */


/*******************************************************************************
* coreotn_ghao_mpma_dsp_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Used to enable or disable the MPMA dsp during G.HAO
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mpma_inst           - The MPMA instance to operate on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   enable              - TRUE : enable the DSP
*                         FALSE : disable the DSP
*   target_ts           - Number of TS when enabled
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
PUBLIC PMC_ERROR coreotn_ghao_mpma_dsp_enable_set(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mpma_inst_t mpma_inst,
                                                  UINT32 chnl,
                                                  BOOL8 enable,
                                                  UINT32 target_ts)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    mpma_handle_t *mpma_handle;


    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT( ((mpma_inst == COREOTN_MPMA_TO_COREOTN) || (mpma_inst == COREOTN_MPMA_FROM_MAPOTN)), COREOTN_ERR_INVALID_ARG, 0, 0);


    if (mpma_inst == COREOTN_MPMA_TO_COREOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo1_handle;
    }
    else if (mpma_inst == COREOTN_MPMA_FROM_MAPOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo2_handle;
    }

    PMC_LOG_TRACE("Call coreotn_ghao_mpma_dsp_enable_set(): mpma_inst=%d, chnl_id=%d, enable=%d\n", mpma_inst, chnl, enable);



    if (pmc_is_digi_rev_a_revision(&coreotn_handle->base) == TRUE)
    {
        /*To work-around MPMA DSP bug, we have to:
          scheduling_pacer_en = 0 (if flex >= 58)
          scheduling_pacer_en = 1 (if felc < 58)*/
        if(mpma_inst == COREOTN_MPMA_TO_COREOTN
            && enable == TRUE)
        {
            if(target_ts >= 58)
            {
                mpma_schd_pacer_enable_cfg(mpma_handle, 0);
            }
            else
            {
                mpma_schd_pacer_enable_cfg(mpma_handle, 1);
            }
        }
    }


    ret_val = mpma_dsp_ch_enable_cfg(mpma_handle,
                                     chnl,
                                     enable);


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpma_dsp_enable_set */


/*******************************************************************************
* coreotn_ghao_mpma_bwr_ind_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Used to trigger BWR_IND in the MPMA of FO2 for MAP<->SIFD paths
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   bwr_ind             - 0 : broadcast BWR_IND = 0 on DPI type interface
*                         1 : broadcast BWR_IND = 1 on DPI type interface
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
PUBLIC PMC_ERROR coreotn_ghao_mpma_bwr_ind_set(coreotn_handle_t *coreotn_handle,                                                                                     UINT32 chnl,
                                               UINT32 bwr_ind)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);

    PMC_LOG_TRACE("Call coreotn_ghao_mpma_bwr_ind_set(): chnl_id=%d, bwr_ind=%d\n", chnl, bwr_ind);

    ret_val = mpma_ghao_bwr_ind_set(coreotn_handle->mpma_fo2_handle,
                                    chnl,
                                    bwr_ind);


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpma_bwr_ind_set */





/*******************************************************************************
* coreotn_ghao_mpma_bwr_end_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Set new PKT_SIZE, PKT_PERIOD_INT, PKT_PERIOD_N, PKT_PERIOD_D, MPMA_GAIN.
*  This can be called at the end of the G.HAO ramp, but must be called before
*  turning the DSP back on.
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mpma_inst           - The MPMA instance to operate on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   odu_bitrate         - client bit rate in bits per second
*   oif_pkt_size        - packet size of the OIF packets
*   oif_header_size     - header size of the OIF packets
*   use_min_eps         - TRUE : use minimum epsilon calculator
*                                (only for packets that will never leave the
*                                 device)
*                         FALSE : use OIF calculator (required for packets
*                                 that might leave device)
*
* OUTPUTS:
*   pkt_size_ptr    - pkt_size returned by OIF calculator
*   pkt_per_int_ptr - pkt_per_int returned by OIF calculator
*   pkt_per_n_ptr   - pkt_per_n returned by OIF calculator
*   pkt_per_d_ptr   - pkt_per_d returned by OIF calculator
*   eps_ptr         - eps returned by OIF calculator
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_ghao_mpma_bwr_end_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpma_inst_t mpma_inst,
                                                     UINT32 chnl,
                                                     DOUBLE odu_bitrate,
                                                     UINT32 oif_pkt_size,
                                                     UINT32 oif_header_size,
                                                     BOOL   use_min_eps,
                                                     UINT32 *pkt_size_ptr,
                                                     UINT32 *pkt_per_int_ptr,
                                                     UINT32 *pkt_per_n_ptr,
                                                     UINT32 *pkt_per_d_ptr,
                                                     DOUBLE *eps_ptr)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    mpma_handle_t *mpma_handle;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    if (mpma_inst == COREOTN_MPMA_TO_COREOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo1_handle;
    }
    else if (mpma_inst == COREOTN_MPMA_FROM_MAPOTN)
    {
        mpma_handle = coreotn_handle->mpma_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_ghao_mpma_bwr_end_param_set(): mpma_inst=%d, chnl_id=%d, odu_bitrate=%g, oif_pkt_size=%d, oif_header_size=%d\n", mpma_inst, chnl, odu_bitrate, oif_pkt_size, oif_header_size);



    if(use_min_eps)
    {
        /* some paths, packets will never leave the chip. No need to use OIF calculator */

        ret_val = coreotn_min_eps_pkt_period_calc(coreotn_handle,
                                                  odu_bitrate,
                                                  pkt_per_int_ptr,
                                                  pkt_per_n_ptr,
                                                  pkt_per_d_ptr,
                                                  pkt_size_ptr,
                                                  eps_ptr);
    }
    else
    {
        /* In paths where the packets can leave the chip, OIF calculator must be used */

        ret_val = coreotn_oif_pkt_period_calc(coreotn_handle,
                                              odu_bitrate,
                                              oif_pkt_size,
                                              oif_header_size,
                                              pkt_size_ptr,
                                              pkt_per_int_ptr,
                                              pkt_per_n_ptr,
                                              pkt_per_d_ptr,
                                              eps_ptr);

    }


    if(ret_val == PMC_SUCCESS)
    {
          /* set new parameters in MPMA*/
          ret_val = mpma_ghao_bwr_end_param_set(mpma_handle,
                                                chnl,
                                                odu_bitrate,
                                                UTIL_OPSA_T_FRAME_UNUSED,
                                                *pkt_size_ptr,
                                                *pkt_per_int_ptr,
                                                *pkt_per_n_ptr,
                                                *pkt_per_d_ptr,
                                                *eps_ptr);
    }


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpma_bwr_end_param_set */




/*******************************************************************************
*  coreotn_mpmo_ghao_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots. Update the XOFF
*   threshold accordingly.
*
* INPUTS:
*   coreotn_handle    - handle to the COREOTN instance to be operated on
*   mpmo_inst         - MPMO instance in COREOTN
*   chnl              - channel to resize
*   oduflex_num_tribslots  - final number of ODUflex tribslots
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_ghao_fifo_resize(coreotn_handle_t *coreotn_handle,
                                               coreotn_mpmo_inst_t mpmo_inst,
                                               UINT32 chnl,
                                               UINT32 oduflex_num_tribslots)
{
    PMC_ERROR result = PMC_SUCCESS;
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    if (mpmo_inst == COREOTN_MPMO_TO_COREOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo1_handle;
    }
    else if (mpmo_inst == COREOTN_MPMO_TO_MAPOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpmo_ghao_fifo_resize(): mpmo_inst=%d, chnl_id=%d, oduflex_num_tribslots=%d\n", mpmo_inst, chnl, oduflex_num_tribslots);

    if (result == PMC_SUCCESS)
    {
        result = mpmo_ghao_fifo_resize(mpmo_handle,
                                       chnl,
                                       oduflex_num_tribslots);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpmo_ghao_fifo_resize */


/*******************************************************************************
*  coreotn_mpmo_ghao_fifo_resize_no_thresh_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo to the specified number of tribslots. The XOFF
*   threshold is not modified. The threshold is expected to be updated in
*   BWR phase using mpmo_ghao_threshold_update() and the information given by
*   mpmo_ghao_threshold_update_setup().
*
* INPUTS:
*   coreotn_handle    - handle to the COREOTN instance to be operated on
*   mpmo_inst         - MPMO instance in COREOTN
*   chnl              - channel to resize
*   oduflex_num_tribslots  - final number of ODUflex tribslots
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_ghao_fifo_resize_no_thresh_update(coreotn_handle_t *coreotn_handle,
                                                                coreotn_mpmo_inst_t mpmo_inst,
                                                                UINT32 chnl,
                                                                UINT32 oduflex_num_tribslots)
{
    PMC_ERROR result = PMC_SUCCESS;
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    if (mpmo_inst == COREOTN_MPMO_TO_COREOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo1_handle;
    }
    else if (mpmo_inst == COREOTN_MPMO_TO_MAPOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpmo_ghao_fifo_resize_no_thresh_update(): mpmo_inst=%d, chnl_id=%d, oduflex_num_tribslots=%d\n", mpmo_inst, chnl, oduflex_num_tribslots);

    if (result == PMC_SUCCESS)
    {
        result = mpmo_ghao_fifo_resize_no_thresh_update(mpmo_handle,
                                                        chnl,
                                                        oduflex_num_tribslots);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpmo_ghao_fifo_resize_no_thresh_update */


/*******************************************************************************
* coreotn_mpmo_ghao_scbs3_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the scbs3 calendar for an ODUFLEX channel to the new number of
*   calendar slots for G.HAO ODUflex resizing.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   mpmo_inst           - MPMO instance in COREOTN
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
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
PUBLIC PMC_ERROR coreotn_mpmo_ghao_scbs3_reprov(coreotn_handle_t *coreotn_handle,
                                                coreotn_mpmo_inst_t mpmo_inst,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate)
{
    DOUBLE rate;
    PMC_ERROR result = PMC_SUCCESS;
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    if (mpmo_inst == COREOTN_MPMO_TO_COREOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo1_handle;
    }
    else if (mpmo_inst == COREOTN_MPMO_TO_MAPOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpmo_ghao_scbs3_reprov(): mpmo_inst=%d, chnl_id=%d, oduflex_gfp_rate=%d\n", mpmo_inst, chnl, oduflex_gfp_rate);

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
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (result == PMC_SUCCESS)
    {
        result = mpmo_schd_ch_reprov(mpmo_handle, chnl, rate);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpmo_ghao_scbs3_reprov */


/*******************************************************************************
* coreotn_mpmo_ghao_scbs3_reprov_force_userbits
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the scbs3 calendar for an ODUFLEX channel to the new number of
*   calendar slots for G.HAO ODUflex resizing.
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   mpmo_inst           - MPMO instance in COREOTN
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*   set_mask            - Mask to identify the bits that can be changed
*   userbit_mask        - The desired userbit_mask (indexed by calendar
*                         entry.  userbit_mask[0][0th bit] = cal entry 0.
*                         userbit_mask[0][31st bit] = cal entry 95.
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
PUBLIC PMC_ERROR coreotn_mpmo_ghao_scbs3_reprov_force_userbits(coreotn_handle_t *coreotn_handle,
                                                               coreotn_mpmo_inst_t mpmo_inst,
                                                               UINT32 chnl,
                                                               UINT32 oduflex_gfp_rate,
                                                               UINT32 set_mask[3],
                                                               UINT32 userbit_mask[3])
{
    DOUBLE rate;
    PMC_ERROR result = PMC_SUCCESS;
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    if (mpmo_inst == COREOTN_MPMO_TO_COREOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo1_handle;
    }
    else if (mpmo_inst == COREOTN_MPMO_TO_MAPOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_mpmo_ghao_scbs3_reprov_force_userbits(): mpmo_inst=%d, chnl_id=%d, oduflex_gfp_rate=%d, userbit_mask= %08X %08X %08X\n", mpmo_inst, chnl, oduflex_gfp_rate, userbit_mask[2], userbit_mask[1], userbit_mask[0]);

    if (result == PMC_SUCCESS)
    {
        result = coreotn_mpmo_set_custom_userbit_mask(coreotn_handle,
                                                      chnl,
                                                      mpmo_inst,
                                                      TRUE,
                                                      set_mask,
                                                      userbit_mask,
                                                      FALSE);
    }

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
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    if (result == PMC_SUCCESS)
    {
        result = mpmo_schd_ch_reprov(mpmo_handle, chnl, rate);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* coreotn_mpmo_ghao_scbs3_reprov_force_userbits */


/*******************************************************************************
* coreotn_mpmo_bwr_ind_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
* INPUTS:
*   *coreotn_handle        - coreotn handle instance
*   mpmo_instance          - See coreotn_mpmo_inst_t
*
* OUTPUTS:
*   *bwr_ind               - BWR_IND read from the MPMO
*
* RETURNS:
*   PMC_ERROR             - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_bwr_ind_get(coreotn_handle_t *coreotn_handle,
                                          coreotn_mpmo_inst_t mpmo_instance,
                                          UINT32 bwr_ind[3])
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();


    switch (mpmo_instance)
    {
        case COREOTN_MPMO_TO_COREOTN:

            ret_val = mpmo_bwr_ind_get(coreotn_handle->mpmo_fo1_handle,
                                       bwr_ind);

            break;
        case COREOTN_MPMO_TO_MAPOTN:

            ret_val = mpmo_bwr_ind_get(coreotn_handle->mpmo_fo2_handle,
                                       bwr_ind);

            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }




    PMC_RETURN(ret_val);

} /* coreotn_mpmo_bwr_ind_get */




/*******************************************************************************
* coreotn_mpmo_xoff_thresh_change
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs per word adjustments on the xoff threshold in the MPMO
*
* INPUTS:
*   *coreotn_handle        - coreotn handle instance
*   mpmo_instance          - See coreotn_mpmo_inst_t
*   chnl_id                - Channel id to perform the adjustment for
*   increase               - TRUE : performing a increase
*                            FALSE : performing a decrease
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR             - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_xoff_thresh_change(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mpmo_inst_t mpmo_instance,
                                                 UINT32 chnl_id,
                                                 BOOL8 increase)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_LOG_TRACE("Call coreotn_mpmo_xoff_thresh_change(): mpmo_instance=%d, chnl_id=%d, increase=%d\n", mpmo_instance, chnl_id, increase);

    switch (mpmo_instance)
    {
        case COREOTN_MPMO_TO_COREOTN:

            ret_val = mpmo_xoff_thresh_change(coreotn_handle->mpmo_fo1_handle,
                                              chnl_id,
                                              increase);

            break;
        case COREOTN_MPMO_TO_MAPOTN:


            ret_val = mpmo_xoff_thresh_change(coreotn_handle->mpmo_fo2_handle,
                                              chnl_id,
                                              increase);

            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }




    PMC_RETURN(ret_val);

} /* coreotn_mpmo_xoff_thresh_change */


/*******************************************************************************
* coreotn_mpmo_set_custom_userbit_mask
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Used for defining a custom userbit configuration in the SCBS3.
*     For G.HAO LCR : call with config_update = FALSE before
*                     you reprov the MPMO
*     For G.HAO BWR : call with config_update = TRUE once ever
*                     TS increase (every 2.5s)
*
* INPUTS:
*   *coreotn_handle   - coreotn handle instance
*   chnl_id           - Channel ID to apply the custom userbit mask to
*   mpmo_instance     - See coreotn_mpmo_inst_t
*   enable            - TRUE : use the userbit_mask to set the userbits
*                              in the MPMO
*   set_mask          - Mask to identify the bits that can be changed
*   userbit_mask      - The desired userbit_mask (indexed by calendar
*                       entry.  userbit_mask[0][0th bit] = cal entry 0.
*                       userbit_mask[0][31st bit] = cal entry 95.
*   config_update     - TRUE : will update the standby and active
*                              page with the userbits
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR             - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_set_custom_userbit_mask(coreotn_handle_t *coreotn_handle,
                                                      UINT32 chnl_id,
                                                      coreotn_mpmo_inst_t mpmo_instance,
                                                      BOOL enable,
                                                      UINT32 set_mask[3],
                                                      UINT32 userbit_mask[3],
                                                      BOOL8 config_update)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();


    switch (mpmo_instance)
    {
        case COREOTN_MPMO_TO_COREOTN:


            ret_val = mpmo_set_custom_userbit_mask(coreotn_handle->mpmo_fo1_handle,
                                                   chnl_id,
                                                   enable,
                                                   set_mask,
                                                   userbit_mask,
                                                   config_update);


            break;
        case COREOTN_MPMO_TO_MAPOTN:

            ret_val = mpmo_set_custom_userbit_mask(coreotn_handle->mpmo_fo2_handle,
                                                   chnl_id,
                                                   enable,
                                                   set_mask,
                                                   userbit_mask,
                                                   config_update);


            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }




    PMC_RETURN(ret_val);

} /* coreotn_mpmo_set_custom_userbit_mask */


/*******************************************************************************
* coreotn_mpmo_update_userbit_mask
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Used for updating the userbit configuration in the SCBS3. The user bit for
*   the specified calendar slot is inverted in the MPMO SCBS3.
*
* INPUTS:
*   *coreotn_handle   - coreotn handle instance
*   mpmo_instance     - See coreotn_mpmo_inst_t
*   chnl_id           - Channel ID to apply the custom userbit mask to
*   cal_slot_to_add_remove - Calendar slot to add or remove in current userbits
*                            (will add reove num_to_add_remove)
*   num_to_add_remove - number of calendar enables to add/remove
*   cal_slot_mask     - Current mask to identify the bits that can be changed
*
* OUTPUTS:
*   userbit_mask      - The obtained userbit_mask once updated in SCBS3.
*
* RETURNS:
*   PMC_ERROR         - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_update_userbit_mask(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mpmo_inst_t mpmo_instance,
                                                  UINT32 chnl_id,
                                                  UINT32 cal_slot_to_add_remove[96],
                                                  UINT32 num_to_add_remove,
                                                  UINT32 cal_slot_mask[3],
                                                  UINT32 userbit_mask[3])
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 idx, i;

    PMC_ENTRY();

    /* Calculate new userbit mask */
    for(i=0;i<num_to_add_remove;i++)
    {
        PMC_LOG_TRACE("Call coreotn_mpmo_update_userbit_mask(): mpmo_instance=%d, chnl_id=%d, cal_slot_to_add_remove[%d]=%d\n", mpmo_instance, chnl_id, i, cal_slot_to_add_remove[i]);

        idx = cal_slot_to_add_remove[i]/32;
        userbit_mask[idx] = userbit_mask[idx] ^ (1 << (cal_slot_to_add_remove[i] % 32));
    }

    PMC_LOG_TRACE("coreotn_mpmo_update_userbit_mask(): cal_slot_mask= %08X %08X %08X\n", cal_slot_mask[2], cal_slot_mask[1], cal_slot_mask[0]);
    PMC_LOG_TRACE("coreotn_mpmo_update_userbit_mask(): userbit_mask=  %08X %08X %08X\n", userbit_mask[2], userbit_mask[1], userbit_mask[0]);

    /* Update userbit mask */
    ret_val = coreotn_mpmo_set_custom_userbit_mask(coreotn_handle,
                                                   chnl_id,
                                                   mpmo_instance,
                                                   TRUE,
                                                   cal_slot_mask,
                                                   userbit_mask,
                                                   TRUE);

    PMC_RETURN(ret_val);

} /* coreotn_mpmo_update_userbit_mask */


/*******************************************************************************
* coreotn_mpmo_adjust_age_gain
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Used to adjust the AGE_PROP_GAIN and AGE_INT_GAIN in the MPMO DSP.
*   These adjustment need to be made every TS adjustment.
*
* INPUTS:
*   *coreotn_handle   - coreotn handle instance
*   mpmo_instance     - See coreotn_mpmo_inst_t
*   chnl_id           - Channel id to make the adjustment for
*   num_cal_entries   - The new number of calendar entries
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR             - PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mpmo_adjust_age_gain(coreotn_handle_t *coreotn_handle,
                                              coreotn_mpmo_inst_t mpmo_instance,
                                              UINT32 chnl_id,
                                              UINT32 num_cal_entries)

{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_LOG_TRACE("Call coreotn_mpmo_adjust_age_gain(): mpmo_instance=%d, chnl_id=%d, num_cal_entries=%d\n", mpmo_instance, chnl_id, num_cal_entries);

    switch (mpmo_instance)
    {
        case COREOTN_MPMO_TO_COREOTN:


            ret_val = mpmo_ghao_age_gain_set(coreotn_handle->mpmo_fo1_handle,
                                             chnl_id,
                                             num_cal_entries);

            break;
        case COREOTN_MPMO_TO_MAPOTN:

            ret_val = mpmo_ghao_age_gain_set(coreotn_handle->mpmo_fo2_handle,
                                             chnl_id,
                                             num_cal_entries);

            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }




    PMC_RETURN(ret_val);

} /* coreotn_mpmo_adjust_age_gain */



/*******************************************************************************
* coreotn_ghao_mpmo_bwr_rate_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is call for the MPMO in FO1/FO2 when it is in the G.HAO path.
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mpmo_instance       - MPMO instance type (see coreotn_mpmo_inst_t)
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - The target number of tribslots at the end of the ramp
*   increase            - TRUE : ramp increase
*                         FALSE : ramp decrease
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
PUBLIC PMC_ERROR coreotn_ghao_mpmo_bwr_rate_param_set(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mpmo_inst_t mpmo_instance,
                                                      UINT32 chnl,
                                                      UINT32 oduflex_gfp_rate,
                                                      BOOL8 increase)
{
    /* Variable declaration */
    PMC_ERROR ret_val;
    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);

    switch (mpmo_instance)
    {
        case COREOTN_MPMO_TO_COREOTN:
            mpmo_handle = coreotn_handle->mpmo_fo1_handle;
            break;
        case COREOTN_MPMO_TO_MAPOTN:
            mpmo_handle = coreotn_handle->mpmo_fo2_handle;
            break;
        default:
            PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Call coreotn_ghao_mpmo_bwr_rate_param_set(): mpmo_instance=%d, chnl_id=%d, oduflex_gfp_rate=%d, increase=%d\n", mpmo_instance, chnl, oduflex_gfp_rate, increase);

    ret_val = mpmo_ghao_bwr_setup_param_set(mpmo_handle,
                                            chnl,
                                            oduflex_gfp_rate,
                                            increase);

    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpmo_bwr_rate_param_set */



/*******************************************************************************
* coreotn_ghao_mpmo_bwr_end_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Called for MPMO at end of ramp.  Will:
*    - set new PKT_SIZE
*    - set new CN_BASE, CN_BASE_INT, CN_BASE_D
*    - set RAMP_EN to 0
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mpmo_inst           - The MPMO instance to operate on
*                         (see coreotn_mpmo_inst_t)
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   odu_bitrate         - client bit rate in bits per second
*   pkt_size            - pkt_size returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_int         - pkt_per_int returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_n           - pkt_per_n returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_d           - pkt_per_d returned by the OIF calculator
*                         for the sourcing MPMA
*   eps_ptr             - eps_ptr returned by the OIF calculator
*                         for the sourcing MPMA
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
PUBLIC PMC_ERROR coreotn_ghao_mpmo_bwr_end_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpmo_inst_t mpmo_inst,
                                                     UINT32 chnl,
                                                     DOUBLE odu_bitrate,
                                                     UINT32 pkt_size,
                                                     UINT32 pkt_per_int,
                                                     UINT32 pkt_per_n,
                                                     UINT32 pkt_per_d,
                                                     DOUBLE eps_ptr)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    mpmo_handle_t *mpmo_handle;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);

    if (mpmo_inst == COREOTN_MPMO_TO_COREOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo1_handle;
    }
    else if (mpmo_inst == COREOTN_MPMO_TO_MAPOTN)
    {
        mpmo_handle = coreotn_handle->mpmo_fo2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Call coreotn_ghao_mpmo_bwr_end_param_set(): mpmo_inst=%d, chnl_id=%d, odu_bitrate=%g, pkt_size=%d \n", mpmo_inst, chnl, odu_bitrate, pkt_size);


    if(ret_val == PMC_SUCCESS)
    {

        ret_val = mpmo_ghao_bwr_end_param_set(mpmo_handle,
                                              chnl,
                                              odu_bitrate,
                                              pkt_size,
                                              pkt_per_int,
                                              pkt_per_n,
                                              pkt_per_d);
    }


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpmo_bwr_end_param_set */






/*******************************************************************************
* coreotn_ghao_mpmo_calculate_new_cn_max_cn_d
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Wrapper function for calculating new Cn base for the MPMO
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   client_bitrate      - client bit rate in bits per second
*   pkt_size            - pkt_size returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_int         - pkt_per_int returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_n           - pkt_per_n returned by the OIF calculator
*                         for the sourcing MPMA
*   pkt_per_d           - pkt_per_d returned by the OIF calculator
*                         for the sourcing MPMA
*
* OUTPUTS:
*   cn_base_int            - Integer of Cn base
*   cn_base_n              - Decimal (numerator) of cn base
*   cn_base_d              - Decimal (denomenator) of cn base
*   program_cn_n_first     - TRUE : program new numerator before
*                                   new denominator
*                            FALSE : program new denominator
*                                    before new numerator
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(coreotn_handle_t *coreotn_handle,
                                                             UINT32 chnl,
                                                             DOUBLE client_bitrate,
                                                             UINT32 pkt_size,
                                                             UINT32 pkt_per_int,
                                                             UINT32 pkt_per_n,
                                                             UINT32 pkt_per_d,
                                                             UINT32 *cn_base_int,
                                                             UINT32 *cn_base_n,
                                                             UINT32 *cn_base_d,
                                                             BOOL *program_cn_n_first)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(): chnl_id=%d, odu_bitrate=%g, pkt_size=%d \n", chnl, client_bitrate, pkt_size);



    if(ret_val == PMC_SUCCESS)
    {

        ret_val = mpmo_ghao_calculate_new_cn_max_cn_d(coreotn_handle->mpmo_fo1_handle,
                                                      chnl,
                                                      client_bitrate,
                                                      pkt_size,
                                                      pkt_per_int,
                                                      pkt_per_n,
                                                      pkt_per_d,
                                                      cn_base_int,
                                                      cn_base_n,
                                                      cn_base_d,
                                                      program_cn_n_first);
    }


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpmo_calculate_new_cn_max_cn_d */




/*******************************************************************************
* coreotn_ghao_mpmo_new_cn_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Wrapper function for calculating new Cn base for the MPMO
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   cn_base_int            - Integer of Cn base
*   cn_base_n              - Decimal (numerator) of cn base
*   cn_base_d              - Decimal (denomenator) of cn base
*   program_cn_n_first     - TRUE : program new numerator before
*                                   new denominator
*                            FALSE : program new denominator
*                                    before new numerator
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
PUBLIC PMC_ERROR coreotn_ghao_mpmo_new_cn_set(coreotn_handle_t *coreotn_handle,
                                              UINT32 chnl,
                                              UINT32 cn_base_int,
                                              UINT32 cn_base_n,
                                              UINT32 cn_base_d,
                                              BOOL program_cn_n_first)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_ghao_mpmo_new_cn_set(): chnl_id=%d, cn_base_int=%d, cn_base_n=%d, cn_base_d=%d \n", chnl, cn_base_int, cn_base_n, cn_base_d);



    if(ret_val == PMC_SUCCESS)
    {

        ret_val = mpmo_ghao_new_cn_set(coreotn_handle->mpmo_fo1_handle,
                                       chnl,
                                       program_cn_n_first,
                                       cn_base_int,
                                       cn_base_n,
                                       cn_base_d);
    }


    PMC_RETURN(ret_val);

} /* coreotn_ghao_mpmo_new_cn_set */






/*******************************************************************************
* coreotn_oduksc_scbs3_cgen_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision in ODUKSC the specified ODUFLEX channel for a specific rate in
*   G.HAO. No ramp is configured. The scbs3 is configured with the number of
*   calendar slots corresponding to the new rate. The credit generator is
*   configured so that the rate doesn't change despite the new number of
*   calendar slots. This is done for each ODUKSC G.HAO instance.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) coreotn_oduksc_scbs3_cgen_reprov
*   2) coreotn_oduksc_ramp_prov
*   3) coreotn_oduksc_ramp_start
*   4) coreotn_oduksc_ramp_done_isr
*   Ramp-down:
*   1) coreotn_oduksc_ramp_prov
*   2) coreotn_oduksc_ramp_start
*   3) coreotn_oduksc_ramp_done_isr
*   4) coreotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mux_stage           - MUX stage instance in COREOTN
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
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
PUBLIC PMC_ERROR coreotn_oduksc_scbs3_cgen_reprov(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mux_stage_t mux_stage,
                                                  UINT32 chnl,
                                                  UINT32 oduflex_gfp_rate)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_oduksc_scbs3_cgen_reprov(): mux_stage=%d, chnl_id=%d, oduflex_gfp_rate=%d\n", mux_stage, chnl, oduflex_gfp_rate);

    if(mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF1 */
            ret_val = oduksc_chnl_scbs3_cgen_reprov(coreotn_handle->oduksc1_handle,
                                                    chnl,
                                                    UTIL_GLOBAL_ODUFLEX_GFP,
                                                    oduflex_gfp_rate);
        }
    }
    else if(mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF2 */
            ret_val = oduksc_chnl_scbs3_cgen_reprov(coreotn_handle->oduksc2_handle,
                                                    chnl,
                                                    UTIL_GLOBAL_ODUFLEX_GFP,
                                                    oduflex_gfp_rate);
        }
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_RETURN(ret_val);

} /* coreotn_oduksc_scbs3_cgen_reprov */


/*******************************************************************************
* coreotn_oduksc_ramp_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision the ODUKSC ramp configuration for the specified ODUFLEX channel in
*   G.HAO. The ramp is not started yet. The scbs3 and the current rate are not
*   modified. The credit generator and ramp modulo counter are configured so
*   that the ramp is ready to be started. This is done for each ODUKSC G.HAO
*   instance.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) coreotn_oduksc_scbs3_cgen_reprov
*   2) coreotn_oduksc_ramp_prov
*   3) coreotn_oduksc_ramp_start
*   4) coreotn_oduksc_ramp_done_isr
*   Ramp-down:
*   1) coreotn_oduksc_ramp_prov
*   2) coreotn_oduksc_ramp_start
*   3) coreotn_oduksc_ramp_done_isr
*   4) coreotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mux_stage           - MUX stage instance in COREOTN
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
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
PUBLIC PMC_ERROR coreotn_oduksc_ramp_prov(coreotn_handle_t *coreotn_handle,
                                          coreotn_mux_stage_t mux_stage,
                                          UINT32 chnl,
                                          UINT32 oduflex_gfp_rate)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_oduksc_ramp_prov(): mux_stage=%d, chnl_id=%d, oduflex_gfp_rate=%d\n", mux_stage, chnl, oduflex_gfp_rate);

    if(mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF1 */
            ret_val = oduksc_chnl_cgen_ramp_prov(coreotn_handle->oduksc1_handle,
                                                 chnl,
                                                 UTIL_GLOBAL_ODUFLEX_GFP,
                                                 oduflex_gfp_rate,
                                                 FALSE);
        }
    }
    else if(mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF2 */
            ret_val = oduksc_chnl_cgen_ramp_prov(coreotn_handle->oduksc2_handle,
                                                 chnl,
                                                 UTIL_GLOBAL_ODUFLEX_GFP,
                                                 oduflex_gfp_rate,
                                                 FALSE);
        }
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_RETURN(ret_val);

} /* coreotn_oduksc_ramp_prov */


/*******************************************************************************
* coreotn_oduksc_ramp_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Start the ODUKSC ramp for the resizing of an ODUFLEX channel in G.HAO. The
*   ramp modulo counter is started and the credit generator will start using it.
*   When the ramp-up or ramp-down will be completed, the ramp done interrupt
*   will be generated. This is done for each ODUKSC G.HAO instance.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) coreotn_oduksc_scbs3_cgen_reprov
*   2) coreotn_oduksc_ramp_prov
*   3) coreotn_oduksc_ramp_start
*   4) coreotn_oduksc_ramp_done_isr
*   Ramp-down:
*   1) coreotn_oduksc_ramp_prov
*   2) coreotn_oduksc_ramp_start
*   3) coreotn_oduksc_ramp_done_isr
*   4) coreotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mux_stage           - MUX stage instance in COREOTN
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR coreotn_oduksc_ramp_start(coreotn_handle_t *coreotn_handle,
                                           coreotn_mux_stage_t mux_stage,
                                           UINT32 chnl)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_oduksc_ramp_start(): mux_stage=%d, chnl_id=%d\n", mux_stage, chnl);

    if(mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        if(ret_val == PMC_SUCCESS)
        {
             /* ODUKSC in FMF1 */
            ret_val = oduksc_chnl_cgen_ramp_start(coreotn_handle->oduksc1_handle,
                                                  chnl,
                                                  UTIL_GLOBAL_ODUFLEX_GFP);
        }
    }
    else if(mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF2 */
            ret_val = oduksc_chnl_cgen_ramp_start(coreotn_handle->oduksc2_handle,
                                                  chnl,
                                                  UTIL_GLOBAL_ODUFLEX_GFP);
        }
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_RETURN(ret_val);

} /* coreotn_oduksc_ramp_start */


/*******************************************************************************
* coreotn_oduksc_ramp_done_isr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Call the ramp done interrupt routine of each ODUKSC G.HAO instance.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) coreotn_oduksc_scbs3_cgen_reprov
*   2) coreotn_oduksc_ramp_prov
*   3) coreotn_oduksc_ramp_start
*   4) coreotn_oduksc_ramp_done_isr
*   Ramp-down:
*   1) coreotn_oduksc_ramp_prov
*   2) coreotn_oduksc_ramp_start
*   3) coreotn_oduksc_ramp_done_isr
*   4) coreotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mux_stage           - MUX stage instance in COREOTN
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR coreotn_oduksc_ramp_done_isr(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT32 chnl)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    PMC_LOG_TRACE("Call coreotn_oduksc_ramp_done_isr(): mux_stage=%d, chnl_id=%d\n", mux_stage, chnl);

    if(mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF1 */
            ret_val = oduksc_ramp_done_isr(coreotn_handle->oduksc1_handle,
                                               chnl);
        }
    }
    else if(mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF2 */
            ret_val = oduksc_ramp_done_isr(coreotn_handle->oduksc2_handle,
                                           chnl);
        }
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    PMC_RETURN(ret_val);

} /* coreotn_oduksc_ramp_done_isr */


/*******************************************************************************
* coreotn_oduksc_ramp_done_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the ramp_done interrupt status in each ODUKSC G.HAO instances.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) coreotn_oduksc_scbs3_cgen_reprov
*   2) coreotn_oduksc_ramp_prov
*   3) coreotn_oduksc_ramp_start
*   4) coreotn_oduksc_ramp_done_isr
*   Ramp-down:
*   1) coreotn_oduksc_ramp_prov
*   2) coreotn_oduksc_ramp_start
*   3) coreotn_oduksc_ramp_done_isr
*   4) coreotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *coreotn_handle     - pointer to COREOTN handle instance to be operated on
*   mux_stage           - MUX stage instance in COREOTN
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   ramp_done           - Ramp done interrupt status
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_oduksc_ramp_done_get(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT32 chnl,
                                              UINT32 *ramp_done)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(coreotn_handle != NULL, COREOTN_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl < COREOTN_NUM_CHNL, COREOTN_ERR_INVALID_ARG, 0, 0);


    if(mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF1 */
            ret_val = oduksc_chnl_ramp_done_get(coreotn_handle->oduksc1_handle,
                                                chnl,
                                                ramp_done);
        }
    }
    else if(mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        if(ret_val == PMC_SUCCESS)
        {
            /* ODUKSC in FMF2 */
            ret_val = oduksc_chnl_ramp_done_get(coreotn_handle->oduksc2_handle,
                                                chnl,
                                                ramp_done);
        }
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("Call coreotn_oduksc_ramp_done_get(): mux_stage=%d, chnl_id=%d, ramp_done=%d\n", mux_stage, chnl, *ramp_done);


    PMC_RETURN(ret_val);

} /* coreotn_oduksc_ramp_done_get */


/*******************************************************************************
*  coreotn_odujat_ghao_resize_fifo
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the CFC fifo in the ODUJAT instances in the flex pipeline.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_instance           - If 1 : mux in FMF1 is the muxing node
*                               2 : mux in FMF2 is the muxing node
*   chnl_id                - Channel id to perform the adjustment for
*   oduflex_num_tribslots  - Final number of ODUflex tribslots
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_odujat_ghao_resize_fifo(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_instance,
                                                 UINT32 chnl_id,
                                                 UINT32 oduflex_num_tribslots)
{
    PMC_ENTRY();
    PMC_ERROR ret_val = PMC_SUCCESS;

    if(mux_instance == COREOTN_MUX_STAGE_ONE)
    {
        if(coreotn_handle->var.init_operation == COREOTN_SS_OPERATIONAL_MODE)
        {
            /* ODUJAT adjustments in FMF1(muxing) */
            ret_val = odujat_ghao_resize_fifo(coreotn_handle->odujat1_handle,
                                              chnl_id,
                                              oduflex_num_tribslots);


            /* ODUJAT adjustments in FMF2(passthru) */
            ret_val = odujat_ghao_resize_fifo(coreotn_handle->odujat2_handle,
                                              chnl_id,
                                              oduflex_num_tribslots);


        }
        else if(coreotn_handle->var.init_operation == COREOTN_SS_ONE_STAGE_MUXING_MODE)
        {
            /* ODUJAT adjustments in FMF1(muxing) */
            ret_val = odujat_ghao_resize_fifo(coreotn_handle->odujat1_handle,
                                              chnl_id,
                                              oduflex_num_tribslots);

        }

    }
    else if(mux_instance == COREOTN_MUX_STAGE_TWO)
    {
        /* ODUJAT adjustments in FMF2(muxing) */
        ret_val = odujat_ghao_resize_fifo(coreotn_handle->odujat2_handle,
                                          chnl_id,
                                          oduflex_num_tribslots);
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }

    PMC_RETURN(ret_val);

} /* coreotn_odujat_ghao_resize_fifo */



/*******************************************************************************
*  coreotn_mux_ohfs_insert_lo_ipt_clear_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   When performing GHAO, special configurations are required when changing
*   the last tribslot.  However, at the end of the knife edge, there
*   Will be two JC_WR_IPT_VALIDs set.  It's required to clean up the
*   old valid.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   old_highest_cs         - The old highest calendar slots
*   chnl_id                - Channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ohfs_insert_lo_ipt_clear_cfg(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 old_highest_cs,
                                                          UINT8 chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;

    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    if (result == PMC_SUCCESS)
    {
        result = ohfs_insert_lo_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                        old_highest_cs,
                                        chnl_id,
                                        0,
                                        0);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ohfs_insert_lo_ipt_clear_cfg */




/*******************************************************************************
*  coreotn_mux_ghao_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovs the current LO in the MUX context before updating the database.
*
* INPUTS:
*   coreotn_handle         - handle to the COREOTN instance to be operated on
*   mux_stage              - ODU MUX stage instance in COREOTN
*   ho_chnl_id             - HO channel id
*   lo_chnl_id             - LO channel id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 lo_chnl_id,
                                                 UINT32 ho_chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    odtu_mux_handle_t *odtu_mux_handle;
    UINT8 dummy_num_cal;



    if (mux_stage == COREOTN_MUX_STAGE_ONE)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux1_handle;
    }
    else if (mux_stage == COREOTN_MUX_STAGE_TWO)
    {
        odtu_mux_handle = coreotn_handle->odtu_mux2_handle;
    }
    else
    {
        PMC_RETURN(COREOTN_ERR_INVALID_ARG);
    }


    if (result == PMC_SUCCESS)
    {
        result = odtu_mux_ghao_lo_chnl_deprov(odtu_mux_handle,
                                              (odtu_mux_stage_instance_t)mux_stage,
                                              lo_chnl_id,
                                              ho_chnl_id,
                                              FALSE,
                                              0,
                                              &dummy_num_cal);
    }

    PMC_RETURN(result);
} /* coreotn_mux_ghao_lo_chnl_deprov */




/*******************************************************************************
*  coreotn_mux_ghao_clear_old_last_cal_slot
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovs the current LO in the MUX context before updating the database.
*
* INPUTS:
*   coreotn_handle            - handle to the COREOTN instance to be operated on
*   ghao_mux_stage            - See coreotn_mux_stage_t
*   ghao_increase             - TRUE : rate is increasing
*                               FALSE : rate is decreasing
*   coreotn_lo_chnl_id        - LO channel id
*   coreotn_tx_old_trib_slots - old tribslot mask
*   coreotn_tx_new_trib_slots - new tribslot mask
*   coreotn_tx_old_cal_mask   - old calendar mask
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_clear_old_last_cal_slot(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t ghao_mux_stage,
                                                          UINT32 coreotn_lo_chnl_id,
                                                          BOOL ghao_increase,
                                                          UINT8 coreotn_tx_old_trib_slots[96],
                                                          UINT8 coreotn_tx_new_trib_slots[96],
                                                          UINT32 coreotn_tx_old_cal_mask[3])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 old_highest_ts;
    UINT32 new_highest_ts;
    UINT32 old_highest_cs = 0;
    UINT32 i,j;

    PMC_ENTRY();


    /* After changing the last tribslot, two bits will be set in
    JC_WR_IPT_VALID after the LCR resize.  It's required to remove the "old" valid */
    if (result == PMC_SUCCESS)
    {
        old_highest_ts = 0;
        new_highest_ts = 0;
        for(i=0;i<96;i++)
        {
            if(coreotn_tx_old_trib_slots[i])
            {
                old_highest_ts = i;
            }
            if(coreotn_tx_new_trib_slots[i])
            {
                new_highest_ts = i;
            }
        }

        /* check if the new last tribslot is greater than the last */
        if( (new_highest_ts > old_highest_ts && ghao_increase == TRUE) ||
            (old_highest_ts > new_highest_ts && ghao_increase == FALSE) )
        {
            /*set the old JC_WR_IPT_VALID to zero*/

            for(i=0;i<3;i++)
            {
                for(j=0;j<32;j++)
                {
                      if((coreotn_tx_old_cal_mask[i] & (1 << j)) > 0)
                      {
                          old_highest_cs = (i*32)+j;
                      }
                }
            }

            result = coreotn_mux_ohfs_insert_lo_ipt_clear_cfg(coreotn_handle,
                                                              ghao_mux_stage,
                                                              old_highest_cs,
                                                              coreotn_lo_chnl_id);
        }
    }

    PMC_RETURN(result);


} /* coreotn_mux_ghao_clear_old_last_cal_slot */




/*******************************************************************************
*  coreotn_mux_ghao_passthru_cal_enable_diable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs the same operation ascoreotn_mux_ghao_recfg_exp_lo_calen(),
*   enables and disables calendar entries in the passthru MUX. The intention of
*   this API is to simplify to API for firmware prototyping.
*
* INPUTS:
*   coreotn_handle            - handle to the COREOTN instance to be operated on
*   cycle                     - Calendar cycle to add/remove
*   enable                    - TRUE : Add calendar
*                               FALSE : Remove calendar
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR coreotn_mux_ghao_passthru_cal_enable_diable(coreotn_handle_t *coreotn_handle,
                                                             UINT32 cycle,
                                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    result = odtu_mux_ghao_passthru_cal_enable_diable(coreotn_handle->odtu_mux2_handle,
                                                      cycle,
                                                      enable);

    PMC_RETURN(result);


} /* coreotn_mux_ghao_passthru_cal_enable_diable */




/*
** End of file
*/
