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

#ifndef _EMPEI_I_H
#define _EMPEI_I_H

#ifdef __cplusplus
extern "C" { 
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "empei_i_api.h"
#include "eclass.h"
#include "eclass_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: empei_i_pmon_type_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  PMON counter type
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum
{
    EMPEI_I_PMON_COUNTER_TYPE
} empei_i_pmon_type_enum_t;

/*******************************************************************************
* ENUM: empei_i_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Status type
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum
{
    EMPEI_I_EXT_Q_OVERFLOW_INT_I
} empei_i_status_t;


/*******************************************************************************
* ENUM: empei_i_datapath_type_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Datapath type
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum 
{
    EMPEI_I_MAC_DATAPATH = 0x0,
    EMPEI_I_TRANSPARENT_DATAPATH = 0x1
} empei_i_datapath_type_enum_t;

/*******************************************************************************
* ENUM: empei_i_data_rate_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Data rate 
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum 
{
    EMPEI_I_LINK_10G = 0x0,
    EMPEI_I_LINK_40G = 0x1,
    EMPEI_I_LINK_100G = 0x2
} empei_i_data_rate_enum_t;

/*
** Constants
*/
#define BASE_ADDR_EMPEI_I_ECLASS               0x00004000

#define EMPEI_I_NUM_LINKS    12
/* #define EMPEI_I_NUM_CHANNELS 96 */
#define EMPEI_I_NUM_PRIORITY 8
#define EMPEI_I_NUM_CPB_CHANNELS (EMPEI_I_NUM_LINKS * EMPEI_I_NUM_PRIORITY)
#define EMPEI_I_NUM_TS 12

/* Insert Queue Manager Min Slots Available Threshold(Ingress Traffic Manager) */
#define EMPEI_I_INS_QM_MIN_SLOT_THRESH_10G     1
#define EMPEI_I_INS_QM_MIN_SLOT_THRESH_40G     9
#define EMPEI_I_INS_QM_MIN_SLOT_THRESH_100G    16

/* Level Monitor Threshold (Ingress Traffic Manager) */
#define EMPEI_I_LVL_MON_THRESH_10G     5
#define EMPEI_I_LVL_MON_THRESH_40G     5
#define EMPEI_I_LVL_MON_THRESH_100G    5

/* 40G valid links */
#define EMPEI_I_40G_LINK_0    0
#define EMPEI_I_40G_LINK_4    4
#define EMPEI_I_40G_LINK_8    8

/* Extract PMONs */
#define EMPEI_I_NUM_EXT_PMON 64

/* CPB flow control (priority 0 only) */
#define EMPEI_I_CPB_PFC_0     1

/*
** Macro Definitions
*/
/* EMPEI_I_INT(handle, tsb_name, tsb_field, struct_field) */
#define EMPEI_I_INT_TABLE_DEFINE() \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, INS_Q_OVERFLOW_INT , ins_q_overflow_int_i ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, EXT_Q_OVERFLOW_INT , ext_q_overflow_int_i ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, SYS_Q_OVERFLOW_INT , sys_q_overflow_int_i ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, INTL_Q_OVERFLOW_INT, intl_q_overflow_int_i); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, SYS_Q_UNDERRUN_INT , sys_q_underrun_int_i ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, INS_Q_UNDERRUN_INT , ins_q_underrun_int_i ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, SYS_Q_RESYNC_INT   , sys_q_resync_int_i   ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, INS_Q_RESYNC_INT   , ins_q_resync_int_i   ); \
    EMPEI_I_INT(empei_i_handle, empei_i, field_range, EXT_Q_RESYNC_INT   , ext_q_resync_int_i   ); \

#define EMPEI_I_STATUS_TABLE_DEFINE() \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, INS_Q_OVERFLOW_INT_V , ins_q_overflow_int_v ); \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, EXT_Q_OVERFLOW_INT_V , ext_q_overflow_int_v ); \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, SYS_Q_OVERFLOW_INT_V , sys_q_overflow_int_v ); \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, INTL_Q_OVERFLOW_INT_V, intl_q_overflow_int_v); \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, SYS_Q_UNDERRUN_INT_V , sys_q_underrun_int_v ); \
    EMPEI_I_STATUS(empei_i_handle, empei_i, field_range, INS_Q_UNDERRUN_INT_V , ins_q_underrun_int_v ); \

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: empei_i_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {

/*    UINT32 num_empei_i_chans; */

} empei_i_cfg_t;


/*******************************************************************************
* STRUCTURE: empei_i_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*   ext_pmon_link_map - Link map
*   ext_pmon_tag_map - Tap map
*   ext_pmon - PMON
*   ext_pmon_link_tag_map - link tag map
*
*******************************************************************************/
typedef struct {

    /* 
     * Extract PMONs:
     *      There are 64 extract PMONs  - each can be configured to count
     *      management frames for a particular link & classification code.
     *      The ext_pmon_link_map and ext_pmon_tag_map arrays map counters
     *      to a link & classification code.  If 'counter' is enabled 
     *      for 'link' and 'tag', then: 
     *          ext_pmon_link_map[counter] = link;
     *          ext_pmon_tag_map[counter] = tag;
     *      If a counter is disabled, the entry for both the link and 
     *      tag will be 0. The counter value is stored in ext_pmon[counter].
     *      ext_pmon_link_tag_map is used to return a list of
     *      active classification codes for a particular link.
     *      Note: separate arrays are used so that that the arrays
     *      can be passed to the user.
     */
    UINT8 ext_pmon_link_map[EMPEI_I_NUM_EXT_PMON];
    UINT8 ext_pmon_tag_map[EMPEI_I_NUM_EXT_PMON];
    UINT32 ext_pmon[EMPEI_I_NUM_EXT_PMON];
    UINT8 ext_pmon_link_tag_map[EMPEI_I_NUM_EXT_PMON];
} empei_i_var_t;

/*******************************************************************************
* STRUCTURE: empei_i_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the empei_i context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   eclass_handle - ECLASS handle 
*******************************************************************************/
typedef struct empei_i_handle_struct_t {
    pmc_handle_t base;

    empei_i_cfg_t cfg;
    empei_i_var_t var;

    eclass_handle_t  *eclass_handle;

} empei_i_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC empei_i_handle_t *empei_i_ctxt_create(pmc_handle_t *parent, 
                                             UINT32 base_address,  
                                             pmc_sys_handle_t *sys_handle,  
                                             const char *tsb_name);

PUBLIC void empei_i_ctxt_destroy(empei_i_handle_t *empei_i_handle);

PUBLIC void empei_i_handle_init(empei_i_handle_t *empei_i_handle);
PUBLIC PMC_ERROR empei_i_handle_restart_init(empei_i_handle_t *empei_i_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                             pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR empei_i_init(empei_i_handle_t *hndl,
                              pmc_energy_state_t energy_state);

PUBLIC BOOL8 empei_i_start_state_test(empei_i_handle_t *hndl);

PUBLIC UINT32 empei_i_base_address_get(empei_i_handle_t *empei_i_handle);

PUBLIC PMC_ERROR empei_i_reset_eclass_nreg(empei_i_handle_t *hndl,
                                           BOOL8 reset);

PUBLIC PMC_ERROR empei_i_reset_eclass_core(empei_i_handle_t *hndl, 
                                           BOOL8 reset);

PUBLIC PMC_ERROR empei_i_activate_datapath(empei_i_handle_t *hndl);

PUBLIC PMC_ERROR empei_i_datapath_sel_cfg(empei_i_handle_t *hndl, 
                                          UINT32 link, 
                                          empei_i_datapath_type_enum_t datapath);

PUBLIC PMC_ERROR empei_i_cpb_link_sch_cfg(empei_i_handle_t *hndl, 
                                          UINT32 last_tslot, 
                                          UINT32 offset_extract, 
                                          BOOL8 update_sch, 
                                          UINT32 link[EMPEI_I_NUM_TS]);

PUBLIC PMC_ERROR empei_i_internal_link_sch_cfg(empei_i_handle_t *hndl, 
                                               UINT32 last_tslot, 
                                               BOOL8 update_sch, 
                                               UINT32 link[EMPEI_I_NUM_TS]);

PUBLIC PMC_ERROR empei_i_chan_map_cfg(empei_i_handle_t *hndl, 
                                      UINT32 link, 
                                      UINT32 dest_chnls[EMPEI_I_NUM_PRIORITY]);

PUBLIC PMC_ERROR empei_i_wrr_priority_cfg(empei_i_handle_t *hndl, 
                                          BOOL8 clienta_weight[EMPEI_I_NUM_LINKS], 
                                          BOOL8 clientb_weight[EMPEI_I_NUM_LINKS]);

PUBLIC PMC_ERROR empei_i_data_cpb_flow_ctrl_cfg(empei_i_handle_t *hndl, 
                                                UINT32 link, 
                                                BOOL8 enable,
                                                
                                                UINT32 cpb_chnl,
                                                UINT32 zone_thres, 
                                                UINT32 priority_bit_map);

PUBLIC PMC_ERROR empei_i_data_cpb_zone_cfg(empei_i_handle_t *hndl, 
                                           UINT32 zone_enables, 
                                           UINT32 thresholds[EMPEI_I_NUM_CPB_CHANNELS], 
                                           UINT32 priority_maps[EMPEI_I_NUM_CPB_CHANNELS]);

PUBLIC PMC_ERROR empei_i_enable_cpb_flow_ctrl_cfg(empei_i_handle_t *hndl, 
                                                  UINT32 link,
                                                  UINT32 sys_zone_thresh);

PUBLIC PMC_ERROR empei_i_cpb_flow_ctrl_enable(empei_i_handle_t *hndl, 
                                              UINT32 link, 
                                              BOOL8 enable);
PUBLIC BOOL8 empei_i_cpb_flow_ctrl_enable_get(empei_i_handle_t *hndl, 
                                             UINT32 link);
PUBLIC PMC_ERROR empei_i_cpb_link_type_cfg(empei_i_handle_t *hndl,
                                           UINT32 link_types[EMPEI_I_NUM_LINKS]);

PUBLIC PMC_ERROR empei_i_pmon_get(empei_i_handle_t *hndl, 
                                  UINT32 link, 
                                  empei_i_pmon_type_enum_t count_type,
                                  UINT32 *count_val_ptr);

PUBLIC PMC_ERROR empei_i_link_reset(empei_i_handle_t *hndl, 
                                    UINT32 link);

PUBLIC PMC_ERROR empei_i_link_reset_duration_cfg(empei_i_handle_t *hndl, 
                                                 UINT32 duration);

PUBLIC PMC_ERROR empei_i_link_enable(empei_i_handle_t *hndl, 
                                     UINT32 link, BOOL8 enable); 

PUBLIC PMC_ERROR empei_i_ext_pmon_cfg(empei_i_handle_t *hndl,
                                      BOOL8 enables[EMPEI_I_NUM_EXT_PMON], 
                                      UINT32 pmon_cfg[EMPEI_I_NUM_EXT_PMON]);

PUBLIC void empei_i_block_non_default_cfg(empei_i_handle_t *hndl);


PUBLIC void empei_i_eclass_energy_state_reg_set ( empei_i_handle_t *hndl, 
                                                  pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR empei_i_int_chnl_enable(empei_i_handle_t    *empei_i_handle,
                                         UINT32               link,
                                         empei_i_int_chnl_t  *int_table_ptr,
                                         empei_i_int_chnl_t  *int_en_table_ptr,
                                         BOOL8                 enable);

PUBLIC PMC_ERROR empei_i_int_chnl_clear(empei_i_handle_t    *empei_i_handle,
                                        UINT32               link,
                                        empei_i_int_chnl_t  *int_table_ptr,
                                        empei_i_int_chnl_t  *int_en_table_ptr);

PUBLIC PMC_ERROR empei_i_int_chnl_retrieve(empei_i_handle_t    *empei_i_handle,
                                           UINT32               link,
                                           empei_i_int_chnl_t  *filt_table_ptr,
                                           empei_i_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR empei_i_int_chnl_enabled_check(empei_i_handle_t     *empei_i_handle,
                                                UINT32                link,
                                                empei_i_int_chnl_t   *int_en_table_ptr,
                                                BOOL8                 *int_found_ptr);

/* For Simulation only */
#if defined(PMC_SW_SIMULATION) | defined (PMC_VERIFICATION) | defined (PMC_EMULATION)
PUBLIC void empei_i_simulation_setup(empei_i_handle_t *hndl);
#endif

/* Test functions */
PUBLIC PMC_ERROR empei_i_ext_pmon_enable(empei_i_handle_t *hndl, UINT32 link, UINT32 tag);
PUBLIC PMC_ERROR empei_i_ext_pmon_disable(empei_i_handle_t *hndl, UINT32 link, UINT32 tag);
PUBLIC PMC_ERROR empei_i_ext_pmon_disable_all(empei_i_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR empei_i_ext_pmon_get(empei_i_handle_t *hndl, 
                                       UINT32 link,
                                       BOOL8 accumulate,
                                       UINT32 **counters_ptr,
                                       UINT8 **tags_ptr);
PUBLIC PMC_ERROR empei_i_pmon_latch(empei_i_handle_t *hndl);

PUBLIC PMC_ERROR empei_i_datapath_cfg(empei_i_handle_t *hndl, UINT32 link, empei_i_data_rate_enum_t data_rate,
                                      empei_i_datapath_type_enum_t datapath, BOOL8 enable);
PUBLIC UINT32 empei_i_intr_enable_set(empei_i_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC void empei_i_activated_link_get(empei_i_handle_t   *hndl,
                                       BOOL8              *actv_link);


PUBLIC void empei_i_cleanup(empei_i_handle_t *hndl, 
                            UINT32            num_links, 
                            UINT32           *links);

#ifdef __cplusplus
}
#endif

#endif /* _EMPEI_I_H */

/*
** end of file
*/
