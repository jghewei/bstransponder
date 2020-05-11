/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*     C8 serdes block include file.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _C8_H
#define _C8_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "c8_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/* C8_MAX_NUM_SERDES: Maximum number of serdes in block */
#define C8_MAX_NUM_SERDES 8

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: c8_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is for dynamically changing context.
*
* ELEMENTS:
*    reset - TRUE: is in reset
*    patt_gen_enable - TRUE: pattern generator is enabled
*    loopback_enable - TRUE: loopback is enabled
*******************************************************************************/
typedef struct c8_var_t
{
    BOOL8 reset;
    BOOL8 patt_gen_enable;
    BOOL8 loopback_enable;
} c8_var_t;

/*******************************************************************************
* STRUCTURE: c8_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Context structure for T8 SERDES
*
* ELEMENTS:
*    base - base type for all block handles
*    var - dynamically changing values during run-time
*******************************************************************************/
typedef struct c8_handle_struct_t 
{
    pmc_handle_t base;
    c8_var_t *var;
} c8_handle_struct_t;

/*******************************************************************************
* STRUCTURE: c8_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque datatype
*******************************************************************************/
typedef struct c8_handle_struct_t c8_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC c8_handle_t *c8_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);

PUBLIC void c8_ctxt_destroy(c8_handle_t *c8_handle);

PUBLIC void c8_handle_init(c8_handle_t *c8_handle);

PUBLIC PMC_ERROR c8_patt_mon_cfg(c8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);

PUBLIC PMC_ERROR c8_patt_mon_resync(c8_handle_t *handle,
                                    UINT8 serdes_id);

PUBLIC PMC_ERROR c8_patt_mon_err_count_get(c8_handle_t *handle,
                                           UINT8 serdes_id,
                                           util_patt_status_t    *status,
                                           UINT32 *err_count);

PUBLIC PMC_ERROR c8_patt_gen_err_insert_cfg(c8_handle_t *handle,
                                            UINT8 serdes_id);

PUBLIC PMC_ERROR c8_patt_gen_cfg(c8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
  
PUBLIC PMC_ERROR c8_loopback_activate(c8_handle_t *handle,
                                      UINT8 serdes_id);

PUBLIC PMC_ERROR c8_loopback_deactivate(c8_handle_t *handle,
                                        UINT8 serdes_id);

PUBLIC PMC_ERROR c8_loopback_status_get(c8_handle_t   *handle,
                                        UINT8          serdes_id,
                                        BOOL8         *enabled_ptr);

PUBLIC void c8_polarity_cfg(c8_handle_t              *handle,
                            UINT8                     serdes_id,
                            util_global_direction_t   dir,
                            BOOL8                     enable);       

PUBLIC PMC_ERROR c8_tx_swing_emphasis_levels_set(c8_handle_t           *handle,
                                                 UINT8                  serdes_id,
                                                 c8_tx_swing_levels_t  *levels_cfg_ptr);     

PUBLIC void c8_patt_gen_bus_width_set(c8_handle_t           *handle,
                                      UINT8                  serdes_id,
                                      UINT8                  bus_width);

PUBLIC PMC_ERROR c8_handle_restart_init(c8_handle_t                      *c8_handle,
                                        util_global_restart_init_cfg_t  *restart_init_cfg_ptr,
                                        UINT32                           c8_reset_msk);

PUBLIC PMC_ERROR c8_ext_ctxt_state_set(c8_handle_t      *c8_handle, 
                                       UINT16            lane_msk,
                                       BOOL8             out_of_reset);

PUBLIC BOOL8 c8_reset_state_get(c8_handle_t *handle,
                                UINT32       lane) ;

#ifdef __cplusplus
}
#endif

#endif /* _C8_H */

/*
** end of file
*/
