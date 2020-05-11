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

#ifndef _MGMT_FEGE_H
#define _MGMT_FEGE_H

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
#include "mgmt_fege_api.h"
#include "enet_fege.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: mgmt_fege_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify MGMT_FEGE state range of values.
*
* ELEMENTS:
*   MGMT_FEGE_STATE_START         - start state
*   MGMT_FEGE_STATE_EQUIPPED      - provisioned state
*   MGMT_FEGE_STATE_OPERATIONAL   - activated and operational state
*           
*******************************************************************************/
typedef enum mgmt_fege_state_t
{
    MGMT_FEGE_STATE_START        = 0,
    MGMT_FEGE_STATE_EQUIPPED     = 1,
    MGMT_FEGE_STATE_OPERATIONAL  = 2,
    LAST_MGMT_FEGE_STATE
} mgmt_fege_state_t;

/*
** Constants
*/
/* Maximum number of entries in MGMT_FEGE in VLAN replacement table */
#define MGMT_FEGE_VLAN_REPL_TABLE_SIZE 64




/*
** Macro Definitions
*/
#define MGMT_FEGE_INT_TABLE_DEFINE() \
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, TMVP_FIFO_OVERFLOW, tmvp_fifo_overflow_i);\
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, TMVP_INVALID_DPI_ID, tmvp_invalid_dpi_id_i);\
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, TMVP_INVALID_TYPE, tmvp_invalid_type_i);\
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, RMVP_FIFO_OVERFLOW, rmvp_fifo_overflow_i);\
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, RMVP_INVALID_VLAN_ID, rmvp_invalid_vlan_id_i);\
    MGMT_FEGE_INT(mgmt_fege_handle, mgmt_fege, RMVP_INVALID_TYPE, rmvp_invalid_type_i);\


/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: mgmt_fege_fwd_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining packet forwarding configuration.
*
* ELEMENTS:
*   dpi_id         - DPI ID associated dorwarding entry.
*   ins_vlan_id    - TRUE when packet VLAN ID is repalced by the given VLAN_ID.
*   vlan_entry     - Depending on the forwarding path:
*                    MGMT_FEGE -> ENET: it is the VLAN entry used to find dpi_id.
*                    ENET -> MGMT_FEGE: it specifies the inserted VLAN tag.
*
*******************************************************************************/
typedef struct mgmt_fege_fwd_cfg_t
{    
    UINT16 dpi_id;
    BOOL8 ins_vlan_id;
    util_global_vlan_cfg_t vlan_entry;
} mgmt_fege_fwd_cfg_t;
    

/*******************************************************************************
* STRUCTURE: mgmt_fege_fwd_tbl_entry_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DPI_ID/VLAN ID forwarding table entry.
*
* ELEMENTS:
*   valid         - define entry status: valid(TRUE) or invalid (FALSE)
*   is_vlan        - TRUE when packet 
*   vlan_match     - vlan matching configuration associated to forwarding table entry.
*
*******************************************************************************/
typedef struct mgmt_fege_fwd_tbl_entry_struct_t
{    
    BOOL8 valid;
    UINT32 num_regs;
    mgmt_fege_fwd_cfg_t fwd_entry;
} mgmt_fege_fwd_tbl_entry_t;

/*******************************************************************************
* STRUCTURE: mgmt_fege_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This is the config struct for initial values.
*
* ELEMENTS:
*    num_mgmt_fege_chans - number of channels in block to be configured
*******************************************************************************/
typedef struct mgmt_fege_cfg_t
{    
    UINT32 num_mgmt_fege_chans;   
} mgmt_fege_cfg_t;


/*******************************************************************************
* STRUCTURE: mgmt_fege_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for MGMT_FEGE context 
*
* ELEMENTS:
*    state                - MGMT_FEGE state
*    flow_ctrl_en         - flow control status
* 
*******************************************************************************/
typedef struct mgmt_fege_ctxt_struct_t
{
    mgmt_fege_state_t state;
    BOOL8             flow_ctrl_en;
} mgmt_fege_ctxt_t;


/*******************************************************************************
* STRUCTURE: fec_pmon_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is for dynamically changing context.
*
* ELEMENTS:
*    ctxt - additional context (see substructure definition)
*    ing_fwd_tbl[MGMT_FEGE_VLAN_REPL_TABLE_SIZE] - ingress forwarding table
*    egr_fwd_tbl[MGMT_FEGE_VLAN_REPL_TABLE_SIZE] - egress forwarding table
* 
*******************************************************************************/
typedef struct mgmt_fege_var_t 
{    
    mgmt_fege_ctxt_t ctxt;
    mgmt_fege_fwd_tbl_entry_t  ing_fwd_tbl[MGMT_FEGE_VLAN_REPL_TABLE_SIZE];
    mgmt_fege_fwd_tbl_entry_t  egr_fwd_tbl[MGMT_FEGE_VLAN_REPL_TABLE_SIZE];
} mgmt_fege_var_t;

/*******************************************************************************
* STRUCTURE: mgmt_fege_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Context structure for MGMT FEGE
*
* ELEMENTS:
*    base - base type for all block handles
*    cfg - fixed values from startup
*    var - dynamically changing values during run-time
*    enet_fege_handle - handle to sub-block enet_fege to call its methods
*******************************************************************************/
typedef struct mgmt_fege_handle_struct_t 
{
    pmc_handle_t base;
    
    mgmt_fege_cfg_t cfg;
    mgmt_fege_var_t var;

    enet_fege_handle_t *enet_fege_handle;
} mgmt_fege_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC mgmt_fege_handle_t *mgmt_fege_ctxt_create(pmc_handle_t *parent, 
                                                 UINT32 base_address, 
                                                 pmc_sys_handle_t *sys_handle, 
                                                 const char *tsb_name, 
                                                 UINT32 enet_fege_base_addr);

PUBLIC void mgmt_fege_ctxt_destroy(mgmt_fege_handle_t *mgmt_fege_handle);
PUBLIC void mgmt_fege_handle_init(mgmt_fege_handle_t *mgmt_fege_handle);
PUBLIC PMC_ERROR mgmt_fege_handle_restart_init(mgmt_fege_handle_t *mgmt_fege_handle,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                               pmc_energy_state_t energy_state);
PUBLIC UINT32 mgmt_fege_base_address_get(mgmt_fege_handle_t *mgmt_fege_handle);

PUBLIC PMC_ERROR  mgmt_fege_init(mgmt_fege_handle_t *mgmt_fege_handle);

PUBLIC PMC_ERROR  mgmt_fege_uninit(mgmt_fege_handle_t *mgmt_fege_handle,
                                   BOOL8               skip_active_filters_check);

PUBLIC BOOL8 mgmt_fege_pkt_all_dpi_deprov_get(mgmt_fege_handle_t        *mgmt_fege_handle);

PUBLIC PMC_ERROR mgmt_fege_ingr_pkt_fwd_cfg(mgmt_fege_handle_t          *mgmt_fege_handle,
                                           BOOL8                         enable,
                                           mgmt_fege_fwd_cfg_t          *fwd_cfg_ptr);

PUBLIC void mgmt_fege_ingr_pkt_fwd_cfg_get(mgmt_fege_handle_t *mgmt_fege_handle, 
                                           UINT16 dpi_id,
                                           BOOL8 *enable_ptr, 
                                           mgmt_fege_fwd_cfg_t *fwd_cfg_ptr);

PUBLIC PMC_ERROR mgmt_fege_egr_pkt_fwd_cfg(mgmt_fege_handle_t           *mgmt_fege_handle,
                                            BOOL8                        enable,
                                            mgmt_fege_fwd_cfg_t         *fwd_cfg_ptr,
                                            BOOL8                        fcs_calc_en,
                                            BOOL8                        priority_en);

PUBLIC void mgmt_fege_egr_pkt_fwd_cfg_get(mgmt_fege_handle_t         *mgmt_fege_handle,
                                          UINT16                      dpi_id,
                                          BOOL8                      *enable_ptr,
                                          mgmt_fege_fwd_cfg_t        *fwd_cfg_ptr,
                                          BOOL8                      *fcs_calc_en_ptr,
                                          BOOL8                      *priority_en_ptr);

PUBLIC PMC_ERROR mgmt_fege_egr_pkt_fwd_vlan_prio_cfg_get(mgmt_fege_handle_t     *mgmt_fege_handle,
                                                         UINT16                 dpi_id,
                                                         BOOL8                  *enable,
                                                         util_global_vlan_cfg_t *vlan_cfg_ptr,
                                                         BOOL8                  *priority_en_ptr);

PUBLIC PMC_ERROR mgmt_fege_frame_sizes_set(mgmt_fege_handle_t          *mgmt_fege_handle,
                                           UINT16                       txMaxFrameSize,
                                           UINT16                       txMinFrameSize,
                                           UINT16                       rxMaxFrameSize,
                                           UINT16                       rxMinFrameSize);

PUBLIC void mgmt_fege_frame_sizes_get(mgmt_fege_handle_t           *mgmt_fege_handle,
                                           UINT16                       *txMaxFrameSize,
                                           UINT16                       *txMinFrameSize,
                                           UINT16                       *rxMaxFrameSize,
                                           UINT16                       *rxMinFrameSize);

PUBLIC void mgmt_fege_rx_flow_control_set(mgmt_fege_handle_t     *mgmt_fege_handle,
                                          BOOL8                   enable,
                                          BOOL8                   drop_pause_frm);

PUBLIC void mgmt_fege_rx_flow_control_get(mgmt_fege_handle_t    *mgmt_fege_handle,
                                          BOOL8                 *enable,
                                          BOOL8                 *drop_pause_frm);

PUBLIC PMC_ERROR mgmt_fege_int_enable(mgmt_fege_handle_t    *mgmt_fege_handle,
                                      mgmt_fege_int_t       *int_table_ptr,
                                      mgmt_fege_int_t       *int_en_table_ptr,
                                      BOOL                   enable);

PUBLIC PMC_ERROR mgmt_fege_int_retrieve(mgmt_fege_handle_t    *mgmt_fege_handle,
                                        mgmt_fege_int_t       *filt_table_ptr,
                                        mgmt_fege_int_t       *int_table_ptr);

PUBLIC PMC_ERROR mgmt_fege_int_clear(mgmt_fege_handle_t    *mgmt_fege_handle,
                                     mgmt_fege_int_t       *int_table_ptr,
                                     mgmt_fege_int_t       *int_en_table_ptr);

PUBLIC PMC_ERROR mgmt_fege_int_enabled_check(mgmt_fege_handle_t    *mgmt_fege_handle,
                                             mgmt_fege_int_t       *int_en_table_ptr,
                                             BOOL                  *int_found_ptr);

PUBLIC PMC_ERROR mgmt_fege_restart_init(mgmt_fege_handle_t               *mgmt_fege_handle,
                                        util_global_restart_init_cfg_t   *restart_init_cfg_ptr,
                                        BOOL8                             top_level_reset);
PUBLIC BOOL8 mgmt_fege_pkt_fwd_status_get(mgmt_fege_handle_t          *mgmt_fege_handle,
                                          UINT32                       dpi_id,
                                          util_global_datapath_dir_t   dir);

PUBLIC void mgmt_fege_ingr_pkt_fwd_cleanup(mgmt_fege_handle_t          *mgmt_fege_handle,
                                           UINT32                       dpi_id);

PUBLIC void mgmt_fege_egr_pkt_fwd_cleanup(mgmt_fege_handle_t          *mgmt_fege_handle,
                                          UINT32                       dpi_id);
    
#ifdef __cplusplus
}
#endif

#endif /* _MGMT_FEGE_H */

/*
** end of file
*/
