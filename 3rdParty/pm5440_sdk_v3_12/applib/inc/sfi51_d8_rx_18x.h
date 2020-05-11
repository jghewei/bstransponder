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

#ifndef _SFI51_D8_RX_18X_H
#define _SFI51_D8_RX_18X_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "sfi51_d8_rx_18x_api.h"
#include "d8_rx_2x.h"
#include "util_global.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: sfi51_d8_tx_18x_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify SFI51_D8_RX_18X port state 
*
* ELEMENTS:
*   SFI51_D8_TX_18X_RESET         - reset state
*   SFI51_D8_TX_18X_START         - start state
*           
*******************************************************************************/
typedef enum {
    SFI51_D8_RX_18X_RESET        = 0,
    SFI51_D8_RX_18X_START        = 1,
    LAST_SFI51_D8_RX_18X_STATE
} sfi51_d8_rx_18x_state_t;

/*
** Constants
*/
/* Maximum number of serdes slices in block */
#define SFI51_D8_RX_NUM_SLICES 9

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: sfi51_d8_rx_18x_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct is for dynamically changing context.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    sfi51_d8_rx_18x_state_t state;
    BOOL8 loopback_enable;
} sfi51_d8_rx_18x_var_t;

/*******************************************************************************
* STRUCTURE: sfi51_d8_rx_18x_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   D8_RX_2X handle structure.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct sfi51_d8_rx_18x_handle_struct_t {
    pmc_handle_t base;
    d8_rx_2x_handle_t **d8_rx_2x_hndl;
    sfi51_d8_rx_18x_var_t var;

} sfi51_d8_rx_18x_handle_struct_t;

/*! SFI51_D8_RX_18X block handle used by its parent */
typedef struct sfi51_d8_rx_18x_handle_struct_t sfi51_d8_rx_18x_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_ctxt_create(pmc_handle_t *parent, 
                                                             UINT32 base_address, 
                                                             pmc_sys_handle_t *sys_handle, 
                                                             const char *tsb_name );
PUBLIC void sfi51_d8_rx_18x_ctxt_destroy(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle);
    
PUBLIC void sfi51_d8_rx_18x_handle_init(sfi51_d8_rx_18x_handle_t *sfi51_d8_rx_18x_handle);
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr); 
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_resync(sfi51_d8_rx_18x_handle_t *handle,
                                                 UINT8 serdes_id); 
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_mon_err_count_get(sfi51_d8_rx_18x_handle_t *handle,
                                                        UINT8 serdes_id,
                                                        util_patt_status_t    *status,
                                                        UINT32 *err_count);    
PUBLIC PMC_ERROR sfi51_d8_rx_18x_patt_gen_cfg(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR sfi51_d8_rx_18x_loopback_activate(sfi51_d8_rx_18x_handle_t *handle);
PUBLIC PMC_ERROR sfi51_d8_rx_18x_loopback_deactivate(sfi51_d8_rx_18x_handle_t *handle);                 

PUBLIC PMC_ERROR sfi51_d8_rx_18x_polarity_cfg(sfi51_d8_rx_18x_handle_t *handle,   
                                              UINT8 serdes_id,
                                              BOOL8 enable);

PUBLIC PMC_ERROR sfi51_d8_rx_18x_txdclk_enable(sfi51_d8_rx_18x_handle_t *handle);

PUBLIC PMC_ERROR sfi51_d8_rx_18x_txdclk_disable(sfi51_d8_rx_18x_handle_t *handle);

PUBLIC PMC_ERROR sfi51_d8_rx_18x_handle_restart_init(sfi51_d8_rx_18x_handle_t       *sfi51_d8_rx_18x_handle,
                                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                     BOOL8                           sfi51_d8_rx_18x_reset_msk);

PUBLIC BOOL8 sfi51_d8_rx_18x_reset_state_get(sfi51_d8_rx_18x_handle_t *handle);

PUBLIC PMC_ERROR sfi51_d8_rx_18x_ext_ctxt_state_set(sfi51_d8_rx_18x_handle_t      *sfi51_d8_rx_18x_handle, 
                                                    BOOL8                          out_of_reset);
 
PUBLIC PMC_ERROR sfi51_d8_rx_18x_polarity_get(sfi51_d8_rx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                    *enable);

#ifdef __cplusplus
}
#endif

#endif /* _SFI51_D8_RX_18X_H */

/*
** end of file
*/
