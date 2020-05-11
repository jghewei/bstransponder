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

#include "empei_i_loc.h"

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
const char EMPEI_I_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EMPEI_I_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EMPEI_I_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE void empei_i_var_default_init(empei_i_handle_t *hndl);

#endif

/*
** Public Functions
*/

/*******************************************************************************
* empei_i_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a EMPEI_I block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EMPEI_I block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   empei_i_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC empei_i_handle_t *empei_i_ctxt_create(pmc_handle_t *parent, 
                                             UINT32 base_address, 
                                             pmc_sys_handle_t *sys_handle, 
                                             const char *tsb_name )
{
    empei_i_handle_t *empei_i_handle;


    PMC_ENTRY();

    empei_i_handle = (empei_i_handle_t*)PMC_CTXT_CALLOC(sizeof(empei_i_handle_t), parent);


    pmc_handle_init( parent, empei_i_handle, sys_handle, PMC_MID_DIGI_EMPEI_I, 
                     tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(EMPEI_I_LOG_ERR_STRINGS[0], 
                                   EMPEI_I_LOG_ERR_TABLE_BASE, EMPEI_I_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    empei_i_handle->eclass_handle = eclass_ctxt_create(
        (pmc_handle_t *)empei_i_handle, base_address+BASE_ADDR_EMPEI_I_ECLASS, 
        sys_handle, "ECLASS");
    PMC_CTXT_REGISTER_PTR(&empei_i_handle->eclass_handle, parent);

    
    PMC_RETURN(empei_i_handle);
} /* empei_i_ctxt_create */

/*******************************************************************************
* empei_i_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EMPEI_I block instance.
*
*
* INPUTS:
*   *empei_i_handle        - pointer to EMPEI_I handle instance
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
PUBLIC void empei_i_ctxt_destroy(empei_i_handle_t *empei_i_handle)
{

    PMC_ENTRY();

    eclass_ctxt_destroy(empei_i_handle->eclass_handle);

    PMC_CTXT_FREE(&empei_i_handle, empei_i_handle);

    PMC_RETURN();
} /* empei_i_ctxt_destroy */

/*******************************************************************************
* empei_i_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a EMPEI_I block handle instance.
*
*
* INPUTS:
*   *empei_i_handle        - pointer to EMPEI_I handle instance
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
PUBLIC void empei_i_handle_init(empei_i_handle_t *empei_i_handle)
{
    PMC_ENTRY();

    eclass_handle_init(empei_i_handle->eclass_handle);
    empei_i_var_default_init(empei_i_handle);

    PMC_RETURN();
} /* empei_i_handle_init */

/*******************************************************************************
* empei_i_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on empei_i.
*
* INPUTS:
*   *empei_i_handle             - pointer to empei_i handle instance
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
PUBLIC PMC_ERROR empei_i_handle_restart_init(empei_i_handle_t *empei_i_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                             pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(EMPEI_I_LOG_ERR_STRINGS[0], EMPEI_I_LOG_ERR_TABLE_BASE, EMPEI_I_LOG_ERR_COUNT);
    }

    ret_val = eclass_handle_restart_init(empei_i_handle->eclass_handle, restart_init_cfg_ptr, top_energy_state_reg);

    PMC_RETURN(ret_val);
} /* empei_i_handle_restart_init */

/*******************************************************************************
* empei_i_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the EMPEI_I instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to empei_i_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  
*
*
* INPUTS:
*   *hndl                 - pointer to EMPEI_I handle instance
*   energy_state          - enum defining state to set the EMPEI_I module to
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
PUBLIC PMC_ERROR empei_i_init(empei_i_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0); 

    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Configure EMPEI_I energy state*/
        empei_i_eclass_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
        /* Configure non default values */
        empei_i_block_non_default_cfg(hndl);
        /* Initialize child blocks */
        ret_val = eclass_init(hndl->eclass_handle, PMC_ENERGY_STATE_RUNNING);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure EMPEI_I energy state*/
        empei_i_eclass_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
        /* Not required default var init due to having no memeber */
        /* Initialize child blocks */
        ret_val = eclass_init(hndl->eclass_handle, PMC_ENERGY_STATE_RESET);
        empei_i_var_default_init(hndl);
    }
    else {
        PMC_ASSERT(FALSE, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
  
    PMC_RETURN(ret_val);
} /* empei_i_init */

/*******************************************************************************
* empei_i_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EMPEI_I instance.  
*
*
* INPUTS:
*   *hndl                      - pointer to empei_i handle instance
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
PUBLIC BOOL8 empei_i_start_state_test(empei_i_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if(eclass_start_state_test(hndl->eclass_handle) == FALSE)
        PMC_RETURN(FALSE);

    /* no local var_t in empei_i to test at this time */

    PMC_RETURN(TRUE);
}/* empei_i_start_state_test */

/*******************************************************************************
* empei_i_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a EMPEI_I block instance.
*
*
* INPUTS:
*   *empei_i_handle        - pointer to EMPEI_I handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of EMPEI_I.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 empei_i_base_address_get(empei_i_handle_t *empei_i_handle)
{
    PMC_ENTRY();
    PMC_RETURN(empei_i_handle->base.base_address);
} /* empei_i_base_address_get */

/*******************************************************************************
*  empei_i_reset_eclass_nreg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets EMPEI_I's ECLASS configuration (NREG logic)
*
* INPUTS:
*   hndl - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_reset_eclass_nreg(empei_i_handle_t *hndl, BOOL8 reset)
{
    PMC_ENTRY();

    /* reset ECLASS NREG */
    empei_i_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, reset);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_reset_eclass_nreg */

/*******************************************************************************
*  empei_i_reset_eclass_core
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets EMPEI_I's ECLASS internal logic(CORE logic)
*
* INPUTS:
*   hndl - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_reset_eclass_core(empei_i_handle_t *hndl, BOOL8 reset)
{
    PMC_ENTRY();

    /* reset ECLASS CORE */
    empei_i_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, reset);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_reset_eclass_core */

/*******************************************************************************
*  empei_i_activate_datapath
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Activates the EMPEI_I datapath gracefully,once schedulers are configured.
*   Updates the link scheduler calendar for the Extract Path (traffic to Mgmt CPB)
*   the Ingress CPB Interface and the internal scheduler by applying the calendar
*   timeslots previously written.
*   Then polls the LS_UPDATE self-clearing bits until they clear, meaning the scheduler is ready
*   to operate its schedule.
*
* INPUTS:
*   hndl            - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_activate_datapath(empei_i_handle_t *hndl)
{
    UINT32 reg_val;
    UINT8 retry_cnt = 0;
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    /* Update CPB scheduler (EXTRACT) */
    empei_i_field_EXTRACT_LS_UPDATE_set(b, hndl, TRUE);

    /* Update internal scheduler */
    empei_i_field_INTL_LS_UPDATE_set(b, hndl, TRUE);

    empei_i_buffer_flush(b);

    /* Poll UPD_LSC register to make sure both bits auto clears before enabling traffic */
    do       
    {
        reg_val = empei_i_reg_UPD_LSC_read(NULL, hndl);
        retry_cnt ++;
    } while(reg_val != 0x0 && retry_cnt < UPD_LSC_MAX_POLLS);
    
    if(retry_cnt >= UPD_LSC_MAX_POLLS)
    {
        PMC_RETURN(EMPEI_I_ERR_POLL_TIMEOUT);
    }


    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_activate_datapath */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
*  empei_i_datapath_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EMPEI_I datapath.
*
* INPUTS:
*   hndl      - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_datapath_cfg(empei_i_handle_t *hndl, 
                                      UINT32 link, 
                                      empei_i_data_rate_enum_t data_rate,
                                      empei_i_datapath_type_enum_t datapath, 
                                      BOOL8 enable)
{
    PMC_ENTRY();

    /* Configure the EMPEI_I datapath */

    /* empei_i_datapath_sel_cfg(hndl, link, datapath); */

    /* Configure schedulers operating off the CPB clock domain:
       extract link scheduler, insert link scheduler */

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_datapath_cfg */

/*******************************************************************************
*  empei_i_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup EMPEI_I specified links. 
*   Reset and set default value in configuration registers.
*
* INPUTS:
*   hndl       - handle to the EMPEI_I instance to be operated on
*   num_links  - number of links to cleanup
*   links      - list of link to cleanup indices
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
PUBLIC void empei_i_cleanup(empei_i_handle_t *hndl, 
                            UINT32            num_links, 
                            UINT32           *links)
                               
{
    UINT32 link_itr;
    UINT32 link_idx;
    empei_i_buffer_t empei_i_buffer[1];
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    empei_i_buffer_init(empei_i_buffer, hndl);

    for (link_itr = 0; link_itr < num_links && PMC_SUCCESS == rc; link_itr++)
    {
        link_idx = links[link_itr]; 
        /* reset DPI calendar slot */
        rc = empei_i_link_reset( hndl, link_idx);
        (void) empei_i_link_enable(hndl, link_idx, FALSE);
    }

    empei_i_buffer_flush(empei_i_buffer);

    PMC_RETURN();

} /* empei_i_cleanup */

/*******************************************************************************
*  empei_i_datapath_sel_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects between MAC terminated and Transparent datapaths.
*
* INPUTS:
*   hndl     - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_datapath_sel_cfg(empei_i_handle_t *hndl, 
                                          UINT32 link, 
                                          empei_i_datapath_type_enum_t datapath)
{
    PMC_ENTRY();

    /* Configure the link to the specified datapath */

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_datapath_sel_cfg */

/*******************************************************************************
*  empei_i_cpb_link_scheduler_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the link scheduler calendar for the Extract Path (traffic to Mgmt CPB)
*   and the Ingress CPB Interface.
*
* INPUTS:
*   hndl            - handle to the EMPEI_I instance to be operated on
*   link            - array (12) of links to configure for each timeslot
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
PUBLIC PMC_ERROR empei_i_cpb_link_sch_cfg(empei_i_handle_t *hndl, 
                                          UINT32 last_tslot, 
                                          UINT32 offset_extract, 
                                          BOOL8 update_sch, 
                                          UINT32 link[EMPEI_I_NUM_TS])
{
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    /* Assign the specified link to the calendar timeslots */
    empei_i_field_CPB_LINK_SCH_TS0_set ( b, hndl, link[0] );
    empei_i_field_CPB_LINK_SCH_TS1_set ( b, hndl, link[1] );
    empei_i_field_CPB_LINK_SCH_TS2_set ( b, hndl, link[2] );
    empei_i_field_CPB_LINK_SCH_TS3_set ( b, hndl, link[3] );
    empei_i_field_CPB_LINK_SCH_TS4_set ( b, hndl, link[4] );
    empei_i_field_CPB_LINK_SCH_TS5_set ( b, hndl, link[5] );
    empei_i_field_CPB_LINK_SCH_TS6_set ( b, hndl, link[6] );
    empei_i_field_CPB_LINK_SCH_TS7_set ( b, hndl, link[7] );
    empei_i_field_CPB_LINK_SCH_TS8_set ( b, hndl, link[8] );
    empei_i_field_CPB_LINK_SCH_TS9_set ( b, hndl, link[9] );
    empei_i_field_CPB_LINK_SCH_TS10_set( b, hndl, link[10] );
    empei_i_field_CPB_LINK_SCH_TS11_set( b, hndl, link[11] );

    empei_i_field_CPB_LINK_SCH_LAST_TIMESLOT_set(b, hndl, last_tslot);
    empei_i_field_CPB_LINK_SCH_OFFSET_EXTRACT_set(b, hndl, offset_extract);

    if (update_sch) {
        /* Update schedulers (EXTRACT) */
        empei_i_field_EXTRACT_LS_UPDATE_set(b, hndl, TRUE);

    } /* end if */

    empei_i_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_cpb_link_scheduler_cfg */

/*******************************************************************************
*  empei_i_internal_link_sch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the link scheduler calendar for the Ingress Traffic Manager.
*   The Ingress Traffic Manager is responsible for storing and scheduling traffic
*   received from the Insert Path Block and from the Packet Distributor block.
*   Data output from the Ingress Traffic Manager is sent to the Ingress CPB Interface Block.
*
* INPUTS:
*   hndl       - handle to the EMPEI_I instance to be operated on
*   link       - array (12) of links to configure for each timeslot
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
PUBLIC PMC_ERROR empei_i_internal_link_sch_cfg(
    empei_i_handle_t *hndl, 
    UINT32 last_tslot, 
    BOOL8 update_sch, 
    UINT32 link[EMPEI_I_NUM_TS])
{
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    /* Assign the specified link to the calendar timeslots */
    empei_i_field_INTL_LINK_SCH_TS0_set ( b, hndl, link[0] );
    empei_i_field_INTL_LINK_SCH_TS1_set ( b, hndl, link[1] );
    empei_i_field_INTL_LINK_SCH_TS2_set ( b, hndl, link[2] );
    empei_i_field_INTL_LINK_SCH_TS3_set ( b, hndl, link[3] );
    empei_i_field_INTL_LINK_SCH_TS4_set ( b, hndl, link[4] );
    empei_i_field_INTL_LINK_SCH_TS5_set ( b, hndl, link[5] );
    empei_i_field_INTL_LINK_SCH_TS6_set ( b, hndl, link[6] );
    empei_i_field_INTL_LINK_SCH_TS7_set ( b, hndl, link[7] );
    empei_i_field_INTL_LINK_SCH_TS8_set ( b, hndl, link[8] );
    empei_i_field_INTL_LINK_SCH_TS9_set ( b, hndl, link[9] );
    empei_i_field_INTL_LINK_SCH_TS10_set( b, hndl, link[10] );
    empei_i_field_INTL_LINK_SCH_TS11_set( b, hndl, link[11] );

    empei_i_field_INTL_LINK_SCH_LAST_TIMESLOT_set(b, hndl, (UINT8) last_tslot);

    if (update_sch) {
        empei_i_field_INTL_LS_UPDATE_set(b, hndl, TRUE);
    } /* end if */

    empei_i_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_internal_link_sch_cfg */


/*******************************************************************************
*  empei_i_chan_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Map the channel priority (0-7) in each link to one of eight channels in the CPB.
*   EMPEI_I has 96 channels going into the Data CPB.
*   In normal mode only four destination channels are allocated per link and the programmed values
*   must be between 0 and 3. In segregation mode 8 destination channels are allocated per link and destination
*   channels can be from 0 to 7.
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   link          - link to configure
*   dest_chnls    - array of destination channels for the CPB

* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:i
*   dest_chnls can be one of the following:
*                   000 - For link N, the destination channel will be N*8
*                   001 - For link N, the destination channel will be N*8+1
*                   ...
*                   111 - For link N, the destination channel will be N*8+7
**
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_chan_map_cfg(empei_i_handle_t *hndl, 
                                      UINT32 link, 
                                      UINT32 dest_chnls[EMPEI_I_NUM_PRIORITY])
{
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    /* Assign the priority of the specified link to the CPB channel */
    empei_i_field_PRIORITY_0_CHAN_MAP_set ( b, hndl, link, dest_chnls[0] );
    empei_i_field_PRIORITY_1_CHAN_MAP_set ( b, hndl, link, dest_chnls[1] );
    empei_i_field_PRIORITY_2_CHAN_MAP_set ( b, hndl, link, dest_chnls[2] );
    empei_i_field_PRIORITY_3_CHAN_MAP_set ( b, hndl, link, dest_chnls[3] );
    empei_i_field_PRIORITY_4_CHAN_MAP_set ( b, hndl, link, dest_chnls[4] );
    empei_i_field_PRIORITY_5_CHAN_MAP_set ( b, hndl, link, dest_chnls[5] );
    empei_i_field_PRIORITY_6_CHAN_MAP_set ( b, hndl, link, dest_chnls[6] );
    empei_i_field_PRIORITY_7_CHAN_MAP_set ( b, hndl, link, dest_chnls[7] );

    empei_i_buffer_flush(b);


    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_chan_map_cfg */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
*  empei_i_wrr_priority_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the weighted round robin priority for clients A and B for each link.
*
* INPUTS:
*   hndl           - handle to the EMPEI_I instance to be operated on
*   clienta_weight - Bit map for client A weight [11:0]
*   clientb_weight - Bit map for client B weight [11:0]
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_wrr_priority_cfg(empei_i_handle_t *hndl, 
                                          BOOL8 clienta_weight[EMPEI_I_NUM_LINKS], 
                                          BOOL8 clientb_weight[EMPEI_I_NUM_LINKS])
{

    /* Configure weighted round robin priority for clients A and B */
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    empei_i_field_WRR0_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[0] );
    empei_i_field_WRR1_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[1] );
    empei_i_field_WRR2_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[2] );
    empei_i_field_WRR3_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[3] );
    empei_i_field_WRR4_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[4] );
    empei_i_field_WRR5_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[5] );
    empei_i_field_WRR6_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[6] );
    empei_i_field_WRR7_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[7] );
    empei_i_field_WRR8_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[8] );
    empei_i_field_WRR9_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[9] );
    empei_i_field_WRR10_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[10] );
    empei_i_field_WRR11_CLIENTA_WEIGHT_set ( b, hndl, clienta_weight[11] );

    empei_i_field_WRR0_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[0] );
    empei_i_field_WRR1_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[1] );
    empei_i_field_WRR2_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[2] );
    empei_i_field_WRR3_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[3] );
    empei_i_field_WRR4_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[4] );
    empei_i_field_WRR5_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[5] );
    empei_i_field_WRR6_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[6] );
    empei_i_field_WRR7_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[7] );
    empei_i_field_WRR8_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[8] );
    empei_i_field_WRR9_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[9] );
    empei_i_field_WRR10_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[10] );
    empei_i_field_WRR11_CLIENTB_WEIGHT_set ( b, hndl, clientb_weight[11] );

    empei_i_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_wrr_priority_cfg */

/*******************************************************************************
*  empei_i_data_cpb_flow_ctrl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enable/disable Data CPB Priority-based Flow Control.
*    Configure channel mapper block to associate each CPB channel to an appropriate link priority.
*    Configure zone thresholds.
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   link          - link to configure
*   enable        - TRUE: enable link for flow control
*                   FALSE: disable link for flow control
*   cpb_chnl      - CPB Channel to configure (0-96)
*   zone_thres    - 2-bit threshold for the channel zone which when met or exceeded will cause a PFC XOFF or a LINK XOFF
*   priority_bit_map - Priority bit map for the link (Bit 0 is associated to Priority 0 of the Link, etc.)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_data_cpb_flow_ctrl_cfg(empei_i_handle_t *hndl, 
                                                UINT32 link, BOOL8 enable,
                                                UINT32 cpb_chnl, 
                                                UINT32 zone_thres, 
                                                UINT32 priority_bit_map)
{

    PMC_ENTRY();

    /* Configure the zone threshold, and the associated link priorities for the CPB channel */
    /* Register 0x700 + 4*N(N=0:95): EMPEI120_I - System Channel to Priority Mapper (Traffic Controller) */

    /* Enable/disable data CPB zone for PFC messaging for the link */
    /* Register 0x98: EMPEI120_I - CPB Pause Frame Enable (Traffic Controller) */

    PMC_RETURN(EMPEI_I_ERR_NO_SW_CONTENT);

} /* empei_i_data_cpb_flow_ctrl_cfg */

/*******************************************************************************
*  empei_i_data_cpb_zone_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enable/disable Data CPB Zone Priority-based Flow Control.
*    Configure channel mapper block to associate each CPB channel to an appropriate link priority.
*    Configure zone thresholds.
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   zone_enables  - enable flags (UINT32 where bit N is for link N)
*                   TRUE: enable link for zone flow control
*                   FALSE: disable link for zone flow control
*   thresholds    - Array of 96 zone thresholds (2-bit)
*   priority_maps - Array of 96 priority bit map (8-bit)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_data_cpb_zone_cfg(empei_i_handle_t *hndl, 
                                           UINT32 zone_enables, 
                                           UINT32 thresholds[EMPEI_I_NUM_CPB_CHANNELS], 
                                           UINT32 priority_maps[EMPEI_I_NUM_CPB_CHANNELS])
{
    UINT32 i;

    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);

    for(i = 0; i < EMPEI_I_NUM_CPB_CHANNELS; i++ )
    {
        empei_i_field_SYS_ZONE_THRESH_set(b, hndl, i, thresholds[i]);
        empei_i_field_SYS_MAP_TBL_set(b, hndl, i, priority_maps[i]);
    }

    empei_i_field_SYS_ZONE_ENABLE_set(b, hndl, zone_enables);

    empei_i_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_data_cpb_zone_cfg */

/*******************************************************************************
*  empei_i_enable_cpb_flow_ctrl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configuration to enable Data CPB Zone Priority-0 Flow Control.
*    Data CPB channel 0 is statically mapped to priority 0.
*    The enabling of CPB flow control is a separate operation.
*
* INPUTS:
*   hndl               - handle to the EMPEI_I instance to be operated on
*   link               - link to configure
*   sys_zone_thresh    - CPB ZONE Threshold used to start dropping frames and
*                        sendind XON pause frames
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_enable_cpb_flow_ctrl_cfg(empei_i_handle_t *hndl, 
                                                  UINT32 link,
                                                  UINT32 sys_zone_thresh)
{
    PMC_ENTRY();

    empei_i_field_SYS_MAP_TBL_set(NULL, hndl, link * 8, EMPEI_I_CPB_PFC_0);
    
    /* 2-bit threshold for channel zone must be 1 for XOFF to work */
    empei_i_field_SYS_ZONE_THRESH_set(NULL, hndl, link * 8, sys_zone_thresh);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_enable_cpb_flow_ctrl_cfg */

/*******************************************************************************
*  empei_i_cpb_flow_ctrl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enable/disable Data CPB Zone Priority-0 Flow Control.
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   link          - link id 
*   enable        - enable flag (TRUE = enable & FALSE = disable)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_cpb_flow_ctrl_enable(empei_i_handle_t *hndl, 
                                              UINT32 link, 
                                              BOOL8 enable)
{

    PMC_ENTRY();

    empei_i_field_range_SYS_ZONE_ENABLE_set(NULL, hndl, link, link, (UINT32)enable);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_cpb_flow_ctrl_enable */

/*******************************************************************************
*  empei_i_cpb_flow_ctrl_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Get Enable/disable Data CPB Zone Priority-0 Flow Control.
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   link          - link id 
*
* OUTPUTS:
*
* RETURNS:
*   BOOL8 - enable flag (TRUE = enable & FALSE = disable)
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 empei_i_cpb_flow_ctrl_enable_get(empei_i_handle_t *hndl, 
                                             UINT32 link)
{
    UINT32 enable;
    BOOL8 result;
    PMC_ENTRY();

    enable = empei_i_field_range_SYS_ZONE_ENABLE_get(NULL, hndl, link, link);
    
    result = enable == 1?TRUE:FALSE;
    PMC_RETURN(result);
    
} /* empei_i_cpb_flow_ctrl_enable_get */

/*******************************************************************************
*  empei_i_cpb_link_type_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the link type for each link to be sent to the Data CPB
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
*   link_types    - Array of 12 CPB link types to be sent on the DPI bus to the Data CPB
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR empei_i_cpb_link_type_cfg(empei_i_handle_t *hndl, 
                                           UINT32 link_types[EMPEI_I_NUM_LINKS])
{

    PMC_ENTRY();

    empei_i_lfield_LINK_TYPE_set(NULL, hndl, link_types);

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_cpb_link_type_cfg */



/*******************************************************************************
*  empei_i_link_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*    reset the link specified by the link number
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_link_reset(empei_i_handle_t *hndl, 
                                    UINT32 link) 
{
  UINT32 reg_value = 0;

  PMC_ENTRY();

  reg_value = ( 1 << link );
  empei_i_field_LINK_RESET_set( NULL, hndl, reg_value);
  
 
  PMC_RETURN(PMC_SUCCESS);

} /* empei_i_link_reset */


/*******************************************************************************
*  empei_i_link_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*    enable/disable specified link so that incoming data to flow through EMPEI Links (put in operational state) 
*
* INPUTS:
*   hndl          - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_link_enable(empei_i_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 enable) 
{
  PMC_ENTRY();

  empei_i_field_range_LINK_ENABLE_set(NULL, hndl, link, link, (UINT32) enable);
  
  PMC_RETURN(PMC_SUCCESS);

} /* empei_i_link_enable */

 
#if defined(PMC_SW_SIMULATION) | defined (PMC_VERIFICATION) | defined (PMC_EMULATION)
/*******************************************************************************
*  empei_i_simulation_setup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Relevant for simulation only - this pollutes the Ingress DPI -> DCPB control signals
*
* INPUTS:
*   hndl           - handle to the ECLASS instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_i_simulation_setup(empei_i_handle_t *hndl)
{
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004024, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004224, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004424, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004624, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004824, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004a24, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004c24, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00004e24, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00005024, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00005224, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00005424, 0 );
    empei_i_reg_write( NULL, hndl, MEM_TYPE_CONFIG, 0x00005624, 0 );
} /* empei_i_simulation_setup */
#endif

/*******************************************************************************
*  empei_i_eclass_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the ECLASS block energy state from EMPEI_I local register fields. 
*
* INPUTS:
*   hndl           - handle to the ECLASS instance to be operated on
*   energy_state   - energy_state
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_i_eclass_energy_state_reg_set ( empei_i_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            empei_i_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, 0);
            empei_i_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            empei_i_field_SW_RESET_ECLASS_NREG_set(NULL, hndl, 1);
            empei_i_field_SW_RESET_ECLASS_CORE_set(NULL, hndl, 1);
            break;
        default:
            PMC_ASSERT(FALSE, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN();

} /* empei_i_eclass_energy_state_reg_set */

/*******************************************************************************
*  empei_i_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the EMPEI_I block in 
*   ENET subsystem.
*
*
* INPUTS:
*   *hndl         - pointer to EMPEI_I handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void empei_i_block_non_default_cfg(empei_i_handle_t *hndl)
{
    empei_i_buffer_t b[1];

    PMC_ENTRY();

    /* Initialize buffer */
    empei_i_buffer_init(b, hndl);
    /* Remove Sys Data packet queue shut down state */
    empei_i_field_SD_CTRL_set(b, hndl, 0);
    empei_i_buffer_flush(b);
    
    /* writing to RAM BIST - SD_CTRL */
    empei_i_reg_write(NULL, hndl, MEM_TYPE_CONFIG, 0x0000000C, 0);

    PMC_RETURN();
} /* empei_i_block_non_default_cfg */


/*******************************************************************************
*  empei_i_ext_pmon_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables an Extract PMON for a particular link and 
*   classification tag.
*
* INPUTS:
*   *hndl       - EMPEI_I handle
*   link        - link number
*   tag         - classification tag
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
PUBLIC PMC_ERROR empei_i_ext_pmon_enable(empei_i_handle_t *hndl, UINT32 link, UINT32 tag)
{
    UINT32 counter_no;
    UINT8 *pmon_link_map;
    UINT8 *pmon_tag_map;
    UINT32 *pmons;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_link_map = hndl->var.ext_pmon_link_map;
    pmon_tag_map = hndl->var.ext_pmon_tag_map;
    pmons = hndl->var.ext_pmon;

    /* find a free counter */
    for(counter_no=0; counter_no < EMPEI_I_NUM_EXT_PMON ; counter_no++) 
    {
        if(pmon_link_map[counter_no] == 0 && pmon_tag_map[counter_no] == 0)
            break;
    }
   
    if (counter_no >= EMPEI_I_NUM_EXT_PMON) 
    {
        PMC_RETURN(EMPEI_I_LOG_ERR_NO_AVAIL_EXT_PMON);
    } 
        
    /* Mark the counter as used */
    pmon_link_map[counter_no] = link;
    pmon_tag_map[counter_no] = tag;

    /* Enable the counter */
    empei_i_field_EXT_PMON_CFG_set(NULL, hndl, counter_no, (link << 5) | tag);
    empei_i_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 1);

    PMC_RETURN(PMC_SUCCESS);
} /* empei_i_ext_pmon_enable */

/*******************************************************************************
*  empei_i_ext_pmon_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function disables an Extract PMON for a particular link and 
*   classification tag.
*
* INPUTS:
*   *hndl        - handle to the EMPEI_I instance to be operated on
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
PUBLIC PMC_ERROR empei_i_ext_pmon_disable(empei_i_handle_t *hndl,
                                          UINT32 link,
                                          UINT32 tag)
{
    UINT32 counter_no;
    UINT8 *pmon_link_map;
    UINT8 *pmon_tag_map;
    UINT32 *pmons;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_link_map = hndl->var.ext_pmon_link_map;
    pmon_tag_map = hndl->var.ext_pmon_tag_map;
    pmons = hndl->var.ext_pmon;

    for(counter_no=0; counter_no < EMPEI_I_NUM_EXT_PMON; counter_no++) 
    {
        if(pmon_link_map[counter_no] == link && pmon_tag_map[counter_no] == tag)
            break;
    }
    if (counter_no >= EMPEI_I_NUM_EXT_PMON) 
    {
        /* link/classification code not found */
        PMC_RETURN(EMPEI_I_LOG_ERR_EXT_PMON_NOT_FOUND);
    } 
    
    /* mark the counter as free */
    pmon_link_map[counter_no] = 0;
    pmon_tag_map[counter_no] = 0;

    /* disable the counter */
    empei_i_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 0); 
    PMC_RETURN(PMC_SUCCESS);

} /*empei_i_ext_pmon_disable */

/*******************************************************************************
*  empei_i_ext_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Retrieve all PMONs for a particular link. The counter values 
*                are stored in counters_ptr and the corresponding counter tag
*                is stored in the tags_ptr. The tags_ptr entry is set to 0 if
*                the corresponding PMON is not enabled for the 'link'.
*
* INPUTS:
*   *hndl           - EMPEI_I handle
*   link            - link number
*   accumulate      - Accumulate counter when TRUE.
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
PUBLIC PMC_ERROR empei_i_ext_pmon_get(empei_i_handle_t *hndl, 
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

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    link_map = hndl->var.ext_pmon_link_map;
    tag_map = hndl->var.ext_pmon_tag_map;
    link_tag_map = hndl->var.ext_pmon_link_tag_map;
    pmons = hndl->var.ext_pmon;

    /*
     * Read all the Ext PMONs for 'link'.  The tags_ptr array is 
     * updated with the classification code corresponding to the PMON.
     * If the PMON is not enabled for the 'link', the entry is set to 0.
     */
    for(counter_no=0; counter_no < EMPEI_I_NUM_EXT_PMON ; counter_no++) 
    {
        if(link_map[counter_no] == link)
        {
            val = empei_i_reg_EXT_PMON_PKT_CNT_array_read(NULL, hndl, counter_no);
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

} /*empei_i_ext_pmon_get */

/*******************************************************************************
*  empei_i_ext_pmon_disable_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Disable all extract PMONs for a particular link.
*
* INPUTS:
*   *hndl        - EMPEI_I handle
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
PUBLIC PMC_ERROR empei_i_ext_pmon_disable_all(empei_i_handle_t *hndl, UINT32 link)
{
    UINT32 counter_no;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for(counter_no=0; counter_no < EMPEI_I_NUM_EXT_PMON ; counter_no++) 
    {
        if(hndl->var.ext_pmon_link_map[counter_no] == link)
        {
            /* mark the counter as free */
            hndl->var.ext_pmon_link_map[counter_no] = 0;
            hndl->var.ext_pmon_tag_map[counter_no] = 0;
            hndl->var.ext_pmon[counter_no] = 0;
        
            /* disable the counter */
            empei_i_field_EXT_PMON_EN_set(NULL, hndl, counter_no, 0); 
        }
    }
    PMC_RETURN(PMC_SUCCESS);
} /* empei_i_ext_pmon_disable_all */

/*******************************************************************************
*  empei_i_pmon_latch
* ______________________________________________________________________________
*
* DESCRIPTION:
*                Latch and clear PMONs.
*
* INPUTS:
*   *hndl        - EMPEI_E handle
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
PUBLIC PMC_ERROR empei_i_pmon_latch(empei_i_handle_t *hndl)
{
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    empei_i_field_PMON_CNT_CLR_set(NULL, hndl, 1); 

    PMC_RETURN(PMC_SUCCESS);
} /* empei_i_pmon_latch */

/*******************************************************************************
* empei_i_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns EMPEI_E links status: enabled or disabled.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to EMPEI_I handle instance
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
PUBLIC void empei_i_activated_link_get(empei_i_handle_t   *hndl,
                                       BOOL8              *actv_link)
{
    UINT32 itr;

    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < EMPEI_I_NUM_LINKS; itr++)
    {
        actv_link[itr] = empei_i_field_range_LINK_ENABLE_get(NULL, hndl, itr, itr);
    }

    PMC_RETURN();
}  /* empei_i_activated_link_get */


/*******************************************************************************
* FUNCTION: empei_i_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   empei_i_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR empei_i_int_validate(empei_i_handle_t *empei_i_handle,
                                      UINT32            link,
                                      void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != empei_i_handle, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);
   
    if (link >= EMPEI_I_NUM_LINKS)
    {
        result = EMPEI_I_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = EMPEI_I_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* empei_i_int_validate */


/*******************************************************************************
* FUNCTION: empei_i_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   empei_i_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR empei_i_int_chnl_enable(empei_i_handle_t    *empei_i_handle,
                                         UINT32               link,
                                         empei_i_int_chnl_t  *int_table_ptr,
                                         empei_i_int_chnl_t  *int_en_table_ptr,
                                         BOOL8                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_i_int_validate(empei_i_handle, link, int_en_table_ptr);

    #ifdef EMPEI_I_INT
    #undef EMPEI_I_INT
    #endif
    #define EMPEI_I_INT    UTIL_GLOBAL_INT_ENABLE_RANGE
    
    

    EMPEI_I_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* empei_i_int_chnl_enable */




/*******************************************************************************
* FUNCTION: empei_i_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   empei_i_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR empei_i_int_chnl_clear(empei_i_handle_t    *empei_i_handle,
                                        UINT32               link,
                                        empei_i_int_chnl_t  *int_table_ptr,
                                        empei_i_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_i_int_validate(empei_i_handle, link, int_en_table_ptr);

    #ifdef EMPEI_I_INT
    #undef EMPEI_I_INT
    #endif
    #define EMPEI_I_INT UTIL_GLOBAL_INT_CLEAR_RANGE
    

    EMPEI_I_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* empei_i_int_chnl_clear */




/*******************************************************************************
* FUNCTION: empei_i_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   empei_i_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR empei_i_int_chnl_retrieve(empei_i_handle_t    *empei_i_handle,
                                           UINT32               link,
                                           empei_i_int_chnl_t  *filt_table_ptr,
                                           empei_i_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;
    empei_i_buffer_t    *empei_i_buffer_ptr = NULL;

    PMC_ENTRY();

    result = empei_i_int_validate(empei_i_handle, link, int_table_ptr);

    
    #ifdef EMPEI_I_INT
    #undef EMPEI_I_INT
    #endif
    #define EMPEI_I_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE
    

    EMPEI_I_INT_TABLE_DEFINE();
    
    
    #ifdef EMPEI_I_STATUS
    #undef EMPEI_I_STATUS
    #endif
    #define EMPEI_I_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    

    EMPEI_I_STATUS_TABLE_DEFINE();
    


    PMC_RETURN(result);
} /* empei_i_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: empei_i_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   empei_i_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR empei_i_int_chnl_enabled_check(empei_i_handle_t     *empei_i_handle,
                                                UINT32                link,
                                                empei_i_int_chnl_t   *int_en_table_ptr,
                                                BOOL8                 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = link;
    UINT32          link2 = link;

    PMC_ENTRY();

    result = empei_i_int_validate(empei_i_handle, link, int_found_ptr);


    #ifdef EMPEI_I_INT
    #undef EMPEI_I_INT
    #endif
    #define EMPEI_I_INT UTIL_GLOBAL_INT_CHECK_RANGE
    

    EMPEI_I_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* empei_i_int_chnl_enabled_check */





/*
** Private Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
*  empei_i_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize EMPEI_I var_t struct.
*
* INPUTS:
*   *hndl      - pointer to EMPEI_I handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void empei_i_var_default_init(empei_i_handle_t *hndl)
{
    UINT32 counter_no;
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, EMPEI_I_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Reset stored EXT PMONs and maps */
    for(counter_no=0; counter_no < EMPEI_I_NUM_EXT_PMON; counter_no++) 
    {
        hndl->var.ext_pmon_link_map[counter_no] = 0;
        hndl->var.ext_pmon_tag_map[counter_no] = 0;
        hndl->var.ext_pmon[counter_no] = 0;
    }
   
    PMC_RETURN();
}/* empei_i_var_default_init*/

#endif
/*
** End of file
*/
