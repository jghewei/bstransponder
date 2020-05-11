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

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include <stdio.h>

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: digi_ghao_datapath_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for the datapath.
*
* ELEMENTS:
*   DIGI_LINE_LINE  - DIGI as an intermediate node
*   DIGI_LINE_SIFD  - DIGI as an intermediate node interfacing with SIFD
*   DIGI_LINE_MAP   - DIGI as a source/end node (interfacing with LINE)
*   DIGI_MAP_SIFD   - DIGI as a source/end node (interfacing with SIFD)
*
*******************************************************************************/
typedef enum
{
        DIGI_LINE_LINE   = 0,
        DIGI_LINE_SIFD   = 1,
        DIGI_LINE_MAP    = 2,
        DIGI_MAP_SIFD    = 3,
        DIGI_PATH_UNDEFINED
} digi_ghao_datapath_t;




/*******************************************************************************
* ENUM: digi_ghao_firmware_seg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for decribing a firmware segment (used only for the 
*   firmware adjustments)
*
* ELEMENTS:
*   See element description
*******************************************************************************/
typedef enum
{
    DIGI_GHAO_CORE_SEG = 0, /* datapath segment in COREOTN */
    DIGI_GHAO_MAP_SEG = 1,  /* datapath segment from ODUKSW to MAPOTN (includes FO2 MPMA/MPMO) */
} digi_ghao_firmware_seg_t;




/*******************************************************************************
* ENUM: digi_ghao_firmwate_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for defining the stage of the G.HAO segment
*
* ELEMENTS:
*        DIGI_GHAO_FW_START
*        DIGI_GHAO_FW_RAMP
*        DIGI_GHAO_FW_END
*
*******************************************************************************/
typedef enum
{
        DIGI_GHAO_FW_START             = 0,
        DIGI_GHAO_FW_RAMP              = 1,
        DIGI_GHAO_FW_END               = 2,
        DIGI_GHAO_FW_DONE              = 3
} digi_ghao_firmwate_state_t;

/*
** Constants
*/


#define END_FOR_BWR_IND_250US 0

/* the number of 50us ticks to wait before timing out for an event*/
#define WAIT_TSCC_TIMOUT 1000
#define WAIT_RP_TIMOUT 1000


#define AUGMENTED_CN_DELAY_US 1000



/*
testing has found that 1 userbit must always be
to unsure no datahit during the ramp
#define MPMO_USERBIT_INC_PRE_W_PASSTHRU  0*/
#define FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU  2
#define FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU_FROM1  2
#define FMF2_MUX_XOFF_INC_PRE_W_PASSTHRU  0
#define MPMO_XOFF_INC_PRE_W_PASSTHRU  0
#define FMF2_MUX_CAL_EN_INC_PRE_W_PASSTHRU  0

#define MPMO_USERBIT_INC_PRE_WO_PASSTHRU 1
#define FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU  0
#define FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU_FROM1  2
#define MPMO_XOFF_INC_PRE_WO_PASSTHRU  0

#define MPMO_USERBIT_DEC_POST_W_PASSTHRU  1
#define FMF1_MUX_XOFF_DEC_POST_W_PASSTHRU  1
#define FMF2_MUX_XOFF_DEC_POST_W_PASSTHRU  1
#define FMF2_MUX_CAL_EN_DEC_POST_W_PASSTHRU  1
#define MPMO_XOFF_DEC_POST_W_PASSTHRU  2

#define MPMO_USERBIT_DEC_PRE_WO_PASSTHRU 0
#define FMF1_MUX_XOFF_DEC_PRE_W0_PASSTHRU 1
#define MPMO_XOFF_DEC_PRE_W0_PASSTHRU   0





/*
** Macro Definitions
*/

/*
** Structures and Unions
*/




/*******************************************************************************
* STRUCTURE: digi_ghao_seg_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for a G.HAO segment
*
* ELEMENTS:
*   oif_pkt_size          - OIF packet size
*   header_size           - header size
*   segment_in_use        - TRUE : G.HAO segment is in use
*                           FALSE : G.HAO segment is not in use
*   datapath_type         - see digi_ghao_datapath_t
*   ghao_increase         - TRUE : performing G.HAO increase
*                           FALSE : performing G.HAO decrease
*   mux_passthru          - if set, MUX/DEMUX in Stage 2-3A are in passthru mode
*   coreotn_ptr           - if CORE segment is in the path, the COREOTN
*                           channel context pointer
*   coreotn_lo_chnl_id    - if CORE segment is in the path, LO channel ID in COREOTN
*   coreotn_ho_chnl_id    - HO channel ID in COREOTN
*   coreotn_odu_level     - if CORE segment is in the path, The ODU level of the LO
*   ghao_mux_stage        - See coreotn_mux_stage_t 
*   mapotn_ptr            - if MAP segment is in the path, the MAP
*                           channel context pointer
*   mapotn_chnl_id        - If MAP segment is in the path, the MAPOTN channel id
*   sifd_ptr              - If SIFD segment is in the path, the SIFD channel
*                           context pointer
*   sifd_chnl_id          - if SFD segment is in the path, the SIFD channel is
*   old_trib_slot_mask_rx - CORE segment RX old tribslot mask
*   old_trib_slot_mask_tx - CORE segment TX old tribslot mask
*   new_trib_slot_mask_rx - CORE segment RX new tribslot mask
*   new_trib_slot_mask_tx - CORE segment TX new tribslot mask
*   coreotn_rx_old_cal_mask - COREOTN RX old calendar mask
*   coreotn_rx_new_cal_mask - COREOTN RX new calendar mask
*   coreotn_rx_old_trib_slots - COREOTN RX old tribslot array
*   coreotn_rx_new_trib_slots - COREOTN RX new tribslot array
*   coreotn_rx_trib_slots_add_rem - COREOTN RX boolean array of tribslots to add/remove
*   coreotn_rx_tpid       - COREOTN RX trib port ID
*   coreotn_tx_old_cal_mask - COREOTN TX old calendar mask
*   coreotn_tx_new_cal_mask - COREOTN TX new calendar mask
*   coreotn_tx_cal_slots_add_rem - COREOTN TX boolean array of calendar slot to add/rem
*   coreotn_tx_old_trib_slots - COREOTN TX old tribslot array
*   coreotn_tx_new_trib_slots - COREOTN TX new tribslot array
*   coreotn_tx_tpid       - COREOTN TX trib port ID
*   mapotn_oduksc_old_cal_mask - MAPOTN ODUKSC old calendar mask
*   mapotn_oduksc_new_cal_mask - MAPOTN ODUKSC new calendar mask
*   mapotn_mpma_old_cal_mask - MAPOTN MPMA old calendar mask
*   mapotn_mpma_new_cal_mask - MAPOTN MPMA new calendar mask
*   coreotn_mpmo3b_userbits_mask - COREOTN stage 3B MPMO userbit mask
*   coreotn_mpmo4_userbits_mask - COREOTN stage 4 MPMO userbit mask
*   sifd_new_cal_mask           - New calendar mask for SIFD ODUKSW port
*   sifd_old_cal_mask           - Old calendar mask for SIFD ODUKSW port
*   coreotn_rx_cal_to_add_remove - COREOTN rx entries to add remove
*                                  (order to add/remove is specified)
*   coreotn_tx_cal_to_add_remove - COREOTN tx entries to add remove
*                                  (order to add/remove is specified)
*   mapotn_mpma_cal_to_add_remove - MAPOTN MPMA calendar entries to add
*                                   remove (order to add/remove is specified).
*                                   Stage 4 MPMO inherits this calendar.
*   sifd_cal_to_add_remove - SIFD ODUKSW port entries to add remove
*                            (order to add/remove is specified)
*   cal_to_add_remove_num - The number of calendar entries to add/remove
*   ts_start              - Starting number of tribslots
*   ts_target             - End number of tribslots
*   coreotn_mux_xoff_thresh - XOFF threshold at muxing node MUX
*   coreotn_mux_passthru_xoff_thresh - xoff threshold at FMF2 mux if
*                                      it's in passthru
*   fo1_mpmo_thresh_pre_post_fw - Number or pre/post asjustments for FO1 MPMO
*                                 XOFF threshold
*   fo2_mpmo_thresh_pre_post_fw - Number or pre/post asjustments for FO1 MPMO
*                                 XOFF threshold
*   sifd_mpmo_thresh_pre_post_fw - Number of pre/post asjustments for
*                                  SIFD ODUKSW port
*   mux_thresh_pre_post_fw - Number of pre/post adjustments for muxing node
*                            MUX
*  mux_passthru_thresh_pre_post_fw - Number of pre/post adjustments
*                                    for FMF2 mux if it's in passthru
*  fo1_mpmo_userbit_update_pre_post_fw - Number of pre/post userbit adjustments
*                                        for FO1 MPMO userbits
*  fo2_mpmo_userbit_update_pre_post_fw - Number of pre/post userbit adjustments
*                                        for FO2 MPMO userbits
*  sifd_oduksw_cal_update_pre_post_fw - Number of pre/post calendar adjustments for
*                                       SIFD ODUKSW port
*  fmf2_mux_cal_en_pre_post_fw - Number of pre/post adjustments for FMF2 MUX
*                                calendar enable (LO_FIRST_CYCLE)
*  ghao_step_tx                - State of G.HAO for TX segment (see digi_ghao_step_t)
*  ghao_step_rx                - State of G.HAO for TX segment (see digi_ghao_step_t)
*******************************************************************************/
typedef struct digi_ghao_seg_ctrl_t {
    
    /* desired packet size defined in OIF recommendations. */
    UINT32 oif_pkt_size;
    UINT32 header_size;

    /* set by digi_ghao_segment_id_get*/
    BOOL8  segment_in_use;

    digi_ghao_datapath_t datapath_type;
    
    /*if set to true the g.hao path is set to increase */
    BOOL8 ghao_increase;
    /* if set, MUX/DEMUX in Stage 2-3A are in passthru mode */
    BOOL8 mux_passthru;

    /* G.HAO will always involved a minimum of two of these channel context pointers*/
    digi_otn_odu_chnl_t *coreotn_ptr;

    /*channel ID of the OFUflex client.  Also the chnl id at the ODUKSW */
    UINT32 coreotn_lo_chnl_id;
    /*channel ID of the HO that the ODUflex channel is being muxed into */
    UINT32 coreotn_ho_chnl_id;

    /* will be 1 (first stage), or 2 (second stage) */
    coreotn_mux_stage_t ghao_mux_stage;

    digi_mapper_chnl_t *mapotn_ptr;
    UINT32 mapotn_chnl_id;

    digi_ilkn_chnl_t *sifd_ptr;
    UINT32 sifd_chnl_id;

    /* Tribslot masks */
    UINT32 old_trib_slot_mask_rx[3];
    UINT32 old_trib_slot_mask_tx[3];
    UINT32 new_trib_slot_mask_rx[3];
    UINT32 new_trib_slot_mask_tx[3];

    /* calendar mask before and after the ramp.
       for coreotn this is also the tribslot mask before and after the ramp*/
    
    /*COREOTN needs support for asymmetric tribslot/calendar assignments */
    UINT32 coreotn_rx_old_cal_mask[3];
    UINT32 coreotn_rx_new_cal_mask[3];

    BOOL8  coreotn_rx_old_trib_slots[96];
    BOOL8  coreotn_rx_new_trib_slots[96];
    BOOL8  coreotn_rx_trib_slots_add_rem[96];
    UINT8  coreotn_rx_tpid;

    UINT32 coreotn_tx_old_cal_mask[3];
    UINT32 coreotn_tx_new_cal_mask[3];
    BOOL8  coreotn_tx_cal_slots_add_rem[96];

    BOOL8  coreotn_tx_old_trib_slots[96];
    BOOL8  coreotn_tx_new_trib_slots[96];
    UINT8  coreotn_tx_tpid;

    UINT32 mapotn_oduksc_old_cal_mask[3];
    UINT32 mapotn_oduksc_new_cal_mask[3];
    UINT32 mapotn_mpma_old_cal_mask[3];
    UINT32 mapotn_mpma_new_cal_mask[3];

    UINT32 coreotn_mpmo3b_userbits_mask[3];
    UINT32 coreotn_mpmo4_userbits_mask[3];

    UINT32 sifd_new_cal_mask[3];
    UINT32 sifd_old_cal_mask[3];

    /* To give control of the order the that calendar/tribslots are removed*/
    UINT32 coreotn_rx_cal_to_add_remove[79];
    UINT32 coreotn_tx_cal_to_add_remove[79];
    UINT32 mapotn_mpma_cal_to_add_remove[79];
    UINT32 sifd_cal_to_add_remove[79];
    UINT32 cal_to_add_remove_num;

    /*the number of tribslots at the beggining and end of ramp*/
    UINT32 ts_start;
    UINT32 ts_target;

    /* XOFF thresholds */
    UINT32 coreotn_mux_xoff_thresh;
    UINT32 coreotn_mux_passthru_xoff_thresh;

    /*context for userbit/xoff adjustments performed before the firmware */
    UINT8 fo1_mpmo_thresh_pre_post_fw;
    UINT8 fo2_mpmo_thresh_pre_post_fw;
    UINT8 sifd_mpmo_thresh_pre_post_fw;
    UINT8 mux_thresh_pre_post_fw;
    UINT8 mux_passthru_thresh_pre_post_fw;
    UINT8 fo1_mpmo_userbit_update_pre_post_fw;
    UINT8 fo2_mpmo_userbit_update_pre_post_fw;
    UINT8 sifd_oduksw_cal_update_pre_post_fw;
    UINT8 fmf2_mux_cal_en_pre_post_fw;

    /* Current G.HAO step */
    digi_ghao_step_t ghao_step_tx;
    digi_ghao_step_t ghao_step_rx;

} digi_ghao_seg_ctrl_t;


/*******************************************************************************
* STRUCTURE: digi_ghao_firmware_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for firmware routine.
*
* ELEMENTS:
*   seg_type          - see digi_ghao_firmware_seg_t
*   state             - See digi_ghao_firmwate_state_t
*   increase          - TRUE : G.HAO increase
*                       FALSE : G.HAO decrease
*   fmf2_passthru     - TRUE : If CORE seg_type, FMF2 is in passthru
*                       FALSE : FMF2 not in passthru
*   chnl_id           - Channel id
*   ghao_mux_stage    - See coreotn_mux_stage_t
*   ts_start          - Start number of tribslots
*   ts_target         - Target number of tribslots
*   cal_slot_to_add_remove - calendar entries to add/rem
*   old_cal_mask      - old calendar mask
*   new_cal_mask      - new calendar mak
*   current_userbit   - the current userbit mask
*   current_cal_slots - current number of calendar slots
*   current_ts_step   - current trbslot adjustment step
*   current_word_step - current word adjustment step
*   current_sub_word_step - current sub word adjustment
*                           (only used for CORE segement)
*   xoff_thresh       - current xoff threshold of muxing
*                       node (FMF1 or FMF2)
*   xoff_thresh_passthru_mux - current xoff threshold of
*                              FMF2 mux (if fmf2_passthru == TRUE)
*   thresh_update_itr - current xoff threshold iterator for MPMO
*   userbits_update_itr - current userbit update iterator for MPMO
*   passthru_mux_cal_update_itr - current calendar enable iterator
*                                 (if fmf2_passthru == TRUE)
*   mpmo_thresh_update_itr_last - last MPMO xoff threshold iteration
*                                 to perform
*   mux_thresh_update_itr_last - last xoff thresold iteration for
*                                muxing node (FMF1 or FMF2)
*   userbits_update_itr_last - last userbit adjustment iteration
*                              for MPMO
*   passthru_mux_thresh_update_itr_last - last xoff threshold iteration for
*                                         FMF2 MUX if fmf2_passthru == TRUE
*   passthru_mux_cal_update_itr_last - last calendar enable iteration for
*                                      FMF2 MUX if fmf2_passthru == TRUE
*   total_fw_ticks - the number of ticks of length FIRMWARE_TICK_US
*                    that have occured since the start of the ramp
*   ts_update_step_us - Delay between each TS increment/decrement in 
*                       firmware in units of us
*   word_update_step_core_map_us - Delay between each COREOTN/MAPOTN word 
*                                  increment/decrement in firmware in units of us
*   word_update_step_sifd_us - Delay between each SIFD word increment/decrement 
*                              in firmware in units of us
*   new_cn_base_int - CN_BASE_INT to get programmed by firmware
*   new_cn_base_d   - CN_BASE_D to get programmed by firmware
*   new_cn_base_n   - CN_BASE_N to get programmed by firmware
*   program_cn_n_first - TRUE : program CN_BASE_N first, then CN_BASE_D
*                        FALSE : program CN_BASE_D first, then CN_BASE_N
*   cn_changed         - TRUE : Cn has been reprogrammed by firmware
*                        FALSE : Cn has not been reprogrammed by firmware
*******************************************************************************/
typedef struct digi_ghao_firmware_ctrl_t {

    /* CORE, MAP, or SIFD  */
    digi_ghao_firmware_seg_t seg_type;


    digi_ghao_firmwate_state_t state;

    /*TRUE, performing and increase. FALSE performing a decrease*/
    BOOL8 increase;

    /*If TRUE, FMF2 MUX is in passthru mode, if FALSE, FMF2 mux is being bypassed*/
    BOOL8 fmf2_passthru;

    UINT32 chnl_id;


    coreotn_mux_stage_t ghao_mux_stage;

    UINT32 ts_start;
    UINT32 ts_target;


    /*copied from G.HAO context */
    UINT32 cal_slot_to_add_remove[79];

    /* required for userbit adjustments */
    UINT32 old_cal_mask[3];
    UINT32 new_cal_mask[3];


    /*used to keep track of the current calendar mask*/
    UINT32 current_userbit[3];
    /* used to keep track of the current number of calendar slots */
    UINT32 current_cal_slots;

    /*used to keep track of the current ts step
      and word step */
    UINT32 current_ts_step;
    UINT32 current_word_step;
    UINT32 current_sub_word_step;
    /* XOFF threshold update during firmware */
    UINT32 xoff_thresh;
    UINT32 xoff_thresh_passthru_mux;
    /* Iterators for XOFF thresholds and userbits updates */
    UINT32 thresh_update_itr;
    UINT32 userbits_update_itr;
    UINT32 passthru_mux_cal_update_itr;
    /* Iterators for XOFF thresholds and userbits updates */
    UINT32 mpmo_thresh_update_itr_last;
    UINT32 mux_thresh_update_itr_last;
    UINT32 userbits_update_itr_last;
    /* Used only for FMF2 mux in passthru */
    UINT32 passthru_mux_thresh_update_itr_last;
    /* Used only for FMF2 mux in passthru */
    UINT32 passthru_mux_cal_update_itr_last;

    /* this is the number of ticks of length FIRMWARE_TICK_US
    that have occured since the start of the ramp*/
    UINT32 total_fw_ticks;
    
    /* Delay between each TS increment/decrement in firmware in units of us*/
    DOUBLE ts_update_step_us;
    /* Delay between each COREOTN/MAPOTN word increment/decrement in firmware in units of us*/
    DOUBLE word_update_step_core_map_us;
    /* Delay between each SIFD word increment/decrement in firmware in units of us*/
    DOUBLE word_update_step_sifd_us;

    /* It is required to change the MPMO_DSP parameters
    after the firmware has detected the start of ramp*/
    UINT32 new_cn_base_int;
    UINT32 new_cn_base_d;
    UINT32 new_cn_base_n;
    BOOL   program_cn_n_first;

    BOOL  cn_changed;
} digi_ghao_firmware_ctrl_t;

/*******************************************************************************
* STRUCTURE: digi_ghao_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for firmware routine.
*
* ELEMENTS:
*   ghao_segments     - Array of G.HAO segments (see digi_ghao_seg_ctrl_t)
*   ghao_fw_segments  - Array of G.HAO firmware segmenets.  This context will
*                       eventually live in firmware (see digi_ghao_firmware_seg_t)
*   num_fw_segments   - The number of G.HAO segments currently provisioned
*   fw_tick_us        - Configurable firmware tick until firmware is complete
*******************************************************************************/
typedef struct digi_ghao_cfg_t {
    digi_ghao_seg_ctrl_t *ghao_segments;
    digi_ghao_firmware_ctrl_t *ghao_fw_segments;
    UINT32 num_fw_segments;
    UINT32 fw_tick_us;
} digi_ghao_cfg_t;




/*
** Global variables
*/

/*
** Function Prototypes
*/


PUBLIC PMC_ERROR digi_ghao_ctxt_init(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_ghao_ctxt_destroy(digi_handle_t *digi_handle);

/* Helper functions */

PUBLIC PMC_ERROR digi_ghao_segment_id_get(digi_handle_t *digi_handle,
                                           UINT32 *ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_validate_segment_id(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id);

PUBLIC UINT32 digi_ghao_coreotn_odu_level_get(digi_handle_t                    *digi_handle,
                                              util_global_oduk_port_data_t     *chnl_handle);


PUBLIC PMC_ERROR digi_ghao_pop_line_map_ctxt(digi_handle_t              *digi_handle,
                                             util_global_oduk_port_data_t *coreotn_ptr,
                                             util_global_oduk_port_data_t *mapotn_ptr,
                                             UINT32 ts_target,
                                             UINT32 old_tribslot_mask_rx[3],
                                             UINT32 old_tribslot_mask_tx[3],
                                             UINT32 new_tribslot_mask_rx[3],
                                             UINT32 new_tribslot_mask_tx[3],
                                             UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_pop_line_line_ctxt(digi_handle_t              *digi_handle,
                                              util_global_oduk_port_data_t *coreotn_ptr,
                                              UINT32 ts_target, 
                                              UINT32 old_tribslot_mask_rx[3],
                                              UINT32 old_tribslot_mask_tx[3],
                                              UINT32 new_trib_slot_mask_rx[3],
                                              UINT32 new_trib_slot_mask_tx[3],
                                              UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_pop_line_sifd_ctxt(digi_handle_t              *digi_handle,
                                              util_global_oduk_port_data_t *coreotn_ptr,
                                              util_global_oduk_port_data_t *sifd_ptr,
                                              UINT32 ts_target,
                                              UINT32 old_tribslot_mask_rx[3],
                                              UINT32 old_tribslot_mask_tx[3],
                                              UINT32 new_trib_slot_mask_rx[3],
                                              UINT32 new_trib_slot_mask_tx[3],
                                              UINT32 sifd_old_cal_mask[3],
                                              UINT32 sifd_new_cal_mask[3],
                                              UINT32 ghao_segment_id);                                            

PUBLIC PMC_ERROR digi_ghao_pop_map_sifd_ctxt(digi_handle_t              *digi_handle,
                                             util_global_oduk_port_data_t *mapotn_ptr,
                                             util_global_oduk_port_data_t *sifd_ptr,
                                             UINT32 sifd_old_cal_mask[3],
                                             UINT32 sifd_new_cal_mask[3],
                                             UINT32 ts_target, 
                                             UINT32 ghao_segment_id);    
PUBLIC PMC_ERROR digi_ghao_pop_firmware_ctxt(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_ghao_save_firmware_ctxt(digi_handle_t *digi_handle);


PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_cfg_plus_userbits(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id);

/* Firmware routines */
PUBLIC PMC_ERROR digi_ghao_do_fw_tick(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_map_inc(digi_handle_t *digi_handle, UINT32 seg_idx);
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_map_dec(digi_handle_t *digi_handle, UINT32 seg_idx);
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_core_inc(digi_handle_t *digi_handle, UINT32 seg_idx);
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_core_dec(digi_handle_t *digi_handle, UINT32 seg_idx);

/* LINE <-> MAP G.HAO APIs */ 

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_tx_dec_4_rp(digi_handle_t *digi_handle,
                                                               UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_lcr_knife_edge_rx_dec_4_rp(digi_handle_t *digi_handle,
                                                               UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_setup_tx_2_ncs(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_setup_rx_2_ncs(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                           UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                           UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_tx_2_ncs(digi_handle_t *digi_handle,
                                                          UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_rx_2_ncs(digi_handle_t *digi_handle,
                                                          UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                             UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_map_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                             UINT32 ghao_segment_id);

/* LINE <-> LINE G.HAO APIs */ 

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id);


/* MAP <-> ILKN G.HAO APIs */ 

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                    UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_lcr_knife_edge_ilkn(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_setup_tx_2_ncs(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_setup_rx_2_ncs(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                           UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                           UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_tx_2_ncs(digi_handle_t *digi_handle,
                                                          UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_rx_2_ncs(digi_handle_t *digi_handle,
                                                          UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                             UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_map_ilkn_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                             UINT32 ghao_segment_id);

/* LINE <-> SIFD G.HAO APIs */ 


PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_ilkn(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id);

PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id);






/* Helpers APIS */


PUBLIC void digi_ghao_gen_slots_add_rem_from_masks(UINT32 cal_mask_1[3],
                                                   UINT32 cal_mask_2[3],
                                                   BOOL8 cal_slots[96]);

PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_add_rem(digi_handle_t *digi_handle,
                                                    digi_ghao_cpb_datapath_t cpb_datapath,
                                                    UINT32 chnl,
                                                    BOOL increase,
                                                    UINT32 *num_slots);

PUBLIC PMC_ERROR digi_ghao_cpb_db_chnl_rec_update(digi_handle_t *digi_handle,
                                                  digi_ghao_cpb_datapath_t cpb_datapath,
                                                  UINT32 chnl, 
                                                  UINT32 num_cal_entries_added);

PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_cfg(digi_handle_t *digi_handle,
                                                digi_ghao_cpb_datapath_t cpb_datapath,
                                                UINT32 chnl);






#ifdef __cplusplus
}
#endif


/* 
** end of file 
*/

