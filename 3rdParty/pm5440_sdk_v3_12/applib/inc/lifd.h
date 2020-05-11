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

#ifndef _LIFD_H
#define _LIFD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_sys.h"
#include "util_global.h"

#include "lifd_api.h"

/*
** Enumerated Types
*/
/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: xifd_dsis_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to define the subsystems which may be connected to DSIS block.
*
* ELEMENTS:
*       XIFD_DSIS_NONE                 - Init or default value.
*       XIFD_DSIS_CBRC                 - CBRC SS
*       XIFD_DSIS_OTN                  - OTN SS
*       XIFD_DSIS_ENET                 - ENET SS
*       XIFD_LAST_DSIS                 - out of range value
*
*******************************************************************************/
typedef enum {

    XIFD_DSIS_NONE     = 0,
    XIFD_DSIS_CBRC     = 1,
    XIFD_DSIS_OTN      = 2,
    XIFD_DSIS_ENET     = 3,
    XIFD_DSIS_LAST

} xifd_dsis_type_t;

/*******************************************************************************
* ENUM: lifd_sig_src_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to define the CKCTL ACB timing conrol C input data source
*
* ELEMENTS:
*       LIFD_SRC_NONE            - Default value.
*       LIFD_SRC_CBRC            - CBRC SS
*       LIFD_SRC_ENET            - ENET SS
*       LIFD_SRC_OTN_IPM_CBRC    - CBRC SS Zone, FO2 SS Dithering
*       LIFD_SRC_OTN_IPM_ENET    - ENET SS Zone, FO2 SS Dithering
*       LIFD_SRC_OTN_LPM         - COREOTN FO1 SS
*       LIFD_SRC_OTN_EPM         - COREOTN CTRL SS
*       LIFD_SRC_DIG_M1          - Internal DIGI M1
*       LIFD_LAST_SRC            - out of range value
*
*******************************************************************************/
typedef enum {

    LIFD_SRC_NONE          = 0,
    LIFD_SRC_CBRC          = 1,
    LIFD_SRC_ENET          = 2,
    LIFD_SRC_OTN_IPM_CBRC  = 3,
    LIFD_SRC_OTN_IPM_ENET  = 4,
    LIFD_SRC_OTN_LPM       = 5,
    LIFD_SRC_OTN_EPM       = 6,
    LIFD_SRC_DIG_M1        = 7,
    LIFD_LAST_SRC

} lifd_sig_src_type_t;

/*******************************************************************************
* ENUM: lifd_sfi5point1_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to define the SFI 5.1 Instance.
*
* ELEMENTS:
*       LIFD_SFI5POINT1_NONE                 - No instance
*       LIFD_SFI5POINT1_ONE                  - Instance 1
*       LIFD_SFI5POINT1_TWO                  - Instance 2
*       LIFD_SFI5POINT1_LAST                 - out of range value
*                        
*
*******************************************************************************/
typedef enum {

    LIFD_SFI5POINT1_NONE = 0,
    LIFD_SFI5POINT1_ONE  = 1,
    LIFD_SFI5POINT1_TWO  = 2,
    LIFD_SFI5POINT1_LAST

} lifd_sfi5point1_int_t;

/*
** Constants
*/

#define LIFD_SERDES_DSIS_LANES_MAX          20
#define LIFD_MAXIMUM_INTERNAL_LANE_NUM      11
#define LIFD_MAXIMUM_SUBSYSTEM_LANE         11

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: lifd_los_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the LOS configuration.
*
* ELEMENTS:
*       pin_los_inv     - LOS inversion control
*       pin_los_en      - LOS enable control
*       los_pol_inv     - LOS pin polarity control
*
*******************************************************************************/
typedef struct {
    BOOL pin_los_inv;
    BOOL pin_los_en;
    BOOL los_pol_inv;
} lifd_los_cfg_t;

/*******************************************************************************
* STRUCTURE: lifd_los_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining pin LOS context
*
* ELEMENTS:
*       active        - pin active state
*       los_cfg       - see lifd_los_cfg_t
*
*******************************************************************************/
typedef struct {
    BOOL8 active;
    lifd_los_cfg_t los_cfg;
} lifd_los_ctxt_t;


/*******************************************************************************
* STRUCTURE: lifd_rxs_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SFI5.1 RXS confirguration structure.
*
* ELEMENTS:
*       rxs_inv        - See register description SFI1_RXS_INV and SFI2_RXS_INV
*       rxs_en         - see register description SFI1_RXS_EN and SFI2_RXS_EN
*
*******************************************************************************/
typedef struct {
    BOOL rxs_inv;
    BOOL rxs_en;
} lifd_rxs_cfg_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC lifd_handle_t *lifd_ctxt_create(pmc_handle_t *parent,
                                       UINT32 base_address,
                                       pmc_sys_handle_t *sys_handle,
                                       const char *tsb_name);
PUBLIC void lifd_ctxt_destroy(lifd_handle_t *lifd_handle);
PUBLIC void lifd_handle_init(lifd_handle_t *lifd_handle);
PUBLIC PMC_ERROR lifd_handle_restart_init(lifd_handle_t *lifd_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR lifd_serdes_subsystem_configure(lifd_handle_t *lifd_handle,
                                                 UINT32 subsystem_lane,
                                                 UINT32 serdes_rx_lane,
                                                 UINT32 serdes_tx_lane,
                                                 xifd_dsis_type_t subsystem_type,
                                                 BOOL single_lane_otn_cbrc,
                                                 UINT32 master_serdes_lane,
                                                 BOOL is_cfp_application);
PUBLIC PMC_ERROR lifd_serdes_subsystem_disable(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_lane,
                                               UINT32 subsystem_lane);
PUBLIC PMC_ERROR lifd_enet_clock_select(lifd_handle_t *lifd_handle,
                                        UINT32 subsystem_lane,
                                        BOOL enet_clk_enable);
PUBLIC PMC_ERROR lifd_serdes_c_ckctl_configure(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_lane,
                                               lifd_sig_src_type_t clk_source);
PUBLIC void lifd_serdes_c_ckctl_source_lane_get(lifd_handle_t *lifd_handle,
                                                UINT32 serdes_lane,
                                                UINT32 *internal_acb_lane_ptr);
PUBLIC PMC_ERROR lifd_sfi_c_ckctl_source_select(lifd_handle_t *lifd_handle,
                                                lifd_sig_src_type_t clk_source,
                                                lifd_sfi5point1_int_t sfi_num);
PUBLIC PMC_ERROR lifd_sfi_subsystem_select(lifd_handle_t *lifd_handle,
                                           lifd_sfi5point1_int_t sfi_num,
                                           BOOL swap_sfi_one_and_two,
                                           xifd_dsis_type_t subsystemtype,
                                           BOOL enable,
                                           lifd_rxs_cfg_t rxs_config);
PUBLIC PMC_ERROR lifd_serdes_los_cfg_get(lifd_handle_t *lifd_handle,
                                         UINT32 serdes_lane,
                                         lifd_los_cfg_t* los_cfg);
PUBLIC PMC_ERROR lifd_serdes_los_force(lifd_handle_t *lifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 dlos_en, BOOL8 los_inv);
PUBLIC PMC_ERROR lifd_serdes_los_force_get(lifd_handle_t *lifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 *dlos_en, BOOL8 *los_pol_inv);
PUBLIC PMC_ERROR lifd_los_internal_context_set(lifd_handle_t *lifd_handle,
                                               UINT32 los_pin,
                                               UINT32 los_pin_val,
                                               BOOL is_line_side);
PUBLIC PMC_ERROR lifd_los_internal_context_unset(lifd_handle_t *lifd_handle,
                                                 UINT32 los_pin,
                                                 BOOL is_line_side);
PUBLIC PMC_ERROR lifd_internal_context_set(lifd_handle_t *lifd_handle,
                                           UINT32 serdes_pin,
                                           BOOL is_line_side);
PUBLIC PMC_ERROR lifd_dsis_los_cfg(lifd_handle_t *lifd_handle,
                                   UINT32 los_pin,
                                   UINT32 los_inv,
                                   UINT32 post_mux_los_mask,
                                   BOOL8 enable,
                                   lifd_los_cfg_t* los_cfg,
                                   BOOL8 use_dlos_los_source);
PUBLIC void lifd_internal_los_context_clear(lifd_handle_t *lifd_handle);
PUBLIC void lifd_internal_context_clear(lifd_handle_t *lifd_handle);
/* LCOV_EXCL_START */
PUBLIC void lifd_serdes_dsis_rx_sel_and_rx_en_set(lifd_handle_t *lifd_handle,
                                                  UINT32 subsystem_lane,
                                                  UINT32 internal_rx_lane,
                                                  UINT32 clock_enable);
/* LCOV_EXCL_STOP */

PUBLIC PMC_ERROR lifd_serdes_tx_if_sel_update(lifd_handle_t   *lifd_handle, 
                                              UINT32           subsystem_lane, 
                                              xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR lifd_serdes_tx_if_sel_update(lifd_handle_t    *lifd_handle, 
                                              UINT32            subsystem_lane, 
                                              xifd_dsis_type_t  subsystem_type);


#ifdef __cplusplus
}
#endif

#endif /* _LIFD_H */

/*
** end of file
*/
