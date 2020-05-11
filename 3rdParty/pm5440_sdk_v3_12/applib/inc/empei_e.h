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

#ifndef _EMPEI_E_H
#define _EMPEI_E_H

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
#include "empei_e_api.h"
#include "eclass.h"
#include "eclass_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: empei_e_pmon_type_enum_t
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
    EMPEI_E_PMON_COUNTER_TYPE
} empei_e_pmon_type_enum_t;

/*******************************************************************************
* ENUM: empei_e_status_t
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
    EMPEI_E_EXT_Q_OVERFLOW_INT_I
} empei_e_status_t;

/*******************************************************************************
* ENUM: empei_e_ts_back_to_back_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  PTP Timestamp Back to Back Control Mechanism
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum
{
    EMPEI_E_TS_CONSUME,
    EMPEI_E_TS_TIMER
} empei_e_ts_back_to_back_type_t;

/*******************************************************************************
* ENUM: empei_e_datapath_type_enum_t
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
    EMPEI_E_TRANSPARENT_DATAPATH = 0x0,
    EMPEI_E_MAC_DATAPATH = 0x1
} empei_e_datapath_type_enum_t;

/*******************************************************************************
* ENUM: empei_e_data_rate_enum_t
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
    EMPEI_E_LINK_10G = 0x0,
    EMPEI_E_LINK_40G = 0x1,
    EMPEI_E_LINK_100G = 0x2
} empei_e_data_rate_enum_t;

/*
** Constants
*/
#define BASE_ADDR_EMPEI_E_ECLASS               0x00004000

#define EMPEI_E_NUM_LINKS    12
#define EMPEI_E_DATA_CPB_CHANNELS (EMPEI_E_NUM_LINKS * 8)
#define EMPEI_E_MGMT_CPB_CHANNELS (EMPEI_E_NUM_LINKS * 4)
#define EMPEI_E_NUM_TS  12

/* Insert Queue Manager Min Slots Available Threshold(Egress Traffic Manager) */
#define EMPEI_E_INS_QM_MIN_SLOT_THRESH_10G     1
#define EMPEI_E_INS_QM_MIN_SLOT_THRESH_40G     8
#define EMPEI_E_INS_QM_MIN_SLOT_THRESH_100G    16

/* 40G valid links */
#define EMPEI_E_40G_LINK_0    0
#define EMPEI_E_40G_LINK_4    4
#define EMPEI_E_40G_LINK_8    8

/* Extract PMONs */
#define EMPEI_E_NUM_EXT_PMON 64

/* CPB flow control (priority 0 only) */
#define EMPEI_E_CPB_PFC_0     1

/*
** Macro Definitions
*/

/* EMPEI_E_INT(handle, tsb_name, tsb_field, struct_field) */
#define EMPEI_E_INT_TABLE_DEFINE() \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, SYS_Q_OVERFLOW_INT , sys_q_overflow_int_i ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, INTL_Q_OVERFLOW_INT, intl_q_overflow_int_i); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, SYS_Q_UNDERRUN_INT , sys_q_underrun_int_i ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, INS_Q_UNDERRUN_INT , ins_q_underrun_int_i ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, SYS_Q_RESYNC_INT   , sys_q_resync_int_i   ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, INS_Q_RESYNC_INT   , ins_q_resync_int_i   ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, EXT_Q_RESYNC_INT   , ext_q_resync_int_i   ); \
    EMPEI_E_INT(empei_e_handle, empei_e, field_range, INTL_Q_RESYNC_INT  , intl_q_resync_int_i  ); \

#define EMPEI_E_STATUS_TABLE_DEFINE() \
    EMPEI_E_STATUS(empei_e_handle, empei_e, field_range, SYS_Q_OVERFLOW_INT_V , sys_q_overflow_int_v ); \
    EMPEI_E_STATUS(empei_e_handle, empei_e, field_range, INTL_Q_OVERFLOW_INT_V, intl_q_overflow_int_v); \
    EMPEI_E_STATUS(empei_e_handle, empei_e, field_range, SYS_Q_UNDERRUN_INT_V , sys_q_underrun_int_v ); \
    EMPEI_E_STATUS(empei_e_handle, empei_e, field_range, INS_Q_UNDERRUN_INT_V , ins_q_underrun_int_v ); \

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: empei_e_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {

    /*    UINT32 num_empei_e_chans; */

} empei_e_cfg_t;

/*******************************************************************************
* STRUCTURE: empei_e_var_t
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
     * Extract PMON:
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
    UINT8 ext_pmon_link_map[EMPEI_E_NUM_EXT_PMON];
    UINT8 ext_pmon_tag_map[EMPEI_E_NUM_EXT_PMON];
    UINT32 ext_pmon[EMPEI_E_NUM_EXT_PMON];
    UINT8 ext_pmon_link_tag_map[EMPEI_E_NUM_EXT_PMON];
} empei_e_var_t;

/*******************************************************************************
* STRUCTURE: empei_e_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the empei_e context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   eclass_handle - ECLASS handle 
*******************************************************************************/
typedef struct empei_e_handle_struct_t {
    pmc_handle_t base;

    empei_e_cfg_t cfg;
    empei_e_var_t var;

    eclass_handle_t  *eclass_handle;

} empei_e_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC empei_e_handle_t *empei_e_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void empei_e_ctxt_destroy(empei_e_handle_t *empei_e_handle);
PUBLIC void empei_e_handle_init(empei_e_handle_t *empei_e_handle);
PUBLIC PMC_ERROR empei_e_handle_restart_init(empei_e_handle_t *empei_e_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                             pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR empei_e_init(empei_e_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL8 empei_e_start_state_test(empei_e_handle_t *hndl);
PUBLIC UINT32 empei_e_base_address_get(empei_e_handle_t *empei_e_handle);

PUBLIC PMC_ERROR empei_e_reset_eclass_nreg(empei_e_handle_t *hndl, BOOL8 reset);
PUBLIC PMC_ERROR empei_e_reset_eclass_core(empei_e_handle_t *hndl, BOOL8 reset);
PUBLIC PMC_ERROR empei_e_activate_datapath(empei_e_handle_t *hndl);
PUBLIC PMC_ERROR empei_e_datapath_sel_link_cfg(empei_e_handle_t *hndl, UINT32 link, empei_e_datapath_type_enum_t datapath);
PUBLIC PMC_ERROR empei_e_datapath_sel_cfg(empei_e_handle_t *hndl, UINT32 datapaths);
PUBLIC PMC_ERROR empei_e_gfp_enable(empei_e_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC BOOL8  empei_e_gfp_enable_get(empei_e_handle_t *hndl, 
                                    UINT32            link);
PUBLIC PMC_ERROR empei_e_ts_back_to_back_link_cfg(empei_e_handle_t *hndl, UINT32 link,
                                             empei_e_ts_back_to_back_type_t ts_back_to_back_type,
                                             UINT32 ts_timer);
PUBLIC PMC_ERROR empei_e_ts_back_to_back_cfg(empei_e_handle_t *hndl,
                                             UINT32 ts_consume_enables,
                                             UINT32 ts_timer_enables,
                                             UINT32 ts_timers[EMPEI_E_NUM_LINKS]);
PUBLIC PMC_ERROR empei_e_etrans_len_err_timer_link_cfg(empei_e_handle_t *hndl, UINT32 link,
                                                  UINT32 len_err_tmr);
PUBLIC PMC_ERROR empei_e_etrans_len_err_timer_cfg(empei_e_handle_t *hndl, UINT32 len_err_tmrs[EMPEI_E_NUM_LINKS]);
PUBLIC PMC_ERROR empei_e_cpb_link_sch_cfg(empei_e_handle_t *hndl, UINT32 last_tslot, UINT32 offset_extract, BOOL8 update_sch, UINT32 link[EMPEI_E_NUM_TS]);
PUBLIC PMC_ERROR empei_e_internal_link_sch_cfg(empei_e_handle_t *hndl, UINT32 last_tslot, BOOL8 update_sch, UINT32 link[EMPEI_E_NUM_TS]);

PUBLIC BOOL8 empei_e_sys_fcs_is_enabled(empei_e_handle_t *hndl, 
                                       UINT32 link);

PUBLIC PMC_ERROR empei_e_sys_fcs_enable(empei_e_handle_t *hndl, UINT32 link, BOOL8 enable);

PUBLIC PMC_ERROR empei_e_data_cpb_flow_ctrl_link_cfg(empei_e_handle_t *hndl, UINT32 link, BOOL8             enable);
PUBLIC PMC_ERROR empei_e_pmon_get(empei_e_handle_t *hndl, UINT32 link, empei_e_pmon_type_enum_t count_type,
                                  UINT32 *count_val_ptr);

PUBLIC void empei_e_drr_quantum_get(empei_e_handle_t *hndl, 
                                    UINT32 link, 
                                    UINT32* sys_quantum, 
                                    UINT32* ins_quantum);

PUBLIC PMC_ERROR empei_e_drr_quantum_cfg(empei_e_handle_t *hndl, UINT32 link, UINT32 sys_quantum, UINT32 ins_quantum);
PUBLIC void empei_e_link_reset(empei_e_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR empei_e_link_reset_duration_cfg(empei_e_handle_t *hndl, UINT32 duration);
PUBLIC PMC_ERROR empei_e_link_enable(empei_e_handle_t *hndl, UINT32 link, BOOL8 enable); 
PUBLIC PMC_ERROR empei_e_ext_pmon_cfg(empei_e_handle_t *hndl, BOOL8 enables[EMPEI_E_NUM_EXT_PMON], UINT32 pmon_cfg[EMPEI_E_NUM_EXT_PMON]);
PUBLIC PMC_ERROR empei_e_da_sa_swap_cfg(empei_e_handle_t *hndl, UINT32 link, BOOL8 enable );
PUBLIC void empei_e_eclass_energy_state_reg_set ( empei_e_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR empei_e_ext_pmon_enable(empei_e_handle_t *hndl, UINT32 link, UINT32 tag);
PUBLIC PMC_ERROR empei_e_ext_pmon_disable(empei_e_handle_t *hndl, UINT32 link, UINT32 tag);
PUBLIC PMC_ERROR empei_e_ext_pmon_disable_all(empei_e_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR empei_e_ext_pmon_get(empei_e_handle_t *hndl, 
                                       UINT32 link,
                                       BOOL8 accumulate,
                                       UINT32 **counters_ptr,
                                       UINT8 **tags_ptr);
PUBLIC PMC_ERROR empei_e_pmon_latch(empei_e_handle_t *hndl);

PUBLIC PMC_ERROR empei_e_datapath_cfg(empei_e_handle_t *hndl, UINT32 link, 
                                      empei_e_data_rate_enum_t data_rate,
                                      empei_e_datapath_type_enum_t datapath, 
                                      BOOL8 enable);

PUBLIC PMC_ERROR empei_e_pkt_gen_cfg(empei_e_handle_t *hndl, UINT32 pkt_size, 
                                     UINT32 pyld_val, UINT32 pkt_interval,
                                     UINT32 da_hi, UINT32 da_low, 
                                     UINT32 sa_hi, UINT32 sa_low, 
                                     UINT32 len_etype, 
                                     UINT32 vlan_out, UINT32 vlan_in);

PUBLIC PMC_ERROR empei_e_pkt_gen_cfg_get(empei_e_handle_t *hndl,
                                         UINT32 *pkt_size,
                                         UINT32 *pyld_val,
                                         UINT32 *pkt_interval,
                                         UINT32 *da_hi,
                                         UINT32 *da_low,
                                         UINT32 *sa_hi,
                                         UINT32 *sa_low,
                                         UINT32 *len_etype,
                                         UINT32 *vlan_out,
                                         UINT32 *vlan_in);

PUBLIC PMC_ERROR empei_e_pkt_gen_enable(empei_e_handle_t *hndl, UINT32 link, 
                                        BOOL8 enable);
PUBLIC PMC_ERROR empei_e_pkt_gen_status_get(empei_e_handle_t *hndl, 
                                        UINT32 *link_no_ptr, 
                                        BOOL8 *enabled_ptr);
PUBLIC void empei_e_activated_link_get(empei_e_handle_t   *hndl,
                                       BOOL8               *actv_link);

PUBLIC void empei_e_cleanup(empei_e_handle_t *hndl, 
                            UINT32            num_links, 
                            UINT32           *links);

PUBLIC PMC_ERROR empei_e_int_chnl_enable(empei_e_handle_t    *empei_e_handle,
                                         UINT32               link,
                                         empei_e_int_chnl_t  *int_table_ptr,
                                         empei_e_int_chnl_t  *int_en_table_ptr,
                                         BOOL8                 enable);

PUBLIC PMC_ERROR empei_e_int_chnl_clear(empei_e_handle_t    *empei_e_handle,
                                        UINT32               link,
                                        empei_e_int_chnl_t  *int_table_ptr,
                                        empei_e_int_chnl_t  *int_en_table_ptr);

PUBLIC PMC_ERROR empei_e_int_chnl_retrieve(empei_e_handle_t    *empei_e_handle,
                                           UINT32               link,
                                           empei_e_int_chnl_t  *filt_table_ptr,
                                           empei_e_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR empei_e_int_chnl_enabled_check(empei_e_handle_t     *empei_e_handle,
                                                UINT32                link,
                                                empei_e_int_chnl_t   *int_en_table_ptr,
                                                BOOL8                 *int_found_ptr);


#ifdef __cplusplus
}
#endif

#endif /* _EMPEI_E_H */

/*
** end of file
*/
