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
/*
** Include Files
*/

#include "empei_e_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define UPD_LSC_MAX_POLLS 5


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
const char EMPEI_E_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EMPEI_E_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EMPEI_E_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE void empei_e_var_default_init(empei_e_handle_t *hndl);

#endif
/*
** Public Functions
*/

/*******************************************************************************
* empei_e_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EMPEI_E block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EMPEI_E block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   empei_e_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC empei_e_handle_t *empei_e_ctxt_create(pmc_handle_t *parent, 
                                             UINT32 base_address, 
                                             pmc_sys_handle_t *sys_handle, 
                                             const char *tsb_name )
{
    empei_e_handle_t *empei_e_handle;


    PMC_ENTRY();

    empei_e_handle = (empei_e_handle_t*)PMC_CTXT_CALLOC(sizeof(empei_e_handle_t), parent);

    pmc_handle_init( parent, empei_e_handle, sys_handle, PMC_MID_DIGI_EMPEI_E, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EMPEI_E_LOG_ERR_STRINGS[0], EMPEI_E_LOG_ERR_TABLE_BASE, EMPEI_E_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    empei_e_handle->eclass_handle = eclass_ctxt_create((pmc_handle_t *)empei_e_handle, base_address+BASE_ADDR_EMPEI_E_ECLASS, sys_handle, "ECLASS");
    PMC_CTXT_REGISTER_PTR(&empei_e_handle->eclass_handle, parent);

    PMC_RETURN(empei_e_handle);
} /* empei_e_ctxt_create */

/*******************************************************************************
* empei_e_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EMPEI_E block instance.
*
*
* INPUTS:
*   *empei_e_handle        - pointer to EMPEI_E handle instance
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
PUBLIC void empei_e_ctxt_destroy(empei_e_handle_t *empei_e_handle)
{
    PMC_ENTRY();

    eclass_ctxt_destroy(empei_e_handle->eclass_handle);

    PMC_CTXT_FREE(&empei_e_handle, empei_e_handle);

    PMC_RETURN();
} /* empei_e_ctxt_destroy */

/*******************************************************************************
* empei_e_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EMPEI_E block handle instance.
*
*
* INPUTS:
*   *empei_e_handle        - pointer to EMPEI_E handle instance
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
PUBLIC void empei_e_handle_init(empei_e_handle_t *empei_e_handle)
{
    PMC_ENTRY();

    eclass_handle_init(empei_e_handle->eclass_handle);
    empei_e_var_default_init(empei_e_handle);

    PMC_RETURN();
} /* empei_e_handle_init */

/*******************************************************************************
* empei_e_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on empei_e.
*
* INPUTS:
*   *empei_e_handle             - pointer to empei_e handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state_reg        - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_handle_restart_init(empei_e_handle_t *empei_e_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                             pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(EMPEI_E_LOG_ERR_STRINGS[0], EMPEI_E_LOG_ERR_TABLE_BASE, EMPEI_E_LOG_ERR_COUNT);
    }

    ret_val = eclass_handle_restart_init(empei_e_handle->eclass_handle, restart_init_cfg_ptr, top_energy_state_reg);

    PMC_RETURN(ret_val);
} /* empei_e_handle_restart_init */

/*******************************************************************************
* empei_e_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the EMPEI_E instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to empei_e_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  
*
*
* INPUTS:
*   *hndl                 - pointer to EMPEI_E handle instance
*   energy_state          - enum defining state to set the EMPEI_E module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_init(empei_e_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
        
        PMC_ENTRY();    

        PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0); 

    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Configure EMPEI_E energy state */
        empei_e_eclass_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
        /* Not required non default cfg */
        /* Initialize child blocks */
        ret_val = eclass_init(hndl->eclass_handle, PMC_ENERGY_STATE_RUNNING);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure EMPEI_E energy state */
        empei_e_eclass_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
        /* Not required default var init due to having no members */
        /* Initialize child blocks */
        ret_val = eclass_init(hndl->eclass_handle, PMC_ENERGY_STATE_RESET);
        empei_e_var_default_init(hndl);
    } else {
        PMC_ASSERT(FALSE, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN(ret_val);

} /* empei_e_init */



/*******************************************************************************
* empei_e_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EMPEI_E instance.  
*
*
* INPUTS:
*   *hndl                      - pointer to empei_e handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                       - FALSE:  Resource(s) not in start state \n
*                                TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 empei_e_start_state_test(empei_e_handle_t *hndl)
{
        BOOL8 result = TRUE;
    PMC_ENTRY();
        
    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if(eclass_start_state_test(hndl->eclass_handle) == FALSE)
        result = FALSE;

    /* no local var_t in empei_e to test at this time */

    PMC_RETURN(result);
    
}/* empei_e_start_state_test */

/*******************************************************************************
* empei_e_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EMPEI_E block instance.
*
*
* INPUTS:
*   *empei_e_handle        - pointer to EMPEI_E handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of EMPEI_E.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 empei_e_base_address_get(empei_e_handle_t *empei_e_handle)
{
    PMC_ENTRY();
    PMC_RETURN(empei_e_handle->base.base_address);
} /* empei_e_base_address_get */

/*******************************************************************************
*  empei_e_reset_eclass_nreg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets EMPEI_E's ECLASS configuration (NREG logic)
*
* INPUTS:
*   hndl - handle to the EMPEI_E instance to be operated on
*   reset - TRUE = reset, FALSE = release reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_reset_eclass_nreg(empei_e_handle_t *hndl, BOOL8 reset)
{
    PMC_ENTRY();

    /* reset ECLASS NREG */
    empei_e_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, reset);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_reset_eclass_nreg */

/*******************************************************************************
*  empei_e_reset_eclass_core
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets EMPEI_E's ECLASS internal logic(CORE logic)
*
* INPUTS:
*   hndl - handle to the EMPEI_E instance to be operated on
*   reset - TRUE = reset, FALSE = release reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_reset_eclass_core(empei_e_handle_t *hndl, BOOL8 reset)
{
    PMC_ENTRY();

    /* reset ECLASS CORE */
    empei_e_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, reset);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_reset_eclass_core */

/*******************************************************************************
*  empei_e_activate_datapath
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Activates the EMPEI_E datapath gracefully,once schedulers are configured.
*   Updates the link scheduler calendar for the Extract Path (traffic to Mgmt CPB)
*   the Ingress CPB Interface and the internal scheduler by applying the calendar
*   timeslots previously written.
*   Then polls the LS_UPDATE self-clearing bits until they clear, meaning the scheduler is ready
*   to operate its schedule.
*
* INPUTS:
*   hndl - handle to the EMPEI_E instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL if the polling of the self clearing bits times out
*   PMC_SUCCESS otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_activate_datapath(empei_e_handle_t *hndl)
{
    UINT32 reg_val;
    UINT8 retry_cnt = 0;
    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);

    /* Update CPB scheduler (EXTRACT) */
    empei_e_field_EXTRACT_LS_UPDATE_set(b, hndl, TRUE);

    /* Update internal scheduler */
    empei_e_field_INTL_LS_UPDATE_set(b, hndl, TRUE);

    empei_e_buffer_flush(b);

    /* Poll UPD_LSC register to make sure both bits auto clears before enabling traffic */
    do       
    {
        reg_val = empei_e_reg_UPD_LSC_read(NULL, hndl);
        retry_cnt ++;
    } while(reg_val != 0x0 && retry_cnt < UPD_LSC_MAX_POLLS);
    
    if(retry_cnt >= UPD_LSC_MAX_POLLS)
    {
        PMC_RETURN(EMPEI_E_ERR_POLL_TIMEOUT);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_activate_datapath */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  empei_e_datapath_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EMPEI_E datapath.
*
* INPUTS:
*   hndl      - handle to the EMPEI_E instance to be operated on
*   link      - link to configure
*   data_rate - data rate for the link
*   datapath  - datapath to select for the specified link
*   enable    - TRUE: enable the link. FALSE: disable the link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_datapath_cfg(empei_e_handle_t *hndl, 
                                      UINT32 link, 
                                      empei_e_data_rate_enum_t data_rate,
                                      empei_e_datapath_type_enum_t datapath, 
                                      BOOL8 enable)
{
    PMC_ENTRY();
    /* Configure the EMPEI_E datapath */

    /* empei_e_datapath_sel_cfg(hndl, link, datapath); */

    /* Configure schedulers operating off the CPB clock domain:
       extract link scheduler, insert link scheduler and packet generator link scheduler */

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_datapath_cfg */


/*******************************************************************************
*  empei_e_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup EMPEI_E specified links. 
*   Reset and set default value in configuration registers.
*
* INPUTS:
*   hndl       - handle to the EMPEI_E instance to be operated on
*   num_links  - number of links to cleanup
*   links      - list of link to cleanup indices
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_e_cleanup(empei_e_handle_t *hndl, 
                            UINT32            num_links, 
                            UINT32           *links)
    
{
    UINT32 link_itr;
    UINT32 link_idx;
    empei_e_buffer_t empei_e_buffer[1];

    PMC_ENTRY();

    empei_e_buffer_init(empei_e_buffer, hndl);

    for (link_itr = 0; link_itr < num_links; link_itr++)
    {
        link_idx = links[link_itr];
        /* reset DPI calendar slot and set GFP enable default value */
        empei_e_link_reset( hndl, link_idx);
        empei_e_gfp_enable( hndl, link_idx, FALSE);
        empei_e_link_enable(hndl, link_idx, FALSE);
    }

    empei_e_buffer_flush(empei_e_buffer);

    PMC_RETURN();

} /* empei_e_cleanup */


/*******************************************************************************
*  empei_e_datapath_sel_link_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects between MAC terminated and Transparent datapaths.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*   datapath - datapath to select for the specified link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_datapath_sel_link_cfg(empei_e_handle_t *hndl, 
                                               UINT32 link, 
                                               empei_e_datapath_type_enum_t datapath)
{
    PMC_ENTRY();

    /* Configure the link to the specified datapath */
    empei_e_field_range_DATA_PATH_SELECT_set(NULL, hndl, link, link, datapath);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_datapath_sel_link_cfg */

/*******************************************************************************
*  empei_e_datapath_sel_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects between MAC terminated and Transparent datapaths.
*
* INPUTS:
*   hndl      - handle to the EMPEI_E instance to be operated on
*   datapaths - datapaths to select (UINT32 where bit N is for link N)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_datapath_sel_cfg(empei_e_handle_t *hndl, 
                                          UINT32 datapaths)
{
    PMC_ENTRY();

    /* Configure the link to the specified datapath */
    empei_e_field_DATA_PATH_SELECT_set(NULL, hndl, datapaths);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_datapath_sel_cfg */

/*******************************************************************************
*  empei_e_gfp_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables packet length information transfers from the EMPEI to the ETRANS block.
*   This will enable Length information to be issued to the ETRANS block and then on to the OTN Mapper.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable link for GFP
*              FALSE: disable link for GFP
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_gfp_enable(empei_e_handle_t *hndl, 
                                    UINT32 link, 
                                    BOOL8 enable)
{
    PMC_ENTRY();

    /* Configure the specified link for GFP */
    empei_e_field_range_GFP_LINK_ENABLE_set(NULL, hndl, link, link, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_gfp_enable */


/*******************************************************************************
*  empei_e_gfp_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return status of GFP enable.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE:  link enabled for GFP
*   FALSE: link disable for GFP
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 empei_e_gfp_enable_get(empei_e_handle_t *hndl, 
                                    UINT32            link)
{
    UINT32 reg_val;

    PMC_ENTRY();

    reg_val = empei_e_field_range_GFP_LINK_ENABLE_get(NULL, hndl, link, link);

    PMC_RETURN((1 == reg_val));

} /* empei_e_gfp_enable_get */

/*******************************************************************************
*  empei_e_ts_back_to_back_link_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Define the control mechanism which manages back to back packets that require timestamping by the MAC.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*   ts_back_to_back_type - PTP timestamp back to back mechanism
*   ts_timer             - Back to back timestamp timer value [9:0].
*                          Only used if ts_back_to_back_type == EMPEI_E_TS_TIMER.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ts_back_to_back_link_cfg(empei_e_handle_t *hndl, 
                                                  UINT32 link,
                                                  empei_e_ts_back_to_back_type_t ts_back_to_back_type,
                                                  UINT32 ts_timer)
{
    PMC_ENTRY();

    /* Configure the control mechanism which manages back to back packets that require timestamping by the MAC */
    /* ( Register 0x380 + 4*N(N=0:11): EMPEI120_E - TS Timer (Egress Traffic Manager) */
    /* Register 0x7C: EMPEI120_E - TS Back to Back Control (Egress CPB Interface) */

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_ts_back_to_back_cfg */

/*******************************************************************************
*  empei_e_ts_back_to_back_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Define the control mechanism which manages back to back packets that require timestamping by the MAC.
*
* INPUTS:
*   hndl                 - handle to the EMPEI_E instance to be operated on
*   ts_consume_enables   - Enable Consume PTP timestamp back to back mechanism (UINT32 where bit N is for link N)
*   ts_timer_enables     - Enable PTP timestamp back to back mechanism (UINT32 where bit N is for link N)
*   ts_timers            - Array of 12 links Back to back timestamp timer value [9:0].
*                          Only used if corresponding ts_timer_enables bit == 1.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ts_back_to_back_cfg(empei_e_handle_t *hndl,
                                             UINT32 ts_consume_enables,
                                             UINT32 ts_timer_enables,
                                             UINT32 ts_timers[EMPEI_E_NUM_LINKS])
{

    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);
    
    /* Set the timsetamp consume enables for all links */
    empei_e_field_TS_CONSUME_ENABLE_set(b, hndl, ts_consume_enables);

    /* Set the timsetamp timer enables for all links */
    empei_e_field_TS_TIMER_ENABLE_set(b, hndl, ts_timer_enables);

    /* Set the timer counter values for all links */
    empei_e_lfield_TS_TIMER_set(b, hndl, ts_timers);

    empei_e_buffer_flush(b);
    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_ts_back_to_back_cfg */


/*******************************************************************************
*  empei_e_cpb_link_scheduler_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the link scheduler calendar for the Extract Path (traffic to Mgmt CPB) and
*   the Egress CPB Interface.
*
* INPUTS:
*   hndl            - handle to the EMPEI_E instance to be operated on
*   link            - array of links to configure for each timeslot
*   last_tslot      - last timeslot of calendar that will take effect before the scheduler
*                     update is asserted
*   offset_extract  - offset of the extract interface, relative to top of page reference
*   update_sch      - TRUE: Update scheduler.
*                     FALSE: Do not update scheduler.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_cpb_link_sch_cfg(empei_e_handle_t *hndl, 
                                          UINT32 last_tslot, 
                                          UINT32 offset_extract, 
                                          BOOL8  update_sch, 
                                          UINT32 link[EMPEI_E_NUM_TS])
{
    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);

    /* Assign the specified link to the calendar timeslots */
    empei_e_field_CPB_LINK_SCH_TS0_set ( b, hndl, link[0] );
    empei_e_field_CPB_LINK_SCH_TS1_set ( b, hndl, link[1] );
    empei_e_field_CPB_LINK_SCH_TS2_set ( b, hndl, link[2] );
    empei_e_field_CPB_LINK_SCH_TS3_set ( b, hndl, link[3] );
    empei_e_field_CPB_LINK_SCH_TS4_set ( b, hndl, link[4] );
    empei_e_field_CPB_LINK_SCH_TS5_set ( b, hndl, link[5] );
    empei_e_field_CPB_LINK_SCH_TS6_set ( b, hndl, link[6] );
    empei_e_field_CPB_LINK_SCH_TS7_set ( b, hndl, link[7] );
    empei_e_field_CPB_LINK_SCH_TS8_set ( b, hndl, link[8] );
    empei_e_field_CPB_LINK_SCH_TS9_set ( b, hndl, link[9] );
    empei_e_field_CPB_LINK_SCH_TS10_set( b, hndl, link[10] );
    empei_e_field_CPB_LINK_SCH_TS11_set( b, hndl, link[11] );

    empei_e_field_CPB_LINK_SCH_LAST_TIMESLOT_set(b, hndl, last_tslot);
    empei_e_field_CPB_LINK_SCH_OFFSET_EXTRACT_set(b, hndl, offset_extract);

    if (update_sch) {
        /* Update CPB scheduler (EXTRACT) */
        empei_e_field_EXTRACT_LS_UPDATE_set(b, hndl, TRUE);

    } /* end if */

    empei_e_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_cpb_link_scheduler_cfg */

/*******************************************************************************
*  empei_e_internal_link_sch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the link scheduler calendar for the Egress CPB Interface.
*
* INPUTS:
*   hndl       - handle to the EMPEI_E instance to be operated on
*   link       - array of links to configure for each timeslot
*   last_tslot - last timeslot of calendar that will take effect before the scheduler
*                update is asserted
*   update_sch - TRUE: Update scheduler.
*                FALSE: Do not update scheduler.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_internal_link_sch_cfg(empei_e_handle_t *hndl, 
                                               UINT32 last_tslot, 
                                               BOOL8 update_sch, 
                                               UINT32 link[EMPEI_E_NUM_TS])
{
    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);

    /* Assign the specified link to the calendar timeslots */
    empei_e_field_INTL_LINK_SCH_TS0_set ( b, hndl, link[0] );
    empei_e_field_INTL_LINK_SCH_TS1_set ( b, hndl, link[1] );
    empei_e_field_INTL_LINK_SCH_TS2_set ( b, hndl, link[2] );
    empei_e_field_INTL_LINK_SCH_TS3_set ( b, hndl, link[3] );
    empei_e_field_INTL_LINK_SCH_TS4_set ( b, hndl, link[4] );
    empei_e_field_INTL_LINK_SCH_TS5_set ( b, hndl, link[5] );
    empei_e_field_INTL_LINK_SCH_TS6_set ( b, hndl, link[6] );
    empei_e_field_INTL_LINK_SCH_TS7_set ( b, hndl, link[7] );
    empei_e_field_INTL_LINK_SCH_TS8_set ( b, hndl, link[8] );
    empei_e_field_INTL_LINK_SCH_TS9_set ( b, hndl, link[9] );
    empei_e_field_INTL_LINK_SCH_TS10_set( b, hndl, link[10] );
    empei_e_field_INTL_LINK_SCH_TS11_set( b, hndl, link[11] );

    empei_e_field_INTL_LINK_SCH_LAST_TIMESLOT_set(b, hndl, last_tslot);

    if (update_sch) {
        empei_e_field_INTL_LS_UPDATE_set(b, hndl, TRUE);
    } /* end if */

    empei_e_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_internal_link_sch_cfg */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  empei_e_sys_fcs_is_enabled   
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Determine the status of the FCS insertion for packets received from the 
*    Data CPB and destined to the MAC.
*    Note that the FCS will be inserted by the MAC.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*            - TRUE: FCS insertion is enabled at the MAC for this link.
*              FALSE: FCS insertion is disabled at the MAC for this link
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 empei_e_sys_fcs_is_enabled(empei_e_handle_t *hndl, 
                                       UINT32 link)
{
    PMC_ENTRY();

    /* Determine the FCS insertion state for the link at the MAC */
    BOOL8 is_enabled = empei_e_field_range_SYS_FCS_ENABLE_get(NULL, hndl, link, link);

    PMC_RETURN(is_enabled);

} /* empei_e_sys_fcs_is_enabled */

/*******************************************************************************
*  empei_e_sys_fcs_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enable/disable FCS insertion for packets received from the Data CPB and destined to the MAC.
*    Note that the FCS will be inserted by the MAC.
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*   enable   - TRUE: enable FCS insertion at the MAC for this packet
*              FALSE: disable FCS insertion at the MAC for this packet
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_sys_fcs_enable(empei_e_handle_t *hndl, 
                                        UINT32 link, 
                                        BOOL8 enable)
{
    PMC_ENTRY();

    /* Enable/disable FCS insertion for the link at the MAC */
    empei_e_field_range_SYS_FCS_ENABLE_set(NULL, hndl, link, link, enable);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_sys_fcs_enable */

/*******************************************************************************
*  empei_e_data_cpb_flow_ctrl_link_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure channel mapper block to associate each Data CPB channel to an appropriate link priority.
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   link          - link to configure
*   enable        - when TRUE PAUSE frame reception at the RX MAC will cause the CPB
*                   to stop sending data at the next packet boundary.
*                   When FALSE, then the reception of PAUSE frames on link will 
*                   have no effect on the data transmission from the CPB.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_data_cpb_flow_ctrl_link_cfg(empei_e_handle_t *hndl, 
                                                     UINT32            link, 
                                                     BOOL8             enable)
{
    PMC_ENTRY();

    /* Configure the associated link priorities for the Data CPB channel */
    /* Register 0x400 + 4*N(N=0:95): EMPEI120_E - System Data Path Priority Mapper (Egress CPB Interface) */
    empei_e_lfield_range_SYS_PRIORITY_MAP_set(NULL, 
                                              hndl, 
                                              EMPEI_E_SYS_MAP_LINK_OFFSET * link * 32, 
                                              EMPEI_E_SYS_MAP_LINK_OFFSET * link * 32, 
                                              TRUE == enable ? 1 : 0);
    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_data_cpb_flow_ctrl_link_cfg */

/*******************************************************************************
*  empei_e_pkt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure packet generator as specified
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   pkt_size      - packet size in bytes
*   pyld_val      - repeating byte value in the payload
*   pkt_interval  - defines the Packet SOP to Packet SOP interval
*   da_hi         - Ethernet DA value (Upper 16-bits)
*   da_low        - Ethernet DA value (Lower 32-bits)
*   sa_hi         - Ethernet SA value (Upper 16-bits)
*   sa_low        - Ethernet SA value (Lower 32-bits)
*   len_etype     - Ethernet Length/Ethertype field (16-bits)
*   vlan_out      - Outer VLAN tag
*   vlan_in       - Inner VLAN tag
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pkt_gen_cfg(empei_e_handle_t *hndl, 
                                     UINT32 pkt_size, 
                                     UINT32 pyld_val, 
                                     UINT32 pkt_interval, 
                                     UINT32 da_hi, 
                                     UINT32 da_low, 
                                     UINT32 sa_hi, 
                                     UINT32 sa_low,
                                     UINT32 len_etype, 
                                     UINT32 vlan_out, 
                                     UINT32 vlan_in)
{

    empei_e_buffer_t b[1];
    UINT32 eop_size;

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);
    
    empei_e_field_PKT_GEN_LEN_set(b, hndl, pkt_size);

    /* Calculate the eop_size and words_per_pkt from the pkt_size */

    if(pkt_size % 48 == 0){
        eop_size = 48;
    }
    else {
        eop_size = pkt_size % 48;
    }
    empei_e_field_PKT_GEN_EOP_SIZE_set(b, hndl, eop_size);

    empei_e_field_PKT_GEN_WORDS_PER_PKT_set(b, hndl, (pkt_size+47)/48);

    /* Configure the packet generator using registers 0xD0-0xF4 */
    empei_e_field_PKT_GEN_PLD_set(b, hndl, pyld_val);
    empei_e_field_PKT_GEN_PKT_INT_set(b, hndl, pkt_interval);
    empei_e_field_PKT_GEN_DA_UPPER_set(b, hndl, da_hi);
    empei_e_field_PKT_GEN_DA_LOW_set(b, hndl, da_low);
    empei_e_field_PKT_GEN_SA_UPPER_set(b, hndl, sa_hi);
    empei_e_field_PKT_GEN_SA_LOW_set(b, hndl, sa_low);
    empei_e_field_PKT_GEN_LEN_TYPE_set(b, hndl, len_etype);
    empei_e_field_PKT_GEN_VLAN_OUT_set(b, hndl, vlan_out);
    empei_e_field_PKT_GEN_VLAN_IN_set(b, hndl, vlan_in);


    empei_e_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_pkt_gen_cfg */

/*******************************************************************************
*  empei_e_pkt_gen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Gets packet generator settings
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*
* OUTPUTS:
*   pkt_size      - packet size in bytes
*   pyld_val      - repeating byte value in the payload
*   pkt_interval  - defines the Packet SOP to Packet SOP interval
*   da_hi         - Ethernet DA value (Upper 16-bits)
*   da_low        - Ethernet DA value (Lower 32-bits)
*   sa_hi         - Ethernet SA value (Upper 16-bits)
*   sa_low        - Ethernet SA value (Lower 32-bits)
*   len_etype     - Ethernet Length/Ethertype field (16-bits)
*   vlan_out      - Outer VLAN tag
*   vlan_in       - Inner VLAN tag
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pkt_gen_cfg_get(empei_e_handle_t *hndl,
                                         UINT32 *pkt_size,
                                         UINT32 *pyld_val,
                                         UINT32 *pkt_interval,
                                         UINT32 *da_hi,
                                         UINT32 *da_low,
                                         UINT32 *sa_hi,
                                         UINT32 *sa_low,
                                         UINT32 *len_etype,
                                         UINT32 *vlan_out,
                                         UINT32 *vlan_in)
{

    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);

    *pkt_size     = empei_e_field_PKT_GEN_LEN_get(b, hndl);
    *pyld_val     = empei_e_field_PKT_GEN_PLD_get(b, hndl);
    *pkt_interval = empei_e_field_PKT_GEN_PKT_INT_get(b, hndl);
    *da_hi        = empei_e_field_PKT_GEN_DA_UPPER_get(b, hndl);
    *da_low       = empei_e_field_PKT_GEN_DA_LOW_get(b, hndl);
    *sa_hi        = empei_e_field_PKT_GEN_SA_UPPER_get(b, hndl);
    *sa_low       = empei_e_field_PKT_GEN_SA_LOW_get(b, hndl);
    *len_etype    = empei_e_field_PKT_GEN_LEN_TYPE_get(b, hndl);
    *vlan_out     = empei_e_field_PKT_GEN_VLAN_OUT_get(b, hndl);
    *vlan_in      = empei_e_field_PKT_GEN_VLAN_IN_get(b, hndl);

    empei_e_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_pkt_gen_cfg_get */

/*******************************************************************************
*  empei_e_pkt_gen_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure packet generator link and enable/disable
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   link          - link to generate packets on
*   enable        - TRUE: Enable packet generation on specified link
*                   FALSE: Disable packet generation on specified link
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pkt_gen_enable(empei_e_handle_t *hndl, 
                                        UINT32 link, 
                                        BOOL8 enable)
{                               
    int polling_usec = 250;
    UINT32 busy_flag;

    PMC_ENTRY();


    /* Configure link and enable/disable Packet Generator */
    /* Register 0xF4: EMPEI120_E - Packet Generator Enable */
    /* Packet Generator busy flag added (after disable SW must wait until busy 
       deasserted before being able to use link). */

    if (enable)
    {
        /* Wait until busy flag is de-asserted */
        do {
            busy_flag = empei_e_field_PKT_GEN_BUSY_get(NULL, hndl);
            if (busy_flag == 0) 
                break;
            PMC_Q_USLEEP(hndl,1);
        } while (--polling_usec > 0);
        
        if (busy_flag != 0)
        {
            PMC_RETURN(EMPEI_E_LOG_ERR_PKT_GEN_BUSY);
        }    
    }
    
    empei_e_field_PKT_GEN_LINK_set(NULL, hndl, link);
    empei_e_field_PKT_GEN_ENABLE_set(NULL, hndl, enable ? 1 : 0);
    

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_pkt_gen_enable */

/*******************************************************************************
*  empei_e_pkt_gen_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function returns the status of the packet generator.
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*
* OUTPUTS:
*   *enabled_ptr  - Set to true if the packet generator is enabled.
*   *link_no_ptr  - Set to the link no the packet generator is active on.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pkt_gen_status_get(empei_e_handle_t *hndl, 
                                        UINT32 *link_no_ptr, 
                                        BOOL8 *enabled_ptr)
{                               
    PMC_ENTRY();

    /* Configure link and enable/disable Packet Generator */
    /* Register 0xF4: EMPEI120_E - Packet Generator Enable */
    /* Packet Generator busy flag added (after disable SW must wait until busy deasserted before being able to use link). */
    *enabled_ptr = empei_e_field_PKT_GEN_ENABLE_get(NULL, hndl);
    *link_no_ptr = empei_e_field_PKT_GEN_LINK_get(NULL, hndl);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_pkt_gen_status_get */

/*******************************************************************************
*  empei_e_drr_quantum_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Obtain the DRR quantums for the SYS and INS paths for the specified link
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to obtain information from 
*
* OUTPUTS:
*   *ins_quantum   - quantum for the Insert path
*   *sys_quantum   - quantum for the System path
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_e_drr_quantum_get(empei_e_handle_t *hndl, 
                                    UINT32 link, 
                                    UINT32* sys_quantum, 
                                    UINT32* ins_quantum)
{
    PMC_ENTRY();

    /* Configure system and insert path quantums */
    *sys_quantum = empei_e_field_SYS_QUANTUM_get(NULL, hndl, link);
    *ins_quantum = empei_e_field_INS_QUANTUM_get(NULL, hndl, link);

    PMC_RETURN();

} /* empei_e_drr_quantum_get */

/*******************************************************************************
*  empei_e_drr_quantum_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure the DRR quantums for the SYS and INS paths for the specified link
*
* INPUTS:
*   hndl     - handle to the EMPEI_E instance to be operated on
*   link     - link to configure
*   ins_quantum   - quantum for the Insert path
*   sys_quantum   - quantum for the System path
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_drr_quantum_cfg(empei_e_handle_t *hndl, 
                                         UINT32 link, 
                                         UINT32 sys_quantum, 
                                         UINT32 ins_quantum)
{
    empei_e_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_e_buffer_init(b, hndl);

    /* Configure system and insert path quantums */
    empei_e_field_SYS_QUANTUM_set(b, hndl, link, sys_quantum);
    empei_e_field_INS_QUANTUM_set(b, hndl, link, ins_quantum);

    empei_e_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_drr_quantum_cfg */

/*******************************************************************************
*  empei_e_link_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*    reset the link specified by the link number
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   link          - link id 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_e_link_reset(empei_e_handle_t *hndl, 
                                    UINT32 link) 
{
  UINT32 reg_value = 0;

  PMC_ENTRY();

  reg_value = ( 1 << link );
  empei_e_field_LINK_RESET_set( NULL, hndl, reg_value);

  
  
  PMC_RETURN();

} /* empei_e_link_reset */


/*******************************************************************************
*  empei_e_link_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*    enable/disable specified link so that incoming data to flow through EMPEI Links (put in operational state) 
*
* INPUTS:
*   hndl          - handle to the EMPei_e instance to be operated on
*   link          - link id
*   enable        - enable flag (BOOL)
*                   TRUE: enable link (de-assert Link XOFFs towards the MCPB and DCPB)
*                   FALSE: disable link (assert Link XOFFs towards the MCPB and DCPB)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_link_enable(empei_e_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 enable) 
{
  PMC_ENTRY();

  empei_e_field_range_LINK_ENABLE_set(NULL, hndl, link, link, enable);

  PMC_RETURN(PMC_SUCCESS);

} /* empei_e_link_enable */


/*******************************************************************************
*  empei_e_da_sa_swap_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure DA SA swap mode  as specified
*
* INPUTS:
*   hndl          - handle to the EMPEI_E instance to be operated on
*   link          - link for which to enable DA-SA swap
*   enable        - DA-SA swap enable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_da_sa_swap_cfg(empei_e_handle_t *hndl, 
                                        UINT32 link, BOOL8 enable )
{

    PMC_ENTRY();

    empei_e_field_range_DA_SA_SWAP_set(NULL, hndl, link, link, enable);
    PMC_RETURN(PMC_SUCCESS);

} /* empei_e_da_sa_swap_cfg */

/*******************************************************************************
*  empei_e_eclass_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the ECLASS block energy state from EMPEI_E local register fields. 
*
* INPUTS:
*   hndl           - handle to the ECLASS instance to be operated on
*   energy_state   - energy_state
*
* OUTPUTS:
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_e_eclass_energy_state_reg_set ( empei_e_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            empei_e_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, 0);
            empei_e_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, 0);
            break;            
        case PMC_ENERGY_STATE_RESET:
            empei_e_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, 1);
            empei_e_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, 1);
            break;            
        default:
            PMC_ASSERT(FALSE, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
    
} /* empei_e_eclass_energy_state_reg_set */


/*******************************************************************************
*  empei_e_ext_pmon_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables an Extract PMON for a particular link and 
*   classification code.
*
* INPUTS:
*   *hndl        - EMPEI_E handle
*   link         - link number
*   tag          - classification tag.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ext_pmon_enable(empei_e_handle_t *hndl, 
                                         UINT32 link, 
                                         UINT32 tag)
{
    UINT32 counter_no;
    UINT8 *pmon_link_map;
    UINT8 *pmon_tag_map;
    UINT32 *pmons;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_link_map = hndl->var.ext_pmon_link_map;
    pmon_tag_map = hndl->var.ext_pmon_tag_map;
    pmons = hndl->var.ext_pmon;

    /* find a free counter */
    for(counter_no=0; counter_no < EMPEI_E_NUM_EXT_PMON; counter_no++) 
    {
        if(pmon_link_map[counter_no] == 0 && pmon_tag_map[counter_no] == 0)
            break;
    }
   
    if (counter_no >= EMPEI_E_NUM_EXT_PMON) 
    {
        PMC_RETURN(EMPEI_E_LOG_ERR_NO_AVAIL_EXT_PMON);
    } 
        
    /* Mark the counter as used */
    pmon_link_map[counter_no] = link;
    pmon_tag_map[counter_no] = tag;

    /* Enable the counter */
    empei_e_field_EXT_PMON_CFG_set(NULL, hndl, counter_no, (link << 5) | tag);
    empei_e_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 1);
    PMC_RETURN(PMC_SUCCESS);

} /*empei_e_ext_pmon_enable */

/*******************************************************************************
*  empei_e_ext_pmon_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
* This function disables an Extract PMON for a particular link and 
* classification code.
*
* INPUTS:
*   *hndl        - handle to the EMPEI_E instance to be operated on
*   link         - link number
*   tag          - classification tag.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ext_pmon_disable(empei_e_handle_t *hndl, 
                                          UINT32 link, 
                                          UINT32 tag)
{
    UINT32 counter_no;
    UINT8 *pmon_link_map;
    UINT8 *pmon_tag_map;
    UINT32 *pmons;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_link_map = hndl->var.ext_pmon_link_map;
    pmon_tag_map = hndl->var.ext_pmon_tag_map;
    pmons = hndl->var.ext_pmon;

    for(counter_no=0; counter_no < EMPEI_E_NUM_EXT_PMON; counter_no++) 
    {
        if(pmon_link_map[counter_no] == link && pmon_tag_map[counter_no] == tag)
            break;
    }
    if (counter_no >= EMPEI_E_NUM_EXT_PMON) 
    {
        /* link/classification code not found */
        PMC_RETURN(EMPEI_E_LOG_ERR_EXT_PMON_NOT_FOUND);
    } 
    
    /* mark the counter as free */
    pmon_link_map[counter_no] = 0;
    pmon_tag_map[counter_no] = 0;

    /* disable the counter */
    empei_e_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 0); 
    PMC_RETURN(PMC_SUCCESS);

} /*empei_e_ext_pmon_disable */

/*******************************************************************************
*  empei_e_ext_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Retrieve all PMONs for a particular link. The counter values 
*                are stored in counters_ptr and the corresponding counter tag
*                is stored in the tags_ptr. The tags_ptr entry is set to 0 if
*                the corresponding PMON is not enabled for the 'link'.
*
* INPUTS:
*   *hndl            - EMPEI_E handle
*   link             - link number
*   accumulate       - Accumulate counter when TRUE.
*
* OUTPUTS:
*   **counters_pptr - Pointer to an array of counters. An entry in the array is 
*                     valid if the corresponding entry in the tags_ptr contains
*                     a tag (tag > 0).
*   **tags_pptr     - Pointer to an array of tags
*
* RETURNS:
*   PMC_SUCCESS or error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ext_pmon_get(empei_e_handle_t *hndl, 
                                       UINT32 link,
                                       BOOL8 accumulate,
                                       UINT32 **counters_pptr,
                                       UINT8 **tags_pptr)

{
    UINT32 counter_no;
    UINT32 val;
    UINT8 *link_map;
    UINT8 *tag_map;
    UINT8 *link_tag_map;
    UINT32 *pmons;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    link_map = hndl->var.ext_pmon_link_map;
    tag_map = hndl->var.ext_pmon_tag_map;
    link_tag_map = hndl->var.ext_pmon_link_tag_map;
    pmons = hndl->var.ext_pmon;

    /*
     * Read all the Ext PMONs for 'link'.  The tags_ptr array is 
     * updated with the classification code corresponding to the PMON.
     * If the PMON is not enabled for the 'link', the entry is set to 0.
     */
    for(counter_no=0; counter_no < EMPEI_E_NUM_EXT_PMON ; counter_no++) 
    {
        if(link_map[counter_no] == link)
        {
            val = empei_e_reg_EXT_PMON_PKT_CNT_array_read(NULL, hndl, counter_no);
            #ifdef PMC_SW_SIMULATION
            val = counter_no + 1;
            #endif
            pmons[counter_no] = (accumulate ? pmons[counter_no] + val : val);
            link_tag_map[counter_no] = tag_map[counter_no];
        }
        else
        {
            link_tag_map[counter_no] = 0;
        }
    }
    *counters_pptr = pmons;
    *tags_pptr = link_tag_map;
    PMC_RETURN(PMC_SUCCESS);

} /*empei_e_ext_pmon_get */

/*******************************************************************************
*  empei_e_ext_pmon_disable_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Disable all extract PMONs for a particular link.
*
* INPUTS:
*   *hndl        - EMPEI_E handle
*   link         - link number
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_ext_pmon_disable_all(empei_e_handle_t *hndl, UINT32 link)
{
    UINT32 counter_no;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for(counter_no = 0; counter_no < EMPEI_E_NUM_EXT_PMON ; counter_no++) 
    {
        if(hndl->var.ext_pmon_link_map[counter_no] == link)
        {
            /* mark the counter as free */
            hndl->var.ext_pmon_link_map[counter_no] = 0;
            hndl->var.ext_pmon_tag_map[counter_no] = 0;
            hndl->var.ext_pmon[counter_no] = 0;
        
            /* disable the counter */
            empei_e_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 0); 
        }
    }
    PMC_RETURN(PMC_SUCCESS);
} /* empei_e_ext_pmon_disable_all */

/*******************************************************************************
*  empei_e_pmon_latch
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Latch and clear PMONs.
*
* INPUTS:
*   hndl         - EMPEI_E handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_pmon_latch(empei_e_handle_t *hndl)
{
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    empei_e_field_PMON_CNT_CLR_set(NULL, hndl, 1); 

    PMC_RETURN(PMC_SUCCESS);
} /* empei_e_pmon_latch */

/*******************************************************************************
* empei_e_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns EMPEI_E links status: enabled or disabled.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EMPEI_E handle instance
*                                                                             
* OUTPUTS:
*   *actv_link         - list of link status.
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void empei_e_activated_link_get(empei_e_handle_t   *hndl,
                                       BOOL8              *actv_link)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < EMPEI_E_NUM_LINKS; itr++)
    {
        actv_link[itr] = empei_e_field_range_LINK_ENABLE_get(NULL, hndl, itr, itr) != 0 ? TRUE : FALSE;
    }

    PMC_RETURN();
}  /* empei_e_activated_link_get */



/*******************************************************************************
* FUNCTION: empei_e_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   empei_e_handle    - Pointer to subsystem handle
*   link              - Link
*   non_null_ptr      - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_int_validate(empei_e_handle_t *empei_e_handle,
                                      UINT32            link,
                                      void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != empei_e_handle, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);
   
    if (link >= EMPEI_E_NUM_LINKS)
    {
        result = EMPEI_E_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = EMPEI_E_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* empei_e_int_validate */


/*******************************************************************************
* FUNCTION: empei_e_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   empei_e_handle    - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_int_chnl_enable(empei_e_handle_t    *empei_e_handle,
                                         UINT32               link,
                                         empei_e_int_chnl_t  *int_table_ptr,
                                         empei_e_int_chnl_t  *int_en_table_ptr,
                                         BOOL8                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_e_int_validate(empei_e_handle, link, int_en_table_ptr);

    #ifdef EMPEI_E_INT
    #undef EMPEI_E_INT
    #endif
    #define EMPEI_E_INT    UTIL_GLOBAL_INT_ENABLE_RANGE
    
    

    EMPEI_E_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* empei_e_int_chnl_enable */




/*******************************************************************************
* FUNCTION: empei_e_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   empei_e_handle    - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with bits
*                       set to clear interrupts. If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_int_chnl_clear(empei_e_handle_t    *empei_e_handle,
                                        UINT32               link,
                                        empei_e_int_chnl_t  *int_table_ptr,
                                        empei_e_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_e_int_validate(empei_e_handle, link, int_en_table_ptr);

    #ifdef EMPEI_E_INT
    #undef EMPEI_E_INT
    #endif
    #define EMPEI_E_INT UTIL_GLOBAL_INT_CLEAR_RANGE
    

    EMPEI_E_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* empei_e_int_chnl_clear */




/*******************************************************************************
* FUNCTION: empei_e_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   empei_e_handle    - Pointer to subsystem handle
*   link              - Link
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_int_chnl_retrieve(empei_e_handle_t    *empei_e_handle,
                                           UINT32               link,
                                           empei_e_int_chnl_t  *filt_table_ptr,
                                           empei_e_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;
    empei_e_buffer_t    *empei_e_buffer_ptr = NULL;

    PMC_ENTRY();

    result = empei_e_int_validate(empei_e_handle, link, int_table_ptr);

    
    
    #ifdef EMPEI_E_INT
    #undef EMPEI_E_INT
    #endif
    #define EMPEI_E_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE
    

    EMPEI_E_INT_TABLE_DEFINE();
    
    
    #ifdef EMPEI_E_STATUS
    #undef EMPEI_E_STATUS
    #endif
    #define EMPEI_E_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    

    EMPEI_E_STATUS_TABLE_DEFINE();
    
    

    PMC_RETURN(result);
} /* empei_e_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: empei_e_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   empei_e_handle    - Pointer to subsystem handle
*   link              - Link
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_e_int_chnl_enabled_check(empei_e_handle_t     *empei_e_handle,
                                                UINT32                link,
                                                empei_e_int_chnl_t   *int_en_table_ptr,
                                                BOOL8                *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_e_int_validate(empei_e_handle, link, int_found_ptr);


    #ifdef EMPEI_E_INT
    #undef EMPEI_E_INT
    #endif
    #define EMPEI_E_INT UTIL_GLOBAL_INT_CHECK_RANGE
    

    EMPEI_E_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* empei_e_int_chnl_enabled_check */



/*
** Private Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
*  empei_e_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize EMPEI_E var_t struct.
*
* INPUTS:
*   *hndl      - pointer to EMPEI_E handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void empei_e_var_default_init(empei_e_handle_t *hndl)
{
    UINT32 counter_no;
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_E_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Reset stored EXT PMONs and maps */
    for(counter_no=0; counter_no < EMPEI_E_NUM_EXT_PMON ; counter_no++) 
    {
        hndl->var.ext_pmon_link_map[counter_no] = 0;
        hndl->var.ext_pmon_tag_map[counter_no] = 0;
        hndl->var.ext_pmon[counter_no] = 0;
    }
   
    PMC_RETURN();
}/* empei_e_var_default_init*/

#endif
/*
** End of file
*/
