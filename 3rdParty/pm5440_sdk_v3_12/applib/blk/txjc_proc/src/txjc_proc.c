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

#include "txjc_proc_loc.h"


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
const char TXJC_PROC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    TXJC_PROC_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char TXJC_PROC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE PMC_ERROR txjc_proc_njo_pjo_cnt_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                            UINT32 chnl, 
                                            UINT32 pjo_cnt, 
                                            UINT32 njo_cnt);
PRIVATE PMC_ERROR txjc_proc_golden_cn_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                          UINT32 chnl, 
                                          UINT32 golden_cn);    
                                                                                  
PRIVATE PMC_ERROR txjc_proc_cn_ramp_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                        UINT32 chnl, UINT32 cn_n, UINT32 cn_d);

PRIVATE PMC_ERROR txjc_proc_int_validate(txjc_proc_handle_t *txjc_proc_handle,
                                         UINT32 chnl_id,
                                         void *non_null_ptr);
/*
** Public Functions
*/

/*******************************************************************************
* txjc_proc_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a TXJC_PROC block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the TXJC_PROC subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   txjc_proc_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC txjc_proc_handle_t *txjc_proc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    txjc_proc_handle_t *txjc_proc_handle;

    PMC_ENTRY();
    txjc_proc_handle = (txjc_proc_handle_t*)PMC_CTXT_CALLOC(sizeof(txjc_proc_handle_t), parent);
    
    pmc_handle_init(parent, txjc_proc_handle, sys_handle, PMC_MID_DIGI_TXJC_PROC, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(TXJC_PROC_LOG_ERR_STRINGS[0], TXJC_PROC_LOG_ERR_TABLE_BASE, TXJC_PROC_LOG_ERR_COUNT);
    
    /* Create CFC handle to be accessed within TXJC_PROC */
    txjc_proc_handle->cfc_handle = cfc_ctxt_create((pmc_handle_t *)txjc_proc_handle, base_address+BASE_ADDR_TXJC_PROC_REG_CFC, sys_handle, "CFC"); 
    PMC_CTXT_REGISTER_PTR(&txjc_proc_handle->cfc_handle, parent);


    PMC_RETURN(txjc_proc_handle);

} /* txjc_proc_ctxt_create */

 
/*******************************************************************************
* txjc_proc_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a TXJC_PROC block instance.
*
*
* INPUTS:
*   *txjc_proc_handle         - pointer to TXJC_PROC handle instance
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
PUBLIC void txjc_proc_ctxt_destroy(txjc_proc_handle_t *txjc_proc_handle)
{
    PMC_ENTRY();

    /* Destroy handle for CFC in TXJC_PROC */
    cfc_ctxt_destroy(txjc_proc_handle->cfc_handle); 
    
    PMC_CTXT_FREE(&txjc_proc_handle, txjc_proc_handle);
    
    PMC_RETURN();    
} /* txjc_proc_ctxt_destroy */


/*******************************************************************************
* txjc_proc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an TXJC_PROC block handle instance.
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   txjc_proc_instance    - TXJC_PROC of this instance \n 
*                           Refer to txjc_proc_instance_type_t \n
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
PUBLIC void txjc_proc_handle_init(txjc_proc_handle_t *txjc_proc_handle,
                                  txjc_proc_instance_type_t txjc_proc_instance)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();
    
    txjc_proc_handle->cfg.instance = txjc_proc_instance;
    
    /* CFC parameters */
    txjc_proc_handle->cfg.cfc_cfg.max_fifo_num  = TXJC_PROC_CFC_MAX_FIFO_NUM; 
    txjc_proc_handle->cfg.cfc_cfg.page_size     = TXJC_PROC_CFC_PAGE_SIZE;   
    
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = txjc_proc_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = txjc_proc_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(txjc_proc_handle->cfc_handle, &cfc_cfg);
    
    PMC_RETURN();
     
} /* txjc_proc_handle_init */


/*******************************************************************************
* txjc_proc_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Allocate any memory freed in ctxt_destroy
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
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
PUBLIC void txjc_proc_alloc_init(txjc_proc_handle_t *txjc_proc_handle)
{
    PMC_ENTRY();

    cfc_alloc_init(txjc_proc_handle->cfc_handle);
    
    PMC_RETURN();
     
} /* txjc_proc_alloc_init */


/*******************************************************************************
* txjc_proc_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an TXJC_PROC block handle instance.
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
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
PUBLIC void txjc_proc_handle_restart_init(txjc_proc_handle_t *txjc_proc_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                          pmc_energy_state_t top_energy_state)

{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();
    
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(TXJC_PROC_LOG_ERR_STRINGS[0], TXJC_PROC_LOG_ERR_TABLE_BASE, TXJC_PROC_LOG_ERR_COUNT);
    }

    /* Initialize CFC */
    cfc_cfg.max_fifo_num = txjc_proc_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = txjc_proc_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(txjc_proc_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);
    
    PMC_RETURN();
     
} /* txjc_proc_handle_restart_init */


/*******************************************************************************
*  txjc_proc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize TXJC_PROC with parameters specific to this instantiation.
*
* INPUTS:
*   *txjc_proc_handle   - pointer to TXJC_PROC handle instance
*   energy_state        - enum defining state to set the TXJC_PROC module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for TXJC_PROC module 
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_init(txjc_proc_handle_t *txjc_proc_handle, 
                                pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(txjc_proc_handle!=NULL, TXJC_PROC_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Initialize IPT in CFC */
        ret_val = cfc_init(txjc_proc_handle->cfc_handle, energy_state);
        if(PMC_SUCCESS == ret_val)
        {    
            if (txjc_proc_handle->cfg.instance == TXJC_PROC_INSTANCE_TGMP)
            {
                ret_val = cfc_bypass(txjc_proc_handle->cfc_handle, TRUE);
            }
            else if (txjc_proc_handle->cfg.instance == TXJC_PROC_INSTANCE_MUX)
            {
                ret_val = cfc_bypass(txjc_proc_handle->cfc_handle, FALSE);    
            }
        }  
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        ret_val = cfc_init(txjc_proc_handle->cfc_handle, energy_state);  
        
    } else {
        PMC_ASSERT(FALSE, TXJC_PROC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);
    
} /* txjc_proc_init */

/*******************************************************************************
* txjc_proc_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ram based configuration.
*
* INPUTS:
*   *txjc_proc_handle      - pointer to TXJC_PROC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_ram_init(txjc_proc_handle_t *txjc_proc_handle)
{
    /* variable declaration */
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(txjc_proc_handle != NULL, TXJC_PROC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
   
    for (i = 0; i < 96; i++)
    { 
        txjc_proc_reg_CHANNEL_COMMON_CFG_array_write(NULL, txjc_proc_handle, i, 0);
        txjc_proc_field_CN_GOLDEN_NUMERATOR_set(NULL, txjc_proc_handle, i, 0);
        txjc_proc_field_CN_GOLDEN_DENOMINATOR_set(NULL, txjc_proc_handle, i, 0);
        txjc_proc_field_CN_GOLDEN_set(NULL, txjc_proc_handle, i, 0);
    }
    
    /* initialize IPT_ID = 0x7F, IPT_VA = 0x7F */
    for (i = 0; i < 96; i++)
    {
        result = cfc_ipt_cfg(txjc_proc_handle->cfc_handle, i, 0, 0x7F, 0x7F);
    }
    
    PMC_RETURN(result); 
} /* txjc_proc_ram_init */ 

/*******************************************************************************
* txjc_proc_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   TXJC_PROC instance.
*
*
* INPUTS:
*   *txjc_proc_handle   - pointer to TXJC_PROC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                - FALSE:  Resource(s) not in start state \n
*                         TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL txjc_proc_start_state_test(txjc_proc_handle_t *txjc_proc_handle) 
{
    PMC_ENTRY();
    PMC_ASSERT(txjc_proc_handle!=NULL, TXJC_PROC_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    
    if(cfc_start_state_test(txjc_proc_handle->cfc_handle) == FALSE)
        PMC_RETURN(FALSE);
  
    PMC_RETURN(TRUE);

} /* txjc_proc_start_state_test */


/*******************************************************************************
* txjc_proc_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes and provisions a channel based on the channel's
*   client type and ODUk rate.
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*   server_type           - for DEMUX, HO ODU rate \n 
*                           Refer to util_global_odukp_type_t \n
*                           for RGMP, ODUkp or ODUjp type \n
*                           Refer to util_global_odukp_type_t \n
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for DEMUX, LO ODU rate \n
*                           Refer to util_global_odukp_type_t \n
*                           for RGMP, adaptaion_function \n
*                           Refer to adaptaion_function_enum for valid types
*   client_rate           - client data rate bits/sec for the case of \n
*                           ODUJP_CBRX_GMP \n
*                           ODUKP_CBRX_B_A \n
*                           ODUKP_CBRX_A \n
*   mapping_mode          - mapping mode \n
*                           Refer to util_global_mapping_mode_t
*   ts_type               - tributary slot size. See util_global_odu_line_payload_t.
*   m                     - number of tributary slots in the LO channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_chnl_prov(txjc_proc_handle_t *txjc_proc_handle, 
                                     UINT32 chnl,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type,
                                     DOUBLE client_rate,
                                     util_global_mapping_mode_t mapping_mode, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 m)
{
    /* variable declaration */
    UINT32 njo_cnt;
    UINT32 pjo_cnt;
    UINT32 golden_cn;
    UINT32 cn_n;
    UINT32 cn_d;
    UINT32 chnl_end_addr;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* variable initialization */
    njo_cnt = 0;
    pjo_cnt = 0;
    golden_cn = 0;
    chnl_end_addr = 0;
    
    /* argument checking */
    if ((chnl > 95) || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || (mapping_mode > UTIL_GLOBAL_NO_MAP))
    {
        PMC_LOG_TRACE("chnl = %u, server_type = %u, client_type = %u, mapping_mode = %u, \n", chnl, server_type, client_type, mapping_mode);   
        PMC_RETURN(TXJC_PROC_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if(mapping_mode == UTIL_GLOBAL_AMP)
    {
        if(txjc_proc_handle->cfg.instance == TXJC_PROC_INSTANCE_TGMP)
        {
            njo_cnt = 1;
            pjo_cnt = 1;
        }
        else if (txjc_proc_handle->cfg.instance == TXJC_PROC_INSTANCE_MUX)
        {
            if(server_type == UTIL_GLOBAL_ODU3E1 
                && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
            {
                njo_cnt = 2;
                pjo_cnt = 3;
            }
            else if (server_type == UTIL_GLOBAL_ODU1 && client_type == UTIL_GLOBAL_ODU0)
            {
                njo_cnt = 1;
                pjo_cnt = 1;
            }
            else
            {
                njo_cnt = 1;
                pjo_cnt = 2;
            }
        }
        else
        {
            PMC_RETURN(TXJC_PROC_LOG_ERR_INVALID_PARAMETERS);
        }       
    }
    else
    {
        njo_cnt = 0;
        pjo_cnt = 0;
    }

    if (mapping_mode == UTIL_GLOBAL_AMP || mapping_mode == UTIL_GLOBAL_GMP)
    {
        ret_val = util_global_cn_calc((util_global_parent_t)txjc_proc_handle->cfg.instance,
                                      server_type, client_type, client_rate, 
                                      ts_type, m, &golden_cn, &cn_n, &cn_d);                                        
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    
        txjc_proc_njo_pjo_cnt_cfg(txjc_proc_handle, chnl, pjo_cnt, njo_cnt);
        txjc_proc_golden_cn_cfg(txjc_proc_handle, chnl, golden_cn);
        txjc_proc_cn_ramp_cfg(txjc_proc_handle, chnl, cn_n, cn_d);   
    }    


    if (txjc_proc_handle->cfg.instance == TXJC_PROC_INSTANCE_TGMP)
    {
        /* Default is golden Cn */
        txjc_proc_field_PREV_CN_SEL_set(NULL, txjc_proc_handle, chnl, 0);

        /* Configure CFC end_address to (chnl_number*4)+3 */
        chnl_end_addr = (chnl*4)+3;

        ret_val = cfc_fifo_end_addr_cfg(txjc_proc_handle->cfc_handle, chnl, &chnl_end_addr, FALSE);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        } 
#if  defined(PMC_SW_SIMULATION)
        /* Take cfc fifo out of reset */
        ret_val = cfc_fifo_activate(txjc_proc_handle->cfc_handle, chnl);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
#endif /* defined(PMC_EMULATION) || defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */
    }   
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* txjc_proc_chnl_prov */


/*******************************************************************************
* txjc_proc_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_chnl_deprov(txjc_proc_handle_t *txjc_proc_handle, 
                                       UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
#if   defined(PMC_SW_SIMULATION)
    /* Put cfc fifo in reset */
    ret_val = cfc_fifo_deactivate(txjc_proc_handle->cfc_handle, chnl);
#endif /* defined(PMC_EMULATION) || defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */
  
        PMC_RETURN(ret_val);
    
} /* txjc_proc_chnl_deprov */

/*******************************************************************************
* txjc_proc_mux_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified LO channel in ODTU_MUX.
*
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_mux_lo_chnl_deprov(txjc_proc_handle_t *txjc_proc_handle, 
                                              UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS ;

    PMC_ENTRY();
    
    ret_val = txjc_proc_cn_select(txjc_proc_handle, chnl, 0);
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = txjc_proc_njo_pjo_cnt_cfg(txjc_proc_handle, chnl, 0, 0);  
    } 
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = txjc_proc_golden_cn_cfg(txjc_proc_handle, chnl, 0);
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = txjc_proc_cn_ramp_cfg(txjc_proc_handle, chnl, 0, 0);
    }    

  
    PMC_RETURN(ret_val);
    
} /* txjc_proc_mux_lo_chnl_deprov */

/*******************************************************************************
*  txjc_proc_cn_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function selects between the Golden Cn and previous Cn.
*
* INPUTS:
*   *txjc_proc_handle   - pointer to TXJC_PROC handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95  
*   cn_sel              - type of Cn value \n
*                         0 -- Golden Cn \n
*                         1 -- Previous Cn \n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_cn_select(txjc_proc_handle_t *txjc_proc_handle,
                                     UINT32 chnl, UINT32 cn_sel)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    txjc_proc_field_PREV_CN_SEL_set(NULL, txjc_proc_handle, chnl, cn_sel);

    PMC_RETURN(PMC_SUCCESS);

} /* txjc_proc_cn_select */


/*******************************************************************************
* txjc_proc_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel.
*                                                                               
* INPUTS:
*   *txjc_proc_handle   - pointer to TXJC_PROC handle instance                                                                            
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                 
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR txjc_proc_chnl_activate(txjc_proc_handle_t *txjc_proc_handle, 
                                         UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
#if   defined(PMC_SW_SIMULATION)
    /* take the specified CFC FIFO out of reset */
    ret_val = cfc_fifo_activate(txjc_proc_handle->cfc_handle,chnl);
#endif /*  defined(PMC_EMULATION) || defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */
    
    PMC_RETURN(ret_val);

} /* txjc_proc_chnl_activate */


/*******************************************************************************
* txjc_proc_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:
*   *txjc_proc_handle   - pointer to TXJC_PROC handle instance                                                                            
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                              
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC PMC_ERROR txjc_proc_chnl_deactivate(txjc_proc_handle_t *txjc_proc_handle, 
                                           UINT32 chnl) 
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
#if   defined(PMC_SW_SIMULATION)
    /* put the specified CFC FIFO into reset */
    ret_val = cfc_fifo_deactivate(txjc_proc_handle->cfc_handle,chnl);
#endif /* defined(PMC_EMULATION) || defined(PMC_VERIFICATION) || defined(PMC_SW_SIMULATION) */
   
    PMC_RETURN(ret_val);
   
} /* txjc_proc_chnl_deactivate */


                             
/*******************************************************************************
* txjc_proc_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of TXJC_PROC channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *txjc_proc_handle  - pointer to TXJC_PROC handle instance  
*   *chnl              - set of channel ID to be cleaned
*   num_chnl           - number of channel ID to be cleaned
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
PUBLIC void txjc_proc_chnl_clean(txjc_proc_handle_t *txjc_proc_handle, 
                                 UINT32 *chnl,
                                 UINT32 num_chnl) 
{
    PMC_ENTRY();
    
    /* clean sub-blocks */
    cfc_fifo_clean(txjc_proc_handle->cfc_handle,chnl,num_chnl);
       
    PMC_RETURN();

} /* txjc_proc_chnl_clean */

/*******************************************************************************
* txjc_proc_lo_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of TXJC_PROC LO channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *txjc_proc_handle  - pointer to TXJC_PROC handle instance 
*   *lo_chnl           - set of channel ID to be cleaned
*   num_lo_chnl        - number of channel ID to be cleaned
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
PUBLIC void txjc_proc_lo_chnl_clean( txjc_proc_handle_t *txjc_proc_handle, 
                                     UINT32        *lo_chnl,
                                     UINT32        num_lo_chnl ) 
{
    UINT32 lo_chnl_itr;
    PMC_ENTRY();
    
    /* clean sub-blocks */
    cfc_fifo_clean(txjc_proc_handle->cfc_handle,lo_chnl,num_lo_chnl);
    
    for(lo_chnl_itr=0; lo_chnl_itr<num_lo_chnl;lo_chnl_itr++)
    {
        (void) txjc_proc_cn_select(txjc_proc_handle, lo_chnl[lo_chnl_itr], 0);    
        (void) txjc_proc_njo_pjo_cnt_cfg(txjc_proc_handle, lo_chnl[lo_chnl_itr], 0, 0);  
        (void) txjc_proc_golden_cn_cfg(txjc_proc_handle, lo_chnl[lo_chnl_itr], 0);
        (void) txjc_proc_cn_ramp_cfg(txjc_proc_handle, lo_chnl[lo_chnl_itr], 0, 0);
    }                   
    PMC_RETURN();

} /* txjc_proc_lo_chnl_clean */
               
/*******************************************************************************
* txjc_proc_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function get the TXJC configuration
*
*   When this function is used by ODTU_MUX, the channel refers to LO channel.
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95  
*
* OUTPUTS:
*   *cn_n                 - golden Cn numerator
*   *cn_d                 - golden Cn denominator
*   *golden_cn            - golden Cn value
*   *pjo_cnt              - maximum possible PJOs
*   *njo_cnt              - maximum possible NJOs
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void txjc_proc_cfg_get(txjc_proc_handle_t *txjc_proc_handle,
                              UINT32 chnl, UINT32 *cn_n, UINT32 *cn_d,
                              UINT32 *golden_cn, UINT32 *pjo_cnt, 
                              UINT32 *njo_cnt)
{
    PMC_ENTRY();
    
    *cn_n = txjc_proc_field_CN_GOLDEN_NUMERATOR_get(NULL, txjc_proc_handle, chnl);
    *cn_d = txjc_proc_field_CN_GOLDEN_DENOMINATOR_get(NULL, txjc_proc_handle, chnl);

    *pjo_cnt = txjc_proc_field_PJO_CNT_get(NULL, txjc_proc_handle, chnl);
    *njo_cnt = txjc_proc_field_NJO_CNT_get(NULL, txjc_proc_handle, chnl);
    *golden_cn = txjc_proc_field_CN_GOLDEN_get(NULL, txjc_proc_handle, chnl);

    PMC_RETURN();    
} /* txjc_proc_cfg_get */

/*******************************************************************************
* txjc_proc_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *txjc_proc_handle  - pointer to TXJC_PROC handle instance
*   chnl_id            - Channel ID: Valid range: 0 - 95
*   int_table_ptr      - pointer to channel interrupt table with
*                        bits set to enable interrupts. If NULL, enables/disables
*                        all interrupts
*   int_en_table_ptr   - Pointer to table tracking enabled interrupts
*   enable             - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_int_chnl_enable(txjc_proc_handle_t *txjc_proc_handle,
                                           UINT32 chnl_id,
                                           txjc_proc_int_chnl_t *int_table_ptr,
                                           txjc_proc_int_chnl_t *int_en_table_ptr,
                                           BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = txjc_proc_int_validate(txjc_proc_handle, chnl_id, int_en_table_ptr);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(txjc_proc_handle->cfc_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc,
                                     &int_en_table_ptr->cfc,
                                     enable);
                                     
    }

    PMC_RETURN(result);
} /* txjc_proc_int_chnl_enable */

/*******************************************************************************
* txjc_proc_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *txjc_proc_handle      - pointer to TXJC_PROC handle instance
*   chnl_id                - Channel ID: Valid range: 0 - 95
*   int_table_ptr          - pointer to channel interrupt table with
*                            bits set to clear interrupts
*   int_en_table_ptr       - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_int_chnl_clear(txjc_proc_handle_t *txjc_proc_handle,
                                          UINT32 chnl_id,
                                          txjc_proc_int_chnl_t *int_table_ptr,
                                          txjc_proc_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = txjc_proc_int_validate(txjc_proc_handle, chnl_id, (void *)1);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(txjc_proc_handle->cfc_handle,
                                    chnl_id,
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc));
                                     
    }
    
    PMC_RETURN(result);
} /* txjc_proc_int_chnl_clear */

/*******************************************************************************
* txjc_proc_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *txjc_proc_handle      - pointer to TXJC_PROC handle instance
*   chnl_id                - Channel ID: Valid range: 0 - 95
*   filt_table_ptr         - pointer to filter table. Mark fields as TRUE within this
*                            table to indicate that the interrupt/status will be retrieved
*                            to the output table. NULL to retrieve all 
*                            interrupt/status.
*
* OUTPUTS:
*   int_table_ptr         - pointer to channel interrupt table to fill with 
*                           retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_int_chnl_retrieve(txjc_proc_handle_t *txjc_proc_handle,
                                             UINT32 chnl_id,
                                             txjc_proc_int_chnl_t *filt_table_ptr,
                                             txjc_proc_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = txjc_proc_int_validate(txjc_proc_handle, chnl_id, int_table_ptr);

     /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(txjc_proc_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
                                     
    }


    PMC_RETURN(result);
} /* txjc_proc_int_chnl_retrieve */

/*******************************************************************************
* txjc_proc_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl_id               - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr      - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr         - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR txjc_proc_int_chnl_enabled_check(txjc_proc_handle_t *txjc_proc_handle,
                                                  UINT32 chnl_id,
                                                  txjc_proc_int_chnl_t *int_en_table_ptr,
                                                  BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = txjc_proc_int_validate(txjc_proc_handle, chnl_id, int_found_ptr);

     /* CFC */
    if (PMC_SUCCESS == result && 
        *int_found_ptr == FALSE)
    {
        result = cfc_int_chnl_enabled_check(txjc_proc_handle->cfc_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc,
                                            int_found_ptr);
                                     
    }

    PMC_RETURN(result);
} /* txjc_proc_int_chnl_enabled_check */
                        
/*
** Private Functions
*/
/*******************************************************************************
* txjc_proc_njo_pjo_cnt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the maximum possible PJOs and NJOs for the channel.
*
*   When this function is used by ODTU_MUX, the channel refers to LO channel.
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95 
*   pjo_cnt               - maximum possible PJOs
*   njo_cnt               - maximum possible NJOs
*
*   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR txjc_proc_njo_pjo_cnt_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                            UINT32 chnl, 
                                            UINT32 pjo_cnt, 
                                            UINT32 njo_cnt)
{
    txjc_proc_buffer_t txjc_proc_buffer[1];

    PMC_ENTRY();
    
    txjc_proc_buffer_init(txjc_proc_buffer, txjc_proc_handle);
    txjc_proc_field_PJO_CNT_set(txjc_proc_buffer, txjc_proc_handle, chnl, pjo_cnt);
    txjc_proc_field_NJO_CNT_set(txjc_proc_buffer, txjc_proc_handle, chnl, njo_cnt);
    txjc_proc_buffer_flush(txjc_proc_buffer);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* txjc_proc_njo_pjo_cnt_cfg */


/*******************************************************************************
* txjc_proc_golden_cn_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the golden Cn. Cn value is used for justification 
*   processing in two conditions. One when there is no Cn value coming from 
*   pstream packet monitor or when there is a Server Signal Fail(SSF).
*
*   When this function is used by ODTU_MUX, the channel refers to LO channel.
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95 
*   golden_cn             - golden Cn value
*
*   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR txjc_proc_golden_cn_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                          UINT32 chnl, 
                                          UINT32 golden_cn)
{
    PMC_ENTRY();
    
    txjc_proc_field_CN_GOLDEN_set(NULL, txjc_proc_handle, chnl, golden_cn);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* txjc_proc_golden_cn_cfg */                                                                    
        
               
/*******************************************************************************
* txjc_proc_cn_ramp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the numerator and denominator used for golden Cn   
*   ramp generator.
*
*   When this function is used by ODTU_MUX, the channel refers to LO channel.
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95  
*   cn_n                  - golden Cn numerator
*   cn_d                  - golden Cn denominator
*
*   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR txjc_proc_cn_ramp_cfg(txjc_proc_handle_t *txjc_proc_handle,
                                        UINT32 chnl, UINT32 cn_n, UINT32 cn_d)
{
    /* variable declaration */
    
    
    PMC_ENTRY();
    
    txjc_proc_field_CN_GOLDEN_NUMERATOR_set(NULL, txjc_proc_handle, chnl, cn_n);
    txjc_proc_field_CN_GOLDEN_DENOMINATOR_set(NULL, txjc_proc_handle, chnl, cn_d);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* txjc_proc_cn_ramp_cfg */

/*******************************************************************************
* txjc_proc_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *txjc_proc_handle     - pointer to TXJC_PROC handle instance
*   chnl_id               - chnl id
*   non_null_ptr          - Pointer to check that shouldn't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR txjc_proc_int_validate(txjc_proc_handle_t *txjc_proc_handle,
                                         UINT32 chnl_id,
                                         void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != txjc_proc_handle, TXJC_PROC_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= txjc_proc_handle->cfg.cfc_cfg.max_fifo_num)
    {
        result = TXJC_PROC_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == result && NULL == non_null_ptr)
    {
        result = TXJC_PROC_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* txjc_proc_int_validate */



/*
** End of file
*/
