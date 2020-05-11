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
*      Defines ENET_FEGE structures and function protypes used by upper 
*      layer block.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ENET_FEGE_H
#define _ENET_FEGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "enet_fege_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: enet_fege_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ENET_FEGE channel state definition.
*
* ELEMENTS:
*   ENET_FEGE_STATE_START         - start state
*   ENET_FEGE_STATE_EQUIPPED      - provisioned state
*   ENET_FEGE_STATE_OPERATIONAL   - activated and operational state
*           
*******************************************************************************/
typedef enum 
{
    ENET_FEGE_STATE_START        = 0,
    ENET_FEGE_STATE_EQUIPPED     = 1,
    ENET_FEGE_STATE_OPERATIONAL  = 2,
    LAST_ENET_FEGE_STATE
} enet_fege_state_t;

/*
** Constants
*/
/* Define ENET FEGE maximum and minimum frame size */
#define ENET_FEGE_RXMAXFRMSIZE_UPPERLT       0xFFFB
#define ENET_FEGE_TXMAXFRMSIZE_UPPERLT       0xFFFC
#define ENET_FEGE_TXMINFRMSIZE_LOWERLT       0x16
#define ENET_FEGE_L1RPP_AN_ABLTY_ADV_DFLT    0x1801
    
/*
** Macro Definitions
*/
#define ENET_FEGE_INT_TABLE_DEFINE() \
    ENET_FEGE_INT(enet_fege_handle, enet_fege, OVERFLOW, overflow_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_COMMA_DEL_CHG, rx_comma_del_chg_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_SOF, tx_sof_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_SOF, rx_sof_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_TSTMPR_TOD_SIGN_OVRFL, tx_tstmpr_tod_sign_ovfl_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_TSTMPR_TOD_SIGN_OVRFL, rx_tstmpr_tod_sign_ovfl_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_TSTMP_FIFO, tx_tstmp_fifo_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_LONG_LENGTH_ERROR, rx_long_length_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_IN_RANGE_LENGTH_ERROR, rx_inrange_length_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_LAYER1_ERROR, rx_layer1_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_SYMBOL_ERROR, rx_symbol_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FCS_ERROR, rx_fcs_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_MAC_CONTROL_FRAMES_OK, rx_mac_control_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_MAC_PAUSE_CONTROL_FRAMES_OK, rx_mac_pause_control_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_TAGGED_FRAMES_OK, rx_tagged_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_MULTICAST_FRAMES_OK, rx_multicast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_BROADCAST_FRAMES_OK, rx_broadcast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_UNICAST_FRAMES_OK, rx_unicast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_OCTETS, rx_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES, rx_frames_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_OCTETS_OK, rx_octets_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_OK, rx_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_VARIABLE, rx_variable_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_STATION_ADDRESS_FILTERED, rx_station_address_filtered_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1519_TO_MAX_SIZE, rx_frames_1519_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1024_TO_1518_OCTETS, rx_frames_1024_to_1518_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_512_TO_1023_OCTETS, rx_frames_512_to_1023_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_256_TO_511_OCTETS, rx_frames_256_to_511_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_128_TO_255_OCTETS, rx_frames_128_to_255_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_65_TO_127_OCTETS, rx_frames_65_to_127_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_64_OCTETS, rx_frames_64_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_SHORT_LENGTH_CRC_ERROR, rx_short_length_crc_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_SHORT_LENGTH_ERROR, rx_short_length_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_LONG_LENGTH_CRC_ERROR, rx_long_length_crc_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_2501_TO_MAX_SIZE, rx_frames_2501_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_2001_TO_2500_OCTETS, rx_frames_2001_to_2500_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1549_TO_2000_OCTETS, rx_frames_1549_to_2000_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1523_TO_1548_OCTETS, rx_frames_1523_to_1548_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1519_TO_1522_OCTETS, rx_frames_1519_to_1522_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_UNICAST_FRAMES_ATTEMPTED, tx_unicast_frames_attempted_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_MULTICAST_FRAMES_OK, tx_multicast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_BROADCAST_FRAMES_OK, tx_broadcast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_UNICAST_FRAMES_OK, tx_unicast_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_MAC_CONTROL_FRAMES_OK, tx_mac_control_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1523_TO_MAX_SIZE, tx_frames_1523_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_MAC_ERROR, tx_mac_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_SYSTEM_ERROR, tx_system_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_LONG_LENGTH_ERROR, tx_long_length_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_SHORT_LENGTH_ERROR, tx_short_length_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FCS_ERROR, tx_fcs_error_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_MAC_PAUSE_CONTROL_FRAMES_OK, tx_mac_pause_control_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_TAGGED_FRAMES_OK, tx_tagged_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_OCTETS, tx_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_OCTETS_OK, tx_octets_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_OK, tx_frames_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_2501_TO_MAX_SIZE, tx_frames_2501_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_2001_TO_2500_OCTETS, tx_frames_2001_to_2500_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1549_TO_2000_OCTETS, tx_frames_1549_to_2000_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1523_TO_1548_OCTETS, tx_frames_1523_to_1548_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1519_TO_1522_OCTETS, tx_frames_1519_to_1522_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, RX_FRAMES_1523_TO_MAX_SIZE, rx_frames_1523_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_VARIABLE, tx_variable_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1519_TO_MAX_SIZE, tx_frames_1519_to_max_size_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_1024_TO_1518_OCTETS, tx_frames_1024_to_1518_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_512_TO_1023_OCTETS, tx_frames_512_to_1023_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_256_TO_511_OCTETS, tx_frames_256_to_511_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_128_TO_255_OCTETS, tx_frames_128_to_255_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_65_TO_127_OCTETS, tx_frames_65_to_127_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_FRAMES_64_OCTETS, tx_frames_64_octets_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_MULTICAST_FRAMES_ATTEMPTED, tx_multicast_frames_attempted_i);\
    ENET_FEGE_INT(enet_fege_handle, mstatx, TX_BROADCAST_FRAMES_ATTEMPTED, tx_broadcast_frames_attempted_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LOS, los_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, SDET_IN, sdet_in_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, ALIGN_CHANGE, align_change_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LINK_MON, link_mon_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, FEC_STAT, fec_stat_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, CARRIER_STAT, carrier_stat_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, RX_ERR, rx_err_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LCV, lcv_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, AUTO_NEG_LP_ABLTY_UPD, auto_neg_lp_ablty_upd_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LINK_OK, link_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, SYNC_OK, sync_ok_i);\

#define ENET_FEGE_INT_CLEAR_TABLE_DEFINE() \
    ENET_FEGE_INT(enet_fege_handle, enet_fege, OVERFLOW, overflow_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_COMMA_DEL_CHG, rx_comma_del_chg_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_SOF, tx_sof_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_SOF, rx_sof_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_TSTMPR_TOD_SIGN_OVRFL, tx_tstmpr_tod_sign_ovfl_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, RX_TSTMPR_TOD_SIGN_OVRFL, rx_tstmpr_tod_sign_ovfl_i);\
    ENET_FEGE_INT(enet_fege_handle, enet_fege, TX_TSTMP_FIFO, tx_tstmp_fifo_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LOS, los_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, SDET_IN, sdet_in_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, ALIGN_CHANGE, align_change_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LINK_MON, link_mon_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, FEC_STAT, fec_stat_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, CARRIER_STAT, carrier_stat_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, RX_ERR, rx_err_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LCV, lcv_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, AUTO_NEG_LP_ABLTY_UPD, auto_neg_lp_ablty_upd_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, LINK_OK, link_ok_i);\
    ENET_FEGE_INT(enet_fege_handle, l1rpp_fege, SYNC_OK, sync_ok_i);\


#define ENET_FEGE_STATUS_TABLE_DEFINE() \
    ENET_FEGE_STATUS(enet_fege_handle, enet_fege, TX_TSTMPR_TOD_SIGN_OVRFL_V, tx_tstmpr_tod_sign_ovfl_v);\
    ENET_FEGE_STATUS(enet_fege_handle, enet_fege, RX_TSTMPR_TOD_SIGN_OVRFL_V, rx_tstmpr_tod_sign_ovfl_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, LOS_V, los_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, SDET_IN_V, sdet_in_v);    \
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, ALIGN_CHANGE_V, align_change_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, LINK_MON_V, link_mon_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, FEC_STAT_V, fec_stat_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, CARRIER_STAT_V, carrier_stat_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, AUTO_NEG_LP_ABLTY_UPD_V, auto_neg_lp_ablty_upd_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, LINK_OK_V, link_ok_v);\
    ENET_FEGE_STATUS(enet_fege_handle, l1rpp_fege, SYNC_OK_V, sync_ok_v);\

/*
** Structures and Unions
*/
typedef struct enet_fege_handle_struct_t enet_fege_handle_t; /*!< Declare enet fege handle */


/*******************************************************************************
* STRUCTURE: enet_fege_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for ENET FEGE context 
*
* ELEMENTS:
*    state                - ENET_FEGE state
*    rx_enabled           - RX block status
*    tx_enabled           - TX block status
* 
*******************************************************************************/
typedef struct enet_fege_ctxt_struct_t
{
    enet_fege_state_t state;
    BOOL8 rx_enabled;
    BOOL8 tx_enabled;
} enet_fege_ctxt_t;


/*******************************************************************************
* STRUCTURE: enet_fege_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {
    
   
} enet_fege_cfg_t;

/*******************************************************************************
* STRUCTURE: enet_fege_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*  ctxt - Context 
*
*******************************************************************************/
typedef struct {
    enet_fege_ctxt_t  ctxt;        
} enet_fege_var_t;


/*******************************************************************************
* STRUCTURE: enet_fege_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the enet_fege context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*******************************************************************************/
typedef struct enet_fege_handle_struct_t {
    pmc_handle_t base;
    
    enet_fege_cfg_t cfg;
    enet_fege_var_t var;
 
} enet_fege_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC enet_fege_handle_t *enet_fege_ctxt_create(pmc_handle_t       *parent, 
                                                 UINT32              base_address, 
                                                 pmc_sys_handle_t   *sys_handle, 
                                                 const char         *tsb_name);

PUBLIC void enet_fege_ctxt_destroy(enet_fege_handle_t *enet_fege_handle);

PUBLIC void enet_fege_handle_init(enet_fege_handle_t *enet_fege_handle);

PUBLIC UINT32 enet_fege_base_address_get(enet_fege_handle_t *enet_fege_handle);
        
PUBLIC PMC_ERROR enet_fege_prov(enet_fege_handle_t   *hndl,
                                BOOL8                 ohp,
                                BOOL8                 enable_tx,
                                BOOL8                 enable_rx);

PUBLIC PMC_ERROR enet_fege_deprov(enet_fege_handle_t *hndl);
                                      

PUBLIC PMC_ERROR enet_fege_int_enable(enet_fege_handle_t    *enet_fege_handle,
                                      enet_fege_int_t       *int_table_ptr,
                                      enet_fege_int_t       *int_en_table_ptr,
                                      BOOL                   enable);

PUBLIC PMC_ERROR enet_fege_int_retrieve(enet_fege_handle_t    *enet_fege_handle,
                                        enet_fege_int_t       *filt_table_ptr,
                                        enet_fege_int_t       *int_table_ptr);

PUBLIC PMC_ERROR enet_fege_int_clear(enet_fege_handle_t    *enet_fege_handle,
                                     enet_fege_int_t       *int_table_ptr,
                                     enet_fege_int_t       *int_en_table_ptr);

PUBLIC PMC_ERROR enet_fege_int_enabled_check(enet_fege_handle_t    *enet_fege_handle,
                                             enet_fege_int_t       *int_en_table_ptr,
                                             BOOL                  *int_found_ptr);

PUBLIC void enet_fege_mstatx_trigger(enet_fege_handle_t    *enet_fege_handle);

PUBLIC UINT32 enet_fege_mstatx_retrieve(enet_fege_handle_t    *enet_fege_handle,
                                        UINT32 count_addr, BOOL8 byte);
 
#ifdef __cplusplus
}
#endif

#endif /* _ENET_FEGE_H */

/*
** end of file
*/
