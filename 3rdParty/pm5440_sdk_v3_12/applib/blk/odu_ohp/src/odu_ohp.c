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
*     Implementation of the OHP software driver.
*
*   NOTES:
*       See also digi_otn.c and coreotn.c
*
*******************************************************************************/
/*
** Include Files
*/

#include "odu_ohp_loc.h"
#include "odu_ohp_core_io.h"
#include "cfc_io.h"


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
const char ODU_OHP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODU_OHP_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODU_OHP_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/

PRIVATE void odu_ohp_var_default_init(odu_ohp_handle_t *hndl);


PRIVATE void odu_ohp_ctxt_state_set(odu_ohp_handle_t      *hndl,
                                    odu_ohp_state_t   state);

PRIVATE void odu_ohp_ctxt_state_get(odu_ohp_handle_t      *hndl,
                                    odu_ohp_state_t       *state);

PRIVATE PMC_ERROR odu_ohp_ctxt_state_transition_request(odu_ohp_handle_t      *hndl,
                                                        odu_ohp_state_t   next_state);

PRIVATE PMC_ERROR odu_ohp_port_init(odu_ohp_handle_t *hndl, 
                                    odu_ohp_port_cfg_t *port_ptr, 
                                    UINT32 max_ch);

PRIVATE PMC_ERROR odu_ohp_reg_update_schedule(odu_ohp_handle_t *hndl, 
                                              odu_ohp_port_t port, 
                                              UINT8 *schedule, 
                                              BOOL  do_page_switch);

PRIVATE PMC_ERROR odu_ohp_sched_page_switch(odu_ohp_handle_t *hndl, 
                                            odu_ohp_port_t port);

PRIVATE UINT32 odu_ohp_schedule_resize(odu_ohp_handle_t *hndl, 
                                       odu_ohp_port_t port, 
                                       UINT32 ch_id, 
                                       INT32 resize);

PRIVATE PMC_ERROR odu_ohp_reg_port_cfg(odu_ohp_handle_t *hndl, 
                                        odu_ohp_port_t port, 
                                        UINT32 sch_en,  
                                        UINT32 sd_pcc,  
                                        UINT32 src_user_id,
                                        UINT32 dest_user_id,
                                        UINT32 num_ch,
                                        UINT32 ch_en);

PRIVATE void odu_ohp_reg_ch_cfg(odu_ohp_handle_t *hndl, 
                                        odu_ohp_port_t port, 
                                        UINT32 ch_id,  
                                        UINT32 extract_mask,  
                                        UINT32 insert_mask,  
                                        UINT32 tx_offset,
                                        BOOL ch_en);

PRIVATE void odu_ohp_ch_tx_cfg(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,  
                               UINT32 insert_mask,  
                               UINT32 tx_offset);

PRIVATE void odu_ohp_ch_rx_cfg(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,  
                               UINT32 extract_mask);

PRIVATE BOOL odu_ohp_ch_en_get(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id);

PRIVATE UINT32 odu_ohp_tx_offset_get(odu_ohp_handle_t *hndl, 
                                     odu_ohp_port_t port, 
                                     UINT32 ch_id);

PRIVATE void odu_ohp_tx_offset_set(odu_ohp_handle_t *hndl, 
                                   odu_ohp_port_t port, 
                                   UINT32 ch_id,
                                   UINT32 tx_offset);

PRIVATE void odu_ohp_ch_en_set(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,
                               UINT32 ch_en);

PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_retrieve_internal(cfc_handle_t    *cfc_handle,
                                                         UINT32              fifo_id,
                                                         cfc_int_chnl_t  *int_en_table_ptr,
                                                         cfc_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_enable_internal(cfc_handle_t    *cfc_handle,
                                                       UINT32           fifo_id,
                                                       cfc_int_chnl_t  *int_table_ptr,
                                                       cfc_int_chnl_t  *int_en_table_ptr,
                                                       BOOL8 enable);

PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_clear_internal(cfc_handle_t    *cfc_handle,
                                                      UINT32           fifo_id,
                                                      cfc_int_chnl_t  *int_en_table_ptr,
                                                      cfc_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_enabled_check_internal(cfc_handle_t    *cfc_handle,
                                                              UINT32           fifo_id,
                                                              cfc_int_chnl_t  *int_en_table_ptr,
                                                              BOOL8           *int_found_ptr);

PRIVATE PMC_ERROR odu_ohp_schedule_add_ch(odu_ohp_handle_t *hndl, 
                                          odu_ohp_port_t port, 
                                          UINT32 ch_id, 
                                          UINT32 num_cal_entries,
                                          UINT32 *cal_entries);

PRIVATE PMC_ERROR odu_ohp_schedule_remove_ch(odu_ohp_handle_t *hndl, 
                                             odu_ohp_port_t port, 
                                             UINT32 ch_id);

PRIVATE UINT32 odu_ohp_mask_get(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id, 
                                BOOL ins_mask);
/*
** Public Functions
*/

/*******************************************************************************
* odu_ohp_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an ODU OHP block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the OHP relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*   ohp                   - OHP1, OHP2, OHP3
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odu_ohp_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_ohp_handle_t *odu_ohp_ctxt_create(pmc_handle_t       *parent, 
                                             UINT32              base_address, 
                                             pmc_sys_handle_t   *sys_handle, 
                                             const char         *tsb_name,
                                             odu_ohp_inst_t      ohp)
{
    odu_ohp_handle_t *odu_ohp_handle;

    PMC_ENTRY();

    odu_ohp_handle = (odu_ohp_handle_t*)PMC_CTXT_CALLOC(sizeof(odu_ohp_handle_t), parent);

    pmc_handle_init(parent, odu_ohp_handle, sys_handle, PMC_MID_DIGI_ODU_OHP, tsb_name, base_address);

    /* Register log strings */
    pmc_log_block_strings_register(ODU_OHP_LOG_ERR_STRINGS[0], ODU_OHP_LOG_ERR_TABLE_BASE, ODU_OHP_LOG_ERR_COUNT);

    odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0] = cfc_ctxt_create((pmc_handle_t *)odu_ohp_handle, base_address+BASE_ADDR_OHP_96CH_OHP96_OHP_96CH_CORE_CFC96_EX0_CFC, sys_handle, "ODU_OHP_CFC_EX0"); 
    PMC_CTXT_REGISTER_PTR(&odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0], parent);
    odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1] = cfc_ctxt_create((pmc_handle_t *)odu_ohp_handle, base_address+BASE_ADDR_OHP_96CH_OHP96_OHP_96CH_CORE_CFC96_EX1_CFC, sys_handle, "ODU_OHP_CFC_EX1"); 
    PMC_CTXT_REGISTER_PTR(&odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1], parent);
    odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0] = cfc_ctxt_create((pmc_handle_t *)odu_ohp_handle, base_address+BASE_ADDR_OHP_96CH_OHP96_OHP_96CH_CORE_CFC96_IN0_CFC, sys_handle, "ODU_OHP_CFC_IN0"); 
    PMC_CTXT_REGISTER_PTR(&odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0], parent);
    odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1] = cfc_ctxt_create((pmc_handle_t *)odu_ohp_handle, base_address+BASE_ADDR_OHP_96CH_OHP96_OHP_96CH_CORE_CFC96_IN1_CFC, sys_handle, "ODU_OHP_CFC_IN1"); 
    PMC_CTXT_REGISTER_PTR(&odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1], parent);
    
    /* create ENET_FEGE handle */
    odu_ohp_handle->enet_fege_handle = enet_fege_ctxt_create((pmc_handle_t *)odu_ohp_handle, base_address+BASE_ADDR_OHP_96CH_FEGE_ENET_FEGE_MTSB, sys_handle, "ODU_OHP_ENET_FEGE");
    PMC_CTXT_REGISTER_PTR(&odu_ohp_handle->enet_fege_handle, parent);

    /* update odu_ohp configuration  */
    odu_ohp_handle->cfg.ohp = ohp;
    
    switch (ohp)
    {
    case ODU_OHP_1:
        odu_ohp_handle->cfg.num_chnl = ODU_OHP_1_CH_MAX; /* 12 */
        break;
    case ODU_OHP_2:
        odu_ohp_handle->cfg.num_chnl = ODU_OHP_2_CH_MAX; /* 96 */
        break;
    case ODU_OHP_3:
        odu_ohp_handle->cfg.num_chnl = ODU_OHP_3_CH_MAX; /* 96 */
        break;
    default:
        PMC_ASSERT(FALSE, ODU_OHP_ERR_INVALID_OHP,0,0);
        odu_ohp_handle->cfg.num_chnl = ODU_OHP_1_CH_MAX;
    }

    odu_ohp_handle->var.ctxt.state = LAST_ODU_OHP_STATE;
    
    PMC_RETURN(odu_ohp_handle);
} /* odu_ohp_ctxt_create */


/*******************************************************************************
* odu_ohp_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an ODU_OHP block instance.
*
*
* INPUTS:
*   *odu_ohp_handle   - pointer to ODU_OHP handle instance
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
PUBLIC void odu_ohp_ctxt_destroy(odu_ohp_handle_t *odu_ohp_handle)
{
    UINT32 i;

    PMC_ENTRY();

    /* destroy ENET_FEGE handle */
    enet_fege_ctxt_destroy(odu_ohp_handle->enet_fege_handle);

    /* destroy CFCs handle */
    for (i = 0; i < ODU_OHP_CFC_MAX; i++)
    {
        cfc_ctxt_destroy(odu_ohp_handle->cfc_handle[i]);
    }

    PMC_CTXT_FREE(&odu_ohp_handle, odu_ohp_handle);
   
    PMC_RETURN();
} /* odu_ohp_ctxt_destroy */

/*******************************************************************************
* odu_ohp_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODU_OHP block handle instance.
*
*
* INPUTS:
*   *hndl           - pointer to ODU_OHP handle instance
*   rx_db_handle    - Schedule DB handle
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
PUBLIC void odu_ohp_handle_init(odu_ohp_handle_t *hndl, UINT32 rx_db_handle)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    UINT32 i;
    pmc_handle_t *parent;


    PMC_ENTRY();

 
    for (i = 0; i <  ODU_OHP_CFC_MAX; i++)
    {
        cfc_cfg.max_fifo_num =  hndl->cfg.num_chnl;
        cfc_cfg.page_size    = ODU_OHP_CFC_PAGE_SIZE;
        cfc_handle_init(hndl->cfc_handle[i],&cfc_cfg);
        hndl->cfc_handle[i]->cfg.cfg_fct_table.cfc_int_chnl_retrieve_ptr = odu_ohp_cfc_int_chnl_retrieve_internal;
        hndl->cfc_handle[i]->cfg.cfg_fct_table.cfc_int_chnl_enable_ptr = odu_ohp_cfc_int_chnl_enable_internal;
        hndl->cfc_handle[i]->cfg.cfg_fct_table.cfc_int_chnl_clear_ptr = odu_ohp_cfc_int_chnl_clear_internal;
        hndl->cfc_handle[i]->cfg.cfg_fct_table.cfc_int_chnl_enabled_check_ptr = odu_ohp_cfc_int_chnl_enabled_check_internal;
    }
    

    enet_fege_handle_init(hndl->enet_fege_handle); 
    
    odu_ohp_var_default_init(hndl);
   
    /* get the ultimate parent for DB usage */
    parent = pmc_handle_ultimate_parent_get(hndl);

    hndl->cfg.db_dev_addr = (UINT32*)parent;
    PMC_CTXT_REGISTER_PTR(&hndl->cfg.db_dev_addr, parent);

    hndl->cfg.rx_db_handle = rx_db_handle;

    PMC_RETURN();

} /* odu_ohp_handle_init */


/*******************************************************************************
* odu_ohp_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handle local memory allocation freed in ctxt_destroy
*
*
* INPUTS:
*   *hndl               - pointer to ODU_OHP handle instance
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
PUBLIC void odu_ohp_alloc_init(odu_ohp_handle_t *hndl)
{
    UINT32 i;

    PMC_ENTRY();

     for (i = 0; i <  ODU_OHP_CFC_MAX; i++)
    {
      cfc_alloc_init(hndl->cfc_handle[i]);
    }
    
    PMC_RETURN();

} /* odu_ohp_alloc_init */


/*******************************************************************************
* odu_ohp_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an ODU_OHP block handle instance.
*
*
* INPUTS:
*   *hndl                 - Pointer to ODU_OHP handle instance
*   rx_db_handle          - Schedule DB handle
*   *restart_init_cfg_ptr - Pointer to restart init config
*   top_energy_state      - The expected energy state 
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
PUBLIC void odu_ohp_handle_restart_init(odu_ohp_handle_t *hndl, 
                                        UINT32 rx_db_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                        pmc_energy_state_t top_energy_state)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    UINT32 i;


    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODU_OHP_LOG_ERR_STRINGS[0], ODU_OHP_LOG_ERR_TABLE_BASE, ODU_OHP_LOG_ERR_COUNT);
    }
 
    for (i = 0; i <  ODU_OHP_CFC_MAX; i++)
    {
        cfc_cfg.max_fifo_num =  hndl->cfg.num_chnl;
        cfc_cfg.page_size    = ODU_OHP_CFC_PAGE_SIZE;
        cfc_handle_restart_init(hndl->cfc_handle[i],&cfc_cfg, restart_init_cfg_ptr, top_energy_state);
    }
    
    PMC_RETURN();

} /* odu_ohp_handle_restart_init */


/*******************************************************************************
* odu_ohp_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODU_OHP block instance.
*
*
* INPUTS:
*   *odu_ohp_handle   - pointer to ODU_OHP handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 odu_ohp_base_address_get(odu_ohp_handle_t *odu_ohp_handle)
{
    PMC_ENTRY();

    PMC_RETURN(odu_ohp_handle->base.base_address);
} /* odu_ohp_base_address_get */


/*******************************************************************************
* odu_ohp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   There are three ODU Overhead processors (OHPs). Each OHP contains two 
*   ports (port A and port B) which can connect to two different framers. The 
*   first OHP (DIGI_OHP_1) supports 12 channels and has a fixed connection to 
*   the stage 1 (HO) framer (PORT A) and the stage N framer (PORT B). The 
*   second and third OHPs can be configured to connect to framer 2 (MO), 
*   framer 3A (LO), framer 3B (LO) or framer 4 (SNC).
*
*   This function initializes a single OHP. The function should be called once
*   for each of the three OHPs. The port to framer connections are specified 
*   using the port_a and port_b parameters.  If a port is unused, the value 
*   should be set to DIGI_OHP_PORT_UNUSED.
*
*   The ethernet source and destination addresses consits of the 32 bit 
*   source/dest user ID, 8 bit framer ID and 8 bit channel id.
*              
*
* INPUTS:
*   *hndl                  - OHP handle instance
*   cfg_ptr                - Configuration structure containing the setup
*                            for the OHP port.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_init(odu_ohp_handle_t *hndl, 
                              odu_ohp_cfg_t *cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_a_ptr;
    odu_ohp_port_cfg_t *port_cfg_b_ptr;
    BOOL port_a_en, port_b_en;
    odu_ohp_inst_t ohp = cfg_ptr->ohp;
    odu_ohp_port_framer_t port_a_framer = cfg_ptr->port_a_framer; 
    odu_ohp_port_framer_t port_b_framer = cfg_ptr->port_b_framer; 
    UINT32 port_a_src_addr = cfg_ptr->port_a_src_addr; 
    UINT32 port_b_src_addr = cfg_ptr->port_b_src_addr; 
    UINT32 port_a_dest_addr = cfg_ptr->port_a_dest_addr; 
    UINT32 port_b_dest_addr = cfg_ptr->port_b_dest_addr; 
    BOOL config_fege = TRUE;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port_a_framer < ODU_OHP_PORT_FRAMER_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
    PMC_ASSERT(port_b_framer < ODU_OHP_PORT_FRAMER_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
    
    PMC_LOG_TRACE("ohp: init: %x port a: %x %x %x\n",ohp,port_a_framer,port_a_src_addr,port_a_dest_addr);
    PMC_LOG_TRACE("ohp: init: %x port b: %x %x %x\n",ohp,port_b_framer,port_b_src_addr,port_b_dest_addr);

    rc = odu_ohp_ctxt_state_transition_request(hndl, ODU_OHP_STATE_OPERATIONAL);
    if (rc != PMC_SUCCESS)
    {
        PMC_RETURN(rc);
    }
   
    switch(ohp)
    {
        case ODU_OHP_1:
            /* 
             *  OHP 1: Port A can only be connected to FRAMER_1
             *         Port B can only be connected to FRAMER_N
             */
            if (port_a_framer != ODU_OHP_PORT_FRAMER_1 && port_a_framer != ODU_OHP_PORT_UNUSED)
            {
                PMC_RETURN(ODU_OHP_ERR_INVALID_FRAMER);
            }
            if (port_b_framer != ODU_OHP_PORT_FRAMER_N && port_b_framer != ODU_OHP_PORT_UNUSED) 
            {
                PMC_RETURN(ODU_OHP_ERR_INVALID_FRAMER);
            }
            break;
        case ODU_OHP_2:
        case ODU_OHP_3:
            /* OHP 2 and 3 can't connect to the 12 channel framers */
            if (port_a_framer == ODU_OHP_PORT_FRAMER_1 || port_b_framer == ODU_OHP_PORT_FRAMER_1)
            {
                PMC_RETURN(ODU_OHP_ERR_INVALID_FRAMER);
            }
            if (port_a_framer == ODU_OHP_PORT_FRAMER_N || port_b_framer == ODU_OHP_PORT_FRAMER_N)
            {
                PMC_RETURN(ODU_OHP_ERR_INVALID_FRAMER);
            }
            break;
        default:
           PMC_RETURN(ODU_OHP_ERR_INVALID_ARGS);
    }

    odu_ohp_core_field_OHP_EN_set(NULL, hndl, 0);

    port_cfg_a_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_A]);
    port_cfg_a_ptr->ohp = ohp;
    port_cfg_a_ptr->port_framer = port_a_framer;
    port_cfg_a_ptr->port = ODU_OHP_PORT_A;
    port_cfg_a_ptr->src_user_id = port_a_src_addr;
    port_cfg_a_ptr->dest_user_id = port_a_dest_addr;
    port_cfg_a_ptr->cfc_in = hndl->cfc_handle[ODU_OHP_CFC_IN0];
    PMC_CTXT_REGISTER_PTR(&port_cfg_a_ptr->cfc_in, hndl);
    port_cfg_a_ptr->cfc_ex = hndl->cfc_handle[ODU_OHP_CFC_EX0];
    PMC_CTXT_REGISTER_PTR(&port_cfg_a_ptr->cfc_ex, hndl);

    rc  = cfc_init(port_cfg_a_ptr->cfc_in,PMC_ENERGY_STATE_RUNNING);
    if (rc == PMC_SUCCESS) 
    {
        rc = cfc_init(port_cfg_a_ptr->cfc_ex,PMC_ENERGY_STATE_RUNNING);
    }

    port_cfg_b_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_B]);
    port_cfg_b_ptr->ohp = ohp;
    port_cfg_b_ptr->port_framer = port_b_framer;
    port_cfg_b_ptr->port = ODU_OHP_PORT_B;
    port_cfg_b_ptr->src_user_id = port_b_src_addr;
    port_cfg_b_ptr->dest_user_id = port_b_dest_addr;
    port_cfg_b_ptr->cfc_in = hndl->cfc_handle[ODU_OHP_CFC_IN1];
    PMC_CTXT_REGISTER_PTR(&port_cfg_b_ptr->cfc_in, hndl);
    port_cfg_b_ptr->cfc_ex = hndl->cfc_handle[ODU_OHP_CFC_EX1];
    PMC_CTXT_REGISTER_PTR(&port_cfg_b_ptr->cfc_ex, hndl);

    if (rc == PMC_SUCCESS) 
    {
        rc = cfc_init(port_cfg_b_ptr->cfc_in,PMC_ENERGY_STATE_RUNNING);
    }
    if (rc == PMC_SUCCESS) 
    {
        rc = cfc_init(port_cfg_b_ptr->cfc_ex,PMC_ENERGY_STATE_RUNNING);
    }
    if (rc == PMC_SUCCESS)
    {
        /* configure fege */
        if (config_fege)
        {   
           
            rc = enet_fege_prov(hndl->enet_fege_handle, FALSE, TRUE, TRUE);  /* use rcp config seq, tx enable=true, rx enable =true*/
            odu_ohp_core_field_SGMII_TX_FLIP_set(NULL, hndl, 1);
            odu_ohp_core_field_SGMII_RX_FLIP_set(NULL, hndl, 0);
            odu_ohp_core_field_FEGE_RX_FLIP_set(NULL, hndl, 0);
            odu_ohp_core_field_FEGE_TX_FLIP_set(NULL, hndl, 0);
        }
    }
    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }

    port_a_en = (port_cfg_a_ptr->port_framer != ODU_OHP_PORT_UNUSED);
    port_b_en = (port_cfg_b_ptr->port_framer != ODU_OHP_PORT_UNUSED);
    
    PMC_LOG_TRACE("OHP: porta en = %d,portb en = %d\n",port_a_en,port_b_en);

    if (port_a_en || port_b_en)
    {
        /* enable OHP but leave all channels disabled */
        odu_ohp_port_init(hndl,port_cfg_a_ptr,hndl->cfg.num_chnl); 
        odu_ohp_port_init(hndl,port_cfg_b_ptr,hndl->cfg.num_chnl); 

        odu_ohp_core_field_OHP_EN_set(NULL, hndl, 1);
        /* rx schedule enable */
        if (port_a_en) 
        {
            odu_ohp_core_field_RXFA_SCH_EN_set(NULL, hndl, 0); 
            odu_ohp_core_field_RXFA_SCH_EN_set(NULL, hndl, 1); 
        }
        if (port_b_en) 
        {
            odu_ohp_core_field_RXFB_SCH_EN_set(NULL, hndl,0); 
            odu_ohp_core_field_RXFB_SCH_EN_set(NULL, hndl,1); 
        }
        odu_ohp_ctxt_state_set(hndl,ODU_OHP_STATE_OPERATIONAL);
    }
    else
    {
        PMC_LOG_TRACE("OHP: Warning: No ports enabled !\n");
        /* disable OHP */
        odu_ohp_core_field_OHP_EN_set(NULL, hndl, 0);
    }

    PMC_RETURN(rc);
} /* odu_ohp_init */

/*******************************************************************************
* odu_ohp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable an OHP. This function is used to power up OHP when RCP APS 
*   insertion is used. For normal OHP configuration, the odu_ohp_init 
*   function should be used to initialize OHP.
*
* INPUTS:
*   *hndl                  - OHP handle instance
*   enable                 - enable the OHP when TRUE, disable if FALSE
*                            
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_cfg(odu_ohp_handle_t *hndl, BOOL enable)
{
    PMC_ENTRY();
    odu_ohp_core_field_OHP_EN_set(NULL, hndl, enable ? 1 : 0);
    PMC_RETURN(PMC_SUCCESS);
} /* odu_ohp_cfg */



/*******************************************************************************
* odu_ohp_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset one of the three OHPs.
*              
*
* INPUTS:
*   *hndl               - OHP handle instance
*
*                            
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_uninit(odu_ohp_handle_t *hndl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_a_ptr;
    odu_ohp_port_cfg_t *port_cfg_b_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);

    rc = odu_ohp_ctxt_state_transition_request(hndl,  ODU_OHP_STATE_START);


    if (rc != PMC_SUCCESS)
    {
        PMC_RETURN(rc);
    }


    /* disable OHP */
    odu_ohp_core_field_OHP_EN_set(NULL, hndl, 0);

    port_cfg_a_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_A]);
    rc  = cfc_init(port_cfg_a_ptr->cfc_in,PMC_ENERGY_STATE_RESET);
    rc |= cfc_init(port_cfg_a_ptr->cfc_ex,PMC_ENERGY_STATE_RESET);
    PMC_MEMSET(port_cfg_a_ptr,0,sizeof(*port_cfg_a_ptr));

    port_cfg_b_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_B]);
    rc |= cfc_init(port_cfg_b_ptr->cfc_in,PMC_ENERGY_STATE_RESET);
    rc |= cfc_init(port_cfg_b_ptr->cfc_ex,PMC_ENERGY_STATE_RESET);
    PMC_MEMSET(port_cfg_b_ptr,0,sizeof(*port_cfg_b_ptr));
    
    odu_ohp_ctxt_state_set(hndl,ODU_OHP_STATE_START);

    rc = enet_fege_deprov(hndl->enet_fege_handle);
   
    PMC_RETURN(rc);
} /*odu_ohp_uninit*/

/*******************************************************************************
* odu_ohp_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets OHP configuration and enable status.
*              
*
* INPUTS:
*   *hndl                  - OHP handle instance
*
* OUTPUTS:
*   cfg_ptr                - Configuration structure for the OHP port.
*                            Its content is invalid when it is uninited.
*   enable                 - TRUE when OHP inited or FALSE when OHP uninited
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_init_get(odu_ohp_handle_t *hndl,
                                  odu_ohp_cfg_t *cfg_ptr,
                                  BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_state_t ohp_status = ODU_OHP_STATE_START;
    odu_ohp_port_cfg_t *port_cfg_a_ptr;
    odu_ohp_port_cfg_t *port_cfg_b_ptr;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);

    odu_ohp_ctxt_state_get(hndl, &ohp_status);

    if (ODU_OHP_STATE_OPERATIONAL == ohp_status)
    {
        *enable = TRUE;
    }
    else
    {
        *enable = FALSE;
    }

    if (TRUE == *enable)
    {
        port_cfg_a_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_A]);
        port_cfg_b_ptr = &(hndl->cfg.port_cfg[ODU_OHP_PORT_B]);

        cfg_ptr->ohp              = port_cfg_a_ptr->ohp;
        cfg_ptr->port_a_framer    = port_cfg_a_ptr->port_framer;
        cfg_ptr->port_a_src_addr  = port_cfg_a_ptr->src_user_id;
        cfg_ptr->port_a_dest_addr = port_cfg_a_ptr->dest_user_id;
        cfg_ptr->port_b_framer    = port_cfg_b_ptr->port_framer;
        cfg_ptr->port_b_src_addr  = port_cfg_b_ptr->src_user_id;
        cfg_ptr->port_b_dest_addr = port_cfg_b_ptr->dest_user_id;
    }

    PMC_RETURN(rc);
} /* odu_ohp_init_get */

/*******************************************************************************
* odu_ohp_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ODU_OHP 
*
* INPUTS:
*   *hndl                   - pointer to ODU_OHP handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if started. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL odu_ohp_start_state_test(odu_ohp_handle_t   *hndl)
{
    PMC_ENTRY();
 
    if (hndl->var.ctxt.state != ODU_OHP_STATE_START)
    {
        PMC_RETURN(FALSE);    
    }
    PMC_RETURN(TRUE);    
} /* odu_ohp_start_state_test */

/*******************************************************************************
* odu_ohp_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean register and memory context of specied channels. 
*       
* INPUTS:                                                                       
*  *hndl           - pointer to ODU_OHP handle instance
*   port           - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   *ch_entries    - Number of calendar entries per channel
*   *chnl          - array of channel ID to be cleaned
*   num_chnl       - number of channels to be cleaned
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
PUBLIC void odu_ohp_chnl_clean(odu_ohp_handle_t   *hndl,
                               odu_ohp_port_t port, 
                               UINT32 *ch_entries,
                               UINT32 *chnl,
                               UINT32 num_chnl)
{   
    UINT32 chnl_itr;
    UINT32 i;
    UINT8 *sch_ptr;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    PMC_ENTRY();

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);
    sch_ptr = port_cfg_ptr->schedule;
    
    cfc_fifo_clean(port_cfg_ptr->cfc_in,chnl,num_chnl);
    cfc_fifo_clean(port_cfg_ptr->cfc_ex,chnl,num_chnl);
    
    for (chnl_itr = 0; chnl_itr < num_chnl;chnl_itr++)
    {
        odu_ohp_reg_ch_cfg(hndl,port_cfg_ptr->port,chnl[chnl_itr],0,0,0,FALSE);

    }        
    
    /* update the schedule */
    for(i=0;i < ODU_OHP_SCHED_SIZE_MAX; i++)
    {
        if (ch_entries[i] > ODU_OHP_3_CH_MAX)
        {
            sch_ptr[i] = 0xff;
        } else 
        {
            sch_ptr[i] = ch_entries[i];
        }
    }
    (void) odu_ohp_reg_update_schedule(hndl,port_cfg_ptr->port,sch_ptr, TRUE); 
    
    PMC_RETURN();
} /* odu_ohp_chnl_clean */


/*******************************************************************************
* odu_ohp_int_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables unchannelized interrupts based on bits that are set in 
*   odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle  - Pointer to subsystem handle
*   int_table_ptr     - pointer to odu_ohp interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_enable(odu_ohp_handle_t    *odu_ohp_handle,
                                    odu_ohp_int_t       *int_table_ptr,
                                    odu_ohp_int_t       *int_en_table_ptr,
                                    BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* enable enet fege interrupts */
    enet_fege_int_enable(odu_ohp_handle->enet_fege_handle, &int_table_ptr->enet_fege, &int_en_table_ptr->enet_fege, enable);

    #ifdef ODU_OHP_INT
    #undef ODU_OHP_INT
    #endif
    #define ODU_OHP_INT     UTIL_GLOBAL_INT_ENABLE_SINGLE

    ODU_OHP_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* odu_ohp_int_enable */


/*******************************************************************************
* odu_ohp_int_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OHP unchannelized interrupts.
*   The desired interrupt/status bits to retrieve (to reduce function execution
*   time) can be specified using the filter interrupt table input.
*
* INPUTS:
*   odu_ohp_handle  - Pointer to subsystem handle
*   filt_table_ptr  - filter interrupt table. 
*                     When NULL, all interrupt are retrieved.
*
* OUTPUTS:
*   int_table_ptr     - pointer to odu_ohp interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_retrieve(odu_ohp_handle_t    *odu_ohp_handle,
                                      odu_ohp_int_t       *filt_table_ptr,
                                      odu_ohp_int_t       *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* retrieve enet fege interrupts */
    enet_fege_int_retrieve(odu_ohp_handle->enet_fege_handle, &filt_table_ptr->enet_fege, &int_table_ptr->enet_fege);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_ohp_handle, odu_ohp_core, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef ODU_OHP_INT
    #undef ODU_OHP_INT
    #endif
    #define ODU_OHP_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE   

    ODU_OHP_INT_TABLE_DEFINE();

     UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_ohp_handle, odu_ohp_core);

    PMC_RETURN(result);
} /* odu_ohp_int_retrieve */

/*******************************************************************************
* odu_ohp_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears OHP unchannelized interrupts based on bits that are set in 
*   odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle    - Pointer to subsystem handle
*   int_table_ptr     - pointer to odu_ohp interrupt table with
*                       bits set to clear interrupts
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_clear(odu_ohp_handle_t    *odu_ohp_handle,
                                   odu_ohp_int_t       *int_table_ptr,
                                   odu_ohp_int_t       *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    enet_fege_int_t *enet_fege_int_en_ptr = NULL;
    PMC_ENTRY();

    /* clear enet fege interrupts */
    if (NULL != int_en_table_ptr)
    {
        enet_fege_int_en_ptr = &int_en_table_ptr->enet_fege;
    }

        enet_fege_int_clear(odu_ohp_handle->enet_fege_handle, &int_table_ptr->enet_fege, enet_fege_int_en_ptr);

    #ifdef ODU_OHP_INT
    #undef ODU_OHP_INT
    #endif
    #define ODU_OHP_INT    UTIL_GLOBAL_INT_CLEAR_SINGLE

    ODU_OHP_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* odu_ohp_int_clear */

/*******************************************************************************
* odu_ohp_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables OHP channelized interrupts based on bits that are set in 
*   odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   int_table_ptr     - pointer to odu_ohp interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_chnl_enable(odu_ohp_handle_t    *odu_ohp_handle,
                                         UINT32               chnl_id,
                                         odu_ohp_int_chnl_t  *int_table_ptr,
                                         odu_ohp_int_chnl_t  *int_en_table_ptr,
                                         BOOL                 enable)
{
    cfc_int_chnl_t  cfc_int_table;
    cfc_int_chnl_t  cfc_int_en_table;
    PMC_ERROR result;

    PMC_ENTRY();

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_ohp_handle, odu_ohp_core, UTIL_GLOBAL_INT_OPTIMIZE);
    
    #ifdef ODU_OHP_CHNL_INT
    #undef ODU_OHP_CHNL_INT
    #endif
    #define ODU_OHP_CHNL_INT    UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE
        
    ODU_OHP_CHNL_INT_TABLE_DEFINE();
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_ohp_handle, odu_ohp_core);
    
    PMC_MEMSET((void *)&cfc_int_en_table, 0, sizeof(cfc_int_chnl_t));
    
    /* EX 0 */
    cfc_int_table.ipt_err_i = int_table_ptr->cfc_ex0_ipt_err_i;
    result = cfc_int_chnl_enable(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0], 
                                 chnl_id,
                                 &cfc_int_table,
                                 &cfc_int_en_table,
                                 enable);
    if (PMC_SUCCESS == result)
    {
        int_en_table_ptr->cfc_ex0_ipt_err_i = cfc_int_en_table.ipt_err_i;
        
        /* EX 1 */
        cfc_int_table.ipt_err_i = int_table_ptr->cfc_ex1_ipt_err_i;
        cfc_int_chnl_enable(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1], 
                            chnl_id,
                            &cfc_int_table,
                            &cfc_int_en_table,
                            enable);
    }
    
    if (PMC_SUCCESS == result)
    {
        int_en_table_ptr->cfc_ex1_ipt_err_i = cfc_int_en_table.ipt_err_i;
        
        /* IN 0 */
        cfc_int_table.ipt_err_i = int_table_ptr->cfc_in0_ipt_err_i;
        cfc_int_chnl_enable(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0], 
                            chnl_id,
                            &cfc_int_table,
                            &cfc_int_en_table,
                            enable);
    }
    
    if (PMC_SUCCESS == result)
    {
        int_en_table_ptr->cfc_in0_ipt_err_i = cfc_int_en_table.ipt_err_i;
        
        /* IN 1 */
        cfc_int_en_table.ipt_err_i = int_table_ptr->cfc_in1_ipt_err_i;
        cfc_int_chnl_enable(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1], 
                            chnl_id,
                            &cfc_int_table, 
                            &cfc_int_en_table,
                            enable);
    }
    if (PMC_SUCCESS == result)
    {
        int_en_table_ptr->cfc_in1_ipt_err_i = cfc_int_en_table.ipt_err_i;
    }
    
    PMC_RETURN(result);
} /* odu_ohp_int_chnl_enable */

/*******************************************************************************
* odu_ohp_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all channelized interrupts to the odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   filt_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - pointer to odu_ohp interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_chnl_retrieve(odu_ohp_handle_t    *odu_ohp_handle,
                                           UINT32               chnl_id,
                                           odu_ohp_int_chnl_t  *filt_table_ptr,
                                           odu_ohp_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    cfc_int_chnl_t  cfc_int_table;
    cfc_int_chnl_t  cfc_int_en_table;

    PMC_ENTRY();


    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_ohp_handle, odu_ohp_core, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef ODU_OHP_CHNL_INT
    #undef ODU_OHP_CHNL_INT
    #endif
    #define ODU_OHP_CHNL_INT    UTIL_GLOBAL_INT_LFIELD_RETRIEVE_SINGLE

    ODU_OHP_CHNL_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_ohp_handle, odu_ohp_core);

    if (NULL != filt_table_ptr)
    {
        PMC_MEMSET((void *)&cfc_int_en_table, 0, sizeof(cfc_int_chnl_t));
    }
    else
    {
        PMC_MEMSET((void *)&cfc_int_en_table, 0x1, sizeof(cfc_int_chnl_t));
    }
    
    /* EX 0 */
    if (NULL != filt_table_ptr)
    {
        cfc_int_en_table.ipt_err_i = filt_table_ptr->cfc_ex0_ipt_err_i;
    }
    cfc_int_chnl_retrieve(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0], 
                          chnl_id, 
                          &cfc_int_en_table,
                          &cfc_int_table);
    int_table_ptr->cfc_ex0_ipt_err_i = cfc_int_table.ipt_err_i;

    /* EX 1 */
    if (NULL != filt_table_ptr)
    {
        cfc_int_en_table.ipt_err_i = filt_table_ptr->cfc_ex1_ipt_err_i;
    }
    cfc_int_chnl_retrieve(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1], 
                          chnl_id, 
                          &cfc_int_en_table,
                          &cfc_int_table);
    int_table_ptr->cfc_ex1_ipt_err_i = cfc_int_table.ipt_err_i;

    /* IN 0 */
    if (NULL != filt_table_ptr)
    {
        cfc_int_en_table.ipt_err_i = filt_table_ptr->cfc_in0_ipt_err_i;
    }
    cfc_int_chnl_retrieve(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0], 
                          chnl_id, 
                          &cfc_int_en_table,
                          &cfc_int_table);
    int_table_ptr->cfc_in0_ipt_err_i = cfc_int_table.ipt_err_i;

    /* IN 1 */
    if (NULL != filt_table_ptr)
    {
        cfc_int_en_table.ipt_err_i = filt_table_ptr->cfc_in1_ipt_err_i;
    }
    cfc_int_chnl_retrieve(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1], 
                          chnl_id, 
                          &cfc_int_en_table,
                          &cfc_int_table);
    int_table_ptr->cfc_in1_ipt_err_i =cfc_int_table.ipt_err_i;

    PMC_RETURN(result);
} /* odu_ohp_int_chnl_retrieve */


/*******************************************************************************
* odu_ohp_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channelized interrupts based on bits that are set in 
*   odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   int_table_ptr     - pointer to odu_ohp interrupt table with
*                       bits set to clear interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_chnl_clear(odu_ohp_handle_t    *odu_ohp_handle,
                                        UINT32               chnl_id,
                                        odu_ohp_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    cfc_int_chnl_t  cfc_int_table;
    cfc_int_chnl_t  cfc_int_en_table;

    PMC_ENTRY();


    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_ohp_handle, odu_ohp_core, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef ODU_OHP_CHNL_INT
    #undef ODU_OHP_CHNL_INT
    #endif
    #define ODU_OHP_CHNL_INT    UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE

    ODU_OHP_CHNL_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_ohp_handle, odu_ohp_core);

    PMC_MEMSET((void *)&cfc_int_en_table, 0, sizeof(cfc_int_chnl_t));

    if (NULL == int_table_ptr)
    {
        cfc_int_en_table.ipt_err_i =  TRUE;
    }

    /* EX 0 */
    if (NULL != int_table_ptr)
    {
        cfc_int_en_table.ipt_err_i = int_table_ptr->cfc_ex0_ipt_err_i;
    }

    result = cfc_int_chnl_clear(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0], 
                       chnl_id, 
                       &cfc_int_en_table,
                       &cfc_int_table);

    if (PMC_SUCCESS == result)
    {
        /* EX 1 */    
        if (NULL != int_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_table_ptr->cfc_ex1_ipt_err_i;
        }
        
        result = cfc_int_chnl_clear(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1], 
                           chnl_id, 
                           &cfc_int_en_table,
                           &cfc_int_table);
    }

    if (PMC_SUCCESS == result)
    {
        /* IN 0 */
        if (NULL != int_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_table_ptr->cfc_in0_ipt_err_i;
        }
        result = cfc_int_chnl_clear(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0], 
                           chnl_id, 
                           &cfc_int_en_table,
                           &cfc_int_table);
    }

    if (PMC_SUCCESS == result)
    {
        /* IN 1 */
        if (NULL != int_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_table_ptr->cfc_in1_ipt_err_i;
        }
        result = cfc_int_chnl_clear(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1], 
                           chnl_id, 
                           &cfc_int_en_table,
                           &cfc_int_table);
    }

    PMC_RETURN(result);
} /* odu_ohp_int_chnl_clear */

/*******************************************************************************
* odu_ohp_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channelized interrupts based on bits that are set in 
*   odu_ohp interrupt table.
*   
*
* INPUTS:
*   odu_ohp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_int_chnl_enabled_check(odu_ohp_handle_t    *odu_ohp_handle,
                                                UINT32               chnl_id,
                                                odu_ohp_int_chnl_t  *int_en_table_ptr,
                                                BOOL8               *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    cfc_int_chnl_t  cfc_int_en_table;

    PMC_ENTRY();


    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_ohp_handle, odu_ohp_core, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef ODU_OHP_CHNL_INT
    #undef ODU_OHP_CHNL_INT
    #endif
    #define ODU_OHP_CHNL_INT    UTIL_GLOBAL_INT_LFIELD_CHECK_SINGLE

    ODU_OHP_CHNL_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_ohp_handle, odu_ohp_core);

    if (FALSE == *int_found_ptr)
    {
        PMC_MEMSET((void *)&cfc_int_en_table, 0, sizeof(cfc_int_chnl_t));
        
        if (NULL == int_en_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = TRUE;
        }
        
        /* EX 0 */
        if (NULL != int_en_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_en_table_ptr->cfc_ex0_ipt_err_i;
        }
        
        result = cfc_int_chnl_enabled_check(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX0], 
                                   chnl_id, 
                                   &cfc_int_en_table,
                                   (BOOL8 *)int_found_ptr);
        
    }
    
    /* EX 1 */    
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        if (NULL != int_en_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_en_table_ptr->cfc_ex1_ipt_err_i;
        }
        
        result = cfc_int_chnl_enabled_check(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_EX1], 
                                   chnl_id, 
                                   &cfc_int_en_table,
                                   (BOOL8 *)int_found_ptr);
    }
    
    /* IN 0 */
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        if (NULL != int_en_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_en_table_ptr->cfc_in0_ipt_err_i;
        }

        result = cfc_int_chnl_enabled_check(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN0], 
                                   chnl_id, 
                                   &cfc_int_en_table,
                                   (BOOL8 *)int_found_ptr);
    }
    
    /* IN 1 */
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        if (NULL != int_en_table_ptr)
        {
            cfc_int_en_table.ipt_err_i = int_en_table_ptr->cfc_in1_ipt_err_i;
        }

        result = cfc_int_chnl_enabled_check(odu_ohp_handle->cfc_handle[ODU_OHP_CFC_IN1], 
                                            chnl_id, 
                                            &cfc_int_en_table,
                                            (BOOL8 *)int_found_ptr);
    }

    PMC_RETURN(result);
} /* odu_ohp_int_chnl_enabled_check */


/*******************************************************************************
* odu_ohp_port_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize an OHP port.

*
* INPUTS:
*   *hndl      -  pointer to OHP handle instance
*   *port_ptr  -  pointer to OHP Port configuration
*   max_ch     -  maximum number of channels (12 for OHP1, 96 for OHP2/3)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_port_init(odu_ohp_handle_t *hndl, 
                                    odu_ohp_port_cfg_t *port_ptr, 
                                    UINT32 max_ch)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_t port;
    BOOL port_en;
    UINT32 sd_pcc = 0; /* disable shutdown */
    UINT32 sch_en = 0; /* disable schedule */
    UINT32 ch_en = 0;  /* disable channels */
    UINT32 i;

    PMC_ENTRY();    
    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != port_ptr, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
    
    port = port_ptr->port; 


    port_en = (port_ptr->port_framer != ODU_OHP_PORT_UNUSED);

    PMC_LOG_TRACE("OHP: port init: porta en = %d,ch en=%d sd_pcc=%d sch_en=%d\n",
            port_en,ch_en,sd_pcc,sch_en);

    rc = odu_ohp_reg_port_cfg(hndl, port, sd_pcc,sch_en,
                              port_ptr->src_user_id,port_ptr->dest_user_id,
                              max_ch, ch_en);
    

    for(i=0;i < ODU_OHP_SCHED_SIZE_MAX;i++)
    {
        port_ptr->schedule[i] = 0xff;
    }
    if (rc == PMC_SUCCESS)
    {
        rc = odu_ohp_reg_update_schedule(hndl, port, port_ptr->schedule, TRUE);  
    }
    /* Disable all channels */
    for(i=0; i < max_ch;i++)
    {
        odu_ohp_reg_ch_cfg(hndl,port,i, 0,0,0,FALSE); /* disable ch */
    }
    PMC_RETURN(rc);
} /*odu_ohp_port_init*/

/*******************************************************************************
* odu_ohp_ch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable overhead extraction/insertion for the given channel.

*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*   extract_mask     - Extraction mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word should be 
*                      extracted. Exactly 10 bits must be set to 1
*                      (corresponding to the extraction of 10 words). 
*   insert_mask      - Insertion mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word will be 
*                      inserted. A maximum of 10 bits can be set to 1.
*                      (corresponding to the insertion of 10 words).
*   num_cal_entries  - Number of calendar entries
*   cal_entries      - Calendar 
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_cfg(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id, 
                                UINT16 extract_mask,
                                UINT16 insert_mask,
                                UINT32 num_cal_entries,
                                UINT32 *cal_entries,
                                BOOL enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 cfc_num_pages = 0;
    UINT8 *sch_ptr;
    UINT32 slot,i,tx_offset=0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);

    PMC_LOG_TRACE("ohp: ch_cfg: port = %x ch %x ext %x ins %x enable =%d\n",
               port,ch_id,extract_mask,insert_mask,enable);
  
    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        PMC_RETURN(ODU_OHP_ERR_STATE_NOT_OPERATIONAL);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    /* return an error if trying to disable a channel that has not been previously enabled */
    if (!enable && !odu_ohp_ch_en_get(hndl,port,ch_id)) 
    {
        PMC_RETURN(ODU_OHP_ERR_CH_NOT_ENABLED);
    }

    /* return an error if trying to enable a channel that has already been enabled */
    if (enable && odu_ohp_ch_en_get(hndl,port,ch_id)) 
    {
        PMC_RETURN(ODU_OHP_ERR_CH_ALREADY_ENABLED);
    }

    PMC_LOG_TRACE("OHP: ch_cfg: ch id: %d, framer=%d, enable=%d,num_cal_entries=%d\n",ch_id,
                port_cfg_ptr->port_framer,enable,num_cal_entries);


    sch_ptr = port_cfg_ptr->schedule;
    
    /* update the schedule */
    for(i=0;i < num_cal_entries;i++)
    {
        slot = cal_entries[i];
        sch_ptr[slot] = (enable ? ch_id : 0xff);
    }

    rc = odu_ohp_reg_update_schedule(hndl,port_cfg_ptr->port,sch_ptr,TRUE); 
    
    if (rc != PMC_SUCCESS)
    {
        PMC_RETURN(rc);
    }

    /* calculate the number of CFC FIFO pages and the TX offset */
    if (TRUE == enable)
    {
        if (port_cfg_ptr->ohp == ODU_OHP_1)
        {
            /* OHP1 connects to  12 channel framers (stage_1 or stage_n) - ODU2, ODU3 or ODU4 */
            PMC_ASSERT((num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80),ODU_OHP_ERR_INVALID_SCHED,0,0);

            /* CFC number of pages */
            cfc_num_pages = num_cal_entries/8;

            /* tx_offst: 12 ch OHP: ODU2 -> tx_offset=8, ODU3 -> tx_offset=32, ODU4 -> tx_offset=64 */
            if (num_cal_entries == 8 || num_cal_entries == 32)
            {
                tx_offset = num_cal_entries;
            }
            else 
            {
                tx_offset = 64; /* ODU4 */
            }
        }
        else 
        {
            /* OHP2 and OHP3 - 96 channels */
            /* cfc_num_pages = num_cal_entries * 2 */
            cfc_num_pages = num_cal_entries; 
            /* tx_offset: ODU0 -> tx_offset=8, ODU1->tx_offset=16, ODU2->64, ODU3->256, ODU4->256 */
            if (num_cal_entries < 2)
                tx_offset = 8;
            else if (num_cal_entries < 8)
                tx_offset = 16;
            else if (num_cal_entries < 32)
                tx_offset = 64;
            else 
                tx_offset = 256;
        }
        
        PMC_LOG_TRACE("OHP: ohp=%u, enable=%d, num_cal_entires = %u,tx_offset=%u\n", port_cfg_ptr->ohp,enable,num_cal_entries,tx_offset);
    } 
    
    /* Configure CFC FIFOs */
    if (TRUE == enable)
    {        
      
        rc = cfc_fifo_build(port_cfg_ptr->cfc_in,ch_id,cfc_num_pages);
        if (PMC_SUCCESS == rc)
        {
            rc = cfc_fifo_activate(port_cfg_ptr->cfc_in,ch_id);
        }
        if (PMC_SUCCESS == rc)
        {
            rc = cfc_fifo_build(port_cfg_ptr->cfc_ex,ch_id,cfc_num_pages);
        }
        if (PMC_SUCCESS == rc)
        {
            rc = cfc_fifo_activate(port_cfg_ptr->cfc_ex,ch_id);
        }
        odu_ohp_reg_ch_cfg(hndl,port_cfg_ptr->port,ch_id, extract_mask,insert_mask,tx_offset,enable); 
    } 
    else 
    {
        odu_ohp_reg_ch_cfg(hndl,port_cfg_ptr->port,ch_id, 0,0,0,FALSE); /* disable ch */
        rc = cfc_fifo_deactivate(port_cfg_ptr->cfc_in,ch_id);
        rc |= cfc_fifo_deactivate(port_cfg_ptr->cfc_ex,ch_id);
        rc |= cfc_fifo_destroy(port_cfg_ptr->cfc_in,ch_id);
        rc |= cfc_fifo_destroy(port_cfg_ptr->cfc_ex,ch_id);
    }

    PMC_RETURN(rc);
} /* odu_ohp_ch_cfg */

/*******************************************************************************
* odu_ohp_ch_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets overhead extraction/insertion for the given channel.
*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*
* OUTPUTS:
*   extract_mask     - Extraction mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word should be 
*                      extracted. Exactly 10 bits must be set to 1
*                      (corresponding to the extraction of 10 words). 
*   insert_mask      - Insertion mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word will be 
*                      inserted. A maximum of 10 bits can be set to 1.
*                      (corresponding to the insertion of 10 words).
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_cfg_get(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id, 
                                UINT16 *extract_mask,
                                UINT16 *insert_mask,
                                BOOL *enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 mask = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
 
    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        PMC_RETURN(ODU_OHP_ERR_STATE_NOT_OPERATIONAL);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    *enable = odu_ohp_ch_en_get(hndl, port, ch_id);

    mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, TRUE);
    *insert_mask = (UINT16)(mask&0xFFFF);
    
    mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, FALSE);
    *extract_mask = (UINT16)(mask&0xFFFF);

    PMC_RETURN(rc);
} /* odu_ohp_ch_cfg_get */

/*******************************************************************************
* odu_ohp_ch_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable overhead insertion for the given channel.

*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*   insert_mask      - Insertion mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word will be 
*                      inserted. A maximum of 10 bits can be set to 1.
*                      (corresponding to the insertion of 10 words).
*   num_cal_entries  - Number of calendar entries
*   cal_entries      - Calendar 
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_insert_cfg(odu_ohp_handle_t *hndl, 
                                       odu_ohp_port_t port, 
                                       UINT32 ch_id, 
                                       UINT16 insert_mask,
                                       UINT32 num_cal_entries,
                                       UINT32 *cal_entries,
                                       BOOL enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 cfc_num_pages = 0;
    UINT32 mask,tx_offset=0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);

    PMC_LOG_TRACE("ohp: ch_cfg: port = %x ch %x ins %x enable =%d\n",
               port,ch_id,insert_mask,enable);
  
    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        PMC_RETURN(ODU_OHP_ERR_STATE_NOT_OPERATIONAL);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    /* return an error if trying to disable a channel that has not been previously enabled */
    if (!enable && !odu_ohp_ch_en_get(hndl,port,ch_id)) 
    {
        PMC_RETURN(ODU_OHP_ERR_CH_NOT_ENABLED);
    }

    PMC_LOG_TRACE("OHP: insert_cfg: ch id: %d, framer=%d, enable=%d,num_cal_entries=%d\n",ch_id,
                port_cfg_ptr->port_framer,enable,num_cal_entries);

    if (enable == TRUE)
    {
        /* calculate the number of CFC FIFO pages and the TX offset */
        if (port_cfg_ptr->ohp == ODU_OHP_1)
        {
            /* OHP1 connects to  12 channel framers (stage_1 or stage_n) - ODU2, ODU3 or ODU4 */
            PMC_ASSERT((num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80),ODU_OHP_ERR_INVALID_SCHED,0,0);

            /* CFC number of pages */
            cfc_num_pages = num_cal_entries/8;

            /* tx_offst: 12 ch OHP: ODU2 -> tx_offset=8, ODU3 -> tx_offset=32, ODU4 -> tx_offset=64 */
            if (num_cal_entries == 8 || num_cal_entries == 32)
            {
                tx_offset = num_cal_entries;
            }
            else 
            {
                tx_offset = 64; /* ODU4 */
            }
        }
        else 
        {
            /* OHP2 and OHP3 - 96 channels */
            /* cfc_num_pages = num_cal_entries * 2 */
            cfc_num_pages = num_cal_entries; 
            /* tx_offset: ODU0 -> tx_offset=8, ODU1->tx_offset=16, ODU2->64, ODU3->256, ODU4->256 */
            if (num_cal_entries < 2)
                tx_offset = 8;
            else if (num_cal_entries < 8)
                tx_offset = 16;
            else if (num_cal_entries < 32)
                tx_offset = 64;
            else 
                tx_offset = 256;
        }
        
        PMC_LOG_TRACE("OHP: ohp=%u, enable=%d, num_cal_entires = %u,tx_offset=%u\n", port_cfg_ptr->ohp,enable,num_cal_entries,tx_offset);

        rc = odu_ohp_schedule_add_ch(hndl,port_cfg_ptr->port,ch_id, num_cal_entries, cal_entries);
        if (rc == PMC_SUCCESS)
        {
            rc = cfc_fifo_build(port_cfg_ptr->cfc_in,ch_id,cfc_num_pages);
        }
        if (rc == PMC_SUCCESS)
        {
            rc = cfc_fifo_activate(port_cfg_ptr->cfc_in,ch_id);
        }
        odu_ohp_ch_tx_cfg(hndl,port_cfg_ptr->port,ch_id, insert_mask,tx_offset); 
        odu_ohp_ch_en_set(hndl, port_cfg_ptr->port, ch_id, 1); 
    } 
    else 
    {
        /* Disable Insertion */
        odu_ohp_ch_tx_cfg(hndl,port_cfg_ptr->port,ch_id, 0, 0); 

        /* Need to determine if rx is still active - check extract mask */
        mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, FALSE);

        if (mask == 0)
        {
            /* RX has already been disabled - remove the schedule entires for the channel */
            rc = odu_ohp_schedule_remove_ch(hndl, port_cfg_ptr->port, ch_id);

            odu_ohp_ch_en_set(hndl, port_cfg_ptr->port, ch_id, 0); 
        }
        rc = cfc_fifo_deactivate(port_cfg_ptr->cfc_in,ch_id);
        rc |= cfc_fifo_destroy(port_cfg_ptr->cfc_in,ch_id);
    }

    PMC_RETURN(rc);
} /* odu_ohp_ch_insert_cfg */

/*******************************************************************************
* odu_ohp_ch_insert_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets overhead insertion for the given channel.
*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*
* OUTPUTS:
*   insert_mask      - Insertion mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word will be 
*                      inserted. A maximum of 10 bits can be set to 1.
*                      (corresponding to the insertion of 10 words).
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_insert_cfg_get(odu_ohp_handle_t *hndl, 
                                           odu_ohp_port_t port, 
                                           UINT32 ch_id, 
                                           UINT16 *insert_mask,
                                           BOOL *enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 mask = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);

    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        *enable = FALSE;
        PMC_RETURN(rc);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, TRUE);
    *insert_mask = (UINT16)(mask&0xFFFF);

    if (0 == *insert_mask)
    {
        *enable = FALSE;
    }
    else
    {
        *enable = TRUE;
    }

    PMC_RETURN(rc);
} /* odu_ohp_ch_insert_cfg_get */

/*******************************************************************************
* odu_ohp_ch_extract_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable overhead extraction for the given channel.

*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*   extract_mask     - Extraction mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word should be 
*                      extracted. Exactly 10 bits must be set to 1
*                      (corresponding to the extraction of 10 words). 
*   num_cal_entries  - Number of calendar entries
*   cal_entries      - Calendar 
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_extract_cfg(odu_ohp_handle_t *hndl, 
                                        odu_ohp_port_t port, 
                                        UINT32 ch_id, 
                                        UINT16 extract_mask,
                                        UINT32 num_cal_entries,
                                        UINT32 *cal_entries,
                                        BOOL enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 cfc_num_pages = 0;
    UINT32 mask;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);

    PMC_LOG_TRACE("ohp: ch_cfg: port = %x ch %x ext %x enable =%d\n",
               port,ch_id,extract_mask,enable);
  
    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        PMC_RETURN(ODU_OHP_ERR_STATE_NOT_OPERATIONAL);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    /* return an error if trying to disable a channel that has not been previously enabled */
    if (!enable && !odu_ohp_ch_en_get(hndl,port,ch_id)) 
    {
        PMC_RETURN(ODU_OHP_ERR_CH_NOT_ENABLED);
    }

    PMC_LOG_TRACE("OHP: extract_cfg: ch id: %d, framer=%d, enable=%d,num_cal_entries=%d\n",ch_id,
                port_cfg_ptr->port_framer,enable,num_cal_entries);

    if (enable == TRUE)
    {
        /* calculate the number of CFC FIFO pages and the TX offset */
        if (port_cfg_ptr->ohp == ODU_OHP_1)
        {
            /* OHP1 connects to  12 channel framers (stage_1 or stage_n) - ODU2, ODU3 or ODU4 */
            PMC_ASSERT((num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80),ODU_OHP_ERR_INVALID_SCHED,0,0);

            /* CFC number of pages */
            cfc_num_pages = num_cal_entries/8;
        }
        else 
        {
            /* OHP2 and OHP3 - 96 channels */
            /* cfc_num_pages = num_cal_entries * 2 */
            cfc_num_pages = num_cal_entries; 
        }
        
        PMC_LOG_TRACE("OHP: ohp=%u, enable=%d, num_cal_entires = %u\n", port_cfg_ptr->ohp,enable,num_cal_entries);
        rc = odu_ohp_schedule_add_ch(hndl,port_cfg_ptr->port,ch_id, num_cal_entries, cal_entries);
        if (rc == PMC_SUCCESS)
        {
            rc = cfc_fifo_build(port_cfg_ptr->cfc_ex,ch_id,cfc_num_pages);
        }
        if (rc == PMC_SUCCESS)
        {
            rc = cfc_fifo_activate(port_cfg_ptr->cfc_ex,ch_id);
        }
        odu_ohp_ch_rx_cfg(hndl,port_cfg_ptr->port,ch_id, extract_mask); 
        odu_ohp_ch_en_set(hndl,port_cfg_ptr->port,ch_id,1); 
    } 
    else 
    {
        /* Disable extraction */
        odu_ohp_ch_rx_cfg(hndl,port_cfg_ptr->port,ch_id, 0); /* set extract mask = 0 */

        /* Need to determine if tx is still active - check insertion mask */
        mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, TRUE);

        if (mask == 0)
        {
            /* TX has already been disabled - remove the schedule entires for the channel */
            rc = odu_ohp_schedule_remove_ch(hndl, port_cfg_ptr->port, ch_id);
            odu_ohp_ch_en_set(hndl, port_cfg_ptr->port, ch_id, 0); 
        }
        rc = cfc_fifo_deactivate(port_cfg_ptr->cfc_ex,ch_id);
        rc |= cfc_fifo_destroy(port_cfg_ptr->cfc_ex,ch_id);
    }

    PMC_RETURN(rc);
} /* odu_ohp_ch_extract_cfg */

/*******************************************************************************
* odu_ohp_ch_extract_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets overhead extraction for the given channel.
*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*
* OUTPUTS:
*   extract_mask     - Extraction mask (16 bits). Each bit indicates 
*                      whether the corresponding overhead word should be 
*                      extracted. Exactly 10 bits must be set to 1
*                      (corresponding to the extraction of 10 words). 
*   enable           - Enable (when TRUE) or disable (when FALSE) the overhead 
*                      insertion/extraction for this channel.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_extract_cfg_get(odu_ohp_handle_t *hndl, 
                                            odu_ohp_port_t port, 
                                            UINT32 ch_id, 
                                            UINT16 *extract_mask,
                                            BOOL *enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 mask = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
 
    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        *enable = FALSE;
        PMC_RETURN(rc);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    mask = odu_ohp_mask_get(hndl, port_cfg_ptr->port, ch_id, FALSE);
    *extract_mask = (UINT16)(mask&0xFFFF);

    if (0 == *extract_mask)
    {
        *enable = FALSE;
    }
    else
    {
        *enable = TRUE;
    }

    PMC_RETURN(rc);
} /* odu_ohp_ch_extract_cfg_get */

/*******************************************************************************
* odu_ohp_schedule_add_ch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable overhead insertion for the given channel.

*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*   num_cal_entries  - Number of calendar entries
*   cal_entries      - Calendar 

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_schedule_add_ch(odu_ohp_handle_t *hndl, 
                                          odu_ohp_port_t port, 
                                          UINT32 ch_id, 
                                          UINT32 num_cal_entries,
                                          UINT32 *cal_entries)

{
    PMC_ERROR result = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr = NULL;
    UINT8 *sch_ptr = NULL;
    UINT32 ch_id_entries = 0;
    UINT32 sched_entries = 0;
    UINT32 free_entries = 0;
    UINT32 slot,i;

    PMC_ENTRY();

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);
    sch_ptr = port_cfg_ptr->schedule;

    /* Check if channel id is already scheduled */
    for(i=0;  i < ODU_OHP_SCHED_SIZE_MAX; i++)
    {
        if (sch_ptr[i] == ch_id)
            ch_id_entries++;
        else if (sch_ptr[i] == 0xff)
            free_entries++;
    }
    /* Check if there's enough entries already */
    if (ch_id_entries == num_cal_entries)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    /* If the number of entries is > 0, the rx and tx rates are mismatched */
    if (ch_id_entries > 0)
    {
        PMC_RETURN(ODU_OHP_ERR_MISMATCHED_RATE);
    }
    /* If there's not enough free entries - return an error */
    if (free_entries < num_cal_entries)
    {
        PMC_RETURN(ODU_OHP_ERR_SCHEDULE_FULL);
    }

    /* Try to use same entries as requested */
    for(i=0;i < num_cal_entries;i++)
    {
        slot = cal_entries[i];
        if (sch_ptr[slot] == 0xff)
        {
            sch_ptr[slot] = ch_id;
            sched_entries++;
        }
    }
    /* Add remaining entries - if needed */
    if (sched_entries < num_cal_entries)
    {
        for(i=0; i < ODU_OHP_SCHED_SIZE_MAX; i++)
        {
            if (sch_ptr[i] == 0xff)
            {
                sch_ptr[i] = ch_id;
                if (++sched_entries == num_cal_entries)
                    break;
            }    
        }
    }
    result = odu_ohp_reg_update_schedule(hndl,port_cfg_ptr->port,sch_ptr,TRUE); 
    PMC_RETURN(result);
}  /* odu_ohp_schedule_add_ch */

/*******************************************************************************
* odu_ohp_schedule_remove_ch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable overhead insertion for the given channel.

*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_schedule_remove_ch(odu_ohp_handle_t *hndl, 
                                             odu_ohp_port_t port, 
                                             UINT32 ch_id)

{
    PMC_ERROR result = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr = NULL;
    UINT8 *sch_ptr = NULL;
    UINT32 i;

    PMC_ENTRY();

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);
    sch_ptr = port_cfg_ptr->schedule;

    /* Check if channel id is already scheduled */
    for(i=0;  i < ODU_OHP_SCHED_SIZE_MAX; i++)
    {
        if (sch_ptr[i] == ch_id)
            sch_ptr[i] = 0xff;
    }
    
    result = odu_ohp_reg_update_schedule(hndl,port_cfg_ptr->port,sch_ptr,TRUE); 
    PMC_RETURN(result);
} /* odu_ohp_schedule_remove_ch */



/*******************************************************************************
* odu_ohp_ch_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize OHP for a channel

*
* INPUTS:
*  *hndl            - pointer to ODU_OHP handle instance
*  port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*  ch_id            - channel id
*  resize           - amount to resize: +ve grow, -ve shrink

*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ohp_ch_resize(odu_ohp_handle_t *hndl, 
                                   odu_ohp_port_t port, 
                                   UINT32 ch_id, 
                                   INT32 resize)
{

    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 cfc_num_pages_end, cfc_num_pages_start;
    UINT32 num_cal_entries_start=0,num_cal_entries_end=0,tx_offset=0;
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_OHP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(port < ODU_OHP_PORT_LAST, ODU_OHP_ERR_INVALID_ARGS, 0, 0);


    if (ODU_OHP_STATE_OPERATIONAL != hndl->var.ctxt.state)
    {
        PMC_RETURN(ODU_OHP_ERR_STATE_NOT_OPERATIONAL);
    }

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

    PMC_LOG_TRACE("ohp: resize: ch = %d port = %d framer = %d resize =%d\n",
                ch_id,port,port_cfg_ptr->port_framer,resize);


    /* No resizing of HO channels */
    PMC_ASSERT(port_cfg_ptr->ohp != ODU_OHP_1, ODU_OHP_ERR_INVALID_ARGS,ODU_OHP_1,0);

    /* Check for invalid port */
    if (port_cfg_ptr->port_framer == ODU_OHP_PORT_UNUSED)
    {
        PMC_RETURN(ODU_OHP_ERR_PORT_NOT_CONFIG);
    }

    /* return an error if trying to resize a disabled channel */
    if (!odu_ohp_ch_en_get(hndl,port,ch_id)) 
    {
        PMC_RETURN(ODU_OHP_ERR_CH_NOT_ENABLED);
    }

    num_cal_entries_start = odu_ohp_schedule_resize(hndl, port, ch_id, resize);

    num_cal_entries_end = num_cal_entries_start + resize; /* note: resize is -ve if shrinking */
    PMC_ASSERT((num_cal_entries_end > 0 && num_cal_entries_end < ODU_OHP_SCHED_SIZE_MAX),
                ODU_OHP_ERR_INVALID_ARGS,num_cal_entries_end,resize);


    /* calculate the number of CFC FIFO pages and the TX offset */
    
    cfc_num_pages_end = num_cal_entries_end;
    cfc_num_pages_start = num_cal_entries_start;
    /* tx_offset: ODU0 (1) -> tx_offset=8, ODU1(2) ->tx_offset=16, ODU2(8)->64, ODU3(32)->256, ODU4(80)->256 */
    /* if (num_cal_entries_end < 2) */
    if (num_cal_entries_end < 2)
        tx_offset = 8;
    else if (num_cal_entries_end < 8)
        tx_offset = 16;
    else if (num_cal_entries_end < 32)
        tx_offset = 64;
    else 
        tx_offset = 256;
    
    PMC_LOG_TRACE("OHP: ohp=%u, num_cal_entries_start = %u num_cal_entries_end = %u,tx_offset=%u\n", 
            port_cfg_ptr->ohp,num_cal_entries_start,num_cal_entries_end,tx_offset);
    
    /* Configure CFC FIFOs */

    /* Shrinking -> update TXFA_INS first */
    if ((num_cal_entries_end < num_cal_entries_start) && rc == PMC_SUCCESS) 
    {
        UINT32 cur_tx_offset;
        UINT32 us_delay = 200 * 1000;

        cur_tx_offset = odu_ohp_tx_offset_get(hndl,port,ch_id);
        /* offset can be 8, 16, 64 or 256 */
        
        while (cur_tx_offset > tx_offset)
        {     
            cur_tx_offset -= 8;
            PMC_ASSERT(cur_tx_offset > 0, ODU_OHP_ERR_INVALID_ARGS, cur_tx_offset,0);
            odu_ohp_tx_offset_set(hndl,port,ch_id, cur_tx_offset);
            PMC_Q_USLEEP(hndl,us_delay);
        }
         
        if (rc != PMC_SUCCESS)
        {
            PMC_RETURN(rc);
        }

        PMC_LOG_TRACE("ohp: cfc in resize down, port = %d ch = %d framer %d pages =%d\n",
                port,ch_id,port_cfg_ptr->port_framer,cfc_num_pages_end);
        /* 
         *  if the final size is 1 - reset the fifo.
         *  
         */         
        if (cfc_num_pages_end == 1)
        { 
            if (rc == PMC_SUCCESS)
            {
                odu_ohp_ch_en_set(hndl, port, ch_id, 0); 
                rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_in,ch_id,1,ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
                odu_ohp_ch_en_set(hndl, port, ch_id, 1); 
            }

            /* Resize extraction fifo */
            if (rc == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("ohp: cfc ex resize down port = %d ch = %d pages =%d\n",port,ch_id,cfc_num_pages_end);
                rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_ex,ch_id,cfc_num_pages_end,ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
            }
            if (rc == PMC_SUCCESS) 
            {
                rc = odu_ohp_sched_page_switch(hndl, port);
            }

        }
        else
        {
            /* Resize insertion fifo */
            rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_in,ch_id,cfc_num_pages_end,ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
            /* Resize extraction fifo */
            if (rc == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("ohp: cfc ex resize down port = %d ch = %d pages =%d\n",port,ch_id,cfc_num_pages_end);
                rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_ex,ch_id,cfc_num_pages_end,ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
            }
            if (rc == PMC_SUCCESS) 
            {
                rc = odu_ohp_sched_page_switch(hndl, port);
            }
        }
    }
    else 
    { 
        PMC_LOG_TRACE("ohp: cfc in resize up port = %d ch = %d pages =%d\n",port,ch_id,cfc_num_pages_end);

        /* Growing: If initial size is 1, reset the fifo to resize  */
        if(cfc_num_pages_start == 1) 
        {
           
            if (rc == PMC_SUCCESS)
            {
                odu_ohp_ch_en_set(hndl, port, ch_id, 0); 
                rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_in,ch_id,cfc_num_pages_end, ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
                odu_ohp_ch_en_set(hndl, port, ch_id, 1); 
            }

        }
        else
        {
            rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_in,ch_id,cfc_num_pages_end, ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
        }

        /* Resize the extraction fifo */
        if (PMC_SUCCESS == rc) 
        {
            PMC_LOG_TRACE("ohp: -  cfc ex resize up port = %d ch = %d framer = %d pages =%d\n",
                                                port,ch_id,port_cfg_ptr->port_framer,cfc_num_pages_end);
            rc = cfc_fifo_resize_ext(port_cfg_ptr->cfc_ex,ch_id,cfc_num_pages_end, ODU_OHP_CFC_FIFO_POLLING_INTERVAL, ODU_OHP_CFC_FIFO_POLLING_ITERATIONS);
        }

        /* Increasing -> update TXFA_INS first */
        if (rc == PMC_SUCCESS) 
        {
            odu_ohp_tx_offset_set(hndl,port,ch_id, tx_offset);
        }

        if (rc == PMC_SUCCESS) 
        {
            rc = odu_ohp_sched_page_switch(hndl, port);
        }
    } /* ramp up */
    
    PMC_RETURN(rc);
} /* odu_ohp_ch_resize */


/*******************************************************************************
*  odu_ohp_schedule_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize the schedule
*
* INPUTS:
*   *hndl            - pointer to ODU_OHP handle instance to be operated on 
*   port             - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*   ch_id            - channel id
*   resize           - amount to resize: +ve grow, -ve shrink
*    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Initial number of calendar entries.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_ohp_schedule_resize(odu_ohp_handle_t *hndl, 
                                       odu_ohp_port_t port, 
                                       UINT32 ch_id, 
                                       INT32 resize)
{

    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT8 *sch_ptr;
    UINT32 num_cal_entries_start=0,num_cal_entries_end=0,i;

    PMC_ENTRY();

    /* Count number of slots for channel */
    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);
    sch_ptr = port_cfg_ptr->schedule;
    for(i=0;  i < ODU_OHP_SCHED_SIZE_MAX; i++)
    {
        if (sch_ptr[i] == ch_id) 
            num_cal_entries_start++;
    }
    num_cal_entries_end = num_cal_entries_start + resize; /* note: resize is -ve if shrinking */
    PMC_ASSERT((num_cal_entries_end > 0 && num_cal_entries_end < ODU_OHP_SCHED_SIZE_MAX),
                ODU_OHP_ERR_INVALID_ARGS,num_cal_entries_end,resize);

    /* Resize the schedule */
    for(i=0;i < ODU_OHP_SCHED_SIZE_MAX && resize != 0;i++)
    {
        /* If growing - add entries */
        if (resize > 0 && sch_ptr[i] == 0xff) 
        {
            sch_ptr[i] = ch_id;
            resize--;
        }
        /* If shrinking - remove entries */
        else if (resize < 0 && sch_ptr[i] == ch_id) 
        {
            sch_ptr[i] = 0xff;
            resize++;
        }
    }
    /* Can't happen */
    PMC_ASSERT(resize == 0,ODU_OHP_ERR_INVALID_ARGS,num_cal_entries_end,resize);

    /* Resize the schedule - but don't do the page switch yet */
    (void)odu_ohp_reg_update_schedule(hndl,port_cfg_ptr->port,sch_ptr,FALSE); 
    
    PMC_RETURN(num_cal_entries_start);
} /* odu_ohp_schedule_resize */


/*******************************************************************************
*  odu_ohp_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes ODU_OHP var_t variables.
*
* INPUTS:
*   *hndl             - pointer to ODU_OHP handle instance to be operated on 
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
PRIVATE void odu_ohp_var_default_init(odu_ohp_handle_t   *hndl)
{    
    UINT32 port_no,i;
    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
   
    odu_ohp_ctxt_state_set(hndl, ODU_OHP_STATE_START);


    for(port_no=0;port_no < ODU_OHP_PORT_MAX;port_no++)
    {
        hndl->cfg.port_cfg[port_no].ohp = ODU_OHP_LAST;
        hndl->cfg.port_cfg[port_no].port = ODU_OHP_PORT_LAST;
        hndl->cfg.port_cfg[port_no].port_framer = ODU_OHP_PORT_UNUSED;
        hndl->cfg.port_cfg[port_no].src_user_id = 0;
        hndl->cfg.port_cfg[port_no].dest_user_id = 0;
        PMC_MEMSET(hndl->cfg.port_cfg[port_no].schedule,0xff,sizeof(hndl->cfg.port_cfg[port_no].schedule));
        for(i=0;i < ODU_OHP_SCHED_SIZE_MAX;i++)
        {
            hndl->cfg.port_cfg[port_no].schedule[i] = 0xff;
        }
    }

    PMC_RETURN();
} /* odu_ohp_var_default_init */

/*******************************************************************************
*  odu_ohp_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets ODU OHP instance state.
*
* INPUTS:
*   *hndl             - ODU OHP handle
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
PRIVATE void odu_ohp_ctxt_state_set(odu_ohp_handle_t      *hndl,
                                    odu_ohp_state_t        state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ODU_OHP_ERR_INVALID_ARGS, 0, 0);
    
    hndl->var.ctxt.state = state;
    
    PMC_RETURN();
}

/*******************************************************************************
*  odu_ohp_ctxt_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets ODU OHP instance state.
*
* INPUTS:
*   *hndl             - ODU OHP handle
*
* OUTPUTS:
*    state            - provisioning state
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odu_ohp_ctxt_state_get(odu_ohp_handle_t      *hndl,
                                    odu_ohp_state_t       *state)
{
    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ODU_OHP_ERR_INVALID_ARGS, 0, 0);

    *state = hndl->var.ctxt.state;

    PMC_RETURN();
}

/*******************************************************************************
*  odu_ohp_ctxt_state_transition_request
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks that ODU OHP state transition is allowed.
*
* INPUTS:
*   *hndl             - pointer to ODU OHP handle instance to be operated on  
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
PRIVATE PMC_ERROR odu_ohp_ctxt_state_transition_request(odu_ohp_handle_t      *hndl,
                                                        odu_ohp_state_t   next_state)
{
    
    PMC_ENTRY();
    
    switch(hndl->var.ctxt.state) 
    {
        case ODU_OHP_STATE_START:
            if (next_state == ODU_OHP_STATE_OPERATIONAL)
                PMC_RETURN(PMC_SUCCESS);
            if (next_state == ODU_OHP_STATE_START)
                PMC_RETURN(ODU_OHP_ERR_STATE_ALREADY_START);
            break; 

        case ODU_OHP_STATE_OPERATIONAL:    
            if (next_state == ODU_OHP_STATE_START)
                PMC_RETURN(PMC_SUCCESS);
            if (next_state == ODU_OHP_STATE_OPERATIONAL)
                PMC_RETURN(ODU_OHP_ERR_STATE_ALREADY_PROVISIONED);
            break; 

        default:
            PMC_RETURN(ODU_OHP_ERR_STATE_UNKNOWN);
    }
    PMC_RETURN(ODU_OHP_ERR_STATE_UNKNOWN);
}

/*******************************************************************************
* odu_ohp_reg_port_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure port

*
* INPUTS:
*   *hndl            - Pointer to OHP handle instance
*   port             - OHP port
*   sch_en           - sch enable
*   sd_pcc           - sd ram power control
*   src_user_id      - src user id
*   dest_user_id     - dest user id
*   num_ch           - Number of channels
*   ch_en            - Channel enable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_reg_port_cfg(odu_ohp_handle_t *hndl, 
                                       odu_ohp_port_t port, 
                                       UINT32 sch_en,  
                                       UINT32 sd_pcc,  
                                       UINT32 src_user_id,
                                       UINT32 dest_user_id,
                                       UINT32 num_ch,
                                       UINT32 ch_en)
{
    UINT32 ch_no;
    PMC_ENTRY();    

    if (port == ODU_OHP_PORT_A)
    {
        /* rx schedule enable */
        odu_ohp_core_field_RXFA_SCH_EN_set(NULL, hndl, sch_en); 

        /* ram shutdown cfg */
        odu_ohp_core_field_RXFA_SD_CONFIG_set(NULL, hndl, sd_pcc);  
        odu_ohp_core_field_TXFA_SD_CONFIG_set(NULL, hndl, sd_pcc);  

        /* src & dest UID */
        odu_ohp_core_field_STGA_SADDR_UID_set(NULL, hndl, src_user_id);
        odu_ohp_core_field_STGA_DADDR_UID_set(NULL, hndl, dest_user_id);

        /* channel enable */
        for(ch_no=0; ch_no < num_ch;ch_no++)
        {
             odu_ohp_core_reg_STGA_CH_EN_array_field_set(NULL,hndl, ch_no,0x00000001, 0,ch_en);
        }
    }
    else 
    {
        /* rx schedule enable */
        odu_ohp_core_field_RXFB_SCH_EN_set(NULL, hndl, sch_en); 

        /* ram shutdown cfg */
        odu_ohp_core_field_RXFB_SD_CONFIG_set(NULL, hndl, sd_pcc);  
        odu_ohp_core_field_TXFB_SD_CONFIG_set(NULL, hndl, sd_pcc);  

        /* src & dest UID */
        odu_ohp_core_field_STGB_SADDR_UID_set(NULL, hndl, src_user_id);
        odu_ohp_core_field_STGB_DADDR_UID_set(NULL, hndl, dest_user_id);

        /* channel enable */
        for(ch_no=0; ch_no < num_ch;ch_no++)
        {
             odu_ohp_core_reg_STGB_CH_EN_array_field_set(NULL,hndl, ch_no,0x00000001, 0,ch_en);
        }
    }
    PMC_RETURN(PMC_SUCCESS);
} /* odu_ohp_reg_port_cfg */


/*******************************************************************************
* odu_ohp_reg_ch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure and enable/disable OHP for a channel.

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*   extract_mask            - extract mask
*   insert_mask             - insert mask
*   tx_offset               - tx offset
*   ch_en                   - Channel enable
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
PRIVATE void odu_ohp_reg_ch_cfg(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id,  
                                UINT32 extract_mask,  
                                UINT32 insert_mask,  
                                UINT32 tx_offset,
                                BOOL ch_en)
{
    PMC_ENTRY();    
    if (port == ODU_OHP_PORT_A)
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_A: ch_id %d, ch_en=%d, extract: %x, insert=%x\n",ch_id,ch_en,extract_mask,insert_mask);
        /* txfa ins mask - 16 bits */
        odu_ohp_core_reg_TXFA_INS_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,insert_mask);
        /* rxfa extract mask - 16 bits */
        odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,extract_mask);
        /* txfa ins offset  - 8 bits */
        odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
        /* txfa ch en  */
        odu_ohp_core_reg_STGA_CH_EN_array_field_set(NULL,hndl, ch_id,0x00000001, 0,(ch_en ? 1 : 0)); 
    }
    else 
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_B: ch_id %d, ch_en=%d, extract: %x, insert=%x\n",ch_id,ch_en,extract_mask,insert_mask);
        /* txfb ins mask - 16 bits */
        odu_ohp_core_reg_TXFB_INS_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,insert_mask);
        /* rxfb extract mask - 16 bits */
        odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,extract_mask);
        /* txfb ins offset  - 8 bits */
        odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
        /* txfb ch en  */
        odu_ohp_core_reg_STGB_CH_EN_array_field_set(NULL,hndl, ch_id,0x00000001, 0,(ch_en ? 1 : 0)); 
    }
    PMC_RETURN();
} /* odu_ohp_reg_ch_cfg */

/*******************************************************************************
* odu_ohp_ch_tx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure OHP TX registers (insertion mask and insertion offset)

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*   insert_mask             - insert mask
*   tx_offset               - tx offset
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
PRIVATE void odu_ohp_ch_tx_cfg(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,  
                               UINT32 insert_mask,  
                               UINT32 tx_offset)
{
    PMC_ENTRY();    
    if (port == ODU_OHP_PORT_A)
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_A: ch_id %d, insert=0x%x tx_offset=%d\n",ch_id,insert_mask,tx_offset);
        /* txfa ins mask - 16 bits */
        odu_ohp_core_reg_TXFA_INS_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,insert_mask);
        /* txfa ins offset  - 8 bits */
        odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
        /* txfa ch en  */
    }
    else 
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_B: ch_id %d, insert=0x%x tx_offset=%d\n",ch_id,insert_mask,tx_offset);
        /* txfb ins mask - 16 bits */
        odu_ohp_core_reg_TXFB_INS_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,insert_mask);
        /* txfb ins offset  - 8 bits */
        odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
        /* txfb ch en  */
    }
    PMC_RETURN();
} /* odu_ohp_ch_tx_cfg */

/*******************************************************************************
* odu_ohp_ch_rx_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure OHP RX registers - extraction mask.

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*   extract_mask            - extract mask
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
PRIVATE void odu_ohp_ch_rx_cfg(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,  
                               UINT32 extract_mask)
{
    PMC_ENTRY();    
    if (port == ODU_OHP_PORT_A)
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_A: ch_id %d, extract mask: 0x%x\n",ch_id,extract_mask);
        /* rxfa extract mask - 16 bits */
        odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,extract_mask);
        /* txfa ins offset  - 8 bits */
    }
    else 
    {
        PMC_LOG_TRACE("OHP: ch_cfg PORT_B: ch_id %d, extract mask: 0x%x\n",ch_id,extract_mask);
        odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set(NULL,hndl,ch_id,0xffff,0,extract_mask);
    }
    PMC_RETURN();
} /* odu_ohp_ch_rx_cfg */

/*******************************************************************************
* odu_ohp_ch_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the channel enable status.

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if the channel is enabled
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL odu_ohp_ch_en_get(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id)
{
    UINT32 ch_en;
    PMC_ENTRY();    
    if (port == ODU_OHP_PORT_A)
    {
        /* txfa ch en  */
        ch_en = odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get(NULL,hndl,ch_id*32,ch_id*32);
    }
    else 
    {
        /* txfb ch en  */
        ch_en = odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get(NULL,hndl,ch_id*32,ch_id*32);
    }
    PMC_LOG_TRACE("OHP: ch_en port= %d ch_id=%d, ch_en=%d\n",port,ch_id,ch_en);
    PMC_RETURN((ch_en == 1));
} /* odu_ohp_ch_en_get */


/*******************************************************************************
* odu_ohp_reg_update_schedule
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update schedule

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   schedule                - new schedule
*   do_page_switch          - perform a page switch if true
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_reg_update_schedule(odu_ohp_handle_t *hndl, 
                                              odu_ohp_port_t port, 
                                              UINT8 *schedule,
                                              BOOL do_page_switch)
{
    UINT32 sched_wrds[ODU_OHP_SCHED_SIZE_MAX];
    UINT32 i;
    odu_ohp_port_cfg_t *port_cfg_ptr;

    PMC_ENTRY();    

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

            
    /* convert schedule in bytes to 32-bit words (needed by burst write command) */
    PMC_LOG_TRACE("OHP: update schedule: port = %d\n",port);

    for(i=0;i < ODU_OHP_SCHED_SIZE_MAX;i++)
    {
        sched_wrds[i] = schedule[i];
        if (schedule[i] != 0xff)
        {
            PMC_LOG_TRACE("OHP: sched[%d]=%d\n",i,schedule[i]);
        }
        
    }
    if (port == ODU_OHP_PORT_A)
        odu_ohp_core_lfield_SRV_CH_ID_A_set(NULL,hndl,sched_wrds); /* RXFA */
    else
        odu_ohp_core_lfield_SRV_CH_ID_B_set(NULL,hndl,sched_wrds); /* RXFB */

    if (do_page_switch)
        odu_ohp_sched_page_switch(hndl,port); 


    PMC_RETURN(PMC_SUCCESS);
} /* odu_ohp_reg_update_schedule */

/*******************************************************************************
* odu_ohp_sched_page_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Perform a schedule page swap.

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_sched_page_switch(odu_ohp_handle_t *hndl, odu_ohp_port_t port)
{
    int polling_usec = 250;
    odu_ohp_port_cfg_t *port_cfg_ptr;
    UINT32 x;

    PMC_ENTRY();    

    port_cfg_ptr = &(hndl->cfg.port_cfg[port]);

            
    /* convert schedule in bytes to 32-bit words (needed by burst write command) */
    PMC_LOG_TRACE("OHP: schedule page switch : port = %d\n",port);

    /* Poll the request bit until 0 */
    do {
        if (port == ODU_OHP_PORT_A)
            x = odu_ohp_core_field_RXFA_SCH_REQ_get(NULL,hndl);
        else
            x = odu_ohp_core_field_RXFB_SCH_REQ_get(NULL,hndl);
    
        #ifdef PMC_SW_SIMULATION
        x = 0;
        #endif

        if (x == 0) 
            break;
        PMC_Q_USLEEP(hndl,1000);
    } while (--polling_usec  > 0);
        
    if (port == ODU_OHP_PORT_A)
        odu_ohp_core_field_RXFA_SCH_REQ_set(NULL,hndl,1);
    else
        odu_ohp_core_field_RXFB_SCH_REQ_set(NULL,hndl,1);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_ohp_sched_page_switch */

/*******************************************************************************
* odu_ohp_tx_offset_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write TX Offset

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*   tx_offset               - TX offset
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
PRIVATE void odu_ohp_tx_offset_set(odu_ohp_handle_t *hndl, 
                                   odu_ohp_port_t port, 
                                   UINT32 ch_id,
                                   UINT32 tx_offset)
{
    PMC_ENTRY();    

    /* Update Tx offset */
    if (port == ODU_OHP_PORT_A)
    {
        odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
    }
    else 
    {
        odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set(NULL,hndl,ch_id*32,ch_id*32 + 8,tx_offset);
    }
    PMC_RETURN();
} /* odu_ohp_tx_offset_set */

/*******************************************************************************
* odu_ohp_tx_offset_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the TX Offset
*
*
* INPUTS:
*   *hndl                   - Pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channle ID
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TX Offset
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_ohp_tx_offset_get(odu_ohp_handle_t *hndl, 
                                     odu_ohp_port_t port, 
                                     UINT32 ch_id)
{
    UINT32 tx_offset;

    PMC_ENTRY();    

    /* Update Tx offset */
    if (port == ODU_OHP_PORT_A)
    {
        tx_offset = odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get(NULL,hndl,ch_id*32,ch_id*32 + 8);
    }
    else 
    {
        tx_offset = odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get(NULL,hndl,ch_id*32,ch_id*32 + 8);
    }
    PMC_RETURN(tx_offset);
} /* odu_ohp_tx_offset_get */

/*******************************************************************************
* odu_ohp_mask_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the insertion or extraction mask. 

*
* INPUTS:
*   *hndl                   - Pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel ID
*   ins_mask                - Retrieve INS_MASK if true, otherwise EXT_MASK
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TX Offset
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_ohp_mask_get(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id, 
                                BOOL ins_mask)
{
    UINT32 mask=0;

    PMC_ENTRY();    

    if (ins_mask) 
    {
        /* Update Tx offset */
        if (port == ODU_OHP_PORT_A)
        {
            mask = odu_ohp_core_reg_TXFA_INS_MASK_array_read(NULL,hndl,ch_id);
        }
        else 
        {
            mask = odu_ohp_core_reg_TXFB_INS_MASK_array_read(NULL,hndl,ch_id);
        }
    }
    else
    {
        /* Update Tx offset */
        if (port == ODU_OHP_PORT_A)
        {
            mask = odu_ohp_core_reg_RXFA_EXT_MASK_array_read(NULL,hndl,ch_id);
        }
        else 
        {
            mask = odu_ohp_core_reg_RXFB_EXT_MASK_array_read(NULL,hndl,ch_id);
        }
    }
    PMC_RETURN(mask);
} /* odu_ohp_mask_get */


/*******************************************************************************
* odu_ohp_ch_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the channel enable.

*
* INPUTS:
*   *hndl                   - pointer to OHP handle instance
*   port                    - OHP port
*   ch_id                   - Channel id
*   ch_en                   - enable
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
PRIVATE void odu_ohp_ch_en_set(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id,
                               UINT32 ch_en)
{
    PMC_ENTRY();    
    PMC_LOG_TRACE("OHP: ch_en set port= %d ch_id=%d, ch_en=%d\n",port,ch_id,ch_en);
    if (port == ODU_OHP_PORT_A)
    {
        /* txfa ch en  */
        odu_ohp_core_reg_STGA_CH_EN_array_field_set(NULL,hndl, ch_id,0x00000001, 0,(ch_en ? 1 : 0)); 
    }
    else 
    {
        /* txfb ch en  */
        odu_ohp_core_reg_STGB_CH_EN_array_field_set(NULL,hndl, ch_id,0x00000001, 0,(ch_en ? 1 : 0)); 
    }

    PMC_RETURN();
} /* odu_ohp_ch_en_set */

/*******************************************************************************
* odu_ohp_cfc_int_chnl_retrieve_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve OHP CFC interrupts of a given channel.
*   
*
* INPUTS:
*   cfc_handle     - Pointer to subsystem handle
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_retrieve_internal(cfc_handle_t    *cfc_handle,
                                                         UINT32           fifo_id,
                                                         cfc_int_chnl_t  *int_en_table_ptr,
                                                         cfc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->ipt_err_i)
    {      
        int_table_ptr->ipt_err_i = cfc_lfield_range_IPT_ERR_I_get(NULL,cfc_handle,fifo_id,fifo_id);
    }

    PMC_RETURN(result);
} /* odu_ohp_cfc_int_chnl_retrieve_internal */

/*******************************************************************************
* odu_ohp_cfc_int_chnl_enable_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable OHP CFC interrupts of a given channel.
*   
*
* INPUTS:
*   cfc_handle        - Pointer to subsystem handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_table_ptr     - pointer to channel interrupt table to enable/disable
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_enable_internal(cfc_handle_t    *cfc_handle,
                                                       UINT32           fifo_id,
                                                       cfc_int_chnl_t  *int_table_ptr,
                                                       cfc_int_chnl_t  *int_en_table_ptr,
                                                       BOOL8            enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (NULL == int_table_ptr || TRUE == int_table_ptr->ipt_err_i)
    {
        cfc_lfield_range_IPT_ERR_E_set(NULL,cfc_handle,fifo_id,fifo_id, enable);
        int_en_table_ptr->ipt_err_i = enable;
    }
    

    PMC_RETURN(result);
} /* odu_ohp_cfc_int_chnl_enable_internal */

/*******************************************************************************
* odu_ohp_cfc_int_chnl_clear_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Clear OHP CFC interrupts of a given channel.
*   
* INPUTS:
*   cfc_handle        - Pointer to subsystem handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_clear_internal(cfc_handle_t    *cfc_handle,
                                                      UINT32           fifo_id,
                                                      cfc_int_chnl_t  *int_en_table_ptr,
                                                      cfc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (NULL == int_en_table_ptr || TRUE == int_table_ptr->ipt_err_i)
    {
        cfc_lfield_range_IPT_ERR_I_set_to_clear(NULL,cfc_handle,fifo_id,fifo_id, TRUE);
    }
    

    PMC_RETURN(result);
} /* odu_ohp_cfc_int_chnl_enable_internal */


/*******************************************************************************
* odu_ohp_cfc_int_chnl_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active OHP CFC interrupt bits based on the enabled interrupt table.
*   
* INPUTS:
*   cfc_handle        - Pointer to subsystem handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ohp_cfc_int_chnl_enabled_check_internal(cfc_handle_t    *cfc_handle,
                                                              UINT32           fifo_id,
                                                              cfc_int_chnl_t  *int_en_table_ptr,
                                                              BOOL8           *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->ipt_err_i)
    {
        *int_found_ptr = cfc_lfield_range_IPT_ERR_I_get(NULL, cfc_handle, fifo_id, fifo_id);
    }
    
    PMC_RETURN(result);
} /* odu_ohp_cfc_int_chnl_enabled_check */

/*
** End of file
*/
