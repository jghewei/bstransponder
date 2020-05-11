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
#ifndef _GEN_DB_LINE_CORE_OTN_H_
#define _GEN_DB_LINE_CORE_OTN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "util_gen_db.h"

/*
** Constants
*/

/* definitons of line_core_otn database */
#ifndef CALENDAR_ENTRIES_NUM
#define CALENDAR_ENTRIES_NUM          (96)
#endif
#define LINEOTN_HO_CHANNLES_NUM       (12)
#define COREOTN_MO_CHANNLES_NUM       (96)
#define COREOTN_LO_CHANNLES_NUM       (96)
#define COREOTN_TRIB_SLOTS_NUM        (96)
#define DO_NOT_USE_KEY                (0xffffffff)

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: line_core_otn_db_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold line_core_otn db record.
*
* ELEMENTS:
*  calendar_entry  - associated calender entries (0..95 - can be used as a key)
*  cal_entry_status- indicates whether or not the calendar slot is active or idle (0,1).
*  trib_slot       - Tributary slot number as defined in G.709. Valid range: 1 - 80.
*                    The maximum value allowed will be determined by the HO ODU container
*                    size and the tributary slot type it carries. (1.25G TS vs. 2.5G TS)
*                    The value will be filled in the same sequence as the calendar slot
*                    assignment.
*  ho_channel      - high order cahnnel (0..11 - can be used as a key)
*  ts_port_ho_mo   - Tributary port number of ODTU payload when a MO or a LO channel is added to
*                    the active HO or MO channel. This is supplied by MSI data. This can be the same
*                    as the tributary slot - 1 for fixed assignment or as user defined if flexible
*                    assignment is allowed.
*  mo_channel      - MO channel ID as supplied by the user.
*  ts_port_mo_lo   - Refer to TS port # (HO-MO)
*  lo_channel      - LO channel ID as supplied by the user
*  valid           - indicates whether this is a valid entry or not (TRUE =
*                    valid)
*
*******************************************************************************/
typedef struct
{
    UINT32 calendar_entry;
    UINT32 cal_entry_status;
    UINT32 trib_slot;
    UINT32 ho_channel;
    UINT32 ts_port_ho_mo;
    UINT32 mo_channel;
    UINT32 ts_port_mo_lo;
    UINT32 lo_channel;
    BOOL8  valid;
} line_core_otn_db_rec;

/*******************************************************************************
* STRUCTURE: lineotn_db_key
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold line_core_otn db key.
*
* ELEMENTS:
*       calendar_entry  - associated calender entries (0..95 - can be used as a key)
*       ho_channel      - high order channel (0..11 - can be used as a key)
*       mo_channel      - middle order channel (0..47 - can be used as a key)
*       lo_channel      - low order channel (0..95 - can be used as a key)
*       trib_slot       - tributary slot ID (0..95 - can be used as a key)
*******************************************************************************/
typedef struct
{
    UINT32 calendar_entry;
    UINT32 ho_channel;
    UINT32 mo_channel;
    UINT32 lo_channel;
    UINT32 trib_slot;
}line_core_otn_db_key;

/*******************************************************************************
* STRUCTURE: line_core_otn_db_query_res
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold line_core_otn db query result.  Note that this has the 
*   exact same form as the util_gen_db_key_table struct so we can cast one as 
*   the other easily.
*
* ELEMENTS:
*       p_recs          - pointer to array of pointers to records found
*       calendar_entries- indices into the calendar for each of the recs.  This
*                         should have the same value as calendar_entry in the
*                         rec itself.
*       num_of_recs     - number of records found
*
*******************************************************************************/
typedef struct
{
    line_core_otn_db_rec   *p_recs[CALENDAR_ENTRIES_NUM];
    UINT8                   calendar_entries[CALENDAR_ENTRIES_NUM];
    UINT8                   num_of_recs;
}line_core_otn_db_query_res;

/*
** Function Prototypes
*/
UINT32 line_core_otn_db_init              ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 line_core_otn_db_reinit            ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 line_core_otn_db_destroy           ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 line_core_otn_db_add_rec           ( util_gen_db_id_t db_id,void *metadata,void* rec );
UINT32 line_core_otn_db_rem_rec           ( util_gen_db_id_t db_id,void *metadata,void* key );
UINT32 line_core_otn_db_update_rec        ( util_gen_db_id_t db_id,void *metadata,void* rec );
UINT32 line_core_otn_db_query             ( util_gen_db_id_t db_id,void *metadata,void* key , void **records);
UINT32 line_core_otn_db_query_count       ( util_gen_db_id_t db_id,void *metadata,void* key);

#ifdef __cplusplus
}
#endif

#endif /* _GEN_DB_LINE_CORE_OTN_H_ */

