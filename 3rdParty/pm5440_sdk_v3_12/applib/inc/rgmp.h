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

#ifndef _RGMP_H
#define _RGMP_H

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
#include "rgmp_api.h"
#include "mp_mgen.h"
#include "rxjc_proc.h"
#include "cpact_obuf.h"
#include "ohfs_remove.h"
#include "util_gen_db.h"
#include "util_gen_db_mapotn.h"



/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: rgmp_chnl_state_t
*_______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for RGMP channel state
*
* ELEMENTS:
*   RGMP_CHNL_START       - Channel is in start state (not provisioned)
*   RGMP_CHNL_EQUIPPED    - Channel is equiped (provisioned but not activated)
*   RGMP_CHNL_OPERATIONAL - Channel is operational
*   
*******************************************************************************/
typedef enum {
    RGMP_CHNL_START        = 0,
    RGMP_CHNL_EQUIPPED     = 1,
    RGMP_CHNL_OPERATIONAL  = 2,
} rgmp_chnl_state_t;



/*******************************************************************************
* ENUM: rgmp_instance_type_t
*_______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for RGMP instance (RGMP code is shared between
*   MAPOTN and COREOTN)
*
* ELEMENTS:
*   INSTANCE_RGMP  - RGMP instance (MAPOTN)
*   INSTANCE_DEMUX - ODTU_DMX instance (COREOTN)
*   
*******************************************************************************/
typedef enum rgmp_instance_type_t {
    INSTANCE_RGMP  = 0,
    INSTANCE_DEMUX = 1,
} rgmp_instance_type_t; 



/*
** Constants
*/
/* Number of supported channels */
#define RGMP_NUM_CHANL           96
#define RGMP_NUM_TS_IN_CHNL      80

/*
** Macro Definitions
*/
/* RGMP_INT(handle, tsb_name, field_type, tsb_field, struct_field) */
#define RGMP_INT_TABLE_DEFINE()                                                 \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, DPLM           , dplm_i              ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, CSF            , csf_i               ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, CM_OUT_LIMIT   , cm_out_limit_i      ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, CND_MAX        , cnd_max_i           ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, JC_ERR_SYNCLOSS, jc_err_syncloss_i   ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, CM_CND_CRC_ERR , cm_cnd_crc_err_i    ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, PSERV_EARLY    , pserv_early_i       ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, PSERV_LATE     , pserv_late_i        ); \
    RGMP_INT(rgmp_handle, rgmp, lfield_range, SSF_SF_RCVD    , ssf_sf_rcvd_i       ); \

#define RGMP_STATUS_TABLE_DEFINE()                                              \
    RGMP_STATUS(rgmp_handle, rgmp, lfield_range, DPLM_V       , dplm_v             ); \
    RGMP_STATUS(rgmp_handle, rgmp, lfield_range, CSF_V        , csf_v              ); \
    RGMP_STATUS(rgmp_handle, rgmp, lfield_range, SSF_SF_RCVD_V, ssf_sf_rcvd_v      ); \


/*
** Structures and Unions
*/



/*******************************************************************************
* STRUCTURE: rgmp_chnl_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for channel configuration
*
* ELEMENTS:
*   state           - state of the LO channel
*
*******************************************************************************/
typedef struct rgmp_chnl_cfg_t {
    rgmp_chnl_state_t state; 

} rgmp_chnl_cfg_t;


/*******************************************************************************
* STRUCTURE: rgmp_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for static configuration
*
* ELEMENTS:
*   num_rgmp_chanl  - maximum number of channels
*   instance        - RGMP instance
*   max_fifo_num    - maximum number of fifos
*   db_id           - database id to attach to
*   db_dev_addr     - address to register with database
*   db_handle       - database handle
*
*******************************************************************************/
typedef struct rgmp_cfg_t {
    
    UINT32 num_rgmp_chanl;
    rgmp_instance_type_t instance;
    UINT32 max_fifo_num;
    util_gen_db_id_t db_id;
    UINT32 db_handle;
    
} rgmp_cfg_t;

/*******************************************************************************
* STRUCTURE: rgmp_var_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for dynamic context 
*
* ELEMENTS:
*   rgmp_chnl_cfg - RGMP channel context (see rgmp_chnl_cfg_t)
*
*******************************************************************************/
typedef struct rgmp_var_t {
    
    rgmp_chnl_cfg_t rgmp_chnl_cfg[RGMP_NUM_CHANL];

} rgmp_var_t;

/*******************************************************************************
* STRUCTURE: rgmp_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure RGMP channel handle 
*
* ELEMENTS:
*   base                - See pmc_handle_t
*   cfg                 - Pointer for static configuration
*   var                 - Pointer for dynamic configuration
*   ohfs_remove_handle  - Pointer to OHFS REMOVE handle
*   rxjc_proc_handle    - Pointer to RXJC PROC handle
*   mp_mgen_handle      - Pointer to MP MGEN handle
*   cpact_obuf_handle   - Pointer to CPACT OBUF
*
*******************************************************************************/
typedef struct rgmp_handle_struct_t {
    pmc_handle_t base;
    
    rgmp_cfg_t cfg;
    rgmp_var_t var;
     
    ohfs_remove_handle_t *ohfs_remove_handle;
    rxjc_proc_handle_t *rxjc_proc_handle;
    mp_mgen_handle_t *mp_mgen_handle;
    cpact_obuf_handle_t *cpact_obuf_handle;
     
} rgmp_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC rgmp_handle_t *rgmp_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void rgmp_ctxt_destroy(rgmp_handle_t *rgmp_handle);
PUBLIC void rgmp_handle_init(rgmp_handle_t *rgmp_handle, 
                             rgmp_instance_type_t rgmp_instance);
PUBLIC PMC_ERROR rgmp_handle_restart_init(rgmp_handle_t *rgmp_handle, 
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t energy_state);
                             
PUBLIC PMC_ERROR rgmp_init(rgmp_handle_t *rgmp_handle, 
                           pmc_energy_state_t energy_state);

PUBLIC BOOL8 rgmp_start_state_test(rgmp_handle_t *rgmp_handle);                            
                             
PUBLIC UINT32 rgmp_base_address_get(rgmp_handle_t *rgmp_handle);

PUBLIC PMC_ERROR odtu_dmx_rgmp_lo_chnl_unprov(rgmp_handle_t *rgmp_handle, 
                                              UINT32 chnl);

PUBLIC PMC_ERROR rgmp_chnl_activate(rgmp_handle_t *rgmp_handle, UINT32 chnl);

PUBLIC PMC_ERROR rgmp_chnl_deactivate(rgmp_handle_t *rgmp_handle, UINT32 chnl);
PUBLIC void rgmp_chnl_clean( rgmp_handle_t *rgmp_handle,
                             UINT32        *chnl,
                             UINT32        num_chnl );

PUBLIC PMC_ERROR odtu_dmx_rgmp_lo_chnl_prov(rgmp_handle_t *rgmp_handle,
                                            UINT32 chnl, 
                                            UINT32 client_type, 
                                            util_global_odukp_type_t server_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            util_global_odu_line_payload_t ts_type,
                                            UINT32 num_odu0,
                                            DOUBLE client_rate,
                                            UINT32 num_ts,
                                            BOOL8 offline_update,
                                            BOOL use_custom_ppm,
                                            UINT32 ppm_offset);  

PUBLIC PMC_ERROR odtu_dmx_rgmp_chnl_tribslot_cfg(rgmp_handle_t *rgmp_handle,
                                                 UINT32 chnl, 
                                                 UINT32 ts_loc, 
                                                 UINT32 client_type, 
                                                 UINT32 server_type,
                                                 util_global_odu_line_payload_t ts_type,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 ts_occupied,
                                                 UINT32 first_ts, UINT32 last_ts,
                                                 UINT32 frm_num);            

PUBLIC PMC_ERROR rgmp_int_chnl_enable(rgmp_handle_t    *rgmp_handle,
                                      UINT32            chnl_id,
                                      rgmp_int_chnl_t  *int_table_ptr,
                                      rgmp_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable);

PUBLIC PMC_ERROR rgmp_int_chnl_clear(rgmp_handle_t    *rgmp_handle,
                                     UINT32            chnl_id,
                                     rgmp_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR rgmp_int_chnl_retrieve(rgmp_handle_t    *rgmp_handle,
                                        UINT32            chnl_id,
                                        rgmp_int_chnl_t  *filt_table_ptr,
                                        rgmp_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR rgmp_int_chnl_enabled_check(rgmp_handle_t     *rgmp_handle,
                                             UINT32             chnl_id,
                                             rgmp_int_chnl_t   *int_en_table_ptr,
                                             BOOL8              *int_found_ptr);
PUBLIC void rgmp_activated_chnl_get(rgmp_handle_t *rgmp_handle,
                                    BOOL8 *actv_chnl);    
PUBLIC util_global_mapping_mode_t rgmp_mapping_mode_get(rgmp_handle_t *rgmp_handle,
                                                        UINT32 calendar_cycle);
PUBLIC BOOL8 rgmp_fixed_stuff_enable_get(rgmp_handle_t *rgmp_handle,
                                         UINT32 chnl);
PUBLIC void rgmp_var_update(rgmp_handle_t *rgmp_handle, 
                            UINT32 chnl,
                            util_global_chnl_state_t chnl_state);                                
                                        
#ifdef __cplusplus
}
#endif

#endif /* _RGMP_H */

/*
** end of file
*/
