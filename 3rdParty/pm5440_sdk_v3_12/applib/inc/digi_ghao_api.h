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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring OTN features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_GHAO_API_H
#define _DIGI_GHAO_API_H

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* ENUM: digi_ghao_step_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for defining the stage of the G.HAO segment
*
* ELEMENTS:
*        DIGI_GHAO_NONE
*        DIGI_GHAO_LCR_SIZE_1_RP
*        DIGI_GHAO_LCR_SIZE_2_ALL
*        DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK
*        DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK
*        DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK
*        DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK
*        DIGI_GHAO_LCR_KNIFE_EDGE_4_RP
*        DIGI_GHAO_BWR_SETUP_1_TSCC
*        DIGI_GHAO_BWR_SETUP_2_NCS
*        DIGI_GHAO_BWR_START
*        DIGI_GHAO_BWR_END_1_TSCC
*        DIGI_GHAO_BWR_END_2_NCS
*        DIGI_GHAO_BWR_END_3_RP
*        DIGI_GHAO_LCR_KNIFE_EDGE_SIFD
*
*******************************************************************************/
typedef enum
{
        DIGI_GHAO_NONE                        = 0,
        DIGI_GHAO_LCR_SIZE_1_RP               = 1,
        DIGI_GHAO_LCR_SIZE_2_ALL              = 2,
        DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK    = 3,
        DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK    = 4,
        DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK   = 5,
        DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK  = 6,
        DIGI_GHAO_LCR_KNIFE_EDGE_4_RP         = 7,
        DIGI_GHAO_BWR_SETUP_1_TSCC            = 8,
        DIGI_GHAO_BWR_SETUP_2_NCS             = 9,
        DIGI_GHAO_BWR_START                   = 10,
        DIGI_GHAO_BWR_END_1_TSCC              = 11,
        DIGI_GHAO_BWR_END_2_NCS               = 12,
        DIGI_GHAO_BWR_END_3_RP                = 13,
        DIGI_GHAO_LCR_KNIFE_EDGE_SIFD         = 14
} digi_ghao_step_t;

/*******************************************************************************
* ENUM: digi_ghao_step_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for defining the direction of the G.HAO segment
*
* ELEMENTS:
*        DIGI_GHAO_TX           
*        DIGI_GHAO_RX       
*
*******************************************************************************/
typedef enum
{
        DIGI_GHAO_TX   = 0,
        DIGI_GHAO_RX   = 1
} digi_ghao_dir_t;


/*******************************************************************************
* ENUM: digi_ghao_cpb_datapath_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for defining the CPB datapath of the G.HAO segment
*
* ELEMENTS:
*        DIGI_GHAO_DCPB_SIFD_MAPOTN
*        DIGI_GHAO_DCPB_MAPOTN_SIFD
*        DIGI_GHAO_ODUKSW_MAPOTN_COREOTN
*        DIGI_GHAO_ODUKSW_COREOTN_MAPOTN
*        DIGI_GHAO_ODUKSW_MAPOTN_SIFD
*        DIGI_GHAO_ODUKSW_SIFD_MAPOTN
*        DIGI_GHAO_ODUKSW_COREOTN_SIFD
*        DIGI_GHAO_ODUKSW_SIFD_COREOTN
*
*******************************************************************************/
typedef enum
{
        DIGI_GHAO_DCPB_SIFD_MAPOTN        = 0,
        DIGI_GHAO_ODUKSW_MAPOTN_COREOTN   = 1,
        DIGI_GHAO_ODUKSW_COREOTN_MAPOTN   = 2,
        DIGI_GHAO_ODUKSW_MAPOTN_SIFD1     = 3,
        DIGI_GHAO_ODUKSW_MAPOTN_SIFD2     = 4,
        DIGI_GHAO_ODUKSW_SIFD1_MAPOTN     = 5,
        DIGI_GHAO_ODUKSW_SIFD2_MAPOTN     = 6,
        DIGI_GHAO_ODUKSW_COREOTN_COREOTN  = 7,
        DIGI_GHAO_ODUKSW_COREOTN_SIFD1    = 8,
        DIGI_GHAO_ODUKSW_COREOTN_SIFD2    = 9,
        DIGI_GHAO_ODUKSW_SIFD1_COREOTN    = 10,
        DIGI_GHAO_ODUKSW_SIFD2_COREOTN    = 11,

} digi_ghao_cpb_datapath_t;





/*
** Include Files
*/

PUBLIC PMC_ERROR digi_ghao_segment_prov(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *chnl1_ptr,
                                        util_global_switch_data_t *chnl2_ptr,
                                        UINT32 target_ts,
                                        UINT32 old_trib_slot_mask_rx[3],
                                        UINT32 old_trib_slot_mask_tx[3],
                                        UINT32 new_trib_slot_mask_rx[3],
                                        UINT32 new_trib_slot_mask_tx[3],
                                        UINT32 sifd_old_cal_mask[3],
                                        UINT32 sifd_new_cal_mask[3],
                                        UINT32 oif_pkt_size,
                                        UINT32 header_size,
                                        UINT32 *ghao_segment_id,
                                        UINT32 fw_tick_us);

PUBLIC PMC_ERROR digi_ghao_segment_deprov(digi_handle_t *digi_handle,
                                          UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_step_lcr_size(digi_handle_t *digi_handle,
                                         UINT32 ghao_segment_id,
                                         digi_ghao_dir_t ghao_dir,
                                         digi_ghao_step_t ghao_step);

PUBLIC PMC_ERROR digi_ghao_step_lcr_knife_edge(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id,
                                               digi_ghao_dir_t ghao_dir,
                                               digi_ghao_step_t ghao_step);

PUBLIC PMC_ERROR digi_ghao_step_bwr_setup(digi_handle_t *digi_handle,
                                          UINT32 ghao_segment_id,
                                          digi_ghao_dir_t ghao_dir,
                                          digi_ghao_step_t ghao_step);

PUBLIC PMC_ERROR digi_ghao_step_bwr_ramp_start(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id,
                                               digi_ghao_dir_t ghao_dir);

PUBLIC PMC_ERROR digi_ghao_step_bwr_ramp_end(digi_handle_t *digi_handle,
                                             UINT32 ghao_segment_id,
                                             digi_ghao_dir_t ghao_dir,
                                             digi_ghao_step_t ghao_step);

PUBLIC PMC_ERROR digi_ghao_ilkn_oduk_fc_calendar_reprov(digi_handle_t *digi_handle,
                                                        digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                        BOOL update_rx,
                                                        BOOL update_tx,
                                                        digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                                        digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ghao_bwr_firmware_execute(digi_handle_t *digi_handle1,
                                                digi_handle_t *digi_handle2,
                                                digi_handle_t *digi_handle3);




/*
* Add any new log codes in the following paired definitions
* below. These will be defined as a (enum, string) pair if printing out
* errors or if embedded (limited memory) then will just define the enum.
* This method means that one table of log/error values and string pairs
* can be used to generate just a list of enums or strings and enums
*/
#define DIGI_GHAO_LOG_ERR_TABLE                                              \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GHAO_SEGS_FULL, "DIGI: Can't provision another G.HAO segment because the maximum number is already provisioned") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GHAO_TIMER_ERR, "Firmware loop was not able to complete within FIRMWARE_TICK_US") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GHAO_TSCC_RCV_ERR, "DIGI : TSCC was not recieved by the ODTU_DMX within the timeout window") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GHAO_RP_RCV_ERR, "DIGI : RP was not recieved by the ODTU_DMX within the timeout window") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GHAO_SEG_ID_INVALID, "DIGI : Error, the segment ID provided is not a valid, provisied G.HAO path") \



/*
* The following creates the ERR/LOG indexes from above by pulling out the enum
* portion of the LOG_ERR_TABLE above
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum DIGI_GHAO_LOG_ERR_INDEX
{
    DIGI_GHAO_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_GHAO << 16),
    DIGI_GHAO_LOG_ERR_TABLE
    DIGI_GHAO_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE


#ifdef PMC_LOG_STRINGS
extern const char DIGI_GHAO_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char DIGI_GHAO_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */





#ifdef __cplusplus
}
#endif

#endif /* _DIGI_GHAO_API_H */



/*
** end of file
*/
