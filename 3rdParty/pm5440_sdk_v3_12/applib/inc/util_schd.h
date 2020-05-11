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

#ifndef _UTIL_SCHD_H
#define _UTIL_SCHD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_schd_api.h"
#include "util_gen_db.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: util_schd_variant_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Scheduler type definition.
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_SCHD_SCBS3              = 0,
    UTIL_SCHD_DCS                = 1
} util_schd_variant_t;

/*******************************************************************************
* ENUM: util_schd_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Channel state definition.
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_SCHD_CHNL_START         = 0,
    UTIL_SCHD_CHNL_EQUIPPED      = 1,
    UTIL_SCHD_CHNL_OPERATIONAL   = 2,
    UTIL_SCHD_CHNL_BACKUP        = 3,
    LAST_UTIL_SCHD_CHNL
} util_schd_chnl_state_t;

/*******************************************************************************
* ENUM: util_schd_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Channel rate units.
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_SCHD_1_BIT_SEC          = 0,
    UTIL_SCHD_10_BIT_SEC         = 1,
    UTIL_SCHD_100_BIT_SEC        = 2,
    UTIL_SCHD_1K_BIT_SEC         = 3,
    UTIL_SCHD_10K_BIT_SEC        = 4,
    UTIL_SCHD_100K_BIT_SEC       = 5,
    UTIL_SCHD_1M_BIT_SEC         = 6,
    UTIL_SCHD_10M_BIT_SEC        = 7,
    UTIL_SCHD_100M_BIT_SEC       = 8,
    UTIL_SCHD_1G_BIT_SEC         = 9,
    UTIL_SCHD_10G_BIT_SEC        = 10,
    UTIL_SCHD_100G_BIT_SEC       = 11,
    UTIL_SCHD_N_X_ODU0           = 12
} util_schd_chnl_rate_exp_t;

/*******************************************************************************
* ENUM: util_schd_math_division_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Math division types
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_SCHD_MATH_DIVISION_RAW       = 0,
    UTIL_SCHD_MATH_DIVISION_ROUND     = 1,
    UTIL_SCHD_MATH_DIVISION_CEILING   = 2,
    UTIL_SCHD_MATH_DIVISION_FLOOR     = 3
} util_schd_math_division_t;

/*******************************************************************************
* ENUM: util_schd_alloc_method_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Content allocation method.
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_SCHD_ALLOC_METHOD_UNUSED                      = 0,
    UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL               = 1,
    UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL               = 2,
    UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC               = 3,
    UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA                 = 4,
    UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO                 = 5,
    UTIL_SCHD_ALLOC_METHOD_CBRC                        = 6,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA                = 7,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN    = 8,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL       = 9,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER     = 10,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN      = 11,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1             = 12,
    UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2             = 13,
    UTIL_SCHD_ALLOC_METHOD_CPB_DCS                     = 14,
    UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB               = 15,
    UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB                = 16,
    UTIL_SCHD_ALLOC_METHOD_ENET                        = 17,
    UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB           = 18,
    UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB            = 19,

    LAST_UTIL_SCHD_ALLOC_METHOD

} util_schd_alloc_method_t;


/*
** Constants
*/

#define UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE                              0xFF
#define UTIL_SCHD_SCBS3_CAL_ENTRY_PER_ROW                             4
#define UTIL_SCHD_DCS_NULL_ENTRY_VALUE                                0xFF

#define UTIL_SCHD_INVALID_GROUP_ID                                    0xFF

/* currently we have 5 methods for calendar entries distribution */
/* any new method added needs to increment the nember below     */
#define MAX_CAL_DISTRIBUTION_METHODS        (LAST_UTIL_SCHD_ALLOC_METHOD)


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: util_schd_dcs_calendar_entry_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calendar entry struct.
*
* ELEMENTS:
*   c_ram_sf1           - push port id
*   c_ram_sf2           - wr_queue_id
*   c_ram_channel       - Channel Identifier of Calendar RAM address N.  This
*                           field is only used when C_RAM_GROUP_BIT is logic 0
*   c_ram_identifier    - Queue or Group Identifier of Calendar RAM address N.
*                           A value of 0xFF is a NULL identifier
*
*******************************************************************************/
typedef struct util_schd_dcs_calendar_entry_t
{ 
    UINT32 c_ram_sf1;
    UINT32 c_ram_sf2;
    UINT32 c_ram_channel;
    UINT32 c_ram_identifier;
} util_schd_dcs_calendar_entry_t; 

/*******************************************************************************
* STRUCTURE: util_schd_block_const_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Block context struct.  Contains fixed data on a scheduler instance.
*
* ELEMENTS:
*   base_chnl_num       - base channel number used as a scheduler entry 
*   num_schd_chnl       - number of channels used in scheduler
*   chnl_num_step       - address stepping between each channel number used in a
*                           scheduler:
*                           if incremental, chnl_num_step = 1
*                           if every other address, chnl_num_step = 2
*                           if every fourth address, chnl_num_step = 4
*   num_schd_grp        - number of groups contained in a scheduler
*   total_rate          - total interface rate = 
*                           total_rate * 10^(total_rate_units)
*   max_entries         - total number of entries available for scheduling
*   entry_null_valkue   - value used for scheduler entry that does not have a
*                           user channel populated
*   cal_entries_alloc_method - Calendar entries ts distribution algorithm number
*   db_id               - Database ID for the scheduler instance to access
*   max_dcs_rd_channel_id - Used to determine the size of the dcs_rd_channel_id
*                           lookup array.  This should be set to the largest
*                           dcs_rd_channel_id + 1.  Only needed for CPB.
*                           Should be 0 if you don't need it.
*
*******************************************************************************/
typedef struct util_schd_block_const_ctxt_t  
{
    UINT8                    base_chnl_num;
    UINT8                    num_schd_chnl;
    UINT8                    chnl_num_step;
    UINT8                    num_schd_grp;
    UINT32                   total_rate;
    util_schd_chnl_rate_exp_t   total_rate_units;
    UINT8                    max_entries;
    UINT8                    entry_null_value;
    UINT32                    cal_entries_alloc_method;
    util_gen_db_id_t          db_id;
    UINT32                   max_dcs_rd_channel_id;
} util_schd_block_const_ctxt_t;


/*******************************************************************************
* STRUCTURE: util_schd_grp_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Group context struct.
*
* ELEMENTS:
*   grp_id              - group ID
*   start_addr          - Start address
*   end_addr            - End address
*
*******************************************************************************/
typedef struct util_schd_grp_ctxt_t
{
    UINT32      grp_id;
    UINT32      start_addr;
    UINT32      end_addr;
} util_schd_grp_ctxt_t;

/*******************************************************************************
* STRUCTURE: util_schd_chnl_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel context struct.
*
* ELEMENTS:
*   chnl_id                 - channel id, channel number
*   grp_id                  - scheduler groups channel assigned to
*   min_entries             - minimum number of scheduler entries required by
*                               channel
*   max_entries             - maximum number of scheduler entries required by
*                               channel
*   assign_entries          - entries assigned to the channel
*   eval_entries            - temporary storage for entry requirement 
*                               calculation
*   dcs_rd_channel_id       - dcs read channel associated with the rd_queue_id
*   dcs_sf1_push_port_id    - dcs scpb push_port associated with the rd_queue_id
*   dcs_sf2_wr_port_id      - dcs wr_queue_id associated with the rd_queue_id
*   allocated_ho_channel    - allocated high order channel
*   inUseFlag               - 0 -- unused 1 -- used
*   chnl_type               -   0 -- basic channel
*                               1 -- MDRR channel
*   rate_units              - 1-^ exponent part of rate
*   state                   - channel state
*   rate                    - channel rate = rate * 10^rate_units
*   scbs_user_bits          - SCBS3 mode only.  Side band user bits that are
*                               output by the SCBS3 next to the channel being
*                               scheduled.
*   fo1_mpmo_dummy          - used to mark the calendar entry as a dummy
*
*******************************************************************************/
typedef struct util_schd_chnl_ctxt_t
{
    UINT8                     chnl_id;
    UINT8                     grp_id;
    UINT8                     min_entries;
    UINT8                     max_entries;
    UINT8                     assign_entries;
    UINT8                     eval_entries;
    UINT8                     dcs_rd_channel_id;
    UINT8                     dcs_sf1_push_port_id;
    UINT8                     dcs_sf2_wr_queue_id;
    UINT8                     allocated_ho_channel;
    BOOL8                     inUseFlag;
    BOOL8                     chnl_type;
    util_schd_chnl_rate_exp_t rate_units;
    util_schd_chnl_state_t    state;
    UINT32                    rate;
    UINT32                    scbs_user_bits;
    BOOL8                     fo1_mpmo_dummy;
} util_schd_chnl_ctxt_t;

/*******************************************************************************
* STRUCTURE: util_schd_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Scheduler context.
*
* ELEMENTS:
*   grp_ctxt                - pointer to the group contexts
*   chnl_ctxt               - pointer to the channel contexts
*   rd_queue_ids            - An array used to look up the rd_queu_id (index
*                               into chnl_ctxt) based on the dcs_rd_channel_id
*                               to avoid searching the chnl_ctxt every time.
*
*******************************************************************************/
typedef struct util_schd_ctxt_t
{
    util_schd_grp_ctxt_t *grp_ctxt;
    util_schd_chnl_ctxt_t *chnl_ctxt;
    UINT8                 *rd_queue_ids;

} util_schd_ctxt_t;



/*******************************************************************************
* STRUCTURE: util_schd_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Config struct for initial values.
*
* ELEMENTS:
*   block_ctxt              - Block context.
*   db_handle               - DB handle.
*
*******************************************************************************/
typedef struct util_schd_cfg_t {
    util_schd_block_const_ctxt_t    block_ctxt;
    UINT32                          db_handle;
} util_schd_cfg_t;


/*******************************************************************************
* STRUCTURE: util_schd_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Struct for dynamically changing context.
*
* ELEMENTS:
*   schd_ctxt                   - Scheduler context.
*   util_schd_mem_pool          - Memory pool used for temporary calculations.
*   util_schd_mem_pool_in_use   - If TRUE, the util_schd_mem_pool is being used.
*   restarted                   - If TRUE, the util_schd has been restarted.
*   use_custom_usebit_mask      - If TRUE, use the custom userbit mask.
*   custom_userbit              - The custom userbit
*   cal_pinch_type              - Calendar pinch type.
*
*******************************************************************************/
typedef struct util_schd_var_t {
    util_schd_ctxt_t        *schd_ctxt;
    util_schd_chnl_ctxt_t   *util_schd_mem_pool;
    BOOL8                   util_schd_mem_pool_in_use;    
    BOOL8                   restarted;
    BOOL8 use_custom_userbit_mask[96];
    UINT32 custom_userbit[3];
    util_global_calendar_pinch_t cal_pinch_type;
} util_schd_var_t;



/*******************************************************************************
* STRUCTURE: util_schd_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Scheduler handle containing the complete context for the scheduler.
*
* ELEMENTS:
*   base                        - see pmc_handle_t
*   cfg                         - see util_schd_cfg_t
*   var                         - see util_schd_var_t
*
*******************************************************************************/
struct util_schd_handle_struct_t {
    pmc_handle_t base;
    util_schd_cfg_t cfg;
    util_schd_var_t var;
};





/*
** Global variables
*/


/*it's required to pinch 29 out of the 96 entries (to leave 67 entries) */
#define UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI_NUM 29

#define     UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI { \
     0,  4,  7, 10, 14, 17, 20, 24, 27, 30, \
    34, 37, 40, 44, 47, 50, 53, 57, 60, 63, \
    67, 70, 73, 77, 80, 83, 87, 90, 93 \
} /* UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI */



/*it's required to pinch 20 out of the 96 entries (to leave 76 entries) */
#define UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI_NUM 20

#define     UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI { \
     2,  5, 11, 17, 23, 26, 29, 35, 41, 47, \
    50, 53, 59, 65, 71, 74, 77, 83, 89, 95 \
} /* UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI */



/*
** Function Prototypes
*/
PUBLIC util_schd_handle_t *util_schd_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void util_schd_ctxt_destroy(util_schd_handle_t *util_schd_handle);
PUBLIC void util_schd_handle_init(util_schd_handle_t *h, 
                                  util_schd_block_const_ctxt_t *const_ctxt);
PUBLIC void util_schd_alloc_init(util_schd_handle_t *h);
PUBLIC PMC_ERROR util_schd_handle_restart_init(util_schd_handle_t *h,
                                               util_gen_db_id_t db_id,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                               pmc_energy_state_t energy_state,
                                               BOOL8 ptr_init_only);
PUBLIC void util_schd_init(util_schd_handle_t *h, 
                           pmc_energy_state_t energy_state);
PUBLIC BOOL util_schd_start_state_test(util_schd_handle_t *h);
PUBLIC UINT32 util_schd_base_address_get(util_schd_handle_t *util_schd_handle);

PUBLIC PMC_ERROR util_schd_chnl_data_set(util_schd_handle_t *h, 
                                         UINT32 chnl_id,
                                         UINT32 grp_id,
                                         UINT32 chnl_rate,                                                 
                                         util_schd_chnl_rate_exp_t chnl_rate_units,
                                         UINT32 chnl_type,
                                         UINT32 dcs_rd_channel_id);
PUBLIC PMC_ERROR util_schd_dcs_chnl_data_set( util_schd_handle_t *h, 
                                              UINT32 chnl_id,
                                              UINT32 grp_id,
                                              UINT32 chnl_rate,                                                 
                                              util_schd_chnl_rate_exp_t chnl_rate_units,
                                              UINT32 chnl_type,
                                              UINT32 dcs_rd_channel_id,
                                              UINT32 dcs_sf1_push_port_id,
                                              UINT32 dcs_sf2_wr_queue_id );
PUBLIC PMC_ERROR util_schd_dcs_chnl_data_get( util_schd_handle_t *h,
                                              UINT32 chnl_id,
                                              util_schd_chnl_state_t *chnl_state_ptr,
                                              UINT32 *dcs_rd_channel_id_ptr,
                                              UINT32 *dcs_sf1_push_port_id_ptr,
                                              UINT32 *dcs_sf2_wr_queue_id_ptr );
PUBLIC PMC_ERROR util_schd_chnl_state_set(util_schd_handle_t *h, 
                                          UINT32 chnl_id,
                                          util_schd_chnl_state_t chnl_state);
PUBLIC PMC_ERROR util_schd_chnl_user_bits_set (util_schd_handle_t *h, 
                                               UINT32 chnl_id, UINT32 user_bits);

PUBLIC PMC_ERROR util_schd_grp_entry_eval(util_schd_handle_t *h,
                                          UINT32 grp_id,
                                          UINT32 entries);

PUBLIC PMC_ERROR util_schd_group_dist_gen(util_schd_handle_t *h,
                                          UINT32 grp_id,
                                          UINT32 num_dist_entries,
                                          UINT32 *dist_entries);

PUBLIC PMC_ERROR util_schd_scbs3_format_gen(util_schd_handle_t *h,
                                            UINT32 num_entries,
                                            UINT32 *dist_entries,
                                            UINT32 *scbs_entries);
PUBLIC PMC_ERROR util_schd_scbs3_user_bits_gen(util_schd_handle_t *h,
                                            UINT32 num_entries,
                                            UINT32 *scbs_entries,
                                            UINT32 *scbs_user_bits_entries);

PUBLIC PMC_ERROR util_schd_dcs_format_gen(util_schd_handle_t *h,
                                          UINT32 entries,
                                          UINT32 dist_entries[],
                                          util_schd_dcs_calendar_entry_t dcs_entries[]);

PUBLIC PMC_ERROR util_schd_chnl_state_get(util_schd_handle_t *h,
                                          UINT32 chnl_id,
                                          util_schd_chnl_state_t *chnl_state_ptr);

PUBLIC PMC_ERROR util_schd_custom_userbit_mask_set(util_schd_handle_t *h,
                                                   UINT32 chnl,
                                                   BOOL enable,
                                                   UINT32 set_mask[3],
                                                   UINT32 userbit_mask[3]);
                                          
PUBLIC PMC_ERROR util_schd_chnl_state_is_dummy(util_schd_handle_t *h,
                                               UINT32 chnl_id,
                                               BOOL8 *dummy_chnl_state);   
                                               
PUBLIC PMC_ERROR util_schd_chnl_state_dummy_set(util_schd_handle_t *h,
                                               UINT32 chnl_id,
                                               BOOL8 dummy_chnl_state);   
                                               
PUBLIC PMC_ERROR util_schd_chnl_dist_gen(util_schd_handle_t *h,
                                         UINT32 grp_id,
                                         UINT32 chnl_id,
                                         UINT32 num_dist_entries,
                                         UINT32 *dist_entries);                                                                                                                                  
PUBLIC PMC_ERROR util_schd_calendar_pinch_mode_set(util_schd_handle_t *h,
                                                   util_global_calendar_pinch_t cal_pinch_type);

PUBLIC PMC_ERROR util_schd_math_division(util_schd_math_division_t division_type,       
                                         UINT32 dividend_mantisa, INT32 dividend_exp,   
                                         UINT32 divisor_mantisa, INT32 divisor_exp,     
                                         UINT32 *quotient_mantisa, INT32 *quotient_exp);
PUBLIC PMC_ERROR util_schd_math_multiply(UINT32 multiplicand_mantisa, INT32 multiplicand_exp,
                                         UINT32 multiplier_mantisa, INT32 multiplier_exp,
                                         UINT32 *product_mantisa, INT32 *product_exp);

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_SCHD_H */

/*
** end of file
*/
