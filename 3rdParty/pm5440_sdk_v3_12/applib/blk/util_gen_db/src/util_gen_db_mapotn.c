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
#include "util_gen_db_mapotn.h"

/*
** Local Constants
*/
#define MAX_MAPOTN_DB           (18)

/*
** Local variables
*/

/**
 * Index table to look up the maximum number of records by db_idx
 */
PRIVATE UINT32 mapotn_db_max_channel_get[] =
{
    MAX_MAPOTN_CHANNELS,        /* UTIL_GEN_DB_ODUKSC_DB */
    MAX_MAPOTN_CHANNELS,        /* UTIL_GEN_DB_MPMA_DB */
    MAX_MAPOTN_CHANNELS,        /* UTIL_GEN_DB_MPMO_DB */
    MAX_DCPB_DCS_0_CHANNELS,    /* UTIL_GEN_DB_DCPB_DCS_0 */
    MAX_DCPB_DCS_1_CHANNELS,    /* UTIL_GEN_DB_DCPB_DCS_1 */
    MAX_DCPB_DCS_2_CHANNELS,    /* UTIL_GEN_DB_DCPB_DCS_2 */
    MAX_DCPB_DCS_3_CHANNELS,    /* UTIL_GEN_DB_DCPB_DCS_3 */
    MAX_OCPB_DCS_0_CHANNELS,    /* UTIL_GEN_DB_OCPB_DCS_0 */
    MAX_OCPB_DCS_1_CHANNELS,    /* UTIL_GEN_DB_OCPB_DCS_1 */
    MAX_OCPB_DCS_2_CHANNELS,    /* UTIL_GEN_DB_OCPB_DCS_2 */
    MAX_OCPB_DCS_3_CHANNELS,    /* UTIL_GEN_DB_OCPB_DCS_3 */
    UTIL_GLOBAL_CBRC_MAX_SLOTS, /* UTIL_GEN_DB_CBRC_SCBS3 */
    UTIL_GLOBAL_CBRC_MAX_SLOTS, /* UTIL_GEN_DB_CBRC_MPMO */
    MAX_ENET_CHANNELS,          /* UTIL_GEN_DB_ENET_LINE_MPMA */
    MAX_ENET_CHANNELS,          /* UTIL_GEN_DB_ENET_SYS_MPMA */
    96                          /* UTIL_GEN_DB_LINEOTN_SYSOTN_MODE */
};

/**
 * Index table to look up the db_idx by db_id
 */
PRIVATE UINT8 mapotn_db_idx_get[] =
{
    0,                          /* UTIL_GEN_DB_ODUKSC_DB */
    1,                          /* UTIL_GEN_DB_MPMA_DB */
    2,                          /* UTIL_GEN_DB_MPMO_DB */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_LINE_CORE_OTN_RX_DB */
    INVALID_DB_IDX,             /* UTIL_GEN_DB_LINE_CORE_OTN_TX_DB */
    3,                          /* UTIL_GEN_DB_DCPB_DCS_0 */
    4,                          /* UTIL_GEN_DB_DCPB_DCS_1 */
    5,                          /* UTIL_GEN_DB_DCPB_DCS_2 */
    6,                          /* UTIL_GEN_DB_DCPB_DCS_3 */
    7,                          /* UTIL_GEN_DB_OCPB_DCS_0 */
    8,                          /* UTIL_GEN_DB_OCPB_DCS_1 */
    9,                          /* UTIL_GEN_DB_OCPB_DCS_2 */
    10,                         /* UTIL_GEN_DB_OCPB_DCS_3 */
    11,                         /* UTIL_GEN_DB_CBRC_SCBS3 */
    12,                         /* UTIL_GEN_DB_CBRC_MPMO */
    13,                         /* UTIL_GEN_DB_ENET_LINE_MPMA */
    14,                         /* UTIL_GEN_DB_ENET_SYS_MPMA */
    15,                         /* UTIL_GEN_DB_LINEOTN_SYSOTN_MODE */
    INVALID_DB_IDX,
    INVALID_DB_IDX
};

PRIVATE PMC_ERROR mapotn_gen_db_add_rec(UINT32 db_idx, void *metadata, void *rec);

/*
** Local Macro Definitions
*/

/*
** Local struct definitions
*/

/*******************************************************************************
* STRUCTURE: mapotn_db_key_table
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the key tables for the mapotn DB.
*
* ELEMENTS:
*   channel_nums  - the channel_num key table.
*******************************************************************************/
typedef struct mapotn_db_key_table {
    util_gen_db_key_table   single_entries[CALENDAR_ENTRIES_NUM];
    util_gen_db_key_table   channel_nums[MAX_MAPOTN_DB_CHANNEL_NUMS];
    util_gen_db_key_table   all_recs;
} mapotn_db_key_table;


/*******************************************************************************
* STRUCTURE: mapotn_db_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the data used for the mapotn DB.
*
* ELEMENTS:
*   db_ptr      - holds db allocated memory per instance
*   db_size     - holds db allocated memory size for internal usage
*   key_table   - pointer to the channel_num key table array struct
*******************************************************************************/
typedef struct mapotn_db_data {
    mapotn_db_rec           *db_ptr[MAX_MAPOTN_DB];
    UINT32                  db_size[MAX_MAPOTN_DB];
    mapotn_db_key_table     *key_table[MAX_MAPOTN_DB];
} mapotn_db_data;

/*******************************************************************************
* mapotn_gen_db_reinit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is called on restart to reinitialize the key tables. 
*
*
* INPUTS:
*   not_used_idx- This isn't actually used, this function will loop through all
*                   the mapotn databases.
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
PUBLIC PMC_ERROR mapotn_gen_db_reinit(util_gen_db_id_t not_used_idx, void *metadata, void *not_used)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8  db_idx;
    UINT8  i;
    util_gen_db_db_metadata_t *db_metadata;
    mapotn_db_data   *db_data;

    PMC_ENTRY();

    not_used = NULL;
    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);

    for (db_idx = 0; NULL != db_data && db_idx < MAX_MAPOTN_DB; db_idx++)
    {
        if (NULL != db_data->db_ptr[db_idx])
        {
            if (PMC_SUCCESS == rc)
            {
                db_data->key_table[db_idx] = (mapotn_db_key_table *)PMC_CALLOC(sizeof(mapotn_db_key_table));
                PMC_ASSERT(NULL != db_data->key_table[db_idx], UTIL_GEN_DB_ERR_FAIL, 0, 0);
                PMC_MEMSET(db_data->key_table[db_idx]->single_entries, 0x0, sizeof(db_data->key_table[db_idx]->single_entries));
                PMC_MEMSET(db_data->key_table[db_idx]->channel_nums, 0x0, sizeof(db_data->key_table[db_idx]->channel_nums));
            }
            for (i = 0; i < CALENDAR_ENTRIES_NUM; i++)
            {
                if (TRUE == db_data->db_ptr[db_idx][i].valid)
                {
                    /* This will result in a useless copy of itself, but it will
                     * also re-insert into the key tables as appropriate */
                    if (PMC_SUCCESS == rc)
                    {
                        rc = mapotn_gen_db_add_rec(db_idx, metadata, &db_data->db_ptr[db_idx][i]);
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
* mapotn_gen_db_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function creates and initializes the database to 0xFF based on the
*   database ID.
*
*
* INPUTS:
*   db_idx      - database index to access
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
PUBLIC PMC_ERROR mapotn_gen_db_init(UINT32 db_idx, void *metadata, void *not_used)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    UINT8 i;
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;

    PMC_ENTRY();

    db_metadata = (util_gen_db_db_metadata_t *)metadata;

    not_used = NULL;

    /* allocate db and init to all 0's */

    /* If this is the first time we are init'ing the DB, allocate the db_data
     * pointer first
     */
    if (NULL == db_metadata->db_data)
    {
        db_data = (mapotn_db_data *)PMC_CTXT_CALLOC(sizeof(mapotn_db_data), db_metadata->pmc_handle);
        db_metadata->db_data = (void *)db_data;
        PMC_CTXT_REGISTER_PTR(&db_metadata->db_data, db_metadata->pmc_handle);
        
        for (i = 0; i < MAX_MAPOTN_DB; i++)
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
        db_data = (mapotn_db_data *)(db_metadata->db_data);
    }

    /* Make sure we haven't init'd already */
    PMC_ASSERT(0 == db_data->db_size[db_idx], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    db_data->db_size[db_idx] = sizeof(mapotn_db_rec) * CALENDAR_ENTRIES_NUM;
    /* allocate db and init to all 0xff's */
    db_data->db_ptr[db_idx] = (mapotn_db_rec*)PMC_CTXT_CALLOC(db_data->db_size[db_idx], db_metadata->pmc_handle);

    if (NULL != db_data->db_ptr[db_idx])
    {
        PMC_MEMSET(db_data->db_ptr[db_idx], 0xFF, db_data->db_size[db_idx]);
        db_data->key_table[db_idx] = (mapotn_db_key_table *)PMC_CALLOC(sizeof(mapotn_db_key_table));
        PMC_MEMSET(db_data->key_table[db_idx]->channel_nums, 0x0, sizeof(db_data->key_table[db_idx]->channel_nums));
        PMC_MEMSET(db_data->key_table[db_idx]->single_entries, 0x0, sizeof(db_data->key_table[db_idx]->single_entries));
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

}/* mapotn_gen_db_init */


/*******************************************************************************
* mapotn_gen_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function frees allocated memory for the specific database.
*
*
* INPUTS:
*   db_idx      - database index to access
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
PUBLIC PMC_ERROR mapotn_gen_db_destroy(UINT32 db_idx, void *metadata, void *not_used)
{
    mapotn_db_data            *db_data;
    util_gen_db_db_metadata_t *db_metadata;

    PMC_ENTRY();

    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);
    not_used = NULL;

    if (NULL != db_data->db_ptr[db_idx])
    {
        /* free up db allocated memory */
        PMC_CTXT_FREE(&db_data->db_ptr[db_idx], db_metadata->pmc_handle);
        PMC_FREE(&db_data->key_table[db_idx]);
        db_data->db_size[db_idx] = 0;
    }

    PMC_RETURN (PMC_SUCCESS);
}/* mapotn_gen_db_destroy */

/*******************************************************************************
* mapotn_gen_all_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function destroys all MAPOTN databases. 
*
*
* INPUTS:
*   not_used_idx- This isn't actually used, this function will loop through all
*                   the mapotn databases.
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
PUBLIC PMC_ERROR mapotn_gen_all_db_destroy(util_gen_db_id_t     not_used_idx,
                                           void                *metadata,
                                           void                *not_used)
{
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    UINT32 db_idx;

    PMC_ENTRY();

    if (NULL != metadata)
    {
        db_metadata = (util_gen_db_db_metadata_t *)metadata;
        db_data = (mapotn_db_data *)(db_metadata->db_data);
        for (db_idx = 0; NULL != db_data && db_idx < MAX_MAPOTN_DB; db_idx++)
        {
            if (NULL != db_data->db_ptr[db_idx])
            {
                /* free up db allocated memory */
                PMC_CTXT_FREE(&db_data->db_ptr[db_idx], db_metadata->pmc_handle);
                PMC_FREE(&db_data->key_table[db_idx]);
                db_data->db_size[db_idx] = 0;
            }
        }
    }
    PMC_RETURN (PMC_SUCCESS);
}/* mapotn_gen_all_db_destroy */

/*******************************************************************************
* mapotn_gen_db_add_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function addes a record to the specified database.
*   user must provide a full record for insertion.
*

*
* INPUTS:
*   db_idx      - database index to access
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
PRIVATE PMC_ERROR mapotn_gen_db_add_rec(UINT32 db_idx, void *metadata, void *rec)
{
    mapotn_db_rec  *p_rec = (mapotn_db_rec*)rec;
    PMC_ERROR rc       = UTIL_GEN_DB_ERR_FAIL;
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    UINT32          max_num_channels;


    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);

    PMC_ASSERT(db_idx < ( sizeof(mapotn_db_max_channel_get)/sizeof(mapotn_db_max_channel_get[0]) ), UTIL_GEN_DB_ERR_INVALID_PARAMETERS, db_idx, 0 );
    max_num_channels = mapotn_db_max_channel_get[db_idx];

    /* user MUST !!!! provide a full record                          */
    /* we are using the calaedar_entry key which is a unique 1:1 key */
    /* to find the right place of insersion                          */
    if (NULL != p_rec && p_rec->calendar_entry < CALENDAR_ENTRIES_NUM )
    {
        db_data->db_ptr[db_idx][p_rec->calendar_entry] = *p_rec;
        /* From here on out, use the pointer to the rec in the DB */
        p_rec = &db_data->db_ptr[db_idx][p_rec->calendar_entry];
        p_rec->valid = TRUE;
        util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->single_entries[p_rec->calendar_entry], p_rec->calendar_entry, p_rec);
        if (p_rec->channel_num < max_num_channels)
        {
            util_gen_db_add_to_key_table(&db_data->key_table[db_idx]->channel_nums[p_rec->channel_num], p_rec->calendar_entry, p_rec);
            /* Success only if the channel number is valid */
            rc = PMC_SUCCESS;
        }
    }

    return (rc);

}/* mapotn_gen_db_add_rec */


/*******************************************************************************
* mapotn_gen_db_rem_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function removes a record from the specified database.
*   user must provide at least one valid key record record.
*   In this function we are using either the unique (1:1) key (calendar_entry)
*   or the (1:n) key (channel_num) in order to remove the records.

*
*
* INPUTS:
*   db_idx      - database index to access
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
PUBLIC PMC_ERROR mapotn_gen_db_rem_rec(UINT32 db_idx, void *metadata, void *key)
{
    PMC_ERROR rc = PMC_SUCCESS;
    mapotn_db_key          *p_key = (mapotn_db_key*)key;
    UINT32          max_num_channels;
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    util_gen_db_key_table match;

    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);
    
    PMC_MEMSET((void *)&match, 0x0, sizeof(match));
    
    max_num_channels = mapotn_db_max_channel_get[db_idx];

    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            /* 1:1 match - we know exactly where to erase*/
            match.q_size = 1;
            match.q_table[0] = p_key->calendar_entry;
        }
        else if (p_key->channel_num != DO_NOT_USE_KEY && p_key->channel_num < max_num_channels)
        {
            match = db_data->key_table[db_idx]->channel_nums[p_key->channel_num];
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
            mapotn_db_rec  *p_rec = &db_data->db_ptr[db_idx][match.q_table[i]];
            /* Update the key table */
            if (p_rec->channel_num < max_num_channels)
            {
                util_gen_db_rm_from_key_table(&db_data->key_table[db_idx]->channel_nums[p_rec->channel_num], match.q_table[i]);
            }
            /* Reset the entry in the DB */
            PMC_MEMSET(&db_data->db_ptr[db_idx][match.q_table[i]], 0xff, sizeof(mapotn_db_rec));
        }
    }

    return (rc);

}/* mapotn_gen_db_rem_rec */

/*******************************************************************************
* mapotn_gen_db_update_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates a record for the specified database.
*   user must provide a full record for insertion.
*   In this case add and update record functions have the same functionality.

*
*
* INPUTS:
*   db_idx      - database index to access
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
PUBLIC PMC_ERROR mapotn_gen_db_update_rec(UINT32 db_idx, void *metadata, void *rec)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    mapotn_db_rec  *p_new_rec = (mapotn_db_rec*)rec;
    mapotn_db_key          key;

    /* The easiest way to do this and update all the key table (which might be
     * changing) is to just remove the old record and re-add the new one */
    if (p_new_rec != NULL && p_new_rec->calendar_entry < CALENDAR_ENTRIES_NUM)
    {
        key.calendar_entry = p_new_rec->calendar_entry;
        mapotn_gen_db_rem_rec(db_idx, metadata, (void *)&key);
        mapotn_gen_db_add_rec(db_idx, metadata, rec);
        rc = PMC_SUCCESS;
    }

    return(rc);
}/* mapotn_gen_db_update_rec */


/*******************************************************************************
* mapotn_gen_db_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function query a record or more from the specified database.
*   user must provide at least one valid key record record.
*   In this function we are using either the unique (1:1) key (calendar_entry)
*   or the (1:n) key (channel_num) in order to find the records to be to be
*   returned to the user.

*
* INPUTS:
*   db_idx      - database index to access
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
PUBLIC UINT32  mapotn_gen_db_query(UINT32 db_idx, void *metadata, void *key, void** records)
{
    mapotn_db_key  *p_key = (mapotn_db_key*)key;
    util_gen_db_key_table *match;
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    UINT32 max_num_channels;


    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);
    /*
    result = (mapotn_db_query_res*)*records;
    */
    match = NULL;
 
    max_num_channels = mapotn_db_max_channel_get[db_idx];

    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            match = &db_data->key_table[db_idx]->single_entries[p_key->calendar_entry];
        }
        else if (p_key->channel_num != DO_NOT_USE_KEY && p_key->channel_num < max_num_channels)
        {
            match = &db_data->key_table[db_idx]->channel_nums[p_key->channel_num];
        }
        else if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry == CALENDAR_ENTRIES_NUM)
        { /* special case to return the entire array contents for all calendar entries */
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
}/* mapotn_gen_db_query */

/*******************************************************************************
* mapotn_gen_db_query_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function query a record or more from the specified database.
*   user must provide at least one valid key record record.
*   In this function we are using either the unique (1:1) key (calendar_entry)
*   or the (1:n) key (channel_num) in order to find the records to be to be
*   returned to the user.

*
* INPUTS:
*   db_idx      - database index to access
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - number of records found;
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32  mapotn_gen_db_query_count(UINT32 db_idx, void *metadata, void *key)
{
    mapotn_db_key  *p_key = (mapotn_db_key*)key;
    mapotn_db_data   *db_data;
    util_gen_db_db_metadata_t *db_metadata;
    UINT32 rc = 0;
    UINT32 max_num_channels;


    db_metadata = (util_gen_db_db_metadata_t *)metadata;
    db_data = (mapotn_db_data *)(db_metadata->db_data);
 
    max_num_channels = mapotn_db_max_channel_get[db_idx];

    if (NULL != p_key)
    {
        if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry < CALENDAR_ENTRIES_NUM)
        {
            /* 1:1 match */
            rc = 1;
        }
        else if (p_key->channel_num != DO_NOT_USE_KEY && p_key->channel_num < max_num_channels)
        {
            rc = db_data->key_table[db_idx]->channel_nums[p_key->channel_num].q_size;

        }
        else if (p_key->calendar_entry != DO_NOT_USE_KEY && p_key->calendar_entry == CALENDAR_ENTRIES_NUM)
        { /* special case to return the entire array contents for all calendar entries */
            rc = CALENDAR_ENTRIES_NUM;
        }        
        else
        {
            rc = 0;
        }
    }

    return(rc);
}/* mapotn_gen_db_query */

/*******************************************************************************
* mapotn_db_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calls the mapotn_gen_db_init which creates and initializes the
*   ODUKSC database.
*
*
* INPUTS:
*   db_id       - enum, database id
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
UINT32 mapotn_db_init(util_gen_db_id_t db_id, void *metadata, void *not_used)
{
    UINT32 rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8  db_idx;

    PMC_ENTRY();

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_init(db_idx, metadata, NULL);
    }

    PMC_RETURN(rc);

} /* mapotn_db_init */


/*******************************************************************************
* mapotn_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function frees allocated memory for the ODUKSC database.
*
*
* INPUTS:
*   db_id       - database ID to access
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
UINT32 mapotn_db_destroy(util_gen_db_id_t db_id, void *metadata, void *not_used)
{
    UINT32 rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8  db_idx;

    PMC_ENTRY();

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_destroy(db_idx, metadata, NULL);
    }

    PMC_RETURN(rc);

} /* mapotn_db_destroy */


/*******************************************************************************
* mapotn_db_add_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function addes a record to the ODUKSC database.
*   user must provide a full record for insertion.

*
*
* INPUTS:
*   db_id       - database ID to access
*   metadata    - pointer to metadata for this DB.
*   rec         - void pointer, to casted to user defined record structure.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*   PMC_SUCCESS if successful
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
UINT32 mapotn_db_add_rec(util_gen_db_id_t db_id, void *metadata, void *rec)
{
    UINT32 rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8  db_idx;

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_add_rec(db_idx, metadata, rec);
    }

    return(rc);

} /* mapotn_db_add_rec */

/*******************************************************************************
* mapotn_db_rem_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function removes a record from the ODUKSC database.
*   user must provide at least one valid key record record.

*
*
* INPUTS:
*   db_id       - database ID to accesss
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
UINT32 mapotn_db_rem_rec(util_gen_db_id_t db_id, void *metadata, void *key)
{
    UINT32 rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8  db_idx;

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_rem_rec(db_idx, metadata, key);
    }

    return(rc);

} /* mapotn_db_rem_rec */


/*******************************************************************************
* mapotn_db_update_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates a record for the ODUKSC database.
*   user must provide a full record for insertion.
*   in this case add and update record functions have the same functionality.

*
*
* INPUTS:
*   db_id       - database ID to accesss
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
UINT32 mapotn_db_update_rec(util_gen_db_id_t db_id, void *metadata, void *rec)
{
    UINT32 rc = UTIL_GEN_DB_ERR_INVALID_PARAMETERS;
    UINT8  db_idx;

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_update_rec(db_idx, metadata, rec);
    }

    return(rc);

} /* mapotn_db_update_rec */


/*******************************************************************************
* mapotn_db_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function query a record or more from the ODUKSC database.
*   user must provide at least one valid key record record.

*
* INPUTS:
*   db_id       - database ID to accesss
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
UINT32  mapotn_db_query(util_gen_db_id_t db_id, void *metadata, void *key, void** records)
{
    UINT32 rc = 0;
    UINT8  db_idx;

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_query(db_idx, metadata, key, records);
    }

    return(rc);

} /* mapotn_db_query */




/*******************************************************************************
* mapotn_db_query_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function query a record or more from the ODUKSC database.
*   user must provide at least one valid key record record.

*
* INPUTS:
*   db_id       - database ID to accesss
*   metadata    - pointer to metadata for this DB.
*   key         - void pointer, to casted to user defined key structure.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - number of records found;
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
UINT32  mapotn_db_query_count(util_gen_db_id_t db_id, void *metadata, void *key)
{
    UINT32 rc = 0;
    UINT8  db_idx;

    db_idx = mapotn_db_idx_get[db_id];

    if (INVALID_DB_IDX != db_idx)
    {
        rc = mapotn_gen_db_query_count(db_idx, metadata, key);
    }

    return(rc);

} /* mapotn_db_query_count */




