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
*     D8_TX_2X serdes block include file only visible by internal blocks or ss.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _D8_TX_2X_H
#define _D8_TX_2X_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "d8_tx_2x_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/* Maximum number of serdes in block */
#define D8_TX_2X_MAX_NUM_SERDES 2

/* User-defined pattern size in word of 32 bit */
#define D8_TX_2X_USR_PATT_NUM_WORD              2

#define D8_TX_2X_TX_SWING_LEVEL_DEFAULT_VALUE   D8_TX_2X_TX_SWING_LEVEL_825

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: d8_tx_2x_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the config struct for initial values that should not changed in runtime.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    UINT8 patt_gen_bus_width;
} d8_tx_2x_cfg_t;

/*******************************************************************************
* STRUCTURE: d8_tx_2x_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct is for dynamically changing context.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8 reset;
    BOOL8 patt_gen_enable;
    BOOL8 loopback_enable;
} d8_tx_2x_var_t;

/*******************************************************************************
* STRUCTURE: d8_tx_2x_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   D8_TX_2X handle struct.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct d8_tx_2x_handle_struct_t {
    pmc_handle_t base;
    
    d8_tx_2x_cfg_t cfg;
    d8_tx_2x_var_t *var;

} d8_tx_2x_handle_struct_t;


/*! D8_TX_2X block handle used by its parent */
typedef struct d8_tx_2x_handle_struct_t d8_tx_2x_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC d8_tx_2x_handle_t *d8_tx_2x_ctxt_create(pmc_handle_t        *parent, 
                                               UINT32               base_address, 
                                               pmc_sys_handle_t    *sys_handle, 
                                               const char          *tsb_name,
                                               UINT8                gen_bus_width);

PUBLIC void d8_tx_2x_ctxt_destroy(d8_tx_2x_handle_t *d8_tx_2x_handle);

PUBLIC void d8_tx_2x_handle_init(d8_tx_2x_handle_t *d8_tx_2x_handle);

PUBLIC PMC_ERROR d8_tx_2x_patt_mon_cfg(d8_tx_2x_handle_t          *handle,
                                       UINT8                       serdes_id,
                                       util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);

PUBLIC PMC_ERROR d8_tx_2x_patt_gen_cfg(d8_tx_2x_handle_t          *handle,
                                       UINT8                       serdes_id,
                                       util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);

PUBLIC PMC_ERROR d8_tx_2x_patt_gen_cfg_get(d8_tx_2x_handle_t          *handle,
                                           UINT8                       serdes_id,
                                           util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);

PUBLIC PMC_ERROR d8_tx_2x_patt_gen_err_insert_cfg(d8_tx_2x_handle_t *handle,
                                                  UINT8              serdes_id);

PUBLIC PMC_ERROR d8_tx_2x_loopback_activate(d8_tx_2x_handle_t *handle,
                                            UINT8              serdes_id);

PUBLIC PMC_ERROR d8_tx_2x_loopback_deactivate(d8_tx_2x_handle_t *handle,
                                              UINT8 serdes_id);                          

PUBLIC void d8_tx_2x_polarity_cfg(d8_tx_2x_handle_t *handle,
                                  UINT8              serdes_id,
                                  BOOL8              enable);

PUBLIC PMC_ERROR d8_tx_2x_tx_swing_level_set(d8_tx_2x_handle_t          *handle,
                                             UINT8                       serdes_id,
                                             d8_tx_2x_tx_swing_level_t   tx_swing_level);
    
    
PUBLIC PMC_ERROR d8_tx_2x_tx_swing_level_get(d8_tx_2x_handle_t          *handle,
                                             UINT8                       serdes_id,
                                             d8_tx_2x_tx_swing_level_t  *tx_swing_level);

PUBLIC BOOL8 d8_tx_2x_loopback_activate_status_get(d8_tx_2x_handle_t *handle,
                                                   UINT8              serdes_id);

PUBLIC void d8_tx_2x_ctxt_cleanup(d8_tx_2x_handle_t *d8_tx_2x_handle);


PUBLIC PMC_ERROR d8_tx_2x_handle_restart_init(d8_tx_2x_handle_t              *d8_tx_2x_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                              BOOL8                           reset);

PUBLIC BOOL8 d8_tx_2x_reset_state_get(d8_tx_2x_handle_t *handle,
                                      UINT32             lane);

PUBLIC PMC_ERROR d8_tx_2x_ext_ctxt_state_set(d8_tx_2x_handle_t      *d8_tx_2x_handle, 
                                             UINT16                  lane_msk,
                                             BOOL8                   out_of_reset);

PUBLIC void d8_tx_2x_polarity_get(d8_tx_2x_handle_t *handle,
                                  UINT8              serdes_id,
                                  BOOL8             *enable);

#ifdef __cplusplus
}
#endif

#endif /* _D8_TX_2X_H */

/*
** end of file
*/
