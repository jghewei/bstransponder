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

#ifndef _STS_SW_H
#define _STS_SW_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "sts_sw_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: sts_sw_inst_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for STS_SW instantiation.
*
* ELEMENTS:
*   INSTANCE_DEMUX_STS_SW   - STS_SW is part of ODTU_DMX module
*   INSTANCE_MUX_STS_SW     - STS_SW is part of ODTU_MUX module
*******************************************************************************/
typedef enum {          
    INSTANCE_DEMUX_STS_SW   = 0,
    INSTANCE_MUX_STS_SW     = 1,
} sts_sw_inst_type_t; 

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
typedef struct sts_sw_handle_struct_t sts_sw_handle_t; /*!< Opaque reference to module handle */

/*******************************************************************************
* STRUCTURE: sts_sw_ho_chnl_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for HO channel context information to be used in the switch
*    configuration
*
* ELEMENTS:
*   oduk_type               - HO ODU container size
*   chnl_id                 - HO channel ID 
*   num_tribslot            - the total number of tributary slots occupied by the channel
*   tribslot_loc_mask[3]    - 3 x 32bit mask to describe the calendar slot 
*                             assignment of the HO channel
*   cal_entry[96]           - compact version of calendar slot array that is
*                             translated from tribslot_loc_mask[3]. The valid
*                             end index for any HO channel should be num_tribslot - 1.
*   lo_chnl_ids_ptr[96]     - array to store LO channel IDs in the HO channel
*
*******************************************************************************/
typedef struct {
    
    util_global_odukp_type_t oduk_type; 
    util_global_odu_line_payload_t ts_type;
    UINT8 chnl_id;
    UINT8 num_tribslot;
    UINT8 cal_entry[96];
    UINT8 lo_chnl_ids_ptr[96];
    
} sts_sw_ho_chnl_cfg_t;



/*******************************************************************************
* STRUCTURE: sts_sw_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for fixed configuration in STS_SW module
*
* ELEMENTS:
*   
*   
*******************************************************************************/
/* This is the config struct for initial values. */
typedef struct {
    
} sts_sw_cfg_t;

/* This struct is to illustrate the dynamically changing context. */
/*******************************************************************************
* STRUCTURE: sts_sw_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for dynamically changing configuration in STS_SW module
*
* ELEMENTS:
*   ho_chnl_cfg - array to store HO ODU channel context
*   inst_type   - enum, sts_sw module type 
*    
*******************************************************************************/
typedef struct {
    
    /* To add dynamic sts_sw config context */
    sts_sw_ho_chnl_cfg_t ho_chnl_cfg[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    sts_sw_inst_type_t inst_type;
      
} sts_sw_var_t;

/*******************************************************************************
* STRUCTURE: sts_sw_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for dynamically changing configuration in STS_SW module
*
* ELEMENTS:
*   base    - base address of sts_sw_handle
*   cfg     - structure to describe fixed configuration of sts_sw module
*   var     - structure to describe dynamically changing configuration of sts_sw_module
*    
*******************************************************************************/
typedef struct sts_sw_handle_struct_t {
    pmc_handle_t base;
    
    sts_sw_cfg_t cfg;
    sts_sw_var_t var;
       
} sts_sw_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC sts_sw_handle_t *sts_sw_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void sts_sw_ctxt_destroy(sts_sw_handle_t *sts_sw_handle);
PUBLIC void sts_sw_handle_init(sts_sw_handle_t *sts_sw_handle, 
                               sts_sw_inst_type_t sts_sw_instance);
PUBLIC PMC_ERROR sts_sw_handle_restart_init(sts_sw_handle_t *sts_sw_handle, 
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);
PUBLIC UINT32 sts_sw_base_address_get(sts_sw_handle_t *sts_sw_handle);
PUBLIC PMC_ERROR sts_sw_init(sts_sw_handle_t *sts_sw_handle, UINT32 mode, 
                             UINT32 enbl);
PUBLIC PMC_ERROR sts_sw_chnl_ctxt_destroy(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id);                         
PUBLIC PMC_ERROR sts_sw_output_ctxt_destroy(sts_sw_handle_t *sts_sw_handle);
PUBLIC PMC_ERROR sts_sw_ho_chnl_cfg(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id);
PUBLIC PMC_ERROR sts_sw_ho_chnl_ts_cfg(sts_sw_handle_t *sts_sw_handle, 
                                       UINT32 chnl_id);
PUBLIC PMC_ERROR sts_sw_ho_chnl_init(sts_sw_handle_t *sts_sw_handle, 
                                     UINT32 ho_chnl_id,
                                     util_global_odukp_type_t oduk_type,
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_trib_slot,
                                     UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS]);                                     
PUBLIC PMC_ERROR sts_sw_lo_chnl_cfg(sts_sw_handle_t *sts_sw_handle, 
                                    UINT32 ho_chnl_id,
                                    UINT32 cycle, 
                                    UINT32 lo_chnl_id, 
                                    UINT32 unset,
                                    BOOL8 super_deprov);
PUBLIC PMC_ERROR sts_sw_lo_chnl_cfg_all(sts_sw_handle_t *sts_sw_handle, 
                                        UINT32 ho_chnl_id,
                                        UINT32 cycle, 
                                        UINT32 lo_chnl_id, 
                                        UINT32 eomf_cycle,
                                        UINT32 first_cycle);
PUBLIC PMC_ERROR sts_sw_lo_chnl_ids_get(sts_sw_handle_t *sts_sw_handle, 
                                        UINT32 ho_chnl_id, UINT32 *lo_chnl_ids);  
PUBLIC PMC_ERROR sts_sw_lo_chnl_ids_ptr_ctxt_reset(sts_sw_handle_t *sts_sw_handle, 
                                                   UINT32 ho_chnl_id,
                                                   UINT32 lo_chnl_id);                                                                          
PUBLIC PMC_ERROR sts_sw_lo_eomf_cycle_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cycle, UINT32 eomf_cycle);
PUBLIC PMC_ERROR sts_sw_lo_first_cycle_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cycle, UINT32 first_cycle);                                                                              
PUBLIC PMC_ERROR sts_sw_cfg_apply(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id,
                                  UINT32 num_tribslot);

PUBLIC PMC_ERROR sts_sw_cfg(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id, BOOL8 cfg_all); 

PUBLIC PMC_ERROR sts_sw_ho_ipt_valid_reset(sts_sw_handle_t *sts_sw_handle, 
                                           UINT32 cal_entry);
                                           
PUBLIC PMC_ERROR sts_sw_ram_init(sts_sw_handle_t *sts_sw_handle);   

PUBLIC PMC_ERROR sts_sw_ho_chnl_deprov(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id);

PUBLIC PMC_ERROR sts_sw_ho_calendar_cfg_deprov(sts_sw_handle_t *sts_sw_handle, 
                                               UINT32 cal_entry);     

PUBLIC PMC_ERROR sts_sw_lo_calendar_cfg_deprov(sts_sw_handle_t *sts_sw_handle, 
                                               UINT32 cal_entry);                                                                                  

PUBLIC PMC_ERROR sts_sw_calendar_null_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cal_entry);
                
PUBLIC PMC_ERROR sts_sw_ho_last_cycle_set(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 chnl_id, UINT32 last_cycle);                

PUBLIC PMC_ERROR sts_sw_map_dmsim_to_ssf_set(sts_sw_handle_t *sts_sw_handle, 
                                             BOOL8 map_dmsim_to_ssf);


PUBLIC PMC_ERROR sts_sw_map_dmsim_to_ssf_get(sts_sw_handle_t *sts_sw_handle, 
                                             BOOL8 *map_dmsim_to_ssf);               
PUBLIC PMC_ERROR sts_sw_ho_clean(sts_sw_handle_t *sts_sw_handle, 
                                 UINT32           *ho_chnl,
                                 UINT32           num_ho_chnl,
                                 UINT32           *lo_chnl,
                                 UINT32           num_lo_chnl);
PUBLIC PMC_ERROR sts_sw_lo_calendar_cfg_clean(sts_sw_handle_t *sts_sw_handle, 
                                              UINT32 *cycle,
                                              UINT32 num_cycle);  
                                         
PUBLIC PMC_ERROR sts_sw_calendar_init(sts_sw_handle_t *sts_sw_handle);    
                                     
PUBLIC PMC_ERROR sts_sw_ho_calendar_null_cfg(sts_sw_handle_t *sts_sw_handle, 
                                             UINT8 chnl_id,
                                             UINT8 num_tribslot,
                                             UINT32 cal_entry[UTIL_GLOBAL_MAX_NUM_CAL_ENTRY],
                                             UINT8 target_reg_mask);                                
                                              

#ifdef __cplusplus
}
#endif

#endif /* _STS_SW_H */

/*
** end of file
*/
