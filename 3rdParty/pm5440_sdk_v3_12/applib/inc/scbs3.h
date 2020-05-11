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

#ifndef _SCBS3_H
#define _SCBS3_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "scbs3_api.h"
#include "util_schd.h"
#include "util_schd_api.h"



/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: scbs3_page_type_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for standby or active memory.
*
* ELEMENTS:
*    SCBS3_STANDBY_MEM     - Standby memory
*    SCBS3_ACTIVE_MEM      - Active memory
*
*******************************************************************************/
typedef enum
{
    SCBS3_STANDBY_MEM = 0x0,
    SCBS3_ACTIVE_MEM = 0x1
} scbs3_page_type_enum_t;

/*******************************************************************************
* ENUM: scbs3_page_swap_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Page Swap mode.
*
* ELEMENTS:
*    SCBS3_PAGE_SWAP_EXTERNAL    - Page Swap with an external pin
*    SCBS3_PAGE_SWAP_BIT         - Page Swap using the register bit
*
*******************************************************************************/
typedef enum
{
    SCBS3_PAGE_SWAP_EXTERNAL = 0x0,
    SCBS3_PAGE_SWAP_BIT = 0x1
} scbs3_page_swap_mode_t;

/*
** Constants
*/
#define SCBS3_BASE_CHNL_NUMBER                 0
#define SCBS3_CHNL_NUMBER_STEP                 1
#define SCBS3_CAL_ENTRY_PER_ROW                4
#define SCBS3_NULL_CHNL_NUMBER                 0xFF
#define SCBS3_NULL_ROW_NUMBER                  0xFFFFFFFF
#define SCBS3_NULL_USERBITS_VAL                0

#define SCBS3_TOC_SYNC_CBRC                    0x1
#define SCBS3_TOC_SYNC_MAPOTN                  0x4
#define SCBS3_TOC_SYNC_ENET                    0xC

#define SCBS3_MAX_ROWS                         24

/* Bit offset of row number in start/end address in group context table */
#define SCBS3_GRP_ROW_OFFSET                   2

/* SCBS3 is a non-MDRR channel type */
#define SCBS3_UTIL_SCHD_CH_TYPE                0

/* SCBS3 only uses one group */
#define SCBS3_DEFAULT_GROUP_ID                 0

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: scbs3_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static SCBS3 module instance data
*
* ELEMENTS:
*   num_grp                   - number of groups
*   max_chnl                  - maximum number of channels in the context of a parent
*   max_rows                  - maximum number of rows in the calendar and userbits
*   total_rate                - total interface rate = total_rate * 10^(total_rate_units)
*   total_rate_units          - 10^ exponent part of total_rate
*   toc_sync_en               - TRUE: enable synchronization to the 'sch_toc_in' input port.
*                               FALSE: ignore the 'sch_toc_in' input port.
*   toc_sync                  - Entry that corresponds to the top of calendar entry\n
*                               toc_sync[6:2] defines the calendar memory row\n
*                               toc_sync[1:0] defines the entry within the row   
*   page_swap_mode            - Page swap with global pin or local register bit
*   bonus_mode_enabled        - semi-work conserving mode enabled
*   enable_row_resizing       - row resizing enabled
*   cal_entries_alloc_method  - Calendar entries distribution algorithm number
*   db_id                     - Database ID for the scheduler instance to access
*       
*******************************************************************************/
typedef struct scbs3_cfg_t {
    UINT32                    num_grp;     
    UINT32                    max_chnl;      
    UINT32                    max_rows;      
    UINT32                    total_rate;
    util_schd_chnl_rate_exp_t total_rate_units; 
    BOOL                      toc_sync_en; 
    UINT32                    toc_sync; 
    scbs3_page_swap_mode_t    page_swap_mode; 
    BOOL                      bonus_mode_enabled;     
    BOOL                      enable_row_resizing;
    UINT32                    cal_entries_alloc_method;
    util_gen_db_id_t          db_id; 
} scbs3_cfg_t;

/*******************************************************************************
* STRUCTURE: scbs3_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SCBS3 Module Instance dynamic data
*
* ELEMENTS:
*   standby_cal_mem_ptr      - Standby calendar pointer for offline memory
*   standby_userbits_mem_ptr - Standby userbits pointer for offline memory
*   active_cal_mem_ptr       - Active calendar pointer for offline memory
*   active_userbits_mem_ptr  - Active userbits pointer for offline memory
*       
*******************************************************************************/
typedef struct scbs3_var_t {  
    UINT32  *standby_cal_mem_ptr;
    UINT32  *standby_userbits_mem_ptr;
    UINT32  *active_cal_mem_ptr ; 
    UINT32  *active_userbits_mem_ptr ; 
} scbs3_var_t;

/*******************************************************************************
* STRUCTURE: scbs3_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SCBS3 Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See scbs3_cfg_t
*       var                   - See scbs3_var_t
*       *util_schd_handle     - See util_schd_handle_t
*       
*******************************************************************************/
typedef struct scbs3_handle_struct_t 
{
    pmc_handle_t base;
    scbs3_cfg_t cfg;
    scbs3_var_t var;
    util_schd_handle_t *util_schd_handle;    
} scbs3_handle_struct_t;

/*******************************************************************************
* STRUCTURE: scbs3_page_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SCBS3 page configuration parameters
*
* ELEMENTS:
*       num_entry    - Number of entries
*       calendar     - Calendar entries of the page
*       userbits     - User bits of specified page
*       
*******************************************************************************/
typedef struct scbs3_page_struct_t
{
    UINT32 num_entry;
    UINT32 calendar[SCBS3_MAX_ROWS*4];
    UINT32 userbits[SCBS3_MAX_ROWS*4];    
} scbs3_page_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC scbs3_handle_t *scbs3_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void scbs3_ctxt_destroy(scbs3_handle_t *scbs3_handle);
PUBLIC void scbs3_handle_init(scbs3_handle_t *scbs3_handle, 
                              scbs3_cfg_t *scbs_cfg_ptr);
PUBLIC void scbs3_alloc_init(scbs3_handle_t *h);
PUBLIC PMC_ERROR scbs3_handle_restart_init(scbs3_handle_t *h,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                           pmc_energy_state_t energy_state,
                                           BOOL8 ptr_init_only);

PUBLIC UINT32 scbs3_schd_init(scbs3_handle_t *scbs3_handle, 
                              pmc_energy_state_t energy_state);
PUBLIC BOOL scbs3_start_state_test(scbs3_handle_t *scbs3_handle);

PUBLIC PMC_ERROR scbs3_chnl_prov(scbs3_handle_t *scbs3_handle, UINT32 chnl, 
                                 UINT32 rate, util_schd_chnl_rate_exp_t rate_units,
                                 UINT32 user_bits);
PUBLIC PMC_ERROR scbs3_chnl_deprov(scbs3_handle_t *scbs3_handle, UINT32 chnl);
PUBLIC PMC_ERROR scbs3_chnl_activate(scbs3_handle_t *scbs3_handle, UINT32 chnl);
PUBLIC PMC_ERROR scbs3_chnl_deactivate(scbs3_handle_t *scbs3_handle, UINT32 chnl);
PUBLIC PMC_ERROR scbs3_chnl_reprov(scbs3_handle_t *scbs3_handle, UINT32 chnl, 
                                   UINT32 rate, util_schd_chnl_rate_exp_t rate_units);
PUBLIC void scbs3_chnl_clean(scbs3_handle_t *scbs3_handle, BOOL8 swap, UINT32 *chnl, UINT32 num_chnl);

PUBLIC PMC_ERROR scbs3_page_udpate_and_swap(scbs3_handle_t *scbs3_handle);
PUBLIC PMC_ERROR scbs3_standby_page_udpate(scbs3_handle_t *scbs3_handle);
PUBLIC PMC_ERROR scbs3_page_swap_req(scbs3_handle_t *scbs3_handle);
PUBLIC void scbs3_page_get(scbs3_handle_t *scbs3_handle,scbs3_page_type_enum_t page_type,scbs3_page_t *scbs3_page);

PUBLIC UINT32 scbs3_userbits_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                          scbs3_page_t *scbs3_page,
                                          UINT32 chnl);
PUBLIC UINT32 scbs3_num_entries_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                             scbs3_page_t *scbs3_page,
                                             UINT32 chnl);
PUBLIC UINT32 scbs3_first_calendar_entry_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                                      scbs3_page_t *scbs3_page,
                                                      UINT32 chnl);
PUBLIC util_schd_handle_t* scbs3_util_schd_handle_get(scbs3_handle_t *hndl); 

PUBLIC PMC_ERROR scbs3_set_custom_userbit_mask(scbs3_handle_t *scbs3_handle,
                                               UINT32 chnl_id,
                                               BOOL enable,
                                               UINT32 set_mask[3],
                                               UINT32 userbit_mask[3]);

PUBLIC UINT32 scbs3_set_custom_userbit_mask(scbs3_handle_t *scbs3_handle,
                                            UINT32 chnl_id,
                                            BOOL enable,
                                            UINT32 set_mask[3],
                                            UINT32 userbit_mask[3]);

PUBLIC PMC_ERROR scbs3_calendar_pinch_mode_set(scbs3_handle_t *scbs3_handle,
                                               util_global_calendar_pinch_t cal_pinch_type);                                     

#ifdef __cplusplus
}
#endif

#endif /* _SCBS3_H */

/*
** end of file
*/
