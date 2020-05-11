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
#include "util_gen_db.h"
#include "util_gen_db_mapotn.h"
#include "util_gen_db_line_core_otn.h"


/*
** Local variables
*/


/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char UTIL_GEN_DB_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    UTIL_GEN_DB_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char UTIL_GEN_DB_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */




/*
** Local Macro Definitions
*/
#define NO_INSTANCE_ID_FOUND    (0xFF)

/**
 * This is an index table that maps db id enumeration to
 * DB implementation template.
 * DB implementation template number is always less than or
 * equale to MAX_DB_NUM
*/
PRIVATE UINT32 db_template_idx[MAX_DB_NUM] =
{
    0,  /* UTIL_GEN_DB_ODUKSC_DB            */
    0,  /* UTIL_GEN_DB_MPMA_DB              */
    0,  /* UTIL_GEN_DB_MPMO_DB              */
    1,  /* UTIL_GEN_DB_LINE_CORE_OTN_RX_DB  */
    1,  /* UTIL_GEN_DB_LINE_CORE_OTN_TX_DB  */
    0,  /* UTIL_GEN_DB_DCPB_DCS_0           */
    0,  /* UTIL_GEN_DB_DCPB_DCS_1           */
    0,  /* UTIL_GEN_DB_DCPB_DCS_2           */
    0,  /* UTIL_GEN_DB_DCPB_DCS_3           */
    0,  /* UTIL_GEN_DB_OCPB_DCS_0           */
    0,  /* UTIL_GEN_DB_OCPB_DCS_1           */
    0,  /* UTIL_GEN_DB_OCPB_DCS_2           */
    0,  /* UTIL_GEN_DB_OCPB_DCS_3           */
    0,  /* UTIL_GEN_DB_CBRC_SCBS3           */
    0,  /* UTIL_GEN_DB_CBRC_MPMO            */
    0,  /* UTIL_GEN_DB_ENET_LINE_MPMA       */
    0,  /* UTIL_GEN_DB_ENET_SYS_MPMA        */
    0, /*UTIL_GEN_DB_LINEOTN_SYSOTN_MODE */
};


/** Array of function pointers to functions that initialize their database */
PRIVATE p_reg_func db_init[MAX_DB_TYPE]       = {mapotn_db_init,               line_core_otn_db_init};
/** Array of function pointers to functions that re-initialize their database */
PRIVATE p_reg_func db_reinit[MAX_DB_TYPE]     = {mapotn_gen_db_reinit,         line_core_otn_db_reinit};
/** Array of function pointers to functions that destroy their database */
PRIVATE p_reg_func db_destroy[MAX_DB_TYPE]    = {mapotn_db_destroy,            line_core_otn_db_destroy};
/** Array of function pointers to functions that destroy all their databases */
PRIVATE p_reg_func all_db_destroy[MAX_DB_TYPE]= {mapotn_gen_all_db_destroy,    NULL};
/** Array of function pointers to functions that add a record to their database */
PRIVATE p_reg_func db_add_rec[MAX_DB_TYPE]    = {mapotn_db_add_rec,            line_core_otn_db_add_rec};
/** Array of function pointers to functions that remove a record from their database */
PRIVATE p_reg_func db_rem_rec[MAX_DB_TYPE]    = {mapotn_db_rem_rec,            line_core_otn_db_rem_rec};
/** Array of function pointers to functions that update a record in their database */
PRIVATE p_reg_func db_update_rec[MAX_DB_TYPE] = {mapotn_db_update_rec,         line_core_otn_db_update_rec};
/** Array of function pointers to functions that query for records in their database */
PRIVATE p_rec_func db_query[MAX_DB_TYPE]      = {mapotn_db_query,              line_core_otn_db_query};
/** Array of function pointers to functions that query for the number of records in their database */
PRIVATE p_reg_func db_query_count[MAX_DB_TYPE]= {mapotn_db_query_count,        line_core_otn_db_query_count};
 
/* Forward references */
PRIVATE util_gen_db_db_metadata_t * util_gen_get_db_metadata_ptr(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id);


/*******************************************************************************
* gen_db_init_metadata
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This allocates the metadata and initializes the metadata pointer in the
*   given pmc_handle.  This should be done at the top of the pmc_handle tree,
*   before any other pmc_handles are allocated so it can be propagated through
*   pmc_handle_init() correctly.
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get at the pointer to the DB metadata for this
*   DIGI instance so we can set it.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   A side-effect of this function is that it sets the util_gen_db pointer in
*   the pmc_handle.
*******************************************************************************/
PUBLIC void gen_db_init_metadata(pmc_handle_t *pmc_handle)
{
    UINT8 i;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != pmc_handle, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    /* If util_gen_db is already set, assert */
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    PMC_ASSERT(NULL == common_info->util_gen_db, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    metadata = (util_gen_db_metadata_t *) PMC_CTXT_CALLOC(sizeof(util_gen_db_metadata_t), pmc_handle);
    common_info->util_gen_db = (void *) metadata;

    pmc_log_block_strings_register(UTIL_GEN_DB_LOG_ERR_STRINGS[0], UTIL_GEN_DB_LOG_ERR_TABLE_BASE, UTIL_GEN_DB_LOG_ERR_COUNT);

    /* Initialize.*/
    for (i=0; i < MAX_DB_NUM; i++)
    {
        metadata->attached_clients_bitmask[i] = 0;
    };
    for (i=0; i < MAX_DB_TYPE; i++)
    {
        metadata->db_metadata[i].template_id = i;
        metadata->db_metadata[i].pmc_handle = pmc_handle;
        PMC_CTXT_REGISTER_PTR(&metadata->db_metadata[i].pmc_handle, pmc_handle);
    }

    PMC_RETURN();
}


/*******************************************************************************
* gen_all_db_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function destroys all databases.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get at the pointer to the DB metadata for 
*   this DIGI instance so we can reinit it.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*******************************************************************************/
PUBLIC void gen_all_db_destroy(pmc_handle_t* pmc_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8  i;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ENTRY();

    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    if (NULL != metadata)
    {
        for (i=0; i < MAX_DB_TYPE && PMC_SUCCESS == rc; i++)
        {
            db_metadata = &metadata->db_metadata[i];
            if (all_db_destroy[i] != NULL)
            {
                /* call user function - destroy the db */
                rc = all_db_destroy[db_metadata->template_id]((util_gen_db_id_t)i, db_metadata, NULL);
            }
        }
    }

    PMC_RETURN();
}/* gen_all_db_destroy */

/*******************************************************************************
* gen_db_reinit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function re-allocates and reconstructs the key tables by calling each
*   of the database re-init functions.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get at the pointer to the DB metadata for 
*   this DIGI instance so we can reinit it.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*******************************************************************************/
PUBLIC PMC_ERROR gen_db_reinit(pmc_handle_t* pmc_handle)
{
    UINT8  i;
    PMC_ERROR rc = PMC_SUCCESS;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ENTRY();

    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    pmc_log_block_strings_register(UTIL_GEN_DB_LOG_ERR_STRINGS[0], UTIL_GEN_DB_LOG_ERR_TABLE_BASE, UTIL_GEN_DB_LOG_ERR_COUNT);

    for (i=0; i < MAX_DB_TYPE; i++)
    {
        if (PMC_SUCCESS == rc)
        {
            db_metadata = &metadata->db_metadata[i];
            PMC_ASSERT(NULL != db_reinit[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
            /* The first argument isn't actually used, but template_id is of the
             * correct type */
            rc = db_reinit[db_metadata->template_id]((util_gen_db_id_t)db_metadata->template_id, db_metadata, NULL);
        }
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* gen_db_attach
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of db attach function.
*   first user to call the gen_db_attach will creates and initializes the database.
*   Evry user that wants to use the DB must call this function to register a handle
*   to the db.
*   If everything goes well the the function will return a handle to the user
*   that will be used later in calling the other generic functions.
*   User must specify a device id and a database id to attach to.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*   db_id           - enum, requested db id
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   client_handle   - unsigned, upon successfull attachment a value different from
*                     0 (2^assigned bit). if unsuccessfull will return 0.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*******************************************************************************/
PUBLIC UINT32 gen_db_attach(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id)
{
    UINT32 client_handle = 0;
    UINT8  i;
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ENTRY();

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != db_init[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (0 == metadata->attached_clients_bitmask[db_id])
    {
        /* call user function - create the db for the first time */
        rc = db_init[db_metadata->template_id](db_id, db_metadata, NULL);
    }
    else
    {
        /* db already created by someone else */
        rc = PMC_SUCCESS;
    }

    if (PMC_SUCCESS == rc )
    {
        /* go over bit mask - find first free entry */
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            if (0 == (metadata->attached_clients_bitmask[db_id] & (1<<i)))
            {
                break;
            }
        }

        PMC_ASSERT(i < MAX_CLIENTS, UTIL_GEN_DB_ERR_CLIENT_BITMASK_FULL, 0, 0);

        /* attach to user handle */
        client_handle = 1 << i;

        metadata->attached_clients_bitmask[db_id] |= client_handle;

    }

    PMC_RETURN(client_handle);
}/* gen_db_attach */

/*******************************************************************************
* gen_db_detach
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of db detach function.
*   last user to call the gen_db_detach will free database allocated memory.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   If everything goes well the the function will return a zerod handle to the user
*   User must specify a device id and a database id to detach from.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   client_handle   - unsigned, upon successfull detachment 0 value will be returned,
*                     other wise the none 0 client handle wil be returned.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*******************************************************************************/
PUBLIC UINT32 gen_db_detach(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle)
{
    UINT8 i;
    UINT8 bit_count = 0;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ENTRY();

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    /*
     * If the client handle is 0 then there is nothing to do so leave 
     * immediately.
     */
    if (0 == client_handle)
    {
        PMC_RETURN(client_handle);
    }

    PMC_ASSERT(NULL != db_destroy[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    /* check that client handle is 1 bit */
    for (i=0; i < MAX_CLIENTS; i++)
    {
        if (client_handle & (1<<i))
        {
            bit_count++;
        }
    }

    /* if incorrect handle - do nothing, return handle to sender */
    if (bit_count > 1)
    {
        PMC_RETURN (client_handle);
    }

    /* handle is OK - remove client from the DB */
    metadata->attached_clients_bitmask[db_id] &= (~client_handle);
    client_handle = PMC_SUCCESS; /* should return as PMC_SUCCESS */

    /* No more clients attached - destroy DB */
    if (0 == metadata->attached_clients_bitmask[db_id])
    {
        /* call user function - destroy the db */
        client_handle = db_destroy[db_metadata->template_id](db_id, db_metadata, NULL);
    }

    /* if all is OK - should be 0 */
    PMC_RETURN(client_handle);

}/* gen_db_detach */

/*******************************************************************************
* gen_db_add_record
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of add record function.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   User should provide a user defined record pointer casted to void pointer in order to
*   forward the user record to the user implementation of the add record function.
*   record content, structure, usage and memory allocation are user defined.
*
*   User must specify a device id and a database id.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*
*   record          - void pointer, user record pointer casted to void.
*                     this pointer should point to valid user record data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC PMC_ERROR gen_db_add_record(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *record)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != record, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != db_add_rec[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (metadata->attached_clients_bitmask[db_id] & client_handle) /* only registered clients can operate on DB */
    {
        rc = db_add_rec[db_metadata->template_id](db_id, db_metadata, record);
    }

    return(rc);
}/* gen_db_add_record */

/*******************************************************************************
* gen_db_rem_record
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of remove record function.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   User should provide a user defined key pointer casted to void pointer in order to
*   forward it to the user implementation of the remove record function.
*   key content, structure, usage and memory allocation are user defined.
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*
*   key             - void pointer, user key pointer casted to void.
*                     this pointer should point to valid user key data by which
*                     the user implementation will look for the records to be removed.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC PMC_ERROR gen_db_rem_record(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != key, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != db_rem_rec[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (metadata->attached_clients_bitmask[db_id] & client_handle)
    {
        rc = db_rem_rec[db_metadata->template_id](db_id, db_metadata, key);
    }

    return(rc);
}/* gen_db_rem_record */

/*******************************************************************************
* gen_db_update_record
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of update record function.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   User should provide a user defined record pointer casted to void pointer in order to
*   forward the user record to the user implementation of the update record function.
*   record content, structure, usage and memory allocation are user defined.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*
*   record          - void pointer, user record pointer casted to void.
*                     this pointer should point to valid user record data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC PMC_ERROR gen_db_update_record(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *record)
{
    PMC_ERROR rc = UTIL_GEN_DB_ERR_FAIL;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != record, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != db_update_rec[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (metadata->attached_clients_bitmask[db_id] & client_handle)
    {
        /* call user function - update record */
        rc = db_update_rec[db_metadata->template_id](db_id, db_metadata, record);
    }

    return(rc);
}/* gen_db_update_record */

/*******************************************************************************
* gen_db_query
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of a query function.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   User should provide a user defined key pointer casted to void pointer in order to
*   forward it to the user implementation of the query function.
*   key content, structure, usage and memory allocation are user defined.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*
*   key             - void pointer, user key pointer casted to void.
*                     this pointer should point to valid user key data by which
*                     the user implementation will look for the requested records.
*
*
* OUTPUTS:
*   records         - void pointer, a pointer to query result records will be 
*                     returned in this pointer.
*
* RETURNS:
*   UINT32 - the number of records that match the key.
*   if an error occured or no records found, this function will return 0.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*   3. This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC UINT32 gen_db_query(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key, void **records)
{
    UINT32 num_of_found_rec = 0;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != key, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != db_query[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (metadata->attached_clients_bitmask[db_id] & client_handle)
    {
        num_of_found_rec = db_query[db_metadata->template_id](db_id, db_metadata, key, records) ;
    }

    return(num_of_found_rec);
} /* gen_db_query */

/*******************************************************************************
* gen_db_query_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a generic implemetation of a query count function.
*
*   User must be registered to the db with a valid handle in order to use this function.
*   User should provide a user defined key pointer casted to void pointer in order to
*   forward it to the user implementation of the query function.
*   key content, structure, usage and memory allocation are user defined.
*
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*
*   db_id           - enum, requested db id
*
*   client_handle   - unsigned, valid handle recieved on attach
*   key             - void pointer, user key pointer casted to void.
*                     this pointer should point to valid user key data by which
*                     the user implementation will look for the requested records.
*
*
*
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - the number of records that match the key.
*   if an error occured or no records found, this function will return 0.
*
* NOTES:
*   1. This generic db supports up to 32 clients.
*   2. Only one client can write to the db - there is no syncronization of client
*      writes !!!
*   3. This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC UINT32 gen_db_query_count(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key)
{
    UINT32 num_of_found_rec = 0;
    util_gen_db_db_metadata_t * db_metadata;
    util_gen_db_metadata_t * metadata;
    pmc_handle_common_info_t *common_info;

    db_metadata = util_gen_get_db_metadata_ptr(pmc_handle, db_id);
    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);

    PMC_ASSERT(NULL != key, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != db_query_count[db_metadata->template_id], UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    if (metadata->attached_clients_bitmask[db_id] & client_handle)
    {
        num_of_found_rec = db_query_count[db_metadata->template_id](db_id, db_metadata, key) ;
    }

    return(num_of_found_rec);
} /* gen_db_query_count */


/*******************************************************************************
* util_gen_get_db_metadata_ptr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Does all the typecasting and redirection required to get at the DB metadata
*   pointer for the given db_id.
*
* INPUTS:
*   pmc_handle      - pointer to a pmc_handle that contains a pointer to the
*   context that we can then use to get a pointer to the DB metadata for this
*   DIGI instance so we can use it.
*   db_id           - enum, requested db id
*
* OUTPUTS:
*   None
*
* RETURNS:
*   util_gen_db_db_metadata_t - pointer to the DB metadata for the given db_id.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PRIVATE util_gen_db_db_metadata_t *util_gen_get_db_metadata_ptr(pmc_handle_t *pmc_handle, util_gen_db_id_t db_id)
{
    UINT8   template_id = 0;
    util_gen_db_metadata_t *metadata;
    util_gen_db_db_metadata_t *db_metadata;
    pmc_handle_common_info_t *common_info;

    PMC_ASSERT(db_id < LAST_UTIL_GEN_DB, UTIL_GEN_DB_ERR_INVALID_PARAMETERS, db_id , 0);
    template_id = db_template_idx[db_id];

    PMC_ASSERT(template_id < MAX_DB_TYPE, UTIL_GEN_DB_ERR_INVALID_PARAMETERS, template_id, 0);

    PMC_ASSERT(NULL != pmc_handle, UTIL_GEN_DB_ERR_INVALID_PTR, 0, 0);

    common_info = (pmc_handle_common_info_t *)(pmc_handle->common_info);
    metadata = (util_gen_db_metadata_t *)(common_info->util_gen_db);
    db_metadata = &metadata->db_metadata[template_id];

    return(db_metadata);

} /* util_gen_get_metadata_ptr */


/*******************************************************************************
* util_gen_db_add_to_key_table
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Adds a new calendary entry into the key table, inserting it such that the 
*   table is kept in order of calendar_entry.
*
* INPUTS:
*   key_table        - pointer to the key table to add the entry to.
*   calendar_entry   - The calendar entry to add.
*   rec              - pointer to the record being added.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Nothing
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*******************************************************************************/
PUBLIC void util_gen_db_add_to_key_table(util_gen_db_key_table *key_table, UINT8 calendar_entry, void *rec)
{
    UINT8 i;
    UINT8 insert_position;
    BOOL8 already_exists = FALSE;


    /* By default, insert at the end */
    insert_position = key_table->q_size;

    /* First, check if the entry already exists */
    if (key_table->q_size > 0)
    {
        for (i = 0; i < key_table->q_size; i++)
        {
            if (key_table->q_table[i] == calendar_entry)
            {
                already_exists = TRUE;
            }
            if (key_table->q_table[i] > calendar_entry)
            {
                insert_position = i;
                break;
            }
        }
    }

    /* Add the entry */
    if (FALSE == already_exists)
    {
        key_table->q_size++;
        /* Insert the new entry at the proper position */

        /* First, move everything after the insert position */
        for (i = key_table->q_size - 1; i > insert_position; i--)
        {
            key_table->q_table[i] = key_table->q_table[i-1];
            key_table->q_recs[i]  = key_table->q_recs[i-1];
        }

        /* Then, insert */
        key_table->q_table[insert_position] = calendar_entry;
        key_table->q_recs[insert_position] = rec;
    }

}

/*******************************************************************************
* util_gen_db_rm_from_key_table
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Removes a calendary entry from the key table.
*
* INPUTS:
*   key_table        - pointer to the key table to remove the entry from.
*   calendar_entry   - The calendar entry to remove.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Nothing
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN due to its high call
*    rate.
*
*******************************************************************************/
PUBLIC void util_gen_db_rm_from_key_table(util_gen_db_key_table *key_table, UINT8 calendar_entry)
{
    UINT8 i;
    UINT8 j = 0;

    if (key_table->q_size > 0)
    {
        for (i = 0; i < key_table->q_size; i++)
        {
            if (key_table->q_table[i] != calendar_entry)
            {
                key_table->q_table[j] = key_table->q_table[i];
                key_table->q_recs[j] = key_table->q_recs[i];
                j++;
            }
        }
        key_table->q_size = j;
    }
}







