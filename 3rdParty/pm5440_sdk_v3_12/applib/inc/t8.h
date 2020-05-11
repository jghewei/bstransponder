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
*
*   NOTES:
*
*******************************************************************************/

#ifndef _T8_H
#define _T8_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "t8_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: t8_loopback_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying T8 loopback mode.
*
* ELEMENTS:
*           T8_FACILITY_LOOPBACK        - T8 facility loopback
*           T8_DIAGNOSTIC_LOOPBACK      - T8 diagnostic loopback
*
*******************************************************************************/
typedef enum t8_loopback_mode_t
{
    T8_FACILITY_LOOPBACK    = 0,
    T8_DIAGNOSTIC_LOOPBACK,
    LAST_T8_LOOPBACK_MODE
} t8_loopback_mode_t;

/*
** Constants
*/
/* Maximum number of serdes in block */
#define T8_MAX_NUM_SERDES 2

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: t8_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This is the config struct for initial values.
*
* ELEMENTS:
*    Currently none
*******************************************************************************/
typedef struct t8_cfg_t
{

} t8_cfg_t;

/*******************************************************************************
* STRUCTURE: t8_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is for dynamically changing context.
*
* ELEMENTS:
*    See typedef
*******************************************************************************/
typedef struct t8_var_t
{    
    BOOL8 reset; /*!< block reset status */
    BOOL8 patt_gen_enable; /*!< TRUE if pattern generator is enabled */
    UINT8 serdes_patt_bus_width; /*!< pattern monitor/generator bus width */
    t8_loopback_mode_t loopback_mode; /*!< configured loopback mode */
    BOOL8  inv_dfe_error; /*!< serdes polarity state */
} t8_var_t;

/*******************************************************************************
* STRUCTURE: t8_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Context structure for T8 SERDES
*
* ELEMENTS:
*    base - base type for all block handles
*    cfg - fixed values from startup
*    var - dynamically changing values during run-time
*******************************************************************************/
typedef struct t8_handle_struct_t 
{
    pmc_handle_t base;
    
    t8_cfg_t cfg;
    t8_var_t *var;
} t8_handle_struct_t;


/*******************************************************************************
* STRUCTURE: t8_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct t8_handle_struct_t t8_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC t8_handle_t *t8_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void t8_ctxt_destroy(t8_handle_t *t8_handle);
PUBLIC void t8_handle_init(t8_handle_t *t8_handle);
PUBLIC PMC_ERROR t8_patt_mon_cfg(t8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR t8_patt_mon_resync(t8_handle_t *handle,
                                    UINT8 serdes_id);
PUBLIC PMC_ERROR t8_patt_mon_err_count_get(t8_handle_t *handle,
                                           UINT8 serdes_id,
                                           util_patt_status_t    *status,
                                           UINT32 *err_cnt);         
PUBLIC PMC_ERROR t8_patt_gen_err_insert_cfg(t8_handle_t *handle,
                                            UINT8 serdes_id);
PUBLIC PMC_ERROR t8_patt_gen_cfg(t8_handle_t *handle,
                                 UINT8 serdes_id,
                                 util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);                      
PUBLIC PMC_ERROR t8_loopback_activate(t8_handle_t *handle,
                                      UINT8 serdes_id,
                                      t8_loopback_mode_t loopback_mode);
PUBLIC PMC_ERROR t8_loopback_deactivate(t8_handle_t *handle,
                                        UINT8 serdes_id);

PUBLIC PMC_ERROR t8_loopback_status_get(t8_handle_t *handle,
                                      UINT8 serdes_id,
                                      t8_loopback_mode_t *loopback_mode_ptr);

PUBLIC void t8_polarity_cfg(t8_handle_t              *handle,
                            UINT8                     serdes_id,                           
                            util_global_direction_t   dir,
                            BOOL8                     enable);

PUBLIC void t8_dfe_error_invert(t8_handle_t *handle,
                                UINT8 serdes_id);

PUBLIC BOOL8 t8_dfe_error_get(t8_handle_t *handle,
                              UINT8 serdes_id);

PUBLIC PMC_ERROR t8_tx_swing_emphasis_levels_set(t8_handle_t        *handle,
                                                 UINT8               serdes_id,
                                                 t8_tx_swing_levels_t  *levels_cfg_ptr);   
    
PUBLIC PMC_ERROR t8_dc_coupling_set(t8_handle_t           *handle,
                                    UINT8                  serdes_id,
                                    BOOL8                  dc_coupling_enabled);

PUBLIC void t8_patt_gen_bus_width_set(t8_handle_t           *handle,
                                      UINT8                  serdes_id,
                                      UINT8                  bus_width);

PUBLIC PMC_ERROR t8_handle_restart_init(t8_handle_t                      *t8_handle,
                                        util_global_restart_init_cfg_t   *restart_init_cfg_ptr,
                                        UINT32                            t8_reset_msk);

PUBLIC PMC_ERROR t8_ext_ctxt_state_set(t8_handle_t      *t8_handle, 
                                       UINT16            lane_msk,
                                       BOOL8             out_of_reset);

PUBLIC BOOL8 t8_reset_state_get(t8_handle_t *handle,
                                UINT32       lane);

PUBLIC PMC_ERROR t8_vert_hist_get(t8_handle_t     **t8_handle_all,
                                  UINT32            lane_0_19_msk,
                                  UINT32            lane_20_39_msk,
                                  util_vhist_test_length_t  test_length,
                                  UINT64            userdef_length,
                                  UINT64            bin_buffer[][64]);

         
#ifdef __cplusplus
}
#endif

#endif /* _T8_H */

/*
** end of file
*/
