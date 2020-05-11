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
*      MGMT_FEGE functions implementation.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "mgmt_fege_loc.h"
#include "mgmt_fege_io.h"
#include "enet_fege_io.h"
#include "l1rpp_fege_io.h"
#include "l1tpp_fege_io.h"
#include "rmac_io.h"
#include "tmac_io.h"

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
const char MGMT_FEGE_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    MGMT_FEGE_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char MGMT_FEGE_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

PRIVATE PMC_ERROR mgmt_fege_enet_fege_cfg(mgmt_fege_handle_t          *hndl,
                                          BOOL8                        enable);

PRIVATE PMC_ERROR mgmt_fege_energy_state_reg_set(mgmt_fege_handle_t  *hndl,
                                                 pmc_energy_state_t energy_state);

PRIVATE void mgmt_fege_ctxt_state_set(mgmt_fege_handle_t      *hndl,
                                      mgmt_fege_state_t   state);

PRIVATE PMC_ERROR mgmt_fege_ctxt_state_get(mgmt_fege_handle_t      *hndl,
                                           mgmt_fege_state_t  *state);

PRIVATE PMC_ERROR mgmt_fege_ctxt_state_transition_request(mgmt_fege_handle_t      *hndl,
                                                          mgmt_fege_state_t   next_state);

PRIVATE PMC_ERROR mgmt_fege_ctxt_audit(mgmt_fege_handle_t     *mgmt_fege_handle,
                                       mgmt_fege_var_t        *var_clean_ptr,
                                        BOOL8                  top_level_reset);

PRIVATE PMC_ERROR mgmt_fege_var_cleanup(mgmt_fege_handle_t      *mgmt_fege_handle,
                                        mgmt_fege_var_t         *var_clean_ptr,
                                        BOOL8                    top_level_reset);
/*
** Public Functions
*/

/*******************************************************************************
* mgmt_fege_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a MGMT_FEGE block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the MGMT_FEGE block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*    enet_fege_base_addr  - ENET FEGE base address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mgmt_fege_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC mgmt_fege_handle_t *mgmt_fege_ctxt_create(pmc_handle_t *parent, 
                                                 UINT32 base_address, 
                                                 pmc_sys_handle_t *sys_handle, 
                                                 const char *tsb_name,
                                                 UINT32 enet_fege_base_addr)
{
    mgmt_fege_handle_t *mgmt_fege_handle;

    PMC_ENTRY();
    mgmt_fege_handle = (mgmt_fege_handle_t*)PMC_CTXT_CALLOC(sizeof(mgmt_fege_handle_t), parent);
    
    pmc_handle_init( parent, mgmt_fege_handle, sys_handle, PMC_MID_DIGI_MGMT_FEGE, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(MGMT_FEGE_LOG_ERR_STRINGS[0], MGMT_FEGE_LOG_ERR_TABLE_BASE, MGMT_FEGE_LOG_ERR_COUNT);

    /* create ENET_FEGE handle */
    mgmt_fege_handle->enet_fege_handle = enet_fege_ctxt_create((pmc_handle_t *)mgmt_fege_handle, enet_fege_base_addr, sys_handle, "MGMT_FEGE_ENET_FEGE");
    PMC_CTXT_REGISTER_PTR(&mgmt_fege_handle->enet_fege_handle, parent);

    PMC_RETURN(mgmt_fege_handle);
} /* mgmt_fege_ctxt_create */  
 
/*******************************************************************************
* mgmt_fege_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a MGMT_FEGE block instance.
*
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
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
PUBLIC void mgmt_fege_ctxt_destroy(mgmt_fege_handle_t *mgmt_fege_handle)
{
    PMC_ENTRY();
    
    enet_fege_ctxt_destroy(mgmt_fege_handle->enet_fege_handle);
    PMC_CTXT_FREE(&mgmt_fege_handle, mgmt_fege_handle);

    PMC_RETURN();
} /* mgmt_fege_ctxt_destroy */

/*******************************************************************************
* mgmt_fege_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a MGMT_FEGE block handle instance.
*
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
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
PUBLIC void mgmt_fege_handle_init(mgmt_fege_handle_t *mgmt_fege_handle)
{
    
    UINT32 i;

    PMC_ENTRY();

    enet_fege_handle_init(mgmt_fege_handle->enet_fege_handle);


    for (i = 0; i < MGMT_FEGE_VLAN_REPL_TABLE_SIZE; i++)
    {
        mgmt_fege_handle->var.ing_fwd_tbl[i].valid = FALSE;
        mgmt_fege_handle->var.egr_fwd_tbl[i].valid = FALSE;
        mgmt_fege_handle->var.ing_fwd_tbl[i].num_regs = 0;
        mgmt_fege_handle->var.egr_fwd_tbl[i].num_regs = 0;
    }

    PMC_RETURN();
    
} /* mgmt_fege_handle_init */

/*******************************************************************************
* mgmt_fege_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on mgmt_fege.
*
*
* INPUTS:
*   *mgmt_fege_handle     - pointer to MGMT_FEGE handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_handle_restart_init(mgmt_fege_handle_t *mgmt_fege_handle,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                               pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(MGMT_FEGE_LOG_ERR_STRINGS[0], MGMT_FEGE_LOG_ERR_TABLE_BASE, MGMT_FEGE_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* mgmt_fege_handle_restart_init */

/*******************************************************************************
* mgmt_fege_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a MGMT_FEGE block instance.
*
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of MGMT_FEGE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 mgmt_fege_base_address_get(mgmt_fege_handle_t *mgmt_fege_handle)
{
    PMC_ENTRY();

    PMC_RETURN(mgmt_fege_handle->base.base_address);
} /* mgmt_fege_base_address_get */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/

/*******************************************************************************
* Run-time Operations
*******************************************************************************/

/*******************************************************************************
* mgmt_fege_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a MGMT_FEGE block instance.
*
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
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
PUBLIC PMC_ERROR mgmt_fege_init(mgmt_fege_handle_t *mgmt_fege_handle)
{
    PMC_ERROR rc;
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    rc = mgmt_fege_ctxt_state_transition_request(mgmt_fege_handle, MGMT_FEGE_STATE_OPERATIONAL);

    if (PMC_SUCCESS == rc)
    {
        /* power up the port */
        rc = mgmt_fege_energy_state_reg_set(mgmt_fege_handle, PMC_ENERGY_STATE_RUNNING);
    }

    if (PMC_SUCCESS == rc)
    {
        mgmt_fege_ctxt_state_set(mgmt_fege_handle, MGMT_FEGE_STATE_OPERATIONAL);
        mgmt_fege_handle->var.ctxt.flow_ctrl_en = FALSE;
    }


    if (PMC_SUCCESS == rc)
    {
        mgmt_fege_field_TDATA_BIT_SWAP_set(NULL, mgmt_fege_handle, 1);
        
        rc = mgmt_fege_enet_fege_cfg(mgmt_fege_handle, TRUE);
    }
    
    for (itr = 0; itr < MGMT_FEGE_VLAN_REPL_TABLE_SIZE; itr++)
    {
        mgmt_fege_field_TXLUT_VALID_set(NULL, mgmt_fege_handle, itr, 0);
        mgmt_fege_field_RXLUT_VALID_set(NULL, mgmt_fege_handle, itr, 0);
    }

    PMC_RETURN(rc);
    
} /* mgmt_fege_init */

/*******************************************************************************
* mgmt_fege_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Uninitializes a MGMT_FEGE block instance.
*
*
* INPUTS:
*   *mgmt_fege_handle          - pointer to MGMT_FEGE handle instance
*    skip_active_filters_check - when TRUE, function will return a error code 
*    if filters are active, otherwise uninitialization will proceed.
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
PUBLIC PMC_ERROR mgmt_fege_uninit(mgmt_fege_handle_t *mgmt_fege_handle,
                                  BOOL8               skip_active_filters_check)
{
    PMC_ERROR rc;
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    rc = mgmt_fege_ctxt_state_transition_request(mgmt_fege_handle, MGMT_FEGE_STATE_START);

    if (FALSE == skip_active_filters_check)
    {
        for (itr = 0; itr < MGMT_FEGE_VLAN_REPL_TABLE_SIZE && PMC_SUCCESS == rc; itr++)
        {
            if (0 != mgmt_fege_handle->var.ing_fwd_tbl[itr].num_regs)
            {
                rc = MGMT_FEGE_CHNL_STILL_CFG;
            }
            
            if (0 != mgmt_fege_handle->var.egr_fwd_tbl[itr].num_regs)
            {
                rc = MGMT_FEGE_CHNL_STILL_CFG;
            }
        }
    }
    else
    {
        PMC_MEMSET((void *)&mgmt_fege_handle->var, 0x0, sizeof(mgmt_fege_var_t));
    }

    if (PMC_SUCCESS == rc)
    {
        rc = mgmt_fege_enet_fege_cfg(mgmt_fege_handle, FALSE);
    }    

    if (PMC_SUCCESS == rc)
    {
        /* power up the port */
        rc = mgmt_fege_energy_state_reg_set(mgmt_fege_handle, PMC_ENERGY_STATE_RESET);
        mgmt_fege_ctxt_state_set(mgmt_fege_handle, MGMT_FEGE_STATE_START);
    }

    PMC_RETURN(rc);
    
} /* mgmt_fege_uninit */


/*******************************************************************************
* mgmt_fege_ingr_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function adds or retrieves a forwarding path in MGMT_FEGE INGRESS 
*   datatpath. Packets are forwarded from MCPB to in from MGMT_FEGE.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
*    enable                  - when TRUE a path is added in the forwarding table
*                              when FALSE, a path is retrieved from the 
*                              forwarding table.
*   *fwd_cfg_ptr             - pointer to fowarding configuration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_ingr_pkt_fwd_cfg(mgmt_fege_handle_t          *mgmt_fege_handle,
                                            BOOL8                        enable,
                                            mgmt_fege_fwd_cfg_t         *fwd_cfg_ptr)
{
    INT32 free_entry_idx = -1;
    mgmt_fege_fwd_tbl_entry_t  *fwd_tbl;
    mgmt_fege_buffer_t buf[1];
    PMC_ERROR    rc = PMC_SUCCESS;
    BOOL8 proceed = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(enable == FALSE || NULL != fwd_cfg_ptr, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > fwd_cfg_ptr->dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* select forwarding table */
    fwd_tbl = &mgmt_fege_handle->var.egr_fwd_tbl[0];

    if (TRUE == enable)
    {
        /* check that retrieve lookup table entry index associated to request */
        if (FALSE == fwd_tbl[fwd_cfg_ptr->dpi_id].valid)
        {
            free_entry_idx = fwd_cfg_ptr->dpi_id;
        }
        else
        {
            rc = MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV;
            fwd_tbl[fwd_cfg_ptr->dpi_id].num_regs += 1;
        }        
    }
        
    if (PMC_SUCCESS == rc && 
        FALSE == enable)
    {            
        /* retrieve lookup table entry index associated to request */
        
        if (TRUE != fwd_tbl[fwd_cfg_ptr->dpi_id].valid ||
            (TRUE == ((TRUE == fwd_cfg_ptr->ins_vlan_id) && 
                      (fwd_cfg_ptr->vlan_entry.vlan_pcp == fwd_tbl[fwd_cfg_ptr->dpi_id].fwd_entry.vlan_entry.vlan_pcp) &&
                      (fwd_cfg_ptr->vlan_entry.vlan_cfi == fwd_tbl[fwd_cfg_ptr->dpi_id].fwd_entry.vlan_entry.vlan_cfi) &&
                      (fwd_cfg_ptr->vlan_entry.vlan_id == fwd_tbl[fwd_cfg_ptr->dpi_id].fwd_entry.vlan_entry.vlan_id))))
        {
            rc = MGMT_FEGE_LOG_ERR_VLAN_NOT_FOUND;
        }
        else
        {
            free_entry_idx = fwd_cfg_ptr->dpi_id;                
        }
    }

    /* update number of entries in forwarding table */
    if (PMC_SUCCESS == rc)
    {        
        if (TRUE == enable)
        {
            fwd_tbl[free_entry_idx].valid = TRUE;                
            fwd_tbl[free_entry_idx].num_regs += 1;
            fwd_tbl[free_entry_idx].fwd_entry.dpi_id = fwd_cfg_ptr->dpi_id;
            fwd_tbl[free_entry_idx].fwd_entry.ins_vlan_id = fwd_cfg_ptr->ins_vlan_id;
            PMC_MEMCPY(&fwd_tbl[free_entry_idx].fwd_entry.vlan_entry, &fwd_cfg_ptr->vlan_entry, sizeof(util_global_vlan_cfg_t));
            proceed = TRUE;
        }
        
        if (FALSE == enable)
        {                
            fwd_tbl[free_entry_idx].num_regs -= 1;
            if (0 == fwd_tbl[free_entry_idx].num_regs)
            {
                fwd_tbl[free_entry_idx].valid = FALSE;
                proceed = TRUE;
            }
            else
            {
                rc = MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV;
            }
        }
    }
    
    
    if (PMC_SUCCESS == rc && TRUE == proceed)
    {    
        mgmt_fege_buffer_init(buf, mgmt_fege_handle);
        
        mgmt_fege_field_TXLUT_VALID_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, enable);             
        if (TRUE == enable)
        {
            mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, fwd_cfg_ptr->ins_vlan_id);  
            mgmt_fege_field_TXLUT_VLAN_ID_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, fwd_cfg_ptr->vlan_entry.vlan_id);
        }        
        else
        {
            mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, 0); 
        }    

        mgmt_fege_buffer_flush(buf);   
    }

    PMC_RETURN(rc);

} /* mgmt_fege_ingr_pkt_fwd_cfg */


/*******************************************************************************
* mgmt_fege_ingr_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the configuration context associated with ingress
*   packet forwarding configuration setup.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
*   dpi_id                   - DPI channel ID
*
* OUTPUTS:
*   *enable_ptr              - when TRUE a path is configured in the forwarding 
*                              table (fwd_cfg_ptr data is valid)
*                              when FALSE, a path is retrieved from the 
*                              forwarding table (fwd_cfg_ptr data is invalid). 
*   *fwd_cfg_ptr             - pointer to fowarding configuration.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mgmt_fege_ingr_pkt_fwd_cfg_get(mgmt_fege_handle_t *mgmt_fege_handle, 
                                           UINT16 dpi_id,
                                           BOOL8 *enable_ptr, 
                                           mgmt_fege_fwd_cfg_t *fwd_cfg_ptr)
{
    mgmt_fege_fwd_tbl_entry_t  *fwd_tbl;

    PMC_ENTRY();

    fwd_tbl = &mgmt_fege_handle->var.egr_fwd_tbl[0];
    
    *enable_ptr = fwd_tbl[dpi_id].valid;

    fwd_cfg_ptr->dpi_id= fwd_tbl[dpi_id].fwd_entry.dpi_id;
    fwd_cfg_ptr->ins_vlan_id = fwd_tbl[dpi_id].fwd_entry.ins_vlan_id;
    PMC_MEMCPY(&fwd_cfg_ptr->vlan_entry, &fwd_tbl[dpi_id].fwd_entry.vlan_entry, sizeof(util_global_vlan_cfg_t));
 
    PMC_RETURN();

} /* mgmt_fege_ingr_pkt_fwd_cfg_get */

/*******************************************************************************
* mgmt_fege_egr_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function adds or retrieves a forwarding path in MGMT_FEGE DIGI EGRESS 
*   datapath. Packets are filtered in from MGMT_FEGE to MCPB.
*
* INPUTS:
*   *mgmt_fege_handle  - pointer to MGMT_FEGE handle instance
*    enable            - when TRUE a path is added in the forwarding table
*                        when FALSE, a path is retrived from the forwarding
*                        table.
*   *fwd_cfg_ptr       - pointer to fowarding configuration.
*   fcs_calc_en        - enable FCS calculation in ENET SS.
*   priority_en        - enable priority (PCP) forwarding.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_egr_pkt_fwd_cfg(mgmt_fege_handle_t          *mgmt_fege_handle,
                                           BOOL8                        enable,
                                           mgmt_fege_fwd_cfg_t         *fwd_cfg_ptr,
                                           BOOL8                        fcs_calc_en,
                                           BOOL8                        priority_en)
{
    INT32 free_entry_idx = -1;
    mgmt_fege_fwd_tbl_entry_t  *fwd_tbl;
    mgmt_fege_buffer_t buf[1];
    PMC_ERROR    rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(enable == FALSE || NULL != fwd_cfg_ptr, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > fwd_cfg_ptr->dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    fwd_tbl = &mgmt_fege_handle->var.ing_fwd_tbl[0];
    
    if (TRUE == enable)
    {
        /* check that retrieve lookup table entry index associated to request */
        if (FALSE == fwd_tbl[fwd_cfg_ptr->dpi_id].valid)
        {
            free_entry_idx = fwd_cfg_ptr->dpi_id;
        }
        else
        {
            rc = MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV;
        }        
    }
    
    if (PMC_SUCCESS == rc && 
        FALSE == enable)
    {
        /* retrieve lookup table entry index associated to request */
        if (TRUE == fwd_tbl[fwd_cfg_ptr->dpi_id].valid)
        {
            free_entry_idx = fwd_cfg_ptr->dpi_id;                
        }
        else
        {
            rc = MGMT_FEGE_LOG_ERR_DPI_CHNL_NOT_RPOV;
        }        
    }
    

    /* update number of entries in forwarding table */
    if (PMC_SUCCESS == rc)
    {        
        mgmt_fege_buffer_init(buf, mgmt_fege_handle);

        mgmt_fege_field_RXLUT_VALID_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, (UINT32) enable);
        if (TRUE == enable)
        {
            mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, priority_en);
            mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, mgmt_fege_handle->var.ctxt.flow_ctrl_en);
            mgmt_fege_field_RXLUT_FCS_ENABLE_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, fcs_calc_en);
            mgmt_fege_field_RXLUT_VLAN_ID_set(buf, mgmt_fege_handle, (UINT32)free_entry_idx, fwd_cfg_ptr->vlan_entry.vlan_id);
        }
        
        mgmt_fege_buffer_flush(buf);   

        if (PMC_SUCCESS == rc)
        {                 
            if (TRUE == enable)
            {
                fwd_tbl[free_entry_idx].valid = TRUE;                
                PMC_MEMCPY(&fwd_tbl[free_entry_idx].fwd_entry.vlan_entry, 
                           &fwd_cfg_ptr->vlan_entry, 
                           sizeof(util_global_vlan_cfg_t));
            }
            
            if (FALSE == enable)
            {
                fwd_tbl[free_entry_idx].valid = FALSE;
            }
        }
    }

    PMC_RETURN(rc);

} /* mgmt_fege_egr_pkt_fwd_cfg */

/*******************************************************************************
* mgmt_fege_egr_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves a forwarding path in MGMT_FEGE DIGI EGRESS 
*   datapath configuration.
*
* INPUTS:
*   *mgmt_fege_handle  - pointer to MGMT_FEGE handle instance
*    dpi_id            - DPI Channel ID
*
* OUTPUTS:
*   *enable_ptr            - when TRUE a path is added in the forwarding table
*                        when FALSE, a path is retrived from the forwarding
*                        table.
*   *fwd_cfg_ptr       - pointer to fowarding configuration.
*   *fcs_calc_en_ptr   - enable FCS calculation in ENET SS.
*   *priority_en_ptr   - enable priority (PCP) forwarding.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mgmt_fege_egr_pkt_fwd_cfg_get(mgmt_fege_handle_t         *mgmt_fege_handle,
                                          UINT16                      dpi_id,
                                          BOOL8                      *enable_ptr,
                                          mgmt_fege_fwd_cfg_t        *fwd_cfg_ptr,
                                          BOOL8                      *fcs_calc_en_ptr,
                                          BOOL8                      *priority_en_ptr)
{
    mgmt_fege_fwd_tbl_entry_t  *fwd_tbl;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    fwd_tbl = &mgmt_fege_handle->var.ing_fwd_tbl[0];

    *enable_ptr = fwd_tbl[dpi_id].valid;

    PMC_MEMCPY(&fwd_cfg_ptr->vlan_entry, 
               &fwd_tbl[dpi_id].fwd_entry.vlan_entry,
               sizeof(util_global_vlan_cfg_t));

    *fcs_calc_en_ptr = mgmt_fege_field_RXLUT_FCS_ENABLE_get(NULL, mgmt_fege_handle, (UINT32)dpi_id);

    *priority_en_ptr = mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get(NULL, mgmt_fege_handle, (UINT32)dpi_id);

    PMC_RETURN();

} /* mgmt_fege_egr_pkt_fwd_cfg_get */

/*******************************************************************************
* mgmt_fege_egr_pkt_fwd_vlan_prio_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves a forwarding path in MGMT_FEGE DIGI EGRESS 
*   datapath. Packets are filtered in from MGMT_FEGE to MCPB.
*
* INPUTS:
*   *mgmt_fege_handle  - pointer to MGMT_FEGE handle instance
*    dpi_id            - DPI Channel ID
*
* OUTPUTS:
*   *enable            - when TRUE a path is added in the forwarding table
*                        when FALSE, a path is retrived from the forwarding
*                        table.
*   *vlan_cfg_ptr      - pointer to VLAN packet configuration.
*   *priority_en_ptr   - enable priority (PCP) forwarding.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_egr_pkt_fwd_vlan_prio_cfg_get(mgmt_fege_handle_t     *mgmt_fege_handle,
                                                         UINT16                 dpi_id,
                                                         BOOL8                  *enable,
                                                         util_global_vlan_cfg_t *vlan_cfg_ptr,
                                                         BOOL8                  *priority_en_ptr)
{
    mgmt_fege_fwd_tbl_entry_t  *fwd_tbl;
    PMC_ERROR    rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    fwd_tbl = &mgmt_fege_handle->var.ing_fwd_tbl[0];

    *enable = fwd_tbl[dpi_id].valid;

    PMC_MEMCPY(vlan_cfg_ptr, 
               &fwd_tbl[dpi_id].fwd_entry.vlan_entry,
               sizeof(util_global_vlan_cfg_t));
    
    *priority_en_ptr = mgmt_fege_field_RXLUT_PRIORITY_ENABLE_get(NULL, mgmt_fege_handle, (UINT32)dpi_id);

    PMC_RETURN(rc);

} /* mgmt_fege_egr_pkt_fwd_vlan_prio_cfg_get */

/*******************************************************************************
* mgmt_fege_pkt_all_dpi_deprov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE when all mgmt_fege ingress & egress DPI channels are 
*   unprovisioned.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when all mgmt_fege ingress & egress DPI channels are unprovisioned, 
*   otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mgmt_fege_pkt_all_dpi_deprov_get(mgmt_fege_handle_t          *mgmt_fege_handle)
{
    BOOL8    rc = TRUE;
    UINT32 i;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    for (i = 0; i < MGMT_FEGE_VLAN_REPL_TABLE_SIZE && TRUE == rc; i++)
    {
        /* check that there is no entry valid in ingress forwarding table  */
        if (TRUE == mgmt_fege_handle->var.ing_fwd_tbl[i].valid)
        {
            rc = FALSE;            
        }
        /* check that there is no entry valid in egress forwarding table  */
        if (TRUE == rc && TRUE == mgmt_fege_handle->var.egr_fwd_tbl[i].valid)
        {
            rc = FALSE;
        }
    }

    PMC_RETURN(rc);
} /* mgmt_fege_pkt_all_dpi_deprov_get */

/*******************************************************************************
* mgmt_fege_rx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MGMT_FEGE RX flow control. When Control flow is enabled, MGMT_FEGE 
*   stops frames transmission when a PAUSE frame is received and it resumes the
*   transmission when pause quanta counter expired.
*
* INPUTS:
*   *mgmt_fege_handle  - pointer to MGMT_FEGE handle instance.
*    enable            - when TRUE, flow control is activated
*                        otherwise it is deactivated
*   drop_pause_frm     - when TRUE, RX pause frames are terminated in MGMT_FEGE
*                        when FALSE, RX pause frames are forwarded.  
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None..
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mgmt_fege_rx_flow_control_set(mgmt_fege_handle_t     *mgmt_fege_handle,
                                          BOOL8                   enable,
                                          BOOL8                   drop_pause_frm)
{
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != mgmt_fege_handle->enet_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

        
    rmac_field_INPAUSE_EN_set(NULL, mgmt_fege_handle->enet_fege_handle, (UINT8)enable);
    rmac_field_PAUSEFRM_DROP_set(NULL, mgmt_fege_handle->enet_fege_handle, (UINT8)drop_pause_frm);
    tmac_field_RMAC_PAUSE_EN_set(NULL, mgmt_fege_handle->enet_fege_handle, (UINT8)enable);

    PMC_RETURN();
} /* mgmt_fege_rx_flow_control_set */

/*******************************************************************************
* mgmt_fege_rx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return MGMT_FEGE RX flow control status: enabled or disabled
*
* INPUTS:
*   *mgmt_fege_handle  - pointer to MGMT_FEGE handle instance.
*
* OUTPUTS:
*   *enable            - flow control status. When TRUE, flow control is 
*                        activated otherwise it is deactivated
*   *drop_pause_frm    - pause frame forwarding status.  When TRUE, RX pause 
*                        frames are terminated in MGMT_FEGE. When FALSE, RX 
*                        pause frames are forwarded.
*
* RETURNS:
*   None.
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mgmt_fege_rx_flow_control_get(mgmt_fege_handle_t       *mgmt_fege_handle,
                                          BOOL8                    *enable,
                                          BOOL8                    *drop_pause_frm)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != mgmt_fege_handle->enet_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

        
    *enable = rmac_field_INPAUSE_EN_get(NULL, mgmt_fege_handle->enet_fege_handle);
    *drop_pause_frm = rmac_field_PAUSEFRM_DROP_get(NULL, mgmt_fege_handle->enet_fege_handle);

    PMC_RETURN();
} /* mgmt_fege_rx_flow_control_get */


/*******************************************************************************
* mgmt_fege_frame_sizes_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MGMT_FEGE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance.
*    txMaxFrameSize          - TX frames maximum size
*    txMinFrameSize          - TX frames minimum size
*    rxMaxFrameSize          - RX frames maximum size
*    rxMinFrameSize          - RX frames minimum size
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_frame_sizes_set(mgmt_fege_handle_t          *mgmt_fege_handle,
                                           UINT16                       txMaxFrameSize,
                                           UINT16                       txMinFrameSize,
                                           UINT16                       rxMaxFrameSize,
                                           UINT16                       rxMinFrameSize)
{
    PMC_ERROR    rc = PMC_SUCCESS;
    enet_fege_handle_t *enet_fege_hndl_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != mgmt_fege_handle->enet_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    enet_fege_hndl_ptr = mgmt_fege_handle->enet_fege_handle;
    
    if (rxMaxFrameSize > ENET_FEGE_RXMAXFRMSIZE_UPPERLT ||
        txMinFrameSize + 4 > txMaxFrameSize ||
        txMaxFrameSize > ENET_FEGE_TXMAXFRMSIZE_UPPERLT - 1) 
    {
        rc = MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS;
    }
    
    rmac_field_MAX_FRM_LEN_set(NULL, enet_fege_hndl_ptr, rxMaxFrameSize);
    rmac_field_MIN_FRM_LEN_set(NULL, enet_fege_hndl_ptr, rxMinFrameSize);
    tmac_field_MAX_FRM_LEN_set(NULL, enet_fege_hndl_ptr, txMaxFrameSize);
    tmac_field_MIN_FRM_LEN_set(NULL, enet_fege_hndl_ptr, txMinFrameSize);

    PMC_RETURN(rc);
} /* mgmt_fege_frame_sizes_set */


/*******************************************************************************
* mgmt_fege_pkt_frame_sizes_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MGMT_FEGE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance.
*
* OUTPUTS:
*   *txMaxFrameSize          - TX frames maximum size
*   *txMinFrameSize          - TX frames minimum size
*   *rxMaxFrameSize          - RX frames maximum size
*   *rxMinFrameSize          - RX frames minimum size
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mgmt_fege_frame_sizes_get(mgmt_fege_handle_t          *mgmt_fege_handle,
                                      UINT16                      *txMaxFrameSize,
                                      UINT16                      *txMinFrameSize,
                                      UINT16                      *rxMaxFrameSize,
                                      UINT16                      *rxMinFrameSize)
{
    enet_fege_handle_t *enet_fege_hndl_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != mgmt_fege_handle->enet_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != txMaxFrameSize, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != txMinFrameSize, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rxMaxFrameSize, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rxMinFrameSize, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    enet_fege_hndl_ptr = mgmt_fege_handle->enet_fege_handle;
    
    
    *rxMaxFrameSize = rmac_field_MAX_FRM_LEN_get(NULL, enet_fege_hndl_ptr);
    *rxMinFrameSize = rmac_field_MIN_FRM_LEN_get(NULL, enet_fege_hndl_ptr);
    *txMaxFrameSize = tmac_field_MAX_FRM_LEN_get(NULL, enet_fege_hndl_ptr);
    *txMinFrameSize = tmac_field_MIN_FRM_LEN_get(NULL, enet_fege_hndl_ptr);

    PMC_RETURN();
} /* mgmt_fege_frame_sizes_get */

/*******************************************************************************
* FUNCTION: mgmt_fege_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in 
*   mgmt_fege interrupt table.
*   
*
* INPUTS:
*   mgmt_fege_handle  - Pointer to subsystem handle
*   int_table_ptr     - Pointer to mgmt_fege interrupt table with
*                       bits set to enable interrupts. If NULL, 
*                       enables/disables all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_int_enable(mgmt_fege_handle_t    *mgmt_fege_handle,
                                      mgmt_fege_int_t       *int_table_ptr,
                                      mgmt_fege_int_t       *int_en_table_ptr,
                                      BOOL                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* enable enet fege interrupts */
    enet_fege_int_enable(mgmt_fege_handle->enet_fege_handle, &int_table_ptr->enet_fege, &int_en_table_ptr->enet_fege, enable);

    #ifdef MGMT_FEGE_INT
    #undef MGMT_FEGE_INT
    #endif
    #define MGMT_FEGE_INT     UTIL_GLOBAL_INT_ENABLE_SINGLE

    MGMT_FEGE_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* mgmt_fege_int_enable */


/*******************************************************************************
* FUNCTION: mgmt_fege_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the mgmt_fege interrupt table.
*   
*
* INPUTS:
*   mgmt_fege_handle  - Pointer to subsystem handle
*   filt_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - Pointer to mgmt_fege interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_int_retrieve(mgmt_fege_handle_t    *mgmt_fege_handle,
                                        mgmt_fege_int_t       *filt_table_ptr,
                                        mgmt_fege_int_t       *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* retrieve enet fege interrupts */
    enet_fege_int_retrieve(mgmt_fege_handle->enet_fege_handle, &filt_table_ptr->enet_fege, &int_table_ptr->enet_fege);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mgmt_fege_handle, mgmt_fege, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef MGMT_FEGE_INT
    #undef MGMT_FEGE_INT
    #endif
    #define MGMT_FEGE_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    MGMT_FEGE_INT_TABLE_DEFINE();

     UTIL_GLOBAL_INT_BUFFER_FLUSH(mgmt_fege_handle, mgmt_fege);

    PMC_RETURN(result);
} /* mgmt_fege_int_retrieve */

/*******************************************************************************
* FUNCTION: mgmt_fege_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in 
*   mgmt_fege interrupt table.
*   
*
* INPUTS:
*   mgmt_fege_handle  - Pointer to subsystem handle
*   int_table_ptr     - pointer to mgmt_fege interrupt table with
*                       bits set to clear interrupts
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_int_clear(mgmt_fege_handle_t    *mgmt_fege_handle,
                                     mgmt_fege_int_t       *int_table_ptr,
                                     mgmt_fege_int_t       *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* clear enet fege interrupts */
    enet_fege_int_clear(mgmt_fege_handle->enet_fege_handle, 
                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, enet_fege),
                        &int_en_table_ptr->enet_fege);

    #ifdef MGMT_FEGE_INT
    #undef MGMT_FEGE_INT
    #endif
    #define MGMT_FEGE_INT    UTIL_GLOBAL_INT_CLEAR_SINGLE

    MGMT_FEGE_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* mgmt_fege_int_clear */


/*******************************************************************************
* FUNCTION: mgmt_fege_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches MGMT_FEGE active interrupts.
*
* INPUTS:
*   mgmt_fege_handle  - Pointer to subsystem handle
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_int_enabled_check(mgmt_fege_handle_t    *mgmt_fege_handle,
                                             mgmt_fege_int_t       *int_en_table_ptr,
                                             BOOL                  *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != int_found_ptr, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* clear enet fege interrupts */
    result = enet_fege_int_enabled_check(mgmt_fege_handle->enet_fege_handle, 
                                         &int_en_table_ptr->enet_fege,
                                         int_found_ptr);
    
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        #ifdef MGMT_FEGE_INT
        #undef MGMT_FEGE_INT
        #endif
        #define MGMT_FEGE_INT    UTIL_GLOBAL_INT_CHECK_SINGLE

        MGMT_FEGE_INT_TABLE_DEFINE();
    }

    PMC_RETURN(result);
} /* mgmt_fege_int_enabled_check */

/*
** Private Functions
*/
   
/*******************************************************************************
* mgmt_fege_enet_fege_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function configures enet_fege block instance.
*
* INPUTS:
*   *hndl            - pointer to MGMT_FEGE handle instance
*    enable          - when TRUE a path is added in the forwarding table
*                      when FALSE, a path is retrived from the forwarding
*                      table.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_enet_fege_cfg(mgmt_fege_handle_t          *hndl,
                                          BOOL8                        enable)
{
    PMC_ERROR    rc = PMC_SUCCESS;
    enet_fege_handle_t *enet_fege_hndl_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != hndl->enet_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    
    enet_fege_hndl_ptr = hndl->enet_fege_handle;

    if (TRUE == enable)
    {        
        enet_fege_field_TSTMP_CORR_UPD_BYPASS_set(NULL, enet_fege_hndl_ptr, 1);
        enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set(NULL, enet_fege_hndl_ptr, 0x0);
        enet_fege_field_MAC_LOWPWR_set(NULL, enet_fege_hndl_ptr, 0);
        enet_fege_field_GE_GFPT_LOWPWR_set(NULL, enet_fege_hndl_ptr, 1);
        enet_fege_field_FEGE_MODE_set(NULL, enet_fege_hndl_ptr, 0x3);

        enet_fege_field_ENET_FEGE_RST_set(NULL, enet_fege_hndl_ptr,      0);
        enet_fege_field_TX_ECLASS_RST_set(NULL, enet_fege_hndl_ptr,      0);
        enet_fege_field_TMAC_RST_set(NULL, enet_fege_hndl_ptr,          0);
        enet_fege_field_L1TPP_FEGE_RST_set(NULL, enet_fege_hndl_ptr,     0);
        enet_fege_field_RX_ECLASS_RST_set(NULL, enet_fege_hndl_ptr, 0);
        enet_fege_field_RMAC_RST_set(NULL, enet_fege_hndl_ptr,           0);
        enet_fege_field_L1RPP_FEGE_RST_set(NULL, enet_fege_hndl_ptr,    0);
        enet_fege_field_MSTATX_RST_set(NULL, enet_fege_hndl_ptr,         0);
        enet_fege_field_GE_GFPT_RST_set(NULL, enet_fege_hndl_ptr,       0);
        enet_fege_field_PTP_NTP_RST_set(NULL, enet_fege_hndl_ptr,        0);
        enet_fege_field_TX_TSTMP_FIFO_RST_set(NULL, enet_fege_hndl_ptr,  0);

        /* L1RPP specific register configuration */

        enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set(NULL, enet_fege_hndl_ptr, 1);
        l1rpp_fege_field_AUTO_SENSE_set(NULL, enet_fege_hndl_ptr, 1);
        l1rpp_fege_field_AN_ABLTY_ADV_set(NULL, enet_fege_hndl_ptr, ENET_FEGE_L1RPP_AN_ABLTY_ADV_DFLT);
        l1rpp_fege_field_AN_ABLTY_ADV_UPD_set(NULL, enet_fege_hndl_ptr, 1); 
        l1rpp_fege_field_MODE_set(NULL, enet_fege_hndl_ptr, 1);
        l1rpp_fege_field_LS_RESET_set(NULL, enet_fege_hndl_ptr, 0);        
        l1rpp_fege_field_RESYNC_COUNT_set(NULL, enet_fege_hndl_ptr, 0x4);
        l1rpp_fege_field_LOS_COUNT_set(NULL, enet_fege_hndl_ptr, 0x4);
        l1rpp_fege_field_SYNC_COUNT_set(NULL, enet_fege_hndl_ptr, 0x3);

        l1rpp_fege_field_PTRN_SEL_set(NULL, enet_fege_hndl_ptr, 0x0);
        l1rpp_fege_field_CUS_PTRN_set(NULL, enet_fege_hndl_ptr, 0x0);

        l1rpp_fege_field_AN_ENABLE_set(NULL, enet_fege_hndl_ptr, 1);  
        l1rpp_fege_field_AN_RESTART_set(NULL, enet_fege_hndl_ptr, 1); 

        l1rpp_fege_field_LOC_NP_NEXT_PAGE_set(NULL, enet_fege_hndl_ptr, 0);
        l1rpp_fege_field_LINK_TIMER_set(NULL, enet_fege_hndl_ptr, 0x1313);
        l1rpp_fege_field_LINK_TIME_DUR_set(NULL, enet_fege_hndl_ptr, 0x0000);

        l1rpp_fege_field_MODE_SEL_set(NULL, enet_fege_hndl_ptr, 0);
        l1rpp_fege_field_EN_CDET_SEL_set(NULL, enet_fege_hndl_ptr, 0);

        l1rpp_fege_field_L1RPP_EN_set(NULL, enet_fege_hndl_ptr, 1);

        /* RMAC specific register configuration */   

        rmac_field_PAMB_CK_set(NULL, enet_fege_hndl_ptr,       1);
        rmac_field_TAG_AWARE_set(NULL, enet_fege_hndl_ptr,      1);
        rmac_field_INRANGE_DIS_set(NULL, enet_fege_hndl_ptr,    0);
        rmac_field_INRANGE_CK_set(NULL, enet_fege_hndl_ptr,   0x0);
        rmac_field_MAX_FRM_TRN_set(NULL, enet_fege_hndl_ptr,    0);
        rmac_field_MAX_FRM_DIS_set(NULL, enet_fege_hndl_ptr,    0);
        rmac_field_MAX_FRM_CK_set(NULL, enet_fege_hndl_ptr,    0);
        rmac_field_MIN_FRM_DIS_set(NULL, enet_fege_hndl_ptr,    1);
        rmac_field_MIN_FRM_CK_set(NULL, enet_fege_hndl_ptr,     1);
        rmac_field_CRC_DIS_set(NULL, enet_fege_hndl_ptr,      1);
        rmac_field_CRC_CK_set(NULL, enet_fege_hndl_ptr,       1);
        rmac_field_TRANSPARENT_set(NULL, enet_fege_hndl_ptr,    0);
        rmac_field_ETHER_MODE_set(NULL, enet_fege_hndl_ptr,     1);

        rmac_field_INTERPKT_L1_EN_set(NULL, enet_fege_hndl_ptr, 0);
        rmac_field_BLIND_set(NULL, enet_fege_hndl_ptr,          0x0);
        rmac_field_MIN_FRM_LEN_set(NULL, enet_fege_hndl_ptr,    0x0);
        rmac_field_PAD_STRIP_EN_set(NULL, enet_fege_hndl_ptr,   0);
        rmac_field_CRC_STRIP_EN_set(NULL, enet_fege_hndl_ptr,   1);
        rmac_field_CARRIER_EN_set(NULL, enet_fege_hndl_ptr,    1);
        rmac_field_INPAUSE_EN_set(NULL, enet_fege_hndl_ptr,     1);
        rmac_field_PAUSEFRM_DROP_set(NULL, enet_fege_hndl_ptr,  0);
        rmac_field_CTRLFRM_DROP_set(NULL, enet_fege_hndl_ptr,   0);
        rmac_field_DA_DROP_set(NULL, enet_fege_hndl_ptr,        0);

        rmac_field_MAX_FRM_LEN_set(NULL, enet_fege_hndl_ptr,   0x7E0); /*= 16'b0000_0111_1110_0000 */
        rmac_reg_STATION_ADR_HIGH_WORD_CFG_write(NULL, enet_fege_hndl_ptr,   0x0);
        rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write(NULL, enet_fege_hndl_ptr, 0x0);
        rmac_reg_STATION_ADR_LOW_WORD_CFG_write(NULL, enet_fege_hndl_ptr, 0x0);

        rmac_field_CTRLFRM_STAT_EN_set(NULL, enet_fege_hndl_ptr,   0);
        rmac_field_PAUSEFRM_STAT_EN_set(NULL, enet_fege_hndl_ptr, 0);
        rmac_field_GPO_set(NULL, enet_fege_hndl_ptr, 0);        

        /* TMAC specific register configuration */

        tmac_field_CRC_CK_set(NULL, enet_fege_hndl_ptr, 0);
        tmac_field_PAD_ADD_EN_set(NULL, enet_fege_hndl_ptr,  0x2);
        tmac_field_CRC_ADD_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_MIN_FRM_CK_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_TRUNC_EN_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_MAX_FRM_CK_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_EBUF_PAUSE_EN_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_IBUF_HTHR_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_XON_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_RMAC_PAUSE_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_EBUF_ERR_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_TRANSPARENT_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_HW_STOP_EN_set(NULL, enet_fege_hndl_ptr,  0);

        tmac_field_LATE_COL_EN_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_SFD_EN_set(NULL, enet_fege_hndl_ptr,  1);
        tmac_field_PAMB_LEN_set(NULL, enet_fege_hndl_ptr,  0x7);

        tmac_field_FIPG1_LEN_set(NULL, enet_fege_hndl_ptr,  0xC);
        tmac_field_FIPG2_LEN_set(NULL, enet_fege_hndl_ptr,  0x8);

        tmac_reg_STATION_ADR_HIGH_WORD_CFG_write(NULL, enet_fege_hndl_ptr,  0x0);
        tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write(NULL, enet_fege_hndl_ptr,  0x0);
        tmac_reg_STATION_ADR_LOW_WORD_CFG_write(NULL, enet_fege_hndl_ptr,  0x0);
        tmac_field_MAX_FRM_LEN_set(NULL, enet_fege_hndl_ptr,  0x7E0);
        tmac_field_MIN_FRM_TAG_set(NULL, enet_fege_hndl_ptr,  0);
        tmac_field_MIN_FRM_LEN_set(NULL, enet_fege_hndl_ptr,  0x0);
        tmac_field_PAUSE_TIME_set(NULL, enet_fege_hndl_ptr,  0xFFFF);
        tmac_field_PAUSE_IVAL_set(NULL, enet_fege_hndl_ptr,  0xEFFF);

        /* L1TPP specific register configuration */ 

        l1tpp_fege_field_RD_ALIGN_EN_set(NULL, enet_fege_hndl_ptr, 0x1);
        l1tpp_fege_field_MODE_set(NULL, enet_fege_hndl_ptr, 0x1);
        l1tpp_fege_field_TENB_ERR_RDNEG_set(NULL, enet_fege_hndl_ptr, 0xF1);
        l1tpp_fege_field_CSF_ERR_CODE_set(NULL, enet_fege_hndl_ptr, 0x1FE);
        l1tpp_fege_field_PTRN_SEL_set(NULL, enet_fege_hndl_ptr, 0x0);
        l1tpp_fege_field_CUS_PTRN_set(NULL, enet_fege_hndl_ptr, 0xF4);
        l1tpp_fege_field_FEF_GEN_EN_set(NULL, enet_fege_hndl_ptr, 1);

        l1tpp_fege_field_L1TPP_EN_set(NULL, enet_fege_hndl_ptr, 1);

        tmac_field_TMAC_ENBL_set(NULL, enet_fege_hndl_ptr,  1);   
        rmac_field_RMAC_ENBL_set(NULL, enet_fege_hndl_ptr,  1);

    }
    else
    {
        /* shutdown ENET FEGE sub blocks */
        
        enet_fege_field_TX_TSTMP_FIFO_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_GE_GFPT_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_GE_GFPT_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_MSTATX_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_L1RPP_FEGE_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_RMAC_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_RX_ECLASS_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_L1TPP_FEGE_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_TMAC_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_ENET_FEGE_RST_set(NULL, enet_fege_hndl_ptr, 0x1);
        
        l1rpp_fege_field_L1RPP_EN_set(NULL, enet_fege_hndl_ptr, 0x0);
        
        rmac_field_RMAC_ENBL_set(NULL, enet_fege_hndl_ptr, 0);
        tmac_field_TMAC_ENBL_set(NULL, enet_fege_hndl_ptr, 0);
        
        enet_fege_field_MAC_LOWPWR_set(NULL, enet_fege_hndl_ptr, 0x1);
        enet_fege_field_GE_GFPT_LOWPWR_set(NULL, enet_fege_hndl_ptr, 0x1);
    }
    
    PMC_RETURN(rc);
}/* mgmt_fege_enet_fege_cfg */

/*******************************************************************************
*  mgmt_fege_ctxt_state_transition_request
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks that MGMT_FEGE state transition is allowed.
*
* INPUTS:
*   *hndl             - pointer to handle instance to operate on  
*    next_state       - next transition state
*
* OUTPUTS:
*    None.
*
* RETURNS:
*   PMC_SUCCESS if state transition is allowed otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_ctxt_state_transition_request(mgmt_fege_handle_t      *hndl,
                                                          mgmt_fege_state_t   next_state)
{
    PMC_ERROR rc;
    mgmt_fege_state_t   state;

    PMC_ENTRY();

    rc = mgmt_fege_ctxt_state_get(hndl, &state);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }
    
    if (MGMT_FEGE_STATE_START == state && MGMT_FEGE_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    else if (MGMT_FEGE_STATE_START == state && MGMT_FEGE_STATE_START == next_state)
    {
        PMC_RETURN(MGMT_FEGE_STATE_ALREADY_START);
    }

    
    if (MGMT_FEGE_STATE_OPERATIONAL == state && MGMT_FEGE_STATE_START == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    } 
    else if (MGMT_FEGE_STATE_OPERATIONAL == state && MGMT_FEGE_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(MGMT_FEGE_STATE_ALREADY_PROVISIONED);
    }

    
    PMC_RETURN(MGMT_FEGE_STATE_UNKOWN);
} /* mgmt_fege_ctxt_state_transition_request */

/*******************************************************************************
*  mgmt_fege_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets MGMT_FEGE instance state.
*
* INPUTS:
*   *hndl             - pointer to handle instance to operate on 
*    state            - provisioning state
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
PRIVATE void mgmt_fege_ctxt_state_set(mgmt_fege_handle_t      *hndl,
                                      mgmt_fege_state_t   state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    hndl->var.ctxt.state = state;

    PMC_RETURN();
} /* mgmt_fege_ctxt_state_set */

/*******************************************************************************
*  mgmt_fege_ctxt_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns MGMT_FEGE  instance state.
*
* INPUTS:
*   *hndl             - pointer to  MGMT_FEGE handle instance to be operated on 
*
* OUTPUTS:
*   *state            - provisioning state
*
* RETURNS:
*   Provisioning state.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_ctxt_state_get(mgmt_fege_handle_t      *hndl,
                                         mgmt_fege_state_t   *state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != state, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    *state = hndl->var.ctxt.state;
    
    PMC_RETURN(PMC_SUCCESS);
} /*mgmt_fege_ctxt_state_set */


/*******************************************************************************
* mgmt_fege_di_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for MGMT_FEGE block.
*
*
* INPUTS:
*   *hndl             - pointer to MGMT_FEGE handle instance
*    energy_state     - block new energy state
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error 
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_energy_state_reg_set(mgmt_fege_handle_t  *hndl,
                                                 pmc_energy_state_t energy_state)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        mgmt_fege_field_MGMT_FEGE_SW_RESET_set(NULL, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        mgmt_fege_field_MGMT_FEGE_SW_RESET_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }  

    PMC_RETURN(rc);
} /* mgmt_fege_energy_state_reg_set */

/*******************************************************************************
* mgmt_fege_restart_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup MGMT_FEGE context at digi top layer and in block level.
*
* INPUTS:
*  *mgmt_fege_handle       - Pointer to the digi handle instance.
*  *restart_init_cfg_ptr   - pointer to restart init config.
*   top_level_reset        - MGMT FEGE reset value at digi top layer.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mgmt_fege_restart_init(mgmt_fege_handle_t               *mgmt_fege_handle,
                                        util_global_restart_init_cfg_t   *restart_init_cfg_ptr,
                                        BOOL8                             top_level_reset)
{
    PMC_ERROR rc = MGMT_FEGE_LOG_ERR_UNKNOWN;
    mgmt_fege_var_t *var_clean_ptr;
    
    PMC_ENTRY();

    PMC_ASSERT(mgmt_fege_handle != NULL, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);

    var_clean_ptr = (mgmt_fege_var_t *) PMC_CALLOC(sizeof(mgmt_fege_var_t)); 

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only enabled channels configruation are recovered
       - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
       - incoherency in register context means that the channel is disabled
    */

    /* 
     * Audit register context and memory context 
     *   - Create a new context that represents the intersection between memory and register space (var_clean_ptr)
     */
    rc = mgmt_fege_ctxt_audit(mgmt_fege_handle,
                              var_clean_ptr,
                              top_level_reset); 
    PMC_LOG_TRACE("mgmt_fege_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    
    if (MGMT_FEGE_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, mgmt_fege_ctxt is the new context
         *    - We keep enabled channels defined in var_clean_ptr
         *    - We clean (deactivate/deprov) all channels that are not enabled
         *       - they may have be partially enabled
         */                                    
        rc = mgmt_fege_var_cleanup(mgmt_fege_handle,
                                   var_clean_ptr,
                                   top_level_reset); 
    }
    
    PMC_FREE(&var_clean_ptr);
    
    PMC_RETURN(rc);
    
} /* mgmt_fege_restart_init */

/*******************************************************************************
* mgmt_fege_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits MGMT_FEGE handle context variable against register 
*   space content.
*
* INPUTS:
*   *mgmt_fege_handle     - pointer to MGMT_FEGE handle instance.
*   *var_clean_ptr        - pointer to MGMT_FEGE context.
*   top_level_reset       - MGMT FEGE reset value at digi top layer.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if auditing is successful otherwise a descriptive error code is
*   returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_ctxt_audit(mgmt_fege_handle_t     *mgmt_fege_handle,
                                       mgmt_fege_var_t        *var_clean_ptr,
                                       BOOL8                   top_level_reset)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR loop_rc = PMC_SUCCESS;
    BOOL8 reset;
    BOOL8 is_op = FALSE;
    UINT32 itr;
    mgmt_fege_state_t   state;
    BOOL8 rx_enet_fege_state;
    BOOL8 tx_enet_fege_state;

    PMC_ENTRY();
    
    reset = mgmt_fege_field_MGMT_FEGE_SW_RESET_get(NULL, mgmt_fege_handle) ? TRUE : FALSE;

    /* Compare top level reset against block reset */    
    if ((top_level_reset != mgmt_fege_field_MGMT_FEGE_SW_RESET_get(NULL, mgmt_fege_handle)))
    {
        rc = MGMT_FEGE_ERR_CONTEXT_INCOHERENT;
    }

    if (PMC_SUCCESS == rc)
    {
        /* check coherency between reset and driver state */
        (void)mgmt_fege_ctxt_state_get(mgmt_fege_handle, &state);
        if (FALSE == reset)
        {
            if (MGMT_FEGE_STATE_OPERATIONAL != state)
            {
                rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;
            }            
            else
            {
                is_op = TRUE;
            }
        }
        else
        { 
            if (MGMT_FEGE_STATE_START != state)
            {
                rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;
            }
        }
    }

    /* check ENET_FEGE status */
    if (PMC_SUCCESS == rc)
    {               
        rx_enet_fege_state = 0x1 == rmac_field_RMAC_ENBL_get(NULL, mgmt_fege_handle->enet_fege_handle) ? TRUE : FALSE;
        tx_enet_fege_state = 0x1 == tmac_field_TMAC_ENBL_get(NULL, mgmt_fege_handle->enet_fege_handle) ? TRUE : FALSE;

        if ((rx_enet_fege_state != is_op) ||
            (rx_enet_fege_state != tx_enet_fege_state))
        {
            rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;
        }
    }

    /* audit context forwarding tables */
    if (PMC_SUCCESS == rc && 
        TRUE == is_op)
    {        
        for (itr = 0; itr < MGMT_FEGE_VLAN_REPL_TABLE_SIZE; itr++)
        {
            loop_rc = PMC_SUCCESS;

            /* audit egress lookup table */
            if (TRUE == mgmt_fege_handle->var.egr_fwd_tbl[itr].valid)
            {                
                if (MGMT_FEGE_VLAN_REPL_TABLE_SIZE > mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.dpi_id)
                {
                    if (0x1 != mgmt_fege_field_TXLUT_VALID_get(NULL, mgmt_fege_handle, mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.dpi_id))
                    {
                        loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                    }  
                    if (PMC_SUCCESS == loop_rc &&
                        (UINT32)mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.ins_vlan_id != mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_get(NULL, mgmt_fege_handle, mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.dpi_id))
                    {
                        rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                    }
                    if (PMC_SUCCESS == loop_rc &&
                        TRUE == mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.ins_vlan_id &&
                        mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.vlan_entry.vlan_id != mgmt_fege_field_TXLUT_VLAN_ID_get(NULL, mgmt_fege_handle, mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.dpi_id))
                    {
                        loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                    }
                }
                else
                {
                    loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;    
                }
            }
            else
            {        
                if (0x0 != mgmt_fege_field_TXLUT_VALID_get(NULL, mgmt_fege_handle, itr))
                {
                    loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                }  
            }
            /* copy egress entry in clean context if no incohency was found */
            if (PMC_SUCCESS == loop_rc)
            {
                var_clean_ptr->egr_fwd_tbl[itr].valid = mgmt_fege_handle->var.egr_fwd_tbl[itr].valid;
                if (TRUE == mgmt_fege_handle->var.egr_fwd_tbl[itr].valid)
                {
                    var_clean_ptr->egr_fwd_tbl[itr].num_regs += 1;
                    var_clean_ptr->egr_fwd_tbl[itr].fwd_entry.dpi_id = mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.dpi_id;
                    if (TRUE ==  mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.ins_vlan_id)
                    {
                        var_clean_ptr->egr_fwd_tbl[itr].fwd_entry.ins_vlan_id = mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.ins_vlan_id;
                        PMC_MEMCPY((void *)&var_clean_ptr->egr_fwd_tbl[itr].fwd_entry.vlan_entry, &mgmt_fege_handle->var.egr_fwd_tbl[itr].fwd_entry.vlan_entry, sizeof(util_global_vlan_cfg_t));
                    }
                }
            }

            if (PMC_SUCCESS == loop_rc)
            {
                /* audit ingress lookup table */
                if (TRUE == mgmt_fege_handle->var.ing_fwd_tbl[itr].valid)
                {            
                    if (0x1 != mgmt_fege_field_RXLUT_VALID_get(NULL, mgmt_fege_handle, itr))
                    {
                        loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                    }      
                }
                else
                {        
                    if (0x0 != mgmt_fege_field_RXLUT_VALID_get(NULL, mgmt_fege_handle, itr))
                    {
                        loop_rc = MGMT_FEGE_ERR_CONTEXT_CLEANUP;                    
                    }  
                }

                /* copy ingress entry in clean context if no incohency was found */
                if (PMC_SUCCESS == loop_rc)
                {
                    var_clean_ptr->ing_fwd_tbl[itr].valid = mgmt_fege_handle->var.ing_fwd_tbl[itr].valid;
                }                
            }

            if (PMC_SUCCESS == rc)
            {
                rc = loop_rc;
            }
        }
    }
    
    PMC_RETURN(rc);
} /* mgmt_fege_ctxt_audit */

/*******************************************************************************
* mgmt_fege_var_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up MGMT_FEGE handle context. 
*
* INPUTS:
*   *mgmt_fege_handle     - pointer to MGMT_FEGE handle instance.
*   *var_clean_ptr        - pointer to MGMT_FEGE lanes clean context.
*   top_level_reset       - MGMT FEGE reset value at digi top layer.
*
* OUTPUTS:
*   None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mgmt_fege_var_cleanup(mgmt_fege_handle_t      *mgmt_fege_handle,
                                        mgmt_fege_var_t         *var_clean_ptr,
                                        BOOL8                    top_level_reset)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr;
    BOOL8 one_entry_valid = FALSE;

    PMC_ENTRY();

    for (itr = 0; itr < MGMT_FEGE_VLAN_REPL_TABLE_SIZE; itr++)
    {        
        /* audit egress lookup table */
        if (FALSE == var_clean_ptr->egr_fwd_tbl[itr].valid)
        {                
            mgmt_fege_field_TXLUT_VALID_set(NULL, mgmt_fege_handle, itr, 0x0);
        }
        else
        {
            one_entry_valid = TRUE;
        }

        /* audit ingress lookup table */
        if (FALSE == var_clean_ptr->ing_fwd_tbl[itr].valid)
        {                
            mgmt_fege_field_RXLUT_VALID_set(NULL, mgmt_fege_handle, itr, 0x0);
        }
        else
        {
            one_entry_valid = TRUE;
        }
    }

    /* copy clean context in mgmt_fege_handle context */
    PMC_MEMCPY((void*)&mgmt_fege_handle->var, (void *)var_clean_ptr, sizeof(mgmt_fege_var_t));

    /* uninitialize mgmt_fege if any channel is not configured */
    if (FALSE == one_entry_valid)
    {
        mgmt_fege_uninit(mgmt_fege_handle, TRUE);
    }

    PMC_RETURN(rc);
} /* mgmt_fege_var_cleanup */

/*******************************************************************************
* mgmt_fege_pkt_fwd_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Returns channel status in given direction: ingress or egress.
*
* INPUTS:
*  *mgmt_fege_handle     - pointer to MGMT_FEGE handle instance
*   dpi_id               - channel dpi identifier.
*   dir                  - channel direction: ingress or egress.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when channel in activated in given direciton.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mgmt_fege_pkt_fwd_status_get(mgmt_fege_handle_t          *mgmt_fege_handle,
                                          UINT32                       dpi_id,
                                          util_global_datapath_dir_t   dir)
{
    PMC_ERROR    rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
                  
    /* retrieve lookup table entry index associated to request */
    switch(dir)
    {
    case UTIL_GLOBAL_DATAPATH_DIR_EGRESS:
        rc =  0x1 == mgmt_fege_field_TXLUT_VALID_get(NULL, mgmt_fege_handle, dpi_id) ? TRUE : FALSE;
        break;
    case UTIL_GLOBAL_DATAPATH_DIR_INGRESS:
        rc =  0x1 == mgmt_fege_field_RXLUT_VALID_get(NULL, mgmt_fege_handle, dpi_id) ? TRUE : FALSE;
        break;
    default:
        break;
    }

    PMC_RETURN(rc);

} /* mgmt_fege_pkt_fwd_status_get */

/*******************************************************************************
* mgmt_fege_ingr_pkt_fwd_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function cleans up forwarding path in MGMT_FEGE INGRESS datatpath.
*   Packets possiblibly forwarded from MCPB to in from MGMT_FEGE.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
*    dpi_id                  - dpi identifier.
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
PUBLIC void mgmt_fege_ingr_pkt_fwd_cleanup(mgmt_fege_handle_t   *mgmt_fege_handle,
                                           UINT32                dpi_id)
{
    mgmt_fege_buffer_t buf[1];

    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
   
    mgmt_fege_handle->var.egr_fwd_tbl[dpi_id].num_regs = 0;
    mgmt_fege_handle->var.egr_fwd_tbl[dpi_id].valid = FALSE;
    
    
    mgmt_fege_buffer_init(buf, mgmt_fege_handle);        
    mgmt_fege_field_TXLUT_VALID_set(buf, mgmt_fege_handle, dpi_id, 0x0);             
    mgmt_fege_field_TXLUT_VLAN_ID_REPLACE_set(buf, mgmt_fege_handle, dpi_id, 0x0);  
    mgmt_fege_buffer_flush(buf);   

    PMC_RETURN();
} /* mgmt_fege_ingr_pkt_fwd_cleanup */

/*******************************************************************************
* mgmt_fege_egr_pkt_fwd_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function cleans up forwarding path in MGMT_FEGE EGRESS datatpath.
*   Packets possiblibly forwarded from MGMT_FEGE to MCPB.
*
* INPUTS:
*   *mgmt_fege_handle        - pointer to MGMT_FEGE handle instance
*    dpi_id                  - dpi indentifier.
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
PUBLIC void mgmt_fege_egr_pkt_fwd_cleanup(mgmt_fege_handle_t   *mgmt_fege_handle,
                                          UINT32                dpi_id)
{

    mgmt_fege_buffer_t buf[1];
    PMC_ENTRY();

    PMC_ASSERT(NULL != mgmt_fege_handle, MGMT_FEGE_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(MGMT_FEGE_VLAN_REPL_TABLE_SIZE > dpi_id, MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    
    mgmt_fege_buffer_init(buf, mgmt_fege_handle);
          
    mgmt_fege_field_RXLUT_VALID_set(buf, mgmt_fege_handle, dpi_id, 0x0); 
    mgmt_fege_field_RXLUT_PRIORITY_ENABLE_set(buf, mgmt_fege_handle, dpi_id, 0x0);
    mgmt_fege_field_RXLUT_FIFO_ZONE_ENABLE_set(buf, mgmt_fege_handle, dpi_id, 0x0);
    mgmt_fege_field_RXLUT_FCS_ENABLE_set(buf, mgmt_fege_handle, dpi_id, 0x0);

    mgmt_fege_buffer_flush(buf); 

   
    mgmt_fege_handle->var.ing_fwd_tbl[dpi_id].num_regs = 0;
    mgmt_fege_handle->var.ing_fwd_tbl[dpi_id].valid = FALSE;

    PMC_RETURN();

} /* mgmt_fege_egr_pkt_fwd_cleanup */

/*
** End of file
*/
