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
*   DESCRIPTION : This file contains C functions for the creation and
*   destruction of DIGI devices and data paths.  Functions for usage in
*   higher level systems have prototypes defined in digi_api.h.  Other functions
*   should be considered private and for the internal use of the SW driver.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi.h"
#include "digi_loc.h"

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
** Global Variables
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_loopback_serdes_port_helper_cfg(digi_handle_t *digi_handle,
                                                       digi_serdes_port_t *port_ctxt_ptr, 
                                                       BOOL8 acb_only,
                                                       digi_loopback_t loopback_mode, 
                                                       BOOL8 enable);

PRIVATE PMC_ERROR get_enet_handle_link(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,        
                                       enet_handle_t **enet_handle_ptr,
                                       UINT32 *link_ptr);

PRIVATE PMC_ERROR digi_loopback_cbrc_cfg(digi_handle_t             *digi_handle,
                                         digi_cbr_client_chnl_t    *cbr_client_chnl_ptr,
                                         digi_loopback_t loopback_mode, 
                                         BOOL8 enable);

PRIVATE PMC_ERROR digi_serdes_facility_loopback_workaround(digi_handle_t       * digi_handle, 
                                                           digi_serdes_port_t  *port_ctxt_ptr);
#endif
/*
** Public Functions
*/


/*******************************************************************************
* digi_loopback_serdes_port_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables a loopback at one of the following 
*   interfaces: Line interface (S16), System Interface (S16) or SFI 5.1 
*   Interface (D8).\n\n
*
*  The loopback_mode can be one of the following: \n
*       -DIGI_SERDES_FACILITY_LOOPBACK: received data is looped back to the 
*           transmitter. \n
*       -DIGI_SERDES_DIAGNOSTIC_LOOPBACK: transmitted data is looped back 
*           to the receiver.  \n\n
*       -DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK: transmitted data is looped back 
*           to the receiver and also broacasted through TX interface.  \n\n
*
*   The port_ctxt_ptr can be a handle to one of the following types of ports: 
*   Line interface (S16), System Interface (S16), SFI 5.1 Interface (D8).\n\n
*
*   The loopback is enabled or disabled based on the 'enable' parameter.\n
*
*   Note that if a facility loopback is enabled on a SFI-S interface, the \n
*   inter-lane skew requirement may not be met. 
* 
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *port_ctxt_ptr  - SERDES port context pointer (see digi_serdes_port_init)
*   loopback_mode   - loopback type can be one of the following:\n
*                     DIGI_SERDES_DIAGNOSTIC_LOOPBACK (S16 or D8 interfaces)\n
*                     DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK (S16 interfaces)\n
*                     DIGI_SERDES_FACILITY_LOOPBACK (S16 interfaces)\n
*   enable         - enable or disable the loopback.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS    - when API execution is successful otherwise a descriptive 
*                    error is returned.
*
* NOTES:
*   See digi_loopback_serdes_bp_cfg for SERDES backplane loopbacks.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_port_cfg(digi_handle_t *digi_handle,
                                               digi_serdes_port_t *port_ctxt_ptr, 
                                               digi_loopback_t loopback_mode, 
                                               BOOL8 enable)
{
    PMC_ERROR rc;
    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_LOG_TRACE("digi_loopback_serdes_port_cfg: loopback mode =%d enable = %d\n",loopback_mode,enable);

    rc = digi_serdes_port_ctxt_validate(digi_handle, port_ctxt_ptr);

    if (PMC_SUCCESS == rc)
    {
        rc = digi_loopback_serdes_port_helper_cfg(digi_handle,port_ctxt_ptr, FALSE, loopback_mode, enable);
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_loopback_serdes_port_cfg */

/*******************************************************************************
* digi_loopback_serdes_port_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the status of the corresponding SERDES loopback. The 
*   port_ctxt_ptr and loopback_mode parameters determine which loopback to 
*   check.\n\n
*
*   The port_ctxt_ptr can be a handle to one of the following types of ports:\n
*       -Line interface (S16)\n
*       -System Interface (S16)\n
*       -SFI 5.1 Interface (D8)\n\n
*
*   The loopback_mode can be one of the following:\n
*       -DIGI_SERDES_FACILITY_LOOPBACK\n
*       -DIGI_SERDES_DIAGNOSTIC_LOOPBACK\n\n
*       -DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK\n\n
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance
*   *port_ctxt_ptr    - SERDES port context pointer (see digi_serdes_port_init)
*   loopback_mode     - loopback type can be one of the following:\n
*                       DIGI_SERDES_DIAGNOSTIC_LOOPBACK (S16 or D8 interfaces)\n
*                       DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK (S16 interfaces)\n
*                       DIGI_SERDES_FACILITY_LOOPBACK (S16 interfaces)
* OUTPUTS:
*   enabled_ptr      - Set to TRUE if the loopback is enabled.
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a descriptive 
*                      error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_port_status_get(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr, 
                                                      digi_loopback_t loopback_mode, 
                                                      BOOL8 *enabled_ptr)
{
    PMC_ERROR              rc = PMC_SUCCESS;
    digi_serdes_port_def_t *port_def_ptr;
    digi_serdes_port_rate_prov_state_t serdes_prov_state;
    s16_loopback_mode_t     s16_lpbk_mode;
    UINT32 itr;
    UINT32 serdes_id;
    s16_handle_t *s16_handle_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);


    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    if (NULL == enabled_ptr)
    {
        rc = DIGI_ERR_INVALID_ARG;
    }
        
    if (PMC_SUCCESS == rc)
    {
        *enabled_ptr = FALSE; 
        rc = digi_serdes_port_ctxt_validate(digi_handle, port_ctxt_ptr);
    }

    if (PMC_SUCCESS == rc)
    {
        /* cast serdes port context */
        port_def_ptr =  (digi_serdes_port_def_t*)port_ctxt_ptr;
        
        /* Check if port_uid is valid & retrieve the provisioning status */
        serdes_prov_state = digi_serdes_port_prov_status_get(digi_handle, 
                                                             port_def_ptr->port_uid);
        
        if (DIGI_SERDES_PORT_RATE_STATE_DEPROV == serdes_prov_state ||
            LAST_DIGI_SERDES_PORT_RATE_STATE == serdes_prov_state)
        {
            rc = DIGI_ERR_SERDES_PORT_NOT_PROV;
        }
        else
        {
            /* check if S16 serdes is not in reset mode */
            if (DIGI_SERDES_SFI_51_INTF != port_def_ptr->intf)
            { 
                if (DIGI_SERDES_FACILITY_LOOPBACK == loopback_mode)
                {
                    *enabled_ptr = (DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK == serdes_prov_state);
                }
                else
                {
                    s16_handle_ptr = NULL;
                    serdes_id = 0;
                    switch (port_def_ptr->intf)
                    {
                    case DIGI_SERDES_SYSTEM_INTF:                
                        for (itr = 0; itr < DIGI_SERDES_SYS_LANES_MAX; itr++) 
                        {
                            if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[itr].port_uid == port_def_ptr->port_uid)
                            {
                                s16_handle_ptr = digi_handle->s16_sys_handle;
                                serdes_id = itr;
                                break;
                            }
                        }
                        break;
                    case DIGI_SERDES_LINE_INTF:                
                        for(itr=0;itr<DIGI_SERDES_XFI_LANES_MAX;itr++)
                        {
                            if (digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[itr].port_uid == port_def_ptr->port_uid)
                            {
                                s16_handle_ptr = digi_handle->s16_line_handle;
                                serdes_id = itr;
                                break;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                    if (NULL != s16_handle_ptr)
                    {
                        s16_lpbk_mode = s16_loopback_cfg_get(s16_handle_ptr, serdes_id);
                        if (DIGI_SERDES_DIAGNOSTIC_LOOPBACK == loopback_mode &&
                            S16_DIAGNOSTIC_LOOPBACK == s16_lpbk_mode)
                        {
                            *enabled_ptr = TRUE;
                        }
                        if (DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK == loopback_mode &&
                            S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_lpbk_mode)
                        {
                            *enabled_ptr = TRUE;
                        }
                    }
                    else
                    {
                        rc = DIGI_ERR_INVALID_ARG;
                    }
                }
            }
            else
            {   
                for(serdes_id=0;serdes_id<DIGI_NUM_SFI51_PORTS_MAX;serdes_id++)
                {
                    if(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[serdes_id].port_uid == port_def_ptr->port_uid)
                    {
                        break;
                    }
                }
                
                if (DIGI_NUM_SFI51_PORTS_MAX > serdes_id)
                {
                    /* check if diagnostic loopback is enabled for SFI51 serdes */
                    *enabled_ptr = (DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK == serdes_prov_state);
                }
                else
                {
                    rc = DIGI_ERR_INVALID_ARG;
                }
            }
        }   
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_loopback_serdes_port_status_get */


/*******************************************************************************
* digi_loopback_serdes_ge_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables a GE management interface (D8) diagnostic
*   loopback. The transmitted data is looped back to the receiver.
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   ge_intf          - GE serdes interface type.
*   enable           - enable or disable the loopback.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a descriptive 
*                      error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_ge_cfg(digi_handle_t       *digi_handle,
                                             digi_ge_serdes_id_t  ge_intf,
                                             BOOL8                enable)
{
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    UINT8 serdes_id;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* retrieve d8_rx_2x handle associated to GE SERDES */
    result = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, 
                                        &serdes_id);
    if (PMC_SUCCESS == result) 
    {
        result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, 
                                             &d8_tx_2x_handle, &serdes_id);        
    }
    if (PMC_SUCCESS == result )
    {
        result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    }

    if (PMC_SUCCESS == result )
    {
        /* check SERDES state */
        if (TRUE == enable && FALSE == ge_ctxt->enabled)
        {
            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_SERDES_PORT_NOT_PROV);
        }
        else if (TRUE == enable && TRUE == ge_ctxt->loopback_enabled)
        {
            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_LOOPBACK_ALREADY_PROV);
        }
        else if (FALSE == enable && FALSE == ge_ctxt->loopback_enabled)
        {
            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_LOOPBACK_NOT_PROV);
        }
    }

    /* enable/disable loopback */
    if (PMC_SUCCESS == result && TRUE == enable)
    {
        result = d8_rx_2x_loopback_activate(d8_rx_2x_handle,serdes_id);
        if (PMC_SUCCESS == result) 
        {
            result = d8_tx_2x_loopback_activate(d8_tx_2x_handle,serdes_id);
        }
        if (PMC_SUCCESS == result) 
            d8_rx_2x_cdru_fifo_reset(d8_rx_2x_handle,serdes_id);
    }
    else
    {
        if (PMC_SUCCESS == result) 
        {
            result = d8_rx_2x_loopback_deactivate(d8_rx_2x_handle,serdes_id);
        }
        if (PMC_SUCCESS == result) 
        {
            result = d8_tx_2x_loopback_deactivate(d8_tx_2x_handle,serdes_id);
        }
    }

    /* update SERDES port status */
    if (PMC_SUCCESS == result)
    {
        ge_ctxt->loopback_enabled = enable;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_serdes_ge_loopback_cfg */

/*******************************************************************************
* digi_loopback_serdes_ge_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the status of the GE management interface (D8) loopback.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   ge_intf         - GE SERDES interface type.
*
* OUTPUTS:
*   *enabled_ptr    - Set to TRUE if the loopback is enabled.
*
* RETURNS:
*   PMC_SUCCESS     - when API execution is successful otherwise a descriptive 
*                     error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_ge_status_get(digi_handle_t *digi_handle,
                                                    digi_ge_serdes_id_t ge_intf, 
                                                    BOOL8 *enabled_ptr)
{
    d8_rx_2x_handle_t *d8_rx_2x_handle;
    d8_tx_2x_handle_t *d8_tx_2x_handle;
    digi_ge_serdes_ctxt_t *ge_ctxt;
    UINT8 serdes_id;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    *enabled_ptr = FALSE;

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* retrieve d8_rx_2x handle associated to GE SERDES */
    result = digi_ge_d8_rx_2x_handle_get(digi_handle, ge_intf, &d8_rx_2x_handle, &serdes_id);
    if (PMC_SUCCESS == result) 
    {
        result = digi_ge_d8_tx_2x_handle_get(digi_handle, ge_intf, &d8_tx_2x_handle, &serdes_id);        
    }
    if (PMC_SUCCESS == result)
    {
        result = digi_ge_serdes_ctxt_get(digi_handle, ge_intf, &ge_ctxt);
    }

    if (PMC_SUCCESS == result)
    {
        *enabled_ptr = ge_ctxt->loopback_enabled;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_loopback_serdes_ge_status_get */

/*******************************************************************************
* digi_loopback_serdes_bp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables a backplane LR (T8) or SR (C8/S16) SERDES 
*   loopback. The transmitted data is looped back to the receiver.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   ilkn_intf        - ILKN interface configured using digi_device_init
*   enable           - enable or disable the loopback.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS     - when API execution is successful otherwise a descriptive 
*                     error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_bp_cfg(digi_handle_t *digi_handle,
                                             digi_ilkn_intf_t ilkn_intf,
                                             BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr = NULL;
    digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_LOG_TRACE("digi_loopback_serdes_bp_cfg: ilkn_intf=%d enable=%d\n",ilkn_intf,enable);
    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ilkn_intf == DIGI_ILKN1 || ilkn_intf == DIGI_ILKN2, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* get ilkn configuration */
    if (DIGI_ILKN1 == ilkn_intf)
    {        
        sifd_1_cfg_ptr = &digi_handle->var.sifd_ilkn_1_cfg;
    }
    if (DIGI_ILKN2 == ilkn_intf)
    {
        sifd_2_cfg_ptr = &digi_handle->var.sifd_ilkn_2_cfg;
    }
    
    if (PMC_SUCCESS == rc)
    {
        rc = digi_bp_intf_serdes_cfg(digi_handle, 
                                     sifd_1_cfg_ptr, sifd_2_cfg_ptr,
                                     DIGI_BP_INTF_SERDES_LPBK_CFG,enable); 
    }  

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_loopback_serdes_bp_cfg  */

/*******************************************************************************
* digi_loopback_serdes_bp_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function retrieves the status of a backplane LR (T8) or SR (C8/S16) 
*   SERDES loopback. 
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   ilkn_intf       - ILKN interface configured using digi_device_init
*
* OUTPUTS:
*   *enabled_ptr    - Set to TRUE if the loopback is enabled.
*
* RETURNS:
*   PMC_SUCCESS     - when API execution is successful otherwise a descriptive 
*                     error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_bp_status_get(digi_handle_t *digi_handle,
                                             digi_ilkn_intf_t ilkn_intf,
                                             BOOL8 *enabled_ptr)

{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr = NULL;
    digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    *enabled_ptr = FALSE; 

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ilkn_intf != LAST_DIGI_ILKN_INTF, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* get ilkn configuration */
    if (DIGI_ILKN1 == ilkn_intf)
    {        
        sifd_1_cfg_ptr = &digi_handle->var.sifd_ilkn_1_cfg;
    }
    if (DIGI_ILKN2 == ilkn_intf)
    {
        sifd_2_cfg_ptr = &digi_handle->var.sifd_ilkn_2_cfg;
    }
     
    if (PMC_SUCCESS == rc)
    {
        rc = digi_bp_intf_serdes_loopback_status_get(digi_handle, 
                                            sifd_1_cfg_ptr, 
                                            sifd_2_cfg_ptr,
                                            enabled_ptr); 
    }  
    PMC_LOG_TRACE("digi_loopback_serdes_bp_status_get: ilkn_intf=%d enable=%d\n",ilkn_intf,*enabled_ptr);
    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_loopback_serdes_bp_serdes_get */





/*******************************************************************************
* digi_loopback_internal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables a loopback within the device. The type of 
*   loopback is specified by the loopback_mode. The input_pc_ptr handle
*   must correspond to the loopback mode as specified below: \n\n
* 
* \code
*       Loopback Type                                input_pc_ptr           
*       -------------                              ----------------------- 
*    DIGI_ENET_MAC_MLD_LOOPBACK                    digi_enet_client_chnl_t/digi_mapper_chnl_t 
*    DIGI_ENET_MAPOTN_G709_LOOPBACK                digi_mapper_chnl_t
*    DIGI_ENET_MAPOTN_GMP_LOOPBACK                 digi_mapper_chnl_t
*    DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK          digi_otn_server_chnl_t
*    DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK     digi_ilkn_link_t 
*    DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK digi_ilkn_link_t
*    DIGI_SIFD_COPI_FACILITY_LOOPBACK              digi_ilkn_link_t 
*    DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK           digi_otn_server_chnl_t
*    DIGI_CBRC_DIAG_LOOPBACK                       digi_cbr_client_chnl_t
* \endcode 
* 
*
*   The provisioning function for the input_pc_ptr handle is as follows:\n\n
*
*\code
*       input_pc_ptr          provisioning function 
*       -------------------  -------------- ---------
*    digi_enet_client_chnl_t digi_enet_line_prov/digi_enet_sys_prov
*    digi_mapper_chnl_t      digi_otn_mapper_prov 
*    digi_ilkn_link_t        digi_device_init 
*    digi_otn_server_chnl_t  digi_otn_server_line_prov/digi_otn_server_sys_prov
*    digi_cbr_client_chnl_t  digi_cbr_client_prov 
*\endcode
*  The loopback is enabled or disabled based on the 'enable' parameter.  \n
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*    *input_pc_ptr        - One of the handles listed above.
*   loopback_mode         - loopback type can be one of the above listed types.
*   enable                - enable or disable the loopack.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_internal_cfg(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *input_pc_ptr,        
                                            digi_loopback_t loopback_mode, 
                                            BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    lineotn_loopback_state_t loopback_state;
    digi_otn_server_chnl_t *otn_server_ptr;
    digi_ilkn_link_def_t * ilkn_link_ptr;
    digi_sifd_ilkn_inst_t ilkn_inst;
    sifd_loopback_cfg_t sifd_loopback;
    digi_cbr_client_chnl_t *cbrc_chnl_handle_ptr;
    enet_handle_t *enet_handle = NULL;
    UINT32 enet_link;
    emac_loopback_cfg_t emac_loopback;
    digi_mapper_chnl_def_t * digi_mapper_chnl_def;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_LOG_TRACE("digi_loopback_internal_cfg: loopback=%d enable = %d\n",loopback_mode,enable);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(input_pc_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    switch(loopback_mode) 
    {
    case DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        otn_server_ptr = (digi_otn_server_chnl_t *)input_pc_ptr;
        loopback_state = (TRUE == enable) ?  LINE_OTN_LOOPBACK_ENABLED : LINE_OTN_LOOPBACK_DISABLED;
        result = digi_lineotn_loopback_cfg(digi_handle,otn_server_ptr,LINE_OTN_TX_DIR,loopback_state);
        break;
    case DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK: 

        /* CPB <-> SIFD COPI 1/2 loopback at Interlaken  */

        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 
        sifd_loopback = (enable ? SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK : SIFD_DISABLE_LOOPBACK);
        result = sifd_loopback_cfg(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,sifd_loopback);
        break;
    

    case DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK: 
        /* Coreotn <-> Sifd COPI 1/2 loopback at Interlaken */
        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 
        sifd_loopback = (enable ? SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK : SIFD_DISABLE_LOOPBACK);
        result = sifd_loopback_cfg(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,sifd_loopback);
        break;

    case DIGI_SIFD_COPI_FACILITY_LOOPBACK: 
        /* sifd serial rx to sifd tx */
        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 
        sifd_loopback = (enable ? SIFD_COPI_FACILITY_LOOPBACK : SIFD_DISABLE_LOOPBACK);
        result = sifd_loopback_cfg(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,sifd_loopback);
        break;
    case DIGI_CBRC_DIAG_LOOPBACK: 
        cbrc_chnl_handle_ptr = (digi_cbr_client_chnl_t *)input_pc_ptr;
        result = digi_loopback_cbrc_cfg(digi_handle,cbrc_chnl_handle_ptr,loopback_mode,enable);
        break;


    case DIGI_ENET_MAC_MLD_LOOPBACK: 
        /* Note: the handle may either be a digi_mapper_chnl_def_t or a digi_enet_client_chnl_t */
        emac_loopback = (enable ? EMAC_MLD_LOOPBACK : EMAC_DISABLE_LOOPBACK);
        result = get_enet_handle_link(digi_handle,input_pc_ptr,&enet_handle,&enet_link);
        if (result == PMC_SUCCESS)
        {
            result = get_enet_handle_link(digi_handle,input_pc_ptr,&enet_handle,&enet_link);
        }
        PMC_ASSERT(enet_handle->emac_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
        result = emac_loopback_cfg(enet_handle->emac_handle, enet_link, emac_loopback);
        break;
        
    case DIGI_ENET_MAPOTN_G709_LOOPBACK: 
    case DIGI_ENET_MAPOTN_GMP_LOOPBACK: 

        /* Note: the handle is a digi_mapper_chnl_def_t */
        digi_mapper_chnl_def = (digi_mapper_chnl_def_t *)input_pc_ptr;
        enet_link = digi_mapper_chnl_def->switch_data.cpb_data.channel;
        result = get_enet_handle_link(digi_handle,input_pc_ptr,&enet_handle,&enet_link);
        if (result == PMC_SUCCESS)
        {
            result = etrans_loopback_cfg(enet_handle->etrans_handle, enet_link, enable);
        }
        else 
        {
            /* A mapotn handle is expected */
            if (result == DIGI_ERR_ENET_INVALID_CHNL_HANDLE)
                result = DIGI_ERR_ETRANS_INVALID_CHNL_HANDLE;
        }
        break;

    case DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        otn_server_ptr = (digi_otn_server_chnl_t *)input_pc_ptr;
        loopback_state = (TRUE == enable) ?  LINE_OTN_LOOPBACK_ENABLED : LINE_OTN_LOOPBACK_DISABLED;
        result = digi_lineotn_loopback_cfg(digi_handle,otn_server_ptr,LINE_OTN_TX_DIR,loopback_state);
        break;

    default:
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
 
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_loopback_internal_cfg */


/*******************************************************************************
* digi_loopback_internal_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of an internal loopback. See the 
*   description of the digi_loopback_internal_cfg function for the specification
*   of the loopback_mode and input_pc_ptr parameters.
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*    *input_pc_ptr   - handle (see digi_loopback_internal_cfg)
*   loopback_mode    - loopback type (see digi_loopback_internal_cfg)
*
* OUTPUTS:
*   *enabled_ptr     - Set to TRUE if the loopback is enabled.
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a descriptive 
*                           error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_internal_status_get(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,        
                                       digi_loopback_t loopback_mode, 
                                       BOOL8 *enabled_ptr)
{
    *enabled_ptr = FALSE; 
    PMC_ERROR result = PMC_SUCCESS;
    digi_otn_server_chnl_def_t *otn_server_chnl_def_ptr;
    digi_ilkn_link_def_t * ilkn_link_ptr;
    digi_sifd_ilkn_inst_t ilkn_inst;
    sifd_loopback_cfg_t sifd_loopback;
    digi_cbr_client_chnl_t *cbrc_chnl_handle_ptr;
    cbrc_loopback_t cbrc_loopback;
    UINT8 cbrc_link; 
    enet_handle_t *enet_handle = NULL;
    UINT32 enet_link;
    emac_loopback_cfg_t emac_loopback;
    BOOL8 etrans_enabled;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(input_pc_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);


    switch(loopback_mode) 
    {
    case DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        /* for both lineotn mld and lineotn otu loopback */
        otn_server_chnl_def_ptr = (digi_otn_server_chnl_def_t *)input_pc_ptr;
        *enabled_ptr = otn_server_chnl_def_ptr->loopback_enabled;
        break;

    case DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK: 

        /* CPB <-> SIFD COPI 1/2 loopback at Interlaken  */

        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 
        result = sifd_loopback_status_get(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,&sifd_loopback);
        if (result == PMC_SUCCESS)
        {
            *enabled_ptr = (sifd_loopback == SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK);
        }
        break;
    case DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK: 

        /* Coreotn <-> Sifd COPI 1/2 loopback at Interlaken */
        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 

        result = sifd_loopback_status_get(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,&sifd_loopback);
        if (result == PMC_SUCCESS)
        {
            *enabled_ptr = (sifd_loopback == SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK);
        }
        break;

    case DIGI_SIFD_COPI_FACILITY_LOOPBACK: 
        /* sifd serial rx to sifd tx */
        ilkn_link_ptr = (digi_ilkn_link_def_t *)input_pc_ptr;
        ilkn_inst = ilkn_link_ptr->ilkn_inst; 
        result = sifd_loopback_status_get(digi_handle->sifd_handle,(sifd_ilkn_inst_t)ilkn_inst,&sifd_loopback);
        if (result == PMC_SUCCESS)
        {
            *enabled_ptr = (sifd_loopback == SIFD_COPI_FACILITY_LOOPBACK);
        }
        break;


    case DIGI_CBRC_DIAG_LOOPBACK: 
        cbrc_chnl_handle_ptr = (digi_cbr_client_chnl_t *)input_pc_ptr;
        cbrc_link = digi_cbr_channel_num_get(digi_handle, cbrc_chnl_handle_ptr);
        result = cbrc_loopback_status_get(digi_handle->cbrc_handle,cbrc_link, &cbrc_loopback);
        *enabled_ptr = (cbrc_loopback == CBRC_DIAG_LOOPBACK);
        break;
    case DIGI_ENET_MAC_MLD_LOOPBACK: 
        /* Note: the handle may either be a digi_mapper_chnl_def_t or a digi_enet_client_chnl_t */
        result = get_enet_handle_link(digi_handle,input_pc_ptr,&enet_handle,&enet_link);
        if (result == PMC_SUCCESS)
        {
            PMC_ASSERT(enet_handle->emac_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
            result = emac_loopback_status_get(enet_handle->emac_handle, enet_link, &emac_loopback);
            *enabled_ptr = (emac_loopback == EMAC_MLD_LOOPBACK);
        }
        break;
    case DIGI_ENET_MAPOTN_G709_LOOPBACK: 
    case DIGI_ENET_MAPOTN_GMP_LOOPBACK: 
        /* Note: the handle is a digi_mapper_chnl_def_t */
        result = get_enet_handle_link(digi_handle,input_pc_ptr,&enet_handle,&enet_link);
        if (result == PMC_SUCCESS)
        {
            PMC_ASSERT(enet_handle->etrans_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
            result = etrans_loopback_status_get(enet_handle->etrans_handle, enet_link, &etrans_enabled);
            *enabled_ptr = etrans_enabled;
        }
        else
        {
            /* A mapotn handle is expected */
            if (result == DIGI_ERR_ENET_INVALID_CHNL_HANDLE)
                result = DIGI_ERR_ETRANS_INVALID_CHNL_HANDLE;
        }
        break;

    case DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        otn_server_chnl_def_ptr = (digi_otn_server_chnl_def_t *)input_pc_ptr;
        *enabled_ptr = otn_server_chnl_def_ptr->loopback_enabled;
        break;

    default:
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }
 
    PMC_LOG_TRACE("digi_loopback_internal_status_get: loopback=%d enable = %d\n",loopback_mode,*enabled_ptr);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_loopback_internal_status_get */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_serdes_loopback_c8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure C8 SERDES in loopback mode. 
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  enable_msk       - Bitmask of lanes to enable (0-7). 
*                     Bit 0 corresponds to lane 0.
*  serdes_port_mode - SERDES rate to configure
*  enable           - Enable or disable loopback
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS      - when API execution is successful otherwise a descriptive 
*                      error is returned.
*
* NOTES:
*   This function is not part of the swdrv API. To enable a C8 loopback, 
*   use digi_loopback_serdes_bp_cfg.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_c8_cfg(digi_handle_t *digi_handle, 
                                             UINT8 enable_msk,
                                             digi_serdes_port_mode_t serdes_port_mode,
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT8 itr;
    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    for ( itr = 0; itr < HOSTMSG_C8_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((enable_msk >> itr) & 0x1))
        {
            if (TRUE == enable) 
            {
                result = c8_loopback_activate(digi_handle->bp_sr_c8_handle,  itr);
            }
            else 
            {
                result = c8_loopback_deactivate(digi_handle->bp_sr_c8_handle,  itr);
                if (PMC_SUCCESS == result)
                {                        
                    (void) digi_serdes_c8_params_wr(digi_handle, (1<<itr));
                }
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_loopback_c8_cfg */

/*******************************************************************************
* digi_serdes_loopback_c8_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieve the status of the C8 SERDES loopback. 
*
* INPUTS:
*  digi_handle      - DIGI device handle.
*  enable_msk       - Bitmask of lanes to enable (0-7). \n
*                     Bit 0 corresponds to lane 0.
*  serdes_port_mode - SERDES rate to configure
*
* OUTPUTS:
*  *enabled_ptr     - Set to true if the loopback is enabled
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function is not part of the swdrv API. To retrieve the status of a 
*   C8 loopback, use digi_loopback_serdes_bp_status_get.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_c8_status_get(digi_handle_t *digi_handle, 
                                             UINT8 enable_msk,
                                             digi_serdes_port_mode_t serdes_port_mode,
                                             BOOL8 *enabled_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT8 itr;
    PMC_ENTRY();

    *enabled_ptr = FALSE;
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    for ( itr = 0; itr < HOSTMSG_C8_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((enable_msk >> itr) & 0x1))
        {
            result = c8_loopback_status_get(digi_handle->bp_sr_c8_handle,  itr,enabled_ptr);
            if (*enabled_ptr)
            {
                PMC_RETURN(result);
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_loopback_c8_status_get */
#endif

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_serdes_loopback_t8_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure T8 SERDES in loopback mode.
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  lane_0_19_enable_msk   - Bitmask of lanes 0-19 to enable. 
*                           Bit 0 corresponds to lane 0.
*  lane_20_39_enable_msk  - Bitmask of lanes 20-39 to enable. 
*                           Bit 0 corresponds to lane 20.
*  lane_0_19_port_mode    - SERDES rate to configure for lanes 0-19
*  lane_20_39_port_mode   - SERDES rate to configure for lanes 20-39
*  enable                 - Enable or disable loopback
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function is not part of the swdrv API. To retrieve the status of a 
*   T8 loopback, use digi_loopback_serdes_bp_cfg.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_t8_cfg(digi_handle_t *digi_handle, 
                                             UINT32 lane_0_19_enable_msk,
                                             UINT32 lane_20_39_enable_msk,
                                             digi_serdes_port_mode_t lane_0_19_port_mode, 
                                             digi_serdes_port_mode_t lane_20_39_port_mode,
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 itr;
    UINT32 serdes_id;
    UINT64 lane_0_39_enable_msk;
    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    lane_0_39_enable_msk = (UINT64) lane_20_39_enable_msk << 20;
    lane_0_39_enable_msk += lane_0_19_enable_msk;

    for (itr = 0; itr < HOSTMSG_T8_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((lane_0_39_enable_msk >> itr) & 0x1))
        {
            serdes_id =  digi_serdes_map_t8_serdes(itr);
            if (TRUE == enable) 
            {
                result = t8_loopback_activate(digi_handle->bp_lr_t8_handle[itr / 2],  serdes_id,T8_DIAGNOSTIC_LOOPBACK);
            }
            else 
            {
                result = t8_loopback_deactivate(digi_handle->bp_lr_t8_handle[itr / 2],  serdes_id);
                if (PMC_SUCCESS == result)
                {                        
                    (void) digi_serdes_one_t8_params_wr(digi_handle, itr);
                }
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_loopback_t8_cfg */

/*******************************************************************************
* digi_serdes_loopback_t8_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the status of the T8 loopback
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  lane_0_19_enable_msk   - Bitmask of lanes 0-19 to enable. 
*                           Bit 0 corresponds to lane 0.
*  lane_20_39_enable_msk  - Bitmask of lanes 20-39 to enable. 
*                           Bit 0 corresponds to lane 20.
*  lane_0_19_port_mode    - SERDES rate to configure for lanes 0-19
*  lane_20_39_port_mode   - SERDES rate to configure for lanes 20-39
*
* OUTPUTS:
*   *enabled_ptr          - TRUE if loopback is enabled
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function is not part of the swdrv API. To retrieve the status of a 
*   T8 loopback, use digi_loopback_serdes_bp_status_get.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_t8_status_get(digi_handle_t *digi_handle, 
                                             UINT32 lane_0_19_enable_msk,
                                             UINT32 lane_20_39_enable_msk,
                                             digi_serdes_port_mode_t lane_0_19_port_mode, 
                                             digi_serdes_port_mode_t lane_20_39_port_mode,
                                             BOOL8 *enabled_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 itr;
    UINT32 serdes_id;
    UINT64 lane_0_39_enable_msk;
    t8_loopback_mode_t loopback_mode;
    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    lane_0_39_enable_msk = (UINT64) lane_20_39_enable_msk << 20;
    lane_0_39_enable_msk += lane_0_19_enable_msk;

    *enabled_ptr = FALSE;
    for ( itr = 0; itr < HOSTMSG_T8_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((lane_0_39_enable_msk >> itr) & 0x1))
        {
            serdes_id =  digi_serdes_map_t8_serdes(itr);
            result = t8_loopback_status_get(digi_handle->bp_lr_t8_handle[itr / 2], 
                                            serdes_id,&loopback_mode);
            *enabled_ptr = (T8_DIAGNOSTIC_LOOPBACK == loopback_mode);
            if (*enabled_ptr == TRUE)
            {
                PMC_RETURN(result);
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_looopback_t8_status_get */
#endif

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_serdes_loopback_s16_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Configure S16 instance in loopback mode
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  enable_msk             - Bitmask of channels to configure. Bit 0
*                           corresponds to channel 0. 
*  serdes_rate            - SERDES rate to configure
*  enable                 - Enable or disable loopback
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function is not part of the swdrv API. To enable a SERDES loopback, 
*   use digi_loopback_serdes_port_cfg.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_s16_cfg(digi_handle_t *digi_handle, 
                                              hostmsg_s16_intf_type_t s16_intf_type, 
                                              UINT16 enable_msk,
                                              digi_serdes_port_mode_t serdes_rate, 
                                              BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;  
    s16_loopback_cfg_t  s16_lpbk_cfg;
    hostmsg_s16_serdes_rate_t s16_serdes_rate;
    s16_port_mode_t s16_port_mode;
    BOOL8 enet_mapping;
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    if ( TRUE == enable )
    {
        /* prepare s16 loopback config */
        
        s16_lpbk_cfg.loopback_mode = S16_DIAGNOSTIC_LOOPBACK;
        result = digi_serdes_s16_rate_convert(serdes_rate,
                                              &s16_serdes_rate,
                                              &s16_lpbk_cfg.serdes_rate,
                                              &s16_lpbk_cfg.is_otu_rate,
                                              &s16_port_mode); 
        enet_mapping = digi_serdes_s16_rate_enet_mapping_get(serdes_rate);
        s16_lpbk_cfg.serdes_if = (TRUE == enet_mapping ? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b);
    }

    for ( itr = 0; itr < HOSTMSG_SYS_S16_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((enable_msk >> itr) & 0x1))
        {
            if (TRUE == enable) 
            {
                result = s16_loopback_activate(digi_handle->s16_sys_handle,itr,&s16_lpbk_cfg);
            }
            else 
            {
                result = s16_loopback_deactivate(digi_handle->s16_sys_handle, itr);
                if (PMC_SUCCESS == result) {
                    (void)digi_serdes_s16_sys_params_wr(digi_handle, (1<<itr));
                }
            }
        }
    }
    PMC_RETURN(result);
} /* digi_serdes_loopback_s16_cfg */

/*******************************************************************************
* digi_serdes_loopback_s16_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieve the S16 loopback status
*
* INPUTS:
*  digi_handle            - DIGI device handle.
*  s16_intf_type          - S16 interface type
*  enable_msk             - Bitmask of channels to configure. Bit 0
*                           corresponds to channel 0. 
*  serdes_rate            - SERDES rate to configure
*
* OUTPUTS:
*  *enabled_ptr           - Set to true if the loopback is enabled
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function is not part of the swdrv API. To retrieve the status of a 
*   SERDES loopback, use digi_loopback_serdes_bp_status_get or 
*   digi_loopack_serdes_port_status_get.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_serdes_loopback_s16_status_get(digi_handle_t *digi_handle, 
                                                     hostmsg_s16_intf_type_t s16_intf_type, 
                                                     UINT16 enable_msk,
                                                     digi_serdes_port_mode_t serdes_rate, 
                                                     BOOL8 *enabled_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;  
    s16_loopback_mode_t loopback_mode;
    UINT32 itr;

    PMC_ENTRY();

    *enabled_ptr = FALSE;
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    for ( itr = 0; itr < HOSTMSG_SYS_S16_MASK_SIZE && result == PMC_SUCCESS; itr++)
    {
        if (0x1 == ((enable_msk >> itr) & 0x1))
        {
            result = s16_loopback_status_get(digi_handle->s16_sys_handle,itr,&loopback_mode);
            if (S16_DIAGNOSTIC_LOOPBACK == loopback_mode) 
            {
                *enabled_ptr = TRUE;
                PMC_RETURN(result);
            }
            else 
            {
                *enabled_ptr = FALSE;
                PMC_RETURN(result);
            }
        }
    }

    PMC_RETURN(result);
} /* digi_serdes_loopback_s16_status_get */
#endif

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_loopback_serdes_port_acb_only_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function is used for post-mld loopbacks when there is no packet 
*   monitoring (MPMO) on the datapath. All s16 SERDES of a port will be placed
*   in ACB rx2tx loopback.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *port_ctxt_ptr  - serdes port context pointer
*   enable          - enable or disable the loopback.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS    - when API execution is successful otherwise a descriptive 
*                    error is returned.
*
*
* NOTES:
*   This function is not part of the swdrv API. To enable a SERDES loopback, 
*   use digi_loopback_serdes_port_cfg.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_loopback_serdes_port_acb_only_cfg(digi_handle_t *digi_handle,
                                                        digi_serdes_port_t *port_ctxt_ptr, 
                                                        BOOL8 enable)
{
    PMC_ERROR rc;
    PMC_ENTRY();
    rc = digi_loopback_serdes_port_helper_cfg(digi_handle,port_ctxt_ptr, TRUE,DIGI_SERDES_FACILITY_LOOPBACK , enable);
    PMC_RETURN(rc);
} /* digi_loopback_serdes_port_acb_only_cfg */

#endif /* DOXYGEN_PUBLIC_ONLY */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_loopback_serdes_port_helper_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function is a helper function (used by digi_loopback_serdes_port_cfg and
*   digi_loopback_serdes_port_acb_only_cfg) that configures a SERDES loopback.
*   
*   Public loopbacks:
*       -Line interface (S16)
*       -System Interface (S16)
*       -SFI 5.1 Interface (D8)
*   
*   The loopback can be either a facility loopback (the received data is looped 
*   back to the transmitter) or a diagnostic loopback (the transmitted data is 
*   looped back to the receiver). The loopback mode can be either 
*   DIGI_SERDES_FACILITY_LOOPBACK, DIGI_SERDES_DIAGNOSTIC_LOOPBACK , 
*   or DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK for the S16 line and system interfaces. 
*   The loopback mode for the SFI 5.1 D8 interface can only
*   be DIGI_SERDES_DIAGNOSTIC_LOOPBACK.
*
*   The loopback is enabled or disabled based on the 'enable' parameter.
*
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   *port_ctxt_ptr     - SERDES port context pointer
*   acb_only           - when this field is TRUE, loopback_mode is ignored
*                        and s16 loopback_mode is forced to S16_ACB_ONLY_LOOPBACK. 
*                        Otherwise loopback_mode is used.
*   loopback_mode      - loopback type can be one of the following:\n
*                         -DIGI_SERDES_DIAGNOSTIC_LOOPBACK (S16 or D8 interfaces)\n
*                         -DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK (S16 interfaces)\n
*                         -DIGI_SERDES_FACILITY_LOOPBACK (S16 interfaces)\n
*   enable             - enable or disable the loopback.
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS        - when API execution is successful otherwise a descriptive 
*                        error is returned.
*
*
* NOTES:
*   This function is not part of the public API. 
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_loopback_serdes_port_helper_cfg(digi_handle_t *digi_handle,
                                                       digi_serdes_port_t *port_ctxt_ptr, 
                                                       BOOL8 acb_only,
                                                       digi_loopback_t loopback_mode, 
                                                       BOOL8 enable)
{
    PMC_ERROR              rc = PMC_SUCCESS;
    digi_serdes_intf_t     serdes_intf;
    digi_serdes_port_def_t *port_def_ptr;
    s16_loopback_cfg_t     s16_lpbk_cfg;
    UINT8                  port_idx;
    UINT32                 itr;
    hostmsg_s16_serdes_rate_t s16_serdes_rate;
    digi_serdes_port_rate_prov_state_t next_rate_prov_state,cur_rate_prov_state;
    BOOL8 prov_status;
    digi_serdes_port_mode_t port_mode;
    digi_serdes_prov_state_t prov_state;
    s16_port_mode_t s16_port_mode;
    BOOL8 enet_mapping;
    BOOL8 disable_fw_proc = FALSE;
    odu_struct_t                 *odu_ptr = NULL;
    util_global_switch_data_def_t *switch_data_ptr = NULL;
    hostmsg_opsa_subsystem_cfg_t subsystem = LAST_OPSA_HOSTMSG_SS;
    UINT32 tx_inv = 0, rx_inv = 0;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
        
    /* cast serdes port context */
    port_def_ptr =  (digi_serdes_port_def_t*) port_ctxt_ptr;
    
    /* Check if port_uid is valid & retrieve the provisioning status */
    cur_rate_prov_state = digi_serdes_port_prov_status_get(digi_handle, port_def_ptr->port_uid);
    
    if (TRUE == enable) 
    {
        /* check if the SERDES is provisioned */
        if (DIGI_SERDES_PORT_RATE_STATE_DEPROV == cur_rate_prov_state)
        {
            PMC_RETURN(DIGI_ERR_SERDES_PORT_NOT_PROV);
        }
        else if (DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK ==cur_rate_prov_state || 
                   DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK == cur_rate_prov_state) 
        {
            PMC_RETURN(DIGI_ERR_LOOPBACK_ALREADY_PROV);
        }
        next_rate_prov_state = (DIGI_SERDES_FACILITY_LOOPBACK == loopback_mode
                                ? DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK 
                                : DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK);
    }
    else 
    {
        /* check if loopback is active */
        if (DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK != cur_rate_prov_state && 
            DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK != cur_rate_prov_state) 
        {
            PMC_RETURN(DIGI_ERR_LOOPBACK_NOT_PROV);
        }
        next_rate_prov_state = DIGI_SERDES_PORT_RATE_STATE_PROV;
    }

    /* get interface */
    serdes_intf = ((digi_serdes_port_def_t*)port_ctxt_ptr)->intf;

    /* Initialize S16 loopback configuration */
    PMC_MEMSET((void *)&s16_lpbk_cfg, 0, sizeof(s16_loopback_cfg_t));
    
    /* validate loopback mode */
    if (TRUE == acb_only)
    {
        if (DIGI_SERDES_SFI_51_INTF == serdes_intf)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        else
        {
            s16_lpbk_cfg.loopback_mode = S16_ACB_ONLY_LOOPBACK;
        }
    }
    else 
    {
        if (DIGI_SERDES_SFI_51_INTF != serdes_intf && DIGI_SERDES_DIAGNOSTIC_LOOPBACK == loopback_mode) 
        {
            s16_lpbk_cfg.loopback_mode = S16_DIAGNOSTIC_LOOPBACK;
        }
        else if (DIGI_SERDES_SFI_51_INTF != serdes_intf && DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK == loopback_mode) 
        {
            s16_lpbk_cfg.loopback_mode = S16_DIAGNOSTIC_WITH_TX_LOOPBACK;
        }
        else if (DIGI_SERDES_SFI_51_INTF != serdes_intf && DIGI_SERDES_FACILITY_LOOPBACK == loopback_mode) 
        {
            s16_lpbk_cfg.loopback_mode = S16_FACILITY_LOOPBACK;
        } 
        else if ( !(DIGI_SERDES_SFI_51_INTF  == serdes_intf && DIGI_SERDES_DIAGNOSTIC_LOOPBACK == loopback_mode)  )
        {
            PMC_RETURN(DIGI_ERR_INVALID_LOOPBACK_MODE);
        }
    } 
    /* get serdes provisioning state */
    rc = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                        port_ctxt_ptr,
                                                        TRUE,
                                                        &prov_status,
                                                        &port_mode,
                                                        &prov_state);
      
    /* prepare s16 loopback configuration */
    if (PMC_SUCCESS == rc && DIGI_SERDES_SFI_51_INTF != port_def_ptr->intf)
    {
        rc = digi_serdes_s16_rate_convert(port_mode,
                                          &s16_serdes_rate,
                                          &s16_lpbk_cfg.serdes_rate,
                                          &s16_lpbk_cfg.is_otu_rate,
                                          &s16_port_mode);
        enet_mapping = digi_serdes_s16_rate_enet_mapping_get(port_mode);
        s16_lpbk_cfg.serdes_if = TRUE == enet_mapping ? ACB_FT_SERDES_20b : ACB_FT_SERDES_32b;
    }

    if (loopback_mode == DIGI_SERDES_FACILITY_LOOPBACK) 
    {
        disable_fw_proc = digi_disable_fw_acb_proc(digi_handle, 
                                                   port_ctxt_ptr,
                                                   &odu_ptr,
                                                   &switch_data_ptr,
                                                   &subsystem);
    }
    if (enable == TRUE && disable_fw_proc== TRUE)
    {
        if (NULL != odu_ptr)
        {
            digi_opsa_cfg_mpmo_one_port_drop(digi_handle,
                                             subsystem,
                                             odu_ptr->switch_data.oduk_data.channel);
        }
        else if (NULL != switch_data_ptr) 
        {
            /*
             * When deactivating OPSA the port_uid, second parameter, is
             * don't care so pass in 0 as it is not used.
             */
            rc = digi_cpb_update_nominal_replacement(digi_handle, 
                                                     0,
                                                     switch_data_ptr,
                                                     DIGI_OPSA_CFG_ACTION_DEACTIVATE,
                                                     TRUE);
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_NULL_HANDLE, 0, 0);
        }
    }
   

    /* Configure appropriate serdes based on interface type */
    switch (port_def_ptr->intf)
    {
    case DIGI_SERDES_SYSTEM_INTF:
    {
        for (itr = 0; itr < DIGI_SERDES_SYS_LANES_MAX; itr++) 
        {
            if(PMC_SUCCESS == rc && digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[itr].port_uid == port_def_ptr->port_uid)
            {
                tx_inv = (UTIL_GLOBAL_DIR_TX & digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].polarity_inversed) ? 1 : 0;
                rx_inv = (UTIL_GLOBAL_DIR_RX & digi_handle->var.serdes_ctxt.serdes_cfg->s16_sys_cfg[itr].polarity_inversed) ? 1 : 0;

                /* case 1) TX pin has P/N swap on board layout, Rx pin doesn't have P/N swap 
                            - the Tx pin inversion must be always turn on. */
                /* Do nothing */

                
                /* case 2) TX pin has P/N swap on board layout, Rx pin has P/N swap */
                if (tx_inv == 1 && rx_inv == 1) 
                {
                    if(S16_FACILITY_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling facility loopback, the Tx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_TX, FALSE);
                        } 
                        else
                        {
                            /* When disabling facility loopback, the Tx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_TX, TRUE);
                        }
                    } 
                    else if(S16_DIAGNOSTIC_LOOPBACK == s16_lpbk_cfg.loopback_mode 
                              || S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling diagnostic loopback, the Rx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_RX, FALSE);
                        } 
                        else 
                        {
                            /* When disabling diagnostic loopback, the Rx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_RX, TRUE);
                        }
                    }
                }
                /* case 3) TX pin doesn't have P/N swap on board layout, Rx pin has P/N swap */
                else if (tx_inv == 0 && rx_inv == 1) 
                {
                    if(S16_FACILITY_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling facility loopback, the Tx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_TX, TRUE);
                        } 
                        else  
                        {
                            /* When disabling facility loopback, the Tx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_TX, FALSE);
                        }
                    } 
                    else if(S16_DIAGNOSTIC_LOOPBACK == s16_lpbk_cfg.loopback_mode 
                              || S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling diagnostic loopback, the Rx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_RX, FALSE);
                        } 
                        else 
                        {
                            /* When disabling diagnostic loopback, the Rx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_sys_handle,itr, UTIL_GLOBAL_DIR_RX, TRUE);
                        }
                    }
                }
                if (TRUE == enable)
                {
                    rc = s16_loopback_activate(digi_handle->s16_sys_handle,itr,&s16_lpbk_cfg);                    
                }
                else
                {
                    rc = s16_loopback_deactivate(digi_handle->s16_sys_handle, itr);
                    
                    if (PMC_SUCCESS == rc)
                    {                        
                        (void) digi_serdes_s16_sys_params_wr(digi_handle, (1<<itr));
                    }                    
                    
                }
            }
        }
        /* disable LIFD MUX PIN_LOS during loopback */
        if ( PMC_SUCCESS == rc)
        {
            if (DIGI_SERDES_PORT_NO_LOS_SOURCE != port_def_ptr->los_pin)
            {
                rc = digi_serdes_port_lifd_sifd_mux_pin_los_cfg(digi_handle, port_def_ptr, enable == TRUE ? FALSE : TRUE);
            }

             /* If the prov state is Unassigned - ignore the error (lifd/sifd not configured) */
            if (rc != PMC_SUCCESS && prov_state == DIGI_SERDES_PROV_STATE_UNASSIGNED)
            {
                PMC_LOG_TRACE("SERDES prov state = Unassigned => Ignoring lifd/sifd mux pin los cfg error: %x\n",rc);
                rc = PMC_SUCCESS;
            }
        }
        break;
    }
    case DIGI_SERDES_LINE_INTF:
    {


        for(itr=0;itr<DIGI_SERDES_XFI_LANES_MAX;itr++)
        {
            if(PMC_SUCCESS == rc && digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[itr].port_uid == port_def_ptr->port_uid)
            {
                
                tx_inv = (UTIL_GLOBAL_DIR_TX & digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].polarity_inversed) ? 1 : 0;
                rx_inv = (UTIL_GLOBAL_DIR_RX & digi_handle->var.serdes_ctxt.serdes_cfg->s16_line_cfg[itr].polarity_inversed) ? 1 : 0;

                /* case 1) TX pin has P/N swap on board layout, Rx pin doesn't have P/N swap 
                            - the Tx pin inversion must be always turn on. */
                /* Do nothing */

                
                /* case 2) TX pin has P/N swap on board layout, Rx pin has P/N swap */
                if (tx_inv == 1 && rx_inv == 1) 
                {
                    if(S16_FACILITY_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling facility loopback, the Tx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_TX, FALSE);
                        } 
                        else
                        {
                            /* When disabling facility loopback, the Tx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_TX, TRUE);
                        }
                    } 
                    else if(S16_DIAGNOSTIC_LOOPBACK == s16_lpbk_cfg.loopback_mode 
                              || S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling diagnostic loopback, the Rx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_RX, FALSE);
                        } 
                        else 
                        {
                            /* When disabling diagnostic loopback, the Rx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_RX, TRUE);
                        }
                    }
                }
                /* case 3) TX pin doesn't have P/N swap on board layout, Rx pin has P/N swap */
                else if (tx_inv == 0 && rx_inv == 1) 
                {
                    if(S16_FACILITY_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling facility loopback, the Tx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_TX, TRUE);
                        } 
                        else  
                        {
                            /* When disabling facility loopback, the Tx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_TX, FALSE);
                        }
                    } 
                    else if(S16_DIAGNOSTIC_LOOPBACK == s16_lpbk_cfg.loopback_mode 
                              || S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_lpbk_cfg.loopback_mode)
                    {
                        if(enable) 
                        {
                            /* When enabling diagnostic loopback, the Rx pin inversion must be turn off. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_RX, FALSE);
                        } 
                        else 
                        {
                            /* When disabling diagnostic loopback, the Rx pin inversion must be turn on. */
                            s16_polarity_cfg(digi_handle->s16_line_handle,itr, UTIL_GLOBAL_DIR_RX, TRUE);
                        }
                    }
                }

                if (TRUE == enable)
                {
                    rc = s16_loopback_activate(digi_handle->s16_line_handle,itr,&s16_lpbk_cfg);                        
                }
                else
                {
                    rc = s16_loopback_deactivate(digi_handle->s16_line_handle, itr);

                    if (PMC_SUCCESS == rc)
                    {                        
                        (void) digi_serdes_s16_line_params_wr(digi_handle, (1<<itr));
                    }
                }
            }
        }
        /* disable LIFD MUX PIN_LOS during loopback */
        if (PMC_SUCCESS == rc)
        {
            if (DIGI_SERDES_PORT_NO_LOS_SOURCE != port_def_ptr->los_pin)
            {
                rc = digi_serdes_port_lifd_sifd_mux_pin_los_cfg(digi_handle, port_def_ptr, enable == TRUE ? FALSE : TRUE);
            }

            /* If the prov state is Unassigned - ignore the error (lifd/sifd not configured) */
            if (rc != PMC_SUCCESS && prov_state == DIGI_SERDES_PROV_STATE_UNASSIGNED)
            {
                PMC_LOG_TRACE("SERDES prov state = Unassigned => Ignoring lifd/sifd mux pin los cfg error: %x\n",rc);
                rc = PMC_SUCCESS;
            }
        }
        break;
    }
    case DIGI_SERDES_SFI_51_INTF:
    {
        for(itr=0;itr<DIGI_NUM_SFI51_PORTS_MAX;itr++)
        {
            if(PMC_SUCCESS == rc && digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[itr].port_uid == port_def_ptr->port_uid)
            {
                port_idx = itr;
                
                if (TRUE == enable) 
                {
                    rc = sfi51_d8_rx_18x_loopback_activate(digi_handle->sfi51_d8_rx_18x_handle[port_idx]); 
                    if (PMC_SUCCESS == rc)
                    {
                        rc = sfi51_d8_tx_18x_loopback_activate(digi_handle->sfi51_d8_tx_18x_handle[port_idx]); 
                    }
                }
                else
                {
                    rc = sfi51_d8_tx_18x_loopback_deactivate(digi_handle->sfi51_d8_tx_18x_handle[port_idx]); 
                    if (PMC_SUCCESS == rc)
                    {
                        rc = sfi51_d8_rx_18x_loopback_deactivate(digi_handle->sfi51_d8_rx_18x_handle[port_idx]); 
                    }            
                }
            }
        }
        break;
    }
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }


    if(rc == PMC_SUCCESS)
    {
        if(S16_FACILITY_LOOPBACK == s16_lpbk_cfg.loopback_mode &&
           enable == FALSE)
        {
            rc = digi_serdes_facility_loopback_workaround(digi_handle,
                                                          port_ctxt_ptr);
        }
    }


    if (PMC_SUCCESS == rc)
    {        
        rc = digi_serdes_port_rate_state_set(digi_handle, port_ctxt_ptr, port_mode, prov_state, next_rate_prov_state);
    }

    if (enable == FALSE && disable_fw_proc== TRUE)
    {
        if (NULL != odu_ptr)
        {
            digi_otn_remove_nominal_replacement(digi_handle,odu_ptr);
        }
        else if (NULL != switch_data_ptr)
        {
            digi_serdes_port_def_t *serdes_ptr = (digi_serdes_port_def_t *)port_ctxt_ptr;
            digi_cpb_update_nominal_replacement(digi_handle, 
                                                serdes_ptr->port_uid,
                                                switch_data_ptr,
                                                DIGI_OPSA_CFG_ACTION_ACTIVATE,
                                                TRUE);
        }
    }

    PMC_RETURN(rc);
} /* digi_loopback_serdes_port_cfg */

#endif /* DOXYGEN_PUBLIC_ONLY */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* get_enet_handle_link
* ______________________________________________________________________________
*
* DESCRIPTION:
*
   Retrieve the enet handle andl link from a mapper or enet channel handle. 
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance
*   *input_pc_ptr     - mapper or enet channel handle
*
* OUTPUTS:
*   *link_ptr         - enet link 
*   *enet_handle_ptr  - enet handle
*  None
*
* RETURNS:
*   PMC_SUCCESS       - when API execution is successful otherwise a descriptive 
*                       error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR get_enet_handle_link(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,        
                                       enet_handle_t **enet_handle_ptr,
                                       UINT32 *link_ptr)
{
    PMC_ERROR  rc = PMC_SUCCESS;
    digi_mapper_chnl_def_t * chnl_def = (digi_mapper_chnl_def_t *)input_pc_ptr;

    PMC_ENTRY();

    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_def);
    

    *link_ptr = chnl_def->switch_data.cpb_data.channel;
    if (chnl_def->switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS)
    {
        *enet_handle_ptr = digi_handle->enet_sys_handle;
    }
    else if (chnl_def->switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE)
    {
        *enet_handle_ptr = digi_handle->enet_line_handle;
    }
    else
    {
        rc = DIGI_ERR_ENET_INVALID_CHNL_HANDLE;
    }
    PMC_RETURN(rc);
} /* get_enet_handle_link */

#endif /* DOXYGEN_PUBLIC_ONLY */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_loopback_cbrc_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the cbrc loopback for an already provisioned link.
*
* INPUTS:
*   digi_handle           - pointer to digi handle instancer
*   *cbr_client_chnl_ptr  - client channel pointer
*   loopback_mode         - cbrc facility/diagnostic
*   enable                - enable/disable loopback
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*       PMC_SUCCESS or an error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_loopback_cbrc_cfg(digi_handle_t             *digi_handle,
                                         digi_cbr_client_chnl_t    *cbr_client_chnl_ptr,
                                         digi_loopback_t loopback_mode, 
                                         BOOL8 enable)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    digi_serdes_prov_state_t prov_state = DIGI_SERDES_PROV_STATE_NO_SERDES;
    UINT32 port_uid = DIGI_SERDES_PROV_STATE_UNASSIGNED;
    UINT32 i,dsi_stream;
    UINT8 cbrc_link;
    cbrc_loopback_t cbrc_loopback,current_loopback;
    digi_cbr_client_chnl_def_t* chan_ptr = (digi_cbr_client_chnl_def_t *)cbr_client_chnl_ptr;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
 
    DIGI_CHNL_HANDLE_MAGIC_CHECK(cbr_client_chnl_ptr);
    
    PMC_ASSERT(loopback_mode == DIGI_CBRC_DIAG_LOOPBACK,DIGI_ERR_INVALID_ARG, loopback_mode,0);
    cbrc_loopback = CBRC_DIAG_LOOPBACK;
    
    cbrc_link = digi_cbr_channel_num_get(digi_handle, cbr_client_chnl_ptr);
    port_uid = chan_ptr->port_uid;

    PMC_LOG_TRACE("cbr loopback cfg: loopback=%d link = %d uid=%x\n" ,
            cbrc_loopback,cbrc_link,port_uid);

    rc = cbrc_loopback_status_get(digi_handle->cbrc_handle,cbrc_link,&current_loopback);
    PMC_ASSERT(rc == PMC_SUCCESS,rc,0,0);
    

    if (current_loopback != CBRC_DISABLE_LOOPBACK && enable)
    {
        PMC_RETURN(DIGI_ERR_LOOPBACK_ALREADY_PROV);
    }
    if (current_loopback == CBRC_DISABLE_LOOPBACK && !enable)
    {
        PMC_RETURN(DIGI_ERR_LOOPBACK_NOT_PROV);
    }
    PMC_ASSERT(port_uid != DIGI_SERDES_UNASSIGNED,DIGI_ERR_LOOPBACK_INVALID_PORT_UID,0,0);

    /* Check if serdes (line-side or system-side) is provisioned */
    for (i=0; i < DIGI_SERDES_DSI_LANES_MAX; i++) 
    {
        if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].port_uid == port_uid)
        {
            prov_state = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state;
            break;
        }
        if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].port_uid == port_uid)
        {
            prov_state = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state;
            break;
        }
    }
    if (i >= DIGI_SERDES_DSI_LANES_MAX)
    {
        PMC_RETURN(DIGI_ERR_LOOPBACK_INVALID_PORT_PROV);
    }

    /* Save the dsi stream # */
    dsi_stream = i;
   
     
    /* For both facility and diagnostic loopbacks */
    if (rc == PMC_SUCCESS)
    {
        rc = cbrc_loopback_cfg(digi_handle->cbrc_handle,cbrc_link, 
                    (enable ? cbrc_loopback : CBRC_DISABLE_LOOPBACK));
    }
    PMC_RETURN(rc);
} /* digi_loopback_cbrc_cfg */

/*******************************************************************************
* digi_otn_server_chnl_port_uid_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The is a helper function which obtains a digi_otn_server_chnl_def_t.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   port_uid               - the associated port UID 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   @return digi_otn_server_chnl_def_t - pointer to the otn server chnl_ctxt.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_otn_server_chnl_def_t* digi_otn_server_chnl_port_uid_get(digi_handle_t* digi_handle, 
                                                                       UINT32 port_uid)
{
    digi_otn_server_chnl_def_t* chnl_ctxt_ptr = NULL;
    UINT32 itr;
    PMC_ENTRY();
    
    for (itr = 0;itr < DIGI_OTN_SERVER_CHNL_MAX && chnl_ctxt_ptr == NULL; itr++)
    {
        if (digi_handle->handle_pool.otn_server_chnl[itr].header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG &&
            digi_handle->handle_pool.otn_server_chnl[itr].port_uid == port_uid)
        {
            chnl_ctxt_ptr = &digi_handle->handle_pool.otn_server_chnl[itr];
        }
    }
    if (digi_handle->handle_pool.sysotn_server_chnl.header.prov_state != UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG &&
        digi_handle->handle_pool.sysotn_server_chnl.port_uid == port_uid)
    {
        chnl_ctxt_ptr = &digi_handle->handle_pool.sysotn_server_chnl;
    }
    PMC_RETURN(chnl_ctxt_ptr);
} /* digi_otn_server_chnl_port_uid_get */

/*******************************************************************************
* digi_disable_fw_acb_proc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns TRUE if FW ACB_FT monitoring should be disabled.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer 
*
* OUTPUTS:
*   *odu_ptr             - HO ODU configuration
*   *switch_data_ptr     - If not a OTN handle then used to hold the data
*                          returned for ENET LINE/SYS or CBRC.
*   *subsystem           - subsystem to configure
*
* RETURNS:
*   TRUE: there is a FW procedure associated to this ACB_FT
*   FALSE: there is a no FW procedure associated to this ACB_FT
*di
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_disable_fw_acb_proc(digi_handle_t       * digi_handle, 
                                      digi_serdes_port_t   *port_ctxt_ptr,
                                      odu_struct_t                 **odu_ptr,
                                      util_global_switch_data_def_t **switch_data_ptr,
                                      hostmsg_opsa_subsystem_cfg_t *subsystem)
{
    digi_serdes_port_def_t *port_def_ptr = (digi_serdes_port_def_t*) port_ctxt_ptr;
    BOOL8 prov_status;
    digi_serdes_port_mode_t port_mode;
    digi_serdes_prov_state_t prov_state;
    BOOL8 disable_fw_proc = FALSE;
    PMC_ENTRY();

     /* get serdes provioning state */
    (void) digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr, 
                                                          TRUE,
                                                          &prov_status, &port_mode, 
                                                          &prov_state);
    
    if (prov_state == DIGI_SERDES_PROV_STATE_LINEOTN ||
        prov_state == DIGI_SERDES_PROV_STATE_SYSOTN)
    {
        digi_otn_server_chnl_def_t* otn_server_def_ptr = digi_otn_server_chnl_port_uid_get(digi_handle, port_def_ptr->port_uid);

        PMC_ASSERT(otn_server_def_ptr != NULL,1,0,0);
       
        {
            BOOL ho_unchannelized = FALSE;
            *odu_ptr = otn_server_def_ptr->odu_struct_ptr;
            if (*odu_ptr != NULL)
            {
                /* we have an odu container attached to the OTU framer */
                /*Rate adaptation require for no MUXING
                  non-sysotn : FMF1->TFRM12 
                  sysotn     : FMF1->TFRM12, FMF2->TFRM12*/                
                ho_unchannelized = (((*odu_ptr)->mem_ptr->odu_level == ODU_STRUCT_LEVEL_HO_ODU &&
                                     (*odu_ptr)->mem_ptr->payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED) ||
                                    ((*odu_ptr)->mem_ptr->odu_level == ODU_STRUCT_LEVEL_LO_ODU &&
                                     DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle)));

                if (ho_unchannelized == TRUE)
                {
                    disable_fw_proc = TRUE;
                    if ( (*odu_ptr)->mem_ptr->odu_level == ODU_STRUCT_LEVEL_LO_ODU &&
                        DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))
                    {
                        *subsystem = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL;
                    } 
                    else 
                    {
                        *subsystem = OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1;
                    }
                }                                                
            }
        }
    }
    else if (prov_state == DIGI_SERDES_PROV_STATE_CBRC ||
             prov_state == DIGI_SERDES_PROV_STATE_ENET_LINE ||
             prov_state == DIGI_SERDES_PROV_STATE_ENET_SYS)
    {
        digi_handle_id_t chnl_hndl_type = LAST_DIGI_HANDLE_ID;
        digi_chnl_hndl_t *chnl_hndl_ptr = NULL;
        digi_chnl_hndl_t *dummy_hndl_ptr = NULL;

        PMC_ERROR result = digi_serdes_port_chnl_hndl_get(digi_handle,
                                                          port_ctxt_ptr,
                                                          &chnl_hndl_type,
                                                          &chnl_hndl_ptr,
                                                          &dummy_hndl_ptr);

        if (PMC_SUCCESS != result || NULL == chnl_hndl_ptr)
        {
            PMC_RETURN(FALSE);
        }
        if (DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T == chnl_hndl_type ||
            DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == chnl_hndl_type ||
            DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == chnl_hndl_type)
        {
            *switch_data_ptr = (util_global_switch_data_def_t *)chnl_hndl_ptr;

            if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED ==
                (*switch_data_ptr)->header.prov_state)
            {
                disable_fw_proc = TRUE;

                *subsystem = OPSA_HOSTMSG_CPB_SS_CBRC;
                
                if (DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T == chnl_hndl_type)
                {
                    *subsystem = OPSA_HOSTMSG_CPB_SS_ENET_LINE;
                }
                else if (DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T == chnl_hndl_type)
                {
                    *subsystem = OPSA_HOSTMSG_CPB_SS_ENET_SYS;
                }
            }
            else
            {
                *switch_data_ptr = NULL;
            }
        }
    }

    PMC_RETURN(disable_fw_proc);
} /* digi_disable_fw_acb_proc */

/*******************************************************************************
* digi_serdes_facility_loopback_workaround
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs a workaround for ENET 40/100G with facility
*   loopback.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_ctxt_ptr         - serdes port context pointer 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_serdes_facility_loopback_workaround(digi_handle_t       * digi_handle, 
                                                           digi_serdes_port_t  *port_ctxt_ptr) 
{
    BOOL8 prov_status;
    digi_serdes_port_mode_t port_mode;
    digi_serdes_prov_state_t prov_state;
    UINT16 pin_bit_mask = 0;
    UINT32 num_pins = 0;
    UINT32 i;
    digi_serdes_port_def_t *port_def_ptr;
    digi_post_mld_dsi_ctxt_t *post_mld_dsi_ptr = NULL;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();


    port_def_ptr =  (digi_serdes_port_def_t*) port_ctxt_ptr;

    (void) digi_serdes_port_rate_prov_status_internal_get(digi_handle, port_ctxt_ptr, 
                                                          TRUE,
                                                          &prov_status, &port_mode, 
                                                          &prov_state);
    (void) digi_port_pin_bit_mask_get(digi_handle,
                                      port_def_ptr, 
                                      &pin_bit_mask);


    for(i=0;i<16;i++)
    {
        if((pin_bit_mask & (1 << i)))
        {
            num_pins++;
        }
    }

    if(num_pins > 1)
    {
        if(prov_state == DIGI_SERDES_PROV_STATE_ENET_LINE)
        {
            post_mld_dsi_ptr = digi_first_post_mld_dsi_find(digi_handle, 
                                                            port_def_ptr->port_uid);

            enet_emac_pcs_reset(digi_handle->enet_line_handle,
                                post_mld_dsi_ptr->lane_index);
        }
        else if(prov_state == DIGI_SERDES_PROV_STATE_ENET_SYS)
        {
            post_mld_dsi_ptr = digi_first_post_mld_dsi_find(digi_handle, 
                                                            port_def_ptr->port_uid);

            enet_emac_pcs_reset(digi_handle->enet_sys_handle,
                                post_mld_dsi_ptr->lane_index);
        }
    }


    PMC_RETURN(rc);
} /* digi_serdes_facility_loopback_workaround */


#endif

/*
** End of file
*/
