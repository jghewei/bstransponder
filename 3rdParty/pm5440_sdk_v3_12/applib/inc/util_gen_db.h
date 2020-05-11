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
#ifndef _GEN_DB_H_
#define _GEN_DB_H_

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_ctxt_mgmt.h"
#include "util_global.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
** Structures and Unions - this enum is required for all specific db implemetations
*/


/* 
** Enum
*/ 
/*******************************************************************************
* ENUM: util_gen_db_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to hold db id's. Any new DB should define an ID here.
*
* ELEMENTS:
*       UTIL_GEN_DB_ODUKSC_DB               - ODUK Scheduler DB ID     
*       UTIL_GEN_DB_MPMA_DB                 - MPMA DB ID
*       UTIL_GEN_DB_MPMO_DB                 - MPMO DB ID 
*       UTIL_GEN_DB_LINE_CORE_OTN_RX_DB     - LINE/CORE OTN DB RX ID
*       UTIL_GEN_DB_LINE_CORE_OTN_RX_DB     - LINE/CORE OTN DB TX ID
*       UTIL_GEN_DB_DCPB_DCS_0              - DCBP DCS 0 DB ID 
*       UTIL_GEN_DB_DCPB_DCS_1              - DCBP DCS 1 DB ID 
*       UTIL_GEN_DB_DCPB_DCS_2              - DCBP DCS 2 DB ID 
*       UTIL_GEN_DB_DCPB_DCS_3              - DCBP DCS 3 DB ID 
*       UTIL_GEN_DB_OCPB_DCS_0              - OCBP DCS 0 DB ID 
*       UTIL_GEN_DB_OCPB_DCS_1              - OCBP DCS 1 DB ID 
*       UTIL_GEN_DB_OCPB_DCS_2              - OCBP DCS 2 DB ID 
*       UTIL_GEN_DB_OCPB_DCS_3              - OCBP DCS 3 DB ID 
*       UTIL_GEN_DB_CBRC_SCBS3              - CBRC SBCS3 DB ID 
*       UTIL_GEN_DB_CBRC_MPMO               - CBRC MPMO DB ID
*       UTIL_GEN_DB_ENET_LINE_MPMA          - ENET LINE MPMA DB ID
*       UTIL_GEN_DB_ENET_SYS_MPMA           - ENET SYS MPMA DB ID
*       UTIL_GEN_DB_LINEOTN_SYSOTN_MODE     - LINE/COREOTN DB in SYSOTN application
*       UTIL_GEN_DB_UNUSED                  - marks an un used DB ID 
*       LAST_UTIL_GEN_DB                    - for error handling
*******************************************************************************/ 
typedef enum
{
    UTIL_GEN_DB_ODUKSC_DB           = 0,
    UTIL_GEN_DB_MPMA_DB             = 1,
    UTIL_GEN_DB_MPMO_DB             = 2,
    UTIL_GEN_DB_LINE_CORE_OTN_RX_DB = 3,
    UTIL_GEN_DB_LINE_CORE_OTN_TX_DB = 4,
    UTIL_GEN_DB_DCPB_DCS_0          = 5,
    UTIL_GEN_DB_DCPB_DCS_1          = 6,
    UTIL_GEN_DB_DCPB_DCS_2          = 7,
    UTIL_GEN_DB_DCPB_DCS_3          = 8,
    UTIL_GEN_DB_OCPB_DCS_0          = 9,
    UTIL_GEN_DB_OCPB_DCS_1          = 10,
    UTIL_GEN_DB_OCPB_DCS_2          = 11,
    UTIL_GEN_DB_OCPB_DCS_3          = 12,
    UTIL_GEN_DB_CBRC_SCBS3          = 13,
    UTIL_GEN_DB_CBRC_MPMO           = 14,
    UTIL_GEN_DB_ENET_LINE_MPMA      = 15,
    UTIL_GEN_DB_ENET_SYS_MPMA       = 16,
    UTIL_GEN_DB_LINEOTN_SYSOTN_MODE = 17,
    UTIL_GEN_DB_UNUSED              = 20,
    LAST_UTIL_GEN_DB
}util_gen_db_id_t;

/* These are here because the database-specific include files need the
 * definition of util_gen_db_id_t
 */
#include "util_gen_db_mapotn.h"
#include "util_gen_db_line_core_otn.h"

/*******************************************************************************
* ENUM: util_gen_db_accessor_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to hold db accessors. Any new DB accessor should define an ID here.
*
* ELEMENTS:
*       UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN               - coreotn mpma accessor id
*       UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN   - coreotn from mapotn mpma accessor id
*       UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_CTL           - coreotn control mpma accessor id  
*       UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER    - coreotn mpmo mux framer accessor id  
*       UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN     - coreotn to mapotn mpmo accessor id
*       UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1            - coreotn oduk scheduler1 accessor id
*       UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2            - coreotn oduk scheduler2 accessor id    
*       UTIL_GEN_DB_ACCESSOR_ODUKSC                     - oduk scheduler accessor id
*       UTIL_GEN_DB_ACCESSOR_MPMA                       - mpma accessor id          
*       UTIL_GEN_DB_ACCESSOR_MPMO                       - mpmo accessor id
*       UTIL_GEN_DB_ACCESSOR_CPB_DCS                    - cpb dcs accessor id
*       UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3                 - cbrc scbs3 accessor id
*       UTIL_GEN_DB_ACCESSOR_CBRC_MPMO                  - cbrc mpmo accessor id
*       UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA             - enet line mpma accessor id
*       UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA              - enet sys mpma accessor id 
*       LAST_UTIL_GEN_DB_ACCESSOR                       - for error handling   
*******************************************************************************/

typedef enum
{
    UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN             = 0,
    UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN,
    UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL,
    UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER,
    UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN,
    UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1,
    UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2,
    UTIL_GEN_DB_ACCESSOR_ODUKSC,
    UTIL_GEN_DB_ACCESSOR_MPMA,
    UTIL_GEN_DB_ACCESSOR_MPMO,
    UTIL_GEN_DB_ACCESSOR_CPB_DCS,
    UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3,
    UTIL_GEN_DB_ACCESSOR_CBRC_MPMO,
    UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA,
    UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA, 
    
    LAST_UTIL_GEN_DB_ACCESSOR
}util_gen_db_accessor_inst_t;



/*
** Constants
*/
#define MAX_DB_NUM              (LAST_UTIL_GEN_DB)
/* line_core_otn and mapotn are the 2 database types */
#define MAX_DB_TYPE             (2)
#define MAX_CLIENTS             (32)

/* common constants for all db's */
#define CALENDAR_ENTRIES_NUM          (96)
#define DO_NOT_USE_KEY                (0xffffffff)
#define INVALID_DB_IDX                (0xff)

/*
** Function pointers
*/

/** Function pointer definition for the query function that returns records */
typedef UINT32  (*p_rec_func)(util_gen_db_id_t,void *, void*, void**);
/** Function pointer definition for the functions that do not return records */
typedef UINT32  (*p_reg_func)(util_gen_db_id_t,void *, void* );


/*
** Function Prototypes
*/
PUBLIC void      gen_db_init_metadata (pmc_handle_t *pmc_handle);
PUBLIC void      gen_all_db_destroy   (pmc_handle_t* pmc_handle);
PUBLIC PMC_ERROR gen_db_reinit        (pmc_handle_t *pmc_handle);
PUBLIC UINT32    gen_db_attach        (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id);
PUBLIC UINT32    gen_db_detach        (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle);
PUBLIC PMC_ERROR gen_db_add_record    (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *record);
PUBLIC PMC_ERROR gen_db_rem_record    (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key);
PUBLIC PMC_ERROR gen_db_update_record (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *record);
PUBLIC UINT32    gen_db_query         (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key, void **records);
PUBLIC UINT32    gen_db_query_count   (pmc_handle_t* pmc_handle, util_gen_db_id_t db_id, UINT32 client_handle, void *key);

/*
** Log Messaging
*/

/*
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums
 */
#define UTIL_GEN_DB_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GEN_DB_ERR_INVALID_PARAMETERS,   "UTIL_GEN_DB: Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GEN_DB_ERR_FAIL,                 "UTIL_GEN_DB: Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GEN_DB_ERR_INVALID_PTR,          "UTIL_GEN_DB: Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GEN_DB_ERR_CLIENT_BITMASK_FULL,  "UTIL_GEN_DB: The maximum number of clients have been attached to this database" )


/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum UTIL_GEN_DB_LOG_ERR_INDEX
{
    UTIL_GEN_DB_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_UTIL_GEN_DB << 16),
    UTIL_GEN_DB_LOG_ERR_TABLE
    UTIL_GEN_DB_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char UTIL_GEN_DB_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char UTIL_GEN_DB_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

/*******************************************************************************
* STRUCTURE: util_gen_db_db_metadata_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the metadata used for one database.  Each instance of 
*   DIGI requires an array of size MAX_DB_TYPE of these structs to handle all 
*   the database types required per DIGI.
*
* ELEMENTS:
*   template_id                 - the template ID for this database, as defined
*                                 by the template ID table.
*   pmc_handle                  - A pointer to a pmc_handle.  This is required 
*                                   for using the context memory.
*******************************************************************************/
typedef struct util_gen_db_db_metadata_t {
    UINT8                       template_id;
    void                        *db_data;
    pmc_handle_t                *pmc_handle;
} util_gen_db_db_metadata_t;



/*******************************************************************************
* STRUCTURE: util_gen_db_metadata
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the metadata used for all the databases.  Each instance of 
*   DIGI requires one of these structs to handle all the data for all the
*   databases required per DIGI.
*
* ELEMENTS:
*   attached_clients_bitmask    - attached clients bit mask - 32 bits per db.
*                                   Note that this one is per DB, not per DB 
*                                   type.  The rest of the fields are per DB 
*                                   type.
*   db_metadata                 - metadata per DB type.
*******************************************************************************/
typedef struct util_gen_db_metadata_t {
    UINT32                      attached_clients_bitmask[MAX_DB_NUM];
    util_gen_db_db_metadata_t   db_metadata[MAX_DB_TYPE];
} util_gen_db_metadata_t;


/*******************************************************************************
* STRUCTURE: util_gen_db_key_table
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold pre-computed query results.
*
* ELEMENTS:
*   q_recs     - pointers to the records for this query result, in the same
*                   order as q_table.
*   q_table    - indices into the calendar table for this query result.
*   q_size     - The number of valid entries in the table.
*******************************************************************************/
typedef struct util_gen_db_key_table {
    void*   q_recs[CALENDAR_ENTRIES_NUM];
    UINT8   q_table[CALENDAR_ENTRIES_NUM];
    UINT8   q_size;
} util_gen_db_key_table;

PUBLIC void util_gen_db_add_to_key_table(util_gen_db_key_table *key_table, UINT8 calendar_entry, void* rec);
PUBLIC void util_gen_db_rm_from_key_table(util_gen_db_key_table *key_table, UINT8 calendar_entry);




#ifdef __cplusplus
}
#endif

#endif /* _GEN_DB_H_ */
