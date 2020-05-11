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
#ifndef _SIFD_LOC_H
#define _SIFD_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/



#include "sifd.h"
#include "sifd_dft_io.h"                         
#include "ilkn_io.h"
#include "copi_io.h"
#include "sys_dsis_io.h"




/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: sifd_srds_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SERDES lanes.
*
* ELEMENTS:
*       SIFD_SRDS_E1_LR                - SERDES lane on E1 connector
*       SIFD_SRDS_N1_SR                - SERDES lane on N1 connector
*       SIFD_SRDS_N2_SR                - SERDES lane on N2 connector
*
*******************************************************************************/
typedef enum {
    SIFD_SRDS_E1_LR    = 0,
    SIFD_SRDS_N1_SR    = 1,
    SIFD_SRDS_N2_SR    = 2,
    SIFD_LAST_SRDS
} sifd_srds_type_t;

/*******************************************************************************
* ENUM: sifd_ilkn_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Interlaken block 'Major Bidirectional States'.
*
* ELEMENTS:
*       SIFD_COPI_ILKN_STATE_INVALID     - invalid 'Major Bidirectional State'
*       SIFD_COPI_ILKN_STATE_INRESET     - 'Interlaken in reset'
*       SIFD_COPI_ILKN_STATE_STANDBY     - 'Interlaken in standby'
*       SIFD_COPI_ILKN_STATE_OPERATIONAL - 'Interlaken is operational'
*
*******************************************************************************/
typedef enum {
    SIFD_COPI_ILKN_STATE_INVALID     = 0,
    SIFD_COPI_ILKN_STATE_INRESET     = 1,
    SIFD_COPI_ILKN_STATE_STANDBY     = 2,
    SIFD_COPI_ILKN_STATE_OPERATIONAL = 3
} sifd_ilkn_state_t;

/*
** Constants
*/

#define SIFD_MAX_E1_LANES         40
#define SIFD_MAX_E1_LANES_PER_DEV 24
#define SIFD_MAX_N1_LANES         16
#define SIFD_MAX_N2_LANES          8
#define SIFD_MAX_FIC_LANES        20
#define SIFD_MAX_DSIS_LANES       12
#define SIFD_MAX_LOTN_LANES        8

#define SIFD_MAX_E1_TO_ILKN1      20
#define SIFD_MAX_N1_TO_ILKN1      16
#define SIFD_MIN_N2_TO_ILKN1      15
#define SIFD_MAX_N2_TO_ILKN1      24

#define SIFD_MAX_E1_TO_ILKN2      24
#define SIFD_MAX_N1_TO_ILKN2       8
#define SIFD_MIN_N2_TO_ILKN2       7
#define SIFD_MAX_N2_TO_ILKN2      16

#define SIFD_MAX_CPB_ARB_CHAN    150
#define SIFD_MAX_ODUK_ARB_CHAN   104
#define SIFD_MAX_ARB_CHAN        (SIFD_MAX_CPB_ARB_CHAN + SIFD_MAX_ODUK_ARB_CHAN)

#define SIFD_MAX_ILKN_INTERFACE    2

#define SIFD_MINIMUM_INTERNAL_LANE_NUM      0
#define SIFD_MAXIMUM_INTERNAL_LANE_NUM      11

#define SIFD_MINIMUM_SERDES_LANE_NUM        0
#define SIFD_MAXIMUM_SERDES_LANE_NUM        11
#define SIFD_MAX_SERDES_LANES               SIFD_MAXIMUM_SERDES_LANE_NUM + 1
#define SIFD_MAXIMUM_LIFD_SERDES_LANE_NUM   19

#define SIFD_CBRC_DATA_ENABLE               0
#define SIFD_OTN_DATA_ENABLE                1
#define SIFD_ENET_DATA_ENABLE               2
#define SIFD_SERDES_PORT_NO_LOS_SOURCE      0xFD  /* LOS DISABLED */

/******************************************************************************
 * defines for constants in hardware                                          *
 ******************************************************************************
 *                                                                            * 
 *                                       Hex      Binary                      *
 ******************************************************************************/

/* Common to both ILKN1 and ILKN2 - not really        */
#define SIFD_ILKN_LANE_ENABLE            0x00  /* 0000 0000 */
#define SIFD_ILKN_LANE_DISABLE           0x01  /* 0000 0001 */
#define SIFD_ILKN_DLOS_ENABLE            0x01  /* 0000 0001 */
#define SIFD_ILKN_DLOS_DISABLE           0x00  /* 0000 0000 */
#define SIFD_ILKN_DLOS_INVERT            0x01  /* 0000 0001 */
#define SIFD_ILKN_DLOS_NOT_INVERT        0x00  /* 0000 0000 */
#define SIFD_ILKN_N1_SELECT              0x00  /* 0000 0000 */
#define SIFD_ILKN1_N2_SELECT             0x00  /* 0000 0000 */
#define SIFD_ILKN2_N2_SELECT             0x01  /* 0000 0001 */
#define SIFD_ILKN1_E1_SELECT             0x01  /* 0000 0001 */
#define SIFD_ILKN2_E1_SELECT_0_7         0x01  /* 0000 0001 */
#define SIFD_ILKN2_E1_SELECT_8_23        0x00  /* 0000 0000 */

/* For DSIS */
#define SIFD_DSIS_LANE_ENABLE            0x00  /* 0000 0000 */
#define SIFD_DSIS_LANE_DISABLE           0x01  /* 0000 0001 */
#define SIFD_DSIS_LOS_INVERT             0x01  /* 0000 0001 */
#define SIFD_DSIS_LOS_NOT_INVERT         0x00  /* 0000 0001 */
#define SIFD_DSIS_LOS_ENABLE             0x01  /* 0000 0001 */
#define SIFD_DSIS_LOS_DISABLE            0x00  /* 0000 0000 */
#define SIFD_DSIS_LOS_P_INVERT           0x01  /* 0000 0001 */
#define SIFD_DSIS_LOS_P_NOT_INVERT       0x00  /* 0000 0001 */

/* For ACB */
/* See ACB RegDoc pg. 73 */
#define SIFD_N1_SERD_ACB_SELECT_DSIS     0x00  /* 0000 0000 */
#define SIFD_N1_SERD_ACB_SELECT_LOTN     0x01  /* 0000 0001 */


/* For LOTN */
#define SIFD_LOTN_LANE_ENABLE            0x00  /* 0000 0000 */
#define SIFD_LOTN_LANE_DISABLE           0x01  /* 0000 0001 */


/* For E1 SERDES */
#define SIFD_E1_LANE_SELECT_ILKN2        0x00  /* 0000 0000 */
#define SIFD_E1_LANE_SELECT_FIC          0x01  /* 0000 0001 */
#define SIFD_E1_LANE_SELECT_ILKN1_20_23  0x02  /* 0000 0010 */
#define SIFD_E1_LANE_SELECT_ILKN1_24_39  0x00  /* 0000 0000 */
/* For N1 SERDES */
#define SIFD_N1_LANE_SELECT_ILKN1        0x00  /* 0000 0000 */
#define SIFD_N1_LANE_SELECT_DSIS         0x01  /* 0000 0001 */
#define SIFD_N1_LANE_SELECT_FIC          0x02  /* 0000 0010 */
#define SIFD_N1_LANE_SELECT_LOTN         0x03  /* 0000 0011 */
#define SIFD_N1_LANE_SELECT_ILKN2        0x03  /* 0000 0011 */
/* Note: SIFD_N1_LANE_SELECT_ILKN2 is intentionally the same as above 
         This is by design
*/

/* For N2 SERDES */
#define SIFD_N2_LANE_SELECT_ILKN1        0x00  /* 0000 0000 */
#define SIFD_N2_LANE_SELECT_FIC          0x02  /* 0000 0010 */
#define SIFD_N2_LANE_SELECT_ILKN2        0x03  /* 0000 0011 */

/* For ILKN to SERDES lane mapping */
#define SIFD_ILKN_MAX_SERDES_LANES       24
#define SIFD_NO_LANE                     0xFF

/* For indirect register access */
#define SIFD_ILKN_READ_CMD               0x01
#define SIFD_ILKN_WRITE_CMD              0x02

/*
** Macro Definitions
*/
#define SIFD_COPI_INT0_ENABLE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_E0_set(NULL, handle, link, i, enable); \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define SIFD_COPI_INT0_CLEAR_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_I0_set_to_clear(NULL, handle, link, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define SIFD_COPI_INT0_RETRIEVE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##_I0_get(tsb_name##_buffer_ptr, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define SIFD_COPI_INT0_CHECK_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; (i < array_len) && (FALSE == *int_found_ptr); i++) {            \
        if (TRUE == int_en_table_ptr->struct_field[i]) {                        \
            *int_found_ptr = tsb_name##_field_##tsb_field##_I0_get(NULL, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define SIFD_ILKN_INTM_ENABLE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_M_set(NULL, handle, link, i, !enable); \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define SIFD_ILKN_INTM_CLEAR_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_I_set(NULL, handle, link, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

/*
** Structures and Unions
*/

/* This is the config struct for initial values. */
/*******************************************************************************
* STRUCTURE: sifd_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining static SIFD module data.
*
* ELEMENTS:
*
*******************************************************************************/
typedef struct {
    
} sifd_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_los_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining pin LOS context 
*
* ELEMENTS:
*       active        - pin active state
*       los_cfg       - see sifd_los_cfg_t
*
*******************************************************************************/
typedef struct {
    BOOL8 active;
    sifd_los_cfg_t los_cfg;    
} sifd_los_ctxt_t;

/*******************************************************************************
* STRUCTURE: sifd_var_fc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining context that dynamically change during configuration
*   of the device associated with ILKN flow control.
*
* ELEMENTS:
*   flow_control_mode         - see sifd_fc_mode_t
*   calendar_length           - flow control calendar length
*   tx_flow_control_calendar  - Tx calendar see sifd_ilkn_calendar_t
*   rx_flow_control_calendar  - Rx calendar see sifd_ilkn_calendar_t
*   tx_link_flow_control      - Tx link level flow control config
*   tx_channel_flow_control   - Tx CPB intf channel level flow control config
*   tx_oduk_channel_flow_control - Tx ODUKSW intf channel level flow control config
*   rx_channel_flow_control   - Rx channel level flow control config
*
*******************************************************************************/
typedef struct {
    sifd_fc_mode_t flow_control_mode;
    UINT32 calendar_length;
    sifd_ilkn_calendar_t tx_flow_control_calendar;
    sifd_ilkn_calendar_t rx_flow_control_calendar;
    BOOL tx_link_flow_control;
    BOOL tx_channel_flow_control;
    BOOL tx_oduk_channel_flow_control;
    BOOL rx_link_flow_control;
    BOOL rx_channel_flow_control;
} sifd_var_fc_t;

/*******************************************************************************
* STRUCTURE: sifd_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining context that dynamically change during configuration
*   of the device.
*
* ELEMENTS:
*   ilkn_state         - ILKN instance energy state
*   ilkn_fc            - ILKN instance flow control context see sifd_var_fc_t
*   internal_lane_tx   - Tx DSIS internal resource management
*   internal_lane_rx   - Rx DSIS internal resource management
*   loopback           - ILKN instance loopback state, see sifd_loopback_cfg_t
*   test_control_state - ILKN instance test feature context, see sifd_ilkn_test_control_state_t
*   los_ctxt           - Rx Serdes Pin LOS context, see sifd_los_ctxt_t
*
*******************************************************************************/
typedef struct {
    pmc_block_reg_access_state_enum ilkn_state[SIFD_MAX_ILKN_INTERFACE];
    sifd_var_fc_t ilkn_fc[SIFD_MAX_ILKN_INTERFACE];
    UINT32 internal_lane_tx[SIFD_MAX_SERDES_LANES];
    UINT32 internal_lane_rx[SIFD_MAX_SERDES_LANES];
    sifd_loopback_cfg_t loopback[SIFD_MAX_ILKN_INTERFACE];
    sifd_ilkn_test_control_state_t **test_control_state;
    sifd_los_ctxt_t los_ctxt[SIFD_MAX_SERDES_LANES];
} sifd_var_t;

/*******************************************************************************
* STRUCTURE: sifd_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SIFD module instance handle.
*
* ELEMENTS:
*       base        - see pmc_handle_t
*       cfg         - static data associated with SIFD module instance
*       var         - dynamic data associated with SIFD module instance
*
*******************************************************************************/
struct sifd_handle_struct_t {
    pmc_handle_t base;
    
    sifd_cfg_t cfg;
    sifd_var_t var;
};

/*******************************************************************************
* STRUCTURE: copi_cpb_oduk_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COPI configuration.
*
* ELEMENTS:
*       oduk_rx_chext_en         - ODUK RX multi-use bit
*       oduk_tx_chext_en         - ODUK TX multi-use bit
*       oduk_tx_burst            - ODUK max burst size (128, 192 or 256 bytes)
*       oduk_tx_enabled          - ODUK enable (TRUE is enabled & FALSE disabled)
*       oduk_tx_mode             - ODUK in packet or segment mode
*       oduk_hdr_insert_extract  - ODUK header (2B, 4B or 6B) insert/extract
*       cpb_rx_type_en           - CPB RX type field
*       cpb_tx_type_en           - CPB TX type field
*       cpb_tx_burst             - CPB max burst size (128, 192 or 256 bytes)
*       cpb_tx_enabled           - CPB enable (TRUE is enabled & FALSE disabled)
*       cpb_tx_mode              - CPB in packet or segment mode
*       rx_burstmax              - COPI RX max burst size (128, 192 or 256 bytes)
*       rx_mode                  - COPI RX in packet or segment mode
*
*******************************************************************************/
typedef struct {
    BOOL                   oduk_rx_chext_en;
    BOOL                   oduk_tx_chext_en;
    sifd_oc_burst_t        oduk_tx_burst; 
    BOOL                   oduk_tx_enabled;
    sifd_mode_t            oduk_tx_mode;
    sifd_oduk_hdr_insert_t oduk_hdr_insert_extract;
    BOOL                   oduk_2b_hdr_mode;
    BOOL                   cpb_rx_type_en;
    BOOL                   cpb_tx_type_en;
    sifd_oc_burst_t        cpb_tx_burst; 
    BOOL                   cpb_tx_enabled;
    sifd_mode_t            cpb_tx_mode;
    sifd_oc_burst_t        rx_burstmax;
    sifd_mode_t            rx_mode;
} copi_cpb_oduk_cfg_t; 


/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _SIFD_LOC_H */

/* 
** end of file 
*/

