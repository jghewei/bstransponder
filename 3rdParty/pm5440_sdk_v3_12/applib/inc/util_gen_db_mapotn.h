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
#ifndef _GEN_DB_MAPOTN_H_
#define _GEN_DB_MAPOTN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "util_gen_db.h"
#include "util_global.h"

/*
** Constants
*/

#ifndef CALENDAR_ENTRIES_NUM
#define CALENDAR_ENTRIES_NUM          (96)
#endif
/* definitons of mapotn database */
#define MAX_MAPOTN_CHANNELS                 (96)
/* definitons of DCPB database */
#define MAX_DCPB_DCS_0_CHANNELS             (150) /* set the #define to 150 */
#define MAX_DCPB_DCS_1_CHANNELS             (150)/* set the #define to 150 */
#define MAX_DCPB_DCS_2_CHANNELS             (224)/* set the #define to 224 */
#define MAX_DCPB_DCS_3_CHANNELS             (224)/* set the #define to 224 */
/* definitons of OCPB database */
#define MAX_OCPB_DCS_0_CHANNELS             (104)/* set the #define to 104 */
#define MAX_OCPB_DCS_1_CHANNELS             (104)/* set the #define to 104 */
#define MAX_OCPB_DCS_2_CHANNELS             (96)/* set the #define to 96 */
#define MAX_OCPB_DCS_3_CHANNELS             (96)/* set the #define to  96 */
/* definitons of ENET database */
#define MAX_ENET_CHANNELS                   (96)
#define MAX_MAPOTN_DB_CHANNEL_NUMS          (224)
/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: mapotn_db_rec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold mapotn db record.
*
* ELEMENTS:
*       channel_num     - channel number (0..95 - can be used as a key)
*       calendar_entry  - associated calender entries (0..95 - can be used as a key)
*       valid           - indicates whether this is a valid entry or not (TRUE =
*                         valid)
*
*******************************************************************************/
typedef struct
{
    UINT32 channel_num;
    UINT32 calendar_entry;
    BOOL8  valid;
} mapotn_db_rec;

/*******************************************************************************
* STRUCTURE: mapotn_db_key
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold mapotn db key.
*
* ELEMENTS:
*       channel_num      - channel number (0..95 - can be used as a key)
*       calendar_entry   - associated calender entries (0..95 - can be used as a key)
*
*******************************************************************************/
typedef struct
{
    UINT32 channel_num;
    UINT32 calendar_entry;

} mapotn_db_key;

/*******************************************************************************
* STRUCTURE: mapotn_db_query_res
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold mapotn db query result.  Note that this has the 
*   exact same form as the util_gen_db_key_table struct so we can cast one as 
*   the other easily.
*
* ELEMENTS:
*       calendar_entries- indices into the calendar for each of the recs.  This
*                         should have the same value as calendar_entry in the
*                         rec itself.
*       p_recs          - pointer to array of pointers to records found
*       num_of_recs     - number of records found
*
*******************************************************************************/
typedef struct
{
    mapotn_db_rec  *p_recs[CALENDAR_ENTRIES_NUM];
    UINT8           calendar_entries[CALENDAR_ENTRIES_NUM];
    UINT8           num_of_recs;
} mapotn_db_query_res;

/*
** Function Prototypes
*/
UINT32 mapotn_db_init              ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 mapotn_gen_db_reinit        ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 mapotn_db_destroy           ( util_gen_db_id_t db_id,void *metadata,void* ptr );
UINT32 mapotn_db_add_rec           ( util_gen_db_id_t db_id,void *metadata,void* rec );
UINT32 mapotn_db_rem_rec           ( util_gen_db_id_t db_id,void *metadata,void* key );
UINT32 mapotn_db_update_rec        ( util_gen_db_id_t db_id,void *metadata,void* rec );
UINT32 mapotn_db_query             ( util_gen_db_id_t db_id,void *metadata,void* key , void **records);
UINT32 mapotn_db_query_count       ( util_gen_db_id_t db_id,void *metadata,void* key );
PMC_ERROR mapotn_gen_all_db_destroy(util_gen_db_id_t db_id,void *metadata,void* not_used);



#ifdef __cplusplus
}
#endif

#endif /* _GEN_DB_INT_H_ */

