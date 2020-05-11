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
*     ODU RCP functions are implemented in the file.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "odu_rcp_loc.h"
#include "enet_fege.h"



/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Number of channels associated to ODU stage 1, N and 2 */
#define ODU_RCP_STG_1_N_NUM_CHNLS_MAX 12
#define ODU_RCP_STG_2_NUM_CHNLS_MAX 96
#define ODU_RCP_NUM_PDU_MAX 28
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
const char ODU_RCP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODU_RCP_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODU_RCP_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
 PRIVATE void otn_odu_ri_rcp_param_validate(odu_rcp_handle_t *hndl,       
                                            odu_rcp_frm_id_t rcp_frm_id,
                                            UINT32 chnl);
PRIVATE PMC_ERROR otn_odu_ri_rcp_insert_backward_defect_enable_cfg(odu_rcp_handle_t *hndl,       
                                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                                   UINT32 chnl,
                                                                   BOOL8 source);
PRIVATE PMC_ERROR otn_odu_ri_rcp_insert_aps_enable_cfg(odu_rcp_handle_t *hndl,       
                                                       odu_rcp_frm_id_t rcp_frm_id,
                                                       UINT32 chnl,
                                                       BOOL8 source);

PRIVATE odu_rcp_cfc_id_t odu_rcp_frm_cfc_id_convert(odu_rcp_frm_id_t frm_id, 
                                                    BOOL8 is_rx_frm);
PRIVATE void odu_rcp_var_default_init(odu_rcp_handle_t *hndl);


PRIVATE void odu_rcp_ctxt_state_set(odu_rcp_handle_t      *hndl,
                                    odu_rcp_inst_t         odu_rcp_id,
                                    odu_rcp_inst_state_t   state);


PRIVATE PMC_ERROR odu_rcp_ctxt_state_get(odu_rcp_handle_t      *hndl,
                                         odu_rcp_inst_t         odu_rcp_id,
                                         odu_rcp_inst_state_t  *state);

PRIVATE PMC_ERROR odu_rcp_ctxt_state_transition_request(odu_rcp_handle_t      *hndl,
                                                        odu_rcp_inst_t         odu_rcp_id,
                                                        odu_rcp_inst_state_t   next_state);

PRIVATE PMC_ERROR odu_ri_rcp_init(odu_rcp_handle_t    *hndl, 
                                  digi_otn_odu_ri_rcp_cfg_t *ri_rcp_cfg_ptr);

PRIVATE PMC_ERROR odu_di_rcp_init(odu_rcp_handle_t    *hndl, 
                                  odu_di_rcp_cfg_t *di_rcp_cfg_ptr);

PRIVATE PMC_ERROR odu_ri_rcp_uninit(odu_rcp_handle_t  *hndl);

PRIVATE PMC_ERROR odu_di_rcp_uninit(odu_rcp_handle_t  *hndl);

PRIVATE PMC_ERROR odu_ri_rcp_init_get(odu_rcp_handle_t *hndl, 
                                      digi_otn_odu_ri_rcp_cfg_t *ri_rcp_cfg_ptr);

PRIVATE PMC_ERROR odu_di_rcp_init_get(odu_rcp_handle_t *hndl, 
                                      odu_di_rcp_cfg_t *di_rcp_cfg_ptr);

PRIVATE PMC_ERROR odu_rcp_energy_state_reg_set(odu_rcp_handle_t  *hndl,
                                               odu_rcp_inst_t odu_rcp_id, 
                                               pmc_energy_state_t energy_state);
PRIVATE PMC_ERROR odu_rcp_eth_cfg(odu_rcp_handle_t  *hndl,
                                  odu_rcp_inst_t odu_rcp_id,
                                  BOOL8 enable);

PRIVATE void odu_rcp_di_build_mac_addr(odu_rcp_handle_t       *hndl,
                                       UINT8                   mac[UTIL_GLOBAL_MAC_NUM_BYTE],
                                       UINT32                 *mac_addr_31_00_bit,
                                       UINT32                 *mac_addr_47_32_bit);

PRIVATE PMC_ERROR otn_odu_ri_rcp_cfc_fifo_cfg(odu_rcp_handle_t  *hndl,
                                              BOOL8 is_rx_frm,
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 num_cal_entries,
                                              BOOL8 enable);

PRIVATE PMC_ERROR otn_odu_ri_rcp_cfc_fifo_resize(odu_rcp_handle_t  *hndl,
                                              BOOL8 is_rx_frm,
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 num_cal_entries);

PRIVATE PMC_ERROR otn_odu_ri_rcp_da_lookup_cfg(odu_rcp_handle_t  *hndl,
                                               odu_rcp_frm_id_t rcp_frm_id,
                                               UINT32 chnl, 
                                               digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr);

PRIVATE PMC_ERROR otn_odu_ri_rcp_da_lookup_cfg_get(odu_rcp_handle_t  *hndl,
                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                   UINT32 chnl, 
                                                   digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr);

PRIVATE PMC_ERROR otn_odu_ri_rcp_aps_pcc_cfg(odu_rcp_handle_t  *hndl,
                                             odu_rcp_frm_id_t rcp_frm_id,
                                             UINT32 chnl,
                                             UINT8 aps_pcc_mon_level);

PRIVATE PMC_ERROR otn_odu_ri_rcp_aps_pcc_cfg_get(odu_rcp_handle_t  *hndl,
                                                 odu_rcp_frm_id_t rcp_frm_id,
                                                 UINT32 chnl,
                                                 UINT8 *aps_pcc_mon_level);

PRIVATE PMC_ERROR otn_odu_ri_rcp_scheduler_sync(odu_rcp_handle_t  *hndl,
                                                odu_rcp_frm_id_t rcp_frm_id,
                                                UINT32 *ch_enties);

/*CFC functions*/
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_enable_internal(cfc_handle_t    *cfc_handle,
                                                       UINT32           fifo_id,
                                                       cfc_int_chnl_t  *int_table_ptr,
                                                       cfc_int_chnl_t  *int_en_table_ptr,
                                                       BOOL8            enable);

PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_clear_internal(cfc_handle_t    *cfc_handle,
                                                      UINT32           fifo_id,
                                                      cfc_int_chnl_t  *int_table_ptr,
                                                      cfc_int_chnl_t  *int_en_table_ptr);

PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_retrieve_internal(cfc_handle_t    *cfc_handle,
                                                         UINT32          fifo_id,    
                                                         cfc_int_chnl_t  *int_en_table_ptr,
                                                         cfc_int_chnl_t  *int_table_ptr);
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_enabled_check_internal(cfc_handle_t     *cfc_handle,
                                                              UINT32             fifo_d,
                                                              cfc_int_chnl_t   *int_en_table_ptr,
                                                              BOOL8              *int_found_ptr);    

PRIVATE void odu_rcp_cfc_ipt_entry_get_cfg(cfc_handle_t *cfc_handle, 
                                           UINT32 index,
                                           UINT32 *ipt_valid,
                                           UINT32 *ipt_id);
PRIVATE PMC_ERROR odu_rcp_cfc_inverted_page_table_cfg(cfc_handle_t *cfc_handle,  
                                                      UINT32 index,
                                                      UINT32 ipt_valid,
                                                      UINT32 ipt_id, 
                                                      UINT32 ipt_va); 
PRIVATE PMC_ERROR odu_rcp_cfc_fifo_reset_cfg(cfc_handle_t *cfc_handle, 
                                             UINT32 fifo_id, 
                                             UINT32 reset_value);                                             
PRIVATE void odu_rcp_cfc_ipt_page_mask_cfg(cfc_handle_t *cfc_handle);

PRIVATE void odu_rcp_cfc_bypass_cfg(cfc_handle_t *cfc_handle, UINT32 bypass);
PRIVATE void odu_rcp_cfc_ovr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                            UINT32 fifo_id,
                                            UINT32 enable);
PRIVATE void  odu_rcp_cfc_udr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                             UINT32 fifo_id,
                                             UINT32 enable);
PRIVATE void odu_rcp_cfc_ipt_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                            UINT32 fifo_id,
                                            UINT32 enable);
PRIVATE void odu_rcp_cfc_ipt_entry_state_get_cfg(cfc_handle_t *cfc_handle, UINT32 index,
                                                 UINT32 *ipt_valid);

PRIVATE PMC_ERROR odu_rcp_cfc_fifo_overflow_mon_cfg(cfc_handle_t *cfc_handle, UINT32 fifo_id);
PRIVATE void odu_rcp_cfc_fifo_end_addr_req_cfg(cfc_handle_t *cfc_handle, 
                                               UINT32 fifo_id,  
                                               UINT32 end_address);

PRIVATE UINT32 odu_rcp_cfc_fifo_end_addr_req_get(cfc_handle_t *cfc_handle);
PRIVATE UINT32 odu_rcp_cfc_fifo_end_addr_chg_success_get(cfc_handle_t *cfc_handle);
PRIVATE void odu_rcp_cfc_fifo_end_addr_chg_abort_cfg(cfc_handle_t *cfc_handle);

PRIVATE PMC_ERROR odu_rcp_di_energy_state_reg_set(odu_rcp_handle_t  *hndl,
                                                  pmc_energy_state_t energy_state);

PRIVATE UINT32 odu_rcp_set_bit(UINT32    src_word,
                               UINT8     bit_idx,
                               BOOL8     bit_val);

PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_retrieve_internal(odu_rcp_handle_t *odu_ri_rcp_handle,
                                                            UINT32            fifo_id,
                                                            cfc_int_chnl_t   *int_en_table_ptr,
                                                            cfc_int_chnl_t   *int_table_ptr);

PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_enable_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                          UINT32            fifo_id,
                                                          cfc_int_chnl_t    *int_table_ptr,
                                                          cfc_int_chnl_t    *int_en_table_ptr,
                                                          BOOL8              enable);

PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_clear_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                         UINT32             fifo_id,
                                                         cfc_int_chnl_t    *int_en_table_ptr);

PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_enabled_check_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                                 UINT32             fifo_id,
                                                                 cfc_int_chnl_t    *int_en_table_ptr,
                                                                 BOOL8             *int_found_ptr);
/*
** Public Functions
*/

/*******************************************************************************
* odu_rcp_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an ODU OHP block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odu_rcp_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_rcp_handle_t *odu_rcp_ctxt_create(pmc_handle_t       *parent, 
                                             UINT32              base_address, 
                                             pmc_sys_handle_t   *sys_handle, 
                                             const char         *tsb_name)
{
    odu_rcp_handle_t *odu_rcp_handle;

    PMC_ENTRY();

    odu_rcp_handle = (odu_rcp_handle_t*) PMC_CTXT_CALLOC(sizeof(odu_rcp_handle_t), parent);

    pmc_handle_init(parent, odu_rcp_handle, sys_handle, PMC_MID_DIGI_ODU_RCP, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(ODU_RCP_LOG_ERR_STRINGS[0], ODU_RCP_LOG_ERR_TABLE_BASE, ODU_RCP_LOG_ERR_COUNT);
    
    /* create CFCs handle */
    odu_rcp_handle->cfc_handle = (cfc_handle_t **) PMC_CTXT_CALLOC(sizeof(cfc_handle_t *) * ODU_RI_RCP_NUM_CFC_MAX, parent);
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle, parent);

    /* note: we don't care about base address since we use an external interface for these instances */
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG1] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_STG1"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG1], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STGN] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_STGN"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STGN], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG2] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_STG2"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG2], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG3A] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_STG3A"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG3A], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG3B] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_ST3B"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG3B], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG4] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_RX_CFC_STG4"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_RX_CFC_STG4], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG1] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_STG1"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG1], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STGN] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_STGN"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STGN], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG2] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_STG2"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG2], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG3A] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_STG3A"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG3A], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG3B] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_ST3B"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG3B], parent);
    odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG4] = cfc_ctxt_create((pmc_handle_t *)odu_rcp_handle, base_address, sys_handle, "ODU_RCP_TX_CFC_STG4"); 
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfc_handle[(UINT32)ODU_RCP_TX_CFC_STG4], parent);
    /* create ENET_FEGE handle */
    odu_rcp_handle->enet_fege_handle = (enet_fege_handle_t **) PMC_CTXT_CALLOC(sizeof(enet_fege_handle_t *) * ODU_RCP_NUM_INST_MAX, parent);
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->enet_fege_handle, parent);
    
    odu_rcp_handle->enet_fege_handle[(UINT32)ODU_RCP_RI_INST] = enet_fege_ctxt_create((pmc_handle_t *)odu_rcp_handle, 
                                                                                      base_address+BASE_ADDR_ODU_RCP_RI_FEGE_ENET_FEGE_MTSB, 
                                                                                      sys_handle, "ODU_RCP_RI_ENET_FEGE");    
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->enet_fege_handle[(UINT32)ODU_RCP_RI_INST], parent);

    odu_rcp_handle->enet_fege_handle[(UINT32)ODU_RCP_DI_INST] = enet_fege_ctxt_create((pmc_handle_t *)odu_rcp_handle, 
                                                                                      base_address+BASE_ADDR_ODU_RCP_DI_FEGE_ENET_FEGE_MTSB, 
                                                                                      sys_handle, "ODU_RCP_DI_ENET_FEGE");
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->enet_fege_handle[(UINT32)ODU_RCP_DI_INST], parent);

    /* initialized context state */
    odu_rcp_handle->var.ri_rcp_ctxt.state = ODU_RCP_STATE_START;
    odu_rcp_handle->var.di_rcp_ctxt.state = ODU_RCP_STATE_START;

    PMC_RETURN(odu_rcp_handle);
} /* odu_rcp_ctxt_create */


/*******************************************************************************
* odu_rcp_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an ODU_RCP block instance.
*
*
* INPUTS:
*   *odu_rcp_handle   - pointer to ODU_RCP handle instance
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
PUBLIC void odu_rcp_ctxt_destroy(odu_rcp_handle_t *odu_rcp_handle)
{
    UINT32 i;

    PMC_ENTRY();
    /* destroy enet_fege handle */
    for (i = 0; i < ODU_RCP_NUM_INST_MAX; i++)
    {
        enet_fege_ctxt_destroy(odu_rcp_handle->enet_fege_handle[i]);
    }
    
    PMC_CTXT_FREE(&odu_rcp_handle->enet_fege_handle, odu_rcp_handle);
    
    /* destroy CFCs handle */
    for (i = 0; i < ODU_RI_RCP_NUM_CFC_MAX; i++)
    {
        cfc_ctxt_destroy(odu_rcp_handle->cfc_handle[i]);
    }
    PMC_CTXT_FREE(&odu_rcp_handle->cfc_handle, odu_rcp_handle);
    PMC_CTXT_FREE(&odu_rcp_handle, odu_rcp_handle);

    PMC_RETURN();
} /* odu_rcp_ctxt_destroy */

/*******************************************************************************
* odu_rcp_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODU_RCP block handle instance.
*
*
* INPUTS:
*   *odu_rcp_handle     - pointer to ODU_RCP handle instance
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
PUBLIC void odu_rcp_handle_init(odu_rcp_handle_t   *odu_rcp_handle)
{
    cfc_fifo_cfg_fct_table_t cfg_fct_table;
    cfc_cfg_t cfc_cfg;
    pmc_handle_t *parent;
    UINT32 itr;
    
    PMC_ENTRY();

    PMC_MEMSET((void *)&cfg_fct_table, 0, sizeof(cfc_fifo_cfg_fct_table_t));

    /* setup fct table with local functions */
    cfg_fct_table.cfc_fifo_end_addr_chg_abort_cfg_ptr     = odu_rcp_cfc_fifo_end_addr_chg_abort_cfg;
    cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr   = odu_rcp_cfc_fifo_end_addr_chg_success_get;
    cfg_fct_table.cfc_fifo_end_addr_req_get_ptr           = odu_rcp_cfc_fifo_end_addr_req_get;
    cfg_fct_table.cfc_fifo_end_addr_req_cfg_ptr           = odu_rcp_cfc_fifo_end_addr_req_cfg;
    cfg_fct_table.cfc_fifo_overflow_mon_cfg_ptr           = odu_rcp_cfc_fifo_overflow_mon_cfg;
    cfg_fct_table.cfc_ipt_entry_state_get_cfg_ptr         = odu_rcp_cfc_ipt_entry_state_get_cfg;
    cfg_fct_table.cfc_ipt_entry_get_cfg_ptr               = odu_rcp_cfc_ipt_entry_get_cfg;
    cfg_fct_table.cfc_ovr_int_enable_cfg_ptr              = odu_rcp_cfc_ovr_int_enable_cfg;
    cfg_fct_table.cfc_udr_int_enable_cfg_ptr              = odu_rcp_cfc_udr_int_enable_cfg;
    cfg_fct_table.cfc_ipt_int_enable_cfg_ptr              = odu_rcp_cfc_ipt_int_enable_cfg;
    cfg_fct_table.cfc_bypass_cfg_ptr                      = odu_rcp_cfc_bypass_cfg;
    cfg_fct_table.cfc_ipt_page_mask_cfg_ptr               = odu_rcp_cfc_ipt_page_mask_cfg;
    cfg_fct_table.cfc_inverted_page_table_cfg_ptr         = odu_rcp_cfc_inverted_page_table_cfg;
    cfg_fct_table.cfc_fifo_reset_cfg_ptr                  = odu_rcp_cfc_fifo_reset_cfg;
    cfg_fct_table.cfc_int_chnl_enable_ptr                 = odu_rcp_cfc_int_chnl_enable_internal;
    cfg_fct_table.cfc_int_chnl_clear_ptr                  = odu_rcp_cfc_int_chnl_clear_internal;
    cfg_fct_table.cfc_int_chnl_retrieve_ptr               = odu_rcp_cfc_int_chnl_retrieve_internal;
    cfg_fct_table.cfc_int_chnl_enabled_check_ptr          = odu_rcp_cfc_int_chnl_enabled_check_internal;

    cfc_cfg.page_size        = ODU_RI_RCP_CFC_PAGE_SIZE;
 
    for (itr = 0; itr <  ODU_RI_RCP_NUM_CFC_MAX; itr++)
    {
        /* pass the framer id */
        cfg_fct_table.opaque = itr;
        cfg_fct_table.ext_handle = (cfc_ext_handle_t*) odu_rcp_handle;
        if (ODU_RCP_RX_CFC_STGN == (odu_rcp_cfc_id_t) itr || ODU_RCP_RX_CFC_STG1 == (odu_rcp_cfc_id_t) itr ||
            ODU_RCP_TX_CFC_STGN == (odu_rcp_cfc_id_t) itr || ODU_RCP_TX_CFC_STG1 == (odu_rcp_cfc_id_t) itr )
        {
            cfc_cfg.max_fifo_num = ODU_RI_RCP_STG1N_NUM_CHNL;
        } 
        else 
        {
            cfc_cfg.max_fifo_num = ODU_RI_RCP_STG234_NUM_CHNL;
        }
        cfc_handle_init_ext_cfg(odu_rcp_handle->cfc_handle[itr],&cfc_cfg,&cfg_fct_table);
    }


    /* initialize enet_fege handle */
    for (itr = 0; itr < ODU_RCP_NUM_INST_MAX; itr++)
    {
        enet_fege_handle_init(odu_rcp_handle->enet_fege_handle[itr]);
    }
    
    /* get the ultimate parent for DB usage */
    parent = pmc_handle_ultimate_parent_get(odu_rcp_handle);    
    odu_rcp_handle->cfg.db_dev_addr = (UINT32*)parent;
    PMC_CTXT_REGISTER_PTR(&odu_rcp_handle->cfg.db_dev_addr, parent);
    odu_rcp_var_default_init(odu_rcp_handle);

    PMC_RETURN();

} /* odu_rcp_handle_init */


/*******************************************************************************
* odu_rcp_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles local memory allocations that will be freed in ctxt_destroy
*
*
* INPUTS:
*   *odu_rcp_handle       - pointer to ODU_RCP handle instance
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
PUBLIC void odu_rcp_alloc_init(odu_rcp_handle_t *odu_rcp_handle)
{
    UINT32 itr;

    PMC_ENTRY();

    for (itr = 0; itr < ODU_RI_RCP_NUM_CFC_MAX; itr++)
    {
        cfc_alloc_init(odu_rcp_handle->cfc_handle[itr]);
    }

    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an ODU_RCP block handle instance.
*
*
* INPUTS:
*   *odu_rcp_handle       - pointer to ODU_RCP handle instance
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
PUBLIC void odu_rcp_handle_restart_init(odu_rcp_handle_t   *odu_rcp_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                        pmc_energy_state_t top_energy_state
)
{
    cfc_fifo_cfg_fct_table_t cfg_fct_table;
    cfc_cfg_t cfc_cfg;
    UINT32 itr;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODU_RCP_LOG_ERR_STRINGS[0], ODU_RCP_LOG_ERR_TABLE_BASE, ODU_RCP_LOG_ERR_COUNT);
    }

    PMC_MEMSET((void *)&cfg_fct_table, 0, sizeof(cfc_fifo_cfg_fct_table_t));

    /* setup fct table with local functions */
    cfg_fct_table.cfc_fifo_end_addr_chg_abort_cfg_ptr     = odu_rcp_cfc_fifo_end_addr_chg_abort_cfg;
    cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr   = odu_rcp_cfc_fifo_end_addr_chg_success_get;
    cfg_fct_table.cfc_fifo_end_addr_req_get_ptr           = odu_rcp_cfc_fifo_end_addr_req_get;
    cfg_fct_table.cfc_fifo_end_addr_req_cfg_ptr           = odu_rcp_cfc_fifo_end_addr_req_cfg;
    cfg_fct_table.cfc_fifo_overflow_mon_cfg_ptr           = odu_rcp_cfc_fifo_overflow_mon_cfg;
    cfg_fct_table.cfc_ipt_entry_state_get_cfg_ptr         = odu_rcp_cfc_ipt_entry_state_get_cfg;
    cfg_fct_table.cfc_ipt_entry_get_cfg_ptr               = odu_rcp_cfc_ipt_entry_get_cfg;
    cfg_fct_table.cfc_ovr_int_enable_cfg_ptr              = odu_rcp_cfc_ovr_int_enable_cfg;
    cfg_fct_table.cfc_udr_int_enable_cfg_ptr              = odu_rcp_cfc_udr_int_enable_cfg;
    cfg_fct_table.cfc_ipt_int_enable_cfg_ptr              = odu_rcp_cfc_ipt_int_enable_cfg;
    cfg_fct_table.cfc_bypass_cfg_ptr                      = odu_rcp_cfc_bypass_cfg;
    cfg_fct_table.cfc_ipt_page_mask_cfg_ptr               = odu_rcp_cfc_ipt_page_mask_cfg;
    cfg_fct_table.cfc_inverted_page_table_cfg_ptr         = odu_rcp_cfc_inverted_page_table_cfg;
    cfg_fct_table.cfc_fifo_reset_cfg_ptr                  = odu_rcp_cfc_fifo_reset_cfg;
    cfg_fct_table.cfc_int_chnl_enable_ptr                 = odu_rcp_cfc_int_chnl_enable_internal;
    cfg_fct_table.cfc_int_chnl_clear_ptr                  = odu_rcp_cfc_int_chnl_clear_internal;
    cfg_fct_table.cfc_int_chnl_retrieve_ptr               = odu_rcp_cfc_int_chnl_retrieve_internal;
    cfg_fct_table.cfc_int_chnl_enabled_check_ptr          = odu_rcp_cfc_int_chnl_enabled_check_internal;    

    cfc_cfg.page_size        = ODU_RI_RCP_CFC_PAGE_SIZE;
 
    for (itr = 0; itr <  ODU_RI_RCP_NUM_CFC_MAX; itr++)
    {
        /* pass the framer id */
        cfg_fct_table.opaque = itr;
        cfg_fct_table.ext_handle = (cfc_ext_handle_t*) odu_rcp_handle;
        if (ODU_RCP_RX_CFC_STGN == (odu_rcp_cfc_id_t) itr || ODU_RCP_RX_CFC_STG1 == (odu_rcp_cfc_id_t) itr ||
            ODU_RCP_TX_CFC_STGN == (odu_rcp_cfc_id_t) itr || ODU_RCP_TX_CFC_STG1 == (odu_rcp_cfc_id_t) itr )
        {
            cfc_cfg.max_fifo_num = ODU_RI_RCP_STG1N_NUM_CHNL;
        } 
        else 
        {
            cfc_cfg.max_fifo_num = ODU_RI_RCP_STG234_NUM_CHNL;
        }
        cfc_handle_restart_init_ext_cfg(odu_rcp_handle->cfc_handle[itr],&cfc_cfg,&cfg_fct_table, restart_init_cfg_ptr, top_energy_state);
    }


    PMC_RETURN();

} /* odu_rcp_handle_restart_init */


/*******************************************************************************
* odu_rcp_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODU_RCP block instance.
*
*
* INPUTS:
*   *odu_rcp_handle   - pointer to ODU_RCP handle instance
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
PUBLIC UINT32 odu_rcp_base_address_get(odu_rcp_handle_t *odu_rcp_handle)
{
    PMC_ENTRY();

    PMC_RETURN(odu_rcp_handle->base.base_address);
} /* odu_rcp_base_address_get */


/*******************************************************************************
* odu_rcp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize a RI or a DI RCP instance.
*
*
* INPUTS:
*   *hndl                   - pointer to ODU_RCP handle instance
*    odu_rcp_id             - identify the RI or DI RCP instance
*   *rcp_cfg_ptr            - pointer to the RCP configuration
*
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
PUBLIC PMC_ERROR odu_rcp_init(odu_rcp_handle_t   *hndl,
                              odu_rcp_inst_t      odu_rcp_id,
                              odu_rcp_init_cfg_t *rcp_cfg_ptr)
{
    PMC_ERROR rc;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rcp_cfg_ptr, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    PMC_ASSERT((UINT32)LAST_ODU_RCP_INST > (UINT32)odu_rcp_id, ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    rc = odu_rcp_ctxt_state_transition_request(hndl, odu_rcp_id, ODU_RCP_STATE_OPERATIONAL);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }

    if (ODU_RCP_RI_INST == odu_rcp_id)
    {
        rc = odu_ri_rcp_init(hndl, (digi_otn_odu_ri_rcp_cfg_t*) rcp_cfg_ptr);
    } 
    else 
    {
        rc = odu_di_rcp_init(hndl, (odu_di_rcp_cfg_t*) rcp_cfg_ptr);
    }
    if (PMC_SUCCESS == rc)
    {
        rc = enet_fege_prov(hndl->enet_fege_handle[(UINT32)odu_rcp_id], FALSE,TRUE, TRUE);
    }
    
    if (PMC_SUCCESS == rc)
    {
        odu_rcp_ctxt_state_set(hndl, odu_rcp_id, ODU_RCP_STATE_OPERATIONAL);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_rcp_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Un-initilize a RI or a DI RCP instance.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    odu_rcp_id       - identify the RI or DI RCP instance
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
PUBLIC PMC_ERROR odu_rcp_uninit(odu_rcp_handle_t *hndl,
                                odu_rcp_inst_t odu_rcp_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((UINT32)LAST_ODU_RCP_INST > (UINT32)odu_rcp_id, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    
    rc = odu_rcp_ctxt_state_transition_request(hndl, odu_rcp_id, ODU_RCP_STATE_START);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }
    if (PMC_SUCCESS == rc)
    {
        if (ODU_RCP_RI_INST == odu_rcp_id)
        {
            rc = odu_ri_rcp_uninit(hndl);
        } 
        else 
        {
            rc = odu_di_rcp_uninit(hndl);
        }
    }
  
    rc = enet_fege_deprov(hndl->enet_fege_handle[(UINT32)odu_rcp_id]);
      
    if (PMC_SUCCESS == rc)
    {
        odu_rcp_ctxt_state_set(hndl, odu_rcp_id, ODU_RCP_STATE_START);
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
* odu_rcp_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets a RI or a DI RCP instance init config and enable status.
*
*
* INPUTS:
*   *hndl                   - pointer to ODU_RCP handle instance
*    odu_rcp_id             - identify the RI or DI RCP instance
*
* OUTPUTS:
*   *rcp_cfg_ptr            - pointer to the RCP configuration
*    enable                 - TRUE when RCP inited or FALSE when RCP uninited
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error 
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rcp_init_get(odu_rcp_handle_t   *hndl,
                                  odu_rcp_inst_t      odu_rcp_id,
                                  odu_rcp_init_cfg_t *rcp_cfg_ptr,
                                  BOOL               *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_rcp_inst_state_t state = ODU_RCP_STATE_START;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((UINT32)LAST_ODU_RCP_INST > (UINT32)odu_rcp_id, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
   
    if (PMC_SUCCESS == rc)
    {
        rc = odu_rcp_ctxt_state_get(hndl, odu_rcp_id, &state);
    }

    if (PMC_SUCCESS == rc)
    {
        if (ODU_RCP_STATE_OPERATIONAL == state)
        {
            *enable = TRUE;
        }
        else if (ODU_RCP_STATE_START == state)
        {
            *enable = FALSE;
        }
    }

    if (TRUE == *enable)
    {
        if (ODU_RCP_RI_INST == odu_rcp_id)
        {
            rc = odu_ri_rcp_init_get(hndl, (digi_otn_odu_ri_rcp_cfg_t*) rcp_cfg_ptr);
        } 
        else if (ODU_RCP_DI_INST == odu_rcp_id)
        {
            rc = odu_di_rcp_init_get(hndl, (odu_di_rcp_cfg_t*) rcp_cfg_ptr);
        }
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_extract_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*  Configures DI RCP extraction for an ODU framer channel. A RI_RCP PDU is 
*  assembled  with the following information for the specified channel:\n
*    - The BDI/BEI/BIAE bits from the TCMs and PM layers are extracted. \n
*    - The four APS/PCC bytes from a single connection monitoring level are \n
*      extracted (one of ODUk Path, ODUk TCM1-6, or ODUk server layer trail \n
*      where the server layer trail is an OTUk or HO ODUk). \n
*    - The Server Signal Fail (SSF) over the 8 ODUk frame window where the \n
*      APS/PCC bytes are captured. 
*
*  Assembled RI_RCP DPU are sent to a the destination specified by a \n
*  digi_otn_odu_ri_rcp_dst_cfg_t structure.  odu_ri_rcp_dest_get \n
*  function can be used to retrieve a digi_otn_odu_ri_rcp_dst_cfg_t structure \n
*  from a provisioned ODU framer channel on this DIGI or on a remote DIGI.\n
*  The APS/PCC connection monitoring level can updated dynamically. \n
*  odu_ri_rcp_aps_pcc_cfg should be used to update this parameter.
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*   *ri_rcp_dst_cfg_ptr - pointer to ODU RI RCP destination configuration
*   aps_pcc_mon_level   - The APS/PCC connection monitoring level  (0 to 7)
*   *ch_entries         - Number of calendar entries per channel
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: Enabled \n 
*                         FALSE: Disabled \n 
*                         When the is parameter is FALSE, aps_pcc_mon_level \n 
*                         and ri_rcp_dst_cfg_ptr parameters are ignored 
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
PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_cfg(odu_rcp_handle_t *hndl,         
                                            odu_rcp_frm_id_t rcp_frm_id,
                                            UINT32 chnl,
                                            digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                            UINT8 aps_pcc_mon_level,
                                            UINT32 *ch_entries,
                                            BOOL8 enable)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);
        
    
    /* check state */
    if (TRUE == enable && TRUE == hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_EXTRACT_ALREADY_ENABLED);
    } else if (FALSE == enable && FALSE == hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_EXTRACT_NOT_ENABLED);
    }

    /* build CFC FIFO */
    if (PMC_SUCCESS == rc)
    {
        UINT32 num_cal_entries = ch_entries[chnl];
        rc =  otn_odu_ri_rcp_cfc_fifo_cfg(hndl,TRUE,rcp_frm_id,chnl,num_cal_entries,enable);
    }
    /* update DA lookup */
    if (PMC_SUCCESS == rc && TRUE == enable )
    {
        /* validate configuration */
        if (ri_rcp_dst_cfg_ptr->dst_frm >= ODU_RI_RCP_NUM_FRAMER_MAX)
        {
            rc = ODU_RCP_RI_ERR_INVALID_DEST_FRM;
        } else if (ODU_RCP_FRM_STGN == (odu_rcp_frm_id_t) ri_rcp_dst_cfg_ptr->dst_frm || ODU_RCP_FRM_STG1 == (odu_rcp_frm_id_t) ri_rcp_dst_cfg_ptr->dst_frm) 
        {
            if (ri_rcp_dst_cfg_ptr->dst_channel >= ODU_RI_RCP_STG1N_NUM_CHNL)
            {
                rc = ODU_RCP_RI_ERR_INVALID_DEST_CHNL;
            }
        }
        else 
        {
            if (ri_rcp_dst_cfg_ptr->dst_channel >= ODU_RI_RCP_STG234_NUM_CHNL)
            {
                rc = ODU_RCP_RI_ERR_INVALID_DEST_CHNL;
            }        
        }
        if (PMC_SUCCESS == rc)
        {
            rc = otn_odu_ri_rcp_da_lookup_cfg(hndl,rcp_frm_id,chnl,ri_rcp_dst_cfg_ptr);
        }
    }
    /* configure APS/PCC */
    if (PMC_SUCCESS == rc && TRUE == enable )
    {    
        rc = otn_odu_ri_rcp_aps_pcc_cfg(hndl,rcp_frm_id,chnl,aps_pcc_mon_level);
    }
        
    /* update context */
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl] =  enable;
    }

    /* update scheduler */
    if (PMC_SUCCESS == rc)
    {
        rc = otn_odu_ri_rcp_scheduler_sync(hndl,rcp_frm_id,ch_entries);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_extract_cfg_get
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*  Gets DI RCP extraction enable status for an ODU framer channel. 
*
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*   
* OUTPUTS:
*   *ri_rcp_dst_cfg_ptr - pointer to ODU RI RCP destination configuration
*   aps_pcc_mon_level   - The APS/PCC connection monitoring level  (0 to 7)
*   enable              - RI_RCP extraction control. \n 
*                         TRUE: Enabled \n 
*                         FALSE: Disabled \n 
*                         When the is parameter is FALSE, aps_pcc_mon_level \n 
*                         and ri_rcp_dst_cfg_ptr parameters are ignored 
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/        
PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_cfg_get(odu_rcp_handle_t *hndl,         
                                                odu_rcp_frm_id_t rcp_frm_id,
                                                UINT32 chnl,
                                                digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                UINT8 *aps_pcc_mon_level,
                                                BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        *enable = FALSE;
        PMC_RETURN(PMC_SUCCESS);
    }

    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);

    *enable = hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl];

    if (TRUE == *enable)
    {
        rc = otn_odu_ri_rcp_da_lookup_cfg_get(hndl,rcp_frm_id,chnl,ri_rcp_dst_cfg_ptr);
        if(PMC_SUCCESS == rc)
        {
            rc = otn_odu_ri_rcp_aps_pcc_cfg_get(hndl,rcp_frm_id,chnl,aps_pcc_mon_level);
        }
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_extract_resize
* ______________________________________________________________________________
* 
* DESCRIPTION:
*  RI RCP extraction resize. 
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*   *ch_entries         - Number of calendar entries per channel
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
PUBLIC PMC_ERROR otn_odu_ri_rcp_extract_resize(odu_rcp_handle_t *hndl,         
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 *ch_entries)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();


    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);
        
    
    /* check state */
    if (! hndl->var.ri_rcp_ctxt.extract_rx_enable[rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_EXTRACT_NOT_ENABLED);
    } 

    /* build CFC FIFO */
    if (PMC_SUCCESS == rc)
    {
        UINT32 num_cal_entries = ch_entries[chnl];
        rc =  otn_odu_ri_rcp_cfc_fifo_resize(hndl,TRUE,rcp_frm_id,chnl,num_cal_entries);
    }

    /* update scheduler */
    if (PMC_SUCCESS == rc)
    {
        rc = otn_odu_ri_rcp_scheduler_sync(hndl,rcp_frm_id,ch_entries);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_aps_pcc_mon_level_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Configures the APS/PCC connection monitoring for an ODU framer channel. This function 
*   shall be called on a channel where the DI RCP extraction has been already enabled using 
*   otn_odu_ri_rcp_extract_cfg. 
*   
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*   rcp_frm_id            - The framer stage 
*   chnl                  - The channel id
*   aps_pcc_mon_level     - The APS/PCC connection monitoring level  (0 to 7)
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
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_pcc_mon_level_cfg(odu_rcp_handle_t *hndl,       
                                                      odu_rcp_frm_id_t rcp_frm_id,
                                                      UINT32 chnl,
                                                      UINT8 aps_pcc_mon_level)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();
    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);
    
    /* check context */
    if (FALSE == hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_EXTRACT_NOT_ENABLED);
    }

    /* configure APS/PCC */
    if (PMC_SUCCESS == rc)
    {    
        rc = otn_odu_ri_rcp_aps_pcc_cfg(hndl,rcp_frm_id,chnl,aps_pcc_mon_level);
    }
    
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_dest_get
* ______________________________________________________________________________
* 
* DESCRIPTION:
*  
*  This function provides the RI_RCP destination configuration of a specified \n
*  ODU framer channel
*
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*   ri_rcp_frm_id         - The framer stage 
*   chnl                  - The channel id
*   
* OUTPUTS:
*   *ri_rcp_dst_cfg_ptr   - pointer to ODU RI RCP destination configuration
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_odu_ri_rcp_dest_get( odu_rcp_handle_t *hndl,       
                                          odu_rcp_frm_id_t ri_rcp_frm_id,
                                          UINT32 chnl,
                                          digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl, ri_rcp_frm_id, chnl);

    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((UINT32)LAST_ODU_RCP_FRM_ID > (UINT32)ri_rcp_frm_id, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    PMC_ASSERT(96 > chnl, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    
    /* fill destination cfg */
    ri_rcp_dst_cfg_ptr->dst_digi    = hndl->cfg.ri_rcp_port_cfg.eth_mac_digi_id;
    ri_rcp_dst_cfg_ptr->dst_frm     = (UINT8) ri_rcp_frm_id;
    ri_rcp_dst_cfg_ptr->dst_channel = chnl;

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_bdi_bei_biae_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   Configures the source of BDI/BEI bits in the TX framer of a specified \n
*   channel. They could come from the RI_RCP or a local RX framer. This enable\n
*    affects  all TCM's and PM layers of a particular channel.
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*   num_cal_entries     - Number of calendar entries for the channel
*   source              - Transmit BDI/BEI bits source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: Local Rx Framer 
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
PUBLIC PMC_ERROR otn_odu_ri_rcp_bdi_bei_biae_insert_cfg(odu_rcp_handle_t *hndl,       
                                                        odu_rcp_frm_id_t rcp_frm_id,
                                                        UINT32 chnl,
                                                        UINT32 num_cal_entries,
                                                        BOOL8 source)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state )
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);
        
    /* check state */
    if (TRUE == source && TRUE == hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_ALREADY_ENABLED);
    } 
    else if (FALSE == source && FALSE == hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_NOT_ENABLED);
    }

    /* build TX CFC FIFO */
    if (PMC_SUCCESS == rc)
    { 
        if (source == 1 && hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl] == FALSE)
        {
            /* build CFC fifo since it's not present */    
            rc =  otn_odu_ri_rcp_cfc_fifo_cfg(hndl, FALSE, rcp_frm_id, chnl, num_cal_entries, source);
        }
        else if (source == 0 && hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl] == FALSE)
        {
            /* no one is using FIFO so destroy for this channel */
            rc =  otn_odu_ri_rcp_cfc_fifo_cfg(hndl, FALSE, rcp_frm_id, chnl, num_cal_entries, source);
        }
    }            
            
    /* enable insertion */
    if (PMC_SUCCESS == rc )
    {    
        rc = otn_odu_ri_rcp_insert_backward_defect_enable_cfg(hndl,rcp_frm_id,chnl,source);
    }
        
    /* update context */
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl] =  source;
    }

    PMC_RETURN(rc);
} /* otn_odu_ri_rcp_bdi_bei_biae_insert_cfg */


/*******************************************************************************
* otn_odu_ri_rcp_bdi_bei_biae_insert_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   Get the source of BDI/BEI bits in the TX framer of a specified \n
*   channel. 
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id       - The framer stage 
*   chnl                 - The channel id
*
* OUTPUTS:
*   source              - Transmit BDI/BEI bits source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: Local Rx Framer 
*
* RETURNS:
*   None.  
*
* NOTES:
*
*******************************************************************************/
PUBLIC void otn_odu_ri_rcp_bdi_bei_biae_insert_get(odu_rcp_handle_t *hndl,       
                                                        odu_rcp_frm_id_t rcp_frm_id,
                                                        UINT32 chnl,
                                                        BOOL8 *source)
{


    PMC_ENTRY();

    otn_odu_ri_rcp_param_validate(hndl,rcp_frm_id,chnl);

    *source = hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl];

    PMC_RETURN();

}/* otn_odu_ri_rcp_bdi_bei_biae_insert_get */

/*******************************************************************************
* otn_odu_ri_rcp_aps_insert_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Configures the APS/PCC bytes  insertion for an ODU framer channel from the \n
*   RI RCP port. The insertion monitoring level is taken from the RI_RCP PDU \n
*   received on the port. 
* 
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*   num_cal_entries     - Number of calendar entries for the channel
*   source              - APS/PCC bytes source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: non-selected 
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
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_insert_cfg( odu_rcp_handle_t *hndl,    
                                                odu_rcp_frm_id_t rcp_frm_id,
                                                UINT32 chnl,   
                                                UINT32 num_cal_entries, 
                                                BOOL8 source)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state)
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_OPERATIONNAL);
    }
    otn_odu_ri_rcp_param_validate(hndl ,rcp_frm_id,chnl);
        
    /* check state */
    if (TRUE == source && TRUE == hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_ALREADY_ENABLED);
    }
    else if (FALSE == source && FALSE == hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl])
    {
        PMC_RETURN(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_NOT_ENABLED);
    }
    
    /* check if TX_FIFO has been built for this channel */
    if (PMC_SUCCESS == rc)
    {
        if (source == TRUE && hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl] == FALSE)
        {
            /* no FIFO has been built */
            rc = otn_odu_ri_rcp_cfc_fifo_cfg(hndl, FALSE, rcp_frm_id, chnl, num_cal_entries, source);
        }
        else if (source == FALSE && hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl] == FALSE)
        {
            /* no one is using FIFO so destroy */
            rc = otn_odu_ri_rcp_cfc_fifo_cfg(hndl, FALSE, rcp_frm_id, chnl, num_cal_entries, source);
        } 
    }               

    /* enable insertion */
    if (PMC_SUCCESS == rc)
    {    
        rc = otn_odu_ri_rcp_insert_aps_enable_cfg(hndl,rcp_frm_id,chnl,source);
    }
        
    /* update context */
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl] =  source;
    }

    PMC_RETURN(rc);
} /* otn_odu_ri_rcp_aps_insert_cfg */

/*******************************************************************************
* otn_odu_ri_rcp_aps_insert_en_get
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Gets the APS/PCC bytes insertion status for an ODU framer channel from the \n
*   RI RCP port. The insertion monitoring level is taken from the RI_RCP PDU \n
*   received on the port. 
* 
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   rcp_frm_id          - The framer stage 
*   chnl                - The channel id
*                                                                         
*    
* OUTPUTS:
*   source              - APS/PCC bytes source: \n 
*                         TRUE: RI_RPC \n 
*                         FALSE: non-selected 
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_odu_ri_rcp_aps_insert_en_get( odu_rcp_handle_t *hndl,    
                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                   UINT32 chnl,
                                                   BOOL8 *source)
{
    PMC_ERROR   rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check context */
    if (ODU_RCP_STATE_OPERATIONAL != hndl->var.ri_rcp_ctxt.state)
    {
        *source = FALSE;
        PMC_RETURN(PMC_SUCCESS);
    }
    otn_odu_ri_rcp_param_validate(hndl ,rcp_frm_id,chnl);      
        
    *source = hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl];

    PMC_RETURN(rc);
} /* otn_odu_ri_rcp_aps_insert_en_get */

/*******************************************************************************
* odu_rcp_di_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures one or multiple PDUs of DI RCP.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    pdu_msk          - PDU mask
*    enable           - when TRUE masked PDUs are enabled,
*                       when FALSE masked PDUs are disabled
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
PUBLIC PMC_ERROR odu_di_rcp_cfg(odu_rcp_handle_t *hndl, 
                                UINT32            pdu_msk,
                                BOOL8             enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_rcp_inst_state_t state;
    UINT32 new_pdu_msk = 0;
    UINT32 i;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    /* check PDU mask range of values */ 
   if ((pdu_msk >> (ODU_RCP_NUM_PDU_MAX)) != 0)
   {
       PMC_RETURN(ODU_RCP_ERR_INVALID_ARGS);
   }
    
   if ((((DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK >> DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF) | pdu_msk)
        ^  (DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK >> DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF)) != 0)
    { 
        PMC_RETURN(ODU_RCP_ERR_INVALID_ARGS);
    }     

    /* check that di rcp is operational */
    rc = odu_rcp_ctxt_state_get(hndl,
                                ODU_RCP_DI_INST,
                                &state);

    if(PMC_SUCCESS == rc && ODU_RCP_STATE_OPERATIONAL == state)
    {
        
        for (i = 0; i < ODU_RCP_NUM_PDU_MAX; i++)
        {
            if (((pdu_msk >> i) & 0x1) != 0)
            {
                new_pdu_msk |= ((((pdu_msk >> i) & (UINT32)enable & 0x1) << i));
            }
            else
            {
                new_pdu_msk |= (((hndl->var.di_rcp_ctxt.pdu_msk >> i) & 0x1) << i);
            }
        }
        
        /* Enable or disable PDUs */   
        di_rcp_field_REG_ETH_TX_EN_set(NULL, hndl, new_pdu_msk);    
        
        hndl->var.di_rcp_ctxt.pdu_msk = new_pdu_msk;
    }
    else
    {
        PMC_RETURN(ODU_RCP_STATE_NOT_PROVISIONED);
    }

    PMC_RETURN(rc);
}/*odu_di_rcp_cfg*/

/*******************************************************************************
* odu_rcp_di_drcohm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures DI RCP instance RCOHM defect status.
*
*   The function should be called after channel based STG 1 or N or 2 ODUk PDU
*   transfer enabling, otherwise an error code will be returned.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    stg              - ODU stage identifier
*    chnl_id          - channel identifier associated to defect
*    status           - defect status    
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
PUBLIC PMC_ERROR odu_rcp_di_drcohm_cfg(odu_rcp_handle_t           *hndl,  
                                       odu_rcp_frm_id_t            stg,
                                       UINT32                      chnl_id,
                                       BOOL8                       status)
{
    PMC_ERROR       rc = ODU_RCP_DI_PDU_NOT_ENABLED;
    UINT32 pdu_msk;
    UINT32 new_val = 0;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
     
    pdu_msk = hndl->var.di_rcp_ctxt.pdu_msk;
    
    switch(stg)
    {
    case ODU_RCP_FRM_STG1:        
        /* check that PDU is enabled */
        if (((pdu_msk >> ((UINT32)DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_CHNL)) & 0x1) != 0)
        {
            if (ODU_RCP_STG_1_N_NUM_CHNLS_MAX > chnl_id)
            {               
                /* build register new value */
                new_val = odu_rcp_set_bit(hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_1_N_DRCOHM_IDX].stg_31_00_d_rcohm_msk, 
                                          chnl_id, 
                                          status);

                di_rcp_reg_REG_STG_1_N_D_RCOHM_write(NULL, hndl, new_val);

                /* save drcohm mask in context */
                hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_1_N_DRCOHM_IDX].stg_31_00_d_rcohm_msk = new_val;

                rc = PMC_SUCCESS;
            }
        }
        break;
    case ODU_RCP_FRM_STGN:
        /* check that PDU is enabled */
        if (((pdu_msk >> ((UINT32)DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_CHNL)) & 0x1) != 0)
        {
            if (ODU_RCP_STG_1_N_NUM_CHNLS_MAX > chnl_id)
            {               
                /* build register new value */
                new_val = odu_rcp_set_bit(hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_1_N_DRCOHM_IDX].stg_31_00_d_rcohm_msk, 
                                          chnl_id, 
                                          status);

                di_rcp_reg_REG_STG_1_N_D_RCOHM_write(NULL, hndl, new_val);

                /* save drcohm mask in context */
                hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_1_N_DRCOHM_IDX].stg_31_00_d_rcohm_msk = new_val;

                rc = PMC_SUCCESS;
            }
        }
        
        break;    
    case ODU_RCP_FRM_STG2:
        /* check that PDU is enabled */
        if (((pdu_msk >> ((UINT32)DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_CHNL)) & 0x1) != 0)
        {
            if (0 <= chnl_id && 31 >= chnl_id)
            {
                /* build register new value */
                new_val = odu_rcp_set_bit(hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_31_00_d_rcohm_msk, 
                                          chnl_id, 
                                          status);

                di_rcp_reg_REG_STG_2_D_RCOHM1_write(NULL, hndl, new_val);

                /* save drcohm mask in context */
                hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_31_00_d_rcohm_msk = new_val;
                rc = PMC_SUCCESS;
            }
            else if (32 <= chnl_id && 63 >= chnl_id)
            {
                /* build register new value */
                new_val = odu_rcp_set_bit(hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_63_32_d_rcohm_msk, 
                                          chnl_id, 
                                          status);

                di_rcp_reg_REG_STG_2_D_RCOHM2_write(NULL, hndl, new_val);   

                /* save drcohm mask in context */
                hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_63_32_d_rcohm_msk = new_val;
                rc = PMC_SUCCESS;
            }
            else if (64 <= chnl_id && ODU_RCP_STG_2_NUM_CHNLS_MAX > chnl_id)
            {
                /* build register new value */
                new_val = odu_rcp_set_bit(hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_95_64_d_rcohm_msk, 
                                          chnl_id, 
                                          status);

                di_rcp_reg_REG_STG_2_D_RCOHM3_write(NULL, hndl, new_val);

                /* save drcohm mask in context */
                hndl->var.di_rcp_ctxt.drcohm_cfg[ODU_RCP_STG_2_DRCOHM_IDX].stg_95_64_d_rcohm_msk = new_val;
                rc = PMC_SUCCESS;
            }
            else
            {
                rc = ODU_RCP_ERR_INVALID_ARGS;
            }
        }
        break;
    default:
        rc = ODU_RCP_ERR_INVALID_ARGS;
        break;
    }

    PMC_RETURN(rc);
} /* odu_rcp_di_drcohm_cfg */

/*******************************************************************************
* odu_rcp_di_pdu_msk_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns DI RCP PDU mask and SDH counts select mask.
*
*
*
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*
* OUTPUTS:
*   *pdu_msk_ptr          - PDU mask.
*   *sdh_cnt_sel_ptr      - SDH counts select mask
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_rcp_di_pdu_msk_get(odu_rcp_handle_t           *hndl,
                                   UINT32                     *pdu_msk_ptr,
                                   UINT16                     *sdh_cnt_sel_ptr)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != pdu_msk_ptr, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    PMC_ASSERT(NULL != sdh_cnt_sel_ptr, ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    *pdu_msk_ptr = hndl->var.di_rcp_ctxt.pdu_msk;
    *sdh_cnt_sel_ptr = hndl->var.di_rcp_ctxt.sdh_cnt_sel;

    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_di_sdh_cnt_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set DI RCP SDH counts select mask.
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    sdh_cnt_sel      - SDH counts select mask
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
PUBLIC void odu_rcp_di_sdh_cnt_sel_set(odu_rcp_handle_t         *hndl,
                                       UINT16                    sdh_cnt_sel)
{
    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    
    hndl->var.di_rcp_ctxt.sdh_cnt_sel = sdh_cnt_sel;

    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ODU_RCP 
*
* INPUTS:
*   *hndl                   - pointer to ODU_RCP handle instance
*    odu_rcp_id             - identify the RI or DI RCP instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if RCP is in start state, else FALSE
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL odu_rcp_start_state_test(odu_rcp_handle_t   *hndl,
                                     odu_rcp_inst_t      odu_rcp_id)
{
    odu_rcp_inst_state_t   state;
    PMC_ENTRY();
    
    (void) odu_rcp_ctxt_state_get(hndl,odu_rcp_id, &state);
    if (ODU_RCP_STATE_START !=state)
    {
        PMC_RETURN(FALSE);    
    }
    PMC_RETURN(TRUE);    
} /* odu_rcp_start_state_test */

/*******************************************************************************
* odu_rcp_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean register and memory context of specied channels. 
*                                                                                                                                                     
* INPUTS:                                                                       
*   *hndl          - pointer to ODUKSC handle instance to be operated on
*   is_rx_frm      - indicate if it is a CFC associated to a RX framer 
*   rcp_frm_id     - ODU stage identifier
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
PUBLIC void odu_rcp_chnl_clean(odu_rcp_handle_t   *hndl,
                               BOOL8 is_rx_frm,                               
                               odu_rcp_frm_id_t rcp_frm_id,
                               UINT32 *ch_entries,
                               UINT32 *chnl,
                               UINT32 num_chnl)
{   
    digi_otn_odu_ri_rcp_dst_cfg_t dummy_dst_cfg;
    UINT32 cfc_id;
    UINT32 chnl_itr;
    PMC_ENTRY();

    /* CFC fifo cleanup */
    cfc_id  = (UINT32) odu_rcp_frm_cfc_id_convert(rcp_frm_id, is_rx_frm);
    cfc_fifo_clean(hndl->cfc_handle[cfc_id],chnl,num_chnl);
        
    /* update DA lookup */
    dummy_dst_cfg.dst_channel = 0;
    dummy_dst_cfg.dst_frm = 0;
    dummy_dst_cfg.dst_digi = 0;
    for(chnl_itr = 0; chnl_itr < num_chnl; chnl_itr++)
    {
        if (is_rx_frm == TRUE)
        {
            otn_odu_ri_rcp_da_lookup_cfg(hndl,rcp_frm_id,chnl[chnl_itr],&dummy_dst_cfg);
            /* configure APS/PCC */
            (void) otn_odu_ri_rcp_aps_pcc_cfg(hndl,rcp_frm_id,chnl[chnl_itr],0);
            hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][chnl[chnl_itr]] = FALSE;
        } 
        else 
        {
            (void) otn_odu_ri_rcp_insert_backward_defect_enable_cfg(hndl,rcp_frm_id,chnl[chnl_itr],FALSE);
            hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[(UINT32) rcp_frm_id][chnl[chnl_itr]] = FALSE;
            (void) otn_odu_ri_rcp_insert_aps_enable_cfg(hndl,rcp_frm_id,chnl[chnl_itr],FALSE);
            hndl->var.ri_rcp_ctxt.insert_aps_enable[(UINT32) rcp_frm_id][chnl[chnl_itr]] = FALSE;
        }
    }
    if (is_rx_frm == TRUE)
    {
        /* resync calendar enable */
        hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id] = ri_rcp_field_range_REG_SCH_CAL_EN_get(NULL,hndl,(UINT32) rcp_frm_id,(UINT32) rcp_frm_id);
        (void) otn_odu_ri_rcp_scheduler_sync(hndl,rcp_frm_id,ch_entries);
    }
    
    PMC_RETURN();
} /* odu_rcp_chnl_clean */

/*******************************************************************************
* FUNCTION: odu_rcp_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables unchannelized interrupts based on bits that are set in 
*   DI or RI RCP interrupt table.
*   
*
* INPUTS:
*   odu_rcp_handle    - Pointer to subsystem handle
*   rcp_inst          - identify the RI or DI RCP instance
*   int_table_ptr     - pointer to DI or RI RCP interrupt table with
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
PUBLIC PMC_ERROR odu_rcp_int_enable(odu_rcp_handle_t    *odu_rcp_handle,
                                    odu_rcp_inst_t       rcp_inst,
                                    odu_rcp_int_t       *int_table_ptr,
                                    odu_rcp_int_t       *int_en_table_ptr,
                                    BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    if (LAST_ODU_RCP_INST > rcp_inst)
    {
        /* enable enet fege interrupts */
        enet_fege_int_enable(odu_rcp_handle->enet_fege_handle[(UINT32)rcp_inst], &int_table_ptr->enet_fege, &int_en_table_ptr->enet_fege, enable);
        
        if (ODU_RCP_DI_INST == rcp_inst)
        {
            #ifdef ODU_DI_RCP_INT
            #undef ODU_DI_RCP_INT
            #endif
            #define ODU_DI_RCP_INT     UTIL_GLOBAL_INT_ENABLE_SINGLE

            ODU_DI_RCP_INT_TABLE_DEFINE();
        }
    }
    else
    {
        result = ODU_RCP_ERR_INVALID_ARGS;
    }

    PMC_RETURN(result);
} /* odu_rcp_int_enable */


/*******************************************************************************
* FUNCTION: odu_rcp_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DI or RI RCP unchannelized interrupts.
*   The desired interrupt/status bits to retrieve (to reduce function execution
*   time) can be specified using the filter interrupt table input.
*
* INPUTS:
*   odu_rcp_handle  - Pointer to subsystem handle
*   rcp_inst        - identify the RI or DI RCP instance
*   filt_table_ptr  - filter interrupt table. 
*                     When NULL, all interrupt are retrieved.
*
* OUTPUTS:
*   int_table_ptr   - pointer to DI or RI RCP interrupt table to fill with 
*                     retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rcp_int_retrieve(odu_rcp_handle_t    *odu_rcp_handle,
                                      odu_rcp_inst_t       rcp_inst,
                                      odu_rcp_int_t       *filt_table_ptr,
                                      odu_rcp_int_t       *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    if (LAST_ODU_RCP_INST > rcp_inst)
    {
        /* retrieve enet fege interrupts */
        enet_fege_int_retrieve(odu_rcp_handle->enet_fege_handle[(UINT32)rcp_inst], &filt_table_ptr->enet_fege, &int_table_ptr->enet_fege);
        

        if (ODU_RCP_DI_INST == rcp_inst)
        {
            UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_rcp_handle, di_rcp, UTIL_GLOBAL_INT_OPTIMIZE);

            #ifdef ODU_DI_RCP_INT
            #undef ODU_DI_RCP_INT
            #endif
            #define ODU_DI_RCP_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    

            ODU_DI_RCP_INT_TABLE_DEFINE();

            UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_rcp_handle, di_rcp);
        }
    }
    else
    {
        result = ODU_RCP_ERR_INVALID_ARGS;
    }


    PMC_RETURN(result);
} /* odu_rcp_int_retrieve */

/*******************************************************************************
* FUNCTION: odu_rcp_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears DI or RI RCP unchannelized interrupts based on bits that are set in 
*   DI or RI RCP interrupt table.
*   
*
* INPUTS:
*   odu_rcp_handle    - Pointer to subsystem handle
*   rcp_inst          - identify the RI or DI RCP instance
*   int_table_ptr     - pointer to DI or RI RCP interrupt table with
*                       bits to clear
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rcp_int_clear(odu_rcp_handle_t    *odu_rcp_handle,
                                   odu_rcp_inst_t       rcp_inst,
                                   odu_rcp_int_t       *int_table_ptr,
                                   odu_rcp_int_t       *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    enet_fege_int_t *enet_fege_int_en_ptr = NULL;
    PMC_ENTRY();

    if (LAST_ODU_RCP_INST > rcp_inst)
    {
        /* clear enet fege interrupts */
        if (NULL != int_en_table_ptr)
        {
            enet_fege_int_en_ptr = &int_en_table_ptr->enet_fege;
        }

        enet_fege_int_clear(odu_rcp_handle->enet_fege_handle[(UINT32)rcp_inst],
                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, enet_fege),
                            enet_fege_int_en_ptr);
        
        if (ODU_RCP_DI_INST == rcp_inst)
        {
            #ifdef ODU_DI_RCP_INT
            #undef ODU_DI_RCP_INT
            #endif

            #define ODU_DI_RCP_INT    UTIL_GLOBAL_INT_CLEAR_SINGLE

            ODU_DI_RCP_INT_TABLE_DEFINE();
        }
    }
    else
    {
        result = ODU_RCP_ERR_INVALID_ARGS;
    }

    PMC_RETURN(result);
} /* odu_rcp_int_clear */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_sel_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Select RI RCP CFC interrupt source and framer.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle.
*   framer_id        - framer from which CFC interrupts are extracted.
*   src_dir          - CFC framer direction: TX or RX.
*                      UTIL_GLOBAL_DIR_BOTH is not supported.  
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
PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_sel_set(odu_rcp_handle_t        *ri_rcp_handle,
                                            odu_rcp_frm_id_t         framer_id,
                                            util_global_direction_t  src_dir)
{
    PMC_ERROR result = PMC_SUCCESS;
    BOOL8 sel = TRUE;

    PMC_ENTRY();
    
    switch (src_dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        sel = FALSE;
        break;
    case UTIL_GLOBAL_DIR_TX:
        break;
    default:
        result = ODU_RCP_ERR_INVALID_ARGS;
        break;
    }

    if (PMC_SUCCESS == result)
    {
        if (framer_id >= LAST_ODU_RCP_FRM_ID)
        {
            result = ODU_RCP_ERR_INVALID_ARGS;
        }
    }

    if (PMC_SUCCESS == result)
    {
        ri_rcp_field_REG_CFC_IRQ_SRC_SEL_set(NULL, ri_rcp_handle, (UINT32)sel);
        ri_rcp_field_REG_CFC_IRQ_STG_SEL_set(NULL, ri_rcp_handle, (UINT32)framer_id);
    }
    
    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_src_sel_set */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_sel_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables RI RCP channelized interrupts based on bits that are set in 
*   RI RCP interrupt table.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
*
* OUTPUTS:
*   framer_id_ptr        - selected framer
*   src_dir_ptr          - framer direction: TX or RX.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_ri_rcp_cfc_int_sel_get(odu_rcp_handle_t        *ri_rcp_handle,
                                       odu_rcp_frm_id_t        *framer_id_ptr,
                                       util_global_direction_t *src_dir_ptr)
{

    PMC_ENTRY();
    
    *src_dir_ptr = 0x1 == ri_rcp_field_REG_CFC_IRQ_SRC_SEL_get(NULL, ri_rcp_handle) ? UTIL_GLOBAL_DIR_TX : UTIL_GLOBAL_DIR_RX;
    *framer_id_ptr = (odu_rcp_frm_id_t)ri_rcp_field_REG_CFC_IRQ_STG_SEL_get(NULL, ri_rcp_handle);
    
    PMC_RETURN();
} /* odu_ri_rcp_cfc_int_sel_get */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_type_sel_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects RI RCP CFC interrupt type: underflow or overflow interrupt.
*   
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle.
*   chnl             - channel id
*   int_type         - interrupt type.  
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
PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_type_sel_set(odu_rcp_handle_t          *ri_rcp_handle,
                                                 UINT32                     chnl,
                                                 odu_ri_rcp_cfc_int_type_t  int_type)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (LAST_ODU_RI_RCP_CFC_TYPE <= int_type)
    {
        result = ODU_RCP_ERR_INVALID_ARGS;
    }

    if (PMC_SUCCESS == result)
    {
        if (ODU_RI_RCP_STG234_NUM_CHNL <= chnl)
        {
            result = ODU_RCP_ERR_INVALID_ARGS;
        }
    }

    if (PMC_SUCCESS == result)
    {
        ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_set(NULL, ri_rcp_handle, chnl, chnl, (UINT32)int_type);
    }
    
    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_type_sel_set */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_type_sel_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns RI RCP CFC selected interrupt type: underflow or overflow interrupt.
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
*   chnl             - channel id
*
* OUTPUTS:
*   int_type_ptr     - interrupt type.  
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*         
*******************************************************************************/
PUBLIC PMC_ERROR odu_ri_rcp_cfc_int_type_sel_get(odu_rcp_handle_t          *ri_rcp_handle,
                                                 UINT32                     chnl,
                                                 odu_ri_rcp_cfc_int_type_t *int_type_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (ODU_RI_RCP_STG234_NUM_CHNL <= chnl)
    {
        result = ODU_RCP_ERR_INVALID_ARGS;
    }

    if (PMC_SUCCESS == result)
    {
        *int_type_ptr = (odu_ri_rcp_cfc_int_type_t)ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_get(NULL, ri_rcp_handle, chnl, chnl);
    }
    
    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_type_sel_get */


/*******************************************************************************
* FUNCTION: odu_ri_rcp_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables RI RCP channelized interrupts based on bits that are set in 
*   RI RCP interrupt table.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   int_table_ptr     - pointer to ri_rcp interrupt table with
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
PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_enable(odu_rcp_handle_t        *ri_rcp_handle,
                                            UINT32                  chnl_id,
                                            odu_ri_rcp_int_chnl_t  *int_table_ptr,
                                            odu_ri_rcp_int_chnl_t  *int_en_table_ptr,
                                            BOOL                    enable)
{
    PMC_ERROR result;

    PMC_ENTRY();

    result = odu_ri_rcp_cfc_int_chnl_enable_internal(ri_rcp_handle,
                                                     chnl_id,
                                                     &int_table_ptr->cfc,
                                                     &int_en_table_ptr->cfc,
                                                     enable);
    
    PMC_RETURN(result);
} /* odu_ri_rcp_int_chnl_enable */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all channelized interrupts to the ri_rcp interrupt table.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   filt_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - pointer to ri_rcp interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_retrieve(odu_rcp_handle_t        *ri_rcp_handle,
                                              UINT32                  chnl_id,
                                              odu_ri_rcp_int_chnl_t  *filt_table_ptr,
                                              odu_ri_rcp_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_ri_rcp_cfc_int_chnl_retrieve_internal(ri_rcp_handle,
                                                       chnl_id,
                                                       &filt_table_ptr->cfc,
                                                       &int_table_ptr->cfc);

    PMC_RETURN(result);
} /* odu_ri_rcp_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: odu_ri_rcp_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channelized interrupts based on bits that are set in 
*   ri_rcp interrupt table.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
*   chnl_id           - channel ID  
*   int_table_ptr     - pointer to ri_rcp interrupt table with
*                       bits set to clear interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_clear(odu_rcp_handle_t      *ri_rcp_handle,
                                           UINT32                 chnl_id,
                                           odu_ri_rcp_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_ri_rcp_cfc_int_chnl_clear_internal(ri_rcp_handle,
                                                    chnl_id,
                                                    &int_table_ptr->cfc);

    PMC_RETURN(result);
} /* odu_ri_rcp_int_chnl_clear */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channelized interrupts based on bits that are set in 
*   ri_rcp interrupt table.
*   
*
* INPUTS:
*   ri_rcp_handle    - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_ri_rcp_int_chnl_enabled_check(odu_rcp_handle_t       *ri_rcp_handle,
                                                   UINT32                  chnl_id,
                                                   odu_ri_rcp_int_chnl_t  *int_en_table_ptr,
                                                   BOOL8                  *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_ri_rcp_cfc_int_chnl_enabled_check_internal(ri_rcp_handle,
                                                    chnl_id,
                                                    &int_en_table_ptr->cfc,
                                                    int_found_ptr);

    PMC_RETURN(result);
} /* odu_ri_rcp_int_chnl_enabled_check */



/* private functions */
/*******************************************************************************
* FUNCTION: odu_rcp_cfc_int_chnl_enable_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cfc_handle        - Pointer to CFC handle
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
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
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_enable_internal(cfc_handle_t     *cfc_handle,
                                                       UINT32            fifo_id,
                                                       cfc_int_chnl_t   *int_table_ptr,
                                                       cfc_int_chnl_t   *int_en_table_ptr,
                                                       BOOL8            enable)
{
    PMC_ERROR       result = ODU_RCP_ERR_CODE_ASSERT;

    PMC_ENTRY();
    PMC_ASSERT(0, ODU_RCP_ERR_CODE_ASSERT, 0, 0);
    
    PMC_RETURN(result);
} /* odu_rcp_cfc_int_chnl_enable_internal */

/*******************************************************************************
* FUNCTION: odu_rcp_cfc_int_chnl_clear_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cfc_handle        - Pointer to CFC handle
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_clear_internal(cfc_handle_t    *cfc_handle,
                                                      UINT32          fifo_id,
                                                      cfc_int_chnl_t  *int_en_table_ptr,
                                                      cfc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = ODU_RCP_ERR_CODE_ASSERT;

    PMC_ENTRY();
    PMC_ASSERT(0, ODU_RCP_ERR_CODE_ASSERT, 0, 0);
    
    PMC_RETURN(result);
} /* odu_rcp_cfc_int_chnl_clear_internal */

/*******************************************************************************
* FUNCTION: odu_rcp_cfc_int_chnl_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cfc_handle        - Pointer to CFC handle
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
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_retrieve_internal(cfc_handle_t    *cfc_handle,
                                                         UINT32          fifo_id,
                                                         cfc_int_chnl_t  *int_en_table_ptr,
                                                         cfc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = ODU_RCP_ERR_CODE_ASSERT;

    PMC_ENTRY();
    PMC_ASSERT(0, ODU_RCP_ERR_CODE_ASSERT, 0, 0);
    
    PMC_RETURN(result);
} /* odu_rcp_cfc_int_chnl_retrieve_internal */

/*******************************************************************************
* FUNCTION: odu_rcp_cfc_int_chnl_enabled_check_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cfc_handle        - Pointer to CFC handle
*   fifo_id           - Channel ID: Valid range: 0 - 95
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
PRIVATE PMC_ERROR odu_rcp_cfc_int_chnl_enabled_check_internal(cfc_handle_t   *cfc_handle,
                                                              UINT32         fifo_id,
                                                              cfc_int_chnl_t *int_en_table_ptr,
                                                              BOOL8          *int_found_ptr)
{
    PMC_ERROR       result = ODU_RCP_ERR_CODE_ASSERT;

    PMC_ENTRY();
    PMC_ASSERT(0, ODU_RCP_ERR_CODE_ASSERT, 0, 0);
    
    PMC_RETURN(result);
} /* odu_rcp_cfc_int_chnl_enabled_check_internal */

/*******************************************************************************
*  otn_odu_ri_rcp_param_validate
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   This helper function is used to validate parameter of RI RCP functions. \n 
*   Whenever there is a problem, it asserts
*
* INPUTS:
*   *hndl           - pointer to ODU_RCP handle instance
*   rcp_frm_id      - The framer stage 
*   chnl            - The channel id
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
 PRIVATE void otn_odu_ri_rcp_param_validate(odu_rcp_handle_t *hndl,       
                                            odu_rcp_frm_id_t rcp_frm_id,
                                            UINT32 chnl)
                                             
{

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((UINT32)LAST_ODU_RCP_FRM_ID > (UINT32)rcp_frm_id, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    if (ODU_RCP_FRM_STG1 == rcp_frm_id || ODU_RCP_FRM_STGN ==  rcp_frm_id) 
    {
        PMC_ASSERT(ODU_RI_RCP_STG1N_NUM_CHNL > chnl, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    } else 
    {
        PMC_ASSERT(ODU_RI_RCP_STG234_NUM_CHNL > chnl, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    }
    
    PMC_RETURN();
}


/*******************************************************************************
* otn_odu_ri_rcp_insert_backward_defect_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   Enable insertion of backward defect on the specified framer/channel
*
* INPUTS:
*   *hndl           - pointer to ODU_RCP handle instance
*   rcp_frm_id      - The framer stage 
*   chnl            - The channel id
*   source          - Transmit BDI/BEI bits source: \n 
*                      TRUE: RI_RPC \n 
*                      FALSE: Local Rx Framer 
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_insert_backward_defect_enable_cfg(odu_rcp_handle_t *hndl,       
                                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                                   UINT32 chnl,
                                                                   BOOL8 source)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    switch(rcp_frm_id)
    {
    case ODU_RCP_FRM_STG1:
        ri_rcp_field_range_REG_TX_STG_1_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STGN:
        ri_rcp_field_range_REG_TX_STG_N_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG2:
        ri_rcp_lfield_range_REG_TX_STG_2_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG3A:        
        ri_rcp_lfield_range_REG_TX_STG_3A_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG3B:
        ri_rcp_lfield_range_REG_TX_STG_3B_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG4:
        ri_rcp_lfield_range_REG_TX_STG_4_RCP_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    default:
        break;
    }
           
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_insert_aps_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   Enable insertion of APS on the specified framer/channel
*
* INPUTS:
*   *hndl           - pointer to ODU_RCP handle instance
*   rcp_frm_id      - The framer stage 
*   chnl            - The channel id
*   source          - Transmit BDI/BEI bits source: \n 
*                      TRUE: RI_RPC \n 
*                      FALSE: Local Rx Framer 
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_insert_aps_enable_cfg(odu_rcp_handle_t *hndl,       
                                                       odu_rcp_frm_id_t rcp_frm_id,
                                                       UINT32 chnl,
                                                       BOOL8 source)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();


    switch(rcp_frm_id)
    {
    case ODU_RCP_FRM_STG1:
        ri_rcp_field_range_REG_TX_STG_1_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STGN:
        ri_rcp_field_range_REG_TX_STG_N_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG2:
        ri_rcp_lfield_range_REG_TX_STG_2_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG3A:        
        ri_rcp_lfield_range_REG_TX_STG_3A_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG3B:
        ri_rcp_lfield_range_REG_TX_STG_3B_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    case ODU_RCP_FRM_STG4:
        ri_rcp_lfield_range_REG_TX_STG_4_APS_INS_EN_set(NULL,hndl,chnl,chnl,source);
        break;
    default:
        break;
    }
           
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_cfc_fifo_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure RI RCP CFC instances
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   is_rx_frm         - indicate if it is a CFC associated to a RX framer 
*   rcp_frm_id        - identify the RI or DI RCP instance
*   chnl              - channel id
*   num_cal_entries   - Number of calendar entries
*   enable            - TRUE: we enable the CFC instance \n
*                       FALSE: we disable the CFC instance 
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_cfc_fifo_cfg(odu_rcp_handle_t  *hndl,
                                              BOOL8 is_rx_frm,
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 num_cal_entries,
                                              BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 cfc_id;
    PMC_ENTRY();
    
    /* get cfc id */
    cfc_id  = (UINT32) odu_rcp_frm_cfc_id_convert(rcp_frm_id, is_rx_frm);

    if (TRUE == enable)
    {
        /*the calendar is based on a 96 slot calendar*/
        if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
        {
            PMC_ASSERT(num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80, ODU_RCP_ERR_BAD_DB_ENTRY, 0, 0);
            num_cal_entries /= 8;
        }
        
        if (PMC_SUCCESS == rc) 
        {
            rc = cfc_fifo_build(hndl->cfc_handle[cfc_id], chnl, num_cal_entries);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = cfc_fifo_activate(hndl->cfc_handle[cfc_id], chnl);
        }
    } 
    else 
    {
        rc = cfc_fifo_deactivate(hndl->cfc_handle[cfc_id], chnl);
        if (PMC_SUCCESS == rc )
        {
            rc = cfc_fifo_destroy(hndl->cfc_handle[cfc_id], chnl);
        }
    }
    
    PMC_RETURN(rc);
} /* otn_odu_ri_rcp_cfc_fifo_cfg */

/*******************************************************************************
* otn_odu_ri_rcp_cfc_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize RI RCP CFCs
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   is_rx_frm         - indicate if it is a CFC associated to a RX framer 
*   rcp_frm_id        - identify the RI or DI RCP instance
*   chnl              - channel id
*   num_cal_entries   - Number of calendar entries
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_cfc_fifo_resize(odu_rcp_handle_t  *hndl,
                                              BOOL8 is_rx_frm,
                                              odu_rcp_frm_id_t rcp_frm_id,
                                              UINT32 chnl,
                                              UINT32 num_cal_entries)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 cfc_id;
    PMC_ENTRY();
    
    /* get cfc id */
    cfc_id  = (UINT32) odu_rcp_frm_cfc_id_convert(rcp_frm_id, is_rx_frm);

    /*the calendar is based on a 96 slot calendar*/
    if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
    {
        PMC_ASSERT(num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80, ODU_RCP_ERR_BAD_DB_ENTRY, 0, 0);
        num_cal_entries /= 8;
    }
    
    if (PMC_SUCCESS == rc) 
    {
        rc = cfc_fifo_resize_ext(hndl->cfc_handle[cfc_id], chnl, num_cal_entries, 1000, 1000); /* num iterations, usec/iteration */
    }
    
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_da_lookup_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure DA lookup table 
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    rcp_frm_id       - identify the RI or DI RCP instance
*    chnl              - channel id
*    *ri_rcp_dst_cfg_ptr - pointer to destination config
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_da_lookup_cfg(odu_rcp_handle_t  *hndl,
                                               odu_rcp_frm_id_t rcp_frm_id,
                                               UINT32 chnl, 
                                               digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr)
{
    ri_rcp_buffer_t ri_b[1];

    UINT32 idx;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    
    ri_rcp_buffer_init(ri_b,hndl);

    if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
    {
        /* Address[14:0]    Type    Function */
        /* 0x0080 + 4*N (Channel N=0:11)    Rx Framer Stage 1    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x00C0 + 4*N (Channel N=0:11)    Rx Framer Stage N    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        idx = chnl;
        if (ODU_RCP_FRM_STGN == rcp_frm_id)
        {
            idx += 0x10;
        }        
        ri_rcp_field_HO_ODU_DST_CHANNEL_set(ri_b, hndl,idx ,ri_rcp_dst_cfg_ptr->dst_channel);
        ri_rcp_field_HO_ODU_DST_FRM_set(ri_b, hndl, idx,ri_rcp_dst_cfg_ptr->dst_frm);
        ri_rcp_field_HO_ODU_DST_DIGI_set(ri_b, hndl, idx,ri_rcp_dst_cfg_ptr->dst_digi);
    } else 
    {
        /* Address[14:0]    Type    Function */        
        /* 0x0100 + 4*N (Channel N=0:95)    Rx Framer Stage 2    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0300 + 4*N (Channel N=0:95)    Rx Framer Stage 3a    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0500 + 4*N (Channel N=0:95)    Rx Framer Stage 3b    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0700 + 4*N (Channel N=0:95)    Rx Framer Stage N    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        idx = chnl;
        if (ODU_RCP_FRM_STG3A == rcp_frm_id)
        {
            idx += 0x80;
        } else if (ODU_RCP_FRM_STG3B == rcp_frm_id)
        {
            idx += 0x100;
        } else if (ODU_RCP_FRM_STG4 == rcp_frm_id)
        {
            idx += 0x180;
        }    
        
        ri_rcp_field_LO_ODU_DST_CHANNEL_set(ri_b, hndl,idx ,ri_rcp_dst_cfg_ptr->dst_channel);
        ri_rcp_field_LO_ODU_DST_FRM_set(ri_b, hndl, idx,ri_rcp_dst_cfg_ptr->dst_frm);
        ri_rcp_field_LO_ODU_DST_DIGI_set(ri_b, hndl, idx,ri_rcp_dst_cfg_ptr->dst_digi);
    }

    ri_rcp_buffer_flush(ri_b);
    
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_da_lookup_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets DA lookup table 
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*    rcp_frm_id         - identify the RI or DI RCP instance
*    chnl               - channel id
*
* OUTPUTS:
*   *ri_rcp_dst_cfg_ptr - pointer to destination config
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR otn_odu_ri_rcp_da_lookup_cfg_get(odu_rcp_handle_t  *hndl,
                                                   odu_rcp_frm_id_t rcp_frm_id,
                                                   UINT32 chnl, 
                                                   digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr)
{
    ri_rcp_buffer_t ri_b[1];

    UINT32 idx;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    ri_rcp_buffer_init(ri_b,hndl);
    
    if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
    {
        /* Address[14:0]    Type    Function */
        /* 0x0080 + 4*N (Channel N=0:11)    Rx Framer Stage 1    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x00C0 + 4*N (Channel N=0:11)    Rx Framer Stage N    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        idx = chnl;
        if (ODU_RCP_FRM_STGN == rcp_frm_id)
        {
            idx += 0x10;
        }        
        ri_rcp_dst_cfg_ptr->dst_channel = ri_rcp_field_HO_ODU_DST_CHANNEL_get(ri_b, hndl,idx);
        ri_rcp_dst_cfg_ptr->dst_frm = ri_rcp_field_HO_ODU_DST_FRM_get(ri_b, hndl, idx);
        ri_rcp_dst_cfg_ptr->dst_digi = ri_rcp_field_HO_ODU_DST_DIGI_get(ri_b, hndl, idx);
    } else 
    {
        /* Address[14:0]    Type    Function */        
        /* 0x0100 + 4*N (Channel N=0:95)    Rx Framer Stage 2    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0300 + 4*N (Channel N=0:95)    Rx Framer Stage 3a    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0500 + 4*N (Channel N=0:95)    Rx Framer Stage 3b    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        /* 0x0700 + 4*N (Channel N=0:95)    Rx Framer Stage N    DST_DIGI[[23:16], DST_FRM[10:8] DST_CHANNEL[6:0] */
        idx = chnl;
        if (ODU_RCP_FRM_STG3A == rcp_frm_id)
        {
            idx += 0x80;
        } else if (ODU_RCP_FRM_STG3B == rcp_frm_id)
        {
            idx += 0x100;
        } else if (ODU_RCP_FRM_STG4 == rcp_frm_id)
        {
            idx += 0x180;
        }
        ri_rcp_dst_cfg_ptr->dst_channel = ri_rcp_field_LO_ODU_DST_CHANNEL_get(ri_b, hndl,idx);
        ri_rcp_dst_cfg_ptr->dst_frm = ri_rcp_field_LO_ODU_DST_FRM_get(ri_b, hndl, idx);
        ri_rcp_dst_cfg_ptr->dst_digi = ri_rcp_field_LO_ODU_DST_DIGI_get(ri_b, hndl, idx);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_aps_pcc_cfg
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Configures the APS/PCC connection monitoring for an ODU framer channel. 
*   
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*   rcp_frm_id            - The framer stage 
*   chnl                  - The channel id
*   aps_pcc_mon_level     - The APS/PCC connection monitoring level  (0 to 7)
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_aps_pcc_cfg(odu_rcp_handle_t  *hndl,
                                             odu_rcp_frm_id_t rcp_frm_id,
                                             UINT32 chnl,
                                             UINT8 aps_pcc_mon_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 val;

    PMC_ENTRY();

    val = aps_pcc_mon_level;

    switch(rcp_frm_id)
    {
    case ODU_RCP_FRM_STG1:
        ri_rcp_field_REG_RX_STG_1_APS_LEVEL_set(NULL,hndl,chnl,val);
        break;
    case ODU_RCP_FRM_STGN:
        ri_rcp_field_REG_RX_STG_N_APS_LEVEL_set(NULL,hndl,chnl,val);
        break;
    case ODU_RCP_FRM_STG2:
        ri_rcp_field_REG_RX_STG_2_APS_LEVEL_set(NULL,hndl,chnl,val);        
        break;
    case ODU_RCP_FRM_STG3A:        
        ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_set(NULL,hndl,chnl,val);
        break;
    case ODU_RCP_FRM_STG3B:
        ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_set(NULL,hndl,chnl,val);
        break;
    case ODU_RCP_FRM_STG4:
        ri_rcp_field_REG_RX_STG_4_APS_LEVEL_set(NULL,hndl,chnl,val);
        break;
    default:
        break;
    }
           
    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_aps_pcc_cfg_get
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Gets the APS/PCC connection monitoring level for an ODU framer channel. 
*   
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*   rcp_frm_id            - The framer stage 
*   chnl                  - The channel id
*   
* OUTPUTS:
*   aps_pcc_mon_level     - The APS/PCC connection monitoring level  (0 to 7)
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR otn_odu_ri_rcp_aps_pcc_cfg_get(odu_rcp_handle_t  *hndl,
                                                 odu_rcp_frm_id_t rcp_frm_id,
                                                 UINT32 chnl,
                                                 UINT8 *aps_pcc_mon_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 val = 0;

    PMC_ENTRY();

    switch(rcp_frm_id)
    {
    case ODU_RCP_FRM_STG1:
        val = ri_rcp_field_REG_RX_STG_1_APS_LEVEL_get(NULL,hndl,chnl);
        break;
    case ODU_RCP_FRM_STGN:
        val = ri_rcp_field_REG_RX_STG_N_APS_LEVEL_get(NULL,hndl,chnl);
        break;
    case ODU_RCP_FRM_STG2:
        val = ri_rcp_field_REG_RX_STG_2_APS_LEVEL_get(NULL,hndl,chnl);        
        break;
    case ODU_RCP_FRM_STG3A:        
        val = ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_get(NULL,hndl,chnl);
        break;
    case ODU_RCP_FRM_STG3B:
        val = ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_get(NULL,hndl,chnl);
        break;
    case ODU_RCP_FRM_STG4:
        val = ri_rcp_field_REG_RX_STG_4_APS_LEVEL_get(NULL,hndl,chnl);
        break;
    default:
        break;
    }

    *aps_pcc_mon_level = val;

    PMC_RETURN(rc);
}

/*******************************************************************************
* otn_odu_ri_rcp_scheduler_sync
* ______________________________________________________________________________
* 
* DESCRIPTION: 
*  
*   Synchronize the scheduler of a specified framer with proper DB
*   
* INPUTS:
*   *hndl                 - pointer to ODU_RCP handle instance
*   rcp_frm_id            - The framer stage 
*   ch_entries            - number of calendar entries per ch
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
PRIVATE PMC_ERROR otn_odu_ri_rcp_scheduler_sync(odu_rcp_handle_t  *hndl,
                                                odu_rcp_frm_id_t rcp_frm_id,
                                                UINT32 *ch_entries)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 num_cal_entries = 0;   
    UINT8 num_element;
    BOOL8 cal_empty = TRUE;
    UINT8 itr, jtr, chnl_cnt;
    UINT8 new_calendar[ODU_RI_RCP_STG234_NUM_CHNL];
    BOOL8 update_cal_en = FALSE;
    BOOL8 update_cal = FALSE;
    UINT32 cal_val[12];
    ri_rcp_buffer_t ri_b[1];

    PMC_ENTRY();

    PMC_MEMSET(ri_b, 0, sizeof(ri_rcp_buffer_t));

    if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
    {
        num_element = ODU_RI_RCP_STG1N_NUM_CHNL;
    }
    else
    {
        num_element = ODU_RI_RCP_STG234_NUM_CHNL;
    }

    /* synchronize calendar with DB */
    /* initialize it */
    for (itr = 0; itr < num_element; itr ++)
    {
        new_calendar[itr] = ODU_RI_RCP_NULL_CHNL;
    }
    
    /* get entries from DB */
    for (itr = 0; itr < num_element && PMC_SUCCESS == rc ; itr ++)
    {
        if (TRUE == hndl->var.ri_rcp_ctxt.extract_rx_enable[(UINT32) rcp_frm_id][itr]&& 
            PMC_SUCCESS == rc )
        {
            num_cal_entries = ch_entries[itr];

            if (0 != num_cal_entries)
            {
                cal_empty  = FALSE;                
                /* handle the 12 channel case */
                if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id) 
                { 
                    PMC_ASSERT(num_cal_entries == 8 || num_cal_entries == 32 || num_cal_entries == 80, ODU_RCP_ERR_BAD_DB_ENTRY, 0, 0);
                    num_cal_entries /= 8;
                }

                chnl_cnt = 0;
                for (jtr = 0; jtr < num_element && chnl_cnt < num_cal_entries; jtr ++)
                {
                    if (ODU_RI_RCP_NULL_CHNL == new_calendar[jtr] )
                    {
                        new_calendar[jtr] = itr;
                        chnl_cnt += 1;
                    }
                }
                PMC_ASSERT(num_cal_entries == chnl_cnt, ODU_RCP_ERR_BAD_DB_ENTRY, 0, 0);                    
            }
        }
    }   
    
    /* update the calendar */
    if (TRUE == cal_empty) 
    {
        /* disable the calendar */
        hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id] = FALSE;
        update_cal_en = TRUE;
    }
    else 
    {
        update_cal = TRUE;
        if (FALSE == hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id])
        {
            hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id] = TRUE;
            update_cal_en = TRUE;
        }
    }

    ri_rcp_buffer_init(ri_b,hndl);    
    if (TRUE == update_cal)
    {
        if (ODU_RCP_FRM_STGN == rcp_frm_id || ODU_RCP_FRM_STG1 == rcp_frm_id)
        {
            for (itr = 0; itr < num_element; itr ++)
            {                
                cal_val[itr] = new_calendar[itr];             
            }
            if (ODU_RCP_FRM_STG1 == rcp_frm_id)
            {
                if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id])
                {
                    ri_rcp_lfield_SCH_STG_1_STBY_CFG_set(ri_b,hndl,cal_val);
                } 
                else 
                {
                    ri_rcp_lfield_SCH_STG_1_ACT_CFG_set(ri_b,hndl,cal_val);
                }                
            } 
            else
            {
                if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id])
                {
                    ri_rcp_lfield_SCH_STG_N_STBY_CFG_set(ri_b,hndl,cal_val);
                } 
                else
                {
                    ri_rcp_lfield_SCH_STG_N_ACT_CFG_set(ri_b,hndl,cal_val);
                }                
            }
        }
        else 
        {
            for (itr = 0; itr < num_element/2; itr ++)
            {                   
                switch(rcp_frm_id)
                {
                case ODU_RCP_FRM_STG2:
                    if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]) 
                    {
                        ri_rcp_field_SCH_STG_2_STBY_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_2_STBY_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    } 
                    else
                    {
                        ri_rcp_field_SCH_STG_2_ACT_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_2_ACT_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    }
                    break;
                case ODU_RCP_FRM_STG3A:  
                    if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]) 
                    {
                        ri_rcp_field_SCH_STG_3A_STBY_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_3A_STBY_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    } 
                    else 
                    {
                        ri_rcp_field_SCH_STG_3A_ACT_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_3A_ACT_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);                    
                    }
                    break;
                case ODU_RCP_FRM_STG3B:
                    if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]) 
                    {
                        ri_rcp_field_SCH_STG_3B_STBY_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_3B_STBY_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    }
                    else
                    {
                        ri_rcp_field_SCH_STG_3B_ACT_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_3B_ACT_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);                    
                    }
                    break;
                case ODU_RCP_FRM_STG4:
                    if (FALSE == hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]) 
                    {
                        ri_rcp_field_SCH_STG_4_STBY_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_4_STBY_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    }
                    else
                    {
                        ri_rcp_field_SCH_STG_4_ACT_CH_N_set(ri_b,hndl,itr,new_calendar[itr*2]);
                        ri_rcp_field_SCH_STG_4_ACT_CH_NP1_set(ri_b,hndl,itr,new_calendar[itr*2+1]);
                    }
                    break;
                default:
                    PMC_ASSERT(FALSE, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
                    break;
                }
            }
        }

        /* swap page*/
        hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id] = (hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]==TRUE)?FALSE:TRUE;        
        ri_rcp_field_range_REG_SCH_CAL_SEL_set(ri_b,hndl,(UINT32) rcp_frm_id,(UINT32) rcp_frm_id,hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]);
    }


    if (TRUE == update_cal_en) 
    {
        ri_rcp_field_range_REG_SCH_CAL_EN_set(ri_b,hndl,(UINT32) rcp_frm_id,(UINT32) rcp_frm_id,hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id]);

        /* when we disable the calendar, we reset the cal select (just to clean the state) */
        if (FALSE == hndl->var.ri_rcp_ctxt.cal_enable[(UINT32) rcp_frm_id]) 
        {
            hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id] = FALSE;
            ri_rcp_field_range_REG_SCH_CAL_SEL_set(ri_b,hndl,(UINT32) rcp_frm_id,(UINT32) rcp_frm_id,hndl->var.ri_rcp_ctxt.cal_select[(UINT32) rcp_frm_id]);
        }
    }
    ri_rcp_buffer_flush(ri_b);    

    
    PMC_RETURN(rc);
}

/*******************************************************************************
*  odu_rcp_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes ODU_RCP var_t variables.
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance to be operated on 
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
PRIVATE void odu_rcp_var_default_init(odu_rcp_handle_t   *hndl)
{    
    UINT32 itr;
    UINT32 jtr;
    UINT32 num_element;
    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
   
    odu_rcp_ctxt_state_set(hndl, ODU_RCP_DI_INST, ODU_RCP_STATE_START);
    odu_rcp_ctxt_state_set(hndl, ODU_RCP_RI_INST, ODU_RCP_STATE_START);
    
    for (itr = 0; itr < ODU_RI_RCP_NUM_FRAMER_MAX; itr ++)
    {
        if (ODU_RCP_FRM_STGN == (odu_rcp_frm_id_t) itr || ODU_RCP_FRM_STG1 == (odu_rcp_frm_id_t) itr)
        {
            num_element = ODU_RI_RCP_STG1N_NUM_CHNL;
        }
        else
        {
            num_element = ODU_RI_RCP_STG234_NUM_CHNL;
        }
        hndl->var.ri_rcp_ctxt.cal_enable[itr] = FALSE;
        hndl->var.ri_rcp_ctxt.cal_select[itr] = FALSE;
        for (jtr = 0; jtr < num_element; jtr ++)
        {            
            hndl->var.ri_rcp_ctxt.extract_rx_enable[itr][jtr] = FALSE;
            hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[itr][jtr] = FALSE;
            hndl->var.ri_rcp_ctxt.insert_aps_enable[itr][jtr] = FALSE;
        }
    }
    for (itr = 0; itr < 2; itr++)
    {
        hndl->var.di_rcp_ctxt.drcohm_cfg[itr].stg_31_00_d_rcohm_msk = 0;
        hndl->var.di_rcp_ctxt.drcohm_cfg[itr].stg_63_32_d_rcohm_msk = 0;
        hndl->var.di_rcp_ctxt.drcohm_cfg[itr].stg_95_64_d_rcohm_msk = 0;
    }

    hndl->var.di_rcp_ctxt.sdh_cnt_sel = 0;
    PMC_RETURN();
} /* odu_rcp_var_default_init */

/*******************************************************************************
*  odu_rcp_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets ODU_RCP RI or DI instance state.
*
* INPUTS:
*   *hndl             - pointer to handle instance to be operated on  
*    odu_rcp_id       - RI or DI block context state to be updated
*    state            - RI or DI ODU RCP provisioning state
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
PRIVATE void odu_rcp_ctxt_state_set(odu_rcp_handle_t      *hndl,
                                    odu_rcp_inst_t         odu_rcp_id,
                                    odu_rcp_inst_state_t   state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    switch(odu_rcp_id)
    {
    case ODU_RCP_RI_INST:
        hndl->var.ri_rcp_ctxt.state = state;
        break;
    case ODU_RCP_DI_INST:
        hndl->var.di_rcp_ctxt.state = state;
        break;
    default:
        break;
    }
    
    PMC_RETURN();
}

/*******************************************************************************
*  odu_rcp_ctxt_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns ODU_RCP RI or DI instance state.
*
* INPUTS:
*   *hndl             - pointer to handle instance to be operated on  
*    odu_rcp_id       - RI or DI block context state to be updated
*
* OUTPUTS:
*   *state            - RI or DI ODU RCP provisioning state
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_ctxt_state_get(odu_rcp_handle_t      *hndl,
                                         odu_rcp_inst_t         odu_rcp_id,
                                         odu_rcp_inst_state_t   *state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    switch(odu_rcp_id)
    {
    case ODU_RCP_RI_INST:
        *state = hndl->var.ri_rcp_ctxt.state;
        break;
    case ODU_RCP_DI_INST:
        *state = hndl->var.di_rcp_ctxt.state;
        break;
    default:
        PMC_RETURN(ODU_RCP_ERR_INVALID_ARGS);
    }
    
    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  odu_rcp_ctxt_state_transition_request
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks that ODU RCP state transition is allowed.
*
* INPUTS:
*   *hndl             - pointer to handle instance to be operated on  
*    odu_rcp_id       - RI or DI block context state to be updated
*    next_state       - next transition state
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if state transition is allowed otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_ctxt_state_transition_request(odu_rcp_handle_t      *hndl,
                                                        odu_rcp_inst_t         odu_rcp_id,
                                                        odu_rcp_inst_state_t   next_state)
{
    PMC_ERROR rc;
    odu_rcp_inst_state_t   state;

    PMC_ENTRY();

    rc = odu_rcp_ctxt_state_get(hndl, odu_rcp_id, &state);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }
    
    if (ODU_RCP_STATE_START == state &&
        ODU_RCP_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    else if (ODU_RCP_STATE_START == state &&
             ODU_RCP_STATE_START == next_state)
    {
        PMC_RETURN(ODU_RCP_STATE_ALREADY_START);
    }

    
    if (ODU_RCP_STATE_OPERATIONAL == state &&
        ODU_RCP_STATE_START == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    } 
    else if (ODU_RCP_STATE_OPERATIONAL == state &&
             ODU_RCP_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(ODU_RCP_STATE_ALREADY_PROVISIONED);
    }

    
    PMC_RETURN(ODU_RCP_STATE_UNKOWN);
}

/*******************************************************************************
* odu_ri_rcp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the RI RCP instance.
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   *ri_rcp_cfg_ptr   - pointer to a RI RCP configuration 
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
PRIVATE PMC_ERROR odu_ri_rcp_init(odu_rcp_handle_t *hndl, 
                                  digi_otn_odu_ri_rcp_cfg_t *ri_rcp_cfg_ptr)
{
    UINT32 itr;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != ri_rcp_cfg_ptr,ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    /* validate configuration */
    
    if ((PMC_ODU_RCP_REG_REG_ETH_MAC_OUI_UNUSED_MASK & hndl->cfg.ri_rcp_port_cfg.eth_mac_oui) != 0)
    {
        rc = ODU_RCP_RI_ERR_INVALID_ETH_MAC_OUI;  
    } 
    else if ((hndl->cfg.ri_rcp_port_cfg.eth_etype & PMC_ODU_RCP_REG_REG_ETH_ETYPE_UNUSED_MASK) != 0)
    {
        rc = ODU_RCP_RI_ERR_INVALID_ETH_ETYPE;
    }

    if (PMC_SUCCESS == rc) 
    {
        /* store the port config */
        PMC_MEMCPY(&hndl->cfg.ri_rcp_port_cfg, ri_rcp_cfg_ptr,sizeof(digi_otn_odu_ri_rcp_cfg_t));
    }
    if (PMC_SUCCESS == rc) 
    {
        /* power up the port */
        rc = odu_rcp_energy_state_reg_set(hndl,ODU_RCP_RI_INST,PMC_ENERGY_STATE_RUNNING);
    }
    /* init cfcs */
    if (PMC_SUCCESS == rc) 
    {
        for (itr = 0; itr < ODU_RI_RCP_NUM_CFC_MAX && rc == PMC_SUCCESS; itr++)
        {
            rc = cfc_init(hndl->cfc_handle[itr], PMC_ENERGY_STATE_RUNNING);
        }
    }
    /* configure ethernet parameters */
    if (PMC_SUCCESS == rc) 
    {
        rc = odu_rcp_eth_cfg(hndl,ODU_RCP_RI_INST,TRUE);    
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_di_rcp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the DI RCP instance.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   *di_rcp_cfg_ptr   - pointer to a DI RCP configuration 
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
PRIVATE PMC_ERROR odu_di_rcp_init(odu_rcp_handle_t *hndl, 
                                  odu_di_rcp_cfg_t *di_rcp_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    di_rcp_buffer_t buf[1];
    UINT32                 mac_addr_31_00_bit;
    UINT32                 mac_addr_47_32_bit;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != di_rcp_cfg_ptr, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
     
    rc = odu_rcp_energy_state_reg_set(hndl,ODU_RCP_DI_INST,PMC_ENERGY_STATE_RUNNING);
    if (PMC_SUCCESS == rc)
    {
        rc =  odu_rcp_eth_cfg(hndl,ODU_RCP_DI_INST,TRUE);   
        if (PMC_SUCCESS == rc)
        {
            
            /* Disable ETH_TX_EN until configuration is done */
            di_rcp_field_REG_ETH_TX_EN_set(NULL, hndl, 0);
            
            di_rcp_buffer_init(buf, hndl);
            
            /* Build SRC DI RCP MAC address */
            odu_rcp_di_build_mac_addr(hndl,
                                      di_rcp_cfg_ptr->src_mac,
                                      &mac_addr_31_00_bit,
                                      &mac_addr_47_32_bit);
            di_rcp_reg_REG_ETH_MAC_SA1_write(buf, hndl, mac_addr_31_00_bit);
            di_rcp_reg_REG_ETH_MAC_SA2_write(buf, hndl, mac_addr_47_32_bit);
            
            /* Build DEST DI RCP MAC address */
            odu_rcp_di_build_mac_addr(hndl,
                                      di_rcp_cfg_ptr->dest_mac,
                                      &mac_addr_31_00_bit,
                                      &mac_addr_47_32_bit);
            di_rcp_reg_REG_ETH_MAC_DA1_write(buf, hndl, mac_addr_31_00_bit);
            di_rcp_reg_REG_ETH_MAC_DA2_write(buf, hndl, mac_addr_47_32_bit);
            
            /* configure ETYPE */
            di_rcp_field_REG_ETH_ETYPE_BASE_set(buf, hndl, di_rcp_cfg_ptr->eth_etype);
            
            /* configure swizzle mode */
            di_rcp_field_REG_ETH_TX_10B_SWIZZLE_set(buf, hndl, TRUE);
            di_rcp_buffer_flush(buf);
            
            /* save configuration in context */
            PMC_MEMCPY(&hndl->var.di_rcp_ctxt.cfg, di_rcp_cfg_ptr, sizeof(odu_di_rcp_cfg_t));
            
            hndl->var.di_rcp_ctxt.pdu_msk = 0;
        }
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_rcp_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Un-Initialize the DI RCP instance.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
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
PRIVATE PMC_ERROR odu_di_rcp_uninit(odu_rcp_handle_t *hndl)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    rc =  odu_rcp_eth_cfg(hndl,ODU_RCP_DI_INST,FALSE);    
    if (PMC_SUCCESS != rc) 
    {
        rc = odu_rcp_energy_state_reg_set(hndl,ODU_RCP_DI_INST,PMC_ENERGY_STATE_RESET);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_ri_rcp_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Un-Initialize the RI RCP instance.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
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
PRIVATE PMC_ERROR odu_ri_rcp_uninit(odu_rcp_handle_t *hndl)
{
    UINT32 itr, jtr;
    UINT32 num_element;
    BOOL8 active_chnl = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    /* Check if we have active */
    for (itr = 0; itr < ODU_RI_RCP_NUM_FRAMER_MAX  && FALSE == active_chnl; itr ++)
    {
        if (ODU_RCP_FRM_STGN == (odu_rcp_frm_id_t) itr || ODU_RCP_FRM_STG1 == (odu_rcp_frm_id_t) itr)
        {
            num_element = ODU_RI_RCP_STG1N_NUM_CHNL;
        }
        else
        {
            num_element = ODU_RI_RCP_STG234_NUM_CHNL;
        }
        for (jtr = 0; jtr < num_element && FALSE == active_chnl; jtr ++)
        {            
            if (TRUE == hndl->var.ri_rcp_ctxt.extract_rx_enable[itr][jtr] ||
                TRUE == hndl->var.ri_rcp_ctxt.insert_backward_defect_enable[itr][jtr] ||
                TRUE == hndl->var.ri_rcp_ctxt.insert_aps_enable[itr][jtr])
            {
                active_chnl = TRUE;
            }

        }
    }
    
    if (TRUE == active_chnl) 
    {
        rc = ODU_RCP_STATE_RI_UNINIT_ERR;
    }
    
    if (PMC_SUCCESS == rc) 
    {
        for (itr = 0; itr < ODU_RI_RCP_NUM_CFC_MAX && rc == PMC_SUCCESS; itr++)
        {
            rc = cfc_init(hndl->cfc_handle[itr], PMC_ENERGY_STATE_RESET);
        }
    }
    rc = odu_rcp_eth_cfg(hndl,ODU_RCP_RI_INST,FALSE);    
    if (PMC_SUCCESS == rc) 
    {
        rc = odu_rcp_energy_state_reg_set(hndl,ODU_RCP_RI_INST,PMC_ENERGY_STATE_RESET);
    }
    
    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_ri_rcp_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the RI RCP instance.
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*
* OUTPUTS:
*   *ri_rcp_cfg_ptr   - pointer to a RI RCP configuration
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ri_rcp_init_get(odu_rcp_handle_t *hndl, 
                                      digi_otn_odu_ri_rcp_cfg_t *ri_rcp_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    PMC_MEMCPY(ri_rcp_cfg_ptr,&hndl->cfg.ri_rcp_port_cfg,sizeof(digi_otn_odu_ri_rcp_cfg_t));

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_di_rcp_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the DI RCP instance.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*
* OUTPUTS:
*   *di_rcp_cfg_ptr   - pointer to a DI RCP configuration 
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_di_rcp_init_get(odu_rcp_handle_t *hndl, 
                                      odu_di_rcp_cfg_t *di_rcp_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    PMC_MEMCPY(di_rcp_cfg_ptr, &hndl->var.di_rcp_ctxt.cfg, sizeof(odu_di_rcp_cfg_t));

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_rcp_di_build_mac_addr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Build MAC address based on mac address configuration.
*
*
* INPUTS:
*   *hndl               - pointer to ODU_RCP handle instance
*   *mac_addr_cfg_ptr   - mac address as a list of byte 
*
* OUTPUTS:
    *mac_addr_31_00_bit - MAC address LSB bit 0 to 31
    *mac_addr_47_32_bit - MAC address MSB bit 32 to 47
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odu_rcp_di_build_mac_addr(odu_rcp_handle_t *hndl,
                                       UINT8            mac_addr_cfg_ptr[UTIL_GLOBAL_MAC_NUM_BYTE],
                                       UINT32           *mac_addr_31_00_bit,
                                       UINT32           *mac_addr_47_32_bit)
{

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != mac_addr_31_00_bit, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    PMC_ASSERT(NULL != mac_addr_47_32_bit, ODU_RCP_ERR_INVALID_ARGS, 0, 0);

    *mac_addr_31_00_bit = mac_addr_cfg_ptr[0];
    *mac_addr_31_00_bit |= ((UINT32)mac_addr_cfg_ptr[1] << 8);
    *mac_addr_31_00_bit |= ((UINT32)mac_addr_cfg_ptr[2] << 16);
    *mac_addr_31_00_bit |= ((UINT32)mac_addr_cfg_ptr[3] << 24);
    *mac_addr_47_32_bit = mac_addr_cfg_ptr[4];
    *mac_addr_47_32_bit |= ((UINT32)mac_addr_cfg_ptr[5] << 8);

    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_di_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for DI RCP block.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*    energy_state     - DI block new energy state
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
PRIVATE PMC_ERROR odu_rcp_di_energy_state_reg_set(odu_rcp_handle_t  *hndl,
                                                  pmc_energy_state_t energy_state)
{
    PMC_ERROR rc = PMC_SUCCESS;
    di_rcp_buffer_t buf[1];

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);

    di_rcp_buffer_init(buf, hndl);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        di_rcp_field_REG_SW_RESET_set(buf, hndl, 0);
        di_rcp_field_REG_SOFT_SW_RESET_set(buf, hndl, 0);
        di_rcp_field_REG_LOWPWR_set(buf, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        di_rcp_field_REG_SW_RESET_set(buf, hndl, 1);
        di_rcp_field_REG_SOFT_SW_RESET_set(buf, hndl, 1);
        di_rcp_field_REG_LOWPWR_set(buf, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ODU_RCP_ERR_INVALID_ARGS, 0, 0);
    }
  
    di_rcp_buffer_flush(buf);

    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_rcp_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for a ODU_RCP module.
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   odu_rcp_id        - RCP instance 
*   energy_state      - requested energy state
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
PRIVATE PMC_ERROR odu_rcp_energy_state_reg_set(odu_rcp_handle_t  *hndl,
                                               odu_rcp_inst_t odu_rcp_id, 
                                               pmc_energy_state_t energy_state)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ri_rcp_buffer_t ri_b[1];

    UINT32 val = (energy_state== PMC_ENERGY_STATE_RUNNING)?FALSE:TRUE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    
    if (ODU_RCP_RI_INST == odu_rcp_id)
    {
        ri_rcp_buffer_init(ri_b,hndl);
        /* always set lowpwr FIRST */
        ri_rcp_field_REG_LOWPWR_set(ri_b,hndl,val);
        ri_rcp_field_REG_SOFT_SW_RESET_set(ri_b,hndl,val);
        ri_rcp_buffer_flush(ri_b);
    }
    else             
    {
        odu_rcp_di_energy_state_reg_set(hndl, energy_state);
    }
    
    PMC_RETURN(rc);
}


/*******************************************************************************
*  odu_rcp_set_bit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set a value in a specific bit of a 32-bit word.
*
* INPUTS:
*   src_word    - source word 
*   bit_idx     - bit index
*   bit_val     - bit new value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - source word new value.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_rcp_set_bit(UINT32    src_word,
                               UINT8     bit_idx,
                               BOOL8     bit_val)
{
    UINT32 new_val;

    PMC_ENTRY();

    new_val = 1 << bit_idx;
    
    if (TRUE == bit_val)
    {
        new_val = src_word | new_val;
    }
    else
    {
        new_val = ~new_val;
        new_val = src_word & new_val;
    }


    PMC_RETURN(new_val);
}

/*******************************************************************************
* odu_rcp_eth_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the ethernet parameter of a RCP instance 
*
*
* INPUTS:
*   *hndl             - pointer to ODU_RCP handle instance
*   odu_rcp_id        - RCP instance 
*   enable            - enable or disable ethernet port
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
PRIVATE PMC_ERROR odu_rcp_eth_cfg(odu_rcp_handle_t  *hndl,
                                  odu_rcp_inst_t odu_rcp_id,
                                  BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ri_rcp_buffer_t ri_b[1];
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ODU_RCP_ERR_NULL_HANDLE, 0, 0);
    
    if (ODU_RCP_RI_INST == odu_rcp_id)
    {        
        ri_rcp_buffer_init(ri_b,hndl);         
        
        if (TRUE == enable) 
        {
            ri_rcp_field_REG_ETH_TX_10B_SWIZZLE_set(ri_b,hndl,TRUE);
            ri_rcp_field_REG_ETH_MAC_OUI_set(ri_b,hndl,hndl->cfg.ri_rcp_port_cfg.eth_mac_oui);
            ri_rcp_field_REG_ETH_MAC_DIGI_ID_set(ri_b,hndl,hndl->cfg.ri_rcp_port_cfg.eth_mac_digi_id);
            ri_rcp_field_REG_ETH_ETYPE_set(ri_b,hndl,hndl->cfg.ri_rcp_port_cfg.eth_etype);
        }
        ri_rcp_field_REG_ETH_TX_EN_set(ri_b,hndl,enable);
        
        ri_rcp_buffer_flush(ri_b);
    } 
    /* Note: nothing to configure for DI_RCP */
    
    PMC_RETURN(rc);
}

/*******************************************************************************
* odu_rcp_frm_cfc_id_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function converts a framer id + direction into a CFC id
*   
*
* INPUTS:
*   frm_id    -  Framer IF
*   is_rx_frm - Is RX Framer
*
* OUTPUTS:
*   None
*
* RETURNS:
*   ODU RCP CFC ID
*
* NOTES:
*
*******************************************************************************/
PRIVATE odu_rcp_cfc_id_t odu_rcp_frm_cfc_id_convert(odu_rcp_frm_id_t frm_id, 
                                                    BOOL8 is_rx_frm)
{
   
    odu_rcp_cfc_id_t result;
    PMC_ENTRY();
    
    result = is_rx_frm == TRUE ? 
        (odu_rcp_cfc_id_t) frm_id: 
        (odu_rcp_cfc_id_t) ((UINT32) frm_id + ODU_RI_RCP_NUM_FRAMER_MAX);

    PMC_RETURN(result);
}

/* CFC functions */

/*******************************************************************************
*  odu_rcp_cfc_fifo_end_addr_chg_abort_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function abort an enfd address change request
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
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
PRIVATE void odu_rcp_cfc_fifo_end_addr_chg_abort_cfg(cfc_handle_t *cfc_handle)
{
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl,TRUE );
        break;
    case ODU_RCP_RX_CFC_STGN:
        ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_RX_CFC_STG2:
        ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_RX_CFC_STG4:
        ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STG1:
        ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STGN:
        ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STG2:
        ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    case ODU_RCP_TX_CFC_STG4:
        ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ABORT_set( NULL, hndl, TRUE );
        break;
    default:
        break;
    }
    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_cfc_fifo_end_addr_req_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the CFC FIFO end address channge success indicator
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   CFC FIFO end address change success indicator
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_rcp_cfc_fifo_end_addr_chg_success_get(cfc_handle_t *cfc_handle)
{
    UINT32 ret_val = 0;
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ret_val = ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ret_val = ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ret_val =  ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ret_val = ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ret_val = ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ret_val = ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG1:
        ret_val = ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ret_val = ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ret_val = ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ret_val = ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ret_val = ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ret_val = ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get( NULL, hndl);
        break;
    default:
        break;
    }
    PMC_RETURN(ret_val);
}


/*******************************************************************************
* odu_rcp_cfc_fifo_end_addr_req_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the fifo end address req status 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   
* OUTPUTS:
*   None
*
* RETURNS:
*   CFC FIFO end address change request status
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_rcp_cfc_fifo_end_addr_req_get(cfc_handle_t *cfc_handle)
{
    UINT32 ret_val = 0;
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ret_val = ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ret_val = ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ret_val =  ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ret_val = ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ret_val = ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ret_val = ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG1:
        ret_val = ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ret_val = ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ret_val = ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ret_val = ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ret_val = ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ret_val = ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_get( NULL, hndl);
        break;
    default:
        break;
    }
    PMC_RETURN(ret_val);
}

/*******************************************************************************
* odu_rcp_cfc_fifo_end_addr_req_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function create an end address change request
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   fifo_id             - FIFO ID                      
*   end_address         - the end address
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
PRIVATE void odu_rcp_cfc_fifo_end_addr_req_cfg(cfc_handle_t *cfc_handle, 
                                               UINT32 fifo_id,  
                                               UINT32 end_address)
{     
    ri_rcp_buffer_t ri_b[1];
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;
    
    ri_rcp_buffer_init(ri_b,hndl);    

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_1_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_N_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_2_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_3A_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_3B_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_RX_STG_4_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
 case ODU_RCP_TX_CFC_STG1:
        ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_1_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_N_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_2_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_3A_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_3B_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ID_set( ri_b, hndl, fifo_id);
        ri_rcp_field_TX_STG_4_CFC_END_ADDR_set( ri_b, hndl, end_address);
        ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_set( ri_b, hndl, TRUE);
        break;
    default:
        break;
    }

    ri_rcp_buffer_flush(ri_b);
    PMC_RETURN();
}

/*******************************************************************************
* odu_rcp_cfc_fifo_overflow_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function monitors CFC FIFO overflow interupt.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   fifo_id             - FIFO ID                      
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_cfc_fifo_overflow_mon_cfg(cfc_handle_t *cfc_handle, UINT32 fifo_id)
{   
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
       
    PMC_ENTRY();

    /* this bit is not supported in RI RCP CFCs */

    PMC_RETURN(ret_val);
} /*odu_rcp_cfc_fifo_overflow_mon_cfg */


/*******************************************************************************
*  odu_rcp_cfc_ipt_entry_get_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
*   *ipt_id             - The channle id associated with this entry (only valid 
*                         when ipt_valid is set)
*                         
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
PRIVATE void odu_rcp_cfc_ipt_entry_get_cfg(cfc_handle_t *cfc_handle, 
                                           UINT32 index,
                                           UINT32 *ipt_valid,
                                           UINT32 *ipt_id)
{   
    UINT32 ret_val = 0;
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_ipt_entry_state_get_cfg(cfc_handle, index, ipt_valid);
    
    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ret_val = ri_rcp_field_RX_STG_1_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ret_val = ri_rcp_field_RX_STG_N_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ret_val =  ri_rcp_field_RX_STG_2_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ret_val = ri_rcp_field_RX_STG_3A_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ret_val = ri_rcp_field_RX_STG_3B_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ret_val = ri_rcp_field_RX_STG_4_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG1:
        ret_val = ri_rcp_field_TX_STG_1_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ret_val = ri_rcp_field_TX_STG_N_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ret_val = ri_rcp_field_TX_STG_2_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ret_val = ri_rcp_field_TX_STG_3A_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ret_val = ri_rcp_field_TX_STG_3B_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ret_val = ri_rcp_field_TX_STG_4_CFC_IPT_ID_get( NULL, hndl,index);
        break;
    default:
        break;

    }

    *ipt_id = ret_val;
    
    PMC_RETURN();

} /* odu_rcp_cfc_ipt_entry_get_cfg*/

/*******************************************************************************
*  odu_rcp_cfc_ipt_entry_state_get_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
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
PRIVATE void odu_rcp_cfc_ipt_entry_state_get_cfg(cfc_handle_t *cfc_handle, 
                                                 UINT32 index,
                                                 UINT32 *ipt_valid)
{   
    UINT32 ret_val = 0;
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ret_val = ri_rcp_field_RX_STG_1_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ret_val = ri_rcp_field_RX_STG_N_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ret_val =  ri_rcp_field_RX_STG_2_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ret_val = ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ret_val = ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ret_val = ri_rcp_field_RX_STG_4_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG1:
        ret_val = ri_rcp_field_TX_STG_1_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ret_val = ri_rcp_field_TX_STG_N_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ret_val = ri_rcp_field_TX_STG_2_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ret_val = ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ret_val = ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ret_val = ri_rcp_field_TX_STG_4_CFC_IPT_VALID_get( NULL, hndl,index);
        break;
    default:
        break;

    }

    *ipt_valid = ret_val;
    PMC_RETURN();
} /* odu_rcp_cfc_ipt_entry_state_get_cfg */

/*******************************************************************************
*  odu_rcp_cfc_ovr_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the overflow interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance.
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt \n     
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
PRIVATE void odu_rcp_cfc_ovr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                    UINT32 fifo_id,
                                    UINT32 enable)
{   
    PMC_ENTRY();
    
    /* this bit is not supported in RI RCP CFCs */
    
    PMC_RETURN(); 

} /* odu_rcp_cfc_ovr_int_enable_cfg */


/*******************************************************************************
*  odu_rcp_cfc_udr_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the underflow interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance.
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt  \n    
*                         
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
PRIVATE void  odu_rcp_cfc_udr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                        UINT32 fifo_id,
                                        UINT32 enable)
{   
    PMC_ENTRY();
    
   /* this bit is not supported in RI RCP CFCs */
    
    PMC_RETURN(); 

} /* odu_rcp_cfc_udr_int_enable_cfg */


/*******************************************************************************
*  odu_rcp_cfc_ipt_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the IPT error interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance.
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt \n     
*                         
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
PRIVATE void odu_rcp_cfc_ipt_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                         UINT32 fifo_id,
                                         UINT32 enable)
{   
    PMC_ENTRY();
    
    /* this bit is not supported in RI RCP CFCs */
    
    PMC_RETURN(); 

} /* odu_rcp_cfc_ipt_int_enable_cfg */

/*******************************************************************************
*  odu_rcp_cfc_bypass_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IPT to bypass mode or not.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance.
*   bypass            - bypass mode \n
*                       0 -- the physical addresses returned by the IPT are used
*                            to drive the write/read address outputs. \n
*                       1 -- the write/read addresses of the controller are used 
*                            to drive the write/read address outputs. \n
*                       This signal must remain static after a rest of the FIFO
*                       controller. \n 
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
PRIVATE void odu_rcp_cfc_bypass_cfg(cfc_handle_t *cfc_handle, UINT32 bypass)
{   
    
    PMC_ENTRY();

    /* this bit is not supported in RI RCP CFCs */

    PMC_RETURN(); 

} /* cfc_bypass_cfg*/


/*******************************************************************************
*  odu_rcp_cfc_ipt_page_mask_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the IPT page mask
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
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
PRIVATE void odu_rcp_cfc_ipt_page_mask_cfg(cfc_handle_t *cfc_handle)
{
    PMC_ENTRY();
    
    /* this bit is not supported in RI RCP CFCs */

    PMC_RETURN();
}

/*******************************************************************************
*  odu_rcp_cfc_inverted_page_table_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the inverted page table for the channelized FIFO
*   controller that maps virtual block addresses to physical block addresses.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   ipt_valid           - qualifies the inverted page table entry \n
*                         0 -- the corresponding FIFO ID specified in IPT_ID is 
*                              invalid \n
*                         1 -- the corresponding FIFO ID specified in IPT_ID is 
*                              valid \n
*   ipt_id              - channel ID associated with the physical block.
*   ipt_va              - the block number associated with the physical block.
*                         
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_cfc_inverted_page_table_cfg(cfc_handle_t *cfc_handle,  
                                                      UINT32 index,
                                                      UINT32 ipt_valid,
                                                      UINT32 ipt_id, 
                                                      UINT32 ipt_va)
{   
    ri_rcp_buffer_t ri_b[1];
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();
 
    /* argument checking */
    if (ipt_valid > 1) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

#ifndef  CFC_REG_OP_CTXT_DISABLE                               
     /* Update var_t context */                               
     cfc_handle->var.inverted_page_table[index].ipt_valid = ipt_va;
     cfc_handle->var.inverted_page_table[index].ipt_id = ipt_id;
     cfc_handle->var.inverted_page_table[index].ipt_va = ipt_va;
#endif /* CFC_REG_OP_CTXT_DISABLE */ 

    
    ri_rcp_buffer_init(ri_b,hndl);    

    /* only 4 bits are supported for some IPT_ID */
    if (ODU_RCP_RX_CFC_STG1 == odu_rcp_cfc_id || ODU_RCP_RX_CFC_STGN == odu_rcp_cfc_id || 
        ODU_RCP_TX_CFC_STG1 == odu_rcp_cfc_id || ODU_RCP_TX_CFC_STGN == odu_rcp_cfc_id )
    {
        if (0x7f == ipt_id) 
        {
            ipt_id = 0xf;
        }
        if (0x7f == ipt_va) 
        {
            ipt_va = 0xf;
        }
    }

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ri_rcp_field_RX_STG_1_CFC_IPT_ID_set( ri_b, hndl, index,ipt_id);
        ri_rcp_field_RX_STG_1_CFC_IPT_VA_set( ri_b, hndl, index,ipt_va);
        ri_rcp_field_RX_STG_1_CFC_IPT_VALID_set( ri_b, hndl, index,ipt_valid);
        break;
    case ODU_RCP_RX_CFC_STGN:
        ri_rcp_field_RX_STG_N_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_RX_STG_N_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_RX_STG_N_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_RX_CFC_STG2:
        ri_rcp_field_RX_STG_2_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_RX_STG_2_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_RX_STG_2_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ri_rcp_field_RX_STG_3A_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_RX_STG_3A_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ri_rcp_field_RX_STG_3B_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_RX_STG_3B_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ri_rcp_field_RX_STG_4_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_RX_STG_4_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_RX_STG_4_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STG1:
        ri_rcp_field_TX_STG_1_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_1_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_1_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STGN:
        ri_rcp_field_TX_STG_N_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_N_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_N_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STG2:
        ri_rcp_field_TX_STG_2_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_2_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_2_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STG3A:
        ri_rcp_field_TX_STG_3A_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_3A_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ri_rcp_field_TX_STG_3B_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_3B_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ri_rcp_field_TX_STG_4_CFC_IPT_ID_set( ri_b, hndl, index, ipt_id);
        ri_rcp_field_TX_STG_4_CFC_IPT_VA_set( ri_b, hndl, index, ipt_va);
        ri_rcp_field_TX_STG_4_CFC_IPT_VALID_set( ri_b, hndl, index, ipt_valid);
        break;
    default:
        break;
    }

    ri_rcp_buffer_flush(ri_b);
    
    PMC_RETURN(PMC_SUCCESS); 

} /* odu_rcp_cfc_inverted_page_table_cfg */

/*******************************************************************************
* odu_rcp_cfc_fifo_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function takes the specified FIFO out of reset or 
*   alternatively puts the specified FIFO into reset
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance.
*   fifo_id             - FIFO ID
*   reset_value         - reset value \n
*                         0 -- into reset \n
*                         1 -- out of reset \n                      
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rcp_cfc_fifo_reset_cfg(cfc_handle_t *cfc_handle, 
                                             UINT32 fifo_id, 
                                             UINT32 reset_value)
{       
    UINT32 read_val = 0;
    odu_rcp_cfc_id_t odu_rcp_cfc_id;
    odu_rcp_handle_t *hndl;
    PMC_ENTRY();

    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num)
    {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    odu_rcp_cfc_id = (odu_rcp_cfc_id_t) cfc_handle->cfg.cfg_fct_table.opaque;
    hndl           = (odu_rcp_handle_t *) cfc_handle->cfg.cfg_fct_table.ext_handle;

    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        read_val = ri_rcp_field_RX_STG_1_FIFO_CLRB_get( NULL, hndl);                
        break;
    case ODU_RCP_RX_CFC_STGN:
        read_val = ri_rcp_field_RX_STG_N_FIFO_CLRB_get( NULL, hndl);        
        break;
    case ODU_RCP_RX_CFC_STG2:
        ri_rcp_lfield_range_RX_STG_2_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break;
    case ODU_RCP_RX_CFC_STG3A:
        ri_rcp_lfield_range_RX_STG_3A_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break;
    case ODU_RCP_RX_CFC_STG3B:
        ri_rcp_lfield_range_RX_STG_3B_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break;
    case ODU_RCP_RX_CFC_STG4:
        ri_rcp_lfield_range_RX_STG_4_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break; 
    case ODU_RCP_TX_CFC_STG1:
        read_val = ri_rcp_field_TX_STG_1_FIFO_CLRB_get( NULL, hndl);                
        break;
    case ODU_RCP_TX_CFC_STGN:
        read_val = ri_rcp_field_TX_STG_N_FIFO_CLRB_get( NULL, hndl);        
        break;
    case ODU_RCP_TX_CFC_STG2:
        ri_rcp_lfield_range_TX_STG_2_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);

        break;
    case ODU_RCP_TX_CFC_STG3A:
        ri_rcp_lfield_range_TX_STG_3A_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break;
    case ODU_RCP_TX_CFC_STG3B:
        ri_rcp_lfield_range_TX_STG_3B_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break;
    case ODU_RCP_TX_CFC_STG4:
        ri_rcp_lfield_range_TX_STG_4_FIFO_CLRB_set(NULL,hndl, fifo_id, fifo_id, reset_value);
        break; 
    default:
        break;

    }
    if (ODU_RCP_RX_CFC_STG1 == odu_rcp_cfc_id || ODU_RCP_RX_CFC_STGN == odu_rcp_cfc_id || 
        ODU_RCP_TX_CFC_STG1 == odu_rcp_cfc_id || ODU_RCP_TX_CFC_STGN == odu_rcp_cfc_id )
    {
        if (reset_value == 1) 
        {
            read_val |= (1 << fifo_id);
        }
        else 
        {
            read_val &= 0xFFF ^ (1 << fifo_id);
        }
    }
    switch(odu_rcp_cfc_id)
    {
    case ODU_RCP_RX_CFC_STG1:
        ri_rcp_field_RX_STG_1_FIFO_CLRB_set( NULL, hndl,read_val);                
        break;
    case ODU_RCP_RX_CFC_STGN:
        ri_rcp_field_RX_STG_N_FIFO_CLRB_set( NULL, hndl,read_val);        
        break;     
    case ODU_RCP_TX_CFC_STG1:
        ri_rcp_field_TX_STG_1_FIFO_CLRB_set( NULL, hndl,read_val);                
        break;
    case ODU_RCP_TX_CFC_STGN:
        ri_rcp_field_TX_STG_N_FIFO_CLRB_set( NULL, hndl,read_val);        
        break;     
    default:
        break;
    }

    PMC_RETURN(PMC_SUCCESS);   

} /*odu_rcp_cfc_fifo_reset_cfg */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_chnl_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve RI RCP CFC interrupts of a given channel.
*   
*
* INPUTS:
*   odu_ri_rcp_handle - Pointer to RCP handle
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking CFC enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - pointer to CFC channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_retrieve_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                            UINT32             fifo_id,
                                                            cfc_int_chnl_t    *int_en_table_ptr,
                                                            cfc_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8 cfc_fifo_irq_sel;
    BOOL8 cfc_fifo_irq = FALSE;

    PMC_ENTRY();

    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->ipt_err_i)
    {
        int_table_ptr->ipt_err_i = ri_rcp_lfield_range_CFC_IPT_ERR_I_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id); 
    }
    
    cfc_fifo_irq_sel = ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id);
    
    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fifo_ovr_i || TRUE == int_en_table_ptr->fifo_udr_i)
    {        
        cfc_fifo_irq = ri_rcp_lfield_range_CFC_FIFO_IRQ_I_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id); 
    }

    if (TRUE == cfc_fifo_irq_sel)
    {
        int_table_ptr->fifo_ovr_i = cfc_fifo_irq;
    }  
    else
    {        
        int_table_ptr->fifo_ovr_i = cfc_fifo_irq;
    }      


    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_chnl_retrieve_internal */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_chnl_enable_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable RI RCP CFC interrupts of a given channel.
*   
*
* INPUTS:
*   odu_ri_rcp_handle - Pointer to RCP handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_table_ptr     - pointer to CFC channel interrupt table to enable/disable
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   int_en_table_ptr  - Pointer to table tracking CFC enabled interrupts
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_enable_internal(odu_rcp_handle_t *odu_ri_rcp_handle,
                                                          UINT32            fifo_id,
                                                          cfc_int_chnl_t   *int_table_ptr,
                                                          cfc_int_chnl_t   *int_en_table_ptr,
                                                          BOOL8             enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8 cfc_fifo_irq;

    PMC_ENTRY();

    if (TRUE == int_table_ptr->ipt_err_i)
    {
        ri_rcp_lfield_range_CFC_IPT_ERR_E_set(NULL, odu_ri_rcp_handle,fifo_id,fifo_id, enable); 
        int_en_table_ptr->ipt_err_i = enable;
    }
    
    cfc_fifo_irq = ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id);
    
    if (NULL == int_table_ptr || 
        TRUE == int_table_ptr->fifo_ovr_i || 
        TRUE == int_table_ptr->fifo_udr_i)
    {        
        ri_rcp_lfield_range_CFC_FIFO_IRQ_E_set(NULL, odu_ri_rcp_handle,fifo_id, fifo_id, enable); 
    }

    if (TRUE == cfc_fifo_irq)
    {
        int_en_table_ptr->fifo_ovr_i = enable;
    }  
    else
    {        
        int_en_table_ptr->fifo_ovr_i = enable;
    }      

    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_chnl_enable_internal */

/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_chnl_clear_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Clear RI RCP CFC interrupts of a given channel.
*   
* INPUTS:
*   odu_ri_rcp_handle - Pointer to RCP handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_en_table_ptr  - Pointer to table tracking CFC enabled interrupts
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_clear_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                         UINT32             fifo_id,
                                                         cfc_int_chnl_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->ipt_err_i)
    {
        ri_rcp_lfield_range_CFC_IPT_ERR_I_set_to_clear(NULL, odu_ri_rcp_handle,fifo_id,fifo_id, TRUE); 
    }
    
    
    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fifo_ovr_i || TRUE == int_en_table_ptr->fifo_udr_i)
    {        
        ri_rcp_lfield_range_CFC_FIFO_IRQ_I_set_to_clear(NULL, odu_ri_rcp_handle,fifo_id,fifo_id, TRUE); 
    }
 
    
    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_chnl_enable_internal */


/*******************************************************************************
* FUNCTION: odu_ri_rcp_cfc_int_chnl_enabled_check_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active RI RCP CFC interrupt bits based on the enabled interrupt table.
*   
* INPUTS:
*   odu_ri_rcp_handle - Pointer to RCP handle
*   fifo_id           - Channel ID: Valid range: 0 - 12/95
*   int_en_table_ptr  - Pointer to table tracking CFC enabled interrupts
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
PRIVATE PMC_ERROR odu_ri_rcp_cfc_int_chnl_enabled_check_internal(odu_rcp_handle_t  *odu_ri_rcp_handle,
                                                                 UINT32             fifo_id,
                                                                 cfc_int_chnl_t    *int_en_table_ptr,
                                                                 BOOL8             *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->ipt_err_i)
    {
        *int_found_ptr = ri_rcp_lfield_range_CFC_IPT_ERR_I_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id); 
    }
    
    if (FALSE == *int_found_ptr)
    {    
        if (NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fifo_ovr_i || TRUE == int_en_table_ptr->fifo_udr_i)
        {        
            *int_found_ptr = ri_rcp_lfield_range_CFC_FIFO_IRQ_I_get(NULL, odu_ri_rcp_handle,fifo_id,fifo_id); 
        }
    }

    PMC_RETURN(result);
} /* odu_ri_rcp_cfc_int_chnl_enabled_check */

/*
** End of file
*/
