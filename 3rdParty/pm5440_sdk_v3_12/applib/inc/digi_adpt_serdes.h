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
*   DESCRIPTION:
*     The file describes the serdes functionality of DIGI device.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_SERDES_H
#define _DIGI_ADPT_SERDES_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_serdes_prbs_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors mode in serdes.
*
* ELEMENTS:
*   DIGI_SERDES_PRBS_5      - PRBS mode 2^5 - 1
*   DIGI_SERDES_PRBS_7      - PRBS mode 2^7 - 1
*   DIGI_SERDES_PRBS_9      - PRBS mode 2^9 - 1
*   DIGI_SERDES_PRBS_11     - PRBS mode 2^11 - 1
*   DIGI_SERDES_PRBS_15     - PRBS mode 2^15 - 1
*   DIGI_SERDES_PRBS_23     - PRBS mode 2^23 - 1
*   DIGI_SERDES_PRBS_31     - PRBS mode 2^31 - 1
*   UTIL_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*
*******************************************************************************/
typedef enum 
{
    DIGI_SERDES_PRBS_5   = UTIL_PATT_MODE_PRBS_5,
    DIGI_SERDES_PRBS_7   = UTIL_PATT_MODE_PRBS_7,
    DIGI_SERDES_PRBS_9   = UTIL_PATT_MODE_PRBS_9,
    DIGI_SERDES_PRBS_11  = UTIL_PATT_MODE_PRBS_11,
    DIGI_SERDES_PRBS_15  = UTIL_PATT_MODE_PRBS_15,
    DIGI_SERDES_PRBS_23  = UTIL_PATT_MODE_PRBS_23,
    DIGI_SERDES_PRBS_31  = UTIL_PATT_MODE_PRBS_31,
    DIGI_SERDES_USER_DEF = UTIL_PATT_MODE_USER_DEF,

    LAST_DIGI_SERDES_PRBS,
} digi_serdes_prbs_t;

/*******************************************************************************
* ENUM: digi_serdes_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors status of serdes.
*
* ELEMENTS:
*   DIGI_SERDES_LOCKED          - Pattern monitor or generator is locked.
*   DIGI_SERDES_TRYING_TO_LOCK  - Pattern monitor or generator is trying to lock.
*   DIGI_SERDES_ZERO_DET        - Pattern monitor is receiving all logic 0.
*   DIGI_SERDES_DISABLED        - Pattern monitor or generator is disabled.
*
*******************************************************************************/
typedef enum 
{
    DIGI_SERDES_LOCKED         = UTIL_PATT_STATUS_LOCKED,
    DIGI_SERDES_TRYING_TO_LOCK = UTIL_PATT_STATUS_TRYING_TO_LOCK,
    DIGI_SERDES_ZERO_DET       = UTIL_PATT_STATUS_ZERO_DET,
    DIGI_SERDES_DISABLED       = UTIL_PATT_STATUS_DISABLED,
    
    LAST_DIGI_SERDES_STATUS
} digi_serdes_status_t;

/*******************************************************************************
* ENUM: digi_serdes_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for serdes direction. The
*   element terms have a syntax:
*      DIGI_SERDES_DIR_<application>
*
* ELEMENTS:
*   DIGI_SERDES_DIR_RX      - received direction
*   DIGI_SERDES_DIR_TX      - transmitted direction
*   DIGI_SERDES_DIR_RX_TX   - received and transmitted direction
*
*******************************************************************************/
typedef enum 
{
    DIGI_SERDES_DIR_RX    = 0x1,
    DIGI_SERDES_DIR_TX    = 0x2,
    DIGI_SERDES_DIR_RX_TX = 0x3,
    
    LAST_DIGI_SERDES_DIR
} digi_serdes_dir_t;

/*******************************************************************************
* ENUM: digi_tx_clk_div_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the clock divider of an extra tx serdes 
*   pin used as a clock. The element terms have a syntax:
*      DIGI_TX_CLK_DIV_<application>
*
* ELEMENTS:
*   DIGI_TX_CLK_DIV_16 - Divide Tx Serdes clock by 16
*   DIGI_TX_CLK_DIV_64 - Divide Tx Serdes clock by 64
*
*******************************************************************************/
typedef enum
{
    DIGI_TX_CLK_DIV_16 = DIGI_S16_CLK_DIVIDER_16,
    DIGI_TX_CLK_DIV_64 = DIGI_S16_CLK_DIVIDER_64,

    LAST_DIGI_TX_CLK_DIV /* this is for out of range error checking */
} digi_tx_clk_div_t;

/*******************************************************************************
* ENUM: digi_op_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the operation mode. The element terms 
*   have a syntax:
*      DIGI_OP_<application>
*
* ELEMENTS:
*   DIGI_OP_DEL         - operation to delete
*   DIGI_OP_ADD         - operation to add
*   DIGI_OP_OVERWRITE   - operation to overwrite
*
*******************************************************************************/
typedef enum
{
    DIGI_OP_DEL       = 0x0,
    DIGI_OP_ADD       = 0x1,
    DIGI_OP_OVERWRITE = 0x2,

    LAST_DIGI_OP_MODE /* this is for out of range error checking */
} digi_op_mode_t;

/*
** Structures and Unions
*/
typedef struct
{
    /* S16 Line SerDes */
    UINT64 s16_line_bin_buffer[DIGI_SERDES_XFI_LANES_MAX][64];
    /* S16 SYS SerDes */
    UINT64 s16_sys_bin_buffer[DIGI_SERDES_MAX_S16_IN_BP][64];
    /* T8 SerDes */
    UINT64 t8_bin_buffer[DIGI_SERDES_MAX_T8_E1_IN_BP_LR][64];
}digi_bin_buffer_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_serdes_prbs_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_serdes_dir_t dir,
                                   digi_serdes_prbs_t prbs_mode,
                                   BOOL inv_cfg,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_tx_err_inst_set(UINT32 dev_id,
                                        UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_serdes_rx_state_get(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_serdes_polarity_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT32 lane_mask,
                                     BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_lane_rx_state_get(UINT32 dev_id, 
                                          UINT32 chnl_id,
                                          UINT32 lane_id,
                                          util_patt_status_t *status,
                                          UINT32 *err_cnt);
PUBLIC PMC_ERROR adpt_serdes_bp_prbs_set(UINT32 dev_id,
                                      digi_serdes_dir_t dir,
                                      digi_serdes_prbs_t prbs_mode,
                                      BOOL inv_cfg,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_bp_tx_err_inst_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_serdes_bp_rx_state_get(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_serdes_bp_polarity_set(UINT32 dev_id,
                                        digi_ilkn_intf_t ilkn_type,
                                        digi_serdes_dir_t dir,
                                        UINT64 lane_msk,
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_ge_prbs_set(UINT32 dev_id,
                                      digi_serdes_dir_t dir,
                                      digi_ge_serdes_id_t ge_serdes,
                                      digi_serdes_prbs_t prbs_mode,
                                      BOOL inv_cfg,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_ge_tx_err_inst_set(UINT32 dev_id,
                                           digi_ge_serdes_id_t ge_serdes);
PUBLIC PMC_ERROR adpt_serdes_ge_rx_state_get(UINT32 dev_id,
                                         digi_ge_serdes_id_t ge_serdes);
PUBLIC PMC_ERROR adpt_serdes_ge_polarity_set(UINT32 dev_id,
                                        digi_serdes_dir_t dir,
                                        digi_ge_serdes_id_t ge_serdes,
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_start_adaptation(UINT32 dev_id,  
                                         hostmsg_adapt_intf_type_t serdes_if,
                                         UINT32 lane_0_19_msk,
                                         UINT32 lane_20_39_msk);
PUBLIC PMC_ERROR adpt_serdes_adapt_status_get(UINT32 dev_id,  
                                         serdes_adapt_status_t *serdes_status);
PUBLIC PMC_ERROR adpt_serdes_cfg_set(UINT32 dev_id, digi_serdes_cfg_t *serdes_cfg);
PUBLIC PMC_ERROR adpt_serdes_cfg_get(UINT32 dev_id, digi_serdes_cfg_t *serdes_cfg);
PUBLIC PMC_ERROR adpt_serdes_cfg_file_parse(UINT32 dev_id, 
                                       char *file_name,
                                       digi_serdes_cfg_t *serdes_cfg);
PUBLIC PMC_ERROR adpt_serdes_cfg_file_save(UINT32 dev_id, const char *file_name);
PUBLIC PMC_ERROR adpt_serdes_c8_values_set(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_c8_adapt_objects_t *c8_values);
PUBLIC PMC_ERROR adpt_serdes_c8_values_get(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_c8_adapt_objects_t *c8_values);
PUBLIC PMC_ERROR adpt_serdes_t8_values_set(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_t8_adapt_objects_t *t8_values);
PUBLIC PMC_ERROR adpt_serdes_t8_values_get(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_t8_adapt_objects_t *t8_values);
PUBLIC PMC_ERROR adpt_serdes_s16_values_set(UINT32 dev_id,  
                                        hostmsg_s16_intf_type_t if_type,
                                        UINT32 lane_id,
                                        serdes_s16_adapt_objects_t *s16_values);
PUBLIC PMC_ERROR adpt_serdes_s16_values_get(UINT32 dev_id,  
                                        hostmsg_s16_intf_type_t if_type,
                                        UINT32 lane_id,
                                        serdes_s16_adapt_objects_t *s16_values);
PUBLIC PMC_ERROR adpt_serdes_debug_cfg(UINT32 dev_id, BOOL skip_cfg);
PUBLIC PMC_ERROR adpt_serdes_debug_status_get(UINT32 dev_id, 
                                          BOOL clear_val,
                                          serdes_debug_t *serdes_status);
PUBLIC PMC_ERROR adpt_serdes_s16_tx_swing_set(UINT32 dev_id,  
                                          digi_serdes_intf_t s16_intf,
                                          UINT32 lane_id,
                                          UINT8 amplitude,
                                          UINT8 post_cursor,
                                          UINT8 pre_cursor);
PUBLIC PMC_ERROR adpt_serdes_c8_tx_swing_set(UINT32 dev_id,  
                                         UINT32 lane_id,
                                         UINT8 tx_dpeni1,
                                         UINT8 tx_dpeni2,
                                         UINT16 deni,
                                         UINT32 tx_psel);
PUBLIC PMC_ERROR adpt_serdes_t8_tx_swing_set(UINT32 dev_id,  
                                         UINT32 lane_id,
                                         UINT8 tx_dpeni1,
                                         UINT8 tx_dpeni2,
                                         UINT32 deni,
                                         UINT32 tx_psel);
PUBLIC PMC_ERROR adpt_serdes_sfi51_tx_swing_set(UINT32 dev_id,  
                                           UINT32 bus_id,
                                           UINT32 lane,
                                           d8_tx_2x_tx_swing_level_t tx_swing_level);
PUBLIC PMC_ERROR adpt_serdes_ge_tx_swing_set(UINT32 dev_id,  
                                         digi_ge_serdes_id_t ge_intf,
                                         d8_tx_2x_tx_swing_level_t tx_swing_level);
PUBLIC PMC_ERROR adpt_serdes_t8_histogram(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_serdes_t8_histogram_per_slice(UINT32 dev_id, UINT32 chnl);
PUBLIC PMC_ERROR adpt_serdes_vert_eye_opening(UINT32 dev_id,
                                          hostmsg_adapt_intf_type_t serdes_if,
                                          UINT64 lane_msk,
                                          util_vhist_test_length_t test_length,
                                          UINT64 userdef_length);
PUBLIC PMC_ERROR adpt_serdes_sfi51_tx_dclk_en(UINT32 dev_id, 
                                         UINT32 chnl_id,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_serdes_tx_refclk_div_cfg(UINT32 dev_id, 
                                         UINT32 chnl_id,
                                         UINT32 lane_msk,
                                         digi_tx_clk_div_t clk_divider);
PUBLIC PMC_ERROR adpt_serdes_port_recfg(UINT32 dev_id,
                                    UINT32 serdes_id,
                                    digi_serdes_intf_t serdes_type,
                                    UINT32 lane_tx[DIGI_SERDES_XFI_LANES_MAX],
                                    UINT32 lane_rx[DIGI_SERDES_XFI_LANES_MAX],
                                    UINT8 los_pin,
                                    digi_op_mode_t op_mode);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_SERDES_H */

/* 
** end of file 
*/

