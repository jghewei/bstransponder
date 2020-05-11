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
*   DESCRIPTION : This file contains C functions for operating OTN features.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_loc.h"

#include <stdio.h>
#include "math.h"

#ifdef PMC_SW_SIMULATION
static UINT32 pmc_log_seq_counter = 0;
#endif

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
* The following creates the LOG/ERR strings from above by pulling out the text
* portion of the LOG_ERR_TABLE above
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char DIGI_GHAO_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    DIGI_GHAO_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char DIGI_GHAO_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_LOG_STRINGS */



/*
** Forward References
*/                                            

/*
** Internal Functions
*/



/*
**  Exported Interface Functions - Start
*/



#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* digi_ghao_ctxt_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the G.HAO context
*   
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_ctxt_init(digi_handle_t *digi_handle)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    UINT32 i, j;

    digi_handle->var.ghao_cfg.ghao_segments = (digi_ghao_seg_ctrl_t *)PMC_CTXT_CALLOC(sizeof(digi_ghao_seg_ctrl_t) * MAX_NUM_GHAO_SEGMENTS, digi_handle);
    digi_handle->var.ghao_cfg.ghao_fw_segments = (digi_ghao_firmware_ctrl_t *)PMC_CTXT_CALLOC(sizeof(digi_ghao_firmware_ctrl_t) * MAX_NUM_FIRMWARE_SEGMENTS, digi_handle);

    PMC_CTXT_REGISTER_PTR(&digi_handle->var.ghao_cfg.ghao_segments, digi_handle);
    PMC_CTXT_REGISTER_PTR(&digi_handle->var.ghao_cfg.ghao_fw_segments, digi_handle);


    for(i=0;i<MAX_NUM_GHAO_SEGMENTS;i++)
    {
        digi_handle->var.ghao_cfg.ghao_segments[i].oif_pkt_size = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].header_size = 0;

        digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use = FALSE;
        digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type = DIGI_PATH_UNDEFINED;
        digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase = FALSE;

        digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_ptr = NULL;
        digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_ptr = NULL;
        digi_handle->var.ghao_cfg.ghao_segments[i].sifd_ptr = NULL;

        digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_ho_chnl_id = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_chnl_id = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].sifd_chnl_id = 0;

        digi_handle->var.ghao_cfg.ghao_segments[i].ts_start = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].ts_target = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh = 0;

        digi_handle->var.ghao_cfg.ghao_segments[i].ghao_mux_stage = COREOTN_MUX_STAGE_NONE;

        for(j=0;j<3;j++)
        {
             digi_handle->var.ghao_cfg.ghao_segments[i].old_trib_slot_mask_rx[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].old_trib_slot_mask_tx[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].new_trib_slot_mask_rx[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].new_trib_slot_mask_tx[j] = 0;

             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_new_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_old_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_new_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_old_cal_mask[j] = 0;

             digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_oduksc_old_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_oduksc_new_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_old_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_new_cal_mask[j] = 0;

             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo4_userbits_mask[j] = 0;

             digi_handle->var.ghao_cfg.ghao_segments[i].sifd_new_cal_mask[j] = 0;
             digi_handle->var.ghao_cfg.ghao_segments[i].sifd_old_cal_mask[j] = 0;
        }

        for(j=0;j<79;j++)
        {
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_cal_to_add_remove[j] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_cal_to_add_remove[j] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
              digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_cal_to_add_remove[j] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
              digi_handle->var.ghao_cfg.ghao_segments[i].sifd_cal_to_add_remove[j] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
        }

        for(j=0;j<96;j++)
        {
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_old_trib_slots[j] = 0;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_new_trib_slots[j] = 0;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_rx_trib_slots_add_rem[j] = 0;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_old_trib_slots[j] = 0;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_new_trib_slots[j] = 0;
              digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_cal_slots_add_rem[j] = 0;
        }


        digi_handle->var.ghao_cfg.ghao_segments[i].ts_start = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].ts_target = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh = 0;

        digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_thresh_pre_post_fw = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].sifd_mpmo_thresh_pre_post_fw = 0;

        digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_userbit_update_pre_post_fw = 0;
        digi_handle->var.ghao_cfg.ghao_segments[i].sifd_oduksw_cal_update_pre_post_fw = 0;

    }

    PMC_RETURN(ret_val);

} /* digi_ghao_ctxt_init */

/*******************************************************************************
* digi_ghao_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys the G.HAO context
*   
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_ctxt_destroy(digi_handle_t *digi_handle)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_CTXT_FREE(&digi_handle->var.ghao_cfg.ghao_segments, digi_handle);
    PMC_CTXT_FREE(&digi_handle->var.ghao_cfg.ghao_fw_segments, digi_handle);  

    PMC_RETURN(ret_val);

} /* digi_ghao_ctxt_destroy */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_ghao_segment_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   G.HAO software is still under development.  The arguments of this
*   API are likely to change.
*
*   Before performing any G.HAO operations, it's first required to provision
*   the G.HAO segment.  This function returns a ghao_segment_id that is
*   used for all other G.HAO operations.
*
*  For increase scenerio follow these steps:
*    -- digi_ghao_segment_prov
*    -- digi_ghao_lcr_size
*    -- [call sifd knife edge sequence here if SIFD is in the path]
*    -- digi_ghao_lcr_knife_edge
*    -- digi_ghao_bwr_setup
*    -- digi_ghao_bwr_ramp_start
*    -- digi_ghao_bwr_ramp_end
*
*  For decrease scenerio follow these steps:
*    -- digi_ghao_segment_prov
*    -- digi_ghao_bwr_setup
*    -- digi_ghao_bwr_ramp_start
*    -- digi_ghao_lcr_knife_edge
*    -- [call sifd knife edge sequence here if SIFD is in the path]
*    -- digi_ghao_lcr_size
*    -- digi_ghao_bwr_ramp_end
*  
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   chnl1_ptr             - Channel context pointer of the G.HAO segment
*                           that is rx/tx of the ODUKSW
*                           MAP, COREOTN, or SIFD.
*   chnl2_ptr             - Channel context pointer of the G.HAO segment
*                           that is rx/tx of the ODUKSW
*                           MAP, COREOTN, or SIFD (intermediate node
*                           LINE <-> LINE, provide COREOTN for cnhl2_ptr)
*   target_ts             - The desired number of tribslots that the
*                           lo will occupy by the end of the ramp
*   old_trib_slot_mask_rx - The initial tribslot mask of the ODUFLEX
*                           before ramp in the rx path
*                           (only required if COREOTN is in the path)
*   old_trib_slot_mask_tx - The initial tribslot mask of the ODUFLEX
*                           before ramp in the tx path
*                           (only required if COREOTN is in the path)
*   new_trib_slot_mask_rx - The desired tribslot mask of the ODUFLEX
*                           when the ramp is done in the rx path
*                           (only required if COREOTN is in the path)
*   new_trib_slot_mask_tx - The desired tribslot mask of the ODUFLEX
*                           when the ramp is done in the tx path
*                           (only required if COREOTN is in the path)
*   sifd_old_cal_mask     - Reserved
*   sifd_new_cal_mask     - Reserved
*   oif_pkt_size          - desired packet size defined in OIF recommendations.
*                           Valid values are:
*                           -- 128B
*                           -- 256B
*                           -- 512B
*   header_size           - Header sizer over the fabric backplane
*   fw_tick_us            - The firmware tick in us (making this configurable for
*                           now).  Suggested config is 100000 us.
*
* OUTPUTS:
*   ghao_segment_id   - The segment ID returned by the software
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
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
                                        UINT32 fw_tick_us)
{
    int i;

    PMC_ERROR ret_val = PMC_SUCCESS;
    coreotn_oduksw_input_port_type_t chnl1_port_type;
    coreotn_oduksw_input_port_type_t chnl2_port_type;
    util_global_oduk_port_data_t *util_global_oduk_port1;
    util_global_oduk_port_data_t *util_global_oduk_port2;

    PMC_ATOMIC_ENTRY(digi_handle);


    util_global_oduk_port1 = &(((util_global_switch_data_def_t *)chnl1_ptr)->oduk_data);
    chnl1_port_type = (coreotn_oduksw_input_port_type_t)util_global_oduk_port1->port_type;
    
    util_global_oduk_port2 = &(((util_global_switch_data_def_t *)chnl2_ptr)->oduk_data);
    chnl2_port_type = (coreotn_oduksw_input_port_type_t)util_global_oduk_port2->port_type;

    if(target_ts == 0)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    /* Obtain a G.HAO id from the pool */
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = digi_ghao_segment_id_get(digi_handle,
                                           ghao_segment_id);
    }

    if(ret_val == PMC_SUCCESS)
    {
        digi_handle->var.ghao_cfg.fw_tick_us = fw_tick_us;
        
        /* Copy parameters */
        if (chnl1_port_type == COREOTN_ODUKSW_INPUT_COREOTN ||
            chnl2_port_type == COREOTN_ODUKSW_INPUT_COREOTN)
        {
            for (i=0; i<3; ++i)
            {
                digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].old_trib_slot_mask_rx[i] = old_trib_slot_mask_rx[i];
                digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].old_trib_slot_mask_tx[i] = old_trib_slot_mask_tx[i];
                digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].new_trib_slot_mask_rx[i] = new_trib_slot_mask_rx[i];
                digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].new_trib_slot_mask_tx[i] = new_trib_slot_mask_tx[i];
            }
            PMC_LOG_TRACE("digi_ghao_segment_prov(): old_trib_slot_mask_rx= %08X %08X %08X, new_trib_slot_mask_rx= %08X %08X %08X\n", old_trib_slot_mask_rx[2], old_trib_slot_mask_rx[1], old_trib_slot_mask_rx[0], new_trib_slot_mask_rx[2], new_trib_slot_mask_rx[1], new_trib_slot_mask_rx[0]);
            PMC_LOG_TRACE("digi_ghao_segment_prov(): old_trib_slot_mask_tx= %08X %08X %08X, new_trib_slot_mask_tx= %08X %08X %08X\n", old_trib_slot_mask_tx[2], old_trib_slot_mask_tx[1], old_trib_slot_mask_tx[0], new_trib_slot_mask_tx[2], new_trib_slot_mask_tx[1], new_trib_slot_mask_tx[0]);

            if (new_trib_slot_mask_rx[0] == old_trib_slot_mask_tx[0] &&
                new_trib_slot_mask_rx[1] == old_trib_slot_mask_tx[1] &&
                new_trib_slot_mask_rx[2] == old_trib_slot_mask_tx[2])
            {
                PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
            }
        }

    }


    if(ret_val == PMC_SUCCESS)
    {
          
          /*set the pkt_size and header_size in the context */
          digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].oif_pkt_size = oif_pkt_size;
          digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].header_size = header_size;


          if(chnl1_port_type == COREOTN_ODUKSW_INPUT_COREOTN)
          {

              if(chnl2_port_type == COREOTN_ODUKSW_INPUT_COREOTN)
              {
                  /*populate LINE_LINE segment context*/
                  ret_val = digi_ghao_pop_line_line_ctxt(digi_handle,
                                                         util_global_oduk_port1,
                                                         target_ts,
                                                         old_trib_slot_mask_rx,
                                                         old_trib_slot_mask_tx,
                                                         new_trib_slot_mask_rx,
                                                         new_trib_slot_mask_tx,
                                                         *ghao_segment_id);

              } 
              else if(chnl2_port_type == COREOTN_ODUKSW_INPUT_MAPOTN)
              {
                  /*populate LINE_MAP segment context*/
                  ret_val = digi_ghao_pop_line_map_ctxt(digi_handle,
                                                        util_global_oduk_port1,
                                                        util_global_oduk_port2,
                                                        target_ts,
                                                        old_trib_slot_mask_rx,
                                                        old_trib_slot_mask_tx,
                                                        new_trib_slot_mask_rx,
                                                        new_trib_slot_mask_tx,
                                                        *ghao_segment_id);

              }
              else if(chnl2_port_type == COREOTN_ODUKSW_INPUT_SIFD1 ||
                      chnl2_port_type == COREOTN_ODUKSW_INPUT_SIFD2 )
              {
                  /*populate LINE_SIFD segment context*/
                  ret_val = digi_ghao_pop_line_sifd_ctxt(digi_handle,
                                                         util_global_oduk_port1,
                                                         util_global_oduk_port2,
                                                         target_ts,
                                                         old_trib_slot_mask_rx,
                                                         old_trib_slot_mask_tx,
                                                         new_trib_slot_mask_rx,
                                                         new_trib_slot_mask_tx,
                                                         sifd_old_cal_mask,
                                                         sifd_new_cal_mask,
                                                         *ghao_segment_id);
              }
          }
          else if(chnl1_port_type == COREOTN_ODUKSW_INPUT_MAPOTN)
          {
              if(chnl2_port_type == COREOTN_ODUKSW_INPUT_COREOTN)
              {
                  /*populate LINE_MAP segment context*/
                  ret_val = digi_ghao_pop_line_map_ctxt(digi_handle,
                                                        util_global_oduk_port2,
                                                        util_global_oduk_port1,
                                                        target_ts,
                                                        old_trib_slot_mask_rx,
                                                        old_trib_slot_mask_tx,
                                                        new_trib_slot_mask_rx,
                                                        new_trib_slot_mask_tx,
                                                       *ghao_segment_id);
              }
              else if(chnl2_port_type == COREOTN_ODUKSW_INPUT_SIFD1 ||
                      chnl2_port_type == COREOTN_ODUKSW_INPUT_SIFD2)
              {
                  /* populate MAP_SIFD segment contect*/
                  ret_val = digi_ghao_pop_map_sifd_ctxt(digi_handle,
                                                         util_global_oduk_port1,
                                                         util_global_oduk_port2,
                                                         sifd_old_cal_mask,
                                                         sifd_new_cal_mask,
                                                         target_ts,
                                                         *ghao_segment_id);
              }

          }

          else if(chnl1_port_type == COREOTN_ODUKSW_INPUT_SIFD1 ||
                  chnl1_port_type == COREOTN_ODUKSW_INPUT_SIFD2)
          {
              if(chnl2_port_type == COREOTN_ODUKSW_INPUT_COREOTN)
              {
                  /*populate LINE_LINE segment context*/
                  ret_val = digi_ghao_pop_line_sifd_ctxt(digi_handle,
                                                         util_global_oduk_port2,
                                                         util_global_oduk_port1,
                                                         target_ts,
                                                         old_trib_slot_mask_rx,
                                                         old_trib_slot_mask_tx,
                                                         new_trib_slot_mask_rx,
                                                         new_trib_slot_mask_tx,
                                                         sifd_old_cal_mask,
                                                         sifd_new_cal_mask,
                                                         *ghao_segment_id);
              }
              else if(chnl2_port_type == COREOTN_ODUKSW_INPUT_MAPOTN)
              {   
                  /*populate MAP_SIFD segment context */
                  ret_val = digi_ghao_pop_map_sifd_ctxt(digi_handle,
                                                        util_global_oduk_port2,
                                                        util_global_oduk_port1,
                                                        sifd_old_cal_mask,
                                                        sifd_new_cal_mask,
                                                        target_ts,
                                                        *ghao_segment_id);
              }
          }
    } 

    if (digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].ts_target == digi_handle->var.ghao_cfg.ghao_segments[*ghao_segment_id].ts_start)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }


    PMC_ATOMIC_RETURN(digi_handle,ret_val);

} /* digi_ghao_segment_prov */



/*******************************************************************************
* digi_ghao_segment_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   After the G.HAO operations are completed on a segment, it is required to
*   deprovision the G.HAO segment. Then the segment ID can be reused for a new
*   G.HAO sequence.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_segment_deprov(digi_handle_t *digi_handle,
                                          UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_LOG_TRACE("G.HAO: digi_ghao_segment_deprov(): digi_handle=%p, Deprovision ghao_segment_id %d,\n", digi_handle, ghao_segment_id);


    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);

    if(ret_val == PMC_SUCCESS)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].segment_in_use = FALSE;
    }

    PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
} /* digi_ghao_segment_deprov */



/*******************************************************************************
* digi_ghao_step_lcr_size
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs the link connection resize step.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*   ghao_dir               - G.HAO datapath direction (TX or RX)
*   ghao_step              - Step to perform in G.HAO sequence
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_step_lcr_size(digi_handle_t *digi_handle,
                                         UINT32 ghao_segment_id,
                                         digi_ghao_dir_t ghao_dir,
                                         digi_ghao_step_t ghao_step)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    digi_ghao_datapath_t datapath_type;
    BOOL8 ghao_increase;
    util_global_oduk_port_t chnl1_port_type;

    PMC_ATOMIC_ENTRY(digi_handle);


    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);
    if(ret_val != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,ret_val); 
    }

    datapath_type = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type;
    ghao_increase = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase;

    /* State checking */
    if (ghao_dir == DIGI_GHAO_TX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_lcr_size(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_tx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
    if (ghao_dir == DIGI_GHAO_RX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_lcr_size(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_lcr_size(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, ghao_step);

    switch (datapath_type) {

        case DIGI_LINE_MAP :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_tx_inc_rp(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_tx_inc(digi_handle,
                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_rx_inc_rp(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_rx_inc(digi_handle,
                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_tx_dec_rp(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_tx_dec(digi_handle,
                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_rx_dec_rp(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_map_lcr_size_rx_dec(digi_handle,
                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_LINE_LINE :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_tx_inc_rp(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_tx_inc(digi_handle,
                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_rx_inc_rp(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_rx_inc(digi_handle,
                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_tx_dec_rp(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_tx_dec(digi_handle,
                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_rx_dec_rp(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                    {
                        ret_val = digi_ghao_line_line_lcr_size_rx_dec(digi_handle,
                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_MAP_SIFD :
            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {   
               if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_tx_inc_rp(digi_handle,
                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_tx_inc(digi_handle,
                                                                         ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_rx_inc_rp(digi_handle,
                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_rx_inc(digi_handle,
                                                                         ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_SIFD;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_tx_dec_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_tx_dec(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_rx_dec_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_size_rx_dec(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
            }
            else
            {

            }

            break;

        case DIGI_LINE_SIFD :
            
            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type; 

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {

                if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_tx_inc_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_tx_inc(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_rx_inc_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_rx_inc(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_SIFD;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_tx_dec_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_tx_dec(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_SIZE_1_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_rx_dec_rp(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_SIZE_2_ALL)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_size_rx_dec(digi_handle,
                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }

            }
            else
            {

            }
            break;

        default:
            break;
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_lcr_size(): Update state: digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step_tx=%d, ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);


    PMC_ATOMIC_RETURN(digi_handle, ret_val);

} /* digi_ghao_step_lcr_size */



/*******************************************************************************
* digi_ghao_step_lcr_knife_edge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs the link connection resize knife edge step.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*   ghao_dir               - G.HAO datapath direction (TX or RX)
*   ghao_step              - Step to perform in G.HAO sequence
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_step_lcr_knife_edge(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id,
                                               digi_ghao_dir_t ghao_dir,
                                               digi_ghao_step_t ghao_step)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    digi_ghao_datapath_t datapath_type;
    BOOL8 ghao_increase;
    util_global_oduk_port_t chnl1_port_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);
    if(ret_val != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,ret_val); 
    }

    datapath_type = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type;
    ghao_increase = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase;

    /* State checking */
    if (ghao_dir == DIGI_GHAO_TX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_lcr_knife_edge(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_tx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
    if (ghao_dir == DIGI_GHAO_RX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_lcr_knife_edge(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_lcr_knife_edge(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, ghao_step);

    switch (datapath_type) {

        case DIGI_LINE_MAP :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_inc_1_add_ack(digi_handle,
                                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_inc_1_add_ack(digi_handle,
                                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle,
                                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_tx_dec_4_rp(digi_handle,
                                                                                ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle,
                                                                                     ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                    {
                        ret_val = digi_ghao_line_map_lcr_knife_edge_rx_dec_4_rp(digi_handle,
                                                                                ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_LINE_LINE :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_inc_1_add_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle,
                                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_inc_1_add_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle,
                                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle,
                                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_tx_dec_4_rp(digi_handle,
                                                                                 ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle,
                                                                                      ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle,
                                                                                       ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle,
                                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                    {
                        ret_val = digi_ghao_line_line_lcr_knife_edge_rx_dec_4_rp(digi_handle,
                                                                                 ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_MAP_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {


                if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_tx_inc_1_add_ack(digi_handle,
                                                                                         ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_ilkn(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_rx_inc_1_add_ack(digi_handle,
                                                                                         ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_ilkn(digi_handle,
                                                                             ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_map_ilkn_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }

            }
            else
            {

            }

            break;

        case DIGI_LINE_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {

                if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_1_add_ack(digi_handle,
                                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle,
                                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_ilkn(digi_handle,
                                                                              ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_ADD_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_1_add_ack(digi_handle,
                                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle,
                                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_1_TSCC;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle,
                                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle,
                                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_4_rp(digi_handle,
                                                                                     ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_SIFD)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_ilkn(digi_handle,
                                                                              ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_1_REM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle,
                                                                                          ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_2_NORM_ACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle,
                                                                                           ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_3_IDLE_NACK)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle,
                                                                                            ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_4_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_LCR_KNIFE_EDGE_4_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_4_rp(digi_handle,
                                                                                     ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }

            }
            else
            {

            }

            break;

        default:
            break;
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_lcr_knife_edge(): Update state: digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step_tx=%d, ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);


    PMC_ATOMIC_RETURN(digi_handle, ret_val);

} /* digi_ghao_step_lcr_knife_edge */



/*******************************************************************************
* digi_ghao_step_bwr_setup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets up the datapath for BWR step
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*   ghao_dir               - G.HAO datapath direction (TX or RX)
*   ghao_step              - Step to perform in G.HAO sequence
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_step_bwr_setup(digi_handle_t *digi_handle,
                                          UINT32 ghao_segment_id,
                                          digi_ghao_dir_t ghao_dir,
                                          digi_ghao_step_t ghao_step)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    digi_ghao_datapath_t datapath_type;
    BOOL8 ghao_increase;
    util_global_oduk_port_t chnl1_port_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);
    if(ret_val != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,ret_val); 
    }

    datapath_type = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type;
    ghao_increase = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase;

    /* State checking */
    if (ghao_dir == DIGI_GHAO_TX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_setup(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_tx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
    if (ghao_dir == DIGI_GHAO_RX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_setup(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_setup(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, ghao_step);

    switch (datapath_type) {

        case DIGI_LINE_MAP :

            if (ghao_dir == DIGI_GHAO_TX)
            {
                /* TX Increase/Decrease */
                if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                {
                    ret_val = digi_ghao_line_map_bwr_setup_tx_1_tscc(digi_handle,
                                                                     ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_2_NCS;
                }
                else if (ghao_step == DIGI_GHAO_BWR_SETUP_2_NCS)
                {
                    ret_val = digi_ghao_line_map_bwr_setup_tx_2_ncs(digi_handle,
                                                                    ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_START;
                }
                else
                {
                    PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                }
            }
            else
            {
                /* RX Increase/Decrease */
                if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                {
                    ret_val = digi_ghao_line_map_bwr_setup_rx_1_tscc(digi_handle,
                                                                     ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_2_NCS;
                }
                else if (ghao_step == DIGI_GHAO_BWR_SETUP_2_NCS)
                {
                    ret_val = digi_ghao_line_map_bwr_setup_rx_2_ncs(digi_handle,
                                                                    ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_START;
                }
                else
                {
                    PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                }
            }
            break;

        case DIGI_LINE_LINE :

            if (ghao_dir == DIGI_GHAO_TX)
            {
                /* TX Increase/Decrease */
                if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                {
                    ret_val = digi_ghao_line_line_bwr_setup_tx_1_tscc(digi_handle,
                                                                      ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_START;
                }
                else
                {
                    PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                }
            }
            else
            {
                /* RX Increase/Decrease */
                if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                {
                    ret_val = digi_ghao_line_line_bwr_setup_rx_1_tscc(digi_handle,
                                                                      ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_START;
                }
                else
                {
                    PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                }
            }
            break;

        case DIGI_MAP_SIFD :
       
            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;       

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {

                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase/Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                    {
                        ret_val = digi_ghao_map_ilkn_bwr_setup_tx_1_tscc(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_SETUP_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_SETUP_2_NCS)
                    {
                        ret_val = digi_ghao_map_ilkn_bwr_setup_tx_2_ncs(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_START;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase/Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                    {
                        ret_val = digi_ghao_map_ilkn_bwr_setup_rx_1_tscc(digi_handle,
                                                                         ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_SETUP_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_SETUP_2_NCS)
                    {
                        ret_val = digi_ghao_map_ilkn_bwr_setup_rx_2_ncs(digi_handle,
                                                                        ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_START;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }

            }
            else
            {

            }

            break;

        case DIGI_LINE_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {

                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase/Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                    {
                        ret_val = digi_ghao_line_ilkn_bwr_setup_tx_1_tscc(digi_handle,
                                                                          ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_START;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase/Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_SETUP_1_TSCC)
                    {
                        ret_val = digi_ghao_line_ilkn_bwr_setup_rx_1_tscc(digi_handle,
                                                                          ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_START;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
        
            }
            else
            {

            }

            break;

        default:
            break;
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_setup(): Update state: digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step_tx=%d, ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);


    PMC_ATOMIC_RETURN(digi_handle, ret_val);

} /* digi_ghao_step_bwr_setup */



/*******************************************************************************
* digi_ghao_step_bwr_ramp_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Start BWR ramp step
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*   ghao_dir               - G.HAO datapath direction (TX or RX)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_step_bwr_ramp_start(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id,
                                               digi_ghao_dir_t ghao_dir)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    digi_ghao_datapath_t datapath_type;
    BOOL8 ghao_increase;
    util_global_oduk_port_t chnl1_port_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);
    if(ret_val != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,ret_val); 
    }

    datapath_type = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type;
    ghao_increase = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase;

    /* State checking */
    if (ghao_dir == DIGI_GHAO_TX &&
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx != DIGI_GHAO_BWR_START)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_ramp_start(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_tx=%d\n", digi_handle, ghao_segment_id, datapath_type, DIGI_GHAO_BWR_START, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
    if (ghao_dir == DIGI_GHAO_RX &&
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx != DIGI_GHAO_BWR_START)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_ramp_start(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, DIGI_GHAO_BWR_START, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_ramp_start(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir);

    switch (datapath_type) {

        case DIGI_LINE_MAP :

            if (ghao_dir == DIGI_GHAO_TX)
            {
                /* TX Increase/Decrease */
                ret_val = digi_ghao_line_map_bwr_ramp_start_tx(digi_handle,
                                                               ghao_segment_id);
                digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_1_TSCC;
            }
            else
            {
                /* RX Increase/Decrease */
                ret_val = digi_ghao_line_map_bwr_ramp_start_rx(digi_handle,
                                                               ghao_segment_id);
                digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_1_TSCC;
            }
            break;

        case DIGI_LINE_LINE :

            if (ghao_dir == DIGI_GHAO_TX)
            {
                /* TX Increase/Decrease */
                ret_val = digi_ghao_line_line_bwr_ramp_start_tx(digi_handle,
                                                               ghao_segment_id);
                digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_1_TSCC;
            }
            else
            {
                /* RX Increase/Decrease */
                ret_val = digi_ghao_line_line_bwr_ramp_start_rx(digi_handle,
                                                               ghao_segment_id);
                digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_1_TSCC;
            }
            break;

        case DIGI_MAP_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase/Decrease */
                    ret_val = digi_ghao_map_ilkn_bwr_ramp_start_tx(digi_handle,
                                                                   ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_1_TSCC;
                }
                else
                {
                    /* RX Increase/Decrease */
                    ret_val = digi_ghao_map_ilkn_bwr_ramp_start_rx(digi_handle,
                                                                   ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_1_TSCC;
                }

            }
            else
            {

            }

            break;

        case DIGI_LINE_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase/Decrease */
                    ret_val = digi_ghao_line_ilkn_bwr_ramp_start_tx(digi_handle,
                                                                   ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_1_TSCC;
                }
                else
                {
                    /* RX Increase/Decrease */
                    ret_val = digi_ghao_line_ilkn_bwr_ramp_start_rx(digi_handle,
                                                                   ghao_segment_id);
                    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_1_TSCC;
                }

            }
            else
            {

            }

            break;

        default:
            break;
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_ramp_start(): Update state: digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step_tx=%d, ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);






    PMC_ATOMIC_RETURN(digi_handle, ret_val);

} /* digi_ghao_step_bwr_ramp_start */



/*******************************************************************************
* digi_ghao_step_bwr_ramp_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This need to be called when the ramp is complete to put to datapath
*  back into normal operating more.  This API will deprovision the G.HAO
*  segment and free up the ghao_segment_id
*
*  For source nodes, call once RAMP #2 done interrupt is seen
*  in MAPOTN-ODUKSC.
*
*  For intermediate nodes, call after source nodes have
*  completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*   ghao_dir               - G.HAO datapath direction (TX or RX)
*   ghao_step              - Step to perform in G.HAO sequence
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_step_bwr_ramp_end(digi_handle_t *digi_handle,
                                             UINT32 ghao_segment_id,
                                             digi_ghao_dir_t ghao_dir,
                                             digi_ghao_step_t ghao_step)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    digi_ghao_datapath_t datapath_type;
    BOOL8 ghao_increase;
    util_global_oduk_port_t chnl1_port_type;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    ret_val = digi_ghao_validate_segment_id(digi_handle,
                                            ghao_segment_id);
    if(ret_val != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,ret_val); 
    }

    datapath_type = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type;
    ghao_increase = digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase;


    /* State checking */
    if (ghao_dir == DIGI_GHAO_TX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_ramp_end(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_tx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
    if (ghao_dir == DIGI_GHAO_RX &&
        ghao_step != digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx)
    {
        PMC_LOG_TRACE("G.HAO: ERROR digi_ghao_step_bwr_ramp_end(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_step=%d != ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_step, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_ramp_end(): digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, ghao_step);

    if(ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
    {
        /* wait 1 ms at the beginning of BWR END to ensure that the system is stable */
        PMC_Q_USLEEP(digi_handle,1000);
    }


    switch (datapath_type) {

        case DIGI_LINE_MAP :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                            ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_tx_2_ncs(digi_handle,
                                                                           ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_3_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_tx_3_inc_rp(digi_handle,
                                                                              ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                            ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_rx_2_ncs(digi_handle,
                                                                           ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_3_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_rx_3_inc_rp(digi_handle,
                                                                              ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                            ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_tx_2_ncs(digi_handle,
                                                                           ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                            ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_2_NCS;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                    {
                        ret_val = digi_ghao_line_map_bwr_ramp_end_rx_2_ncs(digi_handle,
                                                                           ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_LINE_LINE :

            if (ghao_increase)
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Increase */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                             ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_3_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_tx_3_inc_rp(digi_handle,
                                                                               ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Increase */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                             ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_3_RP;
                    }
                    else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_rx_3_inc_rp(digi_handle,
                                                                               ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            else
            {
                if (ghao_dir == DIGI_GHAO_TX)
                {
                    /* TX Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                             ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
                else
                {
                    /* RX Decrease */
                    if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                    {
                        ret_val = digi_ghao_line_line_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                             ghao_segment_id);
                        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                    }
                    else
                    {
                        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                    }
                }
            }
            break;

        case DIGI_MAP_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {
                  /*perform increase on MAP<->ILKN segment*/
                if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                                ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_2_NCS;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_tx_2_ncs(digi_handle,
                                                                               ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_3_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_tx_3_inc_rp(digi_handle,
                                                                                  ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                                ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_2_NCS;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_rx_2_ncs(digi_handle,
                                                                               ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_3_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_rx_3_inc_rp(digi_handle,
                                                                                  ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                                ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_2_NCS;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_tx_2_ncs(digi_handle,
                                                                               ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                                ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_2_NCS;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_2_NCS)
                        {
                            ret_val = digi_ghao_map_ilkn_bwr_ramp_end_rx_2_ncs(digi_handle,
                                                                               ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_KNIFE_EDGE_SIFD;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }

            }
            else
            { 

            }
            break;
        case DIGI_LINE_SIFD :

            chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;

            if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
               chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
            {    
                if (ghao_increase)
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Increase */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                                 ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_BWR_END_3_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_tx_3_inc_rp(digi_handle,
                                                                                   ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Increase */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                                 ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_BWR_END_3_RP;
                        }
                        else if (ghao_step == DIGI_GHAO_BWR_END_3_RP)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_rx_3_inc_rp(digi_handle,
                                                                                   ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_NONE;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }
                else
                {
                    if (ghao_dir == DIGI_GHAO_TX)
                    {
                        /* TX Decrease */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle,
                                                                                 ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                    else
                    {
                        /* RX Decrease */
                        if (ghao_step == DIGI_GHAO_BWR_END_1_TSCC)
                        {
                            ret_val = digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle,
                                                                                 ghao_segment_id);
                            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_2_ALL;
                        }
                        else
                        {
                            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
                        }
                    }
                }

            }
            else
            {

            }

            break;
        default:
            break;
    }

    PMC_LOG_TRACE("G.HAO: digi_ghao_step_bwr_ramp_end(): Update state: digi_handle=%p, ghao_segment_id=%d, datapath_type=%d, ghao_dir=%d, ghao_step_tx=%d, ghao_step_rx=%d\n", digi_handle, ghao_segment_id, datapath_type, ghao_dir, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx);


    PMC_ATOMIC_RETURN(digi_handle, ret_val);

} /* digi_ghao_step_bwr_ramp_end */


/*******************************************************************************
* digi_ghao_ilkn_oduk_fc_calendar_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   NOTE : It is desired to not reprovision the ILKN flow control calendar
*          because there is not a dual calendar implementation in DIGI.
*          ILKN will always work with only 1 entry programmed per channel
*          so there is no need to reprovision.
*
*   This function takes in the Tx and Rx Flow control calendar data and 
*   applies and deltas to the current configuration to the device.
*
*   This API is intended to be used to reprovisioning the FC calendar on an
*   already operational channel.
*
*   For a G.HAO increase:
*     - call this API with the target FC calendar before BWR
*     - Change TX FC calendar on device across packet fabric before this
*       DIGIs's RX FC calendar
*     - Change TX FC calendar on this DIGI before changing device
*       across packet fabric's RX FC calendar
*   For a G.HAO decrease:
*     - call this API with the target FC calendar after BWR.
*     - Change RX FC calenndar on device across packet fabric before this
*       DIGIs's TX FC calendar
*     - Change RX FC calendar on this DIGI before changing device
*       across packet fabric's TX FC calendar
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *chnl_ctxt_ptr         - a pointer to a pointer to a digi_ilkn_chnl_t
*                            structure consisting of a channel data this is
*                            set to NULL for the user.
*   update_rx              - TRUE : update the RX ILKN FC calendar
*                            FALSE : do not update RX FC calendar
*   update_tx              - TRUE : update the TX ILKN FC calendar
*                            FALSE : do not update TX FC calendar
*   *tx_ilkn_fc_ptr        - Defines the set of flow control calendar entries
*                            that an outbound CPB channel will respond to for
*                            XON/XOFF states. If not used pass in NULL.
*                            Used for output_port ILKN1/ILKN2 only.
*   *rx_ilkn_fc_ptr        - Defines the set of flow control calendar entries
*                            that an inbound CPB channel will respond to for
*                            XON/XOFF states. If not used pass in NULL.
*                            Used for input_port ILKN1/ILKN2 only.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS on success or specific error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_ilkn_oduk_fc_calendar_reprov(digi_handle_t *digi_handle,
                                                        digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                        BOOL update_rx,
                                                        BOOL update_tx,
                                                        digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                                        digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    digi_ilkn_chnl_def_t* chnl_data_ptr;
    digi_sifd_ilkn_inst_t sifd_ilkn_inst = LAST_DIGI_SIFD_ILKN;
    BOOL is_oduksw=FALSE;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Check that given handles belong to digi handle */
    DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    /* check that DIGI device is initialized */
    if(FALSE == digi_device_init_status_get(digi_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_DEVICE_NOT_INIT);
    }
    
    chnl_data_ptr = (digi_ilkn_chnl_def_t*)chnl_ctxt_ptr;
    
    if (TRUE == update_rx)
    {
        if (FALSE == util_global_chnl_prov_status_get(chnl_data_ptr->switch_data.header.prov_state, UTIL_GLOBAL_DIR_RX)) 
        {
            
            PMC_ASSERT(FALSE, DIGI_ERR_ILKN_CHNL_NOT_PROV, 0, 0);
            
        }
    }
    if (TRUE == update_tx)
    {
        if (FALSE == util_global_chnl_prov_status_get(chnl_data_ptr->switch_data.header.prov_state, UTIL_GLOBAL_DIR_TX)) 
        {
            
            PMC_ASSERT(FALSE, DIGI_ERR_ILKN_CHNL_NOT_PROV, 0, 0);
            
        }
    }

    switch(chnl_data_ptr->target)
    {
        case DIGI_ILKN_CHNL_TARGET_CPB:
            is_oduksw = FALSE;
            break;
        case DIGI_ILKN_CHNL_TARGET_ODUK:
            is_oduksw = TRUE;
            break;
        default:
            result = DIGI_ERR_INVALID_ARG;
            break;
    }

    if(is_oduksw)
    {

        switch(chnl_data_ptr->switch_data.oduk_data.port_type)
        {
        case UTIL_GLOBAL_ODUK_PORT_ILKN1:
            sifd_ilkn_inst = DIGI_SIFD_ILKN1;
            break;
        case UTIL_GLOBAL_ODUK_PORT_ILKN2:
            sifd_ilkn_inst = DIGI_SIFD_ILKN2;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }

    }
    else
    {

        switch(chnl_data_ptr->switch_data.cpb_data.port_type)
        {
        case UTIL_GLOBAL_CPB_PORT_ILKN1:
            sifd_ilkn_inst = DIGI_SIFD_ILKN1;
            break;
        case UTIL_GLOBAL_CPB_PORT_ILKN2:
            sifd_ilkn_inst = DIGI_SIFD_ILKN2;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
        }

    }





    /* write the DIGI layer SIFD flow control calendar */
    if (PMC_SUCCESS == result)
    {
        
        result = digi_sifd_fc_calendar_save(digi_handle, 
                                            (sifd_ilkn_inst_t)sifd_ilkn_inst,
                                            tx_ilkn_fc_ptr,
                                            rx_ilkn_fc_ptr);
    }


    /*update TX flow control calendar*/
    if (PMC_SUCCESS == result 
        && update_tx == TRUE)
    {

        result = digi_sifd_fc_calendar_update(digi_handle,
                                              (sifd_ilkn_inst_t)sifd_ilkn_inst,
                                              (UINT32)chnl_data_ptr->switch_data.oduk_data.port_type,
                                              TRUE,
                                              is_oduksw,
                                              SIFD_ILKN_NULL_CAL_ENTRY);
    }

    /*update RX flow control calendar*/
    if (PMC_SUCCESS == result
        && update_rx == TRUE)
    {

        result = digi_sifd_fc_calendar_update(digi_handle,
                                              (sifd_ilkn_inst_t)sifd_ilkn_inst,
                                              (UINT32)chnl_data_ptr->switch_data.oduk_data.port_type,
                                              FALSE,
                                              is_oduksw,
                                              SIFD_ILKN_NULL_CAL_ENTRY);
    }



    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_ghao_ilkn_oduk_fc_calendar_reprov */

/*******************************************************************************
* digi_ghao_bwr_firmware_execute
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Note, this API exectue the G.HAO "prototype" firmware.  This function
*   performs the during the ramp adjustments that will eventually be performed
*   in firmware.  It take ~2.5 seconds per tribslot added/removed to perform
*   these adjustments, so this is not a production solution. When the
*   G.HAO firmware is complete, this API will be removed.
* 
*   This routine will poll for the BWR_IND in COREOTN's RFRMs and perform
*   the nessecary consequential actions.  It will also perform the
*   incremental XOFF threshold increases/decreases in the datapath as 
*   the rate ramps.
*
* INPUTS:
*   *digi_handle1           - pointer to DIGI1 handle instance
*   *digi_handle2           - pointer to DIGI2 handle instance
*   *digi_handle3           - pointer to DIGI3 handle instance
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_bwr_firmware_execute(digi_handle_t *digi_handle1,
                                                digi_handle_t *digi_handle2,
                                                digi_handle_t *digi_handle3)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 time_current_sec = 0;
    UINT32 time_current_usec = 0;
    UINT32 time_start_sec = 0;
    UINT32 time_start_usec = 0;
    UINT32 current_tick = 0;
    UINT32 new_tick = 0;
    UINT32 sec_diff;
    UINT32 usec_diff;
    DOUBLE new_tick_double;
    UINT32 i;
    BOOL8 ramp_done = FALSE;
    UINT32 fw_tick_us;

    DOUBLE timeout_time = 0;

    PMC_ATOMIC_ENTRY(digi_handle1);


    /*populate the firmware context for both DIGIs*/
    
    ret_val = digi_ghao_pop_firmware_ctxt(digi_handle1);

    if(digi_handle2 != NULL)
    {
        ret_val = digi_ghao_pop_firmware_ctxt(digi_handle2);
    }
    if(digi_handle3 != NULL)
    {
        ret_val = digi_ghao_pop_firmware_ctxt(digi_handle3);
    }

    fw_tick_us = digi_handle1->var.ghao_cfg.fw_tick_us;

    PMC_TIME_GET(time_start_sec,time_start_usec);   
    
    /* perform "firmware" routine 
    NOTE.  This routine must be performed without prints*/
    while(ramp_done == FALSE)
    {
          
          PMC_TIME_GET(time_current_sec,time_current_usec);

          /*consider roll over */
          if(time_current_usec < time_start_usec) 
          {
            
            sec_diff = time_current_sec-time_start_sec-1;
            usec_diff = 1000000+time_current_usec-time_start_usec;
          
          } 
          else 
          {
            sec_diff = time_current_sec - time_start_sec;
            usec_diff = time_current_usec - time_start_usec;
          }


#ifndef PMC_SW_SIMULATION
          new_tick_double = ((DOUBLE)sec_diff/((DOUBLE)fw_tick_us/1000000) + (DOUBLE)usec_diff/fw_tick_us);
          new_tick = (UINT32)new_tick_double;

          if(new_tick_double - new_tick > 0.5)
          { 
              new_tick++;          
          }

#else
          new_tick_double = 0;
          new_tick++;
#endif
          


          /*determine if a firmware tick has passed*/
          if(new_tick == current_tick+1)
          {
                current_tick = new_tick;

                /*perform firmware tick on DIGI1 and DIGI2*/
                digi_ghao_do_fw_tick(digi_handle1);

                if(digi_handle2 != NULL)
                {
                    digi_ghao_do_fw_tick(digi_handle2);
                }
                if(digi_handle3 != NULL)
                {
                    digi_ghao_do_fw_tick(digi_handle3);
                }

                ramp_done = TRUE;

                /*check if the ramp is done on DIGI1 */
                for(i=0;i<digi_handle1->var.ghao_cfg.num_fw_segments;i++)
                {
                    if(digi_handle1->var.ghao_cfg.ghao_fw_segments[i].state != DIGI_GHAO_FW_DONE)
                    {
                        ramp_done = FALSE;
                    }
                }

                if(digi_handle2 != NULL)
                {
                    /*check if the ramp is done of DIGI2*/
                    for(i=0;i<digi_handle2->var.ghao_cfg.num_fw_segments;i++)
                    {
                        if(digi_handle2->var.ghao_cfg.ghao_fw_segments[i].state != DIGI_GHAO_FW_DONE)
                        {
                            ramp_done = FALSE;
                        }
                    }
                }
                if(digi_handle3 != NULL)
                {
                    /*check if the ramp is done of DIGI2*/
                    for(i=0;i<digi_handle3->var.ghao_cfg.num_fw_segments;i++)
                    {
                        if(digi_handle3->var.ghao_cfg.ghao_fw_segments[i].state != DIGI_GHAO_FW_DONE)
                        {
                            ramp_done = FALSE;
                        }
                    }
                }


          } 
          else if( new_tick != current_tick)
          {
              /*This error will occur if gettimeofday reutrns a time
              greater than one tick period from the last tick */

              PMC_LOG_TRACE("new_tick : %u \n", new_tick);
              PMC_LOG_TRACE("current_tick : %u \n", current_tick);

              PMC_ASSERT(FALSE, DIGI_ERR_GHAO_TIMER_ERR, 0, 0);
          }


#ifndef PMC_SW_SIMULATION
          /* thru experimentation, this delay is required to achieve an accurate time
          measurement */
          PMC_ATOMIC_YIELD(digi_handle1,500);
#endif

          timeout_time = (DOUBLE)current_tick*(DOUBLE)fw_tick_us;

#ifndef PMC_SW_SIMULATION
          if(timeout_time > (90*2500000))
          {
              PMC_LOG_TRACE(" FIRMWARE TIMEOUT \n");
              ramp_done = TRUE;
          }
#endif

    }

    ret_val = digi_ghao_save_firmware_ctxt(digi_handle1);

    if(ret_val == PMC_SUCCESS)
    {
        if(digi_handle2 != NULL)
        {

            ret_val = digi_ghao_save_firmware_ctxt(digi_handle2);

        }
    }

    if(ret_val == PMC_SUCCESS)
    {
        if(digi_handle3 != NULL)
        {

            ret_val = digi_ghao_save_firmware_ctxt(digi_handle3);

        }
    }

    if(ret_val == PMC_SUCCESS)
    {
        if(digi_handle3 != NULL)
        {

            ret_val = digi_ghao_save_firmware_ctxt(digi_handle3);

        }
    }

    PMC_ATOMIC_RETURN(digi_handle1, ret_val);

} /* digi_ghao_bwr_firmware_execute */




/*
** End of file
*/

