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
*     This file contains functions for the creation and destruction of 
*     SIFD subsystem datapaths.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "sifd_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#define SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, enabled, new_state)\
if (TRUE == enabled) (void)sifd_ilkn_util_core_rx_enable(handle, ilkn_inst, new_state)
                            
#define SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, enabled, new_state) \
    if (TRUE == enabled) (void)sifd_ilkn_util_core_tx_enable(handle, ilkn_inst, new_state)

#define SIFD_ILKN_1_DLOS_EN_MASK_GET(lane) \
    (lane.blk_to_E1_enable_mask | lane.blk_to_N1_enable_mask | lane.blk_to_N2_enable_mask)

#define SIFD_ILKN_2_DLOS_EN_MASK_GET(lane) \
    (lane.blk_to_E1_enable_mask | lane.blk_to_N1_enable_mask | lane.blk_to_N2_enable_mask)

#define SIFD_ILKN_1_DLOS_INV_MASK_GET(lane) \
    (~(lane.blk_to_E1_enable_mask | lane.blk_to_N2_enable_mask) & lane.blk_to_N1_enable_mask)

#define SIFD_ILKN_2_DLOS_INV_MASK_GET(lane) \
    (~(lane.blk_to_E1_enable_mask | lane.blk_to_N2_enable_mask) & lane.blk_to_N1_enable_mask)

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
const char SIFD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    SIFD_LOG_ERR_TABLE 
};  
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char SIFD_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/* Local Functions Prototype*/


PRIVATE PMC_ERROR sifd_int_init(sifd_handle_t    *sifd_handle,
                                BOOL              enable);

PRIVATE PMC_ERROR sifd_audit_var_data_register_get(sifd_handle_t *sifd_handle,
                                                   sifd_var_t *var_ptr);

PRIVATE PMC_ERROR sifd_audit_ctxt_register_compare_clean(sifd_handle_t *cpb_handle,
                                                         sifd_var_t *ctxt_var_ptr,
                                                         sifd_var_t *register_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 *mismatch_tx_fc_cal_ptr,
                                                         BOOL8 *mismatch_rx_fc_cal_ptr,
                                                         BOOL8 *mismatch_loopback_ptr,
                                                         BOOL8 *mismatch_pattern_ptr);

PRIVATE void sifd_serdes_dsis_rx_en_set(sifd_handle_t *sifd_handle, 
                                        UINT32 subsystem_lane,
                                        UINT32 clock_enable);

PRIVATE void sifd_serdes_dsis_rx_sel_set(sifd_handle_t *sifd_handle, 
                                         UINT32 subsystem_lane,
                                         UINT32 internal_rx_lane);

PRIVATE void sifd_dsis_tx_clk_set(sifd_handle_t *sifd_handle, 
                                  UINT32 subsystem_lane,
                                  UINT32 value,
                                  BOOL enable);

PRIVATE sifd_ilkn_test_control_state_t sifd_ilkn_test_control_state_get(sifd_handle_t * sifd_handle,
                                                                        UINT32 ilkn_inst,
                                                                        UINT32 dir);

PRIVATE PMC_ERROR sifd_ilkn_validate_prbs_pattern(sifd_handle_t * sifd_handle, UINT32 prbs_seed);



/*******************************************************************************
** Function Prototypes - SIFD COPI and ILKN Configuration                     **
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn1_lanes_configure(sifd_handle_t *h,
                                             sifd_blk_to_srds_msk_cfg_t bts_cfg,
                                             UINT32 lane_mask);

PRIVATE PMC_ERROR sifd_ilkn2_lanes_configure(sifd_handle_t *h,
                                             sifd_blk_to_srds_msk_cfg_t bts_cfg,
                                             UINT32 lane_mask);

PRIVATE PMC_ERROR sifd_ilkn_lane_remap(sifd_handle_t *h, 
                                       sifd_ilkn_inst_t ilkn_inst,
                                       sifd_ilkn_dir_t dir,
                                       sifd_ilkn_lane_remap_cfg_t map);

PRIVATE PMC_ERROR sifd_init(sifd_handle_t *handle);

PRIVATE PMC_ERROR sifd_intf_enable(sifd_handle_t *handle, 
                                   sifd_ilkn_inst_t ilkn_inst,
                                   sifd_ilkn_en_t enable, 
                                   UINT32 update_msk);

PRIVATE PMC_ERROR sifd_lane_disable(sifd_handle_t *handle, 
                                    sifd_ilkn_inst_t ilkn_inst,
                                    UINT32 disable_msk);

PRIVATE PMC_ERROR sifd_flow_ctrl_init(sifd_handle_t *handle, 
                                      sifd_ilkn_inst_t ilkn_inst,
                                      UINT32 cal_len, 
                                      sifd_fc_mode_t fc_mode);

PRIVATE PMC_ERROR sifd_ilkn_init(sifd_handle_t *handle,
                                 sifd_ilkn_inst_t ilkn_inst,
                                 sifd_ilkn_cfg_t *ilkn);

PRIVATE PMC_ERROR sifd_ilkn_tx_rate_limit_cfg(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              UINT32 rate_limit,
                                              UINT32 burst_limit, 
                                              sifd_oc_burst_t burst_max,
                                              UINT32 byte_gran);

PRIVATE PMC_ERROR sifd_ilkn_util_core_enable(sifd_handle_t *handle, 
                                             sifd_ilkn_inst_t ilkn_inst,
                                             sifd_ilkn_state_t mode);

PRIVATE void sifd_copi_soft_enable_status_get(sifd_handle_t    *sifd_handle, 
                                              sifd_ilkn_inst_t   ilkn_inst,
                                              BOOL8             *rx_enabled, 
                                              BOOL8             *tx_enabled);

PRIVATE PMC_ERROR sifd_ilkn_util_core_rx_enable(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_state_t mode);

PRIVATE PMC_ERROR sifd_ilkn_util_core_tx_enable(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_state_t mode);

PRIVATE PMC_ERROR sifd_ilkn_util_core_cfg(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst,
                                          sifd_oc_burst_t burst_max,
                                          sifd_burst_short_t burst_short,
                                          sifd_burst_min_t burst_min,
                                          sifd_ilkn_mf_t metaframe_period);

PRIVATE PMC_ERROR sifd_ilkn_util_core_tx_rate_limit_cfg(sifd_handle_t *handle, 
                                                        sifd_ilkn_inst_t ilkn_inst,
                                                        UINT32 rate_limit, 
                                                        UINT32 burst_limit,
                                                        sifd_oc_burst_t burst_max,
                                                        UINT32 byte_gran);

PRIVATE PMC_ERROR sifd_ilkn_util_core_lane_disable(sifd_handle_t *handle, 
                                                   sifd_ilkn_inst_t ilkn_inst,
                                                   UINT32 disable_msk);

PRIVATE PMC_ERROR sifd_ilkn_util_core_fc_mode_cfg(sifd_handle_t *handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  sifd_fc_mode_t mode, 
                                                  UINT32 cal_len);

PRIVATE PMC_ERROR sifd_ilkn_util_loopback_set(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              sifd_loopback_cfg_t loop_type,
                                              BOOL fc);

PRIVATE PMC_ERROR sifd_ilkn_util_loopback_get(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              sifd_loopback_cfg_t* loop_type);

PRIVATE PMC_ERROR sifd_ilkn_util_copi_cfg(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst, 
                                          copi_cpb_oduk_cfg_t co_cfg);

PRIVATE PMC_ERROR sifd_ilkn_util_cal_data_write(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_dir_t dir,
                                                UINT32 data);

PRIVATE PMC_ERROR sifd_ilkn_util_cal_ctrl_write(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_dir_t dir,
                                                UINT32 cal_addr);

PRIVATE PMC_ERROR sifd_ilkn_util_cal_write(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst,
                                           sifd_ilkn_dir_t dir,
                                           UINT32 cal_addr, 
                                           UINT32 data);

PRIVATE PMC_ERROR sifd_ilkn_util_cal_read(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst,
                                          sifd_ilkn_dir_t dir,
                                          UINT32 cal_addr, 
                                          UINT32 *data);

/*******************************************************************************
** Function Prototypes - SIFD Interlaken Status registers access              **
*******************************************************************************/
/*******************************************************************************
** Function Prototypes - SIFD MUX Configuration                               **
*******************************************************************************/

/* ILKN1 */
PRIVATE PMC_ERROR sifd_ilkn1_serdes_select(sifd_handle_t *h, 
                                           UINT32 b_lane_mask,
                                           sifd_srds_type_t serdes_type);

PRIVATE void sifd_ilkn1_lane_disable(sifd_handle_t *h, UINT32 b_lane_mask);

PRIVATE PMC_ERROR sifd_ilkn1_serdes_lane_cfg(sifd_handle_t *h, 
                                             UINT32 lane, 
                                             sifd_srds_type_t serdes_type);
 
PRIVATE void sifd_ilkn1_single_lane_disable_set(sifd_handle_t *h, 
                                                UINT32 lane,
                                                UINT32 state);

PRIVATE void sifd_ilkn1_dlos_cfg(sifd_handle_t *h, 
                                 UINT32 b_lane_mask,
                                 UINT32 b_dlos_invert,
                                 UINT32 b_dlos_enable);

PRIVATE void sifd_ilkn1_dlos_lane_cfg(sifd_handle_t *h, 
                                      UINT32 lane, 
                                      UINT32 dlos_invert,
                                      UINT32 dlos_enable);

/* ILKN2 */
PRIVATE PMC_ERROR sifd_ilkn2_serdes_select(sifd_handle_t *h, 
                                           UINT32 b_lane_mask,
                                           sifd_srds_type_t serdes_type);

PRIVATE void sifd_ilkn2_lane_disable(sifd_handle_t *h, UINT32 b_lane_mask);

PRIVATE PMC_ERROR sifd_ilkn2_serdes_lane_cfg(sifd_handle_t *h, 
                                             UINT32 lane, 
                                             sifd_srds_type_t serdes_type);

PRIVATE void sifd_ilkn2_single_lane_disable_set(sifd_handle_t *h, 
                                                UINT32 lane,
                                                UINT32 state);

PRIVATE void sifd_ilkn2_dlos_cfg(sifd_handle_t *h, 
                                 UINT32 b_lane_mask,
                                 UINT32 b_dlos_invert,
                                 UINT32 b_dlos_enable);

PRIVATE void sifd_ilkn2_dlos_lane_cfg(sifd_handle_t *h, 
                                      UINT32 lane, 
                                      UINT32 dlos_invert,
                                      UINT32 dlos_enable);

/* DSIS */
PRIVATE PMC_ERROR sifd_internal_lane_param_check(UINT32 i_lane);

PRIVATE PMC_ERROR sifd_serdes_lane_param_check(UINT32 s_lane);

/* E1 */
PRIVATE void sifd_e1_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value);

/* N1 */
PRIVATE void sifd_n1_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value);

/* N2 */
PRIVATE void sifd_n2_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value);

/* Helper functions */
PRIVATE UINT32 sifd_bits_used_more_than_once_get(sifd_blk_to_srds_msk_cfg_t bts_cfg);
PRIVATE UINT32 sifd_ilkn_msk_invert(sifd_blk_to_srds_msk_cfg_t bts_cfg);

/* check to make sure the values are all above 0 and less that 24*/
PRIVATE PMC_ERROR sifd_validate_lane_remap_within_range(sifd_ilkn_lane_remap_cfg_t *map);
/* check to make sure no two values are the same.*/
PRIVATE PMC_ERROR sifd_validate_lane_remap_contains_no_duplicates(sifd_ilkn_lane_remap_cfg_t map);

PRIVATE PMC_ERROR sifd_serdes_subsystem_set(sifd_handle_t *sifd_handle, 
                                            UINT32 serdes_lane, 
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable);
  
PRIVATE PMC_ERROR sifd_subsystem_lane_enable(sifd_handle_t *sifd_handle, 
                                             UINT32 serdes_lane, 
                                             UINT32 subsystem_lane,
                                             UINT32 clock_enable);

PRIVATE PMC_ERROR sifd_serdes_to_internal_param_check(UINT32 s_lane,
                                                      UINT32 i_lane,
                                                      UINT32* ret_value_ptr);

PRIVATE PMC_ERROR sifd_internal_to_serdes_param_check(UINT32 i_lane,
                                                      UINT32 s_lane,
                                                      UINT32 *ret_value_ptr);

PRIVATE PMC_ERROR sifd_internal_lane_configure(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_rx_lane, 
                                               UINT32 serdes_tx_lane, 
                                               UINT32 internal_rx_lane, 
                                               UINT32 internal_tx_lane);





/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
* sifd_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an SIFD block instance.
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the SIFD subsystem relative to
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - the name of this TSB
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   sifd_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC sifd_handle_t *sifd_ctxt_create(pmc_handle_t *parent, 
                                       UINT32 base_address, 
                                       pmc_sys_handle_t *sys_handle, 
                                       const char *tsb_name)
{
    UINT32 i;
    sifd_handle_t *sifd_handle;

    PMC_ENTRY();

#ifdef USE_PMC_CTXT_CALLOC
        if (parent == NULL) {
            void *c = NULL;
            c = pmc_ctxt_mem_create(sys_handle, FALSE);
            pmc_ctxt_mem_init(c);
            sifd_handle = (sifd_handle_t*)(((pmc_ctxt_mgmt_ctxt_t*)c)->header.digi_hdl);
            sifd_handle->base.ctxt = c;
            sifd_handle = (sifd_handle_t*)PMC_CTXT_CALLOC(sizeof(sifd_handle_t), sifd_handle);
        } else {
            sifd_handle = (sifd_handle_t*)PMC_CTXT_CALLOC(sizeof(sifd_handle_t), parent);
        };
#else
        sifd_handle = (sifd_handle_t*)PMC_CTXT_CALLOC(sizeof(sifd_handle_t), parent);
#endif

    pmc_handle_init(parent, sifd_handle, sys_handle, PMC_MID_DIGI_SIFD, 
                    tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(SIFD_LOG_ERR_STRINGS[0],
                                   SIFD_LOG_ERR_TABLE_BASE, 
                                   SIFD_LOG_ERR_COUNT);


#ifdef USE_PMC_CTXT_CALLOC
    sifd_handle->var.test_control_state = (sifd_ilkn_test_control_state_t**)PMC_CTXT_CALLOC(sizeof(sifd_ilkn_test_control_state_t*)*SIFD_MAX_ILKN_INTERFACE, sifd_handle);
    PMC_CTXT_REGISTER_PTR(&sifd_handle->var.test_control_state, sifd_handle);
#else
    sifd_handle->var.test_control_state = (sifd_ilkn_test_control_state_t**)PMC_CTXT_CALLOC(sizeof(sifd_ilkn_test_control_state_t*)*SIFD_MAX_ILKN_INTERFACE, parent);
    PMC_CTXT_REGISTER_PTR(&sifd_handle->var.test_control_state, parent);
#endif

    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
#ifdef USE_PMC_CTXT_CALLOC
        sifd_handle->var.test_control_state[i] = (sifd_ilkn_test_control_state_t*)PMC_CTXT_CALLOC(sizeof(sifd_ilkn_test_control_state_t)*SIFD_NUM_ILKN_DIR_MAX, sifd_handle);
        PMC_CTXT_REGISTER_PTR(&sifd_handle->var.test_control_state[i], sifd_handle);
#else
        sifd_handle->var.test_control_state[i] = (sifd_ilkn_test_control_state_t*)PMC_CTXT_CALLOC(sizeof(sifd_ilkn_test_control_state_t)*SIFD_NUM_ILKN_DIR_MAX, parent);
        PMC_CTXT_REGISTER_PTR(&sifd_handle->var.test_control_state[i], parent);
#endif

                    
    }

    PMC_RETURN(sifd_handle);
} /* sifd_ctxt_create */


/*******************************************************************************
* sifd_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an SIFD block instance.
*
*
* INPUTS:
*   *sifd_handle           - pointer to SIFD handle instance
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
PUBLIC void sifd_ctxt_destroy(sifd_handle_t *sifd_handle)
{
    UINT32 i;

    PMC_ENTRY();
    pmc_handle_mid_verify(sifd_handle, PMC_MID_DIGI_SIFD);
    sifd_handle->base.tsb_mid = PMC_MID_INVALID;

    
    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
        PMC_CTXT_FREE(&sifd_handle->var.test_control_state[i], sifd_handle);
    }

    PMC_CTXT_FREE(&sifd_handle->var.test_control_state, sifd_handle);

    PMC_CTXT_FREE(&sifd_handle, sifd_handle);

    PMC_RETURN();
} /* sifd_ctxt_destroy */

/*******************************************************************************
* sifd_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an SIFD block handle instance.
*
*
* INPUTS:
*   *sifd_handle           - pointer to SIFD handle instance
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
PUBLIC void sifd_handle_init(sifd_handle_t *sifd_handle)
{
    UINT32 i = 0;
    UINT32 j = 0;
    
    PMC_ENTRY();


   /*
    * Initialize the internal lane array.
    */
   for (i = 0; i < SIFD_MAX_SERDES_LANES ; ++i) {
       sifd_internal_lane_configure(sifd_handle, i, i, i, i);
       sifd_handle->var.los_ctxt[i].active = FALSE;
   }

    sifd_handle->var.ilkn_state[SIFD_COPI_ILKN1] = PMC_BLOCK_DEFAULT;     /* default to no power */
    sifd_handle->var.ilkn_state[SIFD_COPI_ILKN2] = PMC_BLOCK_DEFAULT;     /* default to no power */

    /* Initialize flow control */
    for (i = 0; i < 2; ++i) {

        sifd_handle->var.ilkn_fc[i].flow_control_mode = SIFD_FC_MODE_NONE;
        sifd_handle->var.ilkn_fc[i].calendar_length = 0;        
        sifd_handle->var.ilkn_fc[i].tx_link_flow_control = FALSE;
        sifd_handle->var.ilkn_fc[i].tx_channel_flow_control = FALSE;
        sifd_handle->var.ilkn_fc[i].tx_oduk_channel_flow_control = FALSE;
        sifd_handle->var.ilkn_fc[i].rx_link_flow_control = FALSE;
        sifd_handle->var.ilkn_fc[i].rx_channel_flow_control = FALSE;
    

        for (j = 0; j < SIFD_ILKN_MAX_CAL_ENTRIES; ++j) 
        {
            sifd_handle->var.ilkn_fc[i].tx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
            sifd_handle->var.ilkn_fc[i].rx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
        }
    }
    /* track loopback state */ 
    sifd_handle->var.loopback[SIFD_COPI_ILKN1] = SIFD_DISABLE_LOOPBACK;
    sifd_handle->var.loopback[SIFD_COPI_ILKN2] = SIFD_DISABLE_LOOPBACK;

    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
        for(j=0;j<2;j++) /* Direction */
        {
            sifd_handle->var.test_control_state[i][j] = sifd_ilkn_test_control_state_get(sifd_handle, i, j);
        }
    }

    PMC_RETURN();
} /* sifd_handle_init */

/*******************************************************************************
* sifd_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Allocated the memory pools of the underlying sub systems if 
*  applicable.
*
*
* INPUTS:
*   *sifd_handle                - pointer to sifd handle instance
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
PUBLIC void sifd_alloc_init(sifd_handle_t *sifd_handle)
{
    PMC_ENTRY();

    PMC_RETURN();
}

/*******************************************************************************
* sifd_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Performs warm or crash restart processing on SIFD SS handle instance.
*
*
* INPUTS:
*   *sifd_handle                - pointer to sifd handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   energy_state                - energy state of the block relative to parent.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_handle_restart_init(sifd_handle_t *sifd_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t energy_state)
{

    PMC_ERROR result = PMC_SUCCESS;
    sifd_var_t * register_var = NULL; 

    BOOL8 mismatch_tx_fc_cal    = TRUE;
    BOOL8 mismatch_rx_fc_cal    = TRUE;
    BOOL8 mismatch_loopback   = TRUE;
    BOOL8 mismatch_pattern    = TRUE;
    UINT32 i;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(SIFD_LOG_ERR_STRINGS[0], SIFD_LOG_ERR_TABLE_BASE, SIFD_LOG_ERR_COUNT);
    }

    /* memory allocation for register based context generation */
    register_var = (sifd_var_t*)PMC_CALLOC(sizeof(sifd_var_t));
    PMC_ASSERT(register_var != NULL, SIFD_ERR_CODE_ASSERT, 0 , 0);


    /* memory allocation for test patterm parameters */
    register_var->test_control_state = (sifd_ilkn_test_control_state_t**)PMC_CALLOC(sizeof(sifd_ilkn_test_control_state_t*)*SIFD_MAX_ILKN_INTERFACE);
    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
        register_var->test_control_state[i] = (sifd_ilkn_test_control_state_t*)PMC_CALLOC(sizeof(sifd_ilkn_test_control_state_t)*SIFD_NUM_ILKN_DIR_MAX);

    }      

    if(restart_init_cfg_ptr->is_warm_restart && energy_state == PMC_ENERGY_STATE_RESET)
    {
    }
    else if (restart_init_cfg_ptr->is_warm_restart == FALSE && energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* If we are in crash restart and the energy state is reset, simply
         * re-initialize the context to default and exit */
        sifd_init(sifd_handle);
    }
    else
    {
        /* Recover var data from register space. */
        if(result==PMC_SUCCESS)
        {
            result = sifd_audit_var_data_register_get(sifd_handle, register_var);
        }

        if(result==PMC_SUCCESS)
        {
            /* Compare Context with Register Data for Coherency 
             * If crash restart, modify the context and register 
             * data so only the union is kept */
            result = sifd_audit_ctxt_register_compare_clean(sifd_handle, 
                                                      &(sifd_handle->var), 
                                                      register_var,
                                                      restart_init_cfg_ptr,
                                                      &mismatch_tx_fc_cal,
                                                      &mismatch_rx_fc_cal,
                                                      &mismatch_loopback,
                                                      &mismatch_pattern);
        }
 
       
        if(result==PMC_SUCCESS)
            if(mismatch_tx_fc_cal == TRUE ||
               mismatch_rx_fc_cal == TRUE ||
               mismatch_loopback == TRUE ||
               mismatch_pattern == TRUE)
            { 
                PMC_LOG_TRACE("sifd crash restart - Mismatch: mismatch_tx_fc_cal: %u, mismatch_rx_fc_cal: %u, mismatch_loopback: %u, mismatch_pattern: %u\n",
                mismatch_tx_fc_cal,
                mismatch_rx_fc_cal,
                mismatch_loopback,
                mismatch_pattern);
                /* On mismatch, only report failure in warm restart */
                if(restart_init_cfg_ptr->is_warm_restart)
                    result = SIFD_LOG_ERR_CONTEXT_INCOHERENT; 
            }
    }
    
    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
        
       PMC_FREE(&(register_var->test_control_state[i]));
    }
    PMC_FREE(&(register_var->test_control_state));
    PMC_FREE(&(register_var));

    


    PMC_RETURN(result);
} /* sifd_handle_restart_init */



/*******************************************************************************
* sifd_mux_data_path_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Configures the SIFD subsystem at a block level:\n
*       - SERDES Mux\n
*       - COPI\n
*       - Interlaken interfaces\n\n
* 
*   After the initial configuration, the various RX & TX channels can be configured
*   by calling sifd_ilkn_rx_chnl_cfg & sifd_ilkn_tx_chnl_cfg.\n\n
*
*   Note that all of these configurations are static and cannot be changed.
*   Once all the configurations are completed, the Interlaken interface can be
*   enabled by calling sifd_ilkn_enable.\n\n
*
*   The only configuration that can be changed after the enable is the flow control
*   calendars (RX & TX) by calling sifd_chnl_fc_map.
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   *sifd_1_cfg_ptr       - pointer to Interlaken 1 configuration
*   *sifd_2_cfg_ptr       - pointer to Interlaken 2 configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_mux_data_path_cfg(sifd_handle_t *h,
                                        sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                        sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr) 
{
    PMC_ERROR rc = SIFD_LOG_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);


    if (sifd_1_cfg_ptr != NULL) {
        
        h->var.ilkn_state[SIFD_COPI_ILKN1] = PMC_BLOCK_ALTERABLE; /* enable power to the block */

        /* configure SIFD Mux for SERDES lane allocation & DLOS handling */
        rc = sifd_ilkn1_lanes_configure(h, sifd_1_cfg_ptr->lane,
                                        sifd_1_cfg_ptr->ilkn.lane_mask);
        if (rc != PMC_SUCCESS)
            PMC_RETURN(rc);
    }

    if (sifd_2_cfg_ptr != NULL) {
        
        h->var.ilkn_state[SIFD_COPI_ILKN2] = PMC_BLOCK_ALTERABLE; /* enable power to the block */

        /* configure SIFD Mux for SERDES lane allocation & DLOS handling */
        rc = sifd_ilkn2_lanes_configure(h, sifd_2_cfg_ptr->lane,
                                        sifd_2_cfg_ptr->ilkn.lane_mask );
        if (rc != PMC_SUCCESS)
            PMC_RETURN(rc);
    }

    /* initialize SIFD - apply power & take out of reset */
    rc = sifd_init(h);
    if (rc != PMC_SUCCESS)
        PMC_RETURN(rc);

    if (sifd_1_cfg_ptr != NULL) {

        /* configure COPI & Interlaken interfaces */
        rc = sifd_ilkn_init(h, SIFD_COPI_ILKN1, &sifd_1_cfg_ptr->ilkn);
        if (rc != PMC_SUCCESS)
            PMC_RETURN(rc);
    }

    if (sifd_2_cfg_ptr != NULL) {

        /* configure COPI & Interlaken interfaces */
        rc = sifd_ilkn_init(h, SIFD_COPI_ILKN2, &sifd_2_cfg_ptr->ilkn);
        if (rc != PMC_SUCCESS)
            PMC_RETURN(rc);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_mux_data_path_cfg */




/*******************************************************************************
* sifd_ilkn_rx_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Configures the static configuration of channel mapping and ODUK header values
*   for Interlaken RX traffic.\n\n
* 
*   This configuration can only be called after:\n
*       sifd_mux_data_path_cfg\n\n
* 
*   This configuration is static and must be called before:\n
*       sifd_ilkn_enable\n\n
* 
*   The Interlaken Burst Control Word channel ID's must be mapped to either CPB or
*   ODUK Switch before any traffic flows over the Interlaken interface.  This also
*   includes configuring the CPB/ODUK DPI channel ID.  In the case where the traffic
*   is ODUK Mode 0, the 2B header table entry (for replacing the DPI channel ID) must
*   also be configured.  There is no error checking for two 2B header table entries
*   pointing to the same DPI channel ID - this is considered an invalid configuration.\n\n
* 
*   If the traffic is ODUK and 4B/2B header comparison is used (as configured during
*   initialization), then the 4B/2B header table entries must alo be configured.\n\n
* 
*   The default configuration for Interlaken Burst Control Word channel ID's is to
*   route traffic to channel 255 of the CPB (which is a "bit bucket" that discards
*   RX traffic).  This provides protection of stray traffic on unconfigured channels.\n\n
* 
*   Note that all Interlaken Burst Control Word channel ID's that are intended to
*   support user traffic must be configured before the Interlaken interface is enabled.
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   ilkn_inst             - the specific ILKN instance.\n
*                             SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                             SIFD_COPI_ILKN2 Interlaken 2 Instance
*   bcw_chnl_id           - Interlaken RX Burst Control Word channel ID
*   dest                  - destination subsystem for receive traffic where:\n
*                             TRUE (1) targets ODUK Switch path\n
*                             FALSE (0) targets CPB path
*   dpi_chnl_id           - DPI channel ID for CPB or ODUK Switch path which
*                           can be one of:\n
*                             0 to 149 for CPB traffic\n
*                             0 to 103 for ODUK Switch traffic
*   oduk_4b_header        - 4B header that is compared to RX ODUK traffic if
*                           this capability is configured during initialization.
*                           The index for this table is dpi_chnl_id.
*                           Set to 0 if unused.
*   oduk_2b_header        - 2B header that is either compared to RX ODUK traffic
*                           or used to replace the DPI channel ID.  This
*                           capability is also configured during initialization.
*                           The index for this table is dpi_chnl_id.
*                           Set to 0 if unused.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
* 
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_rx_chnl_cfg(sifd_handle_t *h,
                                       sifd_ilkn_inst_t ilkn_inst,
                                       UINT8 bcw_chnl_id,
                                       BOOL dest,
                                       UINT8 dpi_chnl_id,
                                       UINT32 oduk_4b_header,
                                       UINT16 oduk_2b_header)
{
    UINT32 dest_value;

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (bcw_chnl_id >= SIFD_MAX_ARB_CHAN) ||
        (dpi_chnl_id >= SIFD_MAX_CPB_ARB_CHAN) ||
        (dest && (dpi_chnl_id >= SIFD_MAX_ODUK_ARB_CHAN))) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, bcw_chnl_id, dpi_chnl_id);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Interlaken interface must be configured! */
    PMC_ASSERT(PMC_BLOCK_DEFAULT != h->var.ilkn_state[ilkn_inst], SIFD_ERR_CODE_ASSERT, 0, 0);

    if (dest) {                                  /* ODUK */
        dest_value = 1;

        copi_field_RX_ODUK_4B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id, oduk_4b_header);

        copi_field_RX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id, oduk_2b_header);
    }
    else                                         /* CPB */
        dest_value = 0;

    copi_field_RX_CPB_ODUK_PATH_SEL_set(NULL, h, ilkn_inst, bcw_chnl_id, dest_value);

    copi_field_RX_ARB_CH_set(NULL, h, ilkn_inst, bcw_chnl_id, dpi_chnl_id);


    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_rx_chnl_cfg */

/*******************************************************************************
* sifd_ilkn_chnl_2b_header_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update the configuration of the RX or Tx 2B header.\n
*   Note that it is only allowed to update 2b header under the following 
*   condition:\n
*    - RX_ODUK_2B_MODE shall be set to 0 and RX_ODUK_6B_CHK shall be set to 1
*       (see HWS sect. 5.11.4.5.7 CBRI (ODUkSW) Packet Format over the
*        Interlaken Interface)\n
*       -- THIS REQUIREMENT is NOT CHECKED BY THIS FUNCTION, it assumes 
*           upper-level is enforcing it.\n
*    - at least 1 bit should remain static (0 or 1) from the previous and new
*      2B header value (see HWS  5.11.4.5.7 for details)\n
*       -- THIS REQUIREMENT is CHECKED by THIS FUNCTION
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   ilkn_inst             - the specific ILKN instance.\n
*                             SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                             SIFD_COPI_ILKN2 Interlaken 2 Instance
*   dpi_chnl_id           - DPI channel ID for CPB or ODUK Switch path which
*                           can be one of:\n
*                             0 to 149 for CPB traffic\n
*                             0 to 103 for ODUK Switch traffic
*   update_rx              - TRUE: update RX 2b header\n
*                            FALSE: update TX 2b header
*   oduk_cur_2b_header     - Current 2B header (16 bits)
*   oduk_new_2b_header     - New 2B header (16 bits)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_chnl_2b_header_update(sifd_handle_t *h,
                                                 sifd_ilkn_inst_t ilkn_inst,
                                                 UINT8 dpi_chnl_id,
                                                 BOOL   update_rx,
                                                 UINT16 oduk_cur_2b_header,
                                                 UINT16 oduk_new_2b_header)
{
    UINT16 update_mask = 0;
    UINT32 itr;
    PMC_ENTRY();

    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        dpi_chnl_id >= SIFD_MAX_ODUK_ARB_CHAN)
    {
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }
    update_mask = oduk_cur_2b_header ^ oduk_new_2b_header;
    if ( update_mask == 0xFFFF)
    {
        PMC_RETURN(SIFD_LOG_ERR_2B_HEADER_UPDATE_INVALID);
    }  else 
    {
        /* find a static bit */            
        for (itr = 0; itr < 16; itr+=1)
        {
            if ( ( (update_mask >> itr) & 0x1 ) == 0)
            {
                update_mask  = 0x1 << itr;
                break;
            }             
        }
    }
               
    /* Interlaken interface must be configured! */
    PMC_ASSERT(PMC_BLOCK_DEFAULT != h->var.ilkn_state[ilkn_inst], SIFD_ERR_CODE_ASSERT, 0, 0);

    if (TRUE == update_rx)
    {        
        /* Update 2B lookup value from [ORIGINAL] to [ORIGINAL with "disable bit" set] */        
        copi_field_RX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_cur_2b_header ^ update_mask);        
        /* Update 2B lookup value from [ORIGINAL with "disable bit" set] to [NEW with "disable bit set"] */
        copi_field_RX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_new_2b_header ^ update_mask);        
        /* Update 2B lookup value from [NEW with "disable bit" set] to [NEW with "disable bit" clear] */
        copi_field_RX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_new_2b_header);                
    } else 
    {
        /* Update 2B lookup value from [ORIGINAL] to [ORIGINAL with "disable bit" set] */        
        copi_field_TX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_cur_2b_header ^ update_mask);        
        /* Update 2B lookup value from [ORIGINAL with "disable bit" set] to [NEW with "disable bit set"] */
        copi_field_TX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_new_2b_header ^ update_mask);        
        /* Update 2B lookup value from [NEW with "disable bit" set] to [NEW with "disable bit" clear] */
        copi_field_TX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id,  oduk_new_2b_header);        
    }
       

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_chnl_2b_header_update */

/*******************************************************************************
* sifd_ilkn_tx_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Configures the static configuration of channel mapping and ODUK header values
*   for Interlaken TX traffic.\n\n
* 
*   This configuration can only be called after:\n
*       sifd_mux_data_path_cfg\n\n
* 
*   This configuration is static and must be called before:\n
*       sifd_ilkn_enable\n\n
* 
*   The CPB/ODUK DPI channel ID must be mapped to a Interlaken Burst Control Word
*   channel ID before any traffic flows over the Interlaken interface.\n\n
*
*   If the traffic is ODUK and 4B/2B header insertion is used (as configured during
*   initialization), then the 4B/2B header table entries must alo be configured.\n\n
* 
*   Note that all CPB/ODUK channel ID's that are intended to support user traffic
*   over the Interlaken interface must be configured before the Interlaken interface
*   is enabled.
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   ilkn_inst             - the specific ILKN instance.\n
*                             SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                             SIFD_COPI_ILKN2 Interlaken 2 Instance
*   dpi_chnl_id           - DPI channel ID for CPB or ODUK Switch path traffic
*                           which can be one of:\n
*                             0 to 149 for traffic from CPB\n
*                             0 to 103 for traffic from ODUK Switch
*   source                - source subsystem for transmit traffic where:\n
*                             TRUE (1) originates on ODUK Switch path\n
*                             FALSE (0) orginates on CPB path
*   bcw_chnl_id           - Interlaken TX Burst Control Word channel ID
*   oduk_4b_header        - 4B header that is inserted in TX ODUK traffic if
*                           this capability is configured during initialization.
*                           The index for this table is ODUK dpi_chnl_id.
*                           Set to 0 if unused.
*   oduk_2b_header        - 2B header that is inserted in TX ODUK traffic if
*                           this capability is configured during initialization.
*                           The index for this table is ODUK dpi_chnl_id.
*                           Set to 0 if unused.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
* 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_tx_chnl_cfg(sifd_handle_t *h,
                                       sifd_ilkn_inst_t ilkn_inst,
                                       BOOL source,
                                       UINT8 dpi_chnl_id,
                                       UINT8 bcw_chnl_id,
                                       UINT32 oduk_4b_header,
                                       UINT16 oduk_2b_header)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (bcw_chnl_id >= SIFD_MAX_ARB_CHAN) ||
        (dpi_chnl_id >= SIFD_MAX_CPB_ARB_CHAN) ||
        (source && (dpi_chnl_id >= SIFD_MAX_ODUK_ARB_CHAN))) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, bcw_chnl_id, dpi_chnl_id);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Interlaken interface must be configured! */
    PMC_ASSERT(PMC_BLOCK_DEFAULT != h->var.ilkn_state[ilkn_inst], SIFD_ERR_CODE_ASSERT, 0, 0);

    if (source) {                                /* ODUK? */

        copi_field_TX_ODUK_ARB_CH_set(NULL, h, ilkn_inst, dpi_chnl_id, bcw_chnl_id);

        copi_field_TX_ODUK_4B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id, oduk_4b_header);

        copi_field_TX_ODUK_2B_LK_set(NULL, h, ilkn_inst, dpi_chnl_id, oduk_2b_header);
    }
    else {                                       /* CPB */

        copi_field_TX_CPB_ARB_CH_set(NULL, h, ilkn_inst, dpi_chnl_id, bcw_chnl_id);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_tx_chnl_cfg */




/*******************************************************************************
* FUNCTION: sifd_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   sifd_handle       - Pointer to subsystem handle
*   ilkn_inst         - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*   non_null_ptr      - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*    None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_int_validate(sifd_handle_t    *sifd_handle,
                                   sifd_ilkn_inst_t  ilkn_inst,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sifd_handle, SIFD_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (ilkn_inst > SIFD_COPI_ILKN2)
    {
        result = SIFD_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = SIFD_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* sifd_int_validate */

/*******************************************************************************
* FUNCTION: sifd_int_link_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   link interrupt table.
*   
*
* INPUTS:
*   sifd_handle       - Pointer to subsystem handle
*   ilkn_inst         - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*   int_table_ptr     - pointer to link interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   None
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_int_link_enable(sifd_handle_t    *sifd_handle,
                                      sifd_ilkn_inst_t  ilkn_inst,
                                      sifd_int_link_t  *int_table_ptr,
                                      sifd_int_link_t  *int_en_table_ptr,
                                      BOOL              enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = ilkn_inst;
    UINT32          link1 = ilkn_inst;
    UINT32          link2 = 0;
    UINT32          i;
    UINT32          write_value;

    PMC_ENTRY();

    result = sifd_int_validate(sifd_handle, ilkn_inst, int_en_table_ptr);

    
    #ifdef SIFD_INT_RANGE
    #undef SIFD_INT_RANGE
    #endif
    #define SIFD_INT_RANGE  UTIL_GLOBAL_INT_SIFD_RANGE_ENABLE

    #ifdef SIFD_INT_ARRAY
    #undef SIFD_INT_ARRAY
    #endif
    #define SIFD_INT_ARRAY  SIFD_COPI_INT0_ENABLE_ARRAY

    #ifdef SIFD_INT
    #undef SIFD_INT
    #endif
    #define SIFD_INT    UTIL_GLOBAL_INT_SIFD_ENABLE

    SIFD_INT_TABLE_DEFINE();

    /*write value is 24bits, need to handle separately*/
    if ((NULL == int_table_ptr) || (FALSE != int_table_ptr->ser_rx_los_int_i0)) 
    {
        if (TRUE == enable)
        {
            write_value = 0xFFFFFF;
        }
        else
        {
            write_value = 0;
        }
        copi_field_SER_RX_LOS_INT_E0_set(NULL, sifd_handle, link1, write_value);  
        int_en_table_ptr->ser_rx_los_int_i0 = enable;                              
    }        

    /*write value is 2bits, need to handle separately*/
    if ((NULL == int_table_ptr) || (FALSE != int_table_ptr->rx_il_dat_err_int_i0)) 
    {
        if (TRUE == enable)
        {
            write_value = 0x3;
        }
        else
        {
            write_value = 0;
        }
        copi_field_RX_IL_DAT_ERR_INT_E0_set(NULL, sifd_handle, link1, write_value);  
        int_en_table_ptr->rx_il_dat_err_int_i0 = enable;                              
    }                                                                           

    /*
     * If enable is TRUE copy over the passed in interrupt table to ensure
     * read-only register such as RX_ALIGN_GOOD are reflected in the table.
     * If enable is FALSE then reset the interrupt table to again ensure
     * READ-ONLY registers are reset as expected.
     */
    if (PMC_SUCCESS == result && TRUE == enable)
    {
        PMC_MEMCPY(int_en_table_ptr, int_table_ptr, sizeof(sifd_int_link_t));
    }
    else if (PMC_SUCCESS == result && FALSE == enable)
    {
        PMC_MEMSET(int_en_table_ptr, 0, sizeof(sifd_int_link_t));
    }

    PMC_RETURN(result);
} /* sifd_int_link_enable */



/*******************************************************************************
* FUNCTION: sifd_int_link_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   link interrupt table.
*   
*
* INPUTS:
*   sifd_handle       - Pointer to subsystem handle
*   ilkn_inst         - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*   int_table_ptr     - pointer to link interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*    None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_int_link_clear(sifd_handle_t    *sifd_handle,
                                     sifd_ilkn_inst_t  ilkn_inst,
                                     sifd_int_link_t  *int_table_ptr,
                                     sifd_int_link_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = ilkn_inst;
    UINT32          link1 = ilkn_inst;
    UINT32          link2 = 0;
    UINT32          i;

    PMC_ENTRY();

    result = sifd_int_validate(sifd_handle, ilkn_inst, int_en_table_ptr);

    #ifdef SIFD_INT_RANGE
    #undef SIFD_INT_RANGE
    #endif
    #define SIFD_INT_RANGE      UTIL_GLOBAL_INT_SIFD_RANGE_CLEAR
    
    #ifdef SIFD_INT_ARRAY
    #undef SIFD_INT_ARRAY
    #endif
    #define SIFD_INT_ARRAY  SIFD_COPI_INT0_CLEAR_ARRAY
    
    #ifdef SIFD_INT
    #undef SIFD_INT
    #endif
    #define SIFD_INT    UTIL_GLOBAL_INT_SIFD_CLEAR



    if ( (NULL == int_table_ptr) || 
         (int_table_ptr->mf_err_i == TRUE))
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_METAFRAME_ERR,
                       ILAKEN_REG_RX_METAFRAME_ERR_BIT_RX_MF_ERR_MSK);
    }
    if ( (NULL == int_table_ptr) || 
         (int_table_ptr->crc32_err_i == TRUE))
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_CRC32_ERR,
                       ILAKEN_REG_RX_CRC32_ERR_BIT_RX_CRC32_ERR_MSK);
    }    
    if (NULL == int_table_ptr ||  int_table_ptr->badctl_err_i == TRUE)
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_BAD_CTRL_WORD_ERR,
                       ILAKEN_REG_RX_BAD_CTRL_WORD_ERR_BIT_RX_BADCTL_ERR_MSK);
    }
    if (NULL == int_table_ptr ||  int_table_ptr->scram_err_i == TRUE)
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_SCRAMBLER_STATE_ERR,
                       ILAKEN_REG_RX_SCRAMBLER_STATE_ERR_BIT_RX_SCRAM_ERR_MSK);
    }
    if (NULL == int_table_ptr || int_table_ptr->cdr_err_i == TRUE)
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_CDR_ERR,
                       ILAKEN_REG_RX_CDR_ERR_BIT_RX_CDR_ERR_MSK);
    }
    if (NULL == int_table_ptr || int_table_ptr->wrd_sync_change_i == TRUE)
    {
        ilkn_reg_write(NULL,sifd_handle,(UINT32) ilkn_inst,MEM_TYPE_STATUS,
                       PMC_ILAKEN_REG_RX_WORD_SYNC_CHANGED,
                       ILAKEN_REG_RX_WORD_SYNC_CHANGED_BIT_RX_WRD_SYNC_CHG_MSK);
    }

    SIFD_INT_TABLE_DEFINE();
    
    if ((NULL == int_table_ptr) ||
        (FALSE != int_table_ptr->ser_rx_los_int_i0)) 
    {
        copi_field_SER_RX_LOS_INT_I0_set_to_clear(NULL, sifd_handle, link1, 0xFFFFFF);
    }
    
    if ((NULL == int_table_ptr) ||                                              
        (FALSE != int_table_ptr->rx_il_dat_err_int_i0)) 
    {                                
        copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear(NULL, sifd_handle, link1, 0x3); 
    }                                                                           

    PMC_RETURN(result);
} /* sifd_int_link_clear */

/*******************************************************************************
* FUNCTION: sifd_int_link_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the link interrupt table.
*   
*
* INPUTS:
*   sifd_handle       - Pointer to subsystem handle
*   ilkn_inst         - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to link interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_int_link_retrieve(sifd_handle_t    *sifd_handle,
                                        sifd_ilkn_inst_t  ilkn_inst,
                                        sifd_int_link_t  *filt_table_ptr,
                                        sifd_int_link_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link  = ilkn_inst;
    UINT32          link1 = ilkn_inst;
    UINT32          link2 = 0;
    UINT32          i;
    UINT32          read_value;
    sifd_int_link_t  *tmp_filter_ptr = filt_table_ptr;

    PMC_ENTRY();

    result = sifd_int_validate(sifd_handle, ilkn_inst, filt_table_ptr);

    
    UTIL_GLOBAL_INT_BUFFER_INIT(sifd_handle, ilkn, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(sifd_handle, copi, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef SIFD_INT_RANGE
    #undef SIFD_INT_RANGE
    #endif
    #define SIFD_INT_RANGE UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    #ifdef SIFD_INT_ARRAY
    #undef SIFD_INT_ARRAY
    #endif
    #define SIFD_INT_ARRAY  SIFD_COPI_INT0_RETRIEVE_ARRAY

    #ifdef SIFD_INT
    #undef SIFD_INT
    #endif
    #define SIFD_INT    UTIL_GLOBAL_INT_SIFD_RETRIEVE


    SIFD_INT_TABLE_DEFINE();
   
    if ((NULL == filt_table_ptr) ||                                             
        (FALSE != filt_table_ptr->ser_rx_los_int_i0)) 
    {
        read_value = copi_field_SER_RX_LOS_INT_I0_get(copi_buffer_ptr, sifd_handle, link1);
        if(0 == read_value)
        {
            int_table_ptr->ser_rx_los_int_i0 = FALSE;
        }
        else
        {
            int_table_ptr->ser_rx_los_int_i0 = TRUE;
        }
    }  
                                                                           
    if ((NULL == filt_table_ptr) ||                                             
        (FALSE != filt_table_ptr->rx_il_dat_err_int_i0)) 
    {
        read_value = copi_field_RX_IL_DAT_ERR_INT_I0_get(copi_buffer_ptr, sifd_handle, link1);
        if(0 == read_value)
        {
            int_table_ptr->rx_il_dat_err_int_i0 = FALSE;
        }
        else
        {
            int_table_ptr->rx_il_dat_err_int_i0 = TRUE;
        }
    }                                                                           

    #ifdef SIFD_STATUS
    #undef SIFD_STATUS
    #endif
    #define SIFD_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    

    SIFD_STATUS_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(sifd_handle, ilkn);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(sifd_handle, copi);

    filt_table_ptr = tmp_filter_ptr;
    
    PMC_RETURN(result);
} /* sifd_int_link_retrieve */


/*******************************************************************************
* FUNCTION: sifd_int_link_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   sifd_handle       - Pointer to subsystem handle
*   ilkn_inst         - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_int_link_enabled_check(sifd_handle_t     *sifd_handle,
                                             sifd_ilkn_inst_t   ilkn_inst,
                                             sifd_int_link_t   *int_en_table_ptr,
                                             BOOL              *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = ilkn_inst;
    UINT32          link1 = ilkn_inst;
    UINT32          link2 = 0;
    UINT32          i;
    UINT32          read_value;

    PMC_ENTRY();

    result = sifd_int_validate(sifd_handle, ilkn_inst, int_en_table_ptr);


    
    #ifdef SIFD_INT_RANGE
    #undef SIFD_INT_RANGE
    #endif
    #define SIFD_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
    
    #ifdef SIFD_INT_ARRAY
    #undef SIFD_INT_ARRAY
    #endif
    #define SIFD_INT_ARRAY  SIFD_COPI_INT0_CHECK_ARRAY

    #ifdef SIFD_INT
    #undef SIFD_INT
    #endif
    #define SIFD_INT    UTIL_GLOBAL_INT_SIFD_CHECK

    SIFD_INT_TABLE_DEFINE();

    if ((FALSE == *int_found_ptr) &&                                            
        (FALSE != int_en_table_ptr->ser_rx_los_int_i0)) 
    {
        read_value = copi_field_SER_RX_LOS_INT_I0_get(NULL, sifd_handle, link1);
        if(0 == read_value)
        {
            *int_found_ptr = FALSE;
        }
        else
        {
            *int_found_ptr = TRUE;
        }
    }
    
    if ((FALSE == *int_found_ptr) &&                                           
        (FALSE != int_en_table_ptr->rx_il_dat_err_int_i0)) 
    {                             
        read_value = copi_field_RX_IL_DAT_ERR_INT_I0_get(NULL, sifd_handle, link1);
        if(0 == read_value)
        {
            *int_found_ptr = FALSE;
        }
        else
        {
            *int_found_ptr = TRUE;
        }
    }                                                                           

    PMC_RETURN(result);
} /* sifd_int_link_enabled_check */




/* ILKN1 */

/*******************************************************************************
* sifd_ilkn1_lanes_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes signal
*   sources associated with the ILKN 1 specified lane.\n\n
*   
*   If the mask bit is set in the bts_cfg, the lane is enabled. If the bit is
*   not configured to be used by any SERDES, it disabled.\n\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must set all lanes to be used by calling this function
*   with the various bts_cfg masks set.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*
*   bts_cfg               - A structure consisting of three bit masks.
*                           Each corresponding to a separate SERDES.\n
*                            
*                           blk_to_E1_enable_mask: Denotes the lane number this
*                           block to enable to the E1 SERDES.\n
*                           blk_to_N1_enable_mask: The lane number from this
*                           block to enable to the N1 SERDES.\n
*                           blk_to_N2_enable_mask: The lane number from this 
*                           block to enable to the N2 SERDES.\n
*
*                           The LSB representing ILKN1 Lane 0, the MSB
*                           represents ILKN1 Lane 23. Bits higher than the 
*                           23rd are unused.\n
* 
*                           It should be noted that only one lane may be 
*                           targeted towards one SERDES.\n
*
*                           See note for which bits in the bit mask are 
*                           valid. 
*
*   lane_mask             - bit mask of ILKN1 SERDES lanes where LSB is lane 0
*                           (1 for used & 0 for unused)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken1 may connect to are:\n
* ______________________________________________________________________________\n
*
*   The E1 Serdes, Lanes[39:20]\n
*   The N1 Serdes, Lanes[15:00]\n
*   The N2 Serdes, Lanes[07:00]\n
*
* Interlaken1 Lanes can be mapped to the following serdes lanes\n
*
*  Interlaken1 Lanes 23 through 16:\n
*\code
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 ------------------------------------------------[20][21][22][23]\n
*     N1 ----------------------------------------------------------------\n
*     N2 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [24][25][26][27][28][29][30][31][32][33][34][35][36][37][38][39]\n
*     N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn1_lanes_configure(sifd_handle_t *h,
                                             sifd_blk_to_srds_msk_cfg_t bts_cfg,
                                             UINT32 lane_mask)
{
    PMC_ERROR rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
    UINT32 mask_check = sifd_bits_used_more_than_once_get(bts_cfg);
    UINT32 lane_los_invert;
    UINT32 lane_los_enable;


    PMC_ENTRY();
    
    if ( mask_check > 0 ) {

        /* If more than one bit has been set in the same position, this is 
           an invalid configuration */

    } else if ( sifd_ilkn1_serdes_select(h, bts_cfg.blk_to_E1_enable_mask, 
                                         SIFD_SRDS_E1_LR) != PMC_SUCCESS) {

    } else if ( sifd_ilkn1_serdes_select(h, bts_cfg.blk_to_N1_enable_mask, 
                                         SIFD_SRDS_N1_SR) != PMC_SUCCESS) {

    } else if ( sifd_ilkn1_serdes_select(h, bts_cfg.blk_to_N2_enable_mask, 
                                         SIFD_SRDS_N2_SR) != PMC_SUCCESS) {

    } else {

        /* Disable all lanes not in use and set rc to success */
        UINT32 inverse_mask = sifd_ilkn_msk_invert(bts_cfg);

        sifd_ilkn1_lane_disable(h, inverse_mask);

        /* LOS processing should always be enabled for all active ILKN lanes */
        lane_los_enable = SIFD_ILKN_1_DLOS_EN_MASK_GET(bts_cfg);

        /* N1 serdes are always inverted. E1 and N2 are always not inverted */
        lane_los_invert = SIFD_ILKN_1_DLOS_INV_MASK_GET(bts_cfg);

        /* now configure LOS */
        sifd_ilkn1_dlos_cfg(h, lane_mask, lane_los_invert, lane_los_enable);

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_ilkn1_lanes_configure */

/* ILKN2 */

/*******************************************************************************
* sifd_ilkn2_lanes_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes signal
*   sources associated with the ILKN 2 specified lane.\n
*   
*   If the mask bit is set in the bts_cfg, the lane is enabled. If the bit is
*   not configured to be used by any SERDES, it disabled.\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must set all lanes to be used by calling this function
*   with the various bts_cfg masks set.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*
*   bts_cfg               - A structure consisting of three bit masks.
*                           Each corresponding to a separate SERDES.\n
*                            
*                           blk_to_E1_enable_mask: Denotes the lane number this
*                           block to enable to the E1 SERDES.\n
*                           blk_to_N1_enable_mask: The lane number from this
*                           block to enable to the N1 SERDES.\n
*                           blk_to_N2_enable_mask: The lane number from this 
*                           block to enable to the N2 SERDES.\n
*
*                           The LSB representing ILKN2 Lane 0, the MSB
*                           represents ILKN2 Lane 23. Bits higher than the 
*                           23rd are unused.\n
* 
*                           It should be noted that only one lane may be 
*                           targeted towards one SERDES.\n
*
*                           See note for which bits in the bit mask are 
*                           valid. 
*
*   lane_mask             - bit mask of ILKN2 SERDES lanes where LSB is lane 0
*                           (1 for used & 0 for unused)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken2 may connect to are:\n
* ______________________________________________________________________________\n
*
*   The E1 Serdes, Lanes[23:00]\n
*   The N1 Serdes, Lanes[15:08]\n
*   The N2 Serdes, Lanes[07:00]\n
*
*  Interlaken2 Lanes can be mapped to the following serdes lanes
*
*  Interlaken2 Lanes 23 through 16:
*\code
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     N1 ----------------------------------------------------------------\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N1 --------------------------------[15][14][13][12][11][10][ 9][ 8]\n
*     N2 [ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]--------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn2_lanes_configure(sifd_handle_t *h,
                                             sifd_blk_to_srds_msk_cfg_t bts_cfg,
                                             UINT32 lane_mask)
{
    PMC_ERROR rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
    UINT32 lane_los_invert;
    UINT32 lane_los_enable;
    UINT32 mask_check = sifd_bits_used_more_than_once_get(bts_cfg);

    PMC_ENTRY();

    if ( mask_check > 0 ) {

        /* If more than one bit has been set in the same position, this is 
           an invalid configuration */

    } else if ( sifd_ilkn2_serdes_select(h, bts_cfg.blk_to_E1_enable_mask, 
                                        SIFD_SRDS_E1_LR) != PMC_SUCCESS) {

    } else if ( sifd_ilkn2_serdes_select(h, bts_cfg.blk_to_N1_enable_mask, 
                                        SIFD_SRDS_N1_SR) != PMC_SUCCESS) {

    } else if ( sifd_ilkn2_serdes_select(h, bts_cfg.blk_to_N2_enable_mask, 
                                        SIFD_SRDS_N2_SR) != PMC_SUCCESS) {

    } else {

        /* Disable all lanes not in use and set rc to success */
        UINT32 inverse_mask = sifd_ilkn_msk_invert(bts_cfg);

        sifd_ilkn2_lane_disable(h, inverse_mask);

        /* LOS processing should always be enabled for all active ILKN lanes */
        lane_los_enable = SIFD_ILKN_2_DLOS_EN_MASK_GET(bts_cfg);

        /* N1 serdes are always inverted. E1 and N2 are always not inverted */
        lane_los_invert = SIFD_ILKN_2_DLOS_INV_MASK_GET(bts_cfg);

        /* now configure LOS */
        sifd_ilkn2_dlos_cfg(h, lane_mask, lane_los_invert, lane_los_enable);

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_ilkn2_lanes_configure */

/* Common ILKN1/2 */

/*******************************************************************************
* sifd_ilkn_lane_remap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes data
*   sources associated with the ILKN 2 specified lane.\n\n

*   This function controls the mapping of user logical data lanes into the
*   physical TX/RX Serdes lanes. By default, the logical lanes and physical 
*   SERDES lanes have a one-to-one mapping. It can be helpful, for layout 
*   purposes, to swap lanes. Any enabled RX lane can be remapped to any other 
*   enabled RX lane. Any enabled TX lane can be remapped to any other enabled 
*   TX lane.\n\n
*
*   Restrictions:\n
*   - No two lanes can be mapped to the same lane. This means every REMAP_LANE
*     field in these registers must have a unique value.\n\n
*
*   - If a Physical SERDES lane is disabled, no lane may be remapped to it. Any
*     disabled lane must be mapped to itself.\n\n
*
*   - This MMR should not be changed after initialization.
*   
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
* 
*   ilkn_inst             - instance of ILKN block to configure.\n
*                               SIFD_COPI_ILKN1\n
*                               SIFD_COPI_ILKN2\n
* 
*   dir                   - the direction to configure.\n
*                               SIFD_ILKN_TX_DIRECTION\n
*                               SIFD_ILKN_RX_DIRECTION\n
*   
*   map                   - Structure indicating which lanes are to be
*                           remaped where. Each lane is defaulted to the same
*                           numbered SERDES in a 1:1 mapping.\n
*                           For example setting "lane2" to 0x05 will cause
*                           data intended for Lane 2 to appear on the physical 
*                           SERDES pins for lane 5.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Note that the remapping registers control only the order that the user data 
*   is stripped into lanes. It does not change the mapping of per SERDES Lane 
*   registers to the physical SERDES pins.  Any per lane functions and
*   registers stay associated with the SERDES pins. 
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_lane_remap(sifd_handle_t *h, 
                                       sifd_ilkn_inst_t ilkn_inst,
                                       sifd_ilkn_dir_t dir,
                                       sifd_ilkn_lane_remap_cfg_t map)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) || (dir > SIFD_ILKN_RX_DIRECTION)) {

        rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
    /* check to make sure the values are all above 0 and less that 24*/
    } 
    
    /* Check for out of range lanes and update the map for any
     * disabled lanes */
    if(rc == PMC_SUCCESS)
        rc = sifd_validate_lane_remap_within_range(&map);

    if(rc == PMC_SUCCESS)
        rc = sifd_validate_lane_remap_contains_no_duplicates(map);

    if(rc == PMC_SUCCESS)
    {
        ilkn_field_REMAP_LANE0_set(NULL, h, ilkn_inst, dir, map.lane[0]);
        ilkn_field_REMAP_LANE1_set(NULL, h, ilkn_inst, dir, map.lane[1]);
        ilkn_field_REMAP_LANE2_set(NULL, h, ilkn_inst, dir, map.lane[2]);
        ilkn_field_REMAP_LANE3_set(NULL, h, ilkn_inst, dir, map.lane[3]);
        ilkn_field_REMAP_LANE4_set(NULL, h, ilkn_inst, dir, map.lane[4]);
        ilkn_field_REMAP_LANE5_set(NULL, h, ilkn_inst, dir, map.lane[5]);
        ilkn_field_REMAP_LANE6_set(NULL, h, ilkn_inst, dir, map.lane[6]);
        ilkn_field_REMAP_LANE7_set(NULL, h, ilkn_inst, dir, map.lane[7]);
        ilkn_field_REMAP_LANE8_set(NULL, h, ilkn_inst, dir, map.lane[8]);
        ilkn_field_REMAP_LANE9_set(NULL, h, ilkn_inst, dir, map.lane[9]);
        ilkn_field_REMAP_LANE10_set(NULL, h, ilkn_inst, dir, map.lane[10]);
        ilkn_field_REMAP_LANE11_set(NULL, h, ilkn_inst, dir, map.lane[11]);
        ilkn_field_REMAP_LANE12_set(NULL, h, ilkn_inst, dir, map.lane[12]);
        ilkn_field_REMAP_LANE13_set(NULL, h, ilkn_inst, dir, map.lane[13]);
        ilkn_field_REMAP_LANE14_set(NULL, h, ilkn_inst, dir, map.lane[14]);
        ilkn_field_REMAP_LANE15_set(NULL, h, ilkn_inst, dir, map.lane[15]);
        ilkn_field_REMAP_LANE16_set(NULL, h, ilkn_inst, dir, map.lane[16]);
        ilkn_field_REMAP_LANE17_set(NULL, h, ilkn_inst, dir, map.lane[17]);
        ilkn_field_REMAP_LANE18_set(NULL, h, ilkn_inst, dir, map.lane[18]);
        ilkn_field_REMAP_LANE19_set(NULL, h, ilkn_inst, dir, map.lane[19]);
        ilkn_field_REMAP_LANE20_set(NULL, h, ilkn_inst, dir, map.lane[20]);
        ilkn_field_REMAP_LANE21_set(NULL, h, ilkn_inst, dir, map.lane[21]);
        ilkn_field_REMAP_LANE22_set(NULL, h, ilkn_inst, dir, map.lane[22]);
        ilkn_field_REMAP_LANE23_set(NULL, h, ilkn_inst, dir, map.lane[23]);

    }

    PMC_RETURN(rc);
} /* sifd_ilkn_lane_remap */

/* DSIS */

/*******************************************************************************
* sifd_serdes_subsystem_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Selects the internal lane to/from a SERDES lane.  The internal lane can 
*   be swizzled from a specified subsystem lane.  The SERDES lane will be
*   enabled.\n\n
* 
*   When CBRC/OTN is used in a MLD configuration (i.e. multi-lane), a master
*   needs to be specified for the clock source.  Both the master internal lane
*   and its matching master SERDES lane need to be specified.\n\n
* 
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.\n\n
*
*   Multicasting several lanes to the same subsystem may result in
*   undetermined behaviour and is not supported. It is up to the user to
*   ensure that these configurations do not occur.\n\n
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   serdes_rx_lane        - RX SERDES lane number
*   serdes_tx_lane        - TX SERDES lane number
*   subsystem_type        - direct the lane to this subsystem
*   single_lane_otn_cbrc  - single lane CBRC/OTN (TRUE) or MLD configuration
*                           (FALSE) that derives its clock from the master lane
*   master_serdes_lane    - master clock SERDES lane for TX FIFO
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Following is a diagram indicating which SERDES lanes may be mapped
*   to which internal lanes:\n\n
*
*   SERDES Lane [11:00]  -> Internal Lane [11:00]
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_subsystem_configure(sifd_handle_t *sifd_handle, 
                                                 UINT32 subsystem_lane, 
                                                 UINT32 serdes_rx_lane, 
                                                 UINT32 serdes_tx_lane, 
                                                 sifd_dsis_type_t subsystem_type,
                                                 BOOL single_lane_otn_cbrc,
                                                 UINT32 master_serdes_lane)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL valid_sub = FALSE;
    UINT32 clock_enable = SIFD_CLK_DISABLE;

    UINT32 internal_rx_lane = sifd_handle->var.internal_lane_rx[serdes_rx_lane];
    UINT32 internal_tx_lane = sifd_handle->var.internal_lane_tx[serdes_tx_lane];

    UINT32 master_serdes = serdes_tx_lane;                 /* default master */
    UINT32 master_lane = internal_tx_lane;                 /* default master */

    UINT32 ret_value_data = 3; /* 3 is invalid */
    UINT32 ret_value_clock = 3;
    UINT32 ret_value_serdes = 3;

    PMC_ENTRY();

    /*
     * Add check to ensure we do not try to configure outside the correct number
     * of lanes
     */
    if ((serdes_rx_lane > SIFD_MAXIMUM_INTERNAL_LANE_NUM)  ||
        (serdes_tx_lane > SIFD_MAXIMUM_INTERNAL_LANE_NUM)) {


        rc = SIFD_ERR_NO_SW_CONTENT;

    } else {
        switch (subsystem_type) {
        case SIFD_DSIS_CBRC:
            clock_enable = SIFD_CBRC_CLK_ENABLE;               /* enable CBRC clock */
            valid_sub = TRUE;
            if (!single_lane_otn_cbrc) {                   /* master on another lane? */
                master_lane = sifd_handle->var.internal_lane_tx[master_serdes_lane];
                master_serdes = master_serdes_lane;
            }
            break;
        case SIFD_DSIS_OTN:
            clock_enable = SIFD_OTN_CLK_ENABLE;                /* enable OTN clock */
            valid_sub = TRUE;
            if (!single_lane_otn_cbrc) {                   /* master on another lane? */
                master_lane = sifd_handle->var.internal_lane_tx[master_serdes_lane];
                master_serdes = master_serdes_lane;
            }
            break;
        case SIFD_DSIS_ENET:
            clock_enable = SIFD_ENET_CLK_ENABLE;               /* enable ENET clock */
            valid_sub = TRUE;
            break;
        default:
            break;
        }
    }

    /* Check if the connecting subsystem is valid */
    if (!valid_sub)
    {
        rc = PMC_ERR_INVALID_PARAMETERS;
    }
    if (rc == PMC_SUCCESS)
    {
        rc = sifd_internal_lane_param_check(subsystem_lane);
    }
    if (rc == PMC_SUCCESS)
    {
        rc = sifd_serdes_to_internal_param_check(serdes_rx_lane, internal_rx_lane, &ret_value_data);
    }
    if (rc == PMC_SUCCESS)
    {
        rc = sifd_internal_to_serdes_param_check(internal_tx_lane, serdes_tx_lane, &ret_value_serdes);
    }
    if (rc == PMC_SUCCESS)
    {
        rc = sifd_serdes_to_internal_param_check(master_serdes, master_lane, &ret_value_clock);
    }
    if (rc == PMC_SUCCESS)
    {
        /* select SERDES receive clock & data for internal lane */
        sys_dsis_field_RX_I_SEL_set(NULL, sifd_handle, internal_rx_lane, 0);

        /* select rx lane clock & data for subsystem lanes (i.e. swizzle) and 
         * enable rx interface of subsystem lanes */
        sifd_serdes_dsis_rx_sel_and_rx_en_set(sifd_handle, subsystem_lane, 
                                              internal_rx_lane, clock_enable);

        /* select SERDES transmit clock for internal transmit lane (which may be the master clock) */
        sys_dsis_field_TX_I_CLK_SEL_set( NULL, sifd_handle, internal_tx_lane, ret_value_clock );
   

        /* select internal transmit clock for subsystem lanes
               - multi-lane interfaces on CBRC & OTN use a master clock - all lanes
                 are set to the same value */
        sys_dsis_field_TX_CLK_SEL_set(NULL, sifd_handle, subsystem_lane, master_lane);

        if (subsystem_type == SIFD_DSIS_ENET) {
            /* select default transmit clock for ENET subsystem to be the internal lane (SERDES) clock*/
            sys_dsis_field_range_ENET_TX_CLK_SEL_set(NULL, sifd_handle, subsystem_lane, subsystem_lane, 0);
        }

        /* select subsystem that drives internal transmit lane before reordering */
        rc = sifd_serdes_tx_if_sel_update(sifd_handle, subsystem_lane, subsystem_type);
    }

    if (rc == PMC_SUCCESS)
    {        
        /* select internal lane before reordering for SERDES lane (i.e. swizzle) */
        sys_dsis_field_TX_SEL_set(NULL, sifd_handle, serdes_tx_lane, subsystem_lane);  
        
        /* reset TX FIFO */
        sys_dsis_field_range_TX_FIFO_RESET_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 1);
        
        /* clock for FIFO write port - same as transmit clock of master SERDES port */
        sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set(NULL, sifd_handle, serdes_tx_lane, master_serdes);
        
        switch (subsystem_type) {
        case SIFD_DSIS_ENET:
            /* set FIFO to master mode */
            sys_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 0);
            break;

        case SIFD_DSIS_CBRC:
        case SIFD_DSIS_OTN:
            if (single_lane_otn_cbrc || (serdes_tx_lane == master_serdes_lane)) {
                /* set FIFO to master mode */
                sys_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 0);
            } else {
                /* set FIFO to slave mode */
                sys_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 1);
            }
            break;
        default:
            break;
        }

        /* set FIFO for auto-centering */
        sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 1);

        /* release reset of TX FIFO */
        sys_dsis_field_range_TX_FIFO_RESET_set(NULL, sifd_handle, serdes_tx_lane, serdes_tx_lane, 0);

        rc = sifd_subsystem_lane_enable(sifd_handle, serdes_tx_lane, 
                                        subsystem_lane, clock_enable); 
    }

    PMC_RETURN(rc);
 } /* sifd_serdes_subsystem_configure */

/*******************************************************************************
* sifd_serdes_subsystem_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Disables the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_subsystem_disable(sifd_handle_t *sifd_handle, 
                                               UINT32 serdes_lane, 
                                               UINT32 subsystem_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((serdes_lane > SIFD_MAXIMUM_INTERNAL_LANE_NUM) ||
        (subsystem_lane > SIFD_MAXIMUM_INTERNAL_LANE_NUM)) {

        rc = SIFD_ERR_NO_SW_CONTENT;

    } else {

       rc = sifd_serdes_subsystem_set(sifd_handle, 
                                      serdes_lane, 
                                      subsystem_lane,
                                      SIFD_CLK_DISABLE);

    }

    PMC_RETURN(rc);
} /* sifd_serdes_subsystem_disable */

/*******************************************************************************
* sifd_enet_clock_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Selects source for ENET clock - whether the source is a mate DSIS
*   (e.g. LINE ENET DSIS for SYS ENET) or internal lane (i.e. one of the
*   SERDES ports).\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   enet_clk_enable       - enable transmit clock from mate DSIS (TRUE) or
*                           from internal lane (FALSE)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_enet_clock_select(sifd_handle_t *sifd_handle, 
                                        UINT32 subsystem_lane, 
                                        BOOL enet_clk_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if (sifd_internal_lane_param_check(subsystem_lane) == PMC_SUCCESS) {

        /* select transmit clock for ENET subsystem */
        sys_dsis_field_range_ENET_TX_CLK_SEL_set(NULL, sifd_handle, subsystem_lane, subsystem_lane, enet_clk_enable);

        sifd_dsis_tx_clk_set(sifd_handle, subsystem_lane, SIFD_ENET_CLK_ENABLE, enet_clk_enable);

        rc = PMC_SUCCESS;

    } else {

        rc = PMC_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
} /* sifd_enet_clock_select */

/*******************************************************************************
* sifd_serdes_c_ckctl_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
* 
*   Selects the C_CKCTL source (i.e. subsystem) for each internal (SERDES) lane.
*   This also drives C_CKCTL on the equivalent SERDES lanes 0 to 11.\n\n
*
*   Note that lane re-ordering is not possible for C_CKCTL (ACB) selection.\n\n
* 
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane (0 to 11)
*   clk_source            - source block for the ACB signal
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Following is a diagram indicating which internal lanes may be
*   mapped to which SERDES lanes:\n\n
*
*   Internal Lane [11:00]  -> SERDES Lane [11:00]
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_c_ckctl_configure(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_lane, 
                                               sifd_sig_src_type_t clk_source)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 set_value = 0;

    PMC_ENTRY();

    switch (clk_source) {
    case SIFD_SRC_CBRC:
        set_value = 0; 
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_OTN_IPM:
        set_value = 1; 
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_ENET:
        set_value = 2; 
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_OTN_LPM:
        set_value = 3; 
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_OTN_EPM:
        set_value = 4; 
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_DIG_M1:
        set_value = 5;
        rc = PMC_SUCCESS;
        break;
    case SIFD_SRC_DIG_IPME:
        set_value = 6;
        rc = PMC_SUCCESS;
        break;
    default:                 /* all other sources are not valid */
        break;
    }


    if (rc == PMC_SUCCESS) {

        if ((rc = sifd_serdes_lane_param_check(serdes_lane)) == PMC_SUCCESS) {

            sys_dsis_field_ACB_IF_SEL_set(NULL, sifd_handle, serdes_lane, set_value);

        }
    }

    PMC_RETURN(rc);
} /* sifd_serdes_c_ckctl_configure */

/*******************************************************************************
* sifd_serdes_acb_mux_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
* 
*   Configures mux for routing ACB.  ACB can be routed to the LINE side
*   (i.e. to LIFD) or stay on the SYS side (which is the default routing).\n\n
* 
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.\n\n
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - SERDES lane to mux the ACB (12 to 19)
*   select_lifd_enable    - mux selector (TRUE selects LIFD and FALSE selects
*                           SIFD DSIS)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_acb_mux_configure(sifd_handle_t *sifd_handle, 
                                               UINT32 serdes_lane,
                                               BOOL select_lifd_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 mux_lane;

    PMC_ENTRY();

    if ((serdes_lane > SIFD_MAXIMUM_SERDES_LANE_NUM) && 
        (serdes_lane <= SIFD_MAXIMUM_LIFD_SERDES_LANE_NUM)) {

        mux_lane = serdes_lane - SIFD_MAXIMUM_SERDES_LANE_NUM - 1;

        sifd_dft_field_range_N1_SERD_ACB_SEL_set(NULL, sifd_handle, mux_lane, mux_lane, select_lifd_enable);

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_serdes_acb_mux_configure */

/*******************************************************************************
* sifd_line_side_route_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures mux for routing SERDES lanes.  SERDES lanes can be routed
*   to the LINE side (i.e. to LIFD) or stay on the SYS side (which is the
*   default routing).  Although the documentation refers to Line OTN, the
*   real purpose is to route the SERDES lanes to the LINE side.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - SERDES lane to mux the ACB (12 to 19)
*   select_lifd_enable    - mux selector (TRUE selects LIFD and FALSE selects
*                           SIFD DSIS)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_line_side_route_configure(sifd_handle_t *sifd_handle,
                                                UINT32 serdes_lane,
                                                BOOL select_lifd_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 lineotn_lane;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sifd_handle, SIFD_ERR_CODE_ASSERT, 0, 0);

    if ((serdes_lane > SIFD_MAXIMUM_SERDES_LANE_NUM) &&
        (serdes_lane <= SIFD_MAXIMUM_LIFD_SERDES_LANE_NUM))
    {

        lineotn_lane = serdes_lane - SIFD_MAXIMUM_SERDES_LANE_NUM - 1; 

        if (select_lifd_enable) {                /* route to LIFD? */
            sifd_dft_field_LOTN_TX_LANE_DIS_set(NULL, sifd_handle, lineotn_lane, SIFD_LOTN_LANE_ENABLE);
            sifd_dft_field_LOTN_RX_LANE_DIS_set(NULL, sifd_handle, lineotn_lane, SIFD_LOTN_LANE_ENABLE);

            sifd_dft_field_N1_TX_LANE_SEL_set(NULL, sifd_handle, lineotn_lane, SIFD_N1_LANE_SELECT_LOTN);
        }
        else {
            sifd_dft_field_LOTN_TX_LANE_DIS_set(NULL, sifd_handle, lineotn_lane, SIFD_LOTN_LANE_DISABLE);
            sifd_dft_field_LOTN_RX_LANE_DIS_set(NULL, sifd_handle, lineotn_lane, SIFD_LOTN_LANE_DISABLE);
        }

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_line_side_route_configure */


/*******************************************************************************
* sifd_internal_lane_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This sets up the lookup table for the internal lane which is used by
*   sifd_serdes_subsystem_configure, to determine the correct internal lane
*   and then set up the data pather.\n\n
* 
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.\n\n
*
*   Multicasting several lanes to the same subsystem may result in
*   undetermined behaviour and is not supported. It is up to the user to
*   ensure that these configurations do not occur.
*
* INPUTS:
*   *sifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_rx_lane        - RX SERDES lane number
*   serdes_tx_lane        - TX SERDES lane number
*   internal_rx_lane      - RX internal lane number to map to.
*   internal_tx_lane      - TX internal lane number to map to.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Certain SERDES -> internal lane combinations are not possible.
*   Following is a diagram indicating which SERDES lanes may be mapped
*   to which internal lanes:\n\n
*
*   SERDES Lane [11:00]  -> Internal Lane [11:00]\n
*\code
*   Internal Lane:     [11][10][09][08][07][06][05][04][03][02][01][00]\n
*   SERDES Lane [12]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [13]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [14]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [15]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*   SERDES Lane [16]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [17]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [18]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [19]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*\endcode
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_internal_lane_configure(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_rx_lane, 
                                               UINT32 serdes_tx_lane, 
                                               UINT32 internal_rx_lane, 
                                               UINT32 internal_tx_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;
    PMC_ENTRY();
    UINT32 ret_val;

    PMC_ASSERT((sifd_handle != NULL), SIFD_ERR_CODE_ASSERT, 0, 0);

    if (sifd_serdes_to_internal_param_check(serdes_rx_lane, internal_rx_lane, &ret_val) != PMC_SUCCESS) {

    } else if (sifd_serdes_to_internal_param_check(serdes_tx_lane, internal_tx_lane, &ret_val) != PMC_SUCCESS) {

    } else {

        sifd_handle->var.internal_lane_rx[serdes_rx_lane] = internal_rx_lane;
        sifd_handle->var.internal_lane_tx[serdes_tx_lane] = internal_tx_lane;
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_internal_lane_configure */

/*******************************************************************************
* sifd_subsystem_los_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures sources and polarity of LOS signals driven to the DSIS.
*   It also configures the mapping of these signals to internal lanes which
*   terminate on one of the ENET, OTN or CBRC subsystems.  There is an option
*   where one internal lane LOS can be broadcast to other internal lanes.
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - SERDES lane (0 to 11)
*   subsystem_lane        - subsystem lane (0 to 11)
*   *los_cfg              - LOS configuration structure
*   use_dlos_source       - Route internally generated DLOS signal to subsystem 
*                           instead of external LOS pin.\n
*                           TRUE: Use DLOS\n
*                           FALSE: Use LOS
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Following is a diagram indicating which internal lanes may be
*   mapped to which SERDES lanes:\n\n
*
*   Internal Lane [11:00]  -> SERDES Lane [11:00]
*
*******************************************************************************/
 PUBLIC PMC_ERROR sifd_subsystem_los_configure(sifd_handle_t *sifd_handle, 
                                               UINT32 serdes_lane,
                                               UINT32 subsystem_lane, 
                                               sifd_los_cfg_t* los_cfg,
                                               BOOL use_dlos_source)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;
 
    PMC_ENTRY();
    BOOL8 is_los_source = TRUE;
    UINT32 is_los_enable_int;
    UINT32 is_dlos_enable_int;
 
    PMC_ASSERT(sifd_handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(los_cfg != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(subsystem_lane < SIFD_MAX_SERDES_LANES, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (use_dlos_source == TRUE) {

        is_los_enable_int = SIFD_DSIS_LOS_DISABLE;
        is_dlos_enable_int = los_cfg->pin_los_en;

    } else {

        is_los_enable_int = los_cfg->pin_los_en;
        is_dlos_enable_int = SIFD_ILKN_DLOS_DISABLE;
    }

    if (serdes_lane == SIFD_SERDES_PORT_NO_LOS_SOURCE) {

        serdes_lane = subsystem_lane;
        is_los_source = FALSE;
        is_los_enable_int = SIFD_DSIS_LOS_DISABLE;
        is_dlos_enable_int = SIFD_ILKN_DLOS_DISABLE;
 
        rc = PMC_SUCCESS;

    } else {

        is_los_source = TRUE;
        rc = sifd_internal_lane_param_check(serdes_lane);
    }
   
    if (rc == PMC_SUCCESS) {

        /* enable device DLOS to DSIS LOS */
        sifd_dft_field_DSIS_DLOS_EN_set(NULL, sifd_handle, serdes_lane, is_dlos_enable_int);

        /* disable device LOS to DSIS LOS */
        sifd_dft_field_DSIS_PIN_LOS_EN_set(NULL, sifd_handle, serdes_lane, is_los_enable_int);        
    
        /* invert DLOS from device */
        sifd_dft_field_DSIS_DLOS_INV_set(NULL, sifd_handle, serdes_lane, los_cfg->pin_los_inv);
        
        /* invert LOS from device */
        sifd_dft_field_DSIS_PIN_LOS_INV_set(NULL, sifd_handle, serdes_lane, los_cfg->pin_los_inv);
     
        if (is_los_source == TRUE) {

            /* invert LOS from SERDES - used to force LOS to DSIS high
                when device LOS & SERDES LOS are not enabled */
            sifd_dft_field_DSIS_LOS_POL_INV_set(NULL, sifd_handle, serdes_lane, los_cfg->los_pol_inv);
         
            /* control LOS that is driven on DSIS internal lane*/
            sys_dsis_field_RX_I_LSEL_set(NULL, sifd_handle, serdes_lane, 0);
                
            /* selects internal LOS that is driven on subsystem lane from DSIS */
            sys_dsis_field_RX_LSEL_set(NULL, sifd_handle, subsystem_lane, serdes_lane);
        }
         
        sifd_handle->var.los_ctxt[serdes_lane].active = is_los_source;
        PMC_MEMCPY(&sifd_handle->var.los_ctxt[serdes_lane].los_cfg, los_cfg, sizeof(sifd_los_cfg_t));    

    }
 
     PMC_RETURN(rc);
 } /* sifd_subsystem_los_configure */

/*******************************************************************************
* sifd_los_internal_context_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets up the SIFD context to map serdes LOS lanes from the system
*   side to SIFD internal lanes.
*
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   los_pin               - LOS serdes pin index
*   los_pin_val           - LOS serdes pin index
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_los_internal_context_set(sifd_handle_t *sifd_handle,
                                               UINT32         los_pin,
                                               UINT32         los_pin_val)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (los_pin > SIFD_MAXIMUM_SERDES_LANE_NUM) 
    {        
        rc = PMC_ERR_INVALID_PARAMETERS;  
    } 
    else if (los_pin_val > SIFD_MAXIMUM_SERDES_LANE_NUM && los_pin_val !=  SIFD_SERDES_PORT_NO_LOS_SOURCE) 
    {
        
        rc = PMC_ERR_INVALID_PARAMETERS;
        
    } 
    else if (sifd_handle->var.los_ctxt[los_pin].active != FALSE) 
    {        
        rc = PMC_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        sifd_handle->var.los_ctxt[los_pin].active = TRUE;
    }

    PMC_RETURN(rc);

} /* sifd_los_internal_context_set */

/*******************************************************************************
* sifd_los_internal_context_unset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Tears down the SIFD context to map serdes LOS lanes from the system
*   side to SIFD internal lanes.
*
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   los_pin               - LOS serdes pin
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_los_internal_context_unset(sifd_handle_t *sifd_handle,
                                                 UINT32         los_pin)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();


    if(los_pin > SIFD_MAXIMUM_SERDES_LANE_NUM) {
        
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    
    sifd_handle->var.los_ctxt[los_pin].active = FALSE;

    PMC_RETURN(rc);

} /* sifd_los_internal_context_unset */

/*******************************************************************************
* sifd_serdes_los_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures SIFD PIN LOS. 
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - serdes lane id (0 to 11)
*   *los_cfg              - LOS configuration structurePoine
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_los_cfg_get(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane, 
                                         sifd_los_cfg_t* los_cfg)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    PMC_ASSERT(sifd_handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(los_cfg != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_lane <= SIFD_MAXIMUM_SERDES_LANE_NUM, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (TRUE == sifd_handle->var.los_ctxt[serdes_lane].active)
    {
        PMC_MEMCPY(los_cfg, &sifd_handle->var.los_ctxt[serdes_lane].los_cfg, sizeof(sifd_los_cfg_t));
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_serdes_los_cfg_get */

/*******************************************************************************
* sifd_serdes_los_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is called by digi_serdes_los_force to force a LOS state in 
*   the subsystem connected with SIFD.
*
* INPUTS:
*   *sifd_handle   - pointer to sifd handle instance to be operated on
*   serdes_lane    - serdes lane id (0 to 11)
*   dlos_en        - dlos enable
*   los_pol_inv    - los polarity invert
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_los_force(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 dlos_en, BOOL8 los_pol_inv)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sifd_handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    sifd_dft_field_DSIS_DLOS_EN_set(NULL, sifd_handle, serdes_lane, dlos_en);
    sifd_dft_field_DSIS_LOS_POL_INV_set(NULL, sifd_handle, serdes_lane, los_pol_inv);
    
    sifd_handle->var.los_ctxt[serdes_lane].los_cfg.los_pol_inv = los_pol_inv;

    PMC_RETURN(rc);
} /* sifd_serdes_los_force */

/*******************************************************************************
* sifd_serdes_los_force_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is called by digi_serdes_los_force_get to get LOS force config in 
*   the subsystem connected with SIFD.
*
* INPUTS:
*   *sifd_handle   - pointer to sifd handle instance to be operated on
*   serdes_lane    - serdes lane id (0 to 11)
*
* OUTPUTS:
*   *dlos_en       - dlos enable
*   *los_pol_inv   - los polarity invert
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_los_force_get(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 *dlos_en, BOOL8 *los_pol_inv)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(sifd_handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    *dlos_en = sifd_dft_field_DSIS_DLOS_EN_get(NULL, sifd_handle, serdes_lane);
    *los_pol_inv = sifd_dft_field_DSIS_LOS_POL_INV_get(NULL, sifd_handle, serdes_lane);
    
    PMC_RETURN(rc);
} /* sifd_serdes_los_force_get */

/*******************************************************************************
*  sifd_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Initializes the SIFD subsystem.\n\n
*
*                  When configuring for Interlaken mode:\n
*                      1) sifd_serdes_init and sifd_rate_cfg
*                         must be called BEFORE this function.\n
*                      2) sifd_ilkn_init should be called AFTER this
*                         function.\n\n
*
*                  Alternatively, this function can be called to disable the
*                  SIFD subsystem\n\n
* 
*                  Enable/disable is controlled by the Interlaken state
*                  (pmc_block_reg_access_state_enum):\n
*                      PMC_BLOCK_ALTERABLE -- Block is ready for cfg'n\n
*                      PMC_BLOCK_DEFAULT   -- Block is held in reset 
*
*  INPUTS:
*            handle   - pointer to sifd handle instance to be operated on
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_init(sifd_handle_t *handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    UINT32 ilkn_state = 0;

    PMC_ENTRY();

    if (handle->var.ilkn_state[SIFD_COPI_ILKN2] == PMC_BLOCK_DEFAULT) { 

        ilkn_state = PMC_BLOCK_DEFAULT;
        ilkn_state = ilkn_state << 1;

    } 
    if (handle->var.ilkn_state[SIFD_COPI_ILKN1] == PMC_BLOCK_DEFAULT) {

        ilkn_state = ilkn_state | PMC_BLOCK_DEFAULT;

    } 

    sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set(NULL, handle, 0);
    sifd_dft_field_DSIS_LOWPWR_CFG_set(NULL, handle, 0);
    sifd_dft_field_COPI_LOWPWR_CFG_set(NULL, handle, ilkn_state);
    sifd_dft_field_COPI_RAM_LOWPWR_CFG_set(NULL, handle, ilkn_state);

    sifd_dft_field_SIFD_DFT_SOFT_RST_set(NULL, handle, 0);
    sifd_dft_field_DSIS_SOFT_RST_set(NULL, handle, 0);
    sifd_dft_field_COPI_SOFT_RST_set(NULL, handle, ilkn_state);
    
    /* Enable interrupts to go to external pin */
    sifd_int_init(handle, TRUE);

    PMC_RETURN(rc);

} /* sifd_init */





/*******************************************************************************
*  sifd_intf_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function performs SIFD interface level enable and
*                  disable operations.\n\n
*
*                  When enabling:\n
*                    For the Tx direction, it will resume trasmission of data
*                    from the CPB subsystem.\n\n
*
*                    For the Rx direction, it will send valid link level flow
*                    control status to the upstream device.\n\n
*
*                  When disabling:\n
*                    For the Tx direction, it will pause data transfer
*                    from the CPB subsystem.\n\n
*
*                    For the Rx direction, it will backpressure the upstream
*                    device with pause frames or link level XOFF indications.\n\n
*
*                  This function must be called after:\n
*                  sifd_flow_ctrl_init\n\n
*
*                  This function is called inside of sifd_flow_ctrl_init
*                  function to intially disable COPI interface.\n\n
*
*                  When SPI4.2 or Interlaken out-of-band flow control is used,
*                  set RX_OOB_LINK_FC to control sending link level flow
*                  control status.\n\n
*
*                  When Interlaken in-band flow control is used,
*                  set RX_LINK0_FC to control sending link level flow
*                  control status.\n\n
*
*
*  INPUTS:
*                  handle  - pointer to sifd handle instance to be operated on
*                  ilkn_inst - The specific ILKN instance\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  enable   -  1 : enable PIF interface to normal operation\n
*                              0 : send link level XOFF/FULL status or
*                              pause data transmission
*                  update_mask - Mask of link level flow control related
*                              configuration field to updated.
*                              Set the corresponding bit to 1 to update a
*                              field, set to 0 to maintain value of configured
*                              field:\n
*                              bit 0  : update RX_OOB_FC field\n
*                              bit 1  : update RX_IB_FC field\n
*                              bit 2  : update RX_ODUK_CH_FC field\n
*                              bit 3  : update RX_CPB_CH_FC field\n
*                              bit 4  : update RX_LINK_FC field\n
*                              bit 5  : update TX_OOB_FC field\n
*                              bit 6  : update TX_IB_FC field\n
*                              bit 7  : update TX_ODUK_CH_FC field\n
*                              bit 8  : update TX_ODUK_LINK_FC field\n
*                              bit 9  : update TX_CPB_CH_FC field\n
*                              bit 10 : update TX_CPB_LINK_FC field\n
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_intf_enable(sifd_handle_t *handle, 
                                   sifd_ilkn_inst_t ilkn_inst,
                                   sifd_ilkn_en_t enable, 
                                   UINT32 update_mask)
{
    PMC_ENTRY();

    /* argument checking */
    if (update_mask > 0x7FF || enable > SIFD_ILKN_ENABLE) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, update_mask, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    if (update_mask & SIFD_TX_CPB_CH_FC) {

        copi_field_TX_CPB_CH_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_TX_ODUK_LINK_FC) {

        copi_field_TX_ODUK_LINK_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_TX_ODUK_CH_FC) {

        copi_field_TX_ODUK_CH_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_TX_IB) {

        copi_field_TX_IB_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_TX_OOB) {

        copi_field_TX_OOB_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_RX_LINK_FC) {

        copi_field_RX_LINK_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_RX_CPB_CH_FC) {

        copi_field_RX_CPB_CH_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_RX_ODUK_CH_FC) {

        copi_field_RX_ODUK_CH_FC_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_RX_IB) {

        copi_field_RX_IB_EN_set(NULL, handle, ilkn_inst, enable);
    }
    if (update_mask & SIFD_RX_OOB) {
        
        copi_field_RX_OOB_EN_set(NULL, handle, ilkn_inst, enable);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_intf_enable */


/*******************************************************************************
*  sifd_lane_disable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function disables one or more of the 24 SERDES 
*                  interface lanes of the COPI interface associated with 
*                  Interlaken.\n\n
*
*                  The transmit and receive lanes are configured symmetrically.\n\n
*
*                  The Interlaken block must be 'Major Bidirectional States' of
*                  2 in order for this function to be executed, otherwise
*                  SIFD_LOG_ERR_INVALID_STATE will be returned.\n\n
*
*                  This function is only valid for Interlaken.
*
*  INPUTS:
*                  *handle  - base address
*                  ilkn_inst - The specific ILKN instance\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  disable_msk - Mask of lanes to be disabled\n
*                               Legal range 0 to 0x00FFFFFF\n
*                               bit index n to n+23 corresponds to SERDES lanes
*                               m to m+23 where n = 0 and m = starting index of
*                               the groups of 8 SERDES lanes (m = 0, 8, 16, 24)
*                               Note setting of 0x00FFFFFF irrelevant as all 
*                               lanes disabled.\n
*                               Set to 1 to disable a lane.\n
*                               Set to 0 to enable a lane.
*
*  OUTPUTS:
*      None.
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_lane_disable(sifd_handle_t *handle, 
                                    sifd_ilkn_inst_t ilkn_inst,
                                    UINT32 disable_msk)
{
    /* variable declaration */
    PMC_ERROR result;

    PMC_ENTRY();

    /* argument checking */
    if ((disable_msk > SIFD_24_LANE_MASK ) || (ilkn_inst > SIFD_COPI_ILKN2)) {

        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* no need to check major bidirectional state since called from init */

    /* Configure Interlaken lane */
    result = sifd_ilkn_util_core_lane_disable(handle, ilkn_inst, disable_msk);

    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_lane_disable */


/*******************************************************************************
*  sifd_flow_ctrl_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function initializes SIFD Rx or Tx flow control.
*                  It configures flow control mode, zone threshold and
*                  internally enable flow control.\n\n
*
*                  Depending on the operating mode of SIFD subsystem, this 
*                  function internally selects a flow control mode, and then
*                  initializes the Interlaken flow control calendar with user 
*                  specific calendar length, by calling sifd_ilkn_fc_init 
*                  function.\n\n
*
*                  This function disables SIFD interface at last step, by
*                  calling sifd_intf_enable function.\n\n
*
*                  For Interlaken flow control configuration, this function 
*                  must be called after sifd_init and sifd_ilkn_init.\n\n
*
*
*  INPUTS:
*                  handle    - pointer to sifd handle instance to be operated on
*                  ilkn_inst - the specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  cal_len   - flow control calendar length
*                  fc_mode   - flow control mode
*
*  OUTPUTS:
*                  None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_flow_ctrl_init(sifd_handle_t *handle, 
                                      sifd_ilkn_inst_t ilkn_inst,
                                      UINT32 cal_len, 
                                      sifd_fc_mode_t fc_mode)
{
    PMC_ERROR result;
    UINT32 update_mask;
    sifd_ilkn_en_t oob_state;

    PMC_ENTRY();

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, ilkn_inst);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* argument checking */
    if ((fc_mode == SIFD_FC_MODE_IN_BAND_16 ||
         fc_mode == SIFD_FC_MODE_IN_BAND_24 ||
         fc_mode == SIFD_FC_MODE_OUT_OF_BAND) && 
        (cal_len < SIFD_ILKN_MIN_CAL_ENTRIES ||
         cal_len > SIFD_ILKN_MAX_CAL_ENTRIES)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, fc_mode, cal_len);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* initialize dynamic configuration variables */
    handle->var.ilkn_fc[ilkn_inst].flow_control_mode = fc_mode;
    handle->var.ilkn_fc[ilkn_inst].calendar_length = cal_len;

    
    if (fc_mode == SIFD_FC_MODE_NONE) {               /* no flow control? */
        /* disable IB and OB flow control */
        result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_RX_OOB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_TX_OOB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_RX_IB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_TX_IB);
        /* initialize the calendars to NULL */
        if (result == PMC_SUCCESS)       
            result = sifd_ilkn_util_core_fc_mode_cfg(handle, ilkn_inst, fc_mode, SIFD_ILKN_MAX_CAL_ENTRIES);
        PMC_RETURN(result);
    }

    /* Enable the OOB logic if we are using OOB FC, enter lowpwr otherwise */
    if (fc_mode == SIFD_FC_MODE_OUT_OF_BAND) {

        oob_state = SIFD_ILKN_DISABLE;

    } else {

        oob_state = SIFD_ILKN_ENABLE;
    }

    copi_field_OOB_LOWPWR_EN_set(NULL, handle, ilkn_inst, oob_state);
    copi_field_IL_SOFT_RST_RX_FC_X2_set(NULL, handle, ilkn_inst, oob_state);

    /* enable flow control */
    /* initialize the flow control calendar */
    result = sifd_ilkn_util_core_fc_mode_cfg(handle, ilkn_inst, fc_mode, cal_len);
    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    /* disable all link & channel flow control to CPB & ODUK */
    update_mask = SIFD_TX_ODUK_LINK_FC | SIFD_RX_LINK_FC;
    result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, update_mask);
    if (result == PMC_SUCCESS) {
        update_mask = SIFD_TX_CPB_CH_FC | SIFD_TX_ODUK_CH_FC | SIFD_RX_CPB_CH_FC | SIFD_RX_ODUK_CH_FC;
        result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, update_mask);
    }

    /* configure either in-band or out-of-band flow control */
    switch(fc_mode) {
    case SIFD_FC_MODE_IN_BAND_16:
    case SIFD_FC_MODE_IN_BAND_24:
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_RX_OOB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_TX_OOB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_ENABLE, SIFD_RX_IB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_ENABLE, SIFD_TX_IB);
        break;
    case SIFD_FC_MODE_OUT_OF_BAND:
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_RX_IB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_DISABLE, SIFD_TX_IB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_ENABLE, SIFD_RX_OOB);
        if (result == PMC_SUCCESS)
            result = sifd_intf_enable(handle, ilkn_inst, SIFD_ILKN_ENABLE, SIFD_TX_OOB);
        break;
    default:
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PRECONFIG, fc_mode, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PRECONFIG);
    }

    /* Initially send link level XOFF/FULL status or pause data transmission
       to peer device */
    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    PMC_RETURN(PMC_SUCCESS);

} /*  sifd_flow_ctrl_init */

/*******************************************************************************
*  sifd_ilkn_fc_calendar_update
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function map or de-map a flow control port to the
*                  specific location in Tx or Rx flow control calendar entry.\n\n
*
*                  When an entry == 0 to 253:\n
*                    This function is used as per channel level operation.\n
*                  When the entry == 254:\n
*                    This function is used as link level operation.\n\n
* 
*                  As part of this function, if link or channel flow control
*                  is enabled/disabled, then the COPI flow control
*                  configuration will be updated.\n\n
*
*                  This function must be called after:\n
*                  sifd_mux_data_path_cfg\n
*                  sifd_ilkn_rx_chnl_cfg\n
*                  sifd_ilkn_tx_chnl_cfg\n\n
* 
*                  It can be called before or after:\n
*                  sifd_ilkn_enable\n\n
*
*                  Parameters *rx_flow_control_calendar_ptr and 
*                  *tx_flow_control_calendar_ptr are changed to reference parameters 
*                  to minimize data on the call stack.
*
*  INPUTS:
*                  handle    - pointer to sifd handle instance to be operated on
*                  ilkn_inst - The specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  *tx_flow_control_calendar_ptr  - new flow control calendar for TX
*                  *rx_flow_control_calendar_ptr  - new flow control calendar for RX. 
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_fc_calendar_update(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              sifd_ilkn_calendar_t *tx_flow_control_calendar_ptr,
                                              sifd_ilkn_calendar_t *rx_flow_control_calendar_ptr)
{
    PMC_ERROR result;
    UINT32 i;
    UINT32 update_mask;
    sifd_ilkn_en_t update_status;
    BOOL new_link_flow_control = FALSE;
    BOOL new_chan_flow_control = FALSE;
    BOOL new_oduk_chan_flow_control = FALSE;
    UINT32 calendar_length ;
    sifd_var_fc_t *ilkn_fc_inst;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, SIFD_ERR_CODE_ASSERT, 0, 0);

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, ilkn_inst);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Interlaken interface must be configured! */
    PMC_ASSERT(PMC_BLOCK_DEFAULT != handle->var.ilkn_state[ilkn_inst], SIFD_ERR_CODE_ASSERT, 0, 0);

    ilkn_fc_inst = &handle->var.ilkn_fc[ilkn_inst];
    calendar_length = ilkn_fc_inst->calendar_length;

    /* check for no flow control - if none, just return */
    if (ilkn_fc_inst->flow_control_mode == SIFD_FC_MODE_NONE)
        PMC_RETURN(PMC_SUCCESS);

    if(tx_flow_control_calendar_ptr != NULL)
    {
        /* any TX calendar changes? */
        for (i = 0; i < calendar_length; ++i) {
            if (tx_flow_control_calendar_ptr->entry[i] != ilkn_fc_inst->tx_flow_control_calendar.entry[i]) {
                ilkn_fc_inst->tx_flow_control_calendar.entry[i] = tx_flow_control_calendar_ptr->entry[i];
                result = sifd_ilkn_util_cal_write(handle, ilkn_inst, SIFD_ILKN_TX_DIRECTION, 
                                               i, ilkn_fc_inst->tx_flow_control_calendar.entry[i]);
                if (result != PMC_SUCCESS)
                    PMC_RETURN(result);
            }
        }


        /* Flag TX link or channel flow control */
        for (i = 0; i < calendar_length; ++i) {
            if (ilkn_fc_inst->tx_flow_control_calendar.entry[i] != SIFD_ILKN_NULL_CAL_ENTRY) {
                if (ilkn_fc_inst->tx_flow_control_calendar.entry[i] == SIFD_ILKN_LINK_FLOW_CONTROL)
                {
                    new_link_flow_control = TRUE;
                }
                else if(ilkn_fc_inst->tx_flow_control_calendar.entry[i] >= SIFD_MAX_CPB_ARB_CHAN)
                {
                    new_oduk_chan_flow_control = TRUE;
                }
                else
                {
                    new_chan_flow_control = TRUE;
                }
            }
        }
        /* TX link flow control transition? */
        if (new_link_flow_control != ilkn_fc_inst->tx_link_flow_control) {
            ilkn_fc_inst->tx_link_flow_control = new_link_flow_control;
            update_mask = SIFD_TX_ODUK_LINK_FC;
            if (new_link_flow_control)
                update_status = SIFD_ILKN_ENABLE;
            else
                update_status = SIFD_ILKN_DISABLE;
            result = sifd_intf_enable(handle, ilkn_inst, update_status, update_mask);
            if (result != PMC_SUCCESS)
                PMC_RETURN(result);
        }
        /* TX channel flow control transition? */
        if (new_chan_flow_control != ilkn_fc_inst->tx_channel_flow_control) {
            ilkn_fc_inst->tx_channel_flow_control = new_chan_flow_control;
            update_mask = SIFD_TX_CPB_CH_FC;
            if (new_chan_flow_control)
                update_status = SIFD_ILKN_ENABLE;
            else
                update_status = SIFD_ILKN_DISABLE;
            result = sifd_intf_enable(handle, ilkn_inst, update_status, update_mask);
            if (result != PMC_SUCCESS)
                PMC_RETURN(result);
        }
        /* TX ODUKSW channel flow control transition? */
        if (new_oduk_chan_flow_control != ilkn_fc_inst->tx_oduk_channel_flow_control) {
            ilkn_fc_inst->tx_oduk_channel_flow_control = new_oduk_chan_flow_control;
            update_mask = SIFD_TX_ODUK_CH_FC;
            if (new_oduk_chan_flow_control)
                update_status = SIFD_ILKN_ENABLE;
            else
                update_status = SIFD_ILKN_DISABLE;
            result = sifd_intf_enable(handle, ilkn_inst, update_status, update_mask);
            if (result != PMC_SUCCESS)
                PMC_RETURN(result);
        }
    } /* end if(tx_flow_control_calendar_ptr != NULL) */

    if(rx_flow_control_calendar_ptr != NULL)
    {

        /* any RX calendar changes? */
        for (i = 0; i < calendar_length; ++i) {
            if (rx_flow_control_calendar_ptr->entry[i] != ilkn_fc_inst->rx_flow_control_calendar.entry[i]) {
                ilkn_fc_inst->rx_flow_control_calendar.entry[i] = rx_flow_control_calendar_ptr->entry[i];
                result = sifd_ilkn_util_cal_write(handle, ilkn_inst, SIFD_ILKN_RX_DIRECTION, 
                                               i, ilkn_fc_inst->rx_flow_control_calendar.entry[i]);
                if (result != PMC_SUCCESS)
                    PMC_RETURN(result);
            }
        }

        /* Flag RX link or channel flow control */
        new_link_flow_control = FALSE;
        new_chan_flow_control = FALSE;
        for (i = 0; i < calendar_length; ++i) {
            if (ilkn_fc_inst->rx_flow_control_calendar.entry[i] != SIFD_ILKN_NULL_CAL_ENTRY) {
                if (ilkn_fc_inst->rx_flow_control_calendar.entry[i] == SIFD_ILKN_LINK_FLOW_CONTROL)
                    new_link_flow_control = TRUE;
                else
                    new_chan_flow_control = TRUE;
            }
        }
        /* RX link flow control transition? */
        if (new_link_flow_control != ilkn_fc_inst->rx_link_flow_control) {
            ilkn_fc_inst->rx_link_flow_control = new_link_flow_control;
            update_mask = SIFD_RX_LINK_FC;
            if (new_link_flow_control)
                update_status = SIFD_ILKN_ENABLE;
            else
                update_status = SIFD_ILKN_DISABLE;
            result = sifd_intf_enable(handle, ilkn_inst, update_status, update_mask);
            if (result != PMC_SUCCESS)
                PMC_RETURN(result);
        }
        /* RX channel flow control transition? */
        if (new_chan_flow_control != ilkn_fc_inst->rx_channel_flow_control) {
            ilkn_fc_inst->rx_channel_flow_control = new_chan_flow_control;
            update_mask = SIFD_RX_CPB_CH_FC | SIFD_RX_ODUK_CH_FC;
            if (new_chan_flow_control)
                update_status = SIFD_ILKN_ENABLE;
            else
                update_status = SIFD_ILKN_DISABLE;
            result = sifd_intf_enable(handle, ilkn_inst, update_status, update_mask);
            if (result != PMC_SUCCESS)
                PMC_RETURN(result);
        }
        
    } /* end if(rx_flow_control_calendar_ptr != NULL) */

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_fc_calendar_update */


/*******************************************************************************
*  sifd_ilkn_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configure the INTERLAKEN instance to
*                  transition between 'Major Bidirectional States' of:\n
*                  1.  Block in reset with all registers in default values\n
*                  2.  Block in reset with configuration registers maintaining
*                      their last initialized value\n\n
*
*                  This function must be used after: sifd_flow_ctrl_init
*                  cpb_init, sifd_serdes_init and sifd_rate_cfg functions.\n\n
*
*                  This function must be called after sifd_init\n\n
*
*                  Setting enable = 1 moves from state 1 to 2.  Setting
*                  enable = 0 allows an interface to move from state 2 or 3 to
*                  state 1.\n\n
*
*                  When entering state 2, the Interlaken interface will be
*                  initialized with a BURST_MAX of 256 bytes and a metaframe
*                  period of 2048 words.
*
*
*  INPUTS:
*   handle          - pointer to sifd handle instance to be operated on
*   ilkn_inst       - specific ILKN instance to Init.\n
*                       SIFD_COPI_ILKN1 -- Interlaken 1 Instance\n
*                       SIFD_COPI_ILKN2 -- Interlaken 2 Instance
*   ilkn            - pointer to a structure with COPI/Interlaken configuration
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_init(sifd_handle_t *handle,
                                 sifd_ilkn_inst_t ilkn_inst,
                                 sifd_ilkn_cfg_t *ilkn)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 disable_mask = (~ilkn->lane_mask) & SIFD_24_LANE_MASK;
    copi_cpb_oduk_cfg_t co_cfg;

    PMC_ENTRY();

    /* argument checking */
    if (((ilkn->lane_mask | SIFD_24_LANE_MASK) != SIFD_24_LANE_MASK) || (ilkn_inst > SIFD_COPI_ILKN2)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, ilkn->lane_mask);
/* LCOV_EXCL_STOP */
        result = SIFD_LOG_ERR_INVALID_PARAMETERS;
    }

    /* Common configuration */
    co_cfg.oduk_rx_chext_en = FALSE;
    co_cfg.oduk_tx_chext_en = FALSE;
    co_cfg.oduk_tx_burst = ilkn->burst;
    co_cfg.oduk_tx_enabled = ilkn->oduksw_intf_en;
    co_cfg.oduk_tx_mode = ilkn->oduksw_tx_xfer_mode;
    co_cfg.oduk_hdr_insert_extract = ilkn->oduksw_header;
    co_cfg.oduk_2b_hdr_mode = ilkn->oduksw_2b_header_mode;

    if ((ilkn->oduksw_tx_xfer_mode == SIFD_SEGMENT_MODE) && (ilkn->oduksw_header == SIFD_ODUK_2B_HEADER) && (!ilkn->oduksw_2b_header_mode)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn->oduksw_header, 0);
/* LCOV_EXCL_STOP */
        result = SIFD_LOG_ERR_INVALID_PARAMETERS;
    }

    if(result == PMC_SUCCESS)
    {
        co_cfg.rx_mode = ilkn->rx_xfer_mode;
    
        co_cfg.cpb_rx_type_en = TRUE;
        co_cfg.cpb_tx_type_en = TRUE;
        co_cfg.cpb_tx_burst = ilkn->burst;
        co_cfg.cpb_tx_enabled = ilkn->cpb_intf_en;
        co_cfg.cpb_tx_mode = ilkn->cpb_tx_xfer_mode;
    
        co_cfg.rx_burstmax = ilkn->burst;
    
        /*
         * INTERLAKEN initialization
         */


        /* Put COPI TSB & Interlaken cores in reset */
        copi_field_COPI_SOFT_set(NULL, handle, ilkn_inst, SIFD_ILKN_ENABLE);
        copi_field_ILAKEN_SOFT_set(NULL, handle, ilkn_inst, SIFD_ILKN_ENABLE);

        /* Put Interlaken in 'Major Bidirectional State' STANDBY */
        result = sifd_ilkn_util_core_enable(handle, ilkn_inst, SIFD_COPI_ILKN_STATE_STANDBY);
    }

    if(result == PMC_SUCCESS)
    {
        /* Perform COPI Level initialization */
        result = sifd_ilkn_util_copi_cfg(handle, ilkn_inst, co_cfg);
    }

    if(result == PMC_SUCCESS)
    {
        /* Take COPI TSB & Interlaken cores out of reset, but leave data path in reset */
        copi_field_COPI_SOFT_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_ILAKEN_SOFT_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);

        /* apply initial Interlaken Core configuration */
        result = sifd_ilkn_util_core_cfg(handle, 
                                         ilkn_inst, 
                                         ilkn->burst, 
                                         ilkn->burst_short,
                                         ilkn->burst_min, 
                                         ilkn->metaframe);
    }

    if(result == PMC_SUCCESS)
    {
        /* apply TX rate limit configuration */
        result = sifd_ilkn_tx_rate_limit_cfg(handle, ilkn_inst,
                                             ilkn->tx_rate_limit.rate_limit,
                                             ilkn->tx_rate_limit.burst_limit,
                                             ilkn->burst,
                                             ilkn->tx_rate_limit.byte_granularity);
    }

    if(result == PMC_SUCCESS)
    {
        /* Initialize the flow control calendars. Valid values will be be written to the calendar 
         * during calls to CPB and ODUkSW channel activate */
        result = sifd_flow_ctrl_init(handle, ilkn_inst, ilkn->fc_cal_length, ilkn->fc_mode);
    }

    if(result == PMC_SUCCESS)
    {
        /* configure Interlaken SERDES lane remapping */
        result = sifd_ilkn_lane_remap(handle, 
                                      ilkn_inst, 
                                      SIFD_ILKN_TX_DIRECTION, 
                                      ilkn->lane_tx_order);
    }

    if(result == PMC_SUCCESS)
    {
        result = sifd_ilkn_lane_remap(handle, 
                                     ilkn_inst, 
                                     SIFD_ILKN_RX_DIRECTION, 
                                     ilkn->lane_rx_order);
    }

    if(result == PMC_SUCCESS)
    {
        /* enable lanes */
        result = sifd_lane_disable(handle, ilkn_inst, disable_mask);
    }

    if(result == PMC_SUCCESS)
    {
        /* Non-default setting of TX and RX MIN_DIS/MAX_DIS fields */
        
        /* Configure TX direction */
        /*set Tx MAX_DIS and MIN_DIS to 1 */
        ilkn_field_MAX_DIS_set( NULL, handle, ilkn_inst, SIFD_ILKN_TX_DIRECTION, 1);
        ilkn_field_MIN_DIS_set( NULL, handle, ilkn_inst, SIFD_ILKN_TX_DIRECTION, 1);
    
        /* Configure RX direction */
        /*set Rx MAX_DIS and MIN_DIS to 0 */
        ilkn_field_MAX_DIS_set( NULL, handle, ilkn_inst, SIFD_ILKN_RX_DIRECTION, 0);
        ilkn_field_MIN_DIS_set( NULL, handle, ilkn_inst, SIFD_ILKN_RX_DIRECTION, 0);
    }
    /* non-default threshold settings */
    /* FIFO will report XOFF when rising into ZONE b'11 FIFO will report XON
       when falling into ZONE b'01 */

    if(result == PMC_SUCCESS)
    {
        result = sifd_zone_th_cfg(handle, 
                                  SIFD_COPI_ILKN1,
                                  0x3, 0x1,
                                  0x3, 0x1);
    }
    if(result == PMC_SUCCESS)
    {
        result = sifd_zone_th_cfg(handle, 
                                  SIFD_COPI_ILKN2,
                                  0x3, 0x1,
                                  0x3, 0x1);
    }

    PMC_RETURN(result);
} /* sifd_ilkn_init */


/*******************************************************************************
*  sifd_ilkn_tx_rate_limit_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configure the optional transmit rate limiting
*                  feature for one of the INTERLAKEN instance.\n\n
*
*                  The INTERLAKEN block must be 'Major Bidirectional States'
*                  of 2 in order for this function to be executed, otherwise
*                  SIFD_LOG_ERR_INVALID_STATE will be returned.
*
*
*
*  INPUTS:
*                  handle    - pointer to sifd handle instance to be operated on
*                  ilkn_inst - the specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  rate_limit - 0 : disabled no rate limiting applied\n
*                               1 to 50 : amount of rate limiting applied in
*                               1 GBPS increments
*                  burst_limit - burst_limit Parameter setting as described in
*                                Interlaken Protocol Definition.  'The maxiumum
*                                quantity of data that may be burst across the
*                                interface before invoking the rate limiting
*                                logic (bytes)'
*                  burst_max - Interlaken burst size (128, 192 or 256 bytes)
*                  byte_gran - number of credits that are to be subtracted
*                              from the token bucket when an EOP is
*                              encountered.\n
*                               0 : 1 byte\n
*                               1 : 2 bytes\n
*                               2 : 4 bytes\n
*                               3 : 8 bytes\n
*                               4 : 16 bytes\n
*                               5 : 32 bytes
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_tx_rate_limit_cfg(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              UINT32 rate_limit,
                                              UINT32 burst_limit, 
                                              sifd_oc_burst_t burst_max,
                                              UINT32 byte_gran)
{
    PMC_ERROR result;

    PMC_ENTRY();

    /* argument checking */
    if (rate_limit > 100 || byte_gran > 5) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, rate_limit, byte_gran);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* no need to check major bidirectional state since this is done in init */

    result = sifd_ilkn_util_core_tx_rate_limit_cfg(handle, ilkn_inst, rate_limit,
                                                   burst_limit, burst_max, byte_gran);
    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_tx_rate_limit_cfg */


/*******************************************************************************
*  sifd_ilkn_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configure the INTERLAKEN instance to
*                  transition between 'Major Bidirectional States' of:\n
*                  2.  Block in reset with configuration registers maintaining
*                      their last initialized value\n
*                  3.  Block in operational state with the ability to send/
*                      receive user data or SERDES test patterns.\n\n
*
*                  Setting enable = 1 moves from state 2 to 3.  Setting
*                  enable = 0 allows an interface to move from state 3 to
*                  state 2.\n\n
*
*                  It is assumed that the CBRI instance is in state 2 when
*                  calling this function with enable = 1 and in state 3 when
*                  calling this function with enable = 0.  If these
*                  relationships are not trun SIFD_LOG_ERR_INVALID_STATE is
*                  returned.\n\n
* 
*                  Must be called after:\n
*                    sifd_mux_data_path_cfg\n
*                    sifd_ilkn_rx_chnl_cfg\n
*                    sifd_ilkn_tx_chnl_cfg
*
*  INPUTS:
*                  handle    - pointer to sifd handle instance to be operated on
*                  ilkn_inst - the specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  enable    - enable/disable flag\n
*                              0 : disable\n
*                              1 : enable
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_enable(sifd_handle_t *handle, 
                                  sifd_ilkn_inst_t ilkn_inst,
                                  sifd_ilkn_en_t enable)
{
    PMC_ERROR rc = SIFD_LOG_ERR_UNKNOWN;
    sifd_ilkn_state_t new_state;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, SIFD_ERR_CODE_ASSERT, 0, 0);
                    
    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) || (enable > SIFD_ILKN_ENABLE)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, enable);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Interlaken interface must be configured to enable */
    if (handle->var.ilkn_state[ilkn_inst] == PMC_BLOCK_DEFAULT) {
        if (enable == SIFD_ILKN_DISABLE)         /* OK to disable unconfigured interface */
            PMC_RETURN(PMC_SUCCESS);
        else                                     /* unconfigured interface cannot be enabled */
            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* determine desired state */
    if (enable == SIFD_ILKN_DISABLE) {

       new_state = SIFD_COPI_ILKN_STATE_STANDBY;

    } else {

       new_state = SIFD_COPI_ILKN_STATE_OPERATIONAL; 
    }

    /* no need to check major bidirectional state - just do it! */

    /* Configure Interlaken Core Reset bits */
    rc = sifd_ilkn_util_core_enable(handle, ilkn_inst, new_state);

    PMC_RETURN(rc);
} /* sifd_ilkn_enable */

/*******************************************************************************
*  sifd_ilkn_rx_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configure the INTERLAKEN instance to
*                  transition between 'Major Bidirectional States' of:\n
*                  2.  Block in reset with configuration registers maintaining
*                      their last initialized value\n
*                  3.  Block in operational state with the ability to send/
*                      receive user data or SERDES test patterns.
*
*                  Setting enable = 1 moves from state 2 to 3.  Setting
*                  enable = 0 allows an interface to move from state 3 to
*                  state 2.\n\n
*
*                  It is assumed that the CBRI instance is in state 2 when
*                  calling this function with enable = 1 and in state 3 when
*                  calling this function with enable = 0.  If these
*                  relationships are not trun SIFD_LOG_ERR_INVALID_STATE is
*                  returned.\n\n
* 
*                  Must be called after:\n
*                    sifd_mux_data_path_cfg\n
*                    sifd_ilkn_rx_chnl_cfg\n
*                    sifd_ilkn_tx_chnl_cfg
*
*  INPUTS:
*                  handle    - pointer to sifd handle instance to be operated on
*                  ilkn_inst - the specific ILKN instance.\n
*                                SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                                SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  enable    - enable/disable flag\n
*                              0 : disable\n
*                              1 : enable
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_rx_enable(sifd_handle_t *handle, 
                                     sifd_ilkn_inst_t ilkn_inst,
                                     sifd_ilkn_en_t enable)
{
    PMC_ERROR rc = SIFD_LOG_ERR_UNKNOWN;
    sifd_ilkn_state_t new_state;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, SIFD_ERR_CODE_ASSERT, 0, 0);
                    
    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) || (enable > SIFD_ILKN_ENABLE)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, enable);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Interlaken interface must be configured to enable */
    if (handle->var.ilkn_state[ilkn_inst] == PMC_BLOCK_DEFAULT) {
        if (enable == SIFD_ILKN_DISABLE)         /* OK to disable unconfigured interface */
            PMC_RETURN(PMC_SUCCESS);
        else                                     /* unconfigured interface cannot be enabled */
            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* determine desired state */
    if (enable == SIFD_ILKN_DISABLE) {

       new_state = SIFD_COPI_ILKN_STATE_STANDBY;

    } else {

       new_state = SIFD_COPI_ILKN_STATE_OPERATIONAL; 
    }

    /* no need to check major bidirectional state - just do it! */

    /* Configure Interlaken Core Reset bits */
    rc = sifd_ilkn_util_core_rx_enable(handle, ilkn_inst, new_state);

    PMC_RETURN(rc);
} /* sifd_ilkn_rx_enable */

/*******************************************************************************
*  sifd_ilkn_rx_reset_toggle
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*   Toggle sifd ilkn 1 and/or 2 RX reset.
*   
* INPUTS:
*   *handle               - pointer to sifd handle instance to be operated on
*   *sifd_1_cfg_ptr       - pointer to Interlaken 1 configuration
*   *sifd_2_cfg_ptr       - pointer to Interlaken 2 configuration
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_rx_reset_toggle(sifd_handle_t *handle, 
                                           sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                           sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    BOOL ilkn1_enabled, ilkn2_enabled;

    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, SIFD_ERR_CODE_ASSERT, 0, 0);
    ilkn1_enabled = (copi_field_IL_RX_SOFT_get(NULL, handle, SIFD_COPI_ILKN1) == 0);
    ilkn2_enabled = (copi_field_IL_RX_SOFT_get(NULL, handle, SIFD_COPI_ILKN2) == 0);
                    
    /* Disable RX ILKN block if it was previously enabled */
    if (sifd_1_cfg_ptr != NULL && ilkn1_enabled)
    {
        result = sifd_ilkn_util_core_rx_enable(handle, SIFD_COPI_ILKN1, SIFD_COPI_ILKN_STATE_STANDBY);
    }
   
    if (result == PMC_SUCCESS &&
        sifd_2_cfg_ptr != NULL && ilkn2_enabled)
    {
        result = sifd_ilkn_util_core_rx_enable(handle, SIFD_COPI_ILKN2, SIFD_COPI_ILKN_STATE_STANDBY);
    }

    /* Re-enable RX ILKN block if it was disabled above */ 
    if (result == PMC_SUCCESS &&
        sifd_1_cfg_ptr != NULL && ilkn1_enabled)
    {
        result = sifd_ilkn_util_core_rx_enable(handle, SIFD_COPI_ILKN1, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    }
   
    if (result == PMC_SUCCESS &&
        sifd_2_cfg_ptr != NULL && ilkn2_enabled)
    {
        result = sifd_ilkn_util_core_rx_enable(handle, SIFD_COPI_ILKN2, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    }

    PMC_RETURN(result);
} /* sifd_ilkn_rx_reset_toggle */

/*******************************************************************************
* sifd_serdes_subsystem_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   clock_enable          - set to one of:\n
*                           SIFD_CBRC_CLK_ENABLE\n
*                           SIFD_OTN_CLK_ENABLE\n
*                           SIFD_ENET_CLK_ENABLE\n
*                           or to SIFD_CLK_DISABLE in order to disable.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_serdes_subsystem_set(sifd_handle_t *sifd_handle, 
                                            UINT32 serdes_lane, 
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 clk_value = 0;
    BOOL clk_enable = FALSE;

    PMC_ENTRY();

    if ((sifd_serdes_lane_param_check(serdes_lane) == PMC_SUCCESS) &&
        (sifd_internal_lane_param_check(subsystem_lane) == PMC_SUCCESS)) {

        UINT8 dsis_lane_flag = SIFD_DSIS_LANE_DISABLE;
        UINT8 clock_lane_flag = SIFD_CLK_DISABLE;

        /* en/disable receive clock */
        sifd_serdes_dsis_rx_en_set(sifd_handle, subsystem_lane, clock_enable);

        if (clock_enable == SIFD_CLK_DISABLE) {

            clk_value = ~clock_enable;
            clk_enable = FALSE;

        } else {

            clk_value = clock_enable;
            clk_enable = TRUE;
        }

        sifd_dsis_tx_clk_set(sifd_handle, subsystem_lane, clock_enable, clk_enable);

        /* en/disable transmit on SERDES lane */
        if(clock_enable != SIFD_CLK_DISABLE)
        {
            dsis_lane_flag = SIFD_DSIS_LANE_ENABLE;
            clock_lane_flag = 1;
            sifd_dft_field_N1_TX_LANE_SEL_set(NULL, sifd_handle, 
                                              serdes_lane, 
                                              SIFD_N1_LANE_SELECT_DSIS);

        }
        sys_dsis_field_range_TX_LANE_ENABLE_set(NULL, sifd_handle, 
                                            serdes_lane, serdes_lane, 
                                            clock_lane_flag);
        sifd_dft_field_DSIS_TX_LANE_DIS_set(NULL, sifd_handle, 
                                            serdes_lane, dsis_lane_flag);
        sifd_dft_field_DSIS_RX_LANE_DIS_set(NULL, sifd_handle, 
                                            serdes_lane, dsis_lane_flag);
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_serdes_subsystem_set */

/*******************************************************************************
* sifd_subsystem_lane_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   clock_enable          - set to one of:\n
*                           SIFD_CBRC_CLK_ENABLE\n
*                           SIFD_OTN_CLK_ENABLE\n
*                           SIFD_ENET_CLK_ENABLE
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_subsystem_lane_enable(sifd_handle_t *sifd_handle, 
                                             UINT32 serdes_lane, 
                                             UINT32 subsystem_lane,
                                             UINT32 clock_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    rc = sifd_serdes_subsystem_set(sifd_handle, 
                                   serdes_lane, 
                                   subsystem_lane,
                                   clock_enable);

    PMC_RETURN(rc);
} /* sifd_subsystem_lane_enable */

/*******************************************************************************
*  sifd_ilkn_get_chnl_counts
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function retrieves per channel counter statistics for
*                a transmit or receive channel.
*
*                The channel being monitored needs to have been initialized
*                with sifd_ilkn_chnl_cnt_cfg for counter accesses to be 
*                valid.
*
*  INPUTS:
*                 *handle     - base address
*                 ilkn_inst   - instance of ILKN block to configure.\n
*                               SIFD_COPI_ILKN1\n
*                               SIFD_COPI_ILKN2\n
*                 dir          - Direction of travel associated with counters:\n
*                               SIFD_ILKN_TX_DIRECTION : Transmit Chnl Counters\n
*                               SIFD_ILKN_RX_DIRECTION : Receive Chnl Counters
*                 mon_chnl     - Channel for counter access.  Must be set to 
*                                the same mon_chnl as initialized with
*                                sifd_ilkn_chnl_cnt_cfg. Legal range: 0-149
*                 *p_byte_count  - pointer to byte count data.
*                 *p_pkt_count   - pointer to packet count data.
*                 *p_err_count   - pointer to error count data.
*                 *p_b_max_count - pointer to burst max count data.
*                 *p_b_min_count - pointer to burst min count data.
*                 *p_byte_status   - Byte counter status\n
*                                  bit 3 RD_BOVER\n
*                                  bit 2 RD_BHALF\n
*                                  bit 1 RD_BUNCORR\n
*                                  bit 0 RD_BCORR
*                 *p_pkt_status    - Packet counter status\n
*                                  bit 3 RD_POVER\n
*                                  bit 2 RD_PHALF\n
*                                  bit 1 RD_PUNCORR\n
*                                  bit 0 RD_PCORR
*                 *p_err_status    - Error counter status\n
*                                  bit 3 RD_EOVER\n
*                                  bit 2 RD_EHALF\n
*                                  bit 1 RD_EUNCORR\n
*                                  bit 0 RD_ECORR
*                 *p_b_max_status   - Burst Max counter status\n
*                                  bit 3 RD_EOVER\n
*                                  bit 2 RD_EHALF\n
*                                  bit 1 RD_EUNCORR\n
*                                  bit 0 RD_ECORR
*                 *p_b_min_status   - Burst Min counter status\n
*                                  bit 3 RD_EOVER\n
*                                  bit 2 RD_EHALF\n
*                                  bit 1 RD_EUNCORR\n
*                                  bit 0 RD_ECORR
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_get_chnl_counts(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst,
                                           sifd_ilkn_dir_t dir, 
                                           UINT32 mon_chnl, 
                                           UINT64 *p_byte_count, 
                                           UINT64 *p_pkt_count, 
                                           UINT64 *p_err_count, 
                                           UINT64 *p_b_max_count, 
                                           UINT64 *p_b_min_count, 
                                           UINT32 *p_byte_status, 
                                           UINT32 *p_pkt_status, 
                                           UINT32 *p_err_status, 
                                           UINT32 *p_b_max_status, 
                                           UINT32 *p_b_min_status)
{
    /* varible declaration */
    ilkn_buffer_t b_ptr[1];
    UINT32 b_cnt_hi = 0, b_cnt_lo = 0;
    UINT32 p_cnt_hi = 0, p_cnt_lo = 0;
    UINT32 err_cnt_hi = 0, err_cnt_lo = 0;
    UINT32 b_max_cnt_hi = 0, b_max_cnt_lo = 0;
    UINT32 b_min_cnt_hi = 0, b_min_cnt_lo = 0;
    UINT32 b_over_stat = 0, b_half_stat = 0, b_un_corr = 0, b_corr = 0;
    UINT32 p_over_stat = 0, p_half_stat = 0, p_un_corr = 0, p_corr = 0;
    UINT32 e_over_stat = 0, e_half_stat = 0, e_un_corr = 0, e_corr = 0;
    UINT32 b_max_over_stat = 0, b_max_half_stat = 0, b_max_un_corr = 0, b_max_corr = 0;
    UINT32 b_min_over_stat = 0, b_min_half_stat = 0, b_min_un_corr = 0, b_min_corr = 0;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) || (dir > SIFD_ILKN_RX_DIRECTION) || 
        (mon_chnl > (SIFD_ILKN_LINK_FLOW_CONTROL - 1))) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, dir);
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, mon_chnl, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }    
    
    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Set CMD to b'001 to read */
    ilkn_field_CMD_set(b_ptr, handle, ilkn_inst, dir, 1); 

    /* Set TYPE to b'0000 to get Byte counter, Packet counter and Error packet counter*/
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 0);

    /* Set ADDR to user specified channel */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, mon_chnl);

    ilkn_buffer_flush(b_ptr);

    /* Extract Counter Data and Overflow Status */
    /* Byte Counter Read */
    b_cnt_lo = ilkn_field_LSB_BC_RDATA_get(NULL, handle, ilkn_inst, dir);
    b_cnt_hi = ilkn_field_MSB_BC_RDATA_get(NULL, handle, ilkn_inst, dir);
 
    /* Extract status of Byte Counter */   
    b_over_stat = ilkn_field_RD_BOVER_get(NULL, handle, ilkn_inst, dir);
    
    b_half_stat = ilkn_field_RD_BHALF_get(NULL, handle, ilkn_inst, dir);

    b_un_corr = ilkn_field_RD_BUNCORR_get(NULL, handle, ilkn_inst, dir);

    b_corr = ilkn_field_RD_BCORR_get(NULL, handle, ilkn_inst, dir);

    /* Packet Counter Read */ 
    p_cnt_lo = ilkn_field_LSB_PC_RDATA_get(NULL, handle, ilkn_inst, dir);
    p_cnt_hi = ilkn_field_MSB_PC_RDATA_get(NULL, handle, ilkn_inst, dir);
    
    /* Extract status information for Packet Counter */    
    p_over_stat = ilkn_field_RD_POVER_get(NULL, handle, ilkn_inst, dir);
    
    p_half_stat = ilkn_field_RD_PHALF_get(NULL, handle, ilkn_inst, dir);

    p_un_corr = ilkn_field_RD_PUNCORR_get(NULL, handle, ilkn_inst, dir);

    p_corr = ilkn_field_RD_PCORR_get(NULL, handle, ilkn_inst, dir);

    /* Error Counter Read */
    err_cnt_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
    err_cnt_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

    e_over_stat = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);
    
    e_half_stat = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

    e_un_corr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

    e_corr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);

    /* Extract BMAX_CNT and BMIN_CNT data */
    if (dir == SIFD_ILKN_TX_DIRECTION) {

        /* M = Tx, set MAX_DIS and MIN_DIS to 1 */
        ilkn_field_MAX_DIS_set(NULL, handle, ilkn_inst, dir, 1);
        ilkn_field_MIN_DIS_set(NULL, handle, ilkn_inst, dir, 1);
    }
    else if (dir == SIFD_ILKN_RX_DIRECTION) {

        /* M = Rx, set MAX_DIS and MIN_DIS to 0 */
        ilkn_field_MAX_DIS_set(NULL, handle, ilkn_inst, dir, 0);
        ilkn_field_MIN_DIS_set(NULL, handle, ilkn_inst, dir, 0);

        /* Set TYPE to b'0011 to read BMAX_CNT */
        ilkn_field_ACC_TYPE_set(NULL, handle, ilkn_inst, dir, 0x3);

        /* Read Error counter for BMAX_CNT */
        b_max_cnt_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
        b_max_cnt_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

        b_max_over_stat = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);
        
        b_max_half_stat = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

        b_max_un_corr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

        b_max_corr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);

        /* Set TYPE to b'0100 to read BMIN_CNT data */
        ilkn_field_ACC_TYPE_set(NULL, handle, ilkn_inst, dir, 0x4);

        /* Read Error counter for BMIN_CNT */
        b_min_cnt_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
        b_min_cnt_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

        b_min_over_stat = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);
        
        b_min_half_stat = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

        b_min_un_corr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

        b_min_corr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);
    }    
    /* Return values */

    *p_byte_count =  b_cnt_hi;
    *p_byte_count =  (*p_byte_count) << 32;
    *p_byte_count += b_cnt_lo;  

    *p_pkt_count =  p_cnt_hi;
    *p_pkt_count =  (*p_pkt_count) << 32;
    *p_pkt_count += p_cnt_lo;  

    *p_err_count =  err_cnt_hi;
    *p_err_count =  (*p_err_count) << 32;
    *p_err_count += err_cnt_lo;  

    *p_b_max_count =  b_max_cnt_hi;
    *p_b_max_count =  (*p_b_max_count) << 32;
    *p_b_max_count += b_max_cnt_lo;

    *p_b_min_count =  b_min_cnt_hi;
    *p_b_min_count =  (*p_b_min_count) << 32;
    *p_b_min_count += b_min_cnt_lo; 

    *p_byte_status = (b_over_stat << 3) + (b_half_stat << 2) + 
                   (b_un_corr << 1) + b_corr;

    *p_pkt_status = (p_over_stat << 3) + (p_half_stat << 2) + 
                  (p_un_corr << 1) + p_corr;

    *p_err_status = (e_over_stat << 3) + (e_half_stat << 2) + 
                  (e_un_corr << 1) + e_corr;
    
    *p_b_max_status = (b_max_over_stat << 3) + (b_max_half_stat << 2) +
                   (b_max_un_corr  << 1) + b_max_corr; 


    *p_b_min_status = (b_min_over_stat << 3) + (b_min_half_stat << 2) +
                    (b_min_un_corr  << 1) + b_min_corr;


     /* Set CMD to b'100 to clear the counter */  
    ilkn_field_CMD_set(b_ptr, handle, ilkn_inst, dir, 4); 

    /* Set TYPE to b'0011 to clear up BMAX_CNT*/
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 3);
    
    /* Set ADDR to user specified channel */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, mon_chnl);

    ilkn_buffer_flush(b_ptr);  

    /* Set TYPE to b'0100 to clear up BMIN_CNT*/
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 4);
    
    /* Set ADDR to user specified channel */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, mon_chnl);

    ilkn_buffer_flush(b_ptr);  
    
    /* Set TYPE to b'0000 to clear up Byte counter, Packet counter and Error packet counter */
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 0);

    /* Set ADDR to user specified channel */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, mon_chnl);

    ilkn_buffer_flush(b_ptr);  
    
    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_get_chnl_counts */

/*******************************************************************************
*  sifd_ilkn_get_rx_intf_counts
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function retrieves receive interface counters
*
*
*  INPUTS:
*                 *handle     - base address
*                 ilkn_inst   - instance of ILKN block to configure.\n
*                               SIFD_COPI_ILKN1\n
*                               SIFD_COPI_ILKN2\n
*                 *p_ib_fc_err_cnt - pointer to IB_FC_ERR count
*                 *p_crc24_cnt  - pointer to CRC24_CNT count
*                 *p_ib_fc_stat    - pointer to IB_FC_ERR counter status\n
*                                     bit 3 RD_EOVER\n
*                                     bit 2 RD_EHALF\n
*                                     bit 1 RD_EUNCORR\n
*                                     bit 0 RD_ECORR
*                 *p_crc24_stat    - pointer to CRC24 counter status\n
*                                     bit 3 RD_EOVER\n
*                                     bit 2 RD_EHALF\n
*                                     bit 1 RD_EUNCORR\n
*                                     bit 0 RD_ECORR
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_get_rx_intf_counts(sifd_handle_t *handle, 
                                              sifd_ilkn_inst_t ilkn_inst,
                                              UINT64 *p_ib_fc_err_cnt, 
                                              UINT64 *p_crc24_cnt, 
                                              UINT32 *p_ib_fc_stat, 
                                              UINT32 *p_crc24_stat)
{
    /* variable declaration */
    ilkn_buffer_t b_ptr[1];
    UINT32 ib_fc_cnt_hi, ib_fc_cnt_lo;
    UINT32 crc24_cnt_hi, crc24_cnt_lo;
    UINT32 ib_fc_over_stat, ib_fc_half_stat, ib_fc_uncorr, ib_fc_corr;
    UINT32 crc24_over_stat, crc24_half_stat, crc24_uncorr, crc24_corr;

    /* Set Direction to be RX for both of these sets of counters */
    UINT32 dir = SIFD_ILKN_RX_DIRECTION;

    PMC_ENTRY();

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }
    
    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Set CMD to b'011 */
    ilkn_field_CMD_set(b_ptr, handle, ilkn_inst, dir, 0x3); 

    /* Set TYPE to b'0001 for IB_FC_ERR count */
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 0x1);

    /* Set ADDR to 0x0 for both IB_FC_ERR_CNT and CRC24_CNT */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, 0x0);

    ilkn_buffer_flush(b_ptr);

    /* Read Error counter for IB_FC_ERR_CNT */
    ib_fc_cnt_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
    ib_fc_cnt_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

    /* Extract status information for IB_FC_ERR_CNT */
    ib_fc_over_stat = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);

    ib_fc_half_stat = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

    ib_fc_uncorr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

    ib_fc_corr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);

    /* Set TYPE to b'0010 for CRC24_CNT */
    ilkn_field_ACC_TYPE_set(NULL, handle, ilkn_inst, dir, 0x2);

    /* Read CRC24_CNT */
    /* Extract status information for CRC24_CNT */  
    crc24_cnt_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
    crc24_cnt_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

    /* Extract status information for IB_FC_ERR_CNT */
    crc24_over_stat = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);

    crc24_half_stat = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

    crc24_uncorr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

    crc24_corr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);

    *p_ib_fc_err_cnt =  ib_fc_cnt_hi;
    *p_ib_fc_err_cnt =  (*p_ib_fc_err_cnt) << 32;
    *p_ib_fc_err_cnt += ib_fc_cnt_lo;  

    *p_crc24_cnt =  crc24_cnt_hi;
    *p_crc24_cnt = (*p_crc24_cnt) << 32;
    *p_crc24_cnt += crc24_cnt_lo; 

    *p_ib_fc_stat = (ib_fc_over_stat << 3) + (ib_fc_half_stat << 2) + 
                    (ib_fc_uncorr << 1) + ib_fc_corr;

    *p_crc24_stat = (crc24_over_stat << 3) + (crc24_half_stat << 2) +
                    (crc24_uncorr << 1) + crc24_corr;
       
    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_get_rx_intf_counts */

/*******************************************************************************
*  sifd_ilkn_get_serdes_lane_counts
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function retrieves per lane counter statistics for
*                one of the 24 supported SERDES lanes.
*
*  INPUTS:
*                 *handle     - base address
*                 ilkn_inst   - instance of ILKN block to configure.\n
*                               SIFD_COPI_ILKN1\n
*                               SIFD_COPI_ILKN2\n
*                 lane         - Serdes Lane:
*                                0 to 23 
*                 counter      - Counter to extract:\n
*                                0101: BURST_CNT\n
*                                0110: ALIGN_CNT\n
*                                0111: ALIGN_FAIL_CNT\n
*                                1000: WRD_SYNC_CNT\n
*                                1001: CDR_CNT\n
*                                1010: CRC32_CNT_FILL\n
*                                1011: BADCTL_CNT
*               *p_cnt_val     - Retrieved Counter value
*               *p_cnt_stat    - pointer to Counter status\n
*                                 bit 3 RD_EOVER\n
*                                 bit 2 RD_EHALF\n
*                                 bit 1 RD_EUNCORR\n
*                                 bit 0 RD_ECORR
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_get_serdes_lane_counts(sifd_handle_t *handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  UINT32 lane, 
                                                  UINT32 counter, 
                                                  UINT64 *p_cnt_val, 
                                                  UINT32 *p_cnt_stat)
{
    /* variable declaration */
    ilkn_buffer_t b_ptr[1];
    UINT32 cnt_stat_hi, cnt_stat_lo;
    UINT32 cnt_stat_eover, cnt_stat_ehalf, cnt_stat_uncorr, cnt_stat_ecorr;

    /* Set Direction to be RX for both of these sets of counters */
    UINT32 dir = SIFD_ILKN_RX_DIRECTION;

    PMC_ENTRY();
    
    /* argument checking */
    if ( (lane > 23) || (counter > 11) || (ilkn_inst > SIFD_COPI_ILKN2) ) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, lane, counter);
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Set CMD to b'011 */
    ilkn_field_CMD_set(b_ptr, handle, ilkn_inst, dir, 0x3); 

    /* Set TYPE to user specified counter */
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, counter);

    /* Set ADDR to user specified serdes lane */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, lane);

    ilkn_buffer_flush(b_ptr);

    /* Extract status information for CRC24_CNT */  
    cnt_stat_lo = ilkn_field_LSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);
    cnt_stat_hi = ilkn_field_MSB_EC_RDATA_get(NULL, handle, ilkn_inst, dir);

    /* Extract status information for IB_FC_ERR_CNT */
    cnt_stat_eover = ilkn_field_RD_EOVER_get(NULL, handle, ilkn_inst, dir);

    cnt_stat_ehalf = ilkn_field_RD_EHALF_get(NULL, handle, ilkn_inst, dir);

    cnt_stat_uncorr = ilkn_field_RD_EUNCORR_get(NULL, handle, ilkn_inst, dir);

    cnt_stat_ecorr = ilkn_field_RD_ECORR_get(NULL, handle, ilkn_inst, dir);

    *p_cnt_val =  cnt_stat_hi;
    *p_cnt_val =  (*p_cnt_val) << 32;
    *p_cnt_val += cnt_stat_lo;  

    *p_cnt_stat = (cnt_stat_eover << 3) + (cnt_stat_ehalf << 2) +
                  (cnt_stat_uncorr << 1) + cnt_stat_ecorr;

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_get_serdes_lane_counts */

/*******************************************************************************
* FUNCTION: sifd_ilkn_prbs_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves the prbs_monitor synchroniation state for all STS-1 paths.
*
* INPUTS:
*   *handle            - device handle
*   ilkn_inst          - instance of ILKN block to access.\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*
* OUTPUTS:
*    *status           - when /= 0 RX PRBS checker is locked
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_prbs_status_get(sifd_handle_t          *handle, 
                                           sifd_ilkn_inst_t        ilkn_inst,
                                           UINT32                 *status)
{
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(NULL != handle, SIFD_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(SIFD_COPI_ILKN2 >= ilkn_inst,  SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != status, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    *status = ilkn_field_RX_WRD_SYNC_LANE_get(NULL, handle, ilkn_inst);

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_prbs_status_get */

/*******************************************************************************
*  sifd_ilkn_chnl_cnt_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function configures the channel to monitor for per
*                channel statistics.  This function must only be called once
*                for a channel and the channel number used with 
*                sifd_ilkn_get_chnl_counts in order for counter accesses 
*                to be valid.\n\n
*
*                This function will call sifd_ilkn_get_chnl_counts to 
*                clear stale counter data that would represent the previous 
*                channel monitored and the new channel presently being 
*                monitored.
*
*
*  INPUTS:
*                 *handle     - base address     
*                 ilkn_inst   - instance of ILKN block to configure.\n
*                               SIFD_COPI_ILKN1\n
*                               SIFD_COPI_ILKN2\n
*                 dir        - Direction of travel associated with counters:\n
*                               SIFD_ILKN_TX_DIRECTION : Transmit Chnl Counters\n
*                               SIFD_ILKN_RX_DIRECTION : Receive Chnl Counters
*                 mon_chnl   - PIF channel to be monitored 0 to 149
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_chnl_cnt_cfg(sifd_handle_t *handle, 
                                        sifd_ilkn_inst_t ilkn_inst,
                                        sifd_ilkn_dir_t dir,
                                        UINT32 mon_chnl)
{

    ilkn_buffer_t b_ptr[1];
    UINT32 result;
    UINT32 data;
    UINT64 data64;

    PMC_ENTRY();

    /* argument checking */
    if ( (ilkn_inst > SIFD_COPI_ILKN2) || (dir > SIFD_ILKN_RX_DIRECTION) || 
         (mon_chnl > (SIFD_ILKN_LINK_FLOW_CONTROL - 1)) ) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, dir);
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, mon_chnl, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }    

    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Set CMD to b'000 */
    ilkn_field_CMD_set(b_ptr, handle, ilkn_inst, dir, 0 );
    
    /* Set TYPE to b'0000 */
    ilkn_field_ACC_TYPE_set(b_ptr, handle, ilkn_inst, dir, 0);

    /* Set ADDR to user specified channel */
    ilkn_field_ACC_ADDR_set(b_ptr, handle, ilkn_inst, dir, mon_chnl);

    ilkn_buffer_flush(b_ptr);

    /* access counter to clear stale data */
    result = sifd_ilkn_get_chnl_counts(handle, ilkn_inst, dir, mon_chnl, 
                                       &data64,  &data64, &data64, &data64, &data64, 
                                       &data,  &data, &data, &data, &data);
    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_chnl_cnt_cfg */

/*******************************************************************************
*  sifd_ilkn_test_control_state_set
*  ___________________________________________________________________________
*
*  DESCRIPTION: 
*               This function sets the test control state for PRBS and
*               pattern generation.
*
*  INPUTS:
*   *sifd_handle        - device handle
*   ilkn_inst           - instance of ILKN block to configure.\n
*                           SIFD_COPI_ILKN1\n
*                           SIFD_COPI_ILKN2\n
*   dir                 - Direction of data
*   control_state       - Test control state to set
*
*  OUTPUTS:
*   None 
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_test_control_state_set(sifd_handle_t * sifd_handle,
                                                   UINT32 ilkn_inst,
                                                   UINT32 dir,
                                                   sifd_ilkn_test_control_state_t control_state)
{
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;
    PMC_ERROR ret = PMC_SUCCESS;

    PMC_ENTRY();

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(sifd_handle, 
                                     (sifd_ilkn_inst_t)ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(sifd_handle, (sifd_ilkn_inst_t)ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(sifd_handle, (sifd_ilkn_inst_t)ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

    switch (control_state)
    {
        case SIFD_ILKN_TEST_NORMAL_OPERATION:
            ilkn_field_PRBS_EN_set(NULL, sifd_handle, ilkn_inst, dir, 0);
            ilkn_field_PROG_EN_set(NULL, sifd_handle, ilkn_inst, dir, 0);
        break;

        case SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN:
            if(sifd_handle->var.test_control_state[ilkn_inst][dir] == SIFD_ILKN_TEST_NORMAL_OPERATION)
            {
                ilkn_field_PRBS_EN_set(NULL, sifd_handle, ilkn_inst, dir, 0);
                ilkn_field_PROG_EN_set(NULL, sifd_handle, ilkn_inst, dir, 1);
            }
            else
                ret = SIFD_LOG_ERR_TEST_STATE_TRANSITION;
        break;

        case SIFD_ILKN_TEST_PRBS:
            if(sifd_handle->var.test_control_state[ilkn_inst][dir] == SIFD_ILKN_TEST_NORMAL_OPERATION)
            {

            ilkn_field_PRBS_EN_set(NULL, sifd_handle, ilkn_inst, dir, 1);
            ilkn_field_PROG_EN_set(NULL, sifd_handle, ilkn_inst, dir, 0);
            }
            else
                ret = SIFD_LOG_ERR_TEST_STATE_TRANSITION;

        break;

        case SIFD_ILKN_TEST_PRBS_INV:
            if(sifd_handle->var.test_control_state[ilkn_inst][dir] == SIFD_ILKN_TEST_NORMAL_OPERATION)
            {

            ilkn_field_PRBS_EN_set(NULL, sifd_handle, ilkn_inst, dir, 1);
            ilkn_field_PROG_EN_set(NULL, sifd_handle, ilkn_inst, dir, 1);
            }
            else
                ret = SIFD_LOG_ERR_TEST_STATE_TRANSITION;

        break;

        default:
            ret = SIFD_LOG_ERR_INVALID_PARAMETERS;


    }

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(sifd_handle, (sifd_ilkn_inst_t)ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(sifd_handle, (sifd_ilkn_inst_t)ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    if(ret == PMC_SUCCESS)
        sifd_handle->var.test_control_state[ilkn_inst][dir] = control_state;
    
    PMC_RETURN(ret);
} /* sifd_ilkn_test_control_state_set */

/*******************************************************************************
*  sifd_ilkn_test_control_state_get
*  ___________________________________________________________________________
*
*  DESCRIPTION: 
*               This function gets the test control state for PRBS and
*               pattern generation.
*                 
*
*  INPUTS:
*   *sifd_handle        - device handle
*   ilkn_inst           - instance of ILKN block to configure.\n
*                           SIFD_COPI_ILKN1\n
*                           SIFD_COPI_ILKN2\n
*   dir                 - Direction of data
*
*  OUTPUTS:
*   None
*
*  RETURNS:        
*   control_state       - Test control state to get
*
*  NOTES:
*
*******************************************************************************/
PRIVATE sifd_ilkn_test_control_state_t sifd_ilkn_test_control_state_get(sifd_handle_t * sifd_handle,
                                                                        UINT32 ilkn_inst,
                                                                        UINT32 dir)
{
    PMC_ENTRY();
    UINT32 prog_en, prbs_en;
    sifd_ilkn_test_control_state_t test_control_state = SIFD_ILKN_TEST_UNDEFINED;

    prog_en = ilkn_field_PROG_EN_get(NULL, sifd_handle, ilkn_inst, dir);
    prbs_en = ilkn_field_PRBS_EN_get(NULL, sifd_handle, ilkn_inst, dir);

    if(prbs_en == 0 && prog_en == 0)
    {
        test_control_state = SIFD_ILKN_TEST_NORMAL_OPERATION;
    } 
    else if(prbs_en == 0 && prog_en == 1)
    {
        test_control_state = SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN;
    }
    else if(prbs_en == 1 && prog_en == 0)
    {
        test_control_state = SIFD_ILKN_TEST_PRBS;
    }
    else if(prbs_en == 1 && prog_en == 1)
    {
        test_control_state = SIFD_ILKN_TEST_PRBS_INV;
    }

    PMC_RETURN(test_control_state);
} /* sifd_ilkn_test_control_state_t */

/*******************************************************************************
*  sifd_ilkn_test_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function configures the Interlaken Serdes test 
*                generator or detector.  The modes defined in enum 
*                sifd_ilkn_test_control_state_t are supported within this
*                function.\n\n
*
*                For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN, up to 2 - 32 bit patterns
*                may be generated or detected.  The programmable pattern is  
*                formed as follows:\n\n
*
*                Pattern A * Repetition A, followed by Pattern B * Repetition B\n\n
*
*                When Repetion N(A or B) is set to 0, Pattern N is not
*                included in the expected pattern stream.\n\n
*
*                This function modified the reset state of the entire interface
*                to ensure register configuration can be applied as defined in the
*                register description.
*
*
*  INPUTS:
*                 *sifd_handle  - device handle
*                 ilkn_inst     - instance of ILKN block to access.\n
*                                 COPI_ILKN1 (0) : ILKN1\n
*                                 COPI_ILKN2 (1) : ILKN2
*                 dir           - Direction of travel associated with counters:\n
*                                 SIFD_ILKN_TX_DIRECTION : Transmit test pattern Gen\n
*                                 SIFD_ILKN_RX_DIRECTION : Receive test pattern Detector
*                 test_mode     - Enum defining the type of traffic generation to be done.
*                 pattern_a     - For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN 32 bit pattern to be repeated  
*                                 rep_a times, For PRBS or PRBS_INV, one of the values
*                                 in enum sifd_patt_mode_t must be used.
*                 pattern_b     - For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN 32 bit pattern to be repeated  
*                                rep_b times
*                 rep_a         - For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN number of time to repeat 
*                                 pattern_a.  Must be set to 1 for PRBS and PRBS_INV pattern
*                                 generation.  Otherwise set to 0.
*                 rep_b         - For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN number of time to repeat 
*                                 pattern_b.  If not to be repeated set to 0.
*
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_test_cfg(sifd_handle_t     *sifd_handle, 
                                    sifd_ilkn_inst_t   ilkn_inst,
                                    sifd_ilkn_dir_t    dir,
                                    sifd_ilkn_test_control_state_t test_mode,
                                    UINT32             pattern_a, 
                                    UINT32             pattern_b,
                                    UINT8              rep_a, 
                                    UINT8              rep_b)
{
    ilkn_buffer_t b_ptr[1];
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(NULL != sifd_handle, SIFD_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(SIFD_COPI_ILKN2 >= ilkn_inst,  SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(SIFD_ILKN_RX_DIRECTION >= dir, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
 
    PMC_ERROR result = PMC_SUCCESS;

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(sifd_handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    switch (dir)
    {
    case SIFD_ILKN_RX_DIRECTION:
        SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(sifd_handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
        break;
    case SIFD_ILKN_TX_DIRECTION:
        SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(sifd_handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
        break;
    default:
        break;
    }

    ilkn_buffer_init( b_ptr, sifd_handle, ilkn_inst );
    

    if (sifd_handle->var.ilkn_state[(UINT32)ilkn_inst] == PMC_BLOCK_DEFAULT)
    {
        result = SIFD_LOG_ERR_INVALID_STATE;
    }

    if(result == PMC_SUCCESS)
    {
        switch (test_mode)
        {
            case SIFD_ILKN_TEST_NORMAL_OPERATION:
            break;

            case SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN:

                /* set REPA and REPB */
                ilkn_field_REPA_set(b_ptr, sifd_handle, ilkn_inst, (UINT32)dir, rep_a);
                ilkn_field_REPB_set(b_ptr, sifd_handle, ilkn_inst, (UINT32)dir, rep_b);

                /* set TST_PTRN_VALUE for Pattern A, B and C */
                ilkn_field_TST_PTRN_A_VALUE_set(b_ptr, sifd_handle, ilkn_inst, 
                                                dir, pattern_a);
                ilkn_field_TST_PTRN_B_VALUE_set(b_ptr, sifd_handle, ilkn_inst, 
                                                dir, pattern_b);
                
                ilkn_buffer_flush(b_ptr);    

                break;

            case SIFD_ILKN_TEST_PRBS:
            case SIFD_ILKN_TEST_PRBS_INV:
                if(result == PMC_SUCCESS && rep_a != 0)
                    result = sifd_ilkn_validate_prbs_pattern(sifd_handle, pattern_a);
                    
                if(result == PMC_SUCCESS)
                    ilkn_field_TST_PTRN_A_VALUE_set(NULL, sifd_handle, ilkn_inst, (UINT32)dir, pattern_a);

                if(result == PMC_SUCCESS)
                    ilkn_field_TST_PTRN_B_VALUE_set(NULL, sifd_handle, ilkn_inst, (UINT32)dir, 0);

            break;

            default:
                result = SIFD_LOG_ERR_INVALID_PARAMETERS;

        }
    }
    
    /* Set PRBS_EN and PROG_EN */
    if(result == PMC_SUCCESS)
        result = sifd_ilkn_test_control_state_set(sifd_handle,
                                              ilkn_inst,
                                              dir,
                                              test_mode);

    /* re-enable ILKN instance if it was enabled before the begining of the configuration */
    switch (dir)
    {
    case SIFD_ILKN_RX_DIRECTION:
        SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(sifd_handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
        break;
    case SIFD_ILKN_TX_DIRECTION:
        SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(sifd_handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
        break;
    default:
        break;
    }

    PMC_RETURN(result);

} /* sifd_ilkn_test_cfg */

/*******************************************************************************
*  sifd_ilkn_test_cfg_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:  
*                This function gets config of the Interlaken Serdes test 
*                generator or detector.  The modes defined in enum 
*                sifd_ilkn_test_control_state_t are supported within this
*                function.\n\n
*
*
*  INPUTS:
*                 *sifd_handle  - device handle
*                 ilkn_inst     - instance of ILKN block to access.\n
*                                 COPI_ILKN1 (0) : ILKN1\n
*                                 COPI_ILKN2 (1) : ILKN2
*                 dir           - Direction of travel associated with counters:\n
*                                 SIFD_ILKN_TX_DIRECTION : Transmit test pattern Gen\n
*                                 SIFD_ILKN_RX_DIRECTION : Receive test pattern Detector
*
*
*  OUTPUTS:
*                 test_mode     - Enum defining the type of traffic generation to be done.
*                 pattern_a     - For SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN 32 bit pattern to be repeated  
*                                 rep_a times, For PRBS or PRBS_INV, one of the values
*                                 in enum sifd_patt_mode_t must be used.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_test_cfg_get(sifd_handle_t     *sifd_handle, 
                                        sifd_ilkn_inst_t   ilkn_inst,
                                        sifd_ilkn_dir_t    dir,
                                        sifd_ilkn_test_control_state_t *test_mode,
                                        UINT32             *pattern_a)
{
    PMC_ERROR result = PMC_SUCCESS;
    ilkn_buffer_t b_ptr[1];

    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(NULL != sifd_handle, SIFD_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(SIFD_COPI_ILKN2 >= ilkn_inst,  SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(SIFD_ILKN_RX_DIRECTION >= dir, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (sifd_handle->var.ilkn_state[(UINT32)ilkn_inst] == PMC_BLOCK_DEFAULT)
    {
        result = SIFD_LOG_ERR_INVALID_STATE;
    }

    /* Get test_mode */
    *test_mode = sifd_ilkn_test_control_state_get(sifd_handle, ilkn_inst, dir);

    ilkn_buffer_init( b_ptr, sifd_handle, ilkn_inst );

    switch (*test_mode)
    {
        case SIFD_ILKN_TEST_NORMAL_OPERATION:
            break;

        case SIFD_ILKN_TEST_PROGRAMMABLE_PATTERN:
            /* set TST_PTRN_VALUE for Pattern A, B and C */
            *pattern_a = ilkn_field_TST_PTRN_A_VALUE_get(b_ptr,
                                                         sifd_handle,
                                                         ilkn_inst, 
                                                         dir);
            ilkn_buffer_flush(b_ptr);
            break;

        case SIFD_ILKN_TEST_PRBS:
        case SIFD_ILKN_TEST_PRBS_INV:
            *pattern_a = ilkn_field_TST_PTRN_A_VALUE_get(NULL,
                                                         sifd_handle,
                                                         ilkn_inst,
                                                         (UINT32)dir);
            break;

        default:
            break;                
    }

    PMC_RETURN(result);
} /* sifd_ilkn_test_cfg_get */

/*******************************************************************************
* sifd_ilkn_validate_prbs_pattern
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Ensures the pattern is supported by the COPI block 
*
* INPUTS:
*   *sifd_handle            - device handle
*   prbs_seed               - pattern to validate
*
* OUTPUTS:
*    None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_validate_prbs_pattern(sifd_handle_t * sifd_handle, UINT32 prbs_seed)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    if(prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_7 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_11 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_15 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_23 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_31 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_9 &&
       prbs_seed != (UINT32) SIFD_PATT_MODE_PRBS_29)
        result = SIFD_LOG_ERR_PRBS_MODE_NOT_FOUND;

    PMC_RETURN(result);

} /* sifd_ilkn_validate_prbs_pattern */

/*******************************************************************************
* sifd_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables a SIFD loopback.
*
* INPUTS:
*   *handle     - sifd handle
*   ilkn_inst   - ILKN block to access: COPI_ILKN1 or COPI_ILKN2
*   loopback    - type of loopback - see sifd_loopback_cfg_t
*
*                                                            
* OUTPUTS:
*      None
*                                                            
* RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   Note that flow control loopback is not implemented and is always
*   set to FALSE.
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_loopback_cfg(sifd_handle_t *handle, 
                                   sifd_ilkn_inst_t ilkn_inst,
                                   sifd_loopback_cfg_t loopback)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    sifd_loopback_cfg_t prev_loopback;

    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT((ilkn_inst == SIFD_COPI_ILKN1 || ilkn_inst == SIFD_COPI_ILKN2), 
               SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    prev_loopback = handle->var.loopback[ilkn_inst];

    /* The current loopback state should be disabled when enabling a loopback */
    if (loopback != SIFD_DISABLE_LOOPBACK && prev_loopback != SIFD_DISABLE_LOOPBACK)
    {
        PMC_RETURN(SIFD_LOG_ERR_LOOPBACK);
    }

    result = sifd_ilkn_util_loopback_set(handle, ilkn_inst, loopback, FALSE);

    /* track the loopback state */
    if (result == PMC_SUCCESS)
    {
        handle->var.loopback[ilkn_inst] = loopback;
    }
    PMC_RETURN(result);
} /* sifd_loopback_cfg */

/*******************************************************************************
* sifd_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the status of the SIFD loopback. The loopback_ptr 
*   will be set to SIFD_LOOPBACK_DISABLED if loopback is disabled.
*
* INPUTS:
*   *handle         - sifd handle
*   ilkn_inst       - ILKN block to access: COPI_ILKN1 or COPI_ILKN2
*
*                                                            
* OUTPUTS:
*   *loopback_ptr   - type of loopback - see sifd_loopback_cfg_t
*                                                            
* RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_loopback_status_get(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst,
                                          sifd_loopback_cfg_t *loopback_ptr)
{
    PMC_ENTRY();
    /* argument checking */
    PMC_ASSERT(handle != NULL, SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT((ilkn_inst == SIFD_COPI_ILKN1 || ilkn_inst == SIFD_COPI_ILKN2), 
               SIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    *loopback_ptr = handle->var.loopback[ilkn_inst];
    PMC_RETURN(PMC_SUCCESS);

} /* sifd_loopback_status_get */

/*******************************************************************************
*  sifd_flow_control_calendar_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Iterates over the sifd flow control calendar to determine which entries
*   are in use. For each LINK or ununsed value updates the passed in TX or
*   RX calendar entry. For each entry that is found the actual ILKN channel
*   is then searched in the associated rx/tx bcw_dpi_map for the index of
*   that value in the map. The index is then updated in the assocaited rx/tx
*   ilkn_fc_ptr map. The ilkn_fc_ptr calendars were originally provided by
*   the client when the ILKN channel flow control calendar was configured.
*  
*
*  INPUTS:
*    *handle    - pointer to sifd handle instance to be operated on
*    ilkn_inst  - The specific ILKN instance.\n
*                 SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                 SIFD_COPI_ILKN2 Interlaken 2 Instance
*    *tx_bcw_dpi_map - Transmit channels in use for the particular ILKN
*                      interface.
*    *rx_bcw_dpi_map - Received channels in use for the particular ILKN
*                      interface
*
*  OUTPUTS:
*    *tx_ilkn_fc_ptr  - Storage for TX side re-engineered flow control 
*                       calendar
*    *rx_ilkn_fc_ptr  - Storage for RX side re-engineered flow control 
*                       calendar
*
*  RETURNS:
*    None
*
*******************************************************************************/
PUBLIC void sifd_flow_control_calendar_get(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst,
                                           UINT8 *tx_bcw_dpi_map,
                                           UINT8 *rx_bcw_dpi_map,
                                           sifd_ilkn_calendar_t *tx_ilkn_fc_ptr,
                                           sifd_ilkn_calendar_t *rx_ilkn_fc_ptr)
{
    UINT16 i;

    PMC_ENTRY();

    for (i = 0; i < SIFD_ILKN_MAX_CAL_ENTRIES; i++) 
    {
        if (SIFD_ILKN_NULL_CAL_ENTRY == handle->var.ilkn_fc[ilkn_inst].tx_flow_control_calendar.entry[i] || 
            SIFD_ILKN_LINK_FLOW_CONTROL == handle->var.ilkn_fc[ilkn_inst].tx_flow_control_calendar.entry[i])
        {
            /*
             * Assign the value from the tx_flow_control_calendar held for
             * this ILKN Instance if the value is either LINK or unset(FF)
             */
            tx_ilkn_fc_ptr->entry[i] = handle->var.ilkn_fc[ilkn_inst].tx_flow_control_calendar.entry[i];
        }
        else
        {
            /*
             * Else retrieve the value from the SIFD flow control calendar
             * The value represents the ILKN channel that is using the 
             * flow control calendar entry that is being recreated. To get
             * that entry searched through the bcw_dpi_map to find the
             * index of that the channel is used in and then assign it to 
             * the index of the updated flow control calendar.
             * Since the values may be repeated index of the bcw dpi entry
             * only needs to be found once and assigned at different 
             * points.
             */
            UINT16 value = handle->var.ilkn_fc[ilkn_inst].tx_flow_control_calendar.entry[i];
            UINT16 k;

            for (k=0; k < UTIL_GLOBAL_ILKN_ARB_DPI_CHANS_MAX; k++)
            {
                if (value == tx_bcw_dpi_map[k])
                {
                    tx_ilkn_fc_ptr->entry[i] = k;
                    break;
                }
            }
        }
        /*
         * Repeat for RX side
         */
        if (SIFD_ILKN_NULL_CAL_ENTRY == handle->var.ilkn_fc[ilkn_inst].rx_flow_control_calendar.entry[i] || 
            SIFD_ILKN_LINK_FLOW_CONTROL == handle->var.ilkn_fc[ilkn_inst].rx_flow_control_calendar.entry[i])
        {
            rx_ilkn_fc_ptr->entry[i] = handle->var.ilkn_fc[ilkn_inst].rx_flow_control_calendar.entry[i];
        }
        else
        {
            UINT16 value = handle->var.ilkn_fc[ilkn_inst].rx_flow_control_calendar.entry[i];
            UINT16 k;

            for (k=0; k < UTIL_GLOBAL_ILKN_ARB_DPI_CHANS_MAX; k++)
            {
                if (value == rx_bcw_dpi_map[k])
                {
                    rx_ilkn_fc_ptr->entry[i] = k;
                    break;
                }
            }
        }
    }

    PMC_RETURN();
} /* sifd_flow_control_calendar_get */

/*******************************************************************************
*  sifd_flow_control_calendar_array_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   return the sifd_flow_control_calendar.entry as UINT8 array
*   with SIFD_ILKN_MAX_CAL_ENTRIES (300) entries.
*
*
*  INPUTS:
*    *handle    - pointer to sifd handle instance to be operated on
*    ilkn_inst  - The specific ILKN instance.\n
*                 SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                 SIFD_COPI_ILKN2 Interlaken 2 Instance
*
*  OUTPUTS:
*    *tx_sifd_fc_ptr  - Storage for TX side flow control calendar,
*                       or NULL if TX side not needed
*    *rx_sifd_fc_ptr  - Storage for RX side flow control calendar,
*                       or NULL if RX side not needed
*
*  RETURNS:
*    None
*
*******************************************************************************/
PUBLIC void sifd_flow_control_calendar_array_get(sifd_handle_t *handle,
                                                 sifd_ilkn_inst_t ilkn_inst,
                                                 sifd_ilkn_calendar_t **tx_sifd_fc_ptr,
                                                 sifd_ilkn_calendar_t **rx_sifd_fc_ptr)
{

    PMC_ENTRY();

    *tx_sifd_fc_ptr = &(handle->var.ilkn_fc[ilkn_inst].tx_flow_control_calendar);
    *rx_sifd_fc_ptr = &(handle->var.ilkn_fc[ilkn_inst].rx_flow_control_calendar);

    PMC_RETURN();
} /* sifd_flow_control_calendar_array_get */


/*******************************************************************************
* sifd_serdes_dsis_rx_sel_and_rx_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the rx clock and data lines per subsystem lane and
*   then en/disable the lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   internal_rx_lane      - the internal rx lane to use.
*   clock_enable          - set to one of:\n
*                           SIFD_CBRC_CLK_ENABLE\n
*                           SIFD_OTN_CLK_ENABLE\n
*                           SIFD_ENET_CLK_ENABLE\n
*                           or to SIFD_CLK_DISABLE in order to disable.
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
PUBLIC void sifd_serdes_dsis_rx_sel_and_rx_en_set(sifd_handle_t *sifd_handle, 
                                                  UINT32 subsystem_lane,
                                                  UINT32 internal_rx_lane,
                                                  UINT32 clock_enable)
{
    PMC_ENTRY();

    /* Note these are void functions so there is no need to check a return
     * code */
    sifd_serdes_dsis_rx_sel_set(sifd_handle, subsystem_lane, internal_rx_lane);
    sifd_serdes_dsis_rx_en_set(sifd_handle, subsystem_lane, clock_enable);

    PMC_RETURN();
} /* sifd_serdes_dsis_rx_sel_and_rx_en_set */

/*******************************************************************************
* sifd_serdes_dft_dsis_rx_lane_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Enable or disable one lane on DSIS Receive interface.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   enable                - 0: Enable the lane\n
*                           1: Disable the lane
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
PUBLIC void sifd_serdes_dft_dsis_rx_lane_dis_set(sifd_handle_t *sifd_handle, 
                                                 UINT32         subsystem_lane,
                                                 UINT32         enable)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != sifd_handle, SIFD_LOG_ERR_NULL_HANDLE, 0, 0);
    /* enable or disable receive interface of subsystem lanes */
    sifd_dft_field_DSIS_RX_LANE_DIS_set(NULL, sifd_handle, subsystem_lane, enable);

    PMC_RETURN();
} /* sifd_serdes_dft_dsis_rx_lane_dis_set */

/*******************************************************************************
* sifd_ilkn_dlos_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function configures the per-lane SERDES to Interlaken DLOS.
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   *sifd_1_cfg_ptr       - pointer to Interlaken 1 configuration
*   *sifd_2_cfg_ptr       - pointer to Interlaken 2 configuration
*   dlos_enable           - DLOS enable or disable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
* 
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_dlos_cfg(sifd_handle_t     *h,
                                    sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                    sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                    BOOL8              dlos_enable)
{

    PMC_ENTRY();

    PMC_ERROR result = PMC_SUCCESS;
    BOOL ilkn1_enabled, ilkn2_enabled;
    UINT32 lane_los_invert;
    UINT32 lane_los_enable;

    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);

    ilkn1_enabled = (copi_field_IL_RX_SOFT_get(NULL, h, SIFD_COPI_ILKN1) == 0);
    ilkn2_enabled = (copi_field_IL_RX_SOFT_get(NULL, h, SIFD_COPI_ILKN2) == 0);

    /* Disable the block if it was previously enabled */
    if (sifd_1_cfg_ptr != NULL && ilkn1_enabled)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN1,SIFD_ILKN_DISABLE);

    if (sifd_2_cfg_ptr != NULL && ilkn2_enabled && result == PMC_SUCCESS)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN2,SIFD_ILKN_DISABLE);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    /* LOS processing should always be enabled for all active ILKN lanes */
    if (sifd_1_cfg_ptr != NULL)
    {
        lane_los_enable = FALSE == dlos_enable ? 0 : SIFD_ILKN_1_DLOS_EN_MASK_GET(sifd_1_cfg_ptr->lane);

        /* N1 serdes are always inverted. E1 and N2 are always not inverted */
        lane_los_invert = FALSE == dlos_enable ? ~SIFD_ILKN_1_DLOS_INV_MASK_GET(sifd_1_cfg_ptr->lane) & sifd_1_cfg_ptr->lane.blk_to_N1_enable_mask : SIFD_ILKN_1_DLOS_INV_MASK_GET(sifd_1_cfg_ptr->lane);

        /* now configure LOS */
        sifd_ilkn1_dlos_cfg(h, sifd_1_cfg_ptr->ilkn.lane_mask, lane_los_invert, lane_los_enable);
    }

    if (sifd_2_cfg_ptr != NULL)
    {
        lane_los_enable = FALSE == dlos_enable ? 0 : SIFD_ILKN_2_DLOS_EN_MASK_GET(sifd_2_cfg_ptr->lane);

        /* N1 serdes are always inverted. E1 and N2 are always not inverted */
        lane_los_invert = FALSE == dlos_enable ? ~SIFD_ILKN_2_DLOS_INV_MASK_GET(sifd_2_cfg_ptr->lane) & sifd_2_cfg_ptr->lane.blk_to_N1_enable_mask : SIFD_ILKN_1_DLOS_INV_MASK_GET(sifd_2_cfg_ptr->lane);

        /* now configure LOS */
        sifd_ilkn2_dlos_cfg(h, sifd_2_cfg_ptr->ilkn.lane_mask, lane_los_invert, lane_los_enable);
    }

    /* Re-enable block if it was disabled above */ 
    if (sifd_1_cfg_ptr != NULL && ilkn1_enabled)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN1,SIFD_ILKN_ENABLE);
   
    if (sifd_2_cfg_ptr != NULL && ilkn2_enabled && result == PMC_SUCCESS)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN2,SIFD_ILKN_ENABLE);

    PMC_RETURN(result);
} /* sifd_ilkn_dlos_cfg */

/* LCOV_EXCL_START */
/*******************************************************************************
* sifd_ilkn_toggle_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Toggle sifd ilkn 1 and/or 2 resets.
*   
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   *sifd_1_cfg_ptr       - pointer to Interlaken 1 configuration
*   *sifd_2_cfg_ptr       - pointer to Interlaken 2 configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*
*   This function is used by the BP T8/C8 serdes diagnostic loopback.
* 
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_toggle_reset(sifd_handle_t *h,
                                        sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                        sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr)
{

    PMC_ENTRY();

    PMC_ERROR result = PMC_SUCCESS;
    BOOL ilkn1_enabled, ilkn2_enabled;
    PMC_ASSERT(NULL != h, SIFD_ERR_CODE_ASSERT, 0, 0);

    ilkn1_enabled = (copi_field_IL_RX_SOFT_get(NULL, h, SIFD_COPI_ILKN1) == 0);
    ilkn2_enabled = (copi_field_IL_RX_SOFT_get(NULL, h, SIFD_COPI_ILKN2) == 0);

    /* Disable the block if it was previously enabled */
    if (sifd_1_cfg_ptr != NULL && ilkn1_enabled)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN1,SIFD_ILKN_DISABLE);

    if (sifd_2_cfg_ptr != NULL && ilkn2_enabled && result == PMC_SUCCESS)
        result |= sifd_ilkn_enable(h,SIFD_COPI_ILKN2,SIFD_ILKN_DISABLE);

    if (result != PMC_SUCCESS)
    {
        PMC_RETURN(result);
    }

    /* Re-enable block if it was disabled above */ 
    if (sifd_1_cfg_ptr != NULL && ilkn1_enabled)
        result = sifd_ilkn_enable(h,SIFD_COPI_ILKN1,SIFD_ILKN_ENABLE);
   
    if (sifd_2_cfg_ptr != NULL && ilkn2_enabled && result == PMC_SUCCESS)
        result |= sifd_ilkn_enable(h,SIFD_COPI_ILKN2,SIFD_ILKN_ENABLE);

    PMC_RETURN(result);
} /* sifd_ilkn_toggle_reset */
/* LCOV_EXCL_STOP */

/******************************************************************************/
/* END FEATURES FUNCTIONS - Items previously in sifd_features.c               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
* sifd_audit_var_data_register_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sifd_var_t structure data from reading device registe space.\n\n
*
*   Not all fields are updated.
*
*
* INPUTS:
*   *sifd_handle                 - pointer to sifd handle instance
*
* OUTPUTS:
*   *var_ptr                    - pointer to sifd_var_t type structure with 
*                                 subelements defined
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_audit_var_data_register_get(sifd_handle_t *sifd_handle,
                                                   sifd_var_t *var_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, j, temp_entry;
    sifd_loopback_cfg_t temp_loopback = SIFD_DISABLE_LOOPBACK;

    PMC_ENTRY();

    /* Recover flow control calendars */
    for(i=0;i<SIFD_MAX_ILKN_INTERFACE;i++)
    {
        for (j = 0; j < sifd_handle->var.ilkn_fc[i].calendar_length; j++)
        {
            result = sifd_ilkn_util_cal_read(sifd_handle, 
                                             (sifd_ilkn_inst_t)i, 
                                             SIFD_ILKN_TX_DIRECTION,
                                             j, 
                                             &temp_entry);
            if(result!=PMC_SUCCESS)
                PMC_RETURN(result);

            var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j] = temp_entry;

            result = sifd_ilkn_util_cal_read(sifd_handle, 
                                             (sifd_ilkn_inst_t)i, 
                                             SIFD_ILKN_RX_DIRECTION,
                                             j, 
                                             &temp_entry);
            if(result!=PMC_SUCCESS)
                PMC_RETURN(result);

            var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j] = temp_entry;            
            
        }

    }

    /* Recover loopback config */
    for(i=0;i<SIFD_MAX_ILKN_INTERFACE;i++)
    {
        result = sifd_ilkn_util_loopback_get(sifd_handle,
                                             (sifd_ilkn_inst_t)i,
                                             &temp_loopback);  
        if(result!=PMC_SUCCESS)
            PMC_RETURN(result);
        
        var_ptr->loopback[i] = temp_loopback;

    }

    /* Recover test pattern config */
    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {    
        for (j = 0; j < SIFD_NUM_ILKN_DIR_MAX; j++)
        {
            var_ptr->test_control_state[i][j] = sifd_ilkn_test_control_state_get(sifd_handle,i,j);
        }
    }

  
    PMC_RETURN(result);
} /* sifd_audit_var_data_register_get */

/*******************************************************************************
* sifd_audit_ctxt_register_compare_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compared context with register space.  It will identify
*   if there is a mismatch.\n\n
*
*   Only recoverable and relevant fields from device registers are compared.\n\n
*
*   Results of comparison output with mismatch_*_ptr parameters.
*   Logical meaning:\n
*        TRUE:  Mismatch present\n
*        FALSE: No mismatch present
*
*   In the case of crash restart mode it will attempt to correct a mismatch
*   by returning the mismatch back to defualt state in the register and
*   context space.\n
*
*
* INPUTS:
*   *sifd_handle          - pointer to SIFD handle instance.
*   *ctxt_var_ptr         - pointer to Context view of device configuration
*   *register_var_ptr     - pointer to register space recovered view of context
*   *restart_init_cfg_ptr - pointer to restart init config
*
* OUTPUTS:         
*   *mismatch_tx_fc_cal_ptr        - pointer to ILKN1 flow control calendar
*                                  mismatch status
*   *mismatch_rx_fc_cal_ptr        - pointer to ILKN2 flow control calendar
*                                  mismatch status
*   *mismatch_loopback_ptr       - pointer to loopback config mismatch status
*   *mismatch_pattern_ptr        - pointer to test pattern config mismatch
*                                  status
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_audit_ctxt_register_compare_clean(sifd_handle_t *sifd_handle,
                                                         sifd_var_t *ctxt_var_ptr,
                                                         sifd_var_t *register_var_ptr,
                                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                         BOOL8 *mismatch_tx_fc_cal_ptr,
                                                         BOOL8 *mismatch_rx_fc_cal_ptr,
                                                         BOOL8 *mismatch_loopback_ptr,
                                                         BOOL8 *mismatch_pattern_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i,j;

    PMC_ENTRY();
    
    *mismatch_tx_fc_cal_ptr = FALSE;
    *mismatch_rx_fc_cal_ptr = FALSE;
    *mismatch_loopback_ptr = FALSE;
    *mismatch_pattern_ptr = FALSE;

    /* Compare flow control calendars */
    for(i=0;i<SIFD_MAX_ILKN_INTERFACE;i++)
    {
        if(sifd_handle->var.ilkn_state[i] == PMC_BLOCK_DEFAULT)
            continue;

        for (j = 0; j < sifd_handle->var.ilkn_fc[i].calendar_length; j++)
        {
                PMC_LOG_TRACE("sifd_audit_ctxt_register_compare_clean fc entry tx, i = %u, j = %u, channel: %u\n",
                              i,
                              j,
                              ctxt_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j]);
                PMC_LOG_TRACE("sifd_audit_ctxt_register_compare_clean fc entry rx, i = %u, j = %u, channel: %u\n",
                              i,
                              j,
                              ctxt_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j]);


            if(ctxt_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j] !=
               register_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j])
            {
                PMC_LOG_TRACE("sifd_audit_ctxt_register_compare_clean tx fc mismatch, i = %u, j = %u, ctxt = %u, reg = %u\n",
                i,
                j,
                ctxt_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j],
                register_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j]);

                *mismatch_tx_fc_cal_ptr = TRUE;
                if(!restart_init_cfg_ptr->is_warm_restart)
                {
                    ctxt_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
                    register_var_ptr->ilkn_fc[i].tx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
                }

            }
            if(ctxt_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j] !=
               register_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j])
            {
                PMC_LOG_TRACE("sifd_audit_ctxt_register_compare_clean rx fc mismatch, i = %u, j = %u, ctxt = %u, reg = %u\n",
                i,
                j,
                ctxt_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j],
                register_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j]);

                *mismatch_rx_fc_cal_ptr = TRUE;
                if(!restart_init_cfg_ptr->is_warm_restart)
                {
                    ctxt_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
                    register_var_ptr->ilkn_fc[i].rx_flow_control_calendar.entry[j] = SIFD_ILKN_NULL_CAL_ENTRY;
                }

            }

        }
    }

    /* Compare loopback settings */
    for(i=0;i<SIFD_MAX_ILKN_INTERFACE;i++)
    {
        if(sifd_handle->var.ilkn_state[i] == PMC_BLOCK_DEFAULT)
            continue;

        if(ctxt_var_ptr->loopback[i] != register_var_ptr->loopback[i])
        {
            if( (ctxt_var_ptr->loopback[i] == SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK &&
                 register_var_ptr->loopback[i] == SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK) ||
                (ctxt_var_ptr->loopback[i] == SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK &&
                 register_var_ptr->loopback[i] == SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK))
            {
                /* These two loopback modes are the same on the SIFD level */
            }
            else
            {
                *mismatch_loopback_ptr = TRUE;
                if(!restart_init_cfg_ptr->is_warm_restart)
                {
                    ctxt_var_ptr->loopback[i] = SIFD_DISABLE_LOOPBACK;
                    register_var_ptr->loopback[i] = SIFD_DISABLE_LOOPBACK;
                }
            }
        }
    }

    /* Compare test pattern settings */
    for (i = 0; i < SIFD_MAX_ILKN_INTERFACE; i++)
    {
        if(sifd_handle->var.ilkn_state[i] == PMC_BLOCK_DEFAULT)
            continue;

        for (j = 0; j < SIFD_NUM_ILKN_DIR_MAX; j++)
        {
            if(ctxt_var_ptr->test_control_state[i][j] != 
               register_var_ptr->test_control_state[i][j])
            {
                *mismatch_pattern_ptr = TRUE;
                if(!restart_init_cfg_ptr->is_warm_restart)
                {
                    sifd_ilkn_test_control_state_set(sifd_handle, i, j,SIFD_ILKN_TEST_NORMAL_OPERATION);
                    register_var_ptr->test_control_state[i][j] = SIFD_ILKN_TEST_NORMAL_OPERATION;
                }
            }

            if(ctxt_var_ptr->test_control_state[i][j] != 
               register_var_ptr->test_control_state[i][j])
            {
                *mismatch_pattern_ptr = TRUE;
                if(!restart_init_cfg_ptr->is_warm_restart)
                {
                    sifd_ilkn_test_control_state_set(sifd_handle, i, j,SIFD_ILKN_TEST_NORMAL_OPERATION);                   
                    register_var_ptr->test_control_state[i][j] = SIFD_ILKN_TEST_NORMAL_OPERATION;
                }
            }

        }
    }

    PMC_RETURN(result);

} /* sifd_audit_ctxt_register_compare_clean */

/* ILKN1 */

/*******************************************************************************
* sifd_ilkn1_serdes_lane_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes data
*   sources associated with the ILKN 1 specified lane.\n\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must be called multiple times.\n\n
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - Denotes which ILKN1 lane is to be acted upon and
*                           enabled.
*   serdes_type           - enum, the type of serdes that this interlaken lane 
*                           group is connected to:\n
*                           SIFD_SRDS_E1_LR\n
*                           SIFD_SRDS_N1_SR\n
*                           SIFD_SRDS_N2_SR
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken1 may connect to are:\n
* ______________________________________________________________________________\n
*\n
*   SIFD_SRDS_E1_LR: The E1 Serdes, Lanes[39:20]\n
*   SIFD_SRDS_N1_SR: The N1 Serdes, Lanes[15:00]\n
*   SIFD_SRDS_N2_SR: The N2 Serdes, Lanes[08:00]\n
*\n
* Interlaken1 Lanes can be mapped to the following serdes lanes\n
*\n
*  Interlaken1 Lanes 23 through 16:\n
*\code
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 ------------------------------------------------[20][21][23][23]\n
*     N1 ----------------------------------------------------------------\n
*     N2 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [24][25][26][27][28][29][30][31][32][33][34][35][36][37][38][39]\n
*     N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn1_serdes_lane_cfg(sifd_handle_t *h, 
                                             UINT32 lane, 
                                             sifd_srds_type_t serdes_type)
{
    PMC_ERROR rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
    UINT32 serdes_value = SIFD_ILKN_N1_SELECT;
    UINT32 lane_enable_state = SIFD_ILKN_LANE_DISABLE;

    PMC_ENTRY();
    
    /* generate src_serdes value */
    /* generate src_serdes lane state */
    /* generate return code */
    switch (serdes_type) {

    case SIFD_SRDS_E1_LR:

        /* Can only map 20 lanes to E1 */
        if (lane < SIFD_MAX_E1_TO_ILKN1) {

            serdes_value = SIFD_ILKN1_E1_SELECT; 
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    case SIFD_SRDS_N1_SR:

        /* Can only map 16 lanes to N1 */
        if (lane < SIFD_MAX_N1_TO_ILKN1) {

            serdes_value = SIFD_ILKN_N1_SELECT;
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    case SIFD_SRDS_N2_SR:

        /* Can only map lanes 16-23 to N2 */
        if ((lane < SIFD_MAX_N2_TO_ILKN1) && (lane > SIFD_MIN_N2_TO_ILKN1)){

            serdes_value = SIFD_ILKN1_N2_SELECT;
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    default:
        /* Invalid Parameters */
        break;
    }

    if (rc == PMC_SUCCESS) {

        /* Set rx/tx SERDES select */
        sifd_dft_field_IL1_RX_SERDES_SEL_set (NULL, h, lane, serdes_value );
        sifd_dft_field_IL1_TX_SERDES_SEL_set (NULL, h, lane, serdes_value );

        /* Enable/Disable last, if we're bothering to config. */
        sifd_ilkn1_single_lane_disable_set(h, lane, lane_enable_state);

    }

    PMC_RETURN(rc);

} /* sifd_ilkn1_serdes_lane_cfg */

/*******************************************************************************
* sifd_ilkn1_single_lane_disable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Level 1 - Known BLK -> known Lane\n
*   This is a wrapper for a register write to disable Interlaken1 TX and RX 
*   given a lane number to operate on.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*
*   lane                  - the RX and TX lane number to disable
*   state                 - the state to put the lane in.\n
*                           0 : SIFD_ILKN_LANE_ENABLE\n
*                           1 : SIFD_ILKN_LANE_DISABLE           
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken1 lanes that may be disabled are:\n
* ______________________________________________________________________________\n
*
*  Interlaken1 Lanes 23 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*******************************************************************************/
PRIVATE void sifd_ilkn1_single_lane_disable_set(sifd_handle_t *h, 
                                                UINT32 lane, 
                                                UINT32 state)
{
    PMC_ENTRY();
    
    sifd_dft_field_IL1_RX_LANE_DIS_set(NULL, h, lane, state);
    sifd_dft_field_IL1_TX_LANE_DIS_set(NULL, h, lane, state);
    PMC_RETURN();   
} /* sifd_ilkn1_single_lane_disable_set */

/*******************************************************************************
* sifd_ilkn1_dlos_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the value for DLOS_INV and DLOS_EN registers for 
*   Interlaken1 on the specified lanes.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - denotes which ILKN1 lane is to be acted upon and
*                           enabled.
*                           See note for which bits in the bit mask are 
*                           valid. The LSB representing ILKN1 Lane 0, the MSB
*                           represents ILKN1 Lane 23
*   b_dlos_invert         - DLOS inversion for all of ILKN1 lanes 
*                           (1 to invert and 0 for no inversion)
*   b_dlos_enable         - DLOS enable for all of ILKN1 lanes 
*                           (1 to enable and 0 to disable)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken1 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken1 Lanes 31 through 16:\n
\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn1_dlos_cfg(sifd_handle_t *h, 
                                 UINT32 b_lane_mask,
                                 UINT32 b_dlos_invert,
                                 UINT32 b_dlos_enable)
{
    UINT32 i;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; ++i) {

        if ((b_lane_mask & SIFD_ILKN_ENABLE) == TRUE) {

            /* Configure the DLOS settings */
            sifd_ilkn1_dlos_lane_cfg(h, i, (b_dlos_invert & 1), (b_dlos_enable & 1));
        }

        b_lane_mask >>= 1;
        b_dlos_invert >>= 1;
        b_dlos_enable >>= 1;
    }

    PMC_RETURN();
} /* sifd_ilkn1_dlos_cfg */

/*******************************************************************************
* sifd_ilkn1_dlos_lane_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Level 1 - Known BLK -> known Lane\n
*   This is a wrapper for a register write to set the DLOS_INV and DLOS_EN
*   registers for Interlaken1 on the specified lane.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - the lane to operate on.
*   dlos_invert           - DLOS inversion (1 to invert, 0 for no inversion)
*   dlos_enable           - DLOS enable (1 to enable, 0 to disable)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken1 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken1 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn1_dlos_lane_cfg(sifd_handle_t *h, 
                                      UINT32 lane, 
                                      UINT32 dlos_invert,
                                      UINT32 dlos_enable)
{
    PMC_ENTRY();

    /* generate dlos / dlos inv value */
    sifd_dft_field_IL1_DLOS_INV_set( NULL, h, lane, dlos_invert);
    sifd_dft_field_IL1_DLOS_EN_set ( NULL, h, lane, dlos_enable);
    PMC_RETURN();
} /* sifd_ilkn1_dlos_lane_cfg */

/*******************************************************************************
* sifd_ilkn1_serdes_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes signal
*   sources associated with the ILKN 1 specified lane.\n\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must be called multiple times.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - Denotes the set of lanes within serdes_type that
*                           are enabled for use with Interlaken 1 
*                           enabled. \n
*                           See note for which bits in the bit mask are 
*                           valid. The LSB representing ILKN1 Lane 0, the MSB
*                           represents ILKN1 Lane 23 \n
*   serdes_type           - enum, the type of serdes that this interlaken lane 
*                           group is connected to. \n
*                           SIFD_SRDS_E1_LR \n
*                           SIFD_SRDS_N1_SR \n
*                           SIFD_SRDS_N2_SR \n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken1 may connect to are:\n
* ______________________________________________________________________________\n
*
*   SIFD_SRDS_E1_LR: The E1 Serdes, Lanes[39:20]\n
*   SIFD_SRDS_N1_SR: The N1 Serdes, Lanes[15:00]\n
*   SIFD_SRDS_N2_SR: The N2 Serdes, Lanes[07:00]\n
*\n
* Interlaken1 Lanes can be mapped to the following serdes lanes\n
*\n
*  Interlaken1 Lanes 23 through 16:\n
*\code
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 ------------------------------------------------[20][21][23][23]\n
*     N1 ----------------------------------------------------------------\n
*     N2 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [24][25][26][27][28][29][30][31][32][33][34][35][36][37][38][39]\n
*     N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn1_serdes_select(sifd_handle_t *h, 
                                           UINT32 b_lane_mask,
                                           sifd_srds_type_t serdes_type)
{
    PMC_ERROR rc = PMC_SUCCESS;

    UINT32 i = 0;
    UINT8 lane_number;
    UINT32 set_value;

    UINT8 sifd_ilkn1_e1_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE};

    UINT8 sifd_ilkn1_n1_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE};

    UINT8 sifd_ilkn1_n2_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE,
    0, 1, 2, 3, 4, 5, 6, 7};

    PMC_ENTRY();

    for (i = 0; ((i < SIFD_MAX_ILKN_LANES) && (rc == PMC_SUCCESS)); 
                                          i++, b_lane_mask = b_lane_mask >> 1) {

        if ((b_lane_mask & SIFD_ILKN_ENABLE) == TRUE) {

            /* configure and enable the lane */
            rc = sifd_ilkn1_serdes_lane_cfg(h, i, serdes_type);

            switch (serdes_type) {
            case SIFD_SRDS_E1_LR:
                lane_number = sifd_ilkn1_e1_lane_map[i];
                if (lane_number != SIFD_NO_LANE) {
                    if (lane_number < 24)
                        set_value = SIFD_E1_LANE_SELECT_ILKN1_20_23;
                    else
                        set_value = SIFD_E1_LANE_SELECT_ILKN1_24_39;
                    sifd_e1_tx_lane_sel_lane_set(h, lane_number, set_value);
                }
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            case SIFD_SRDS_N1_SR:
                lane_number = sifd_ilkn1_n1_lane_map[i];
                if (lane_number != SIFD_NO_LANE)
                    sifd_n1_tx_lane_sel_lane_set(h, lane_number, SIFD_N1_LANE_SELECT_ILKN1);
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            case SIFD_SRDS_N2_SR:
                lane_number = sifd_ilkn1_n2_lane_map[i];
                if (lane_number != SIFD_NO_LANE)
                    sifd_n2_tx_lane_sel_lane_set(h, lane_number, SIFD_N2_LANE_SELECT_ILKN1);
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            default:
                rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            }

        }
    }

    PMC_RETURN(rc);
} /* sifd_ilkn1_serdes_select */

/*******************************************************************************
* sifd_ilkn1_lane_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the set of Rx and Tx ILKN1 lanes to be disabled.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - the RX and TX lane(s) to disable when bit in the 
*                           mask is set. See note for which bits in the bit mask
*                           are valid. The LSB representing ILKN1 Lane 0, the 
*                           MSB represents ILKN1 Lane 23
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken1 lanes that may be disabled are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken1 Lanes 23 through 16:\n
*\code
*  mask: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken1 Lanes 15 through 0:\n
*\code
*  mask: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn1_lane_disable(sifd_handle_t *h, UINT32 b_lane_mask)
{
    UINT32 i = 0;
    UINT32 lane_state = SIFD_ILKN_LANE_DISABLE;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; i++, b_lane_mask = b_lane_mask >> 1) {

        lane_state = b_lane_mask & SIFD_ILKN_LANE_DISABLE;

        /* disable or enable the lane */
        sifd_ilkn1_single_lane_disable_set(h, i, lane_state);
    }

    PMC_RETURN();
} /* sifd_ilkn1_lane_disable */


/* ILKN2 */

/*******************************************************************************
* sifd_ilkn2_serdes_lane_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes data
*   sources associated with the ILKN 2 specified lane.\n\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must be called multiple times.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - Denotes which ILKN2 lane is to be acted upon and
*                           enabled.
*   serdes_type           - the type of serdes that this interlaken lane is
*                           connected to.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken2 may connect to are:\n
* ______________________________________________________________________________\n
*\n
*   SIFD_SRDS_E1_LR: The E1 Serdes, Lanes[23:00]\n
*   SIFD_SRDS_N1_SR: The N1 Serdes, Lanes[15:08]\n
*   SIFD_SRDS_N2_SR: The N2 Serdes, Lanes[07:00]\n
*\n
*  Interlaken2 Lanes can be mapped to the following serdes lanes\n
*\n
*  Interlaken2 Lanes 23 through 16:\n
*\code
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     N1 ----------------------------------------------------------------\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N1 --------------------------------[15][14][13][12][11][10][ 9][ 8]\n
*     N2 [ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]--------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn2_serdes_lane_cfg(sifd_handle_t *h, 
                                             UINT32 lane, 
                                             sifd_srds_type_t serdes_type)
{
    PMC_ERROR rc = SIFD_LOG_ERR_INVALID_PARAMETERS;

    UINT32 serdes_value = SIFD_ILKN_N1_SELECT;
    UINT32 lane_enable_state = SIFD_ILKN_LANE_DISABLE;

    PMC_ENTRY();
    
    /* generate src_serdes value */
    /* generate src_serdes lane state */
    /* generate return code */
    switch (serdes_type) {

    case SIFD_SRDS_E1_LR:

        /* Can only map 24 lanes to E1 */
        if (lane < SIFD_MAX_E1_TO_ILKN2) {

            if (lane < 8)
                serdes_value = SIFD_ILKN2_E1_SELECT_0_7;
            else
                serdes_value = SIFD_ILKN2_E1_SELECT_8_23; 
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    case SIFD_SRDS_N1_SR:

        /* Can only map 8 lanes to N1 */
        if (lane < SIFD_MAX_N1_TO_ILKN2) {

            serdes_value = SIFD_ILKN_N1_SELECT;
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    case SIFD_SRDS_N2_SR:
        /* Can only map lanes 08-15 to N2 */
        if ((lane < SIFD_MAX_N2_TO_ILKN2) && (lane > SIFD_MIN_N2_TO_ILKN2)){

            serdes_value = SIFD_ILKN2_N2_SELECT;
            lane_enable_state = SIFD_ILKN_LANE_ENABLE;
            rc = PMC_SUCCESS;
        }
        break;
    default:
        /* Invalid Parameters */
        break;
    }

    if (rc == PMC_SUCCESS) {
        /* Set rx/tx SERDES select */
        sifd_dft_field_IL2_RX_SERDES_SEL_set (NULL, h, lane, serdes_value);
        sifd_dft_field_IL2_TX_SERDES_SEL_set (NULL, h, lane, serdes_value);

        /* Enable/Disable last, if we're bothering to config. */
        sifd_ilkn2_single_lane_disable_set(h, lane, lane_enable_state);
    }

    PMC_RETURN(rc);

} /* sifd_ilkn2_serdes_lane_cfg */

/*******************************************************************************
* sifd_ilkn2_single_lane_disable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a wrapper for a register write to disable Interlaken1 TX and RX 
*   given a lane number to operate on.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*
*   lane                  - the RX and TX lane number to disable
*   state                 - the state to put the lane in.\n
*                           0 : SIFD_ILKN_LANE_ENABLE\n
*                           1 : SIFD_ILKN_LANE_DISABLE           
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken2 lanes that may be disabled are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken2 Lanes 23 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*
*******************************************************************************/
PRIVATE void sifd_ilkn2_single_lane_disable_set(sifd_handle_t *h, 
                                                UINT32 lane,
                                                UINT32 state)
{
    PMC_ENTRY();
    
    sifd_dft_field_IL2_RX_LANE_DIS_set(NULL, h, lane, state);
    sifd_dft_field_IL2_TX_LANE_DIS_set(NULL, h, lane, state);
    PMC_RETURN(); 
} /* sifd_ilkn2_single_lane_disable_set */

/*******************************************************************************
* sifd_ilkn2_dlos_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the value for DLOS_INV and DLOS_EN registers for 
*   Interlaken2 on the specified lanes.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - denotes which ILKN2 lane is to be acted upon and
*                           enabled.
*                           See note for which bits in the bit mask are 
*                           valid. The LSB representing ILKN2 Lane 0, the MSB
*                           represents ILKN2 Lane 23
*   b_dlos_invert         - DLOS inversion for all of ILKN1 lanes 
*                           (1 to invert and 0 for no inversion)
*   b_dlos_enable         - DLOS enable for all of ILKN1 lanes 
*                           (1 to enable and 0 to disable)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken2 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken2 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn2_dlos_cfg(sifd_handle_t *h, 
                                 UINT32 b_lane_mask,
                                 UINT32 b_dlos_invert,
                                 UINT32 b_dlos_enable)
{
    UINT32 i;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; ++i) {

        if ((b_lane_mask & SIFD_ILKN_ENABLE) == TRUE) {

            /* Configure the DLOS settings */
            sifd_ilkn2_dlos_lane_cfg(h, i, (b_dlos_invert & 1), (b_dlos_enable & 1));
        }

        b_lane_mask >>= 1;
        b_dlos_invert >>= 1;
        b_dlos_enable >>= 1;
    }
    PMC_RETURN();
} /* sifd_ilkn2_dlos_cfg */

/*******************************************************************************
* sifd_ilkn2_dlos_lane_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a wrapper for a register write to set the DLOS_INV and DLOS_EN
*   registers for Interlaken2 on the specified lane.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - the lane to operate on.
*   dlos_invert           - DLOS inversion (1 to invert, 0 for no inversion)
*   dlos_enable           - DLOS enable (1 to enable, 0 to disable)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken2 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken2 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn2_dlos_lane_cfg(sifd_handle_t *h, 
                                      UINT32 lane, 
                                      UINT32 dlos_invert,
                                      UINT32 dlos_enable)
{
    PMC_ENTRY();

    /* generate dlos / dlos inv value */
    sifd_dft_field_IL2_DLOS_INV_set( NULL, h, lane, dlos_invert);
    sifd_dft_field_IL2_DLOS_EN_set ( NULL, h, lane, dlos_enable);
    PMC_RETURN();
} /* sifd_ilkn2_los_cfg */

/*******************************************************************************
* sifd_ilkn2_serdes_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the Rx Data, Rx Clock and Tx Clock Serdes data
*   sources associated with the ILKN 2 specified lane.\n\n
*   
*   Should an ILKN interface require lanes from more than one serdes type,
*   the function must be called multiple times.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - Denotes the set of lanes within serdes_type that
*                           are enabled for use with Interlaken 2 
*                           enabled.
*                           See note for which bits in the bit mask are 
*                           valid. The LSB representing ILKN2 Lane 0, the MSB
*                           represents ILKN2 Lane 23
*   serdes_type           - enum, the type of serdes that this interlaken lane 
*                           group is connected to.\n
*                           SIFD_SRDS_E1_LR\n
*                           SIFD_SRDS_N1_SR\n
*                           SIFD_SRDS_N2_SR
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   The valid Serdes that Interlaken2 may connect to are:\n
* ______________________________________________________________________________\n
*\code
*   SIFD_SRDS_E1_LR: The E1 Serdes, Lanes[23:00]\n
*   SIFD_SRDS_N1_SR: The N1 Serdes, Lanes[15:08]\n
*   SIFD_SRDS_N2_SR: The N2 Serdes, Lanes[07:00]\n
*\endcode
*  Interlaken2 Lanes can be mapped to the following serdes lanes\n
*\n
*  Interlaken2 Lanes 23 through 16:\n
\code
*  ILKN2 --------------------------------[23][22][21][20][19][18][17][16]\n
*     E1 --------------------------------[23][22][21][20][19][18][17][16]\n
*     N1 ----------------------------------------------------------------\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N1 --------------------------------[15][14][13][12][11][10][ 9][ 8]\n
*     N2 [ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]--------------------------------\n
*\endcode
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn2_serdes_select(sifd_handle_t *h, 
                                           UINT32 b_lane_mask,
                                           sifd_srds_type_t serdes_type)
{
    PMC_ERROR rc = PMC_SUCCESS;

    UINT32 i = 0;
    UINT8 lane_number;

    UINT8 sifd_ilkn2_e1_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};

    UINT8 sifd_ilkn2_n1_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    8, 9, 10, 11, 12, 13, 14, 15,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE};

    UINT8 sifd_ilkn2_n2_lane_map[SIFD_ILKN_MAX_SERDES_LANES] = {
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE,
    0, 1, 2, 3, 4, 5, 6, 7,
    SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE, SIFD_NO_LANE};

    PMC_ENTRY();

    for (i = 0; ((i < SIFD_MAX_ILKN_LANES) && (rc == PMC_SUCCESS)); 
                                          i++, b_lane_mask = b_lane_mask >> 1) {

        if ((b_lane_mask & SIFD_ILKN_ENABLE) == TRUE) {

            /* configure and enable the lane */
            rc = sifd_ilkn2_serdes_lane_cfg(h, i, serdes_type);
            if (rc != PMC_SUCCESS)
            {
                PMC_RETURN(rc);
            }    

            switch (serdes_type) {
            case SIFD_SRDS_E1_LR:
                lane_number = sifd_ilkn2_e1_lane_map[i];
                if (lane_number != SIFD_NO_LANE)
                    sifd_e1_tx_lane_sel_lane_set(h, lane_number, SIFD_E1_LANE_SELECT_ILKN2);
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            case SIFD_SRDS_N1_SR:
                lane_number = sifd_ilkn2_n1_lane_map[i];
                if (lane_number != SIFD_NO_LANE)
                    sifd_n1_tx_lane_sel_lane_set(h, lane_number, SIFD_N1_LANE_SELECT_ILKN2);
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            case SIFD_SRDS_N2_SR:
                lane_number = sifd_ilkn2_n2_lane_map[i];
                if (lane_number != SIFD_NO_LANE)
                    sifd_n2_tx_lane_sel_lane_set(h, lane_number, SIFD_N2_LANE_SELECT_ILKN2);
                else
                    rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            default:
                rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
                break;
            }

        }
    }

    PMC_RETURN(rc);
} /* sifd_ilkn2_serdes_select */

/*******************************************************************************
* sifd_ilkn2_lane_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   This function specifies the set of Rx and Tx ILKN2 lanes to be disabled.
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   b_lane_mask           - the RX and TX lane(s) to disable when bit in the 
*                           mask is set. See note for which bits in the bit mask
*                           are valid. The LSB representing ILKN2 Lane 0, the 
*                           MSB represents ILKN2 Lane 23
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*   The valid Interlaken2 lanes that may be disabled are:\n
* ______________________________________________________________________________\n
*\n
*  Interlaken2 Lanes 23 through 16:\n
*\code
*  mask: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*  ILKN1 --------------------------------[23][22][21][20][19][18][17][16]\n
*\endcode
*  Interlaken2 Lanes 15 through 0:\n
*\code
*  mask: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*  ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_ilkn2_lane_disable(sifd_handle_t *h, UINT32 b_lane_mask)
{
    UINT32 i = 0;
    UINT32 lane_state = SIFD_ILKN_LANE_DISABLE;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; i++, b_lane_mask = b_lane_mask >> 1) {

        lane_state = b_lane_mask & SIFD_ILKN_LANE_DISABLE;

        /* disable or enable the lane */
        sifd_ilkn2_single_lane_disable_set(h, i, lane_state);
    }

    PMC_RETURN();
} /* sifd_ilkn2_lane_disable */


/*******************************************************************************
* sifd_e1_tx_lane_sel_lane_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function specifies the Tx Lane Select receiever associated with the 
*   E1 SERDES on the specified lane.\n\n
*   
*   Should an E1 SERDES require different Tx targets (to more than one block 
*   type), the function must be called multiple times.
*
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - Denotes which E1 lane is to be acted upon.
*   value                 - the value to write to the register
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
*   The valid E1 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  E1 Lanes 63 through 48 (not used):\n
*\code
*  lane: [63][62][61][60][59][58][57][56][55][54][53][52][51][50][49][48]\n
*    E1: ----------------------------------------------------------------\n
*\endcode
*  E1 Lanes 47 through 32:\n
*\code
*  lane: [47][46][45][44][43][42][41][40][39][38][37][36][35][34][33][32]\n
*    E1: --------------------------------[39][38][37][36][35][34][33][32]\n
*\endcode
*  E1 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*     E1 [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*\endcode
*  E1 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_e1_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value)
{
    PMC_ENTRY();
    
    sifd_dft_field_E1_TX_LANE_SEL_set(NULL, h, lane, value);
    PMC_RETURN();
} /* sifd_e1_tx_lane_sel_lane_set */

/*******************************************************************************
* sifd_n1_tx_lane_sel_lane_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function specifies the Tx Lane Select receiever associated with the 
*   N1 SERDES on the specified lane.\n\n
*   
*   Should an N1 SERDES require different Tx targets (to more than one block 
*   type), the function must be called multiple times.\n\n
*
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - Denotes which N1 lane is to be acted upon.
*   value                 - the value to write to the register
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
*   The valid N1 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  N1 Lanes 63 through 48 (not used):\n
*\code
*  lane: [63][62][61][60][59][58][57][56][55][54][53][52][51][50][49][48]\n
*    N1: ----------------------------------------------------------------\n
*\endcode
*  N1 Lanes 47 through 32:\n
*\code
*  lane: [47][46][45][44][43][42][41][40][39][38][37][36][35][34][33][32]\n
*    N1: ----------------------------------------------------------------\n
*\endcode
*  N1 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*     N1 ----------------------------------------------------------------\n
*\endcode
*  N1 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*\endcode
*
*******************************************************************************/
PRIVATE void sifd_n1_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value)
{
    PMC_ENTRY();
    
    sifd_dft_field_N1_TX_LANE_SEL_set(NULL, h, lane, value);
    PMC_RETURN();
} /* sifd_n1_tx_lane_sel_lane_set */

/*******************************************************************************
* sifd_n2_tx_lane_sel_lane_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function specifies the Tx Lane Select receiever associated with the 
*   N2 SERDES on the specified lane.\n\n
*   
*   Should an N2 SERDES require different Tx targets (to more than one block 
*   type), the function must be called multiple times.
*
*
* INPUTS:
*   *h                    - pointer to sifd handle instance to be operated on
*   lane                  - Denotes which N2 lane is to be acted upon.
*   value                 - the value to write to the register
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
*   The valid N2 lanes are:\n
* ______________________________________________________________________________\n
*\n
*  N2 Lanes 63 through 48 (not used):\n
*\code
*  lane: [63][62][61][60][59][58][57][56][55][54][53][52][51][50][49][48]\n
*    N2: ----------------------------------------------------------------\n
*\endcode
*  N2 Lanes 47 through 32:\n
*\code
*  lane: [47][46][45][44][43][42][41][40][39][38][37][36][35][34][33][32]\n
*    N2: ----------------------------------------------------------------\n
*\endcode
*  N2 Lanes 31 through 16:\n
*\code
*  lane: [31][30][29][28][27][26][25][24][23][22][21][20][19][18][17][16]\n
*     N2 ----------------------------------------------------------------\n
*\endcode
*  N2 Lanes 15 through 0:\n
*\code
*  lane: [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*     N1 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\n
*
*\endcode
*******************************************************************************/
PRIVATE void sifd_n2_tx_lane_sel_lane_set(sifd_handle_t *h, 
                                          UINT32 lane, 
                                          UINT32 value)
{
    PMC_ENTRY();
    
    sifd_dft_field_N2_TX_LANE_SEL_set(NULL, h, lane, value);

    PMC_RETURN();
} /* sifd_n2_tx_lane_sel_lane_set */

/*******************************************************************************
*  sifd_bits_used_more_than_once_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Determines if overlap in bit masks.
*
*  INPUTS:
*                  bts_cfg   - structure for lanes on E1, N1 & N2
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      UINT32 - mask of lanes used more than once.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE UINT32 sifd_bits_used_more_than_once_get(sifd_blk_to_srds_msk_cfg_t bts_cfg)
{
    UINT32 return_mask = 0;
    UINT32 temp1 = bts_cfg.blk_to_E1_enable_mask & 
                   bts_cfg.blk_to_N1_enable_mask;
    UINT32 temp2 = bts_cfg.blk_to_N1_enable_mask & 
                   bts_cfg.blk_to_N2_enable_mask;
    UINT32 temp3 = bts_cfg.blk_to_E1_enable_mask & 
                   bts_cfg.blk_to_N2_enable_mask;

    PMC_ENTRY();

    return_mask = temp1 | temp2 | temp3;

    PMC_RETURN(return_mask);

}  /* sifd_bits_used_more_than_once_get */

/*******************************************************************************
*  sifd_ilkn_msk_invert
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                 Provides inverted mask of all serdes pins used.
*
*  INPUTS:
*                  bts_cfg   - structure for lanes on E1, N1 & N2
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      UINT32 - mask of unused lanes (inversion)
*
*  NOTES:
*
*******************************************************************************/
PRIVATE UINT32 sifd_ilkn_msk_invert(sifd_blk_to_srds_msk_cfg_t bts_cfg)
{
    UINT32 temp = 0;

    PMC_ENTRY();

    temp = bts_cfg.blk_to_E1_enable_mask | 
           bts_cfg.blk_to_N1_enable_mask |
           bts_cfg.blk_to_N2_enable_mask;

    temp = ~temp;

    PMC_RETURN(temp);

} /* sifd_ilkn_msk_invert */

/* UTILITY HELPER FUNCTIONS */

/*******************************************************************************
*  sifd_ilkn_util_core_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function controls the setting of INTLKN_SOFT_RST,
*                  RX_SOFT_RST and TX_SOFT_RST to configure the 'Major
*                  Bidirectional States' of the Interlaken block required for
*                  block configuration and operation.
*
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  mode      - major Bidirectional State:\n
*                              SIFD_COPI_ILKN_STATE_INRESET -- 'In reset' (disabled)\n
*                              SIFD_COPI_ILKN_STATE_STANDBY -- standby\n
*                                  (configuration registers accessible)\n
*                              SIFD_COPI_ILKN_STATE_OPERATIONAL -- 'Operational'\n
*                                  (configuration registers not accessible)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_enable(sifd_handle_t *handle, 
                                             sifd_ilkn_inst_t ilkn_inst,
                                             sifd_ilkn_state_t mode)
{
    UINT32 rx_sft_rst_val, tx_sft_rst_val;
    UINT32 cpb_sft_rst_val, oduk_sft_rst_val;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (mode > SIFD_COPI_ILKN_STATE_OPERATIONAL)||
        (mode < SIFD_COPI_ILKN_STATE_INRESET)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, mode);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* modify the register data for the required 'bidirectional mode' */
    if (mode == SIFD_COPI_ILKN_STATE_OPERATIONAL) {        /* set to operational? */
        rx_sft_rst_val = SIFD_ILKN_DISABLE;
        tx_sft_rst_val = SIFD_ILKN_DISABLE;
        cpb_sft_rst_val = SIFD_ILKN_DISABLE;
        oduk_sft_rst_val = SIFD_ILKN_DISABLE;
    } else if (mode == SIFD_COPI_ILKN_STATE_STANDBY){      /* set to standby? */
        rx_sft_rst_val = SIFD_ILKN_ENABLE;
        tx_sft_rst_val = SIFD_ILKN_ENABLE;
        cpb_sft_rst_val = SIFD_ILKN_ENABLE;
        oduk_sft_rst_val = SIFD_ILKN_ENABLE;
    } else {                                               /* put to reset? */
        rx_sft_rst_val = SIFD_ILKN_ENABLE;
        tx_sft_rst_val = SIFD_ILKN_ENABLE;
        cpb_sft_rst_val = SIFD_ILKN_ENABLE;
        oduk_sft_rst_val = SIFD_ILKN_ENABLE;
    }

    copi_field_IL_RX_SOFT_set(NULL, handle, ilkn_inst, rx_sft_rst_val);
    copi_field_IL_TX_SOFT_set(NULL, handle, ilkn_inst, tx_sft_rst_val);
    copi_field_CPB_TX_SOFT_set(NULL, handle, ilkn_inst, cpb_sft_rst_val);
    copi_field_ODUK_TX_SOFT_set(NULL, handle, ilkn_inst, oduk_sft_rst_val);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_enable */

/*******************************************************************************
*  sifd_ilkn_util_core_rx_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function controls the setting of INTLKN_SOFT_RST,
*                  RX_SOFT_RST and TX_SOFT_RST to configure the 'Major
*                  Bidirectional States' of the Interlaken block required for
*                  block configuration and operation.
*
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  mode      - major Bidirectional State:\n
*                              SIFD_COPI_ILKN_STATE_INRESET : 'In reset' (disabled)\n
*                              SIFD_COPI_ILKN_STATE_STANDBY : standby\n
*                                  (configuration registers accessible)\n
*                              SIFD_COPI_ILKN_STATE_OPERATIONAL : 'Operational'\n
*                                  (configuration registers not accessible)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_rx_enable(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_state_t mode)
{
    UINT32 rx_sft_rst_val;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (mode > SIFD_COPI_ILKN_STATE_OPERATIONAL)||
        (mode < SIFD_COPI_ILKN_STATE_INRESET)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, mode);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* modify the register data for the required 'bidirectional mode' */
    if (mode == SIFD_COPI_ILKN_STATE_OPERATIONAL) {        /* set to operational? */
        rx_sft_rst_val = SIFD_ILKN_DISABLE;
    } else if (mode == SIFD_COPI_ILKN_STATE_STANDBY){      /* set to standby? */
        rx_sft_rst_val = SIFD_ILKN_ENABLE;
    } else {                                               /* put to reset? */
        rx_sft_rst_val = SIFD_ILKN_ENABLE;
    }

    copi_field_IL_RX_SOFT_set(NULL, handle, ilkn_inst, rx_sft_rst_val);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_rx_enable */

/*******************************************************************************
*  sifd_ilkn_util_core_tx_enable
*  ___________________________________________________________________________
*
*  DESCRIPTION: 
*               This function controls the setting of IL_TX_SOFT such as ILKN
*               TX slice is in or out of reset state.
*
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  mode      - major Bidirectional State:\n
*                              SIFD_COPI_ILKN_STATE_INRESET -- 'In reset' (disabled)\n
*                              SIFD_COPI_ILKN_STATE_STANDBY -- standby\n
*                                  (configuration registers accessible)\n
*                              SIFD_COPI_ILKN_STATE_OPERATIONAL -- 'Operational'\n
*                                  (configuration registers not accessible)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_tx_enable(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_state_t mode)
{
    UINT32 tx_sft_rst_val;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (mode > SIFD_COPI_ILKN_STATE_OPERATIONAL)||
        (mode < SIFD_COPI_ILKN_STATE_INRESET)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, mode);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* modify the register data for the required 'bidirectional mode' */
    if (mode == SIFD_COPI_ILKN_STATE_OPERATIONAL) {        /* set to operational? */
        tx_sft_rst_val = SIFD_ILKN_DISABLE;
    } else if (mode == SIFD_COPI_ILKN_STATE_STANDBY){      /* set to standby? */
        tx_sft_rst_val = SIFD_ILKN_ENABLE;
    } else {                                               /* put to reset? */
        tx_sft_rst_val = SIFD_ILKN_ENABLE;
    }

    copi_field_IL_TX_SOFT_set(NULL, handle, ilkn_inst, tx_sft_rst_val);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_tx_enable */

/*******************************************************************************
*  sifd_ilkn_util_core_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  The caller function must ensure the that interlaken block is
*                  in 'Major Bidirectional States' of 
*                  SIFD_COPI_ILKN_STATE_STANDBY in order for this function to 
*                  be executed.\n\n
*
*                  It performs the following operations:\n
*                  1. Enables data flow to SERDES links\n
*                  2. Disable Loopbacks\n
*                  3. Transmit FIFO Configuration\n
*                  4. Burst Setting Configuration\n
*                           BURST_SHORT - 32 bytes\n
*                           BURST_MIN - 64 bytes\n
*                           BURST_MAX - 256 bytes\n
*                  5. Metaframe Synchronization Configuration\n
*                           2048 bytes\n
*                  6. Disable SERDES Test Pattern Generation/Reception\n
*
*
*  INPUTS:
*   *handle               - pointer to sifd handle instance to be operated on
*   ilkn_inst             - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                           SIFD_COPI_ILKN2 Interlaken 2 Instance
*   burst_max             - max burst (128, 192 or 256 bytes)
*   burst_short           - short burst (32 or 64 bytes)
*   burst_min             - min burst (32 or 64 bytes)
*   metaframe_period      - metaframe period (1024, 2048 or 4096 words)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_cfg(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst,
                                          sifd_oc_burst_t burst_max,
                                          sifd_burst_short_t burst_short,
                                          sifd_burst_min_t burst_min,
                                          sifd_ilkn_mf_t metaframe_period)
{
    UINT32 i;
    UINT32 low_wm, high_wm;
    UINT32 result;
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    PMC_ENTRY();

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    low_wm = 0x7; /* variable for WM_LOW field setting*/
    high_wm = 0x6; /* variable for WM_HIGH field setting*/

    /* Disable Interlaken Core Loopbacks */
    result = sifd_ilkn_util_loopback_set(handle, 
                                         ilkn_inst,
                                         SIFD_DISABLE_LOOPBACK, 
                                         FALSE);
    if (result != PMC_SUCCESS)
        PMC_RETURN(result);

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

    /* Transmit FIFO water_mark Configuration
       set WM_LOW to 0x4 and WM_HIGH to 0x3. */
    ilkn_field_WM_HIGH_set(NULL, handle, ilkn_inst, high_wm); 
    ilkn_field_WM_LOW_set(NULL, handle, ilkn_inst, low_wm); 

    /* Enable Data Flow to SERDES Links for Rx and Tx both */
    for (i = SIFD_ILKN_TX_DIRECTION; 
         i <= SIFD_ILKN_RX_DIRECTION && PMC_SUCCESS == result; 
         i++) 
    {
        /* Burst Configuration */
        /* Interlaken burst max is 1 greater than COPI burst max*/
        ilkn_field_BURSTMAX_set(NULL, handle, ilkn_inst, i, ((UINT32)burst_max + 1));       
        switch(burst_min)
        {
        case SIFD_BURST_MIN_32:
        case SIFD_BURST_MIN_64:
            ilkn_field_BURSTMIN_set(NULL, handle, ilkn_inst, i, (UINT32)burst_min);
            ilkn_field_MIN_DIS_set(NULL, handle, ilkn_inst, i, 0);
            break;
        case SIFD_BURST_MIN_DISABLE:
            ilkn_field_MIN_DIS_set(NULL, handle, ilkn_inst, i, 1);
            break;
        default:
            result = SIFD_LOG_ERR_INVALID_PARAMETERS;
            break;
        }

        if (result == PMC_SUCCESS)
        {
            ilkn_field_BURSTSHORT_set(NULL, handle, ilkn_inst, i, (UINT32)burst_short);
            
            /* Meta Frame Synchronization Configuration */
            ilkn_field_PERIOD_set(NULL, handle, ilkn_inst, i, (UINT32)metaframe_period);
            /* loopback for SIFD testing */
            /* ilkn_field_FC_TX_TO_RX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_ENABLE);*/
            
            /* Disable SERDES Test Pattern Generation/Reception */
            result = sifd_ilkn_test_cfg(handle, 
                                        ilkn_inst,
                                        (sifd_ilkn_dir_t)i,
                                        SIFD_ILKN_TEST_NORMAL_OPERATION,
                                        0, 
                                        0,
                                        0, 
                                        0);
        }
    }

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    PMC_RETURN(result);

} /* sifd_ilkn_util_core_cfg */

/*******************************************************************************
*  sifd_ilkn_util_core_tx_rate_limit_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configure the optional transmit rate limiting
*                  feature for one of the Interlaken blocks.  Transmit rate
*                  limiting sets the maximum rate for data to be transferred
*                  across the transmit interface to the downstream device.\n\n
*
*                  The calling function must ensure the that ilkn_inst is
*                  'Major Bidirectional States' of 2
*                  in order for this function to be executed.\n\n
*
*                  BURSTMAX must be configured prior to calling this function.
*                  BURSTMAX is configured when calling sifd_ilkn_util_core_cfg.
*
*
*
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance
*                  rate_limit - maximum Rate data may be transferred across
*                               transmit interface to downstream device.\n
*                               0 : disabled no rate limiting applied, maximum
*                                   transfer rate supported.\n
*                               1 to m : amount of rate limiting applied in
*                                        1 Gbps steps (125 MBps), where m is
*                                        the maxiumum number of 1 Gpbs steps
*                                        before exceeding the rate of the
*                                        interface.
*                  burst_limit - burst_limit Parameter setting as described in
*                                Interlaken Protocol Definition.  'The maxiumum
*                                quantity of data that may be burst across the
*                                interface before invoking the rate limiting
*                                logic (bytes)'
*                  burst_max - Interlaken burst size (128, 192 or 256 bytes)
*                  byte_gran - number of credits that are to be subtracted
*                              from the token bucket when an EOP is
*                              encountered.\n
*                               0 -- 1 byte\n
*                               1 -- 2 bytes\n
*                               2 -- 4 bytes\n
*                               3 -- 8 bytes\n
*                               4 -- 16 bytes\n
*                               5 -- 32 bytes
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_tx_rate_limit_cfg(sifd_handle_t *handle, 
                                                        sifd_ilkn_inst_t ilkn_inst,
                                                        UINT32 rate_limit, 
                                                        UINT32 burst_limit,
                                                        sifd_oc_burst_t burst_max,
                                                        UINT32 byte_gran)
{
    sifd_ilkn_en_t enable;
    UINT64 rl_time;                         /* numerator needs to be 64 bits */
    UINT32 rl_delta, credit_max;
    UINT32 burst_max_bytes = 0;
    UINT32 core_freq;
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) || (byte_gran > 5) || (rate_limit > 100)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, byte_gran);
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, rate_limit, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

    core_freq = 500000;                                    /* fixed rate */

    /* enable rate limiting operations */
    if (rate_limit != 0) {

        switch(burst_max){
        case SIFD_ODUK_CPB_BURST_128:
            burst_max_bytes = 128;
            break;
        case SIFD_ODUK_CPB_BURST_192:
            burst_max_bytes = 192;
            break;
        case SIFD_ODUK_CPB_BURST_256:
            burst_max_bytes = 256;
            break;
        default:
/* LCOV_EXCL_START */
            PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, burst_max_bytes, 0);
/* LCOV_EXCL_STOP */
            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
        }

        /* return an error if burst_limit <= burst_max_bytes and 
           if burst_limit - burst_max_bytes is less than or equal 96 */
        if ((burst_limit <= burst_max_bytes) ||
            ((burst_limit - burst_max_bytes) <= 96)) {
/* LCOV_EXCL_START */
            PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, burst_limit, burst_max_bytes);
/* LCOV_EXCL_STOP */
            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
        }

        /* calculate CREDIT_MAX field setting */
        credit_max = burst_limit - burst_max_bytes - 96;

        /* calculate RL_DELTA and RL_RATE */
        /* Transmit Bandwidth (bytes/s) = CORE_FREQ*(RL_DELTA/RL_RATE)
           Transmit Bandwidth (bits/s) = 8*CORE_FREQ*(RL_DELTA/RL_RATE)
           rate_limit is the transmit bandwidth in Gbps
           core_freq is the CORE_FREQ
           RL_DELTA must be less than or equal to CREDIT_MAX
           Need to solve for RL_DELTA and RL_RATE, starting with
           RL_DELTA = CREDIT_MAX;

           rl_time = 8*core_freq*rl_delta/(rate_limit*10^6)
        */

        rl_delta = credit_max;
        rl_time = (UINT64)8 * (UINT64)core_freq * (UINT64)rl_delta;  /* calculate numerator */

        /* if numerator is less than denominator, rl_time will be 0, in this
           case, burst_limit is too small for the system to achieve the
           desired rate limiting setting */
        if ((rl_time < (UINT64)(rate_limit * 1000000)) ||
            ((burst_limit - burst_max_bytes) <= 96) ) {

            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
        }

        /* Devide by zero is checked at the top of this part of the function */
        rl_time = rl_time / (UINT64)rate_limit;  /* divide by denominator less the Gbps to
                                                    kbps conversion */
        rl_time = rl_time / (UINT64)1000000;     /* divide by denonimator for Gbps to kpbs
                                                    conversion */

        /* Set RL_TIME field */
        ilkn_field_RL_TIME_set(NULL, handle, ilkn_inst, (UINT32)rl_time);

        /* Set RL_DELTA field */
        ilkn_field_RL_DELTA_set(NULL, handle, ilkn_inst, rl_delta);

        /* Set CREDIT_MAX field */
        ilkn_field_CREDIT_MAX_set(NULL, handle, ilkn_inst, credit_max);

        /* Set BYTE_GRAN field */
        ilkn_field_BYTE_GRAN_set(NULL, handle, ilkn_inst, byte_gran);

        enable = SIFD_ILKN_ENABLE;

    } else {

        enable = SIFD_ILKN_DISABLE;
    }

    /* Set RL_ENABLE bit */
    ilkn_field_RL_ENABLE_set(NULL, handle, ilkn_inst, enable);

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_tx_rate_limit_cfg */

/*******************************************************************************
*  sifd_ilkn_util_core_lane_disable
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configures an Interlaken ilkn_inst to disable
*                  SERDES lanes\n\n
*
*                  The calling function must ensure the that ilkn_inst is in
*                  'Major Bidirectional States' of 2 for this function
*                  to be executed.
*
*
*  INPUTS:
*                  *handle  - base address
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  disable_msk - For OPI and PIF ilkn_insts:
*                               24 bits one per SERDES lane set to 1 to disable
*                               set to 0 to enable.\n
*                               Legal range 0 to SIFD_24_LANE_MASK
*
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_lane_disable(sifd_handle_t *handle, 
                                                   sifd_ilkn_inst_t ilkn_inst,
                                                   UINT32 disable_msk)
{
    UINT32 i;
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    PMC_ENTRY();

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2 || disable_msk > SIFD_24_LANE_MASK) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, disable_msk);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

        
    /* apply DISABLE bit settings for TX and RX */
    for (i = SIFD_ILKN_TX_DIRECTION; i <= SIFD_ILKN_RX_DIRECTION; i++) {
        
        ilkn_field_DISABLE_set(NULL, handle, ilkn_inst, i, disable_msk);
    }

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_lane_disable */

/*******************************************************************************
*  sifd_ilkn_util_core_fc_mode_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function configures an Interlaken ilkn_inst for in-band
*                  or out-of-band flow control reporting for the Tx or Rx
*                  interface.\n\n
*
*                  The in-band flow control may be programmed to generate
*                  flow control data using only a programmable calendar.\n\n
*
*                  Out-of-band flow control programmable calendar.\n\n
*
*                  When programmable inband/out-of-band calendar mode of
*                  operation is selected, the programmable calendar is
*                  initialized so that all entries are mapped to the NULL flow
*                  control port.\n\n
*
*                  The calling function must ensure the that ilkn_inst is in
*                  'Major Bidirectional States' of 2 for this function
*                  to be executed.
*
*
*  INPUTS:
*                  *handle   - base address
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  mode      - flow control mode
*                  cal_len   - flow control calendar length
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_core_fc_mode_cfg(sifd_handle_t *handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  sifd_fc_mode_t mode, 
                                                  UINT32 cal_len)
{
    UINT32 result;
    UINT32 i;
    UINT32 out_band = SIFD_ILKN_DISABLE;
    UINT32 in_band = SIFD_ILKN_DISABLE;
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    PMC_ENTRY();

    /* argument checking */
    if (mode < SIFD_FC_MODE_NONE || 
        mode >= LAST_SIFD_FC_MODE ||
        cal_len < SIFD_ILKN_MIN_CAL_ENTRIES ||
        cal_len > SIFD_ILKN_MAX_CAL_ENTRIES ||
        ilkn_inst >= LAST_SIFD_ILKN)    
    {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, cal_len, mode);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }
   
    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

    /*
     * configure  INBAND bit and OUTBAND bit
     */
    switch (mode) {
    case SIFD_FC_MODE_IN_BAND_24:
/* LCOV_EXCL_START */
        ilkn_field_MULT_FC_set(NULL, handle, ilkn_inst, 0, 1);  /* TX use multi-use field */
        ilkn_field_MULT_FC_set(NULL, handle, ilkn_inst, 1, 1);  /* RX use multi-use field */
        /* intentionally fall through */
/* LCOV_EXCL_STOP */
    case SIFD_FC_MODE_IN_BAND_16:
        in_band = SIFD_ILKN_ENABLE;      /* value for INBAND bit */
        break;

    case SIFD_FC_MODE_OUT_OF_BAND:
        out_band = SIFD_ILKN_ENABLE;     /* value for OUTBAND bit */
        break;

    default:
        break;                           /* not possible */
    }

    ilkn_field_OUTBAND_set(NULL, handle, ilkn_inst, 0, out_band);    /* TX */
    ilkn_field_OUTBAND_set(NULL, handle, ilkn_inst, 1, out_band);    /* RX */

    ilkn_field_INBAND_set(NULL, handle, ilkn_inst, 0, in_band);      /* TX */
    ilkn_field_INBAND_set(NULL, handle, ilkn_inst, 1, in_band);      /* RX */

    /* configure PROG bit*/
    ilkn_field_PROG_set(NULL, handle, ilkn_inst, 0, 1);              /* TX/RX */
    ilkn_field_PROG_set(NULL, handle, ilkn_inst, 1, 1);              /* RX/TX */

    /* configure LAST bits*/
    ilkn_field_LAST_set(NULL, handle, ilkn_inst, 0, (cal_len - 1));  /* TX/RX */
    ilkn_field_LAST_set(NULL, handle, ilkn_inst, 1, (cal_len - 1));  /* RX/TX */

    /* initialize PIF programmable calendar entries to the null flow control port */
    for (i = 0; i < cal_len; ++i) {
                                                                     /* TX */
        result = sifd_ilkn_util_cal_write(handle, ilkn_inst, 
                                          SIFD_ILKN_TX_DIRECTION, i,
                                          SIFD_ILKN_NULL_CAL_ENTRY);
        if (result != PMC_SUCCESS)
            PMC_RETURN(result);
                                                                     /* RX */
        result = sifd_ilkn_util_cal_write(handle, ilkn_inst, 
                                          SIFD_ILKN_RX_DIRECTION, i,
                                          SIFD_ILKN_NULL_CAL_ENTRY);
        if (result != PMC_SUCCESS)
            PMC_RETURN(result);
    }

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    PMC_RETURN(PMC_SUCCESS);

} /* sifd_ilkn_util_core_fc_mode_cfg */

/*******************************************************************************
*  sifd_ilkn_util_loopback_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Sets or disbales loopbacks in SIFD.\n\n
*
*                  Note: IL_RX_SOFT and IL_TX_SOFT must be held in reset
*                  until this is set.
*
*  INPUTS:
*                  *handle  - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  loop_type - Enum defining the loopback to set. See
*                              sifd_loopback_cfg_t for details.
*                  fc        - FALSE : without flowcontrol\n
*                            - TRUE : with flowcontrol
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_loopback_set(sifd_handle_t *handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  sifd_loopback_cfg_t loop_type,
                                                  BOOL fc)
{
    PMC_ENTRY();
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
    BOOL8         rx_enabled;
    BOOL8         tx_enabled;

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2)  
    {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, loop_type);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    /* disable ILKN instance during the configuration if it is enabled */
    sifd_copi_soft_enable_status_get(handle, 
                                     ilkn_inst,
                                     &rx_enabled,
                                     &tx_enabled);

    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_STANDBY);

    switch (loop_type) {

    case SIFD_DISABLE_LOOPBACK:
        copi_field_DPI_EXT_ODUK_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_DPI_EXT_CPB_LB_set(NULL, handle, ilkn_inst,  SIFD_ILKN_DISABLE);
        copi_field_DPI_INT_ODUK_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_DPI_INT_CPB_LB_set(NULL, handle, ilkn_inst,  SIFD_ILKN_DISABLE);
        copi_field_DPI_FC_EXT_ODUK_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_DPI_FC_EXT_CPB_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_DPI_FC_INT_ODUK_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);
        copi_field_DPI_FC_INT_CPB_LB_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);

        for (i = SIFD_ILKN_TX_DIRECTION; i <= SIFD_ILKN_RX_DIRECTION; i++) 
        {
            ilkn_field_FC_RX_TO_TX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_DISABLE);
            ilkn_field_FC_TX_TO_RX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_DISABLE);
            ilkn_field_DATA_RX_TO_TX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_DISABLE);
            ilkn_field_DATA_TX_TO_RX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_DISABLE);
        }

        ilkn_field_TX_DATA_OUT_KILL_set(NULL, handle, ilkn_inst, 0);
        ilkn_field_RX_DATA_OUT_KILL_set(NULL, handle, ilkn_inst, 0);

        break;

    case SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK:
    case SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK:

        /* Stop data from being passed to the SERDES */
        ilkn_field_TX_DATA_OUT_KILL_set(NULL, handle, ilkn_inst, 1);

        if (result == PMC_SUCCESS)
        {        
            for (i = SIFD_ILKN_TX_DIRECTION; i <= SIFD_ILKN_RX_DIRECTION; i++) 
            {
                ilkn_field_DATA_TX_TO_RX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_ENABLE);
                ilkn_field_FC_TX_TO_RX_LB_set(NULL, handle, ilkn_inst, i, fc);
            }
        }
      
        break;

    case SIFD_COPI_FACILITY_LOOPBACK:

        /* Stop data from being passed to the CPB/ODUkSW */
        ilkn_field_RX_DATA_OUT_KILL_set(NULL, handle, ilkn_inst, 1);

        if (result == PMC_SUCCESS)
        {         
            for (i = SIFD_ILKN_TX_DIRECTION; i <= SIFD_ILKN_RX_DIRECTION; i++) 
            {

                ilkn_field_FC_RX_TO_TX_LB_set(NULL, handle, ilkn_inst, i, fc);
                ilkn_field_DATA_RX_TO_TX_LB_set(NULL, handle, ilkn_inst, i, SIFD_ILKN_ENABLE);

            }
        }    
        break;
    
    default:
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, loop_type, 0);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
        break;
    }

    /* re-enable ILKN instance it i was enabled before the begining of the configuration */
    SIFD_ILKN_UTIL_CORE_RX_CONDITIONAL_ENABLE(handle, ilkn_inst, rx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);
    SIFD_ILKN_UTIL_CORE_TX_CONDITIONAL_ENABLE(handle, ilkn_inst, tx_enabled, SIFD_COPI_ILKN_STATE_OPERATIONAL);

    PMC_RETURN(result);

} /* sifd_ilkn_util_loopback_set */

/*******************************************************************************
*  sifd_ilkn_util_copi_loopback_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                 Gets loopback register values.
*
*
*  INPUTS:
*                  *handle  - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*
*  OUTPUTS:
*                  loop_type - see sifd_loopback_cfg_t
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_loopback_get(sifd_handle_t *handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  sifd_loopback_cfg_t* loop_type)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ext_oduk, ext_cpb, int_oduk, int_cpb,
           rx_tx, tx_rx;

    PMC_ENTRY();

    /* argument checking */
    if (ilkn_inst > SIFD_COPI_ILKN2)
    {
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    ext_oduk = copi_field_DPI_EXT_ODUK_LB_get(NULL, handle, ilkn_inst);
    ext_cpb = copi_field_DPI_EXT_CPB_LB_get(NULL, handle, ilkn_inst);
    int_oduk = copi_field_DPI_INT_ODUK_LB_get(NULL, handle, ilkn_inst);
    int_cpb = copi_field_DPI_INT_CPB_LB_get(NULL, handle, ilkn_inst);

    /* Both direction always get set together so it's safe to use only 0 */
    rx_tx = ilkn_field_DATA_RX_TO_TX_LB_get(NULL, handle, ilkn_inst, 0);
    tx_rx = ilkn_field_DATA_TX_TO_RX_LB_get(NULL, handle, ilkn_inst, 0);

    if(ext_oduk == 0 &&
       ext_cpb == 0 &&
       int_oduk == 0 &&
       int_cpb == 0 &&
       rx_tx == 0 &&
       tx_rx == 0)
    {
        *loop_type = SIFD_DISABLE_LOOPBACK;
    } 
    else if(ext_oduk == 0 &&
            ext_cpb == 0 &&
            int_oduk == 0 &&
            int_cpb == 0 &&
            rx_tx == 0 &&
            tx_rx == 1)
    {
        *loop_type = SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK; /* This is the 
        same as SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK */
    } 
    else if(ext_oduk == 0 &&
            ext_cpb == 0 &&
            int_oduk == 0 &&
            int_cpb == 0 &&
            rx_tx == 1 &&
            tx_rx == 0)
    {
        *loop_type = SIFD_COPI_FACILITY_LOOPBACK;
    } 
    else
    {
        /* Other modes not supported, more than
         * one at a time not supported */
        result = SIFD_LOG_ERR_LOOPBACK_AMBIGUOUS;
    }

    PMC_RETURN(result);

} /* sifd_ilkn_util_copi_loopback_get */

/*******************************************************************************
*  sifd_ilkn_util_copi_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Performs Interlaken Copi block initialization. 
*                  Sets items for CPB / ODUK sw.\n
*                  - Enables or Disables CPB interface and enables or
*                    disables power\n
*                  - BURST_MAX, PKT_MODE\n
*                  - Disables Loopbacks\n
*                  NOTE: The RX mode and the burst should match the ILKN config
*
*  INPUTS:
*                  *handle  - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  co_cfg    - The configuration for the ODUK/CPB and burst 
* 
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_copi_cfg(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst, 
                                          copi_cpb_oduk_cfg_t co_cfg)
{
    UINT32 i;

    PMC_ERROR rc = SIFD_LOG_ERR_UNKNOWN;

    PMC_ENTRY();

    /* argument checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (co_cfg.oduk_tx_burst > SIFD_ODUK_CPB_BURST_256) ||
        (co_cfg.oduk_tx_mode > SIFD_PACKET_MODE) ||
        (co_cfg.cpb_tx_burst > SIFD_ODUK_CPB_BURST_256) ||
        (co_cfg.cpb_tx_mode > SIFD_PACKET_MODE) ||
        (co_cfg.rx_burstmax > SIFD_ODUK_CPB_BURST_256) ||
        (co_cfg.rx_mode > SIFD_PACKET_MODE))
    {
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    rc = sifd_ilkn_util_loopback_set(handle, 
                                          ilkn_inst, 
                                          SIFD_DISABLE_LOOPBACK, 
                                          FALSE);

    /* disable EXTERNAL_LOOP_BACK and INTERNAL_LOOP_BACK */
    if (rc == PMC_SUCCESS) {

        /* Initialize Arbitrary Channel Mapping*/
        /* This may be better is a separate function */

        for (i = 0; i < SIFD_MAX_CPB_ARB_CHAN; ++i)
            copi_reg_TX_CPB_ARB_CH_LK_array_write(NULL, handle, ilkn_inst, i, i);

        for (i = 0; i < SIFD_MAX_ODUK_ARB_CHAN; ++i)
            copi_reg_TX_ODUK_ARB_CH_LK_array_write(NULL, handle, ilkn_inst, i, (SIFD_MAX_CPB_ARB_CHAN + i));

        for (i = 0; i < SIFD_MAX_ODUK_ARB_CHAN; ++i)
            copi_reg_TX_ODUK_4B_LK_array_write(NULL, handle, ilkn_inst, i, 0);

        for (i = 0; i < SIFD_MAX_ODUK_ARB_CHAN; ++i)
            copi_reg_TX_ODUK_2B_LK_array_write(NULL, handle, ilkn_inst, i, i);

        /* this version points all RX traffic to a "bit bucket" CPB channel 255 */
        for (i = 0; i < SIFD_MAX_ARB_CHAN; ++i) {
            copi_reg_RX_ARB_CH_LK_array_write(NULL, handle, ilkn_inst, i, 0xFF);
        }

        for (i = 0; i < SIFD_MAX_ODUK_ARB_CHAN; ++i)
            copi_reg_RX_ODUK_4B_LK_array_write(NULL, handle, ilkn_inst, i, 0);

        for (i = 0; i < SIFD_MAX_ODUK_ARB_CHAN; ++i)
            copi_reg_RX_ODUK_2B_LK_array_write(NULL, handle, ilkn_inst, i, i);

        /* These settings must match those in the correspoding ILKN */
        copi_field_RX_BURSTMAX_set(NULL, handle, ilkn_inst, co_cfg.rx_burstmax);
        copi_field_RX_PKT_MODE_set(NULL, handle, ilkn_inst, co_cfg.rx_mode);

        if ((co_cfg.oduk_tx_enabled == TRUE) || (co_cfg.cpb_tx_enabled == TRUE)) {

            /* If TX is enabled, enable RX */
            copi_field_RX_INTERFACE_EN_set(NULL, handle, ilkn_inst, SIFD_ILKN_ENABLE);
        }

        if (co_cfg.oduk_tx_enabled) {
            /* Light it up */
            copi_field_ODUK_LOWPWR_EN_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);

            /* Configure it */
            copi_field_RX_ODUK_CHEXT_EN_set(NULL, handle, ilkn_inst, co_cfg.oduk_rx_chext_en);
            copi_field_TX_ODUK_CHEXT_EN_set(NULL, handle, ilkn_inst, co_cfg.oduk_tx_chext_en);
            copi_field_TX_ODUK_BURSTMAX_set(NULL, handle, ilkn_inst, co_cfg.oduk_tx_burst);
            copi_field_TX_ODUK_INTERFACE_EN_set(NULL, handle, ilkn_inst, co_cfg.oduk_tx_enabled);
            copi_field_TX_ODUK_PKT_MODE_set(NULL, handle, ilkn_inst, co_cfg.oduk_tx_mode);
            copi_field_TX_ODUK_6B_INS_EN_set(NULL, handle, ilkn_inst, co_cfg.oduk_hdr_insert_extract);

            copi_field_RX_ODUK_6B_CHK_set(NULL, handle, ilkn_inst, co_cfg.oduk_hdr_insert_extract);
            copi_field_RX_ODUK_2B_MODE_set(NULL, handle, ilkn_inst, co_cfg.oduk_2b_hdr_mode);

            {
                UINT32 reg_addr = 0x904010;
                UINT32 value = 0;

                if (SIFD_COPI_ILKN2 == ilkn_inst)
                {
                    reg_addr += 0x4000;
                }
                value = l2sys_reg_read(((pmc_handle_t *)handle)->sys_handle,
                                       MEM_TYPE_STATUS, reg_addr); 
                
                if ((value & 0x20) == 0)
                {
                    value |= 0x20;
                }
                l2sys_reg_write(((pmc_handle_t *)handle)->sys_handle, 
                                MEM_TYPE_STATUS, reg_addr, value);
            }
            
        } else {

            /* Shut it down */
            copi_field_TX_ODUK_INTERFACE_EN_set(NULL, handle, ilkn_inst, co_cfg.oduk_tx_enabled);
            copi_field_ODUK_LOWPWR_EN_set(NULL, handle, ilkn_inst, SIFD_ILKN_ENABLE);
        }

        if (co_cfg.cpb_tx_enabled) {
            /* Light it up */
            copi_field_CPB_LOWPWR_EN_set(NULL, handle, ilkn_inst, SIFD_ILKN_DISABLE);

            /* Configure it */
            copi_field_RX_CPB_TYPE_EN_set(NULL, handle, ilkn_inst, co_cfg.cpb_rx_type_en);
            copi_field_TX_CPB_TYPE_EN_set(NULL, handle, ilkn_inst, co_cfg.cpb_tx_type_en);
            copi_field_TX_CPB_BURSTMAX_set(NULL, handle, ilkn_inst, co_cfg.cpb_tx_burst);
            copi_field_TX_CPB_INTERFACE_EN_set(NULL, handle, ilkn_inst, co_cfg.cpb_tx_enabled);
            copi_field_TX_CPB_PKT_MODE_set(NULL, handle, ilkn_inst, co_cfg.cpb_tx_mode);


        } else {

            /* Shut it down */
            copi_field_TX_CPB_INTERFACE_EN_set(NULL, handle, ilkn_inst, co_cfg.cpb_tx_enabled);
            copi_field_CPB_LOWPWR_EN_set(NULL, handle, ilkn_inst, SIFD_ILKN_ENABLE);
        }

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);

} /* sifd_ilkn_util_copi_cfg */

/*******************************************************************************
*  sifd_ilkn_util_cal_ctrl_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Write control flow control's programmable calender information
*                  from a specified programmable calendar location.
* 
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  dir       - Direction of flow control\n
*                              0 : Tx flow control(flow control to DIGI)\n
*                              1 : Rx flow control(flow control from DIGI)
*                  data      - data representing the channel to write into
*                              the calendar (bits 7:0)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_cal_data_write(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_dir_t dir,
                                                UINT32 data)
{
    UINT32 direction =0;
    PMC_ENTRY();

    /* Argument Checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (dir > SIFD_ILKN_RX_DIRECTION) || 
        (data > SIFD_ILKN_NULL_CAL_ENTRY)) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, dir);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    switch (handle->var.ilkn_fc[ilkn_inst].flow_control_mode) {
    case SIFD_FC_MODE_IN_BAND_16:
    case SIFD_FC_MODE_IN_BAND_24:
        /* dir refers to the direction of data but the reg doc defines direction
         * as the direction of flow control, so this is set opposite of the reg
         * doc */
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 1;
        else
            direction = 0;
        break;
    case SIFD_FC_MODE_OUT_OF_BAND:
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 0;
        else
            direction = 1;
        break;
    default:
        break;
    }
    /* Write data to WR_DAT_OFFSET */
    ilkn_field_CAL_WDATA_set(NULL, handle, ilkn_inst, direction, data);

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_util_cal_data_write */

/*******************************************************************************
*  sifd_ilkn_util_cal_ctrl_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Write flow control's programmable calender information
*                  from a specified programmable calendar location.
* 
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  dir       - Direction of flow control\n
*                              0 : Tx flow control(flow control to DIGI)\n
*                              1 : Rx flow control(flow control from DIGI)
*                  cal_addr  - the location in the programmable calendar to access
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_cal_ctrl_write(sifd_handle_t *handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                sifd_ilkn_dir_t dir,
                                                UINT32 cal_addr)
{
    UINT32 direction =0;
    ilkn_buffer_t b_ptr[1];
    PMC_ENTRY();

    /* Argument Checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (dir > SIFD_ILKN_RX_DIRECTION) ) {
/* LCOV_EXCL_START */
        PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, dir);
/* LCOV_EXCL_STOP */
        PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
    }

    switch (handle->var.ilkn_fc[ilkn_inst].flow_control_mode) {
    case SIFD_FC_MODE_IN_BAND_16:
    case SIFD_FC_MODE_IN_BAND_24:
        /* dir refers to the direction of data but the reg doc defines direction
         * as the direction of flow control, so this is set opposite of the reg
         * doc */
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 1;
        else
            direction = 0;
        break;
    case SIFD_FC_MODE_OUT_OF_BAND:
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 0;
        else
            direction = 1;
        break;
    default:
        break;
    }
    /* Write data to WR_DAT_OFFSET */
 
    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Write the calendar index */
    ilkn_field_CAL_ACC_ADDR_set(b_ptr, handle, ilkn_inst, direction, cal_addr);

    /* Trigger a read operation */
    ilkn_field_CAL_ACC_CMD_set(b_ptr, handle, ilkn_inst, direction, SIFD_ILKN_WRITE_CMD);

    ilkn_buffer_flush(b_ptr);

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_util_cal_ctrl_write */

/*******************************************************************************
*  sifd_ilkn_util_cal_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Write flow control's programmable calender information
*                  from a specified programmable calendar location.
* 
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  dir       - Direction of flow control\n
*                              0 : Tx flow control(flow control to DIGI)\n
*                              1 : Rx flow control(flow control from DIGI)
*                  cal_addr  - the location in the programmable calendar to access
*                  data      - data representing the channel to write into
*                              the calendar (bits 7:0)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_cal_write(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst,
                                           sifd_ilkn_dir_t dir,
                                           UINT32 cal_addr, 
                                           UINT32 data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = sifd_ilkn_util_cal_data_write(handle, ilkn_inst,dir,data);
    if (PMC_SUCCESS == rc)
    {
        rc = sifd_ilkn_util_cal_ctrl_write(handle, ilkn_inst,dir,cal_addr);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_util_cal_write */

/*******************************************************************************
*  sifd_ilkn_util_cal_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  Read the flow control programmable calender information
*                  from a specified programmable calendar location.
* 
*  INPUTS:
*                  *handle   - pointer to sifd handle instance to be operated on
*                  ilkn_inst - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                              SIFD_COPI_ILKN2 Interlaken 2 Instance 
*                  dir       - Direction of flow control\n
*                              0 : Tx flow control(flow control to DIGI)\n
*                              1 : Rx flow control(flow control from DIGI)
*                  cal_addr  - the location in the programmable calendar to access
*
*  OUTPUTS:
*                  data      - data representing the channel read from
*                              the calendar (bits 7:0)    
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_ilkn_util_cal_read(sifd_handle_t *handle, 
                                          sifd_ilkn_inst_t ilkn_inst,
                                          sifd_ilkn_dir_t dir,
                                          UINT32 cal_addr, 
                                          UINT32 *data)
{
    UINT32 direction =0;
    ilkn_buffer_t b_ptr[1];
    PMC_ENTRY();

    /* Argument Checking */
    if ((ilkn_inst > SIFD_COPI_ILKN2) ||
        (dir > SIFD_ILKN_RX_DIRECTION) || 
        (cal_addr >= handle->var.ilkn_fc[ilkn_inst].calendar_length)) 
        {
/* LCOV_EXCL_START */
            PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, ilkn_inst, dir);
            PMC_DEBUG(SIFD_LOG_ERR_INVALID_PARAMETERS, cal_addr, *data);
/* LCOV_EXCL_STOP */
            PMC_RETURN(SIFD_LOG_ERR_INVALID_PARAMETERS);
        }

    switch (handle->var.ilkn_fc[ilkn_inst].flow_control_mode) {
    case SIFD_FC_MODE_IN_BAND_16:
    case SIFD_FC_MODE_IN_BAND_24:
        /* dir refers to the direction of data but the reg doc defines direction
         * as the direction of flow control, so this is set opposite of the reg
         * doc */
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 1;
        else
            direction = 0;
        break;
    case SIFD_FC_MODE_OUT_OF_BAND:
        if (dir == SIFD_ILKN_TX_DIRECTION)
            direction = 0;
        else
            direction = 1;
        break;
    default:
        break;
    }
    
    ilkn_buffer_init(b_ptr, handle, ilkn_inst);

    /* Write the calendar index */
    ilkn_field_CAL_ACC_ADDR_set(b_ptr, handle, ilkn_inst, direction, cal_addr);

    /* Trigger a read operation */
    ilkn_field_CAL_ACC_CMD_set(b_ptr, handle, ilkn_inst, direction, SIFD_ILKN_READ_CMD);

    ilkn_buffer_flush(b_ptr);

    /* Read the data */
    *data = ilkn_field_CAL_RDATA_get(NULL, handle, ilkn_inst, direction);

    PMC_RETURN(PMC_SUCCESS);
} /* sifd_ilkn_util_cal_read */
/*******************************************************************************
*  sifd_validate_lane_remap_within_range
*  ___________________________________________________________________________
*
*  DESCRIPTION:     
*                   This function checks to make sure the lane array entries are 
*                   all between 0 and 23 or equal the null lane value. If an 
*                   entry is equal to the null lane value then the value is set 
*                   to the index.
*
*  INPUTS:
*                map         - structure of remap lanes
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_validate_lane_remap_within_range(sifd_ilkn_lane_remap_cfg_t *map)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; i++) 
    {

        if (map->lane[i] == SIFD_NO_LANE)
        {
            map->lane[i] = i;
        }
    
        if (map->lane[i] >= SIFD_MAX_ILKN_LANES) 
        {
            rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
        }
    }

    PMC_RETURN(rc);
} /* sifd_validate_lane_remap_within_range */

/*******************************************************************************
*  sifd_validate_lane_remap_contains_no_duplicates
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*                  This function checks to make sure no two values are the same.
*                  It also ensures that no logic lanes are mapped to disabled 
*                  serdes lanes because lanes that are not in use will be mapped
*                  to their own index. If another lane is mapped to that serdes 
*                  lane a duplication error will occur.
*
*  INPUTS:
*                map         - structure of remap lanes
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_validate_lane_remap_contains_no_duplicates(sifd_ilkn_lane_remap_cfg_t map)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    UINT32 dup_cnt = 0;

    PMC_ENTRY();

    for (i = 0; i < SIFD_MAX_ILKN_LANES; i++) {

        for (dup_cnt = 0; dup_cnt < SIFD_MAX_ILKN_LANES; dup_cnt++) {

            if (i == dup_cnt) {

                /* This is the case in the map where you are comparing
                identical entries. This duplicate is fine. */

            } else if (map.lane[i] == map.lane[dup_cnt]) {

                rc = SIFD_LOG_ERR_INVALID_PARAMETERS;
            }
        }
    }

    PMC_RETURN(rc);
} /* sifd_validate_lane_remap_contains_no_duplicates */

/*******************************************************************************
* sifd_serdes_to_internal_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*             This function validates lane parameters and maps SERDES lanes to an
*             internal index (after validating the map).
*
* INPUTS:
*   s_lane                 - SERDES lane (0 to 19)
*   i_lane                 - internal lane (0 to 11)
*
* OUTPUTS:
*   *ret_value_ptr         - pointer to SERDES to internal mapping index\n
*                            0 -- direct map internal to SERDES lanes 0 to 11\n
*                            1 -- internal to SERDES lanes 12 to 15\n
*                            2 -- internal to SERDES lanes 16 to 19
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_serdes_to_internal_param_check(UINT32 s_lane,
                                                      UINT32 i_lane,
                                                      UINT32* ret_value_ptr)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((sifd_serdes_lane_param_check(s_lane) == PMC_SUCCESS) &&
        (sifd_internal_lane_param_check(i_lane) == PMC_SUCCESS)) {

        if (s_lane == i_lane) {
            /* direct map for SERDES lanes 0 to 11 */
            *ret_value_ptr = 0;                  
            rc = PMC_SUCCESS;
        }
    }

    PMC_RETURN(rc);
} /* sifd_serdes_to_internal_param_check */

/*******************************************************************************
* sifd_internal_to_serdes_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*             This function validates lane parameters and maps SERDES lanes
*             12 to 19 to an internal index (after validating the map).
*
* INPUTS:
*   i_lane                 - internal lane (0 to 11)
*   s_lane                 - SERDES lane (12 to 19)
*
* OUTPUTS:
*   *ret_value_ptr         - SERDES to internal mapping index\n
*                            0 : internal lanes 0 to 3\n
*                            1 : internal lanes 4 to 7\n
*                            2 : internal lanes 8 to 11
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_internal_to_serdes_param_check(UINT32 i_lane,
                                                      UINT32 s_lane,
                                                      UINT32 *ret_value_ptr)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((sifd_internal_lane_param_check(i_lane) == PMC_SUCCESS) &&
        (s_lane >= SIFD_MINIMUM_INTERNAL_LANE_NUM) &&
        (s_lane <= SIFD_MAXIMUM_SERDES_LANE_NUM)) {

        UINT32 i_lane_rem = i_lane % 4;
        UINT32 s_lane_rem = s_lane % 4;

        if (i_lane_rem == s_lane_rem) {

            if (i_lane <= 3)
                *ret_value_ptr = 0;
            else if (i_lane <= 7)
                *ret_value_ptr = 1;
            else
                *ret_value_ptr = 2;

            rc = PMC_SUCCESS;
        }
    }

    PMC_RETURN(rc);
} /* sifd_internal_to_serdes_param_check */



/*******************************************************************************
* sifd_internal_lane_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the internal lane number (i.e. between 0 & 11).
*
* INPUTS:
*   i_lane                 - internal lane
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_internal_lane_param_check(UINT32 i_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((i_lane >= SIFD_MINIMUM_INTERNAL_LANE_NUM) && 
        (i_lane <= SIFD_MAXIMUM_INTERNAL_LANE_NUM)) {

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_internal_lane_param_check */

/*******************************************************************************
* sifd_serdes_lane_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the SERDES lane number (i.e. between 0 & 11).
*
* INPUTS:
*   s_lane                 - SERDES lane
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_serdes_lane_param_check(UINT32 s_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((s_lane >= SIFD_MINIMUM_SERDES_LANE_NUM) && 
        (s_lane <= SIFD_MAXIMUM_SERDES_LANE_NUM)) {

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* sifd_serdes_lane_param_check */

/*******************************************************************************
*  sifd_zone_th_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*      This function Configures TX and RX burst short and Burst max
*
*  INPUTS:
*                *handle          - base address
*                ilkn_inst   - instance of ILKN block to access\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*                rx_oduk_zone_hi_th       - no description
*                rx_oduk_zone_lw_th       - no description
*                rx_cpb_zone_hi_th        - no description
*                rx_cpb_zone_lw_th        - no description
*
*  OUTPUTS:
*      None
*
*  RETURNS:        
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_zone_th_cfg(sifd_handle_t *handle, 
                                  sifd_ilkn_inst_t ilkn_inst,
                                  UINT32 rx_oduk_zone_hi_th,
                                  UINT32 rx_oduk_zone_lw_th,
                                  UINT32 rx_cpb_zone_hi_th,
                                  UINT32 rx_cpb_zone_lw_th)
{
    PMC_ENTRY();

    copi_field_RX_CPB_ZONE_HI_TH_set(NULL, handle, ilkn_inst, rx_cpb_zone_hi_th);
    copi_field_RX_CPB_ZONE_LW_TH_set(NULL, handle, ilkn_inst, rx_cpb_zone_lw_th);
    copi_field_RX_ODUK_ZONE_HI_TH_set(NULL, handle, ilkn_inst, rx_oduk_zone_hi_th);
    copi_field_RX_ODUK_ZONE_LW_TH_set(NULL, handle, ilkn_inst, rx_oduk_zone_lw_th);
    
    PMC_RETURN(PMC_SUCCESS);
} /* sifd_zone_th_cfg */

/*******************************************************************************
*  sifd_type_en_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*        This function Configures TX and RX burst short and Burst max
*
*  INPUTS:
*                *handle          - base address
*                ilkn_inst   - instance of ILKN block to access\n
*                                COPI_ILKN1 (0) : ILKN1\n
*                                COPI_ILKN2 (1) : ILKN2
*                rx_type_en        - no description
*                tx_type_en        - no description
*
*  OUTPUTS:
*      None
*
*  RETURNS:   
*      None 
*
*  NOTES:
*
*******************************************************************************/
PUBLIC void sifd_type_en_cfg(sifd_handle_t *handle, 
                             sifd_ilkn_inst_t ilkn_inst,
                             UINT32 rx_type_en,
                             UINT32 tx_type_en)
{
    PMC_ENTRY();

    copi_field_RX_CPB_TYPE_EN_set(NULL, handle, ilkn_inst, rx_type_en);
    copi_field_TX_CPB_TYPE_EN_set(NULL, handle, ilkn_inst, tx_type_en);
    
    PMC_RETURN();
} /* sifd_type_en_cfg */


/*******************************************************************************
* FUNCTION: sifd_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   sifd_handle     - Pointer to sifd handle
*   enable          - Enables/disables interrupt and status support.
*
* OUTPUTS:
*    None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sifd_int_init(sifd_handle_t    *sifd_handle,
                                BOOL              enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i, j;

    PMC_ENTRY();

    for (i = 0; i < 4; i++)
    {
        sifd_dft_reg_SIFD_INT_EN_array_write(NULL, sifd_handle, i, 0xFFFF * enable);
    } 

    /* Disable interrupts by default */
    for (i = 0; i < SIFD_NUM_ILKN_DIR_MAX; i++)
    {
        copi_reg_TX_FIFO_INT_E0_write(NULL, sifd_handle, i, 0x0);
        copi_reg_RX_FIFO_INT_E0_write(NULL, sifd_handle, i, 0x0);
        copi_reg_COM_INT_E0_write(NULL, sifd_handle, i, 0x0);
        copi_reg_SERDES_RX_LOS_INT_E0_write(NULL, sifd_handle, i, 0x0);
        copi_reg_RX_INTF_ERR_INT_E0_write(NULL, sifd_handle, i, 0x0);

        for (j = 0; j < 2; j++)
        {
            copi_reg_TX_INTF_ERR_INT_E0_array_write(NULL, sifd_handle, i, j, 0x0);
        }
    }

    PMC_RETURN(result);
} /* sifd_int_init */

/*******************************************************************************
* sifd_serdes_dsis_rx_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the rx clock and data lines per subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   internal_rx_lane      - the internal rx lane to use.
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
PRIVATE void sifd_serdes_dsis_rx_sel_set(sifd_handle_t *sifd_handle, 
                                         UINT32 subsystem_lane,
                                         UINT32 internal_rx_lane)
{
    PMC_ENTRY();

    /* select internal receive lane clock & data for subsystem lanes (i.e. swizzle) */
    sys_dsis_field_RX_SEL_set(NULL, sifd_handle, subsystem_lane, internal_rx_lane);

    PMC_RETURN();
} /* sifd_serdes_dsis_rx_sel_set */

/*******************************************************************************
* sifd_serdes_dsis_rx_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the rx clock and data lines per subsystem lane and
*   then en/disable the lane.
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   clock_enable          - set to one of:\n
*                           SIFD_CBRC_CLK_ENABLE\n
*                           SIFD_OTN_CLK_ENABLE\n
*                           SIFD_ENET_CLK_ENABLE\n
*                           or to SIFD_CLK_DISABLE in order to disable.
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
PRIVATE void sifd_serdes_dsis_rx_en_set(sifd_handle_t *sifd_handle, 
                                        UINT32 subsystem_lane,
                                        UINT32 clock_enable)
{
    PMC_ENTRY();

    /* enable receive interface of subsystem lanes */
    sys_dsis_field_RX_EN_set(NULL, sifd_handle, subsystem_lane, clock_enable);

    PMC_RETURN();
} /* sifd_serdes_dsis_rx_en_set */

/*******************************************************************************
* sifd_dsis_tx_clk_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function to properly enable/disable the
*   dsis_tx_clk_setting\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   value                 - value to add/subtract from the current tx_clk_set
*   enable                - TRUE: Enable this value in tx_clk_set\n
*                           FALSE: Disable this value in tx_clk_set.
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
PRIVATE void sifd_dsis_tx_clk_set(sifd_handle_t *sifd_handle, 
                                  UINT32 subsystem_lane,
                                  UINT32 value,
                                  BOOL enable)
{
    UINT32 tx_clk_en_value = 0;

    PMC_ENTRY();

    /* get transmit clock current value */
    tx_clk_en_value = sys_dsis_field_TX_CLK_EN_get(NULL, sifd_handle, subsystem_lane);

    if (enable == TRUE) {

        tx_clk_en_value = tx_clk_en_value | value;

    } else {

        tx_clk_en_value = tx_clk_en_value & ~value;
    }

    /* en/disable transmit clock */
    sys_dsis_field_TX_CLK_EN_set(NULL, sifd_handle, subsystem_lane, tx_clk_en_value);

    PMC_RETURN();
} /* sifd_dsis_tx_clk_set */

/*******************************************************************************
* sifd_copi_soft_enable_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This is a helper function that returns the status of ILKN TX and RX slices.
*
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*    ilkn_inst            - SIFD_COPI_ILKN1 Interlaken 1 Instance\n
*                           SIFD_COPI_ILKN2 Interlaken 2 Instance 
*
* OUTPUTS:
*  *rx_enabled            - RX slice status: TRUE: enabled, FALSE: disabled.
*  *tx_enabled            - TX slice status: TRUE: enabled, FALSE: disabled.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void sifd_copi_soft_enable_status_get(sifd_handle_t    *sifd_handle, 
                                              sifd_ilkn_inst_t   ilkn_inst,
                                              BOOL8             *rx_enabled, 
                                              BOOL8             *tx_enabled)
{

    PMC_ENTRY();
    PMC_ASSERT (NULL != sifd_handle, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT (NULL != rx_enabled, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT (NULL != tx_enabled, SIFD_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    *rx_enabled = (copi_field_IL_RX_SOFT_get(NULL, sifd_handle, ilkn_inst) == SIFD_ILKN_DISABLE);
    *tx_enabled = (copi_field_IL_TX_SOFT_get(NULL, sifd_handle, ilkn_inst) == SIFD_ILKN_DISABLE);

    PMC_RETURN();
} /* sifd_copi_soft_enable_status_get */


/*******************************************************************************
* sifd_serdes_tx_if_sel_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Updates TX interface select according to given subsystem type.
*
* INPUTS:
*   *sifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   subsystem_type        - direct the lane to this subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_tx_if_sel_update(sifd_handle_t    *sifd_handle, 
                                              UINT32            subsystem_lane, 
                                              sifd_dsis_type_t  subsystem_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 data_enable = 3;


    PMC_ENTRY();

    PMC_ENTRY();
    
    switch (subsystem_type) {
    case SIFD_DSIS_CBRC:
        data_enable = SIFD_CBRC_DATA_ENABLE;               /* enable CBRC data */
        break;
    case SIFD_DSIS_OTN:
        data_enable = SIFD_OTN_DATA_ENABLE;                /* enable OTN data */
        break;
    case SIFD_DSIS_ENET:
        data_enable = SIFD_ENET_DATA_ENABLE;               /* enable ENET data */
        break;
    default:
        rc = PMC_ERR_INVALID_PARAMETERS;
        break;
    }
    
    if (PMC_SUCCESS == rc) 
    {
        /* select subsystem that drives internal transmit lane before reordering */
        sys_dsis_field_TX_IF_SEL_set(NULL, sifd_handle, subsystem_lane, data_enable);
    }

    PMC_RETURN(rc);
}/*sifd_serdes_tx_if_sel_update*/

/*******************************************************************************
*  sifd_tx_fc_en_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*
*   This function sets the TX_IB_EN field or the TX_OOB_EN field to the value 
*   specified by ilkn_en input.
*
*
*  INPUTS:
*   handle       - pointer to sifd handle instance to be operated on
*   ilkn_inst    - The specific ILKN link instance. See sifd_ilkn_inst_t.
*   is_oob_type  - FALSE: inband, TRUE: out of band
*   ilkn_en      - Enable or disable. See sifd_ilkn_en_t
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      None
*
*  NOTES:
*
*******************************************************************************/
PUBLIC void sifd_tx_fc_en_set(sifd_handle_t *handle,
                              sifd_ilkn_inst_t ilkn_inst,
                              BOOL8 is_oob_type,
                              sifd_ilkn_en_t ilkn_en)
{
    UINT32 update_mask = 0;

    PMC_ENTRY();

    update_mask = (is_oob_type == FALSE) ? SIFD_TX_IB : SIFD_TX_OOB;

    sifd_intf_enable(handle,
                     ilkn_inst,
                     ilkn_en,
                     update_mask);

    PMC_RETURN();

} /* sifd_tx_fc_en_set */

/*******************************************************************************
*  sifd_tx_fc_en_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*
*   This function gets the TX_IB_EN field value or the TX_OOB_EN field value 
*
*
*  INPUTS:
*   handle       - pointer to sifd handle instance to be operated on
*   ilkn_inst    - The specific ILKN link instance. See sifd_ilkn_inst_t.
*   is_oob_type  - FALSE: inband, TRUE: out of band
*
*  OUTPUTS:
*   ilkn_en_ptr  - Pointer to enable value
*
*  RETURNS:
*      None
*
*  NOTES:
*
*******************************************************************************/
PUBLIC void sifd_tx_fc_en_get(sifd_handle_t *handle,
                              sifd_ilkn_inst_t ilkn_inst,
                              BOOL8 is_oob_type,
                              sifd_ilkn_en_t *ilkn_en_ptr)
{
    PMC_ENTRY();

    if(is_oob_type == FALSE)
    {
        *ilkn_en_ptr = (sifd_ilkn_en_t)copi_field_TX_IB_EN_get(NULL, handle, ilkn_inst);
    }
    else
    {
        *ilkn_en_ptr = (sifd_ilkn_en_t)copi_field_TX_OOB_EN_get(NULL, handle, ilkn_inst);
    }

    PMC_RETURN();

} /* sifd_tx_fc_en_get */

/*******************************************************************************
*  sifd_ilkn_copi_lowpwr_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:    
*    This function configures COPI in low power state or not. 
*
*  INPUTS:
*    *handle        - pointer to sifd handle instance to be operated on
*    ilkn_inst      - SIFD_COPI_ILKN1 Interlaken 1 Instance\n 
*                     SIFD_COPI_ILKN2 Interlaken 2 Instance 
*    lowpwr_flag    - TRUE: in low power state; FALSE: not in low power state 
* 
*  OUTPUTS:
*    None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_copi_lowpwr_cfg(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst, 
                                           BOOL8 lowpwr_flag)
{
    UINT32 msk = 0;
    UINT32 ilkn_state = 0;
    UINT32 value = 0;

    PMC_ENTRY();

    msk = ~(0x1 << ilkn_inst);

    ilkn_state = (UINT32)((lowpwr_flag == TRUE) ? PMC_BLOCK_DEFAULT : PMC_BLOCK_ALTERABLE);
    ilkn_state = (ilkn_state << ilkn_inst);

    value = sifd_dft_field_COPI_LOWPWR_CFG_get(NULL, handle);
    value &= msk;
    value |= ilkn_state; 
    sifd_dft_field_COPI_LOWPWR_CFG_set(NULL, handle, ilkn_state);

    value = sifd_dft_field_COPI_RAM_LOWPWR_CFG_get(NULL, handle);
    value &= msk;
    value |= ilkn_state; 
    sifd_dft_field_COPI_RAM_LOWPWR_CFG_set(NULL, handle, ilkn_state);

    handle->var.ilkn_state[ilkn_inst] = (lowpwr_flag == TRUE) ? PMC_BLOCK_DEFAULT : PMC_BLOCK_ALTERABLE;

    PMC_RETURN(PMC_SUCCESS);
}

/* end of file */ 
