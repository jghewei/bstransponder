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

#ifndef _TGMP_H
#define _TGMP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_opsa.h"
#include "util_global.h"
#include "tgmp_api.h"
#include "mp_mgen.h"
#include "txjc_proc.h"
#include "expand_ibuf.h"
#include "ohfs_insert.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/* Number of supported channels */
#define TGMP_NUM_CHANL           96

/*
** Macro Definitions
*/
/* TGMP_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define TGMP_INT_CHNL_TABLE_DEFINE()                                  \
    TGMP_INT_CHNL_RANGE(tgmp_handle, tgmp, TXJC_RES_OVRFLW, txjc_res_ovrflw_i); \

/*
** Structures and Unions
*/

/*******************************************************************************
* ENUM: tgmp_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the state of a channel
*
* ELEMENTS:
*   TGMP_CHNL_START       - Channel have not been provisioned
*   TGMP_CHNL_EQUIPPED    - Channel has been provisioned, but no activated
*   TGMP_CHNL_OPERATIONAL - Channel is active
*
*******************************************************************************/
typedef enum {
    TGMP_CHNL_START        = 0,
    TGMP_CHNL_EQUIPPED     = 1,
    TGMP_CHNL_OPERATIONAL  = 2
} tgmp_chnl_state_t;

/*******************************************************************************
* ENUM: rgmp_instance_type_t
*_______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for TGMP instance (TGMP code is shared between
*   MAPOTN and COREOTN)
*
* ELEMENTS:
*   INSTANCE_TGMP  - TGMP instance (MAPOTN)
*   INSTANCE_MUX   - ODTU_MUX instance (COREOTN)
*   
*******************************************************************************/
typedef enum {
    INSTANCE_TGMP   = 0,
    INSTANCE_MUX    = 1,
} tgmp_instance_type_t; 


/*******************************************************************************
* STRUCTURE: tgmp_chnl_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for LO ODUk channel configuration.
*   
* ELEMENTS:
*   state           - state of the LO channel
*                     1 - enabled
*                     0 - disabled 
*   lo_oduk_type    - ODUk type of LO channel
*   ho_oduk_type    - ODUk type of HO channel that this LO channel is associated
*                     with.
*   num_ts          - 
*   mapping_mode    - Mapping mode of the LO channel
*   
*******************************************************************************/
typedef struct 
{
    tgmp_chnl_state_t state;
    UINT32 num_cal_entries;
} tgmp_chnl_cfg_t;


/*******************************************************************************
* STRUCTURE: tgmp_cfg_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for static configuration
*
* ELEMENTS:
*   num_tgmp_chanl  - maximum number of channels
*   instance        - RGMP instance
*   max_fifo_num    - maximum number of fifos
*   db_id           - database id to attach to
*   db_dev_addr     - address to register with database
*   db_handle       - database handle
*
*******************************************************************************/
typedef struct tgmp_cfg_struct_t{
    
    UINT32 num_tgmp_chanl;
    tgmp_instance_type_t instance;
    UINT32 max_fifo_num;
    util_gen_db_id_t db_id;
    UINT32 db_handle;
    
} tgmp_cfg_t;

/*******************************************************************************
* STRUCTURE: tgmp_var_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for dynamic configuration
*
* ELEMENTS:
*   tgmp_chnl_cfg - See tgmp_chnl_cfg_t
*
*******************************************************************************/
typedef struct tgmp_var_struct_t{
    
    tgmp_chnl_cfg_t tgmp_chnl_cfg[TGMP_NUM_CHANL];
          
} tgmp_var_t;

/*******************************************************************************
* STRUCTURE: tgmp_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure TGMP channel handle 
*
* ELEMENTS:
*   base                - See pmc_handle_t
*   cfg                 - Pointer for static configuration
*   var                 - Pointer for dynamic configuration
*   ohfs_insert_handle  - Pointer to OHFS INSERT handle
*   txjc_proc_handle    - Pointer to TXJC PROC handle
*   mp_mgen_handle      - Pointer to MP MGEN handle
*   expand_ibuf_handle  - Pointer to EXPAND IBUF
*
*******************************************************************************/
typedef struct tgmp_handle_struct_t {
    pmc_handle_t base;
    
    tgmp_cfg_t cfg;
    tgmp_var_t var;
    
    /* Child handles needed */
    ohfs_insert_handle_t  *ohfs_insert_handle;
    txjc_proc_handle_t *txjc_proc_handle;
    mp_mgen_handle_t *mp_mgen_handle;
    expand_ibuf_handle_t *expand_ibuf_handle;
       
} tgmp_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC tgmp_handle_t *tgmp_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void tgmp_ctxt_destroy(tgmp_handle_t *tgmp_handle);

PUBLIC void tgmp_handle_init(tgmp_handle_t *tgmp_handle, 
                             tgmp_instance_type_t tgmp_instance);

PUBLIC void tgmp_alloc_init(tgmp_handle_t *tgmp_handle);

PUBLIC void tgmp_handle_restart_init(tgmp_handle_t *tgmp_handle, 
                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                     pmc_energy_state_t top_energy_state);
                             
PUBLIC UINT32 tgmp_base_address_get(tgmp_handle_t *tgmp_handle);

PUBLIC PMC_ERROR tgmp_init(tgmp_handle_t *tgmp_handle, 
                           pmc_energy_state_t energy_state);
                           
PUBLIC BOOL tgmp_start_state_test(tgmp_handle_t *tgmp_handle); 

PUBLIC PMC_ERROR odtu_mux_tgmp_tribslot_cfg(tgmp_handle_t *tgmp_handle,
                                            UINT32 chnl, UINT32 ts_loc, 
                                            UINT32 client_type, 
                                            UINT32 server_type,
                                            util_global_odu_line_payload_t ts_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            UINT32 ts_occupied,
                                            UINT32 last_ts,
                                            UINT32 frm_num, BOOL8 offline_update);

PUBLIC PMC_ERROR odtu_mux_tgmp_lo_chnl_prov(tgmp_handle_t *tgmp_handle,
                                            UINT32 chnl, UINT32 client_type,
                                            util_global_odukp_type_t server_type,
                                            DOUBLE client_rate,
                                            util_global_mapping_mode_t mapping_mode,
                                            util_global_odu_line_payload_t ts_type, 
                                            UINT32 num_ts, BOOL8 offline_update);

PUBLIC PMC_ERROR odtu_mux_tgmp_lo_chnl_deprov(tgmp_handle_t *tgmp_handle, UINT32 chnl);

PUBLIC PMC_ERROR tgmp_chnl_activate(tgmp_handle_t *tgmp_handle, UINT32 chnl);
PUBLIC PMC_ERROR tgmp_chnl_amp_gmp_startup_activate(tgmp_handle_t *tgmp_handle, UINT32 chnl);
    
PUBLIC PMC_ERROR tgmp_chnl_deactivate(tgmp_handle_t *tgmp_handle, UINT32 chnl);
PUBLIC void tgmp_chnl_clean( tgmp_handle_t *tgmp_handle,
                             UINT32        *chnl,
                             UINT32        num_chnl );

PUBLIC PMC_ERROR tgmp_expd_ibuf_cfc_xoff_level_cfg(tgmp_handle_t *tgmp_handle, 
                                                   UINT32 chnl,  
                                                   UINT32 cfc_fill_lvl);
PUBLIC void tgmp_activated_chnl_get(tgmp_handle_t *tgmp_handle,
                                    BOOL8 *actv_chnl);    
PUBLIC util_global_mapping_mode_t tgmp_mapping_mode_get(tgmp_handle_t *tgmp_handle,
                                                        UINT32 calendar_cycle);
PUBLIC BOOL8 tgmp_fixed_stuff_enable_get(tgmp_handle_t *tgmp_handle,
                                         UINT32 chnl);

PUBLIC PMC_ERROR tgmp_int_chnl_enable(tgmp_handle_t     *tgmp_handle,
                                      UINT32             chnl_id,
                                      tgmp_int_chnl_t   *int_table_ptr,
                                      tgmp_int_chnl_t   *int_en_table_ptr,
                                      BOOL8              enable);

PUBLIC PMC_ERROR tgmp_int_chnl_clear(tgmp_handle_t      *tgmp_handle,
                                     UINT32              chnl_id,
                                     tgmp_int_chnl_t    *int_table_ptr,
                                     tgmp_int_chnl_t    *int_en_table_ptr);

PUBLIC PMC_ERROR tgmp_int_chnl_retrieve(tgmp_handle_t      *tgmp_handle,
                                        UINT32              chnl_id,
                                        tgmp_int_chnl_t    *filt_table_ptr,
                                        tgmp_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR tgmp_int_chnl_enabled_check(tgmp_handle_t    *tgmp_handle,
                                             UINT32            chnl_id,
                                             tgmp_int_chnl_t  *int_en_table_ptr,
                                             BOOL8            *int_found_ptr);

PUBLIC PMC_ERROR tgmp_expxand_cfc_fifo_ipt_id_get(tgmp_handle_t *tgmp_handle,
                                                  UINT32 fifo_id,
                                                  UINT32 frm_num,
                                                  UINT32 *ipt_id);

#ifdef __cplusplus
}
#endif

#endif /* _TGMP_H */

/*
** end of file
*/
