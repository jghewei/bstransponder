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

#ifndef _SFI51_D8_TX_18X_H
#define _SFI51_D8_TX_18X_H

#ifdef __cplusplus 
extern "C" { 
#endif 

/* 
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "sfi51_d8_tx_18x_api.h"
#include "d8_tx_2x.h"
#include "acb_ft.h"
#include "acb_ft_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: sfi51_d8_tx_18x_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify SFI51_D8_TX_18X port state 
*
* ELEMENTS:
*   SFI51_D8_TX_18X_RESET         - reset state
*   SFI51_D8_TX_18X_START         - start state
*   SFI51_D8_TX_18X_EQUIPPED      - provisioned state
*   SFI51_D8_TX_18X_OPERATIONAL   - activated and operational state
*   SFI51_D8_TX_18X_OPERATIONAL_LOOPBACK                   - activated and operational state (in loopback mode)
*   SFI51_D8_TX_18X_OPERATIONAL_FORCE_NOMINAL              - activated and operational state (in loopback mode)
*   SFI51_D8_TX_18X_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL - activated and operational state (in loopback mode)
*           
*******************************************************************************/
typedef enum {
    SFI51_D8_TX_18X_RESET        = 0,
    SFI51_D8_TX_18X_START        = 1,
    SFI51_D8_TX_18X_EQUIPPED     = 2,
    SFI51_D8_TX_18X_OPERATIONAL  = 3,
    SFI51_D8_TX_18X_OPERATIONAL_LOOPBACK  = 4,
    SFI51_D8_TX_18X_OPERATIONAL_FORCE_NOMINAL  = 5,
    SFI51_D8_TX_18X_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL  = 6,
    LAST_SFI51_D8_TX_18X_STATE
} sfi51_d8_tx_18x_state_t;

/*******************************************************************************
* ENUM: sfi51_d8_tx_18x_port_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify OTU rate
*
* ELEMENTS:
*   SFI51_D8_TX_18X_OTU3    
*   SFI51_D8_TX_18X_OTU3E1    
*   SFI51_D8_TX_18X_OTU3E2   
*           
*******************************************************************************/
typedef enum {
    SFI51_D8_TX_18X_OTU3        = 0,
    SFI51_D8_TX_18X_OTU3E1      = 1,
    SFI51_D8_TX_18X_ODU3E2      = 2,
    SFI51_D8_TX_18X_OC_768      = 3,
} sfi51_d8_tx_18x_port_mode_t;
/*
** Constants
*/

/*
** Macro Definitions
*/

/* Maximum number of serdes slices in block */
#define SFI51_D8_TX_NUM_SLICES 9

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: sfi51_d8_tx_18x_csu_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   sfi51_d8_tx_18x block CSU properties saved in context.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8 valid;
    UINT64 csu_n;
    UINT64 csu_denomin;
    UINT32 csu_r;
    UINT32 ppm_max;
    
} sfi51_d8_tx_18x_csu_ctxt_t;

/*******************************************************************************
* STRUCTURE: sfi51_d8_tx_18x_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct is for dynamically changing context.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    sfi51_d8_tx_18x_state_t state;
    sfi51_d8_tx_18x_state_t    prev_prev_state;     
    sfi51_d8_tx_18x_state_t    prev_state;  
    BOOL8 use_acb;
    BOOL8 loopback_enable;
    BOOL8 has_lpbk_acb_ft_backup;
    BOOL8 has_force_nominal_acb_ft_backup;
    BOOL8 has_loop_timing_enabled;
    sfi51_d8_tx_18x_csu_ctxt_t csu_ctxt_backup;
} sfi51_d8_tx_18x_var_t;

/*******************************************************************************
* STRUCTURE: sfi51_d8_tx_18x_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   D8_TX_2X handle structure.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct sfi51_d8_tx_18x_handle_struct_t {
    pmc_handle_t base;
    d8_tx_2x_handle_t **d8_tx_2x_hndl;

    sfi51_d8_tx_18x_var_t var;

    /* ACB_FT handle */
    acb_ft_handle_t *acb_ft_handle; 
} sfi51_d8_tx_18x_handle_struct_t;

/*! SFI51_D8_TX_18X block handle used by its parent */
typedef struct sfi51_d8_tx_18x_handle_struct_t sfi51_d8_tx_18x_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_ctxt_create(pmc_handle_t *parent, 
                                                             UINT32 base_address, 
                                                             pmc_sys_handle_t *sys_handle, 
                                                             const char *tsb_name );
PUBLIC void sfi51_d8_tx_18x_ctxt_destroy(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle);    
PUBLIC void sfi51_d8_tx_18x_handle_init(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_mon_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_gen_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8 serdes_id,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_patt_gen_err_insert_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                                         UINT8 serdes_id);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_prov(sfi51_d8_tx_18x_handle_t *handle, acb_ft_prov_cfg_t *cfg_ptr);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_deprov(sfi51_d8_tx_18x_handle_t *handle);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_activate(sfi51_d8_tx_18x_handle_t *handle);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_deactivate(sfi51_d8_tx_18x_handle_t *handle);                             
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_activate(sfi51_d8_tx_18x_handle_t *handle);
PUBLIC PMC_ERROR sfi51_d8_tx_18x_loopback_deactivate(sfi51_d8_tx_18x_handle_t *handle);                     

PUBLIC PMC_ERROR sfi51_d8_tx_18x_polarity_cfg(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8 enable);


PUBLIC PMC_ERROR sfi51_d8_tx_18x_swing_level_set(sfi51_d8_tx_18x_handle_t    *handle,
                                                 UINT8                        serdes_id,
                                                 d8_tx_2x_tx_swing_level_t    tx_swing_level);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_swing_level_get(sfi51_d8_tx_18x_handle_t   *handle,
                                                 UINT8                       serdes_id,
                                                 d8_tx_2x_tx_swing_level_t  *tx_swing_level);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_loop_timing_cfg(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                                 sfi51_d8_tx_18x_port_mode_t port_mode, 
                                                 BOOL enable);

PUBLIC void sfi51_d8_tx_18x_max_ppm_set(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                        UINT32                    max_ppm);

PUBLIC void sfi51_d8_tx_18x_squelch_default_set(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_tx_refclk_source_set(sfi51_d8_tx_18x_handle_t          *sfi51_d8_tx_18x_handle,
                                                      util_global_sfi51_refclk_source_t  refclk_src);

PUBLIC void sfi51_d8_tx_18x_csu_div_val_get(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle, 
                                             DOUBLE *csu_div, 
                                             UINT64 *csu_n, 
                                             UINT64 *csu_denomin, 
                                             UINT32 *csu_r);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_handle_restart_init(sfi51_d8_tx_18x_handle_t        *sfi51_d8_tx_18x_handle,
                                                     util_global_restart_init_cfg_t  *restart_init_cfg_ptr,
                                                     BOOL8                            reset);

PUBLIC BOOL8 sfi51_d8_tx_18x_reset_state_get(sfi51_d8_tx_18x_handle_t *handle);

PUBLIC PMC_ERROR sfi51_d8_tx_18x_ext_ctxt_state_set(sfi51_d8_tx_18x_handle_t      *sfi51_d8_tx_18x_handle, 
                                                    BOOL8                          out_of_reset);

PUBLIC void sfi51_d8_tx_18x_restart(sfi51_d8_tx_18x_handle_t *sfi51_d8_tx_18x_handle);
    
PUBLIC PMC_ERROR sfi51_d8_tx_18x_polarity_get(sfi51_d8_tx_18x_handle_t *handle,
                                              UINT8                     serdes_id,
                                              BOOL8                    *enable);
#ifdef __cplusplus
}
#endif

#endif /* _SFI51_D8_TX_18X_H */

/*
** end of file
*/
