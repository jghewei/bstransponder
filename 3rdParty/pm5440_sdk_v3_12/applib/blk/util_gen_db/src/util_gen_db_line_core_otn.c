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
#include "pmc_sys.h"
#include "pmc_types.h"
#include "util_gen_db.h"
#include "util_gen_db_line_core_otn.h"

/*
** Local Constants
*/
#define LINE_CORE_OTN_RX_DB     (0)
#define LINE_CORE_OTN_TX_DB     (1)
#define MAX_LINE_CORE_OTN_DB    (2)

/*
** Local Variables
*/

/**
 * Index table to look up the db_idx by db_id
 */
PRIVATE UINT8 line_core_otn_db_idx_get[] =
{
    INVALID_DB_IDX,             /* UTIL_GEN_DB_ODUKSC_DB */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_MPMA_DB */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_MPMO_DB */
    LINE_CORE_OTN_RX_DB,        /* UTIL_GEN_DB_LINE_CORE_OTN_RX_DB */
    LINE_CORE_OTN_TX_DB,        /* UTIL_GEN_DB_LINE_CORE_OTN_TX_DB */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_DCPB_DCS_0 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_DCPB_DCS_1 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_DCPB_DCS_2 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_DCPB_DCS_3 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_OCPB_DCS_0 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_OCPB_DCS_1 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_OCPB_DCS_2 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_OCPB_DCS_3 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_CBRC_SCBS3 */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_CBRC_MPMO */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_ENET_LINE_MPMA */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_ENET_SYS_MPMA */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_LINEOTN_SYSOTN_MODE */
    INVALID_DB_IDX,
    INVALID_DB_IDX
};


PRIVATE PMC_ERROR line_core_otn_gen_db_add_rec(UINT8 db_idx, void *metadata, void *rec);

/*
** Local struct definitions
*/

/*******************************************************************************
* STRUCTURE: line_core_otn_db_key_table
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the key tables for the line_core_otn DB.
*
* ELEMENTS:
*   ho_channels  - the ho_channel key table.
*   mo_channels  - the mo_channel key table.
*   lo_channels  - the lo_channel key table.
*   trib_slots   - the trib_slot key table.
*******************************************************************************/
typedef struct line_core_otn_db_key_table {
    util_gen_db_key_table single_entries[CALENDAR_ENTRIES_NUM];
    util_gen_db_key_table ho_channels[LINEOTN_HO_CHANNLES_NUM];
    util_gen_db_key_table mo_channels[COREOTN_MO_CHANNLES_NUM];
    util_gen_db_key_table lo_channels[COREOTN_LO_CHANNLES_NUM];
    util_gen_db_key_table trib_slots[COREOTN_TRIB_SLOTS_NUM];
    util_gen_db_key_table all_recs;
} line_core_otn_db_key_table;


/*******************************************************************************
* STRUCTURE: line_core_otn_db_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the data used for the line_core_otn DB.
*
* ELEMENTS:
*   db_ptr      - holds db allocated memory per instance
*   db_size     - holds db allocated memory size for internal usage
*   key_table   - pointer to the key table arrays.
*******************************************************************************/
typedef struct line_core_otn_db_data {
    line_core_otn_db_rec        *db_ptr[MAX_LINE_CORE_OTN_DB];
    UINT32                      db_size[MAX_LINE_CORE_OTN_DB];
    line_core_otn_db_key_table  *key_table[MAX_LINE_CORE_OTN_DB];
} line_core_otn_db_data;


/*
** Local Macro Definitions
*/


/*******************************************************************************
* line_core_otn_gen_db_reinit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is called on restart to reinitialize the key tables. 
*
*
* INPUTS:
*   not_used_idx- This isn't actually used, this function will loop through all
*                   the line_core_otn databases.
*   metadata    - pointer to metadata for this DB.
*   not_used    - void pointer, not used for anything. was put there to comply
*                 with function pointer definition.
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
PUBLIC PMC_ERROR line_core_otn_db_reinit(util_gen_db_id_t not_used_idx, void *metadata, void *not_used)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8  db_idx = not_used_idx;
    UINT8  i;
    util_gen_db_db_metadata_t *db_metadata;
    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    line_core_otn_db_data   *db_data;

    PMC_ENTRY();

    not_used = NULL;

    db_data = (line_core_otn_db_data *)(db_metadata->db_data);

    for (db_idx = 0; NULL != db_data && db_idx < MAX_LINE_CORE_OTN_DB; db_idx++)
    {
        if (NULL != db_data->db_ptr[db_idx])
        {
            if (PMC_SUCCESS == rc)
            {
                db_data->key_table[db_idx] = (line_core_otn_db_key_table *)PMC_CALLOC(sizeof(line_core_otn_db_key_table));
                PMC_ASSERT(NULL != db_data->key_table[db_idx], UTIL_GEN_DB_ERR_FAIL, 0, 0);
                PMC_MEMSET(db_data->key_table[db_idx]->single_entries, 0x0, sizeof(db_data->key_table[db_idx]->single_entries));
                PMC_MEMSET(db_data->key_table[db_idx]->ho_channels, 0x0, sizeof(db_data->key_table[db_idx]->ho_channels));
                PMC_MEMSET(db_data->key_table[db_idx]->mo_channels, 0x0, sizeof(db_data->key_table[db_idx]->mo_channels));
                PMC_MEMSET(db_data->key_table[db_idx]->lo_channels, 0x0, sizeof(db_data->key_table[db_idx]->lo_channels));
                PMC_MEMSET(db_data->key_table[db_idx]->trib_slots, 0x0, sizeof(db_data->key_table[db_idx]->trib_slots));
            }
            for (i = 0; i < CALENDAR_ENTRIES_NUM; i++)
            {
                if (TRUE == db_data->db_ptr[db_idx][i].valid)
                {
                    /* This will result in a useless copy of itself, but it will
                     * also re-insert into the key tables as appropriate */
                    if (PMC_SUCCESS == rc)
                    {
                        rc = line_core_otn_gen_db_add_rec(db_idx, metadata, &db_data->db_ptr[db_idx][i]);
                    }
                }
                db_data->key_table[db_idx]->all_recs.q_table[i] = i;
                db_data->key_table[db_idx]->all_recs.q_recs[i] = &db_data->db_ptr[db_idx][i];
                db_data->key_table[db_idx]->single_entries[i].q_table[0] = i;
                db_data->key_table[db_idx]->single_entries[i].q_recs[0] = &db_data->db_ptr[db_idx][i];
                db_data->key_table[db_idx]->single_entries[i].q_size = 1;
            }
            db_data->key_table[db_idx]->all_recs.q_size = CALENDAR_ENTRIES_NUM;
        }
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
* line_core_otn_gen_db_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_attach function
*   first user to call the gen_db_attach will creates and initializes the database.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   not_used    - void pointer, not used for anything. was put there to comply
*                 with function pointer definition.
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
PRIVATE UINT32 line_core_otn_gen_db_init(UINT8 db_idx, void *metadata, void *not_used)
{
    UINT32 rc = UTIL_GEN_DB_ERR_FAIL;
    UINT8  i;
    util_gen_db_db_metadata_t *db_metadata;
    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    line_core_otn_db_data   *db_data;

    PMC_ENTRY();

    not_used = NULL;

    /* allocate db and init to all 0's */

    /* If this is the first time we are init'ing the DB, allocate the db_data
     * pointer first
     */
    if (NULL == db_metadata->db_data)
    {
        db_data = (line_core_otn_db_data *)PMC_CTXT_CALLOC(sizeof(line_core_otn_db_data), db_metadata->pmc_handle);
        db_metadata->db_data = (void *)db_data;
        PMC_CTXT_REGISTER_PTR(&db_metadata->db_data, db_metadata->pmc_handle);
        
        for (i = 0; i < MAX_LINE_CORE_OTN_DB; i++)
        {
            /* Shouldn't really need to set this to 0 and NULL since
             * PMC_CTXT_CALLOC does that already
             */
            db_data->db_size[i] = 0;
            db_data->db_ptr[i] = NULL;
            /* We'll register all of them now in case they get PMC_CTXT_CALLOC'd
             * multiple times
             */
            PMC_CTXT_REGISTER_PTR(&db_data->db_ptr[i], db_metadata->pmc_handle);
        }
    }
    else
    {
        db_data = (line_core_otn_db_data *)(db_metadata->db_data);
    }



    /* Make sure we haven't init'd already */
    PMC_ASSERT(0 == db_data->db_size[db_idx], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    db_data->db_size[db_idx] = sizeof(line_core_otn_db_rec)* CALENDAR_ENTRIES_NUM;
    db_data->db_ptr[db_idx] = (line_core_otn_db_rec *)PMC_CTXT_CALLOC(db_data->db_size[db_idx], db_metadata->pmc_handle);
    
    if (NULL != db_data->db_ptr[db_idx])
    {
        /* set db content to 0xff */
        PMC_MEMSET(db_data->db_ptr[db_idx], 0xff, db_data->db_size[db_idx]);
        /* allocate the key tables */
        db_data->key_table[db_idx] = (line_core_otn_db_key_table *)PMC_CALLOC(sizeof(line_core_otn_db_key_table));
        PMC_MEMSET(db_data->key_table[db_idx]->single_entries, 0x0, sizeof(db_data->key_table[db_idx]->single_entries));
        PMC_MEMSET(db_data->key_table[db_idx]->ho_channels, 0x0, sizeof(db_data->key_table[db_idx]->ho_channels));
        PMC_MEMSET(db_data->key_table[db_idx]->mo_channels, 0x0, sizeof(db_data->key_table[db_idx]->mo_channels));
        PMC_MEMSET(db_data->key_table[db_idx]->lo_channels, 0x0, sizeof(db_data->key_table[db_idx]->lo_channels));
        PMC_MEMSET(db_data->key_table[db_idx]->trib_slots, 0x0, sizeof(db_data->key_table[db_idx]->trib_slots));
        for (i = 0; i < CALENDAR_ENTRIES_NUM; i++)
        {
            db_data->key_table[db_idx]->all_recs.q_table[i] = i;
            db_data->key_table[db_idx]->all_recs.q_recs[i] = &db_data->db_ptr[db_idx][i];
            db_data->key_table[db_idx]->single_entries[i].q_table[0] = i;
            db_data->key_table[db_idx]->single_entries[i].q_recs[0] = &db_data->db_ptr[db_idx][i];
            db_data->key_table[db_idx]->single_entries[i].q_size = 1;
        }
        db_data->key_table[db_idx]->all_recs.q_size = CALENDAR_ENTRIES_NUM;
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
}/* line_core_otn_gen_db_init */


/*******************************************************************************
* line_core_otn_gen_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_detach function
*   last user to call the gen_db_detach will free database allocated memory.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   not_used    - void pointer, not used for anything. was put there to comply
*                 with function pointer definition.
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
PRIVATE UINT32 line_core_otn_gen_db_destroy(UINT8 db_idx, void *metadata, void *not_used)
{

    line_core_otn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (line_core_otn_db_data *)(db_metadata->db_data);

    PMC_ENTRY();

    not_used = NULL;

    if (NULL != db_data->db_ptr[db_idx])
    {
        /* free up db allocated memory */
        PMC_CTXT_FREE(&db_data->db_ptr[db_idx], db_metadata->pmc_handle);
        PMC_FREE(&db_data->key_table[db_idx]);
        db_data->db_size[db_idx] = 0;
    }

    PMC_RETURN (PMC_SUCCESS);
}/* line_core_otn_gen_db_destroy */

/*******************************************************************************
* line_core_otn_gen_db_add_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_add_record function.
*   user must provide a full record for insertion.
*   in this line_core_otn we are using the unique (1:1) key to find insertion place.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   rec         - void pointer, to casted to user defined record structure.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PRIVATE UINT32 line_core_otn_gen_db_add_rec(UINT8 db_idx, void *metadata, void *rec)
{
    line_core_otn_db_rec  *p_rec = (line_core_otn_db_rec*)rec;
    UINT32 rc = UTIL_GEN_DB_ERR_FAIL;
    line_core_otn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (line_core_otn_db_data *)(db_metadata->db_data);


    /* user MUST !!!! provide a full record                          */
    /* we are using the calaedar_entry key which is a unique 1:1 key */
    /* to find the right place of insersion                          */
    if (NULL != p_rec && p_rec->calendar_entry < CALENDAR_ENTRIES_NUM )
    {
        db_data->db_ptr[db_idx][p_rec->calendar_entry] = *p_rec;
        /* From here on out, use the pointer to the rec in the DB */
        p_rec = &db_data->db_ptr[db_idx][p_rec->calendar_entry];
        p_rec->valid = TRUE;

        /* Update the key tables if the key has a valid value */
        if (p_rec->ho_channel < LINEOTN_HO_CHANNLES_NUM)
        {
            util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->ho_channels[p_rec->ho_channel], p_rec->calendar_entry, p_rec);
        }
        if (p_rec->mo_channel < COREOTN_MO_CHANNLES_NUM)
        {
            util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->mo_channels[p_rec->mo_channel], p_rec->calendar_entry, p_rec);
        }
        if (p_rec->lo_channel < COREOTN_LO_CHANNLES_NUM)
        {
            util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->lo_channels[p_rec->lo_channel], p_rec->calendar_entry, p_rec);
        }
        if (p_rec->trib_slot < COREOTN_TRIB_SLOTS_NUM)
        {
            util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->trib_slots[p_rec->trib_slot], p_rec->calendar_entry, p_rec);
        }
        rc = PMC_SUCCESS;
    }

    return(rc);

}/* line_core_otn_gen_db_add_rec */

/*******************************************************************************
* line_core_otn_gen_db_rem_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_rem_record function.
*   user must provide at least one valid key record record.
*   in this line_core_otn we are using either the unique (1:1) key or the (1:n) in order to
*   find the records to be removed.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PRIVATE UINT32 line_core_otn_gen_db_rem_rec(UINT8 db_idx, void *metadata, void *key)
{
    UINT32 rc = PMC_SUCCESS;
    util_gen_db_key_table match;
    line_core_otn_db_key          *p_key = (line_core_otn_db_key*)key;
    line_core_otn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;

    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (line_core_otn_db_data *)(db_metadata->db_data);

    PMC_MEMSET((void *)&match, 0x0, sizeof(match));

    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            /* 1:1 match - we know exactly where to erase*/
            match.q_size = 1;
            match.q_table[0] = p_key->calendar_entry;
        }
        else if (p_key->ho_channel != DO_NOT_USE_KEY && p_key->ho_channel < LINEOTN_HO_CHANNLES_NUM)
        {
            match = db_data->key_table[db_idx]->ho_channels[p_key->ho_channel];
        }
        else if (p_key->mo_channel != DO_NOT_USE_KEY && p_key->mo_channel < COREOTN_MO_CHANNLES_NUM)
        {
            match = db_data->key_table[db_idx]->mo_channels[p_key->mo_channel];
        }
        else if (p_key->lo_channel != DO_NOT_USE_KEY && p_key->lo_channel < COREOTN_LO_CHANNLES_NUM)
        {
            match = db_data->key_table[db_idx]->lo_channels[p_key->lo_channel];
        }
        else if (p_key->trib_slot != DO_NOT_USE_KEY && p_key->trib_slot < COREOTN_TRIB_SLOTS_NUM)
        {
            match = db_data->key_table[db_idx]->trib_slots[p_key->trib_slot];
        }
        else
        {
            rc = UTIL_GEN_DB_ERR_FAIL;
        }
    }
    else
    {
        rc = UTIL_GEN_DB_ERR_FAIL;
    }

    if (match.q_size > 0 && rc == PMC_SUCCESS)
    {
        UINT32 i;
        for (i = 0; i < match.q_size; i++)
        {
            line_core_otn_db_rec  *p_rec = &db_data->db_ptr[db_idx][match.q_table[i]];
            /* Update the key tables */
            if (p_rec->ho_channel < LINEOTN_HO_CHANNLES_NUM)
            {
                util_gen_db_rm_from_key_table(&db_data->key_table[db_idx]->ho_channels[p_rec->ho_channel], match.q_table[i]);
            }
            if (p_rec->mo_channel < COREOTN_MO_CHANNLES_NUM)
            {
                util_gen_db_rm_from_key_table(&db_data->key_table[db_idx]->mo_channels[p_rec->mo_channel], match.q_table[i]);
            }
            if (p_rec->lo_channel < COREOTN_LO_CHANNLES_NUM)
            {
                util_gen_db_rm_from_key_table(&db_data->key_table[db_idx]->lo_channels[p_rec->lo_channel], match.q_table[i]);
            }
            if (p_rec->trib_slot < COREOTN_TRIB_SLOTS_NUM)
            {
                util_gen_db_rm_from_key_table(&db_data->key_table[db_idx]->trib_slots[p_rec->trib_slot], match.q_table[i]);
            }
            /* Reset the entry in the DB */
            PMC_MEMSET(&db_data->db_ptr[db_idx][match.q_table[i]], 0xff, sizeof(line_core_otn_db_rec));
        }
    }
    return (rc);

}/* line_core_otn_gen_db_rem_rec */

/*******************************************************************************
* line_core_otn_gen_db_update_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_update_record function.
*   user must provide a valid calender entry for the update to take place.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   rec         - void pointer, to casted to user defined record structure.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PRIVATE UINT32 line_core_otn_gen_db_update_rec(UINT8 db_idx, void *metadata, void *rec)
{
    line_core_otn_db_rec  *p_new_rec = (line_core_otn_db_rec*)rec;
    UINT32 rc       = UTIL_GEN_DB_ERR_FAIL;
    line_core_otn_db_key          key;


    /* The easiest way to do this and update all the key tables (which might be
     * changing) is to just remove the old record and re-add the new one */
    if (NULL != p_new_rec && p_new_rec->calendar_entry < CALENDAR_ENTRIES_NUM )
    {
        key.calendar_entry = p_new_rec->calendar_entry;
        line_core_otn_gen_db_rem_rec(db_idx, metadata, (void *)&key);
        line_core_otn_gen_db_add_rec(db_idx, metadata, rec);
        rc = PMC_SUCCESS;
    }

   return(rc);
}/* line_core_otn_gen_db_update_rec */


/*******************************************************************************
* line_core_otn_gen_db_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_query function.
*   user must provide at least one valid key record record.
*   in this line_core_otn we are using either the unique (1:1) key or the (1:n) in order to
*   find the records to be to be returned to the user.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
* OUTPUTS:
*   records     - void pointer, a pointer to user query result records will be 
*                 returned in this pointer.
*                 If no records found - NULL pointer and number of records found
*                 will be set to 0.
*
* RETURNS:
*   UINT32 - number of records found;
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PRIVATE UINT32  line_core_otn_gen_db_query(UINT8 db_idx, void *metadata, void *key, void** records)
{
    line_core_otn_db_key  *p_key = (line_core_otn_db_key*)key;
    util_gen_db_key_table *match;
    line_core_otn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;

    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (line_core_otn_db_data *)(db_metadata->db_data);
    match = NULL;
    
    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            match = &db_data->key_table[db_idx]->single_entries[p_key->calendar_entry];
        }
        else if (p_key->ho_channel != DO_NOT_USE_KEY && p_key->ho_channel < LINEOTN_HO_CHANNLES_NUM)
        {
            match = &db_data->key_table[db_idx]->ho_channels[p_key->ho_channel];
        }
        else if (p_key->mo_channel != DO_NOT_USE_KEY && p_key->mo_channel < COREOTN_MO_CHANNLES_NUM)
        {
            match = &db_data->key_table[db_idx]->mo_channels[p_key->mo_channel];
        }
        else if (p_key->lo_channel != DO_NOT_USE_KEY && p_key->lo_channel < COREOTN_LO_CHANNLES_NUM)
        {
            match = &db_data->key_table[db_idx]->lo_channels[p_key->lo_channel];
        }
        else if (p_key->trib_slot != DO_NOT_USE_KEY && p_key->trib_slot < COREOTN_TRIB_SLOTS_NUM)
        {
            match = &db_data->key_table[db_idx]->trib_slots[p_key->trib_slot];
        }
        else if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry == CALENDAR_ENTRIES_NUM)
        {
            /* special case to return the entire array contents for all calendar entries */
            match = &db_data->key_table[db_idx]->all_recs;
        }
    }

    /* We can do this bit of magic because the query_res struct has the same
     * format as the key_table struct */
    *records = (void*)match;
    
    if (NULL != match)
    {
        return (match->q_size);
    }
    else
    {
        return 0;
    }
}/* line_core_otn_gen_db_query */


/*******************************************************************************
* line_core_otn_gen_db_query_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a user function implemetation of the generic gen_db_query_count
*   function.
*   user must provide at least one valid key record record.
*   in this line_core_otn we are using either the unique (1:1) key or the (1:n) in order to
*   find the records to be counted.
*
*
* INPUTS:
*   db_idx      - db index point to rx or tx db copy
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - number of records found.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PRIVATE UINT32  line_core_otn_gen_db_query_count(UINT8 db_idx, void *metadata, void *key)
{
    line_core_otn_db_key  *p_key = (line_core_otn_db_key*)key;
    line_core_otn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    UINT32 rc;



    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (line_core_otn_db_data *)(db_metadata->db_data);
    rc = 0;
    

    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            rc = 1;
        }
        else if (p_key->ho_channel != DO_NOT_USE_KEY && p_key->ho_channel < LINEOTN_HO_CHANNLES_NUM)
        {
            rc = db_data->key_table[db_idx]->ho_channels[p_key->ho_channel].q_size;
        }
        else if (p_key->mo_channel != DO_NOT_USE_KEY && p_key->mo_channel < COREOTN_MO_CHANNLES_NUM)
        {
            rc = db_data->key_table[db_idx]->mo_channels[p_key->mo_channel].q_size;
        }
        else if (p_key->lo_channel != DO_NOT_USE_KEY && p_key->lo_channel < COREOTN_LO_CHANNLES_NUM)
        {
            rc = db_data->key_table[db_idx]->lo_channels[p_key->lo_channel].q_size;
        }
        else if (p_key->trib_slot != DO_NOT_USE_KEY && p_key->trib_slot < COREOTN_TRIB_SLOTS_NUM)
        {
            rc = db_data->key_table[db_idx]->trib_slots[p_key->trib_slot].q_size;
        }
    }

    return (rc);
}/* line_core_otn_gen_db_query */

/*******************************************************************************
* line_core_otn_db_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   not_used    - void pointer, not used for anything. was put there to comply
*                 with function pointer definition.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_init(util_gen_db_id_t db_id, void *metadata, void *not_used)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8     db_idx = 0;

    PMC_ENTRY();

    not_used = NULL;

    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc=line_core_otn_gen_db_init(db_idx, metadata, not_used);
    }

    PMC_RETURN (rc);
}/* line_core_otn_db_init */


/*******************************************************************************
* line_core_otn_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   not_used    - void pointer, not used for anything. was put there to comply
*                 with function pointer definition.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_destroy   (util_gen_db_id_t db_id, void *metadata, void *not_used)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8     db_idx = 0;

    PMC_ENTRY();

    not_used = NULL;

    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = line_core_otn_gen_db_destroy(db_idx, metadata, not_used);
    }

    PMC_RETURN (rc);
}/* line_core_otn_db_destroy */

/*******************************************************************************
* line_core_otn_rx_db_add_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   rec         - void pointer, to casted to user defined record structure.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_add_rec   (util_gen_db_id_t db_id, void *metadata, void *rec)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8     db_idx = 0;


    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = line_core_otn_gen_db_add_rec(db_idx, metadata, rec);
    }

    return (rc);

}/* line_core_otn_db_add_rec */

/*******************************************************************************
* line_core_otn_db_rem_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_rem_rec   (util_gen_db_id_t db_id, void *metadata, void *key)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8     db_idx = 0;


    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = line_core_otn_gen_db_rem_rec(db_idx, metadata, key);
    }

    return (rc);
}/* line_core_otn_db_rem_rec */

/*******************************************************************************
* line_core_otn_db_update_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   rec         - void pointer, to casted to user defined record structure.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_update_rec(util_gen_db_id_t db_id, void *metadata, void *rec)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8     db_idx = 0;


    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc=line_core_otn_gen_db_update_rec(db_idx, metadata, rec);
    }

    return (rc);

}/* line_core_otn_db_update_rec */


/*******************************************************************************
* line_core_otn_db_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
*
* OUTPUTS:
*   records     - void pointer, a pointer to user query result records will be 
*                 returned in this pointer.
*                 If no records found - NULL pointer and number of records found
*                 will be set to 0.
*
* RETURNS:
*   UINT32 - number of records found
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32  line_core_otn_db_query(util_gen_db_id_t db_id, void *metadata, void *key, void** records)
{
    UINT32 result = 0;
    UINT8  db_idx = 0;


    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        result = line_core_otn_gen_db_query(db_idx, metadata, key, records);
    }

    return (result);
}/* line_core_otn_db_query */





/*******************************************************************************
* line_core_otn_db_query_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forwader function to appropriate rx/tx 'gen' function copy
*
*
* INPUTS:
*   db_id       - enum, database id
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - number of records found
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 line_core_otn_db_query_count(util_gen_db_id_t db_id, void *metadata, void *key)
{
    UINT32 result = 0;
    UINT8  db_idx = 0;

    db_idx = line_core_otn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        result = line_core_otn_gen_db_query_count(db_idx, metadata, key);
    }

    return (result);
}/* line_core_otn_db_query_count */





