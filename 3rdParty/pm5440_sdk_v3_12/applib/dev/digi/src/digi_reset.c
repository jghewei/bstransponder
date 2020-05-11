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
*   DESCRIPTION : This file contains functions for the reset bits of
*   each subsystem in digi level and calls to initialize subsystem.
*   It provides two types of APIs which are register access operaion and
*   computational operation for each subsystem.
*   Functions for usages have prototypes defined in digi_reset_loc.h and
*   should be considered private and for the internal use of the SW driver.
*
*  NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_loc.h"


/*
** Local Enumerated Types
*/


/*
** Local Constants
*/


/*
** Computational Operation Function
*/
/*******************************************************************************
* digi_dcpb_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the DCPB subsystem as controlled
*   outside the DCPB subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_dcpb_energy_state_set(digi_handle_t *digi_handle, 
                                      pmc_energy_state_request_t energy_state)
{
    UINT32 i,j;
    BOOL port_state;
    BOOL cfg_ready = TRUE;
    pmc_energy_state_t blk_energy_state = (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING) ? PMC_ENERGY_STATE_RUNNING : PMC_ENERGY_STATE_RESET;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* if conditional TRUE verify no resources provisioned */
    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        /* if any resources not in starting state then suppress operation */
        if((cpb_start_state_test(digi_handle->dcpb_handle) == FALSE))
            cfg_ready = FALSE;

        /* if SIFD interface is used, configuration will be valid
           for the life of the system, if either DCS is configured
           then prevent further operation */
        for(i=0; i<2; i++)
        {
            port_state = cpb_port_init_state_get(digi_handle->dcpb_handle, CPB_PORT_TYPE_DCS, i);
            if(TRUE == port_state)
            {
                cfg_ready = FALSE;
            }
        }
    }

    if(cfg_ready == TRUE)
    {
        PMC_ATOMIC_YIELD(digi_handle,0);
        /* set the DIGI level registers to match the desired state */
        digi_cpb_energy_state_reg_set(digi_handle, blk_energy_state);
        /* Initialize DCPB subsystem to match the desired state */
        cpb_init(digi_handle->dcpb_handle, blk_energy_state);

        /* If the CPB was just taken out of reset, reapply the pmon
         * config because it may have been lost when CPB was put into
         * reset */
        if(blk_energy_state == PMC_ENERGY_STATE_RUNNING)
        {
            for(i=0;i<DIGI_DCPB_NUM_DPI_PORTS;i++)
            {
                for(j=0;j<DIGI_CPB_PMON_COUNTERS_PER_PORT;j++)
                {
                    digi_dcpb_pmon_cfg(digi_handle,
                                       (cpb_dcpb_dpi_port_type_t) i,
                                       j,
                                       digi_handle->var.cpb_port_pmon[i].pmon_counter[j].pmon_mode,
                                       digi_handle->var.cpb_port_pmon[i].pmon_counter[j].count_drop_packets,
                                       digi_handle->var.cpb_port_pmon[i].pmon_counter[j].count_err_packets,
                                       digi_handle->var.cpb_port_pmon[i].pmon_counter[j].count_oversize_packets,
                                       digi_handle->var.cpb_port_pmon[i].pmon_counter[j].count_good_packets);
                 }
            }
        }
    }

    PMC_RETURN();

} /* digi_dcpb_energy_state_set */

/*******************************************************************************
* digi_mcpb_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the MCPB subsystem as controlled
*   outside the MCPB subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_mcpb_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state)
{
    BOOL cfg_ready = TRUE;
    pmc_energy_state_t blk_energy_state = (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING) ? PMC_ENERGY_STATE_RUNNING : PMC_ENERGY_STATE_RESET;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);


    /* if conditional TRUE verify no resources provisioned */
    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        /* if any resources not in starting state then suppress operation */
        if((cpb_start_state_test(digi_handle->mcpb_handle) == FALSE))
            cfg_ready = FALSE;
    }

    if(cfg_ready == TRUE)
    {
        /* set the DIGI level registers to match the desired state */
        digi_mcpb_energy_state_reg_set(digi_handle, blk_energy_state);
        /* Initialize MCPB subsystem to match the desired state */
        cpb_init(digi_handle->mcpb_handle, blk_energy_state);
        /* reset MCPB context at DIGI layer when putting in reset state */
        if(blk_energy_state == PMC_ENERGY_STATE_RESET)
        {
            UINT32 i;

            for (i = 0; i < DIGI_MCPB_DCS_MAX; i++)
            {
                digi_handle->var.mcpb_ctxt.mcpb_dcs_num_registered_chnl[i] = 0;
            }
            
            digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init = FALSE;
            digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = FALSE;
            digi_handle->var.mcpb_ctxt.mcpb_cpu_port_cfg = FALSE;
        }
    }

    PMC_RETURN();
} /* digi_mcpb_energy_state_set */

/*******************************************************************************
* digi_lineotn_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LINEOTN subsystem as controlled
*   outside the LINEOTN subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_lineotn_energy_state_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_request_t energy_state)
{
    BOOL cfg_ready = TRUE;
    pmc_energy_state_t blk_energy_state = (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING) ? PMC_ENERGY_STATE_RUNNING : PMC_ENERGY_STATE_RESET;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* if conditional TRUE verify no resources provisioned */
    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        /* if any resources not in starting state then suppress operation */
        if((lineotn_start_state_test(digi_handle->lineotn_handle) == FALSE))
            cfg_ready = FALSE;
    }

    if(cfg_ready == TRUE)
    {
        /* set the DIGI level registers to match the desired state */
        digi_lineotn_energy_state_reg_set(digi_handle, blk_energy_state);
        /* Initialize MCPB subsystem to match the desired state */
        lineotn_init(digi_handle->lineotn_handle, blk_energy_state);
    }

    PMC_RETURN();

} /* digi_lineotn_energy_state_set */


/*******************************************************************************
* digi_sysotn_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LINEOTN subsystem as controlled
*   outside the SYSOTN subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_sysotn_energy_state_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_request_t energy_state)
{
    BOOL cfg_ready = TRUE;
    pmc_energy_state_t blk_energy_state = (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING) ? PMC_ENERGY_STATE_RUNNING : PMC_ENERGY_STATE_RESET;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* if conditional TRUE verify no resources provisioned */
    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        /* if any resources not in starting state then suppress operation */
        if((lineotn_start_state_test(digi_handle->sysotn_handle) == FALSE))
            cfg_ready = FALSE;
    }

    if(cfg_ready == TRUE)
    {
        /* set the DIGI level registers to match the desired state */
        digi_sysotn_energy_state_reg_set(digi_handle, blk_energy_state);
        lineotn_init(digi_handle->sysotn_handle, blk_energy_state);
    }

    PMC_RETURN();

} /* digi_sysotn_energy_state_set */




/*******************************************************************************
* digi_lifd_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LIFD subsystem as controlled
*   outside the LIFD subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*   IMPORTANT NOTE:  The usage of LIFD SS in DIGI is unconditionally
*   enabled.  SIFD SS will only be reset as part of digi_device_restart() 
*   API path and as such cannot be requested into a reset state by this API.  
*   This API only supports PMC_ENERGY_STATE_REQUEST_RUNNING.  Other requests will
*   generate assertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_lifd_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if(energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING)
    {
        digi_lifd_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);
    } else {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_lifd_energy_state_set */


/*******************************************************************************
* digi_lifd_system_required_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function tests device initialization context to determine if LIFD
*   subsystem is required in the scope of system level resources that are
*   provisioned.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                   - TRUE: LIFD Subsystem required for configured
*                                  resources. \n
*                          - FALSE: LIFD Subsystem not required for configured
*                                   resources.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_lifd_system_required_test(digi_handle_t *digi_handle)
{
    PMC_ENTRY();    
    BOOL lifd_required = FALSE;
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* test need for LIFD based on product application */
    if (DIGI_PROD_APP_IS_LINE_CARD(digi_handle)) {

        lifd_required = TRUE;

    } else if (digi_handle->var.prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) {

        lifd_required = TRUE;

    } else if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD) {

         lifd_required = TRUE;

    } else if (digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD) {

         lifd_required = TRUE;
    }

    if (DIGI60_PROD_APP_IS_LINE_CARD(digi_handle) || DIGI60_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) || DIGI60_PROD_APP_IS_TRANSMUXPONDER_SYS_CLIENT_CARD(digi_handle)) {
         lifd_required = TRUE;
    }

    PMC_RETURN(lifd_required);

} /* digi_lifd_system_required_test */

/*******************************************************************************
* digi_sifd_system_required_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function tests device initialization context to determine if LIFD
*   subsystem is required in the scope of system level resources that are
*   provisioned.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                   - TRUE: SIFD Subsystem required for configured
*                                  resources. \n
*                          - FALSE: SIFD Subsystem not required for configured
*                                   resources.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_sifd_system_required_test(digi_handle_t *digi_handle)
{
    PMC_ENTRY();    
    BOOL sifd_required = FALSE;
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* test need for SIFD based on product application */
    if (DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle)) {

        sifd_required = TRUE;

    } else if(digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD) {

         sifd_required = TRUE;

    } else if(digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD) {

         sifd_required = TRUE;

    /* test need for SIFD based on ILKN1/ILKN2 being configured */
    } else {

        /* test need for SIFD based on resources being provisioned in ENET_SYS */
        /* Note: No test performed, based on the potential for enhanced ENET PMON,
                 SIFD must be operational */
        sifd_required = TRUE;
    }


    PMC_RETURN(sifd_required);

} /* digi_sifd_system_required_test */

/*******************************************************************************
* digi_cbrc_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the CBRC subsystem as controlled
*   outside the CBRC subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_cbrc_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state)
{
    BOOL cfg_ready = TRUE;
    pmc_energy_state_t blk_energy_state = (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING) ? PMC_ENERGY_STATE_RUNNING : PMC_ENERGY_STATE_RESET;
    BOOL is_provisioned;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);

    is_provisioned = !cbrc_start_state_test(digi_handle->cbrc_handle);

    /* if conditional TRUE verify no resources provisioned */
    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        /* if any resources not in starting state then suppress operation */
        if((cbrc_start_state_test(digi_handle->cbrc_handle) == FALSE))
            cfg_ready = FALSE;
    }

    if(cfg_ready == TRUE)
    {
        /* CBRC sync resets are by default deasserted. Put them into reset 
         * regardless of REQUEST_RUNNING or STATE_RESET
         */
        cbrc_sync_reset(digi_handle->cbrc_handle, CBRC_RESET);

        /* set the DIGI level registers to match the desired state */
        digi_cbrc_energy_state_reg_set(digi_handle, blk_energy_state);

        /* Initialize CBRC subsystem to match the desired state */
        cbrc_init(digi_handle->cbrc_handle, blk_energy_state);
    }

    if (energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING)
    {
        /* RESET -> RUNNING */
        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] == FALSE)
        {
            /* Start CBRC interrupt monitors in FW */
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_RF,               TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LF,               TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,        TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,         TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOS,              TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,       TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT, TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,      TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,       TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT, TRUE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,      TRUE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] = TRUE;
        }
    }
    else if (energy_state == PMC_ENERGY_STATE_REQUEST_RESET ||
             (energy_state == PMC_ENERGY_STATE_REQUEST_OPTIMAL && !is_provisioned))
    {
        /* RUNNING -> RESET */
        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] == TRUE)
        {
            /* Stop CBRC interrupt monitors in FW */
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_RF,               FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LF,               FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,        FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,         FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PCS_RX_LOS,              FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,       FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT, FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,      FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,       FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT, FALSE);
            digi_fw_cbrc_intr_monitor_cfg(digi_handle, INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,      FALSE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] = FALSE;
        }
    }

    PMC_RETURN();

} /* digi_cbrc_energy_state_set */

/*******************************************************************************
* digi_enet_sys_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the ENET_SYS subsystem as controlled
*   outside the ENET_SYS subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   link                   - requested link 
*   datapath               - enum, requested datapath
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_enet_sys_energy_state_set(digi_handle_t *digi_handle, 
                                           UINT32 link,
                                           enet_datapath_t datapath,
                                           pmc_energy_state_request_t energy_state)
{
    BOOL8 eclass_init  = FALSE;
    PMC_ENTRY();    

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* when requesting a new datapath, enet may be in power down */
    if (PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state &&
        TRUE == enet_start_state_test(digi_handle->enet_sys_handle))
    {
        /* ENET is in power down, we enable it */
        /* set the DIGI level registers to match the desired state */
        digi_enet_sys_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);        
        eclass_init = TRUE;

        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == FALSE)
        {
            /* Start ENET SYS interrupt monitors in FW */
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL, TRUE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] = TRUE;
        }

    }
    
    /* Initialize ENET_LINE subsystem to enable power for requested datapath match the desired state */
    /* _RUNNING will enable datapath */
    /* _OPTIMAL and _RESET will disable it */
    enet_init(digi_handle->enet_sys_handle, link,datapath,energy_state);


    if (eclass_init == TRUE)
    {
        /* Configure ENET Line client source in firmware */
        /* Since we are passing in the client channel handle, there will be no conflict in the
           TRANSMUXPONDER application. FW will just remember the handle of the last configured ENET
           subsystem (LINE or SYS) */
        (void) digi_enet_fw_enet_client_source_conditional_update(digi_handle, 
                                                                  (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_sys_chnl[link]);
        
        /* Initialize ENET Ingress ECLASS and its Configuration RAM.*/
        (void) digi_fw_enet_eclass_init (digi_handle,
                                         ENET_INGRESS);
        (void)  digi_fw_enet_eclass_init (digi_handle,
                                          ENET_EGRESS);
    }


    if ( (PMC_ENERGY_STATE_REQUEST_RESET == energy_state) ||         
         (PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state && 
          TRUE == enet_start_state_test(digi_handle->enet_sys_handle)))
    {
      
        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == TRUE)
        {
            /* Stop ENET SYS interrupt monitors in FW */
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL, FALSE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] = FALSE;
        }

        /* set the DIGI level registers to reset */
        digi_enet_sys_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RESET);  
    }            

    PMC_RETURN();
} /* digi_enet_sys_energy_state_set */

/*******************************************************************************
* digi_enet_line_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the ENET_LINE subsystem as controlled
*   outside the ENET_LINE subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   link                   - requested link 
*   datapath               - enum, requested datapath
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_enet_line_energy_state_set(digi_handle_t *digi_handle, 
                                            UINT32 link,
                                            enet_datapath_t datapath,
                                            pmc_energy_state_request_t energy_state)
{
    BOOL8 eclass_init = FALSE;
    PMC_ENTRY();    

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* when requesting a new datapath, enet may be in power down */
    if (PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state &&
        TRUE == enet_start_state_test(digi_handle->enet_line_handle))
    {
        /* ENET is in power down, we enable it */
        /* set the DIGI level registers to match the desired state */
        digi_enet_line_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);        
        eclass_init = TRUE;

        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == FALSE)
        {
            /* Start ENET LINE interrupt monitors in FW */
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,   TRUE);
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL, TRUE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] = TRUE;
        }

    }
    
    /* Initialize ENET_LINE subsystem to enable power for requested datapath match the desired state */
    /* _RUNNING will enable datapath */
    /* _OPTIMAL and _RESET will disable it */
    enet_init(digi_handle->enet_line_handle, link, datapath, energy_state);

    if (eclass_init == TRUE)
    {
        /* Configure ENET Line client source in firmware */
        /* Since we are passing in the client channel handle, there will be no conflict in the
           TRANSMUXPONDER application. FW will just remember the handle of the last configured ENET
           subsystem (LINE or SYS) */
        (void) digi_enet_fw_enet_client_source_conditional_update(digi_handle, 
                                                                  (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[link]);
                
        /* Initialize ENET Ingress ECLASS and its Configuration RAM.*/
        (void) digi_fw_enet_eclass_init (digi_handle,
                                         ENET_INGRESS);
        (void)  digi_fw_enet_eclass_init (digi_handle,
                                          ENET_EGRESS);
    }

    if ( (PMC_ENERGY_STATE_REQUEST_RESET == energy_state) ||         
         (PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state && 
          TRUE == enet_start_state_test(digi_handle->enet_line_handle)))
    {
     

        if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
        {
            /* Stop ENET LINE interrupt monitors in FW */
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,   FALSE);
            digi_fw_enet_intr_monitor_cfg(digi_handle, INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL, FALSE);

            digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] = FALSE;
        }

        /* set the DIGI level registers to reset */
        digi_enet_line_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RESET);
    }            

    PMC_RETURN();

} /* digi_enet_line_energy_state_set */

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_cbrc_line_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED: This function is deprecated starting in the 3.04 release.
*
*   This function sets the energy state for the CBRC subsystem \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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

/* LCOV_EXCL_START */

PUBLIC void digi_cbrc_line_energy_state_set(digi_handle_t *digi_handle, 
                                            pmc_energy_state_request_t energy_state)
{
    pmc_energy_state_t state = PMC_ENERGY_STATE_RESET;

    PMC_ENTRY();    

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* when requesting a new datapath, CBRC may be in power down */
    if (PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state)
    {

        if (TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
        {
            /* ENET is in power down, we enable it */
            /* set the DIGI level registers to match the desired state */
            digi_cbrc_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);
        }
        state = PMC_ENERGY_STATE_RUNNING;
    }
    
    /* Initialize CBRC subsystem to enable power for requested datapath match the desired state */
    /* _RUNNING will enable datapath */
    /* _OPTIMAL and _RESET will disable it */
    cbrc_init(digi_handle->cbrc_handle, state);

    if (PMC_ENERGY_STATE_RESET == state &&         
        TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        /* set the DIGI level registers to reset */
        digi_cbrc_line_energy_state_reg_set(digi_handle, 
                                            PMC_ENERGY_STATE_RESET);        
    }            

    PMC_RETURN();

} /* digi_cbrc_line_energy_state_set */

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

/*******************************************************************************
* digi_mapotn_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the MAPOTN subsystem as controlled
*   outside the MAPOTN subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   mode                   - Enum, OPU payload mapping modes.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_mapotn_energy_state_set(digi_handle_t *digi_handle, 
                                         digi_mapping_mode_t mode,
                                         pmc_energy_state_request_t energy_state)
{
    util_global_map_adapt_func_t map_mode = UTIL_GLOBAL_MAP_NO_RATE;
    util_global_map_adapt_func_t demap_mode = UTIL_GLOBAL_MAP_NO_RATE;
        
    PMC_ENTRY(); 
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (mode < LAST_DIGI_MAP)
    {
        digi_mapotn_mode_params_convert(mode, &map_mode, &demap_mode);
    }
        
    /* when requesting a new datapath, mapotn may be in power down */
    if (PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state &&
        TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_YIELD(digi_handle, 0);
        digi_mapotn_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);        
    }
    
    /* Initialize MAPOTN subsystem to enable power for requested datapath match the desired state */
    /* _RUNNING will enable datapath */
    /* _OPTIMAL and _RESET will disable it */
    
    mapotn_init(digi_handle->mapotn_handle, energy_state, map_mode);

    if (map_mode != demap_mode)
    {
        mapotn_init(digi_handle->mapotn_handle, energy_state, demap_mode);
    }

    if ( (PMC_ENERGY_STATE_REQUEST_RESET == energy_state) ||         
         (PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state && 
          TRUE == mapotn_start_state_test(digi_handle->mapotn_handle)))
    {
        PMC_ATOMIC_YIELD(digi_handle, 0);
        /* set the DIGI level registers to reset */
        digi_mapotn_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RESET);         
    }        

    PMC_RETURN();

} /* digi_mapotn_energy_state_set */


/*******************************************************************************
* digi_sifd_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the SIFD subsystem as controlled
*   outside the SIFD subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*   IMPORTANT NOTE:  The usage of SIFD SS in DIGI is unconditionally
*   enabled.  SIFD SS will only be reset as part of digi_device_restart() 
*   API path and as such cannot be requested into a reset state by this API.  
*   This API only supports PMC_ENERGY_STATE_REQUEST_RUNNING.  Other requests will
*   generate assertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_sifd_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    if(energy_state == PMC_ENERGY_STATE_REQUEST_RUNNING)
    {
        digi_sifd_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);
    } else {
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_sifd_energy_state_set */



/*******************************************************************************
* digi_coreotn_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the COREOTN subsystem as controlled
*   outside the COREOTN subsystem module. \n\n
*
*   The operation takes in a request for the energy state.  The function is 
*   responsible for ensuring if resources are already provisioned within the
*   subsystem that they will not be corrupted should the request be for
*   RUNNING or OPTIMAL states.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
*   coreotn_power          - in RUNNING request, do we want to enable \n
*                            stage 4, supporting interface or ODUkp data path
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
PUBLIC void digi_coreotn_energy_state_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_request_t energy_state,
                                          coreotn_power_t coreotn_power)
{       
    UINT32 i, j;
    BOOL8 pmon_config = FALSE;

    PMC_ENTRY(); 
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
           
    /* when requesting a new datapath, mapotn may be in power down */
    if (PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state &&
        TRUE == coreotn_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_YIELD(digi_handle,0);
        digi_coreotn_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RUNNING);        
        pmon_config = TRUE;
    }
    
    if (COREOTN_ODU_IF != coreotn_power)
    {
        /* Initialize COREOTN subsystem to enable power for requested datapath match the desired state */
        /* _RUNNING will enable datapath */
        /* _OPTIMAL and _RESET will disable it */
        /* supporting interfaces are handled using OHP/RCP init/uninit functions */
        coreotn_init(digi_handle->coreotn_handle, energy_state,coreotn_power);
    }
    
    if ( (PMC_ENERGY_STATE_REQUEST_RESET == energy_state) ||         
         (PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state && 
          TRUE == coreotn_start_state_test(digi_handle->coreotn_handle)))
    {
        PMC_ATOMIC_YIELD(digi_handle,0);
        /* set the DIGI level registers to reset */
        digi_coreotn_energy_state_reg_set(digi_handle, PMC_ENERGY_STATE_RESET);         
    }        

    if (pmon_config == TRUE)
    {
        /* If the coreotn was just taken out of reset it may have lost
         * ODUk switch PMON settings when it was put in reset. We need
         * to re-apply them */
        for(i=0;i<DIGI_OCPB_NUM_DPI_PORTS;i++)
        {
            for(j=0;j<DIGI_CPB_PMON_COUNTERS_PER_PORT;j++)
            {
                digi_ocpb_pmon_internal_cfg(digi_handle,
                                            (cpb_ocpb_dpi_port_type_t) i,
                                            j,
                                            digi_handle->var.oduksw_port_pmon[i].pmon_counter[j].pmon_mode,
                                            digi_handle->var.oduksw_port_pmon[i].pmon_counter[j].count_drop_packets,
                                            digi_handle->var.oduksw_port_pmon[i].pmon_counter[j].count_err_packets,
                                            digi_handle->var.oduksw_port_pmon[i].pmon_counter[j].count_oversize_packets,
                                            digi_handle->var.oduksw_port_pmon[i].pmon_counter[j].count_good_packets);
            }
        }        
    }

    PMC_RETURN();

} /* digi_coreotn_energy_state_set */

/*
** Register Access Operation
*/

/*******************************************************************************
* digi_m1_energy_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the M1 DIGI top level block.
*
*   The operation takes in a request for the energy state, and unconditionally
*   take the block out of low power/reset or put it into low power/reset.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, requested energy state
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
PUBLIC void digi_m1_energy_state_set(digi_handle_t *digi_handle, 
                                     pmc_energy_state_request_t energy_state)
{
    PMC_ENTRY();    

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(energy_state <= PMC_ENERGY_STATE_REQUEST_OPTIMAL, DIGI_ERR_INVALID_ARG, 0, 0);


    if(energy_state != PMC_ENERGY_STATE_REQUEST_RESET)
    {
        tl_ic_field_DIGI_M1_RESET_set(NULL, digi_handle, 0);
        tl_ic_field_DIGI_M1_LOW_PWR_set(NULL, digi_handle, 0);
        tl_ic_field_DIGI_M1_RAM_LOW_PWR_set(NULL, digi_handle, 0);
    } else {
        tl_ic_field_DIGI_M1_RESET_set(NULL, digi_handle, 1);
        tl_ic_field_DIGI_M1_LOW_PWR_set(NULL, digi_handle, 1);
        tl_ic_field_DIGI_M1_RAM_LOW_PWR_set(NULL, digi_handle, 1);
    }

    PMC_RETURN();

} /* digi_m1_energy_state_set */



/*******************************************************************************
* pgmrclk_energy_state_reg_toggle
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function toggle the PGMRCLK_RESET register bit.  The effect of this
*   operation is to reset the registers in DIGI_M1 subsystem (including registers
*   in PGMRCLK).  Note that toggling of DIGI_M1_RESET only resets FEGE blocks 
*   within DIGI_M1.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
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
PUBLIC void pgmrclk_energy_state_reg_toggle(digi_handle_t *digi_handle)
{
    PMC_ENTRY();    

    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    tl_ic_field_PGMRCLK_RESET_set(NULL, digi_handle, 1);
    tl_ic_field_PGMRCLK_RESET_set(NULL, digi_handle, 0);

    PMC_RETURN();

} /* pgmrclk_energy_state_reg_toggle */

/*******************************************************************************
* digi_cpb_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the CPB subsystem as controlled
*   outside the CPB subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_cpb_energy_state_reg_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_DCPB_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_DCPB_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_DCPB_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_DCPB_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_DCPB_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_DCPB_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_cpb_energy_state_reg_set */

/*******************************************************************************
* digi_mcpb_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the MCPB subsystem as controlled
*   outside the MCPB subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_mcpb_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_MCPB_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_MCPB_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_MCPB_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_MCPB_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_MCPB_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_MCPB_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_mcpb_energy_state_reg_set */

/*******************************************************************************
* digi_lineotn_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LINEOTN subsystem as controlled
*   outside the LINEOTN subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_lineotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_LINEOTN_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_LINEOTN_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_LINEOTN_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_LINEOTN_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_LINEOTN_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_LINEOTN_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_lineotn_energy_state_reg_set */


/*******************************************************************************
* digi_sysotn_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LINEOTN subsystem as controlled
*   outside the SYSOTN subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_sysotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_SYSOTN_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_SYSOTN_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_SYSOTN_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_SYSOTN_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_SYSOTN_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_SYSOTN_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_sysotn_energy_state_reg_set */



/*******************************************************************************
* digi_lifd_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the LIFD subsystem as controlled
*   outside the LIFD subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_lifd_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_LIFD_MUX_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_LIFD_MUX_RESET_set(NULL, digi_handle, 0);
            tl_ic_field_LINE_DSIS_RESET_set(NULL, digi_handle, 0);
            tl_ic_field_LINE_DSIS_LOW_PWR_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_LIFD_MUX_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_LIFD_MUX_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_LINE_DSIS_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_LINE_DSIS_LOW_PWR_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_lifd_energy_state_reg_set */

/*******************************************************************************
* digi_cbrc_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the CBRC subsystem as controlled
*   outside the CBRC subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_cbrc_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_CBRC_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_CBRC_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_CBRC_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_CBRC_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_CBRC_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_CBRC_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_cbrc_energy_state_reg_set */

/*******************************************************************************
* digi_enet_sys_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the ENET_SYS subsystem as controlled
*   outside the ENET_SYS subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_enet_sys_energy_state_reg_set(digi_handle_t *digi_handle, 
                                               pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_ENET120_SYS_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_ENET120_SYS_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_ENET120_SYS_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_ENET120_SYS_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_enet_sys_energy_state_reg_set */

/*******************************************************************************
* digi_enet_line_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the ENET_LINE subsystem as controlled
*   outside the ENET_LINE subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_enet_line_energy_state_reg_set(digi_handle_t *digi_handle, 
                                                pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_ENET120_LINE_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_ENET120_LINE_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_ENET120_LINE_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_ENET120_LINE_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_enet_line_energy_state_reg_set */

#ifdef PMC_DEPRECATED

/*******************************************************************************
* digi_cbrc_line_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.04 release\n 
*
*   This function sets the energy state for the CBRC subsystem as controlled
*   outside the CBRC subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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

/* LCOV_EXCL_START */

PUBLIC void digi_cbrc_line_energy_state_reg_set(digi_handle_t *digi_handle, 
                                                pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_CBRC_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_CBRC_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_CBRC_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_CBRC_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_CBRC_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_CBRC_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_cbrc_line_energy_state_reg_set */

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

/*******************************************************************************
* digi_mapotn_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the MAPOTN subsystem as controlled
*   outside the MAPOTN subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_mapotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                             pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_MAPOTN_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_MAPOTN_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_MAPOTN_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_MAPOTN_RESET_set(NULL, digi_handle, 1);
            tl_ic_field_MAPOTN_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_MAPOTN_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_mapotn_energy_state_reg_set */

/*******************************************************************************
* digi_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function get the energy state for the all subsystems. \n\n
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state          - DIGI energy states
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_energy_state_reg_get(digi_handle_t *digi_handle, 
                                      digi_energy_state_t *energy_state )
{
    UINT32 low_pwr, ram_low_pwr, reset;

    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* MAPOTN */
    low_pwr      = tl_ic_field_MAPOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_MAPOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_MAPOTN_RESET_get(NULL, digi_handle);    
    energy_state->mapotn = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);
    
    /* DCPB */
    low_pwr     = tl_ic_field_DCPB_LOW_PWR_get    (NULL, digi_handle);
    ram_low_pwr = tl_ic_field_DCPB_RAM_LOW_PWR_get(NULL, digi_handle);
    reset       = tl_ic_field_DCPB_RESET_get      (NULL, digi_handle);
    energy_state->dcpb = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    /* MCPB */
    low_pwr     = tl_ic_field_MCPB_LOW_PWR_get    (NULL, digi_handle);
    ram_low_pwr = tl_ic_field_MCPB_RAM_LOW_PWR_get(NULL, digi_handle);
    reset       = tl_ic_field_MCPB_RESET_get      (NULL, digi_handle);
    energy_state->mcpb = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    /* COREOTN */
    low_pwr     = tl_ic_field_COREOTN_LOW_PWR_get    (NULL, digi_handle);
    ram_low_pwr = tl_ic_field_COREOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    reset       = tl_ic_field_COREOTN_RESET_get      (NULL, digi_handle);
    energy_state->coreotn = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);
    
    /* ENET LINE */
    low_pwr      = tl_ic_field_ENET120_LINE_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_ENET120_LINE_RESET_get(NULL, digi_handle);    
    energy_state->enet_line = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);
    
    /* ENET SYS */
    low_pwr      = tl_ic_field_ENET120_SYS_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_ENET120_SYS_RESET_get(NULL, digi_handle);    
    energy_state->enet_sys = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    /* LINEOTN */
    low_pwr      = tl_ic_field_LINEOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_LINEOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_LINEOTN_RESET_get(NULL, digi_handle);    
    energy_state->lineotn = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    /* SYSOTN */
    low_pwr      = tl_ic_field_SYSOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_SYSOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_SYSOTN_RESET_get(NULL, digi_handle);    
    energy_state->sysotn = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    /* CBRC */
    low_pwr      = tl_ic_field_CBRC_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr  = tl_ic_field_CBRC_RAM_LOW_PWR_get(NULL, digi_handle);
    reset        = tl_ic_field_CBRC_RESET_get(NULL, digi_handle);    
    energy_state->cbrc = util_global_recover_energy_state(low_pwr,ram_low_pwr, reset);

    PMC_RETURN();
} /*digi_energy_state_reg_get */

/*******************************************************************************
* digi_sifd_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the SIFD subsystem as controlled
*   outside the SIFD subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_sifd_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_SIFD_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_SIFD_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_SIFD_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_SIFD_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_SIFD_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_SIFD_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_sifd_energy_state_reg_set */



/*******************************************************************************
* digi_coreotn_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function sets the energy state for the COREOTN subsystem as controlled
*   outside the COREOTN subsystem module at the register level. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   energy_state           - enum, energy state
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
PUBLIC void digi_coreotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t energy_state)
{
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            tl_ic_field_COREOTN_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_COREOTN_RAM_LOW_PWR_set(NULL, digi_handle, 0);
            tl_ic_field_COREOTN_RESET_set(NULL, digi_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            tl_ic_field_COREOTN_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_COREOTN_RAM_LOW_PWR_set(NULL, digi_handle, 1);
            tl_ic_field_COREOTN_RESET_set(NULL, digi_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* digi_coreotn_energy_state_reg_set */
 
 
 
/* 
** Deprecated Functions 
*/

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_dcpb_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n 
*
*   This function gets the energy state for the CPB subsystem\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.

*
* OUTPUTS:
*   energy_state_ptr       - poointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_dcpb_energy_state_reg_get(digi_handle_t *digi_handle, 
                                          pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_DCPB_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_DCPB_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_DCPB_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();
} /* digi_dcpb_energy_state_reg_get */


 /*******************************************************************************
* digi_mcpb_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n  
*
*   This function gets the energy state for the MCPB subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mcpb_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    low_pwr = tl_ic_field_MCPB_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_MCPB_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_MCPB_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_mcpb_energy_state_reg_get */


/*******************************************************************************
* digi_lineotn_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n 
*
*   This function sets the energy state for the LINEOTN subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_lineotn_energy_state_reg_get(digi_handle_t      *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    low_pwr = tl_ic_field_LINEOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_LINEOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_LINEOTN_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } 
    else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } 
    else 
    {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }
    
    PMC_RETURN();

} /* digi_lineotn_energy_state_reg_get */


/*******************************************************************************
* digi_sysotn_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
*
*   This function gets the energy state for the LINEOTN subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_sysotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_SYSOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_SYSOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_SYSOTN_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_sysotn_energy_state_reg_get */


/*******************************************************************************
* digi_lifd_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the LIFD subsystem. \n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_lifd_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,mux_rst,dsis_rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_LIFD_MUX_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_LINE_DSIS_LOW_PWR_get(NULL, digi_handle);
    mux_rst = tl_ic_field_LIFD_MUX_RESET_get(NULL, digi_handle);
    dsis_rst = tl_ic_field_LINE_DSIS_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && mux_rst == 0 && dsis_rst)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && mux_rst == 1 && dsis_rst){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_lifd_energy_state_reg_get */


/*******************************************************************************
* digi_cbrc_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the CBRC subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_cbrc_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_CBRC_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_CBRC_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_CBRC_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_cbrc_energy_state_reg_get */


/*******************************************************************************
* digi_enet_sys_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the ENET_SYS subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - ponter to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_sys_energy_state_reg_get(digi_handle_t      *digi_handle, 
                                               pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_ENET120_SYS_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_ENET120_SYS_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_enet_sys_energy_state_reg_get */


/*******************************************************************************
* digi_enet_line_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the ENET_LINE subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_line_energy_state_reg_get(digi_handle_t *digi_handle, 
                                                pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_ENET120_LINE_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_ENET120_LINE_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_enet_line_energy_state_reg_get */


/*******************************************************************************
* digi_mapotn_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the MAPOTN subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mapotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                             pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_MAPOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_MAPOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_MAPOTN_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_mapotn_energy_state_reg_get */


/*******************************************************************************
* digi_sifd_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the SIFD subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr           - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_sifd_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_SIFD_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_SIFD_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_SIFD_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_sifd_energy_state_reg_get */


/*******************************************************************************
* digi_coreotn_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.02 release\n
* 
*
*   This function gets the energy state for the COREOTN subsystem.\n\n
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *energy_state_ptr      - pointer to enum, energy state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_coreotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr)
{
    UINT32 low_pwr,ram_low_pwr,rst;
    PMC_ENTRY();    
    PMC_ASSERT(digi_handle!=NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    low_pwr = tl_ic_field_COREOTN_LOW_PWR_get(NULL, digi_handle);
    ram_low_pwr = tl_ic_field_COREOTN_RAM_LOW_PWR_get(NULL, digi_handle);
    rst = tl_ic_field_COREOTN_RESET_get(NULL, digi_handle);

    if(low_pwr == 0 && ram_low_pwr == 0 && rst == 0)
    {
        *energy_state_ptr = PMC_ENERGY_STATE_RUNNING;
    } else if(low_pwr == 1 && ram_low_pwr == 1 && rst == 1){
        *energy_state_ptr = PMC_ENERGY_STATE_RESET;
    } else {
        *energy_state_ptr = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN();

} /* digi_coreotn_energy_state_reg_get */
                                     
/* LCOV_EXCL_STOP */                                     

#endif /* PMC_DEPRECATED */
                                                                          
/*
** End of file
*/
