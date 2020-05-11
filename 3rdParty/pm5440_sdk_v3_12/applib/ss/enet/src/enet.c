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
*     ENET subsystem datapath configurations.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "enet_loc.h"

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
const char ENET_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ENET_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ENET_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References.
*/
PRIVATE emac_data_rate_t enet_convert_datapath_to_emac_data_rate(enet_datapath_t datapath);
PRIVATE emac_rx_data_src_t enet_convert_datapath_to_emac_rx_data_src(enet_datapath_t datapath);
PRIVATE emac_tx_data_dst_t enet_convert_datapath_to_emac_tx_data_dst(enet_datapath_t datapath);
PRIVATE etime_data_rate_t enet_convert_datapath_to_etime_data_rate(enet_datapath_t datapath);
PRIVATE empei_i_data_rate_enum_t enet_convert_datapath_to_empei_i_data_rate(enet_datapath_t datapath);
PRIVATE empei_e_data_rate_enum_t enet_convert_datapath_to_empei_e_data_rate(enet_datapath_t datapath);
PRIVATE empei_e_datapath_type_enum_t enet_convert_datapath_to_empei_e_datapath_sel(enet_datapath_t datapath);
PRIVATE epmm_datapath_t enet_convert_datapath_to_epmm_datapath(enet_datapath_t datapath);
PRIVATE etrans_datapath_t enet_convert_datapath_to_etrans_datapath(enet_datapath_t datapath);
PRIVATE void enet_var_default_init(enet_var_t *var_ptr);
PRIVATE PMC_ERROR enet_update_hndl_var(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, BOOL8 ts_en, BOOL8 config);
PRIVATE PMC_ERROR enet_validate_datapath_cfg(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, BOOL8 config);
PRIVATE void enet_db_all_init(enet_handle_t *enet_handle_ptr, enet_type_id_t enet_type_id, enet_schd_populate_t schd_populate_mode);
PRIVATE PMC_ERROR enet_db_sch_slot_set(enet_handle_t *enet_handle_ptr, UINT32 sch_slot, UINT32 link);
PRIVATE epmm_link_direction_t enet_convert_dir_to_epmm_dir(enet_dir_t direction);
PRIVATE PMC_ERROR enet_int_init(enet_handle_t    *enet_handle,
                                BOOL8              enable);
PRIVATE BOOL8 enet_etrans_tx_lock_status_get(enet_handle_t     *hndl,
                                             UINT32             link,
                                             etrans_datapath_t  etrans_datapath);
PRIVATE BOOL8 enet_is_gfp_datapath_get(etrans_datapath_t  etrans_datapath);


PRIVATE PMC_ERROR enet_ctxt_reg_recover(enet_handle_t        *enet_handle,
                                        enet_type_id_t        enet_type,
                                        pmc_energy_state_t    top_energy_state_reg,         
                                        enet_recover_var_t   *enet_var_reg,              
                                        enet_energy_state_t  *enet_energy_state_reg);


PRIVATE PMC_ERROR enet_ctxt_audit(enet_handle_t       *enet_handle,
                                  enet_recover_var_t  *enet_var_reg,
                                  enet_energy_state_t *enet_energy_state_reg,
                                  enet_var_t          *enet_var_clean);

PRIVATE void  enet_empei_link_dpi_sch_slot_get(UINT32            link,  
                                               enet_datapath_t   datapath,
                                               UINT32           *dpi_sch_slot);

PRIVATE  UINT32 enet_empei_link_dpi_sch_slot_index_get(UINT32            link,  
                                                       enet_datapath_t   datapath);

PRIVATE BOOL8 enet_ing_ts_en_get(enet_handle_t *hndl);

PRIVATE void enet_reg_cleanup(enet_handle_t    *hndl,
                              UINT32            num_links, 
                              UINT32           *links);

PRIVATE PMC_ERROR enet_db_chnl_entry_audit(enet_handle_t        *enet_handle,
                                           enet_recover_var_t   *enet_var_reg,
                                           UINT32                link);

PRIVATE PMC_ERROR enet_ctxt_cleanup(enet_handle_t         *hndl,
                                    enet_recover_var_t    *enet_var_reg,
                                    enet_energy_state_t   *enet_energy_state_reg,
                                    enet_var_t            *enet_var_clean);

PRIVATE void enet_ctxt_mpmo_cleanup(enet_handle_t         *hndl,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PRIVATE void enet_energy_state_reg_get(enet_handle_t       *hndl, 
                                       enet_energy_state_t *enet_energy_state);

PRIVATE void  enet_empei_link_dpi_sch_slot_get(UINT32            link,  
                                               enet_datapath_t   datapath,
                                               UINT32           *dpi_sch_slot);

PRIVATE UINT32 enet_empei_link_dpi_sch_slot_index_get(UINT32            link,  
                                                      enet_datapath_t   datapath);

PRIVATE enet_lnk_state_t enet_util_global_state_convert(util_global_chnl_state_t  state);


PRIVATE BOOL8 enet_is_pmon_datapath_get(enet_datapath_t datapath);

PUBLIC eclass_handle_t *enet_eclass_handle_get(enet_handle_t *hndl, 
                                               enet_dir_t     dir);

PRIVATE BOOL8 enet_should_emac_use_epmm_int_mon(enet_datapath_t datapath);

PRIVATE BOOL8 enet_check_rx_epmm_state(enet_handle_t        *enet_handle,
                                       enet_recover_var_t   *enet_var_reg,
                                       UINT32 link);
/*
** Public Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* enet_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ENET subsystem instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ENET subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - string representation of the module
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   enet_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC enet_handle_t *enet_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name )
{
    enet_handle_t *enet_handle;

    PMC_ENTRY();

    #ifdef USE_PMC_CTXT_CALLOC
        if (parent == NULL) {
            void *c = NULL;
            c = pmc_ctxt_mem_create(sys_handle, FALSE);
            pmc_ctxt_mem_init(c);
            enet_handle = (enet_handle_t*)(((pmc_ctxt_mgmt_ctxt_t*)c)->header.digi_hdl);
            enet_handle->base.ctxt = c;
            enet_handle = (enet_handle_t*)PMC_CTXT_CALLOC(sizeof(enet_handle_t), enet_handle);
        } else {
            enet_handle = (enet_handle_t*)PMC_CTXT_CALLOC(sizeof(enet_handle_t), parent);
        };
    #else
        enet_handle = (enet_handle_t*)PMC_CTXT_CALLOC(sizeof(enet_handle_t), parent);
    #endif

    pmc_handle_init( parent, enet_handle, sys_handle, PMC_MID_DIGI_ENET, tsb_name, base_address );

    if (parent == NULL) {
        gen_db_init_metadata(&enet_handle->base);
    };    

    /* Register log strings. */
    pmc_log_block_strings_register(ENET_LOG_ERR_STRINGS[0], ENET_LOG_ERR_TABLE_BASE, ENET_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    enet_handle->emac_handle    =    emac_ctxt_create((pmc_handle_t *)enet_handle, base_address+BASE_ADDR_ENET120_LINE_EMAC120_MTSB,     sys_handle, "EMAC");
    PMC_CTXT_REGISTER_PTR(&enet_handle->emac_handle, enet_handle);
    enet_handle->epmm_handle    =    epmm_ctxt_create((pmc_handle_t *)enet_handle, base_address+BASE_ADDR_ENET120_LINE_EPMM120_MTSB,     sys_handle, "EPMM");
    PMC_CTXT_REGISTER_PTR(&enet_handle->epmm_handle, enet_handle);
    enet_handle->empei_e_handle = empei_e_ctxt_create((pmc_handle_t *)enet_handle, base_address+BASE_ADDR_ENET120_LINE_EMPEI120_E_MTSB,  sys_handle, "EMPEI_E");
    PMC_CTXT_REGISTER_PTR(&enet_handle->empei_e_handle, enet_handle);
    enet_handle->empei_i_handle = empei_i_ctxt_create((pmc_handle_t *)enet_handle, base_address+BASE_ADDR_ENET120_LINE_EMPEI120_I_MTSB,  sys_handle, "EMPEI_I");
    PMC_CTXT_REGISTER_PTR(&enet_handle->empei_i_handle, enet_handle);

    enet_handle->empei_e_eclass_handle = enet_handle->empei_e_handle->eclass_handle;
    PMC_CTXT_REGISTER_PTR(&enet_handle->empei_e_eclass_handle, enet_handle);
    enet_handle->empei_i_eclass_handle = enet_handle->empei_i_handle->eclass_handle;
    PMC_CTXT_REGISTER_PTR(&enet_handle->empei_i_eclass_handle, enet_handle);

    /* Note that we allowed the etrans handle for the ENET subsystem, but it is only valid for ENET_LINE flavor */
    enet_handle->etrans_handle = etrans_ctxt_create((pmc_handle_t *)enet_handle, base_address+BASE_ADDR_ENET120_LINE_ETRANS120_MTSB, sys_handle, "ETRANS");
    PMC_CTXT_REGISTER_PTR(&enet_handle->etrans_handle, enet_handle);

    PMC_RETURN(enet_handle);
} /* enet_ctxt_create */

/*******************************************************************************
* enet_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ENET subsystem instance.
*
*
* INPUTS:
*   *enet_handle      - pointer to ENET handle instance to be operated on
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
PUBLIC void enet_ctxt_destroy(enet_handle_t *enet_handle)
{
    PMC_ENTRY();

    /* detach from database if database used */
    if (enet_handle->epmm_handle->mpma_db_alloc_mode == UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB ||
        enet_handle->epmm_handle->mpma_db_alloc_mode == UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        gen_db_detach(&(enet_handle->base), 
                      enet_handle->cfg.schd_use_ctxt.db_id,
                      enet_handle->cfg.schd_use_ctxt.db_client_handle);
    }    

    emac_ctxt_destroy   (enet_handle->emac_handle);
    epmm_ctxt_destroy   (enet_handle->epmm_handle);
    empei_e_ctxt_destroy(enet_handle->empei_e_handle);
    empei_i_ctxt_destroy(enet_handle->empei_i_handle);
    etrans_ctxt_destroy (enet_handle->etrans_handle);
    

    PMC_CTXT_FREE(&enet_handle, enet_handle);

    PMC_RETURN();
} /* enet_ctxt_destroy */

/*******************************************************************************
* enet_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a ENET subsystem handle instance.
*
*
* INPUTS:
*   *enet_handle        - pointer to ENET handle instance to be operated on
*   enet_type_id        - instance of the ENET module
*   schd_populate_mode  - determines in what mode the ENET_LINE or ENET_SYS instance 
*                         is supposed to run in - local or db mode
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
PUBLIC void enet_handle_init(enet_handle_t *enet_handle,
                             enet_type_id_t enet_type_id,
                             enet_schd_populate_t schd_populate_mode)
{
    PMC_ENTRY();

    enet_handle->cfg.enet_type_id = enet_type_id;
    enet_handle->cfg.schd_populate_mode = schd_populate_mode;
    /* Initialize the number of provisioned links to zero */
    enet_var_default_init(&enet_handle->var);
    
    /*initializes the ENET schedulers with local/db mode */
    enet_db_all_init(enet_handle, enet_type_id, schd_populate_mode);

    emac_handle_init   (enet_handle->emac_handle);
    epmm_handle_init   (enet_handle->epmm_handle, TRUE);
    empei_e_handle_init(enet_handle->empei_e_handle);
    empei_i_handle_init(enet_handle->empei_i_handle);

    /* ETRANS is only instantiated in ENET_LINE */
    if (ENET_LINE == enet_type_id) {
        etrans_handle_init(enet_handle->etrans_handle);
    }

    PMC_RETURN();
} /* enet_handle_init */




/*******************************************************************************
* enet_db_entry_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method for the datapath layer to write to the
*    databases that are in use by the schedulers in ENET.
*
* INPUTS:
*   *enet_handle_ptr       - pointer to ENET handle instance
*   schd_addr              - calendar_entry where the channel num for the ENET instance is to be stored. 
*                              Ranges from 0 to 95
*    link                   - The link determined according channels operating in ENET
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
PUBLIC PMC_ERROR enet_db_entry_set(enet_handle_t *enet_handle_ptr,
                                   UINT32         schd_addr,
                                   UINT32         link )
{    
    mapotn_db_rec rec;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT( link < MAX_ENET_CHANNELS , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
    PMC_ASSERT( schd_addr < 96 , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);

    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } 
    else
    {
        /* write schd_addr and link to the data base for
           the specified schd_inst */
        PMC_MEMSET((void *) &rec, 0, sizeof(mapotn_db_rec));
        rec.channel_num = link;
        rec.calendar_entry = schd_addr;

        result = gen_db_add_record(&(enet_handle_ptr->base),
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_id, 
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, 
                                   (void *)&rec);
    }
    PMC_RETURN(result);

} /* enet_db_entry_set */

/*******************************************************************************
* enet_db_entry_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method for the datapath layer to clear the
*    entries of databases that are in use by the schedulers in ENET.  The value 0xFFFFFFFF 
*    is the unused/cleared value for db content.
*
*
* INPUTS:
*   *enet_handle_ptr       - pointer to ENET handle instance
*    link                   - The link determined according channels operating in ENET
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
PUBLIC PMC_ERROR enet_db_entry_clear( enet_handle_t *enet_handle_ptr,
                                      UINT32 link )
{
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 i;
    mapotn_db_rec rec;
    mapotn_db_key key;
    UINT32 num_of_recs;
    mapotn_db_query_res *q_result;
        
    PMC_ENTRY();    
    
    PMC_ASSERT( link < MAX_ENET_CHANNELS , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
    
    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } else
    {
        /* clear the database entry for the rd_channel ID supplied */
        rec.channel_num = link; 
        rec.calendar_entry = DO_NOT_USE_KEY;

        result = gen_db_rem_record(&(enet_handle_ptr->base),
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_id, 
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, 
                                   (void *)&rec);
    }

    PMC_LOG_TRACE("printDB after removing records for channel:%d\n", link);
    for(i=0; i<96; i++)
    {  
        key.calendar_entry = i;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs=gen_db_query(&(enet_handle_ptr->base),enet_handle_ptr->cfg.schd_use_ctxt.db_id, enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, (void *)&key,(void **)&q_result);

        if(num_of_recs == 0)
        {
            PMC_RETURN(ENET_ERR_DB_ENTRY_NOT_FOUND);
        }
        
        PMC_LOG_TRACE("1:n query result: i= %d, channel=0x%x,cal entry=0x%x,  %s\n",
                      i,q_result->p_recs[0]->channel_num,q_result->p_recs[0]->calendar_entry, __FILE__);   

    }              

    PMC_RETURN(result);

} /* enet_db_entry_clear */

/*******************************************************************************
* enet_db_sch_slot_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method to locally set scheduler slot into the
*    databases that are in use by the schedulers in ENET.
*
* INPUTS:
*   *enet_handle_ptr       - pointer to ENET handle instance
*    sch_slot               - scheduler slot
*                             Ranges from 0 to 11
*    link                   - The link determined according channels operating in ENET
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
PRIVATE PMC_ERROR enet_db_sch_slot_set(enet_handle_t *enet_handle_ptr,
                                       UINT32 sch_slot,
                                       UINT32 link )
{
    UINT32 itr;
  
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT( link < MAX_ENET_CHANNELS , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
    PMC_ASSERT( sch_slot < 12 , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
  
    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } else
    {
        /* 8 entry per calendar slot */
        for (itr = 0; (itr < 8) && (PMC_SUCCESS == result); itr ++) {
            result = enet_db_entry_set(enet_handle_ptr,sch_slot*8 + itr, link);
        }
    }
    PMC_RETURN(result);

} /* enet_db_sch_slot_set */


/*******************************************************************************
* enet_db_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method for the datapath layer to retrieve the
*    entries of databases that are in use by the schedulers in ENET.
*
*
* INPUTS:
*   *enet_handle_ptr       - pointer to ENET handle instance
*   schd_addr              - calendar_entry where the channel num for the ENET instance 
*                            is to be stored. 
*                            Ranges from 0 to 95
*
* OUTPUTS:
*   link_ptr               - The link determined according channels operating in ENET
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_db_entry_get( enet_handle_t *enet_handle_ptr,
                                    UINT32 schd_addr,
                                    UINT32 *link_ptr)                                   
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 num_of_recs = 0;
    mapotn_db_key    key;
    mapotn_db_query_res *q_result;
    
        
    PMC_ENTRY();
 
    PMC_ASSERT( schd_addr < 96 , ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
           
    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } else
    {
        /* query rd_channel_id data for the schd_addr */
        key.calendar_entry = schd_addr;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs = gen_db_query(&(enet_handle_ptr->base),
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_id, 
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, 
                                   (void *)&key,
                                   (void **)&q_result);
        if(num_of_recs == 0)
        {
            PMC_RETURN(ENET_ERR_DB_ENTRY_NOT_FOUND);
        }

        PMC_LOG_TRACE("ENET DB Element: schd_addr=0x%x,cal rd_channel_id=0x%x,\n",
                      q_result->p_recs[0]->calendar_entry, q_result->p_recs[0]->channel_num);          
                   
        *link_ptr = q_result->p_recs[0]->channel_num;                         
    }
    PMC_RETURN(result);
} /* enet_db_entry_get */


/*******************************************************************************
* enet_db_link_num_entries_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of calendar entries for a channel and a 
*   database 
*
* INPUTS:
*   *enet_handle_ptr      - pointer to ENET handle instance
*   link                  - the link ID
*
* OUTPUTS:
*   *num_of_recs_ptr  - number of calendar entries required by the channel
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_db_link_num_entries_get(enet_handle_t *enet_handle_ptr, 
                                               UINT32         link,              
                                               UINT32        *num_of_recs_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    mapotn_db_key    key;
            
    PMC_ENTRY();
 
    PMC_ASSERT(num_of_recs_ptr != NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
           
    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } 
    else
    {
        /* query rd_channel_id data for the schd_addr */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = link;
        *num_of_recs_ptr = gen_db_query_count(&(enet_handle_ptr->base),
                                              enet_handle_ptr->cfg.schd_use_ctxt.db_id, 
                                              enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, 
                                              (void *)&key);

        if (*num_of_recs_ptr == 0)
        {
            PMC_RETURN(ENET_ERR_DB_ENTRY_NOT_FOUND);
        }
                
    }
    PMC_RETURN(result);
} /* enet_db_link_num_entries_get */


/*******************************************************************************
* enet_db_entry_all_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method for the datapath layer to retrieve the
*    all the entries of databases that are in use by the schedulers in ENET.
*
* INPUTS:
*   *enet_handle_ptr       - pointer to ENET handle instance
*
* OUTPUTS:
*   link_ptr               - 96 element array that function will populate with
*                            the link data
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_db_entry_all_get( enet_handle_t *enet_handle_ptr,
                                        UINT32 link_ptr[96] )                                   
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, num_of_recs;
    mapotn_db_key    key;
    mapotn_db_query_res *q_result;
    
    PMC_ENTRY();

    /* if data base does not exist return an error */
    if(enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB
       && enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode != UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB)
    {
        result = ENET_ERR_UNSUPPORTED_SCHD_DB_MODE;
    } else 
    {
        key.calendar_entry = CALENDAR_ENTRIES_NUM;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs = gen_db_query(&(enet_handle_ptr->base),
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_id, 
                                   enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle, 
                                   (void *)&key,
                                   (void **)&q_result);
        if(num_of_recs == 0)
        {
            PMC_RETURN(ENET_ERR_DB_ENTRY_NOT_FOUND);
        }

        for(i=0; i < num_of_recs; i++)
        {
            link_ptr[i] = q_result->p_recs[i]->channel_num;
        }
    }
    PMC_RETURN(result);
    
} /* enet_db_entry_all_get */


/*******************************************************************************
* enet_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a ENET subsystem instance.
*
*
* INPUTS:
*   *enet_handle      - pointer to ENET handle instance to be operated on
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
PUBLIC UINT32 enet_base_address_get(enet_handle_t *enet_handle)
{
    PMC_ENTRY();

    PMC_RETURN(enet_handle->base.base_address);
} /* enet_base_address_get */

/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/

/*******************************************************************************
*  enet_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize ENET block.
*
* INPUTS:
*   hndl                  - handle to the ENET instance to be operated on
*   link                  - requested link 
*   datapath              - enum, requested datapath
*   energy_state          - enum, requested energy state
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_init(enet_handle_t *hndl, 
                           UINT32 link, 
                           enet_datapath_t datapath, 
                           pmc_energy_state_request_t energy_state)
{
    PMC_ERROR          ret_val = PMC_SUCCESS;
    epmm_datapath_t    epmm_datapath = EPMM_10GE_GSUP43_73;
    etrans_datapath_t  etrans_datapath = ETRANS_GFP_GSUP43_C73_10GE_MAC;
    
    PMC_ENTRY();
 
    /* Convert datapth to TSB datapath type*/
    epmm_datapath     = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath   = enet_convert_datapath_to_etrans_datapath( datapath ); 

    /* for a RUNNNING request, we start by enabling block at top-level */
    if ( PMC_ENERGY_STATE_REQUEST_RUNNING == energy_state) 
    {
        /* top & EMPEI */
        if (PMC_SUCCESS == ret_val &&
            TRUE == enet_start_state_test(hndl))
        {
            /* top-level registers */
            enet_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
            enet_empei_e_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
            enet_empei_i_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);            
            ret_val = empei_e_init(hndl->empei_e_handle, PMC_ENERGY_STATE_RUNNING);  
            if (PMC_SUCCESS == ret_val) 
            {
                ret_val = empei_i_init(hndl->empei_i_handle, PMC_ENERGY_STATE_RUNNING);   
            }
        }
        
        /* EMAC + ETIME */
        if (PMC_SUCCESS == ret_val &&
            TRUE == emac_start_state_test(hndl->emac_handle))
        {
            ret_val = emac_init(hndl->emac_handle, PMC_ENERGY_STATE_RUNNING);            
        }
        /* EPMM and its sub-blocks */
        if (PMC_SUCCESS == ret_val &&
            TRUE == epmm_start_state_test(hndl->epmm_handle))
        {
            enet_epmm_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
        }
        /* ETRANS and its sub-blocks (only for enet line) */
        if(PMC_SUCCESS == ret_val &&
           ENET_LINE == hndl->cfg.enet_type_id  &&
           ETRANS_UNUSED != etrans_datapath &&          
           TRUE == etrans_start_state_test(hndl->etrans_handle))
        {            
            enet_etrans_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RUNNING);
        }        


        /* Enable interrupts to go to external pin */
        enet_int_init(hndl, TRUE);
    }
    
    /* EPMM/ETRANS block handles energy request for its sub-blocks */
    if (PMC_SUCCESS == ret_val) 
    {
        ret_val = epmm_init(hndl->epmm_handle, link,epmm_datapath,energy_state);
    }
    if(PMC_SUCCESS == ret_val && 
       ENET_LINE == hndl->cfg.enet_type_id &&
       (ETRANS_UNUSED != etrans_datapath ||
        (PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
         PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state)))
    {            
        ret_val = etrans_init(hndl->etrans_handle, link,etrans_datapath,energy_state);        
    }
    
    /* power down some block*/
    if (PMC_SUCCESS == ret_val) 
    {
        if ( PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
             PMC_ENERGY_STATE_REQUEST_OPTIMAL == energy_state) 
        {
            /* EPMM */
            if (PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
                TRUE == epmm_start_state_test(hndl->epmm_handle))
            {
                enet_epmm_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
            }
            /* ETRANS */
            if (ENET_LINE == hndl->cfg.enet_type_id)
            {
                if (PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
                    TRUE == etrans_start_state_test(hndl->etrans_handle))
                {
                    enet_etrans_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
                }
            }
            /* EMAC */
            if (PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
                TRUE == emac_start_state_test(hndl->emac_handle))
            {
                ret_val = emac_init(hndl->emac_handle, PMC_ENERGY_STATE_RESET); 
            }
            /* top & EMPEI */
            if (PMC_ENERGY_STATE_REQUEST_RESET == energy_state || 
                TRUE == enet_start_state_test(hndl))
            {
                ret_val = empei_e_init(hndl->empei_e_handle, PMC_ENERGY_STATE_RESET);  
                if (PMC_SUCCESS == ret_val) 
                {
                    ret_val = empei_i_init(hndl->empei_i_handle, PMC_ENERGY_STATE_RESET);   
                }

                if (FALSE == hndl->var.not_first_reset)
                {
                    enet_empei_e_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
                    enet_empei_i_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
                    hndl->var.not_first_reset = TRUE;
                }
                enet_energy_state_reg_set(hndl, PMC_ENERGY_STATE_RESET);
                enet_var_default_init(&hndl->var);

                /* Disable interrupts from going to external pin */
                enet_int_init(hndl, FALSE);
            }
        }
    }

    PMC_RETURN(ret_val);

} /* enet_init */

/*******************************************************************************
*  enet_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   uninitialize ENET block.
*
* INPUTS:
*   hndl     - handle to the ENET instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_uninit(enet_handle_t *hndl)
{
    PMC_ERROR rc = ENET_ERR_NO_SW_CONTENT;

    PMC_ENTRY();

    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   ENET subsystem instance.  
*
*
* INPUTS:
*   *hndl                      - pointer to ENET handle instance
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
PUBLIC BOOL8 enet_start_state_test(enet_handle_t *hndl)
{
    UINT32 link;
    BOOL8 result = TRUE;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* local var_t start state test */
    for (link = 0; result == TRUE && link < ENET_NUM_LINK_MAX; link++) 
    {
        if((hndl->var.ch_datapath[link] != ENET_UNPROV_DATAPATH) ||
           (hndl->var.lnk_ctxt[link].rx_state != ENET_LNK_START) ||
           (hndl->var.lnk_ctxt[link].tx_state != ENET_LNK_START) ||
           (hndl->var.dpi_sch_slot[link] != 0xF) ||
           (hndl->var.lnk_ctxt[link].empei_i_eclass_en != FALSE) ||
           (hndl->var.lnk_ctxt[link].empei_e_eclass_en != FALSE) ||
           (hndl->var.lnk_ctxt[link].ing_ts_en != FALSE) ||
           (hndl->var.lnk_ctxt[link].egr_ts_en != FALSE))
        {
            result = FALSE;
        }
    }
    
    if(result == TRUE){
        if((emac_start_state_test(hndl->emac_handle) == FALSE) ||
           (epmm_start_state_test(hndl->epmm_handle) == FALSE) ||
           (empei_e_start_state_test(hndl->empei_e_handle) == FALSE) ||
           (empei_i_start_state_test(hndl->empei_i_handle) == FALSE))
        {
            result = FALSE; 
        }
    }
    
    if(result == TRUE && ENET_LINE == hndl->cfg.enet_type_id)
    {
        if(etrans_start_state_test(hndl->etrans_handle) == FALSE)
            result = FALSE;
    }

    PMC_RETURN(result);
}/* enet_start_state_test */

/*******************************************************************************
*  enet_datapath_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure link for the specified datapath
*
* INPUTS:
*   hndl          - handle to the ETRANS instance to be operated on
*   link          - link to configure
*                   10GE: links 0-11 are valid
*                   40GE: only links 0, 4, 8 are valid
*                   100GE: only link 0 is valid
*   datapath      - datapath to configure
*   chan          - MAPOTN channel (only used for MAPOTN source/destination)
*   ts_en         - Enable the time stamp functionality
*   use_page_swap - Enable the use of page swap for this datapath.
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_datapath_cfg(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, UINT32 chan, BOOL8 ts_en, BOOL8 use_page_swap)
{
    PMC_ERROR rc                              = PMC_SUCCESS;
    emac_data_rate_t         emac_data_rate;
    emac_rx_data_src_t       emac_rx_data_src  = EMAC_RX_SRC_RX_SERDES;
    emac_tx_data_dst_t       emac_tx_data_dst  = EMAC_TX_DST_TX_PMA;
    etime_data_rate_t        etime_data_rate;

    empei_i_data_rate_enum_t empei_i_data_rate;
    empei_e_data_rate_enum_t empei_e_data_rate;
    empei_e_datapath_type_enum_t empei_e_dp_sel = EMPEI_E_TRANSPARENT_DATAPATH;

    epmm_datapath_t             epmm_datapath = EPMM_10GE_GSUP43_73;
    etrans_datapath_t         etrans_datapath = ETRANS_GFP_GSUP43_C73_10GE_MAC;
    
    UINT32 ts_timer[] = {0,0,0,0,0,0,0,0,0,0,0,0};

    PMC_ENTRY();

    /* Check for valid link and datapath */
    rc = enet_validate_datapath_cfg( hndl, link, datapath, TRUE );
    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }

    if (hndl->var.lnk_ctxt[link].rx_state != ENET_LNK_START || hndl->var.lnk_ctxt[link].tx_state != ENET_LNK_START)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_START); /* it has either been already provisioned or activated*/
    }

    /* Convert datapth to TSB datapath type*/
    emac_data_rate    = enet_convert_datapath_to_emac_data_rate( datapath );
    emac_rx_data_src  = enet_convert_datapath_to_emac_rx_data_src( datapath );
    emac_tx_data_dst  = enet_convert_datapath_to_emac_tx_data_dst( datapath );
    etime_data_rate   = enet_convert_datapath_to_etime_data_rate( datapath );    
    empei_i_data_rate = enet_convert_datapath_to_empei_i_data_rate( datapath );
    empei_e_data_rate = enet_convert_datapath_to_empei_e_data_rate( datapath );
    empei_e_dp_sel    = enet_convert_datapath_to_empei_e_datapath_sel( datapath );
    epmm_datapath     = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath   = enet_convert_datapath_to_etrans_datapath( datapath );

    /* Updata handle->var struct */    
    enet_update_hndl_var( hndl, link, datapath, ts_en, TRUE );


    /*  Enable Links */
    switch (emac_data_rate)
    {
    case EMAC_LINK_10G:
        enet_field_range_LINK_10G_EN_set(NULL,hndl,link,link,1);
        break;
    case EMAC_LINK_40G:
        enet_field_range_LINK_40G_EN_set(NULL,hndl,link/4,link/4,1);
        break;
    case EMAC_LINK_100G:
        enet_field_LINK_100G_EN_set(NULL,hndl,1);
        break;
    default:
        PMC_RETURN(EMAC_LOG_ERR_INVALID_RATE);
        break;
    }
    
    /* by default */
    if (PMC_SUCCESS == rc) 
    {
        (void)emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
    }

    /* EMAC Source */
    switch (datapath) 
    {
        /* SERDES Datapath configuration. */
    case ENET_SERDES_100GE_MAC:
    case ENET_SERDES_40GE_MAC:
    case ENET_SERDES_10GE_MAC: 
    {
        /*Enable EMAC to Insert FCS at end of packet*/
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_sys_fcs_enable( hndl->empei_e_handle, link, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {            
            rc = empei_e_cpb_link_sch_cfg     (hndl->empei_e_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
    
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_e_internal_link_sch_cfg(hndl->empei_e_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_i_cpb_link_sch_cfg     (hndl->empei_i_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_i_internal_link_sch_cfg(hndl->empei_i_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }

        /* Configure EPMM to enable MPMA scheduler for ingress path*/
        if (PMC_SUCCESS == rc) 
        { 
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }

        /* Configure EMAC and EMPEI for MAC Terminated datapath */
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        if(TRUE == ts_en) 
        {
            if (PMC_SUCCESS == rc) 
            {                 
                rc = etime_rx_ts_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_CFC_48B_MODE, 0);
            }
            if (PMC_SUCCESS == rc) 
            {         
                rc = etime_rx_ts_ena( hndl->emac_handle->etime_handle, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            { 
                rc = etime_tx_ts_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_CFC_48B_MODE, 0);
            }
            if (PMC_SUCCESS == rc) 
            { 
                rc = etime_tx_ts_ena( hndl->emac_handle->etime_handle, link, TRUE);
            }
            /* two step */
            if (PMC_SUCCESS == rc) 
            {         
                rc = etime_refl_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_REFL_DROP, ETIME_REFL_DST_CPU, use_page_swap);
            }
            if (PMC_SUCCESS == rc) 
            { 
                rc = etime_refl_ena( hndl->emac_handle->etime_handle, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            {             
                rc = etime_ptp_cntr_cfg( hndl->emac_handle->etime_handle, 0, ETIME_PTP_CNTR_125MHZ, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            { 
                rc = empei_e_ts_back_to_back_cfg( hndl->empei_e_handle, 0xFFF, 0, ts_timer );
            }
        }
    }
    break;                
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    {        
        if (PMC_SUCCESS == rc) 
        {                         
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_link_lowpwr_cfg( hndl->emac_handle, link, emac_data_rate, EMAC_DIR_EGR, TRUE );
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, TRUE );
        }

        if (PMC_SUCCESS == rc) 
        {                         
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {                         
            rc = epmm_egress_cfg (hndl->epmm_handle, link, epmm_datapath, TRUE);
        }

        break;      
    }
    /* Datapath with Ingress also routed to the Rx MAC for PMON */
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    {

        if (PMC_SUCCESS == rc) 
        {                         
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_link_lowpwr_cfg( hndl->emac_handle, link, emac_data_rate, EMAC_DIR_EGR, TRUE );
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, TRUE );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = epmm_egress_cfg (hndl->epmm_handle, link, epmm_datapath, TRUE);
        }

        break;      
    }
    /* The configurations below are used for PMON only */
    /* CPB -> Tx EPMM -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
    {

        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_link_lowpwr_cfg( hndl->emac_handle, link, emac_data_rate, EMAC_DIR_EGR, TRUE );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, TRUE );
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = epmm_egress_cfg (hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_i_cpb_link_sch_cfg     (hndl->empei_i_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_i_internal_link_sch_cfg(hndl->empei_i_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }

        break;
    }
    /* CPB -> Tx MAC -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON: 
    {
        /* These function do nothing for now */
        /* empei_i_datapath_cfg( hndl->empei_i_handle, link, empei_i_data_rate, EMPEI_I_MAC_DATAPATH, TRUE );*/
        /* empei_e_datapath_cfg( hndl->empei_e_handle, link, empei_e_data_rate, EMPEI_E_MAC_DATAPATH, TRUE );*/
        /*Enable EMAC to Insert FCS at end of packet*/

        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_e_sys_fcs_enable( hndl->empei_e_handle, link, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        { 
            /* empei_i_chan_map_cfg( hndl->empei_i_handle, link, link ); */
            rc = empei_e_cpb_link_sch_cfg     (hndl->empei_e_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_e_internal_link_sch_cfg(hndl->empei_e_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            /* empei_e_update_sch_cfg       (hndl->empei_e_handle); */
            rc = empei_i_cpb_link_sch_cfg     (hndl->empei_i_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = empei_i_internal_link_sch_cfg(hndl->empei_i_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }

        if (PMC_SUCCESS == rc) 
        { 
            /* Configure EPMM to enable MPMA scheduler for ingress path*/
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        /* Configure EMAC and EMPEI for MAC Terminated datapath */
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, TRUE );
        }
        break;
    }
    /* MAPOTN Datapath configuration. Only valid for ENET_LINE. Not valid for ENET_SYS. */
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    {       
        /*Enable EMAC to Insert FCS at end of packet*/
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_sys_fcs_enable( hndl->empei_e_handle, link, TRUE);
        }
        /* empei_i_chan_map_cfg( hndl->empei_i_handle, link, link ); */
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_cpb_link_sch_cfg     (hndl->empei_e_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_internal_link_sch_cfg(hndl->empei_e_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }
        /* empei_e_update_sch_cfg       (hndl->empei_e_handle); */
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_cpb_link_sch_cfg     (hndl->empei_i_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_internal_link_sch_cfg(hndl->empei_i_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }


        /* Enable EMPEI-E GFP mode only for GFP traffic */
        if ((datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC))
        {
            if (PMC_SUCCESS == rc)
            {
                rc = empei_e_gfp_enable( hndl->empei_e_handle, link, TRUE);
            }
        }
        else
        {
            if (PMC_SUCCESS == rc)
            {
                rc = empei_e_gfp_enable( hndl->empei_e_handle, link, FALSE);
            }
        }

        /* Configure EPMM to enable MPMA scheduler for ingress path*/
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = etrans_rx_cfg(hndl->etrans_handle, link, chan, etrans_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = etrans_tx_cfg(hndl->etrans_handle, link, chan, etrans_datapath, TRUE, use_page_swap, FALSE);
        }

        /* Configure EMAC and EMPEI for MAC Terminated datapath */
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        /* Disable consequential action for egress -> ingress loopbacks */
        /*             hndl:              link: auto_fault_handling: send_lf: send_rf: */
        if (PMC_SUCCESS == rc) 
        {    
            rc = emac_lf_rf_cfg(hndl->emac_handle, link, FALSE, FALSE, FALSE, FALSE);
        }

        if(TRUE == ts_en) 
        {
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_rx_ts_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_CFC_48B_MODE, 0);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_rx_ts_ena( hndl->emac_handle->etime_handle, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_tx_ts_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_CFC_48B_MODE, 0);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_tx_ts_ena( hndl->emac_handle->etime_handle, link, TRUE);
            }
            /* two step */
            
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_refl_cfg( hndl->emac_handle->etime_handle, link, etime_data_rate, ETIME_REFL_DROP, ETIME_REFL_DST_CPU, use_page_swap);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_refl_ena( hndl->emac_handle->etime_handle, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_ptp_cntr_cfg( hndl->emac_handle->etime_handle, 0, ETIME_PTP_CNTR_125MHZ, TRUE);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = empei_e_ts_back_to_back_cfg( hndl->empei_e_handle, 0xFFF, 0, ts_timer );
            }
        }
        break;
    }
    /* MAPOTN -> Rx ETRANS -> Rx MAC for PMON */
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
    {
        /* These function do nothing for now */
        /* empei_i_datapath_cfg( hndl->empei_i_handle, link, empei_i_data_rate, EMPEI_I_MAC_DATAPATH, TRUE );*/
        /* empei_e_datapath_cfg( hndl->empei_e_handle, link, empei_e_data_rate, EMPEI_E_MAC_DATAPATH, TRUE );*/
        /*Enable EMAC to Insert FCS at end of packet*/
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_sys_fcs_enable( hndl->empei_e_handle, link, TRUE);
        }
        /* empei_i_chan_map_cfg( hndl->empei_i_handle, link, link ); */
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_cpb_link_sch_cfg     (hndl->empei_e_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_internal_link_sch_cfg(hndl->empei_e_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }
        /* empei_e_update_sch_cfg       (hndl->empei_e_handle); */
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_cpb_link_sch_cfg     (hndl->empei_i_handle, 11, 0, FALSE, hndl->var.dpi_sch_slot);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_internal_link_sch_cfg(hndl->empei_i_handle, 11, FALSE, hndl->var.dpi_sch_slot);
        }

        /* Enable EMPEI-E GFP mode only for GFP traffic */
        if ( datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON ) 
        {
            rc = empei_e_gfp_enable( hndl->empei_e_handle, link, TRUE);
        } else 
        {
            rc = empei_e_gfp_enable( hndl->empei_e_handle, link, FALSE);
        }

        /* Configure EPMM to enable MPMA scheduler for ingress path*/
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = etrans_rx_cfg(hndl->etrans_handle, link, chan, etrans_datapath, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {
            /* PMON only mode */
            rc = etrans_tx_cfg(hndl->etrans_handle, link, chan, etrans_datapath, TRUE, use_page_swap, TRUE); 
        }
        /* Configure EMAC and EMPEI for MAC Terminated datapath */
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_cfg( hndl->emac_handle, link, emac_data_rate, emac_rx_data_src, emac_tx_data_dst );
        }
        /* Disable consequential action for egress -> ingress loopbacks */
        /*             hndl:              link: auto_fault_handling: send_lf: send_rf: */
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_lf_rf_cfg(hndl->emac_handle, link, FALSE, FALSE, FALSE, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, TRUE );
        }
        break;      

    }
    default : 
    {
        PMC_ASSERT(FALSE,ENET_LOG_ERR_INVALID_PARAMETERS,0,0);
        break;
    }
    }

    /* Configure EMPEI_I to select MAC or Transparent datapath to CPB */
    if (PMC_SUCCESS == rc) 
    {
        rc = empei_e_datapath_sel_link_cfg(hndl->empei_e_handle, link, empei_e_dp_sel);
    }   

    /* If successful, then increment the number of configured datapaths */
    hndl->var.lnks_prov_cnt++;
    hndl->var.lnk_ctxt[link].rx_state = ENET_LNK_EQUIPPED;
    hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_EQUIPPED;
       
    PMC_RETURN(rc);

} /* enet_datapath_cfg */


/*******************************************************************************
*  enet_datapath_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unconfigure link for the specified datapath
*
* INPUTS:
*   hndl - handle to the ETRANS instance to be operated on
*   link - link to configure
*          10GE: links 0-11 are valid
*          40GE: only links 0, 4, 8 are valid
*          100GE: only link 0 is valid
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_datapath_uncfg(enet_handle_t *hndl, UINT32 link)
{
    PMC_ERROR          rc = PMC_SUCCESS;
    enet_datapath_t    datapath;
    epmm_datapath_t    epmm_datapath = EPMM_10GE_GSUP43_73;
    etrans_datapath_t  etrans_datapath = ETRANS_GFP_GSUP43_C73_10GE_MAC;
    emac_data_rate_t   emac_data_rate;

    PMC_ENTRY();

    if (hndl->var.lnk_ctxt[link].rx_state != ENET_LNK_EQUIPPED || hndl->var.lnk_ctxt[link].tx_state != ENET_LNK_EQUIPPED)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_EQUIPPED); 
    }

    datapath = hndl->var.ch_datapath[link];
    /* Check for valid link and datapath */
    rc = enet_validate_datapath_cfg( hndl, link, datapath, FALSE );
    PMC_ASSERT(rc==PMC_SUCCESS, rc, 0, 0);

    /* Convert datapth to TSB datapath type*/
    emac_data_rate  = enet_convert_datapath_to_emac_data_rate( datapath );
    epmm_datapath   = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath = enet_convert_datapath_to_etrans_datapath( datapath );

    /* Updata handle->var struct */    
    enet_update_hndl_var( hndl, link, datapath, FALSE, FALSE );
    /* Disable classification on the ENET link */
    if (PMC_SUCCESS == rc) 
    {
         rc = eclass_deactivate(hndl->empei_e_handle->eclass_handle, 
                                link);
    }
    if (PMC_SUCCESS == rc) 
    {
         rc = eclass_deactivate(hndl->empei_i_handle->eclass_handle,
                                link);
    }

    switch (datapath) 
    {
        /* SERDES Datapath configuration. */
    case ENET_SERDES_100GE_MAC:
    case ENET_SERDES_40GE_MAC:
    case ENET_SERDES_10GE_MAC: 
    {
        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_link_reset( hndl->empei_i_handle, link);
        }
        if (PMC_SUCCESS == rc) 
        {
            empei_e_link_reset( hndl->empei_e_handle, link);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }

        /* Disable timestamp if this is the last link*/
        if( hndl->var.lnks_prov_cnt == 1 &&  hndl->var.lnk_ctxt[link].ing_ts_en == TRUE ) {
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_rx_ts_ena( hndl->emac_handle->etime_handle, FALSE);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_refl_lnk_rst( hndl->emac_handle->etime_handle, link);
            }
            hndl->var.lnk_ctxt[link].ing_ts_en = FALSE;
        }

        if( hndl->var.lnk_ctxt[link].egr_ts_en == TRUE ) {
            if (PMC_SUCCESS == rc) 
            {
                rc = etime_tx_ts_ena( hndl->emac_handle->etime_handle, link, FALSE);            
            }
            hndl->var.lnk_ctxt[link].egr_ts_en = FALSE;
        }
          
        break;                
    }
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    {
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_egress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }      
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
        }

        break;      
    }
    /* Datapath with Ingress also routed to the Rx MAC for PMON */
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
        /* The configurations below are used for PMON only */
        /* CPB -> Tx EPMM -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
    {
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_egress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
        }
        break;      
    }      
    /* CPB -> Tx MAC -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON: 
    {
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }
        if (PMC_SUCCESS == rc) 
        { 
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }          
        break;
    }

    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        if ((datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON))
        {
            rc = empei_e_gfp_enable( hndl->empei_e_handle, link, FALSE);
        }

        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_link_reset( hndl->empei_i_handle, link);
        }
        if (PMC_SUCCESS == rc) 
        {
            empei_e_link_reset( hndl->empei_e_handle, link);
        }

        if (PMC_SUCCESS == rc) 
        {                                
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {    
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {    
            rc = etrans_rx_cfg(hndl->etrans_handle, link, link, etrans_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {    
            /* PMON only mode */
            rc = etrans_tx_cfg(hndl->etrans_handle, link, link, etrans_datapath, FALSE, FALSE, TRUE);
        }
        break;

    /* MAPOTN Datapath configuration. Only valid for ENET_LINE. Not valid for ENET_SYS. */
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    {
        if ((datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC)||
            (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC))
        {
            rc = empei_e_gfp_enable( hndl->empei_e_handle, link, FALSE);
        }

        if (PMC_SUCCESS == rc) 
        {
            rc = empei_i_link_reset( hndl->empei_i_handle, link);
        }
        if (PMC_SUCCESS == rc) 
        {
            empei_e_link_reset( hndl->empei_e_handle, link);
        }

        if (PMC_SUCCESS == rc) 
        {                                
            rc = emac_mac_uncfg(hndl->emac_handle, link, emac_data_rate);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = emac_ingr_data_sink_cfg( hndl->emac_handle, link, emac_data_rate, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {    
            rc = epmm_ingress_cfg(hndl->epmm_handle, link, epmm_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc)
        {    
            rc = etrans_rx_cfg(hndl->etrans_handle, link, link, etrans_datapath, FALSE);
        }
        if (PMC_SUCCESS == rc) 
        {    
            rc = etrans_tx_cfg(hndl->etrans_handle, link, link, etrans_datapath, FALSE, FALSE, FALSE);
        }
            
        /* Disable timestamp if this is the last link*/
        if( hndl->var.lnks_prov_cnt == 1 &&  hndl->var.lnk_ctxt[link].ing_ts_en == TRUE ) 
        {
            if (PMC_SUCCESS == rc) 
            {                        
                rc = etime_rx_ts_ena( hndl->emac_handle->etime_handle, FALSE);
            }
            hndl->var.lnk_ctxt[link].ing_ts_en = FALSE;
        }

        if( hndl->var.lnk_ctxt[link].egr_ts_en == TRUE ) 
        {
            if (PMC_SUCCESS == rc) 
            { 
                rc = etime_tx_ts_ena( hndl->emac_handle->etime_handle, link, FALSE);            
            }
            hndl->var.lnk_ctxt[link].egr_ts_en = FALSE;
        }

        break;
    }
    default : 
    {
        PMC_ASSERT(FALSE,ENET_ERR_CODE_ASSERT,0,0);
        break;
    }
    }

    if (PMC_SUCCESS == rc)
    {
        /* Configure link for the specified datapath */
        /* Register 0xC: ENET120_TOP - ENET Datapath Configuration */
        /* Disable Links */
        switch (emac_data_rate)
        {
        case EMAC_LINK_10G:
            enet_field_range_LINK_10G_EN_set(NULL,hndl,link,link,0);
            break;
        case EMAC_LINK_40G:
            enet_field_range_LINK_40G_EN_set(NULL,hndl,link/4,link/4,0);
            break;
            
        case EMAC_LINK_100G:
            enet_field_LINK_100G_EN_set(NULL,hndl,0);
            break;
        default:
            PMC_RETURN(EMAC_LOG_ERR_INVALID_RATE);
            break;            
        }
        
        /* If successful, then decrement the number of configured datapaths */
        hndl->var.lnks_prov_cnt--;
        
        hndl->var.lnk_ctxt[link].rx_state = ENET_LNK_START;
        hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_START;
    }

    PMC_RETURN(rc);

} /* enet_datapath_uncfg */

/*******************************************************************************
*  enet_tx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Tx EGRESS path (from CPB -> (EMPEI_E -> MAC -> ETRANS) or EPMM ) ->
*                          to SERDES or MAPOTN.
*
* INPUTS:
*   hndl     - handle to the ENET instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_tx_activate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath)
{
    BOOL8   transparent_path = FALSE;
    BOOL8   mapotn_enet_pmon = FALSE;            
    epmm_datapath_t    epmm_datapath;
    etrans_datapath_t  etrans_datapath;
    PMC_ERROR          rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (hndl->var.lnk_ctxt[link].tx_state != ENET_LNK_EQUIPPED)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_EQUIPPED); 
    }
    

    /* convert datapath*/
    epmm_datapath   = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath = enet_convert_datapath_to_etrans_datapath( datapath );

    switch (datapath) 
    {
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
        transparent_path = TRUE;
        break;
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        mapotn_enet_pmon = TRUE;    
        break;
    default:
        break;
    }

    if (mapotn_enet_pmon)
    {     
        if (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON || 
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ||
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ||
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON)
        {
            rc = etrans_tx_activate(hndl->etrans_handle,link, etrans_datapath);
        }
        /* update link state */
        if (PMC_SUCCESS == rc) 
        {
            hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_OPERATIONAL;
        }
        PMC_RETURN(rc);
    }
   

    /* Enable EPMM or ETRANS/EMAC/EMPEI path based on the datapath */
    if( hndl->var.lnk_ctxt[link].empei_e_eclass_en == TRUE ) 
    {
        rc = eclass_activate(hndl->empei_e_handle->eclass_handle, link );
    }

    if (PMC_SUCCESS == rc) 
    {
#if  defined (PMC_SW_SIMULATION)
        /* IN verification or simulation mode, this function returns an error since pooling timeout expires*/
        empei_e_activate_datapath(hndl->empei_e_handle);
#else
        rc = empei_e_activate_datapath(hndl->empei_e_handle);
#endif
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = emac_tx_enable(hndl->emac_handle, link, TRUE);
    }
    if (PMC_SUCCESS == rc && TRUE == transparent_path) 
    {
        rc = epmm_tx_activate(hndl->epmm_handle,link, epmm_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = etrans_tx_activate(hndl->etrans_handle,link, etrans_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {


        if (PMC_SUCCESS == rc) 
        {
            rc = empei_e_link_enable(hndl->empei_e_handle,link, TRUE);
        }
        if (PMC_SUCCESS == rc) 
        {
            if (TRUE == enet_is_gfp_datapath_get(etrans_datapath))
            {
                empei_e_gfp_enable(hndl->empei_e_handle, link, TRUE);
            }
        }
    }
    
    /* wait until ETRANS is locked */
    if (PMC_SUCCESS == rc) 
    {
        if (TRUE == enet_is_gfp_datapath_get(etrans_datapath))
        {
            if (FALSE == enet_etrans_tx_lock_status_get(hndl, link, etrans_datapath))
            {
                rc = ENET_ERR_ETRANS_NOT_LOCKED;
            }
        }
    }

    /* update link state */
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_OPERATIONAL;
    }

    
    PMC_RETURN(rc);

} /* enet_tx_activate */

/*******************************************************************************
*  enet_tx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Tx path.
*
* INPUTS:
*   hndl     - handle to the ENET instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_tx_deactivate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath)
{
    /* Disable EPMM or ETRANS/EMAC/EMPEI path based on the datapath */
    BOOL8              transparent_path = FALSE;
    BOOL8 mapotn_enet_pmon = FALSE;
    epmm_datapath_t   epmm_datapath;
    etrans_datapath_t etrans_datapath;
    PMC_ERROR         rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (hndl->var.lnk_ctxt[link].tx_state != ENET_LNK_OPERATIONAL)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_OPERATIONAL); 
    }

    epmm_datapath   = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath = enet_convert_datapath_to_etrans_datapath( datapath );
    
    switch (datapath) 
    {
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
        transparent_path = TRUE;
        break;
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        mapotn_enet_pmon = TRUE;
        break;
    default:
        break;
    }
  
    if (mapotn_enet_pmon)
    {
        if (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON || 
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ||
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ||
            datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON)
        {
            rc = etrans_tx_deactivate(hndl->etrans_handle,link, etrans_datapath);
        }
        if (PMC_SUCCESS == rc) 
        {
            hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_EQUIPPED;
        }
        PMC_RETURN(rc);
    }

    /* Enable EPMM or ETRANS/EMAC/EMPEI path based on the datapath */
    if (PMC_SUCCESS == rc) 
    {
        rc = etrans_tx_deactivate(hndl->etrans_handle, link, etrans_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = emac_tx_enable(hndl->emac_handle,   link, FALSE);
    }
    if (PMC_SUCCESS == rc && TRUE == transparent_path) 
    {
        rc = epmm_tx_deactivate(hndl->epmm_handle,   link, epmm_datapath); 
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = empei_e_link_enable(hndl->empei_e_handle, link, FALSE);
        if (PMC_SUCCESS == rc) 
        {
            if (TRUE == enet_is_gfp_datapath_get(etrans_datapath))
            {
                empei_e_gfp_enable(hndl->empei_e_handle, link, FALSE);
            }
        }
    }
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.lnk_ctxt[link].tx_state = ENET_LNK_EQUIPPED;
    }
    PMC_RETURN(rc);

} /* enet_tx_deactivate */


/*******************************************************************************
*  enet_rx_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Rx INGRESS path. from SERDES or MAPOTN -> (EMAC -> EMPEI_I ) -> CPB
*
* INPUTS:
*   hndl     - handle to the ENET instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_rx_activate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath)
{
    PMC_ERROR         rc = PMC_SUCCESS;
    epmm_datapath_t   epmm_datapath;
    etrans_datapath_t etrans_datapath;
    BOOL8 enable_emac = TRUE;

    PMC_ENTRY();

    if (hndl->var.lnk_ctxt[link].rx_state != ENET_LNK_EQUIPPED)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_EQUIPPED); 
    }

    epmm_datapath = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath = enet_convert_datapath_to_etrans_datapath( datapath );

    /* Enable EPMM or ETRANS/EMAC/EMPEI path based on the datapath */
    /* Configure EMPEI_I to select MAC or Transparent datapath to CPB */
    /* Default path is to MAC/PCS */

    if( hndl->var.lnk_ctxt[link].empei_i_eclass_en == TRUE ) {
        rc = eclass_activate(hndl->empei_i_handle->eclass_handle, link );
    }

    if ((datapath == ENET_SERDES_10GE_MAC                    ) ||
        (datapath == ENET_SLAVE_10GE_MAC_TX_PMON             ) ||
        (datapath == ENET_SERDES_40GE_MAC                    ) ||
        (datapath == ENET_SLAVE_40GE_MAC_TX_PMON             ) ||
        (datapath == ENET_SERDES_100GE_MAC                   ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON            ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC          ) ||
        (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC           ) ||
        (datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC         ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON      ) ||
        (datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON)) {
        if (PMC_SUCCESS == rc) 
        {
#if  defined (PMC_SW_SIMULATION)
            /* In verification or sw simulation mode, this function returns an error since pooling timeout expires*/
            empei_i_activate_datapath(hndl->empei_i_handle);
#else
            rc = empei_i_activate_datapath(hndl->empei_i_handle);
#endif
        }
    }
    switch (datapath)
    {
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_100GE_TRANSPARENT:
        enable_emac = FALSE;
        break;
    default:
        break;            
    }
    if (PMC_SUCCESS == rc) 
    {
        PMC_LOG_TRACE("ENET RX ACTIVATE: en_emac = %d\n", enable_emac);
        rc =  emac_rx_enable (hndl->emac_handle, link, enable_emac);
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = etrans_rx_activate(hndl->etrans_handle, link, etrans_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {
        rc = epmm_rx_activate  (hndl->epmm_handle,   link, epmm_datapath);
    }
    if (PMC_SUCCESS == rc)
    {
        rc = empei_i_link_enable(hndl->empei_i_handle, link, TRUE);
    }
    
    if (PMC_SUCCESS == rc) 
    {
        hndl->var.lnk_ctxt[link].rx_state = ENET_LNK_OPERATIONAL;
    }

    PMC_RETURN(rc);

} /* enet_rx_activate */

/*******************************************************************************
*  enet_rx_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Rx path.
*
* INPUTS:
*   hndl     - handle to the ENET instance to be operated on
*   link     - link to configure
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_rx_deactivate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath)
{
    PMC_ERROR         rc = PMC_SUCCESS;
    epmm_datapath_t   epmm_datapath;
    etrans_datapath_t etrans_datapath;
    BOOL8 enable_emac = TRUE;

    PMC_ENTRY();
    
    if (hndl->var.lnk_ctxt[link].rx_state != ENET_LNK_OPERATIONAL)
    {
        PMC_RETURN(ENET_LNK_STATE_NOT_OPERATIONAL); 
    }

    epmm_datapath = enet_convert_datapath_to_epmm_datapath( datapath );
    etrans_datapath = enet_convert_datapath_to_etrans_datapath( datapath );

    switch (datapath)
    {
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_100GE_TRANSPARENT:
        enable_emac = FALSE;
        break;
    default:
        break;            
    }

    if (TRUE == enable_emac)
    {
        /* Disable EPMM or ETRANS/EMAC/EMPEI path based on the datapath */
        if (PMC_SUCCESS == rc) 
        {
            emac_rx_enable (hndl->emac_handle, link, FALSE);
        }
    }

    /* Note: This function does nothing for non-MAPOTN datapaths */
    if (PMC_SUCCESS == rc) 
    {
        etrans_rx_deactivate(hndl->etrans_handle, link, etrans_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {
        epmm_rx_deactivate  (hndl->epmm_handle,   link, epmm_datapath);
    }
    if (PMC_SUCCESS == rc) 
    {        
        empei_i_link_enable(hndl->empei_i_handle, link, FALSE);
    }

    if (PMC_SUCCESS == rc) 
    {
        hndl->var.lnk_ctxt[link].rx_state = ENET_LNK_EQUIPPED;
    }

    PMC_RETURN(rc);

} /* enet_rx_deactivate */

/*******************************************************************************
* enet_is_link_mapotn_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function specifies if the ENET_LINE processing mode for a link is
*   used with the MAPOTN subsystem or not.  If not used with MAPOTN subsystem
*   the ENET_LINE link is associated with a LINE Serdes port.
*
*   If the ENET_LINE link processing mode is not valid, this function
*   asserts.
*
* INPUTS:
*   hndl                 - handle to the ENET instance to be operated on
*   link                 - link to query \n
*   mapotn_chnl_ptr      - mapotn channel associated with this link\n
*                          This is only valid if the return code is true.
*
* OUTPUTS:
*   mapotn_chnl_ptr     - Pointer to MAPOTN DPI channel ID used by the ENET link.
*
* RETURNS:
*   BOOL8                 - ENET_LINE datapath_t usage with MAPOTN: \n
*                          TRUE:  MAPOTN is used as part of the ENET_LINE mode \n
*                          FALSE: MAPOTN is not used as part of the ENET_LINE 
*                                 mode \n
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 enet_is_link_mapotn_test(enet_handle_t *hndl, 
                                     UINT32 link, 
                                     UINT32* mapotn_chnl_ptr) 
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();
    
    if (hndl->var.ch_datapath[link] < ENET_DATAPATH_LAST_SERDES_OR_SLAVE) {

    } else if ((hndl->var.ch_datapath[link] > ENET_DATAPATH_LAST_SERDES_OR_SLAVE)  && 
               (hndl->var.ch_datapath[link] < ENET_UNPROV_DATAPATH)) {
        *mapotn_chnl_ptr = etrans_ingress_link_to_chan_map_get(hndl->etrans_handle, link);
        rc = TRUE;
    } else {
    
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN(rc);

} /* enet_is_link_mapotn_type */

/*******************************************************************************
*  enet_sw_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapath blocks
*
* INPUTS:
*   hndl           - Handle to the enet instance
*   link           - ENET link     to reset
*   datapath       - ENET datapath to reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_sw_reset(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath)
{
    emac_data_rate_t             emac_data_rate;
    emac_rx_data_src_t           emac_rx_data_src  = EMAC_RX_SRC_RX_SERDES;
    emac_tx_data_dst_t           emac_tx_data_dst  = EMAC_TX_DST_TX_PMA;
    empei_e_data_rate_enum_t     empei_e_data_rate;
    empei_e_datapath_type_enum_t empei_e_dp_sel    = EMPEI_E_TRANSPARENT_DATAPATH;
    empei_i_data_rate_enum_t     empei_i_data_rate;
    epmm_datapath_t              epmm_datapath     = EPMM_10GE_GSUP43_73;
    etime_data_rate_t            etime_data_rate;
    etrans_datapath_t            etrans_datapath   = ETRANS_GFP_GSUP43_C73_10GE_MAC;

    PMC_ENTRY();

    /* Convert datapaths to TSB type */
    emac_data_rate    = enet_convert_datapath_to_emac_data_rate      (datapath);
    emac_rx_data_src  = enet_convert_datapath_to_emac_rx_data_src    (datapath);
    emac_tx_data_dst  = enet_convert_datapath_to_emac_tx_data_dst    (datapath);
    empei_e_data_rate = enet_convert_datapath_to_empei_e_data_rate   (datapath);
    empei_e_dp_sel    = enet_convert_datapath_to_empei_e_datapath_sel(datapath);
    empei_i_data_rate = enet_convert_datapath_to_empei_i_data_rate   (datapath);
    epmm_datapath     = enet_convert_datapath_to_epmm_datapath       (datapath);
    etime_data_rate   = enet_convert_datapath_to_etime_data_rate     (datapath);
    etrans_datapath   = enet_convert_datapath_to_etrans_datapath     (datapath);

    switch (datapath)
    {
    case ENET_SERDES_100GE_TRANSPARENT           :
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT :
    case ENET_SERDES_10GE_TRANSPARENT            :
    case ENET_SERDES_40GE_TRANSPARENT            :
    {
        epmm_sw_reset(hndl->epmm_handle, link, epmm_datapath);
        break;
    }

    /* Note: All of these break at default */
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC                 :
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON            :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC            :
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON       :
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON        :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON        :
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON        :
    case ENET_MAPOTN_GMP_G709_100GE_MAC                  :
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON             :
    case ENET_MAPOTN_GMP_G709_40GE_MAC                   :
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON              :
    case ENET_SERDES_100GE_MAC                           :
    case ENET_SLAVE_100GE_MAC_TX_PMON                    :
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON           :
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON            :
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON :
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON  :
    case ENET_SERDES_10GE_MAC                            :
    case ENET_SLAVE_10GE_MAC_TX_PMON                     :
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON            :
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON             :
    case ENET_SERDES_40GE_MAC                            :
    case ENET_SLAVE_40GE_MAC_TX_PMON                     :
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON            :
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON             :

    default :
    {
        break;
    }
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  enet_lowpwr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Global soft reset of 10, 40, and/or 100G datapath blocks
*
* INPUTS:
*   hndl           - Handle to the enet instance
*   link           - ENET link     to reset
*   datapath       - ENET datapath to reset
*   value          - TRUE/FALSE = LOWPWR/Not
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_lowpwr(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, BOOL8 value)
{
    emac_data_rate_t             emac_data_rate;
    emac_rx_data_src_t           emac_rx_data_src  = EMAC_RX_SRC_RX_SERDES;
    emac_tx_data_dst_t           emac_tx_data_dst  = EMAC_TX_DST_TX_PMA;
    empei_e_data_rate_enum_t     empei_e_data_rate;
    empei_e_datapath_type_enum_t empei_e_dp_sel    = EMPEI_E_TRANSPARENT_DATAPATH;
    empei_i_data_rate_enum_t     empei_i_data_rate;
    epmm_datapath_t              epmm_datapath     = EPMM_10GE_GSUP43_73;
    etime_data_rate_t            etime_data_rate;
    etrans_datapath_t            etrans_datapath   = ETRANS_GFP_GSUP43_C73_10GE_MAC;

    PMC_ENTRY();

    /* Convert datapaths to TSB type */
    emac_data_rate    = enet_convert_datapath_to_emac_data_rate      (datapath);
    emac_rx_data_src  = enet_convert_datapath_to_emac_rx_data_src    (datapath);
    emac_tx_data_dst  = enet_convert_datapath_to_emac_tx_data_dst    (datapath);
    empei_e_data_rate = enet_convert_datapath_to_empei_e_data_rate   (datapath);
    empei_e_dp_sel    = enet_convert_datapath_to_empei_e_datapath_sel(datapath);
    empei_i_data_rate = enet_convert_datapath_to_empei_i_data_rate   (datapath);
    epmm_datapath     = enet_convert_datapath_to_epmm_datapath       (datapath);
    etime_data_rate   = enet_convert_datapath_to_etime_data_rate     (datapath);
    etrans_datapath   = enet_convert_datapath_to_etrans_datapath     (datapath);

    switch (datapath)
    {
        /* Note: All of these are handled similarily */
    case ENET_SERDES_10GE_MAC  :
    case ENET_SERDES_40GE_MAC  :
    case ENET_SERDES_100GE_MAC :
    {
        emac_link_lowpwr_cfg(hndl->emac_handle, link, emac_data_rate, EMAC_DIR_BOTH, value);
        break;
    }

    /* Note: All handled in the same function */
    case ENET_SERDES_10GE_TRANSPARENT  :
    case ENET_SERDES_40GE_TRANSPARENT  :
    case ENET_SERDES_100GE_TRANSPARENT :
    {
        epmm_lowpwr(hndl->epmm_handle, link, epmm_datapath, value);
        break;
    }

    /* Note: All of these break at default */
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC                 :
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON            :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC            :
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON       :
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON        :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON        :
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC             :
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON        :
    case ENET_MAPOTN_GMP_G709_100GE_MAC                  :
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON             :
    case ENET_MAPOTN_GMP_G709_40GE_MAC                   :
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON              :
    case ENET_SLAVE_100GE_MAC_TX_PMON                    :
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON           :
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON            :
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT         :
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON :
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON  :
    case ENET_SLAVE_10GE_MAC_TX_PMON                     :
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON            :
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON             :
    case ENET_SLAVE_40GE_MAC_TX_PMON                     :
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON            :
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON             :

    default :
    {
        break;
    }
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* enet_tx_lane_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the TX_LANE_ID and TX_LANE_SKEW_FIFO_ADDR for
*   40GE/100GE datapath
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*
* OUTPUTS:
*   txLaneId            - array to store TX_LANE_ID, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*   txLaneSkewFifoAddr  - array to store TX_LANE_SKEW_FIFO_ADDR, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_tx_lane_info_get(enet_handle_t *hndl,
                                       UINT32 link,
                                       UINT8 txLaneId[20],
                                       UINT8 txLaneSkewFifoAddr[20])
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    etrans_datapath_t etrans_datapath;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
        /* check that datapath concerns EPMM or ETRANS 10G GSUP43*/
        if ((EPMM_40GE == epmm_datapath) || (EPMM_100GE == epmm_datapath))
        {
            rc = epmm_tx_lane_info_get(hndl->epmm_handle, link, epmm_datapath, txLaneId, txLaneSkewFifoAddr);
        }
        else
        {
            etrans_datapath = enet_convert_datapath_to_etrans_datapath(enet_datapath);
            if (ETRANS_UNUSED != etrans_datapath)
            {                
                rc = etrans_tx_lane_info_get(hndl->etrans_handle, link, etrans_datapath, txLaneId, txLaneSkewFifoAddr);
            }
        }
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_emac_lf_rf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAC to automatically insert remote faults and idles in the egress
*   direction on detection of local faults and remote fauls, respectively, on the ingress direction.
*   Configure MAC to permanently send local fault or remote fault.
*
* INPUTS:
*   hndl     - handle to the EMAC instance to be operated on
*   link     - link to configure
*   auto_fault_handling  - TRUE: Automatically insert remote faults/idles
*                         FALSE: Do not automatically insert remote faults/idles
*   send_lf  - TRUE: Permanently send local  fault sequences. auto_fault_handling must be set to FALSE
*   send_rf  - TRUE: Permanently send remote fault sequences. auto_fault_handling must be set to FALSE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_emac_lf_rf_cfg(enet_handle_t *hndl, UINT32 link, BOOL8 auto_fault_handling,
                                     BOOL8 send_lf, BOOL8 send_rf)
{
    BOOL8 halt_traffic_lf_rf;
    etrans_datapath_t  etrans_datapath;

    PMC_ENTRY();

    etrans_datapath = enet_convert_datapath_to_etrans_datapath(hndl->var.ch_datapath[link]);
    if (etrans_datapath != ETRANS_UNUSED)
    {
        halt_traffic_lf_rf = TRUE;
    }
    else
    {
        halt_traffic_lf_rf = FALSE;
    }

    emac_lf_rf_cfg(hndl->emac_handle, link, auto_fault_handling, send_lf, send_rf, halt_traffic_lf_rf);

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* enet_epmm_lf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EPMM to permanently send local faults in the ingress or egress direction
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*   enable    - TRUE: Permanently send local fault sequences
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_cfg(enet_handle_t *hndl, 
                                  UINT32 link, 
                                  enet_dir_t   dir,
                                  BOOL8 enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_lf_os_cfg(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }


    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_epmm_lf_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configurion of EPMM to permanently send local faults in the ingress or egress direction
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*
* OUTPUTS:
*   enable    - TRUE: Permanently send local fault sequences
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_cfg_get(enet_handle_t *hndl, 
                                  UINT32 link, 
                                  enet_dir_t   dir,
                                  BOOL8 *enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_lf_os_cfg_get(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }


    PMC_RETURN(rc);
}


/*******************************************************************************
* enet_epmm_lf_set_auto_insert_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable automatic insertion of local fault (LF) ordered sets
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*   enable    - TRUE:  Enable automatic insertion
*               FALSE: Disable automatic insertion
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_set_auto_insert_enable(enet_handle_t *hndl, 
                                                     UINT32 link, 
                                                     enet_dir_t   dir,
                                                     BOOL8 enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_lf_os_auto_insert_cfg(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }


    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_epmm_lf_set_auto_insert_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrives the enable status of automatic insertion of local fault (LF) ordered sets
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*
* OUTPUTS:
*   enable    - TRUE:  Enable automatic insertion
*               FALSE: Disable automatic insertion
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_set_auto_insert_enable_get(enet_handle_t *hndl, 
                                                     UINT32 link, 
                                                     enet_dir_t   dir,
                                                     BOOL8 *enable)
{

  PMC_ERROR rc = PMC_SUCCESS;
  enet_datapath_t enet_datapath;
  epmm_datapath_t epmm_datapath;
  epmm_link_direction_t epmm_dir;

  PMC_ENTRY();

  PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

  /* Check that link number is less than 12*/
  if (link >= ENET_NUM_LINK_MAX) {
    rc = ENET_LOG_ERR_INVALID_PARAMETERS;
  }

  if (PMC_SUCCESS == rc)
    {
      enet_datapath = hndl->var.ch_datapath[link];
      epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
      epmm_dir = enet_convert_dir_to_epmm_dir(dir);
      rc = epmm_lf_os_auto_insert_cfg_get(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }


  PMC_RETURN(rc);
}


/*******************************************************************************
* enet_epmm_lf_is_force_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Reads the OS_FORCE_EN bit and returns the result from EPMM.
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress. Only egress is valid.
*
* OUTPUTS:
*   force_enable  - Set to the value of the register.
*
* RETURNS:
*   PMC_SUCCESS - Parameters are good and register was set.
*   PMC_ERROR   - Otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_is_force_enable(enet_handle_t *hndl, 
                                              UINT32 link, 
                                              enet_dir_t dir,
                                              BOOL8 *force_enable)
{

    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) 
    {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }
    else if (dir != ENET_EGRESS)
    {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        epmm_lf_is_force_enable(hndl->epmm_handle, link, force_enable);
    }

    PMC_RETURN(rc);
} /* enet_epm_is_force_enable */

/*******************************************************************************
* enet_epmm_lf_set_force_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sets the OS_FORCE_EN in the egress direction for the specified linnk.
*
* INPUTS:
*  hndl      - Handle to the EPMM instance to be operated on
*  link      - Link to configure
*  dir       - Direction of the connection. Only EGRESS is valid
*  enable    - Whether to set it 1 or 0
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS - Parameters are valid and register set
*   PMC_ERROR   - Otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_lf_set_force_enable(enet_handle_t *hndl, 
                                               UINT32 link, 
                                               enet_dir_t dir,
                                               BOOL8 enable)
{

    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) 
    {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }
    else if (dir != ENET_EGRESS)
    {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        epmm_lf_set_force_enable(hndl->epmm_handle, link, enable);
    }

    PMC_RETURN(rc);
} /* enet_epm_set_force_enable */


/*******************************************************************************
* enet_epmm_rf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EPMM to permanently send remote faults in the ingress or egress direction
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*   enable    - TRUE: Permanently send remote fault sequences
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_rf_cfg(enet_handle_t *hndl, 
                                  UINT32         link, 
                                  enet_dir_t     dir,
                                  BOOL8           enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_rf_os_cfg(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_epmm_rf_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration of EPMM to permanently send remote faults in the ingress or egress direction
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*
* OUTPUTS:
*   enable    - TRUE: Permanently send remote fault sequences
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_rf_cfg_get(enet_handle_t *hndl, 
                                      UINT32         link, 
                                      enet_dir_t     dir,
                                      BOOL8           *enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_rf_os_cfg_get(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
* enet_epmm_rf_set_auto_insert_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable automatic insertion of remote fault (RF) ordered sets
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   dir       - Path direction: ingress or egress
*   enable    - TRUE:  Enable automatic insertion
*               FALSE: Disable automatic insertion
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  1 - Ingress CBR 10G path
*                  2 - Egress  CBR 10G path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_rf_set_auto_insert_enable(enet_handle_t *hndl, 
                                                     UINT32         link, 
                                                     enet_dir_t     dir,
                                                     BOOL8           enable)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    epmm_link_direction_t epmm_dir;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        epmm_dir = enet_convert_dir_to_epmm_dir(dir);
        rc = epmm_rf_os_auto_insert_cfg(hndl->epmm_handle, link, epmm_datapath, epmm_dir, enable);
    }

    PMC_RETURN(rc);
}
/*******************************************************************************
* enet_epmm_los_to_rf_consq_action_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EPMM to set RF ordered set on the 10GE egress direction when RX_LOS is asserted
*   on a 10GE port.
*   RX_LOS->Tx_RF for 10GE port in Gsup7.3 GFP mapping mode. 
*
* INPUTS:
*   hndl      - Handle to the EPMM instance to be operated on
*   link      - Link to configure
*   enable    - TRUE: enable consequential action, FALSE: disable consequential action
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_los_to_rf_consq_action_cfg(enet_handle_t *hndl, 
                                  UINT32 link, 
                                  BOOL8 enable)
{

    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        epmm_los_force_rf_action_set(hndl->epmm_handle, 
                                     link,
                                     enable);

    }
    PMC_RETURN(rc);

} /* enet_epmm_los_to_rf_consq_action_cfg */

/*******************************************************************************
* enet_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - Handle to the EPMM instance to be operated on
*   link                - Link to configure
*   pkt_upi_val         - the value that identifies the transmit data as a 10GE data frame.
*   os_upi_val          - the value that identifies the transmit data as 10GE ordered set.
*   enable_drop         - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either data_pkt_upi_val or os_pkt_upi_val
*                         are dropped.
*                         when FALSE, incoming data packet will be treated as a 
*                         data packet.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  epmm_path may be set to one of the following:
*                  0 - Ingress GSup43 path
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_tx_upi_set(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8          pkt_upi_val,
                                 UINT8          os_upi_val,
                                 BOOL8          enable_drop)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    etrans_datapath_t etrans_datapath;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
        /* check that datapath concerns EPMM or ETRANS 10G GSUP43*/
        if (EPMM_10GE_GSUP43_73 == epmm_datapath)
        {
            rc = epmm_tx_upi_set(hndl->epmm_handle, link, 
                                 epmm_datapath, 
                                 pkt_upi_val,
                                 os_upi_val,
                                 enable_drop);
        }
        else
        {
            etrans_datapath = enet_convert_datapath_to_etrans_datapath(enet_datapath);
            if (ETRANS_UNUSED != etrans_datapath)
            {                
                rc = etrans_tx_upi_set(hndl->etrans_handle, link, 
                                 etrans_datapath, 
                                 pkt_upi_val,
                                 os_upi_val,
                                 enable_drop);
            }
        }
    }

    PMC_RETURN(rc);
}/* enet_tx_upi_set */

/*******************************************************************************
* enet_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves expected UPI values in data or ordered set packet and the action 
*   to be done when incoming packet UPI does not match expected values: DROP or IGNORE.
*
* INPUTS:
*   hndl                - Handle to the EPMM instance to be operated on
*   link                - Link to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - the value that identifies the transmit data as a 10GE data frame.
*  *os_upi_val_ptr      - the value that identifies the transmit data as 10GE ordered set.
*  *enable_drop_ptr     - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either pkt_upi_val or os_upi_val
*                         are dropped.
*                         when FALSE, incoming data packet will be treated as a 
*                         data packet.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_tx_upi_get(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8         *pkt_upi_val_ptr,
                                 UINT8         *os_upi_val_ptr,
                                 BOOL8         *enable_drop_ptr)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    etrans_datapath_t etrans_datapath;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        enet_datapath = hndl->var.ch_datapath[link];
        epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
        /* check that datapath concerns EPMM or ETRANS 10G GSUP43*/
        if (EPMM_10GE_GSUP43_73 == epmm_datapath)
        {
            rc = epmm_tx_upi_get(hndl->epmm_handle, link, 
                                 epmm_datapath, 
                                 pkt_upi_val_ptr,
                                 os_upi_val_ptr,
                                 enable_drop_ptr);
        }
        else
        {
            etrans_datapath = enet_convert_datapath_to_etrans_datapath(enet_datapath);
            if (ETRANS_UNUSED != etrans_datapath)
            {                
                rc = etrans_tx_upi_get(hndl->etrans_handle, link, 
                                       etrans_datapath, 
                                       pkt_upi_val_ptr,
                                       os_upi_val_ptr,
                                       enable_drop_ptr);
            }
        }
    }

    PMC_RETURN(rc);
}/* enet_tx_upi_get */

/*******************************************************************************
* enet_rx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the UPI values for data and ordered set frames for
*   GSUP43 7.3 mode. The UPI values must match the values configured in MAPOTN 
*   (see digi_mapper_orderred_set_upi_cfg). 
*
* INPUTS:
*   hndl                - ENET handle
*   link                - Link to configure
*   pkt_upi_val         - Data frame UPI
*   os_upi_val          - Ordered Set UPI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_rx_upi_set(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8          pkt_upi_val,
                                 UINT8          os_upi_val)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < ENET_NUM_LINK_MAX, ENET_LOG_ERR_INVALID_PARAMETERS,0,0);

    enet_datapath = hndl->var.ch_datapath[link];
    epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);

    /* check if the datapath uses EPMM */


    if (EPMM_10GE_GSUP43_73 == epmm_datapath)
    {
        rc = epmm_rx_upi_set(hndl->epmm_handle, link, 
                             epmm_datapath, 
                             pkt_upi_val,
                             os_upi_val);
    }
    else
    {
        rc = ENET_ERR_INVALID_DATAPATH;
    }

    PMC_RETURN(rc);
}/* enet_rx_upi_set */

/*******************************************************************************
* enet_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configured UPI values for data and ordered set frames.
*
* INPUTS:
*   hndl                - ENET handle
*   link                - Link to configure
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - Data frame UPI
*  *os_upi_val_ptr      - Ordered Set UPI
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_rx_upi_get(enet_handle_t *hndl, 
                                 UINT32 link, 
                                 UINT8  *pkt_upi_val_ptr,
                                 UINT8  *os_upi_val_ptr)
{

    PMC_ERROR rc = PMC_SUCCESS;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < ENET_NUM_LINK_MAX, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    enet_datapath = hndl->var.ch_datapath[link];
    epmm_datapath = enet_convert_datapath_to_epmm_datapath(enet_datapath);

    /* check if the datapath uses EPMM */
    if (EPMM_10GE_GSUP43_73 == epmm_datapath)
    {
        rc = epmm_rx_upi_get(hndl->epmm_handle, link, 
                             epmm_datapath, 
                             pkt_upi_val_ptr,
                             os_upi_val_ptr);
    }
    else
    {
        rc = ENET_ERR_INVALID_DATAPATH;
    }

    PMC_RETURN(rc);
}/* enet_rx_upi_get */

/*******************************************************************************
* enet_eclass_pkt_extract_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures eclass pkt extraction properties.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   enet_datapath         - INGRESS or EGRESS datapath
*   enable                - enable/disable packet extraction
*   extract_cfg_ptr       - pointer to extraction configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_eclass_pkt_extract_cfg(enet_handle_t              *hndl, 
                                             UINT32                      link, 
                                             enet_dir_t                  enet_datapath,
                                             BOOL8                       enable,
                                             eclass_pkt_extract_cfg_t   *extract_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < ENET_NUM_LINK_MAX, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch (enet_datapath)
    {
    case ENET_INGRESS:
        rc = eclass_pkt_extract_cfg(hndl->empei_i_eclass_handle,
                                    link,
                                    enable,
                                    extract_cfg_ptr);    
        if (PMC_SUCCESS == rc)
        {
            hndl->var.lnk_ctxt[link].empei_i_eclass_en = enable;
        }
        break;
    case ENET_EGRESS:
        rc = eclass_pkt_extract_cfg(hndl->empei_e_eclass_handle,
                                    link,
                                    enable,
                                    extract_cfg_ptr);   
        if (PMC_SUCCESS == rc)
        {
            hndl->var.lnk_ctxt[link].empei_e_eclass_en = enable;
        }
        break;
    default:
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/* enet_eclass_pkt_extract_cfg */

/*******************************************************************************
* enet_eclass_pkt_extract_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans eclass packet extract configuration
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   enet_datapath         - INGRESS or EGRESS datapath.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_eclass_pkt_extract_cleanup(enet_handle_t              *hndl, 
                                                 UINT32                      link, 
                                                 enet_dir_t                  enet_datapath)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(link < ENET_NUM_LINK_MAX, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch (enet_datapath)
    {
    case ENET_INGRESS:
        eclass_link_pkt_extract_cleanup(hndl->empei_i_eclass_handle,
                                        link);    
        hndl->var.lnk_ctxt[link].empei_i_eclass_en = FALSE;
        break;
    case ENET_EGRESS:
        eclass_link_pkt_extract_cleanup(hndl->empei_e_eclass_handle,
                                        link);   
        hndl->var.lnk_ctxt[link].empei_e_eclass_en = FALSE;
        break;
    default:
        rc = ENET_LOG_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/* enet_eclass_pkt_extract_cleanup */

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* Run-time Operations
*******************************************************************************/

/*******************************************************************************
*  enet_mpmo_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines if the MPMO configuration is required for a specific channel.
*
* INPUTS:
*   *hndl             - pointer to ENET handle instance to be operated on
*   link              - channel ID to be configured
*                      Valid range: 0 - 11    for enet this is 'link' not 'chnl' and the range is 0 to 11.
*
* OUTPUTS:
*   mpmo_cfg_req      - MPMO configuration required or not
*                       TRUE  : MPMO configuration required
*                       FALSE : MPMO configuration not required
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_mpmo_cfg_get(enet_handle_t *hndl,
                                   UINT32 link,
                                   BOOL8 *mpmo_cfg_req)
{   
    /* Variable declaration */
    enet_datapath_t enet_datapath;
    PMC_ENTRY();
    
    enet_datapath = hndl->var.ch_datapath[link];
    
    if(enet_datapath == ENET_SERDES_100GE_TRANSPARENT
       || enet_datapath == ENET_SERDES_40GE_TRANSPARENT
       || enet_datapath == ENET_SERDES_10GE_TRANSPARENT
       || enet_datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON
       || enet_datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON
       || enet_datapath == ENET_SERDES_10GE_TRANSPARENT_RX_PMON
       || enet_datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON
       || enet_datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON
       || enet_datapath == ENET_SLAVE_10GE_TRANSPARENT_TX_PMON)
    {
        *mpmo_cfg_req = TRUE;    
    }
    else {
        *mpmo_cfg_req = FALSE; 
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* enet_mpmo_cfg_get */

/*******************************************************************************
*  enet_mpma_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMA packet size and packet period information for a channel.
*
* INPUTS:
*   *hndl               - handle to the ENET instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*   epsilon_ptr         - a pointer to the epsilon value.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_mpma_data_get(enet_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr,
                                    DOUBLE *epsilon_ptr)
{   
    /* Variable declaration */
    UINT32 ps_ptr; 
    UINT32 pp_int_ptr;  
    UINT32 pp_n_ptr;    
    UINT32 pp_d_ptr;  
    DOUBLE eps_ptr;  
    PMC_ERROR rc;
    
    PMC_ENTRY();
    
    rc = epmm_mpma_data_get(hndl->epmm_handle, link, 
                            &ps_ptr,
                            &pp_int_ptr,
                            &pp_n_ptr,
                            &pp_d_ptr,&eps_ptr);
                                      
    if (rc != PMC_SUCCESS) {
        PMC_RETURN(rc);
    } 
    
    *pkt_size_ptr = ps_ptr; 
    *pkt_per_int_ptr = pp_int_ptr;                         
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr;
    *epsilon_ptr = eps_ptr;
    
    PMC_LOG_TRACE("ps_ptr=%d  pp_int_ptr=%d  pp_n_ptr=%d  pp_d_ptr=%d  eps_ptr=%f\n", ps_ptr,pp_int_ptr,pp_n_ptr,pp_d_ptr,eps_ptr);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* enet_mpma_data_get */

/*******************************************************************************
*  enet_mpmo_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMO packet size and packet period information for a channel.
*
* INPUTS:
*   *hndl               - handle to the ENET instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   pkt_size_ptr        - pointer to configured packet size
*   pkt_per_int_ptr     - pointer to configured integer portion of packet period
*   pkt_per_n_ptr       - pointer to configured numerator of packet period 
*                         fraction
*   pkt_per_d_ptr       - pointer to configured denominator of packet period 
*                         fraction
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_mpmo_data_get(enet_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr)
{   
    /* Variable declaration */
    UINT32 ps_ptr; 
    UINT32 pp_int_ptr;  
    UINT32 pp_n_ptr;    
    UINT32 pp_d_ptr;  
    PMC_ERROR ret_val;
    
    PMC_ENTRY();
    
    ret_val = epmm_mpmo_data_get(hndl->epmm_handle, link, 
                                 &ps_ptr,
                                 &pp_int_ptr,
                                 &pp_n_ptr,
                                 &pp_d_ptr);
                                      
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    } 
    
    *pkt_size_ptr = ps_ptr; 
    *pkt_per_int_ptr = pp_int_ptr;                         
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr;
    
    PMC_LOG_TRACE("ps_ptr=%d  pp_int_ptr=%d  pp_n_ptr=%d  pp_d_ptr=%d \n", ps_ptr,pp_int_ptr,pp_n_ptr,pp_d_ptr);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* enet_mpmo_data_get */

/*******************************************************************************
*  enet_mpmo_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configures MPMO channel.
*
* INPUTS:
*   *hndl               - pointer to ENET handle instance to be operated on
*   link                - channel ID to be configured
*                         Valid range: 0 - 1
*   pkt_size            - packet size configured at the packet maker
*   pkt_per_int         - integer portion for packet period configured at the 
*                         packet maker
*   pkt_per_n           - numerator for packet period fraction configured at the 
*                         packet maker
*   pkt_per_d           - denominator for packet period fraction configured at  
*                         the packet maker
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_mpmo_prov(enet_handle_t *hndl,
                                UINT32 link,
                                UINT32 pkt_size,
                                UINT32 pkt_per_int,
                                UINT32 pkt_per_n,
                                UINT32 pkt_per_d)
{   
    /* Variable declaration */
    PMC_ERROR rc;
    DOUBLE server_rate = 0;
    enet_datapath_t enet_datapath;
    epmm_datapath_t epmm_datapath;
    
    PMC_ENTRY();
        
    enet_datapath = hndl->var.ch_datapath[link];
    epmm_datapath = enet_convert_datapath_to_epmm_datapath( enet_datapath );
    
    switch(epmm_datapath)
    {
    case EPMM_10GE_GSUP43_73:
        server_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        break;
    case EPMM_10GE_CBR:
    case EPMM_10GE_CBR_GSUP43_73_HI_BER_MON:
        server_rate = UTIL_GLOBAL_ENET_10GE_RATE;
        break;
    case EPMM_40GE:
        server_rate = UTIL_GLOBAL_ENET_40GE_RATE;
        break;
    case EPMM_100GE:
        server_rate = UTIL_GLOBAL_ENET_100GE_RATE;
        break;
    default:
        PMC_ASSERT( FALSE , EPMM_INVALID_DATAPATH, 0, 0);
    }
    
    /*program MPMO */
    rc = epmm_mpmo_ch_prov(hndl->epmm_handle, /* handle */
                           link,              /* chnl */
                           server_rate,       /* client_bitrate */
                           pkt_size,          /* pkt_size */
                           pkt_per_int,       /* pkt_per_int */
                           pkt_per_n,         /* pkt_per_n_ptr */
                           pkt_per_d,         /* pkt_per_d_ptr */
                           UTIL_OPSA_T_FRAME_UNUSED,   /* cn_frame_period */
                           FALSE);            /* golden_cn_en */    
   

    if (rc != PMC_SUCCESS) {
        PMC_RETURN(rc);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* enet_mpmo_prov */  


/*******************************************************************************
*  enet_mpmo_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Deprovisions MPMO channel.
*
* INPUTS:
*   *hndl               - pointer to ENET handle instance to be operated on
*   link                - channel ID to be configured
*                         Valid range: 0 - 1
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_mpmo_deprov(enet_handle_t *hndl,
                                  UINT32 link)
{   
    /* Variable declaration */
    PMC_ERROR rc;
    
    PMC_ENTRY();
       
    /*deprov MPMO */
    rc = epmm_mpmo_ch_deprov(hndl->epmm_handle, /* handle */
                             link);             /* chnl */
                                   
   

    if (rc != PMC_SUCCESS) {
        PMC_RETURN(rc);
    }

    PMC_RETURN(PMC_SUCCESS);
    
} /* enet_mpmo_deprov */               

/*******************************************************************************
*  enet_max_frame_length_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the maximum frame length on the specified
*   link in bytes.
*   The maximum value supported is 16353 bytes.
*   Typical settings are 1518, but initialized to 1536 on SS
*   initialization.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*   frame_len      - maximum frame length supported.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_max_frame_length_set(enet_handle_t *hndl,
                                           UINT32 link,
                                           UINT32 frame_len)
{
    PMC_ERROR rc = ENET_ERR_NO_SW_CONTENT;

    PMC_ENTRY();

    PMC_RETURN(rc);
} /* enet_max_frame_length_set */

/*******************************************************************************
*  enet_max_frame_length_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the maximum frame length that was set on
*   the specified link in bytes.
*   The maximum value supported is 16353 bytes.
*   Typical settings are 1518, but initialized to 1536 on SS
*   initialization.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*
* OUTPUTS:
*   p_frame_len    - maximum frame length supported.
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_max_frame_length_get(enet_handle_t *hndl,
                                           UINT32 link,
                                           UINT32 *p_frame_len)
{
    PMC_ERROR rc = ENET_ERR_NO_SW_CONTENT;

    PMC_ENTRY();

    PMC_RETURN(rc);
} /* enet_max_frame_length_get */

/*******************************************************************************
*  enet_flow_control_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the flow control on the
*   specified link.
*   Datapaths sent through the EPMM (_TRANSPARENT datapaths), do not
*   have flow control and cannot be enabled.
*
* INPUTS:
*   hndl                   - handle to the enet instance to be operated on
*   link                   - link to configure
*   dir                    - flow direction: RX or TX
*   enabled                - TRUE to enable flow control on this link.
*                            FALSE to disable flow control on this link.
*   pause_quanta           - pause quanta for CPB flow control (Ignored when dir = UTIL_GLOBAL_DIR_RX)
*   pause_quanta_thresh    - pause quanta threshold for CPB flow control
*                            It shall be superior or equal to 2. (Ignored when dir = UTIL_GLOBAL_DIR_RX)  
*   cpb_sys_zone_thresh    - CPB ZONE Threshold used to start dropping frames and
*                            sendind XON pause frames (Ignored when dir = UTIL_GLOBAL_DIR_RX)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*             There will be orthoganal APIs to configure the flow
*             control configuration (quantums, thresholds, etc).
*             These are undefined at this time.
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_flow_control_enable(enet_handle_t             *hndl,
                                          UINT32                     link,
                                          util_global_direction_t    dir,
                                          BOOL8                      enabled,
                                          UINT32                     pause_quanta,
                                          UINT32                     pause_quanta_thresh,
                                          UINT32                     cpb_sys_zone_thresh)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);

    switch (dir)
    {
    case UTIL_GLOBAL_DIR_TX:
        if (TRUE == enabled && 2 > pause_quanta_thresh)
        {
            PMC_RETURN(ENET_LOG_ERR_PAUSA_QUANTA_THRESH_LT_TWO);
        }
        
        if (TRUE == enabled)
        {
            /* pause quanta for priority 0 (pause threshold is also set to 0) */
            rc = emac_pause_quanta_cfg(hndl->emac_handle, 
                                       link,
                                       0, 
                                       pause_quanta, 
                                       pause_quanta_thresh);
            
            if (PMC_SUCCESS == rc )
            {            
                rc = empei_i_enable_cpb_flow_ctrl_cfg(hndl->empei_i_handle, 
                                                      link,
                                                      cpb_sys_zone_thresh);
            }

        }

        if (PMC_SUCCESS == rc )
        { 
            empei_e_data_cpb_flow_ctrl_link_cfg(hndl->empei_e_handle, link, enabled);
        }

        if (PMC_SUCCESS == rc )
        {
            rc = empei_i_cpb_flow_ctrl_enable(hndl->empei_i_handle, 
                                              link, 
                                              enabled);
        }      
        
        break;
    case UTIL_GLOBAL_DIR_RX:

        /* Rx flow control */       
        /* configure RX flow control in emac */
        if (PMC_SUCCESS == rc )
        { 
            empei_e_data_cpb_flow_ctrl_link_cfg(hndl->empei_e_handle, link, enabled);
        }
        
        rc = emac_flow_ctrl_mode_cfg(hndl->emac_handle, link,
                                     EMAC_LEGACY_PAUSE, 
                                     TRUE, 
                                     FALSE, 
                                     (enabled == TRUE?FALSE:TRUE));

        if (FALSE == enabled)
        {
            /* unset  check that TX flow flo*/
        }
        break;
    default:
        break;
    }

    PMC_RETURN(rc);
} /* enet_flow_control_enable */

/*******************************************************************************
*  enet_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns flow control configuration
*
* INPUTS:
*   hndl                   - handle to the enet instance to be operated on
*   link                   - link to configure
*   dir                    - flow direction: RX or TX
*
* OUTPUTS:
*   *enabled                - TRUE to enable flow control on this link.
*                             FALSE to disable flow control on this link.
*   *pause_quanta           - pause quanta for CPB flow control
*   *pause_quanta_thresh    - pause quanta threshold for CPB flow control
*                             It shall be superior or equal to 2.  
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_flow_control_get(enet_handle_t             *hndl, 
                                       UINT32                     link, 
                                       util_global_direction_t    dir,
                                       BOOL8                     *enabled,
                                       UINT32                    *pause_quanta,
                                       UINT32                    *pause_quanta_thresh)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
     
    switch (dir)
    {
    case UTIL_GLOBAL_DIR_TX:
        if (PMC_SUCCESS == rc )
        {
            *enabled = empei_i_cpb_flow_ctrl_enable_get(hndl->empei_i_handle, link);
        }           
        if (TRUE == *enabled)
        {
            /* pause quanta for priority 0 */
            rc = emac_pause_quanta_get(hndl->emac_handle, link,
                                       0, 
                                       pause_quanta, 
                                       pause_quanta_thresh);                  
        }
        break;
    case UTIL_GLOBAL_DIR_RX:        
        /* Rx flow control */
        rc = emac_flow_ctrl_mode_get(hndl->emac_handle,
                                     link,
                                     enabled);
        break;
    default:
        break;
    }
    
    PMC_RETURN(rc);
}

/*******************************************************************************
*  enet_emac_pause_frame_fwd_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables pause frame forwarding on the
*   specified link.
*   Datapaths sent through the EPMM (_TRANSPARENT datapaths), do not
*   have flow control and cannot be enabled.
*
* INPUTS:
*   hndl                   - handle to the enet instance to be operated on
*   link                   - link to configure
*   enabled                - TRUE to enable pause frame forwarding on this link.
*                            FALSE to disable pause_frame forwarding on this link.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*  None.
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_emac_pause_frame_fwd_enable(enet_handle_t             *hndl,
                                                  UINT32                     link,
                                                  BOOL8                      enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    
    emac_pause_frame_fwd_cfg(hndl->emac_handle, 
                             link, 
                             enabled);

    PMC_RETURN(rc);
} /* enet_emac_pause_frame_fwd_enable */


/*******************************************************************************
*  enet_emac_pause_frame_fwd_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves enable status of pause frame forwarding on the
*   specified link.
*   Datapaths sent through the EPMM (_TRANSPARENT datapaths), do not
*   have flow control and cannot be enabled.
*
* INPUTS:
*   hndl                   - handle to the enet instance to be operated on
*   link                   - link to configure
*
* OUTPUTS:
*   *enabled_ptr           - TRUE to enable pause frame forwarding on this link.
*                            FALSE to disable pause_frame forwarding on this link.

* RETURNS:
*   None.
*
* NOTES:
*  None.
*
*******************************************************************************/
PUBLIC void enet_emac_pause_frame_fwd_get(enet_handle_t             *hndl,
                                          UINT32                     link,
                                          BOOL8                     *enabled_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
    
    emac_pause_frame_fwd_get(hndl->emac_handle, 
                             link, 
                             enabled_ptr);
    
    PMC_RETURN();
} /* enet_emac_pause_frame_fwd_get */

/*******************************************************************************
*  enet_phy_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the status of the PHY layer on the
*   specified link.
*   Datapaths sent through the EPMM (_TRANSPARENT datapaths), do not
*   have PHY and therefore their statuses may not be obtained.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*
* OUTPUTS:
*   p_is_enabled   - TRUE PHY is enabled on this link.
*                  - FALSE PHY is disabled on this link.
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*             There may be orthoganal APIs to obtain configurion information
*             with respect to PHY. These are undefined at this time.
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_phy_status_get(enet_handle_t *hndl,
                                     UINT32 link,
                                     BOOL8 *p_is_enabled)
{
    PMC_ERROR rc = ENET_ERR_NO_SW_CONTENT;

    PMC_ENTRY();

    PMC_RETURN(rc);
} /* enet_phy_status_get */

/*******************************************************************************
*  enet_send_test_frame_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the test frames on the
*   specified link.
*   Datapaths sent through the EPMM (_TRANSPARENT datapaths), do not
*   have test frames and cannot be enabled.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*   enabled        - TRUE to enable test frames on this link.
*                  - FALSE to disable test frames on this link.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*             There will be orthoganal APIs to configure the test frame
*             configuration (packet size, payload patern, packet interval,
*             DA, SA, Outer VLAN, Inner VLAN and ETH Type/Length, etc).
*             These are undefined at this time.
*
*             Only one byte pattern will be repeated throughout the payload of
*             the packet.
*             Packet interval ensures that the number of scheduling
*             opportunities between two consecutive SOPs are at least the packet
*             interval.
*             If VLANs are not required, configure upper 16-bits of outer
*             VLAN with Type/Length information, while padding the reset with
*             the PAYLOAD pattern.
*             Only one link can be tested at any one time.

*
*******************************************************************************/
PUBLIC PMC_ERROR enet_send_test_frame_enable(enet_handle_t *hndl,
                                             UINT32 link,
                                             BOOL8 enabled)
{
    PMC_ERROR rc = ENET_ERR_NO_SW_CONTENT;

    PMC_ENTRY();

    PMC_RETURN(rc);
} /* enet_send_test_frame_enable */


/*******************************************************************************
*  enet_interrupt_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the interrupts on 
*   specified link.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*   enabled        - TRUE to enable interrupt on this link
*                  - FALSE to disable interrupt on this link.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_interrupt_enable(enet_handle_t *hndl,
                                       UINT32 link,
                                       BOOL8 enabled)
{
  
    PMC_ENTRY();

/* adding the Interrupt Controller masking - for Enable the Interrupt checker IRQ*/
/* there are 4 masks exist for each Int register, one per port, so masking can actually set different for every port.*/
    if( enabled ) {
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x0, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x1, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x2, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x3, 0xffffffff );
      
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x0, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x1, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x2, 0xffffffff );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x3, 0xffffffff );



    } else {
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x0, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x1, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x2, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, hndl,0x3, 0x0 );
      
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x0, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x1, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x2, 0x0 );
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, hndl,0x3, 0x0 );

    }

    PMC_RETURN(PMC_SUCCESS);
} /* enet_interrupt_enable */


/*******************************************************************************
*  enet_extract_pakcet
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables extraction of particular packet type on
*   specified link.
*
* INPUTS:
*   hndl           - handle to the enet instance to be operated on
*   link           - link to configure
*   direction      - ENET_EGRESS  : configure EMPEI_EGRESS_ECLASS to extract packet
*                  - ENET_INGRESS : configure EMPEI_INGRESS_ECLASS to extract packet
*   extract_pkt_type - ENET_EXTRACT_NO_PKTS : configure ECLASS to not extract packets
*                    - ENET_EXTRACT_ALL_PKTS : configure ECLASS to extract all packets
*                    - ENET_EXTRACT_PTP_PKTS : configure ECLASS to extract PTP packets
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS or other on failure
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_extract_packet(enet_handle_t *hndl,
                                     UINT32 link,
                                     enet_dir_t direction,
                                     enet_extract_pkt_t extract_pkt_type)
{

    eclass_extract_pkt_t pkt_type;

    PMC_ENTRY();

    switch( extract_pkt_type ) 
    {
    case ENET_EXTRACT_NO_PKTS : {
        pkt_type = ECLASS_EXTRACT_NO_PKTS;
        break;
    }
    case ENET_EXTRACT_ALL_PKTS_CPU : {
        pkt_type = ECLASS_EXTRACT_ALL_PKTS_CPU;
        break;
    }
    case ENET_EXTRACT_ALL_PKTS_MGMT : {
        pkt_type = ECLASS_EXTRACT_ALL_PKTS_MGMT;
        break;
    }
    case ENET_EXTRACT_PTP_PKTS : {
        pkt_type = ECLASS_EXTRACT_PTP_PKTS;
        break;
    }
    default : {
        pkt_type = ECLASS_EXTRACT_NO_PKTS;
        break;
    }
    }


    if( direction == ENET_INGRESS ) {      
        eclass_extract_packet( hndl->empei_i_handle->eclass_handle, link, pkt_type );
        hndl->var.lnk_ctxt[link].empei_i_eclass_en = TRUE;

    } else {
        eclass_extract_packet( hndl->empei_e_handle->eclass_handle, link, pkt_type );
        hndl->var.lnk_ctxt[link].empei_e_eclass_en = TRUE;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* enet_interrupt_enable */



/*******************************************************************************
* enet_pkt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure packet generator and enable it
*
*
* INPUTS:
*   *hndl               - pointer to enet handle instance
*   link                - link on which packet will be generated
*   enable              - Enable packet generation
*   pkt_size            - packet size in bytes
*   pyld_val            - repeating byte value in the payload
*   pkt_interval        - defines the Packet SOP to Packet SOP interval
*   da_hi               - Ethernet DA value (Upper 16-bits)
*   da_low              - Ethernet DA value (Lower 32-bits)
*   sa_hi               - Ethernet SA value (Upper 16-bits)
*   sa_low              - Ethernet SA value (Lower 32-bits)
*   len_etype           - Ethernet Length/Ethertype field (16-bits)
*   vlan_out            - Outer VLAN tag
*   vlan_in             - Inner VLAN tag
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
PUBLIC PMC_ERROR enet_pkt_gen_cfg(enet_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8 enable,
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
    PMC_ENTRY();

    if( enable == TRUE ) {
        empei_e_pkt_gen_cfg( hndl->empei_e_handle,
                             pkt_size,
                             pyld_val,
                             pkt_interval,
                             da_hi,
                             da_low,
                             sa_hi,
                             sa_low,
                             len_etype,
                             vlan_out,
                             vlan_in);

        empei_e_pkt_gen_enable( hndl->empei_e_handle,
                                link,
                                enable );
    } else {
        empei_e_pkt_gen_enable( hndl->empei_e_handle,
                                link,
                                enable );

    }
      
    

    PMC_RETURN(PMC_SUCCESS);
} /* enet_pkt_gen_cfg */

/*******************************************************************************
* enet_convert_link_to_datapath_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retuns the datapath that was configured on the input link
*
* INPUTS:
*   *hndl               - pointer to enet handle instance
*   link                - channel ID to be configured Valid range: 0 to 11
*
* OUTPUTS:
*   *datapath           - datapath configured on the link to return to caller
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_convert_link_to_datapath_type(enet_handle_t *hndl, 
                                               UINT32 link, 
                                               enet_datapath_t* datapath) 
{
    PMC_ENTRY();

    PMC_ASSERT((hndl != NULL), ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((link < ENET_NUM_LINK_MAX), ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
    PMC_ASSERT((datapath != NULL), ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);

    *datapath =  hndl->var.ch_datapath[link];

    PMC_RETURN();
} /* enet_convert_link_to_datapath_type */


/*******************************************************************************
*  enet_epmm_mpmo_ckctl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure EPMM MPMO clock control ports in ENET.
*
* INPUTS:
*   *hndl             - pointer to enet handle instance
*   ckctl_port        - clock control port number (0-25): \n
*                       Ports 0--11 are line side 10G SERDES. \n
*                       Ports 12--13 are line side 2.5G SERDES. \n
*                       Ports 14--25 are system side 10G SERDES. \n
*   chnl              - channel to map to the port specified by ckctl_port.
*   enable            - enables/disables CKCTL output
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   CBRC_ERR_NULL_HANDLE
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_epmm_mpmo_ckctl_cfg(enet_handle_t *hndl,
                                          UINT32 ckctl_port, 
                                          UINT32 chnl,
                                          BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT((hndl != NULL), ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    rc = epmm_mpmo_ckctl_cfg(hndl->epmm_handle, ckctl_port, chnl, enable);

    PMC_RETURN(rc);

} /* enet_epmm_mpmo_ckctl_cfg */



/*******************************************************************************
* FUNCTION: enet_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   enet_handle       - Pointer to subsystem handle
*   link              - Link
*   non_null_ptr      - Pointer to be checked that should not be null
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_int_validate(enet_handle_t    *enet_handle,
                                   UINT32            link,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != enet_handle, ENET_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (link >= ENET_NUM_LINK_MAX)
    {
        result = ENET_LOG_ERR_INVALID_ARG;
    }

    if (NULL == non_null_ptr)
    {
        result = ENET_LOG_ERR_NULL_PARAMETERS;
    }


    PMC_RETURN(result);
} /* enet_int_validate */


/*******************************************************************************
* FUNCTION: enet_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.\n\n
*
*   For ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON, the interrupt
*   int_table_ptr->epmm.gsup43_c73.hi_ber_i is not available, it must be
*   accessed via int_table_ptr->emac.hi_ber_i.
*   
*
* INPUTS:
*   enet_handle       - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_int_chnl_enable(enet_handle_t    *enet_handle,
                                      UINT32            link,
                                      enet_int_chnl_t  *int_table_ptr,
                                      enet_int_chnl_t  *int_en_table_ptr,
                                      BOOL8             enable,
                                      BOOL8             is_multichannels)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           emac_hi_ber_i = FALSE;
    BOOL8           emac_hi_ber_v = FALSE;

    BOOL8           update_emac_from_epmm = FALSE;
    BOOL8           emac_rx_loc_fault_i = FALSE;

    PMC_ENTRY();

    result = enet_int_validate(enet_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        update_emac_from_epmm = enet_should_emac_use_epmm_int_mon(enet_handle->var.ch_datapath[link]);
        if (TRUE == update_emac_from_epmm)
        {
            emac_hi_ber_i = int_table_ptr->emac.hi_ber_i;
            emac_hi_ber_v = int_table_ptr->emac.hi_ber_v;
            int_table_ptr->emac.hi_ber_i = FALSE;

            emac_rx_loc_fault_i = int_table_ptr->emac.rx_loc_fault_i;
            int_table_ptr->emac.rx_loc_fault_i = FALSE;
        }
        result = emac_int_chnl_enable(enet_handle->emac_handle,
                                      link,
                                      &int_table_ptr->emac,
                                      &int_en_table_ptr->emac,
                                      enable,
                                      is_multichannels);
        if (TRUE == update_emac_from_epmm)
        {
            int_table_ptr->emac.hi_ber_i = emac_hi_ber_i;
            int_table_ptr->emac.rx_loc_fault_i = emac_rx_loc_fault_i;

            if (emac_hi_ber_i)
            {
                int_en_table_ptr->emac.hi_ber_i = enable;
            }

            if (emac_rx_loc_fault_i)
            {
                int_en_table_ptr->emac.rx_loc_fault_i = enable;
            }
        }
    }
    
    if (PMC_SUCCESS == result && ENET_LINE == enet_handle->cfg.enet_type_id)
    {
        result = etrans_int_chnl_enable(enet_handle->etrans_handle,
                                        link,
                                        &int_table_ptr->etrans,
                                        &int_en_table_ptr->etrans,
                                        enable);
    }
    
    if (PMC_SUCCESS == result)
    {
        if (TRUE == update_emac_from_epmm)
        {
            emac_hi_ber_i = int_table_ptr->epmm.gsup43_c73.hi_ber_i;
            emac_hi_ber_v = int_table_ptr->epmm.gsup43_c73.hi_ber_v;
            int_table_ptr->epmm.gsup43_c73.hi_ber_i = int_table_ptr->emac.hi_ber_i;
            int_table_ptr->epmm.gsup43_c73.hi_ber_v = int_table_ptr->emac.hi_ber_v;

            emac_rx_loc_fault_i = int_table_ptr->epmm.gsup43_c73.rx_lf_i;
            int_table_ptr->epmm.gsup43_c73.rx_lf_i = int_table_ptr->emac.rx_loc_fault_i;
        }
        result = epmm_int_chnl_enable(enet_handle->epmm_handle,
                                      link,
                                      &int_table_ptr->epmm,
                                      &int_en_table_ptr->epmm,
                                      enable);
        if (TRUE == update_emac_from_epmm)
        {
            int_table_ptr->epmm.gsup43_c73.hi_ber_i = emac_hi_ber_i;
            int_table_ptr->epmm.gsup43_c73.hi_ber_v = emac_hi_ber_v;

            int_table_ptr->epmm.gsup43_c73.rx_lf_i = emac_rx_loc_fault_i;

            if (enet_handle->var.ch_datapath[link] == ENET_SERDES_10GE_MAC)
            {
                int_en_table_ptr->epmm.gsup43_c73_valid = FALSE;   
            }
        }
    }
    
    if (PMC_SUCCESS == result)
    {
        result = empei_e_int_chnl_enable(enet_handle->empei_e_handle,
                                         link,
                                         &int_table_ptr->empei_e,
                                         &int_en_table_ptr->empei_e,
                                         enable);
    }

    if (PMC_SUCCESS == result)
    {
        result = empei_i_int_chnl_enable(enet_handle->empei_i_handle,
                                         link,
                                         &int_table_ptr->empei_i,
                                         &int_en_table_ptr->empei_i,
                                         enable);
    }

    PMC_RETURN(result);
} /* enet_int_chnl_enable */


/*******************************************************************************
* FUNCTION: enet_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   enet_handle       - Pointer to subsystem handle
*   link              - Link
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_int_chnl_clear(enet_handle_t    *enet_handle,
                                     UINT32            link,
                                     enet_int_chnl_t  *int_table_ptr,
                                     enet_int_chnl_t  *int_en_table_ptr,
                                     BOOL8             is_multichannels)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           update_emac_from_epmm = FALSE;

    PMC_ENTRY();

    result = enet_int_validate(enet_handle, link, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        result = emac_int_chnl_clear(enet_handle->emac_handle,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, emac),
                                     &int_en_table_ptr->emac,
                                     is_multichannels);
    }
    
    if (PMC_SUCCESS == result && ENET_LINE == enet_handle->cfg.enet_type_id)
    {
        result = etrans_int_chnl_clear(enet_handle->etrans_handle,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, etrans),
                                     &int_en_table_ptr->etrans);
    }
    
    if (PMC_SUCCESS == result)
    {
        update_emac_from_epmm = enet_should_emac_use_epmm_int_mon(enet_handle->var.ch_datapath[link]);
        if (NULL != int_table_ptr && 
            TRUE == update_emac_from_epmm)
        {
            int_table_ptr->epmm.gsup43_c73.hi_ber_i = int_table_ptr->emac.hi_ber_i;
            int_table_ptr->epmm.gsup43_c73.rx_lf_i = int_table_ptr->emac.rx_loc_fault_i;
        }
        result = epmm_int_chnl_clear(enet_handle->epmm_handle,
                                     link,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, epmm),
                                     &int_en_table_ptr->epmm);
    }
    
    
    if (PMC_SUCCESS == result)
    {
        result = empei_e_int_chnl_clear(enet_handle->empei_e_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, empei_e),
                                        &int_en_table_ptr->empei_e);
    }

    if (PMC_SUCCESS == result)
    {
        result = empei_i_int_chnl_clear(enet_handle->empei_i_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, empei_i),
                                        &int_en_table_ptr->empei_i);
    }



    PMC_RETURN(result);
} /* enet_int_chnl_clear */

/*******************************************************************************
* FUNCTION: enet_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   enet_handle       - Pointer to subsystem handle
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
PUBLIC PMC_ERROR enet_int_chnl_retrieve(enet_handle_t    *enet_handle,
                                        UINT32            link,
                                        enet_int_chnl_t  *filt_table_ptr,
                                        enet_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL8           update_emac_from_epmm = FALSE;
    BOOL8           filt_gsup43_c73_hi_ber_i_bkp = FALSE;
    BOOL8           filt_gsup43_c73_hi_ber_v_bkp = FALSE;
    BOOL8           filt_gsup43_c73_rx_loc_fault_i_bkp = FALSE;
    BOOL8           filt_gsup43_c73_rx_loc_fault_v_bkp = FALSE;

    PMC_ENTRY();

    result = enet_int_validate(enet_handle, link, int_table_ptr);

    
    if (PMC_SUCCESS == result && ENET_LINE == enet_handle->cfg.enet_type_id)
    {
        result = etrans_int_chnl_retrieve(enet_handle->etrans_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, etrans),
                                        &int_table_ptr->etrans);
    }

    if (PMC_SUCCESS == result)
    {
        update_emac_from_epmm = enet_should_emac_use_epmm_int_mon(enet_handle->var.ch_datapath[link]);
    }

    if (PMC_SUCCESS == result)
    {
        if (TRUE == update_emac_from_epmm && 
            NULL != filt_table_ptr)
        {
            filt_gsup43_c73_hi_ber_i_bkp = filt_table_ptr->epmm.gsup43_c73.hi_ber_i;
            filt_gsup43_c73_hi_ber_v_bkp = filt_table_ptr->epmm.gsup43_c73.hi_ber_v;
            filt_table_ptr->epmm.gsup43_c73.hi_ber_i = filt_table_ptr->emac.hi_ber_i;
            filt_table_ptr->epmm.gsup43_c73.hi_ber_v = filt_table_ptr->emac.hi_ber_v;

            filt_gsup43_c73_rx_loc_fault_i_bkp = filt_table_ptr->epmm.gsup43_c73.rx_lf_i;
            filt_gsup43_c73_rx_loc_fault_v_bkp = filt_table_ptr->epmm.gsup43_c73.rx_lf_v;
            filt_table_ptr->epmm.gsup43_c73.rx_lf_i = filt_table_ptr->emac.rx_loc_fault_i;
            filt_table_ptr->epmm.gsup43_c73.rx_lf_v = filt_table_ptr->emac.rx_loc_fault_v;
        }
        
        result = epmm_int_chnl_retrieve(enet_handle->epmm_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, epmm),
                                        &int_table_ptr->epmm);
        if (TRUE == update_emac_from_epmm && 
            NULL != filt_table_ptr)
        {
            filt_table_ptr->epmm.gsup43_c73.hi_ber_i = filt_gsup43_c73_hi_ber_i_bkp;
            filt_table_ptr->epmm.gsup43_c73.hi_ber_v = filt_gsup43_c73_hi_ber_v_bkp;
            filt_table_ptr->epmm.gsup43_c73.rx_lf_i = filt_gsup43_c73_rx_loc_fault_i_bkp;
            filt_table_ptr->epmm.gsup43_c73.rx_lf_v = filt_gsup43_c73_rx_loc_fault_v_bkp;
        }
    }
        
    if (PMC_SUCCESS == result)
    {
        result = emac_int_chnl_retrieve(enet_handle->emac_handle,
                                        link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, emac),
                                        &int_table_ptr->emac);

        if (PMC_SUCCESS == result &&
            TRUE == update_emac_from_epmm)
        {                       
            int_table_ptr->emac.hi_ber_i = int_table_ptr->epmm.gsup43_c73.hi_ber_i;            
            int_table_ptr->emac.hi_ber_v = int_table_ptr->epmm.gsup43_c73.hi_ber_v;
            int_table_ptr->emac.rx_loc_fault_i = int_table_ptr->epmm.gsup43_c73.rx_lf_i;            
            int_table_ptr->emac.rx_loc_fault_v = int_table_ptr->epmm.gsup43_c73.rx_lf_v;

            if (ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON != enet_handle->var.ch_datapath[link])
            {
                PMC_MEMSET((void *)&int_table_ptr->epmm.gsup43_c73, 0, sizeof(gsup43_c73_int_chnl_t));
            }
            else
            {
                if (NULL !=  filt_table_ptr)
                {
                    if (FALSE == filt_table_ptr->epmm.gsup43_c73.hi_ber_i)
                    {
                        int_table_ptr->epmm.gsup43_c73.hi_ber_i = FALSE;
                    }
                    if (FALSE == filt_table_ptr->epmm.gsup43_c73.hi_ber_v)
                    {
                        int_table_ptr->epmm.gsup43_c73.hi_ber_v = FALSE;
                    }
                    if (FALSE == filt_table_ptr->epmm.gsup43_c73.rx_lf_i)
                    {
                        int_table_ptr->epmm.gsup43_c73.rx_lf_i = FALSE;
                    }
                    if (FALSE == filt_table_ptr->epmm.gsup43_c73.rx_lf_v)
                    {
                        int_table_ptr->epmm.gsup43_c73.rx_lf_v = FALSE;
                    }
                }
            }
        }
    }
    
    if (PMC_SUCCESS == result)
    {
        result = empei_e_int_chnl_retrieve(enet_handle->empei_e_handle,
                                           link,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, empei_e),
                                           &int_table_ptr->empei_e);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = empei_i_int_chnl_retrieve(enet_handle->empei_i_handle,
                                           link,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, empei_i),
                                           &int_table_ptr->empei_i);
    }
    


    PMC_RETURN(result);
} /* enet_int_chnl_retrieve */

/*******************************************************************************
* FUNCTION: enet_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   enet_handle       - Pointer to subsystem handle
*   link              - Link
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   is_multichannels  - TRUE if function is being called in multi channel case
*                       FALSE if function is being called for a real channel
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
PUBLIC PMC_ERROR enet_int_chnl_enabled_check(enet_handle_t     *enet_handle,
                                             UINT32             link,
                                             enet_int_chnl_t   *int_en_table_ptr,
                                             BOOL8              *int_found_ptr,
                                             BOOL8              is_multichannels)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();

    result = enet_int_validate(enet_handle, link, int_found_ptr);

    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = emac_int_chnl_enabled_check(enet_handle->emac_handle,
                                             link,
                                             &int_en_table_ptr->emac,
                                             int_found_ptr,
                                             is_multichannels);
    }
    
    if ((PMC_SUCCESS == result && ENET_LINE == enet_handle->cfg.enet_type_id) &&
        (FALSE == *int_found_ptr))
    {
        result = etrans_int_chnl_enabled_check(enet_handle->etrans_handle,
                                             link,
                                             &int_en_table_ptr->etrans,
                                             int_found_ptr);
    }
    
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    { 
        result = epmm_int_chnl_enabled_check(enet_handle->epmm_handle,
                                             link,
                                             &int_en_table_ptr->epmm,
                                             int_found_ptr); 
    }
    
    
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = empei_e_int_chnl_enabled_check(enet_handle->empei_e_handle,
                                                link,
                                                &int_en_table_ptr->empei_e,
                                                int_found_ptr);
    }

    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = empei_i_int_chnl_enabled_check(enet_handle->empei_i_handle,
                                                link,
                                                &int_en_table_ptr->empei_i,
                                                int_found_ptr);
    }

    PMC_RETURN(result);
} /* enet_int_chnl_enabled_check */


/*******************************************************************************
* enet_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handle internal memory allocation
*
* INPUTS:
*   *enet_handle             - pointer to enet handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_alloc_init(enet_handle_t     *enet_handle)
{
    PMC_ENTRY();

    epmm_alloc_init(enet_handle->epmm_handle);

    PMC_RETURN();
} /* enet_alloc_init */

/*******************************************************************************
* enet_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on enet ss handle instance.
*
* INPUTS:
*   *enet_handle             - pointer to enet handle instance
*   *restart_init_cfg_ptr    - pointer to restart init config
*   top_energy_state_reg     - The expected energy state 
*   enet_type                - ENET SS type: LINe or SYS    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put ENET in RESET mode
*             subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_handle_restart_init(enet_handle_t     *enet_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state_reg,
                                          enet_type_id_t     enet_type)
{
    enet_recover_var_t          *enet_var_reg   = NULL;
    enet_energy_state_t enet_energy_state_reg;
    enet_var_t          *enet_var_clean = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* init it to avoid gcc and g++ compile warning */
    enet_energy_state_reg.etrans=PMC_ENERGY_STATE_RESET;
    enet_energy_state_reg.emac=PMC_ENERGY_STATE_RESET;
    enet_energy_state_reg.epmm=PMC_ENERGY_STATE_RESET;
    enet_energy_state_reg.empei_i=PMC_ENERGY_STATE_RESET;
    enet_energy_state_reg.empei_e=PMC_ENERGY_STATE_RESET; 

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ENET_LOG_ERR_STRINGS[0], ENET_LOG_ERR_TABLE_BASE, ENET_LOG_ERR_COUNT);
    }

    enet_var_reg = (enet_recover_var_t*) PMC_CALLOC(sizeof(enet_recover_var_t));
    
    rc = epmm_handle_restart_init(enet_handle->epmm_handle, restart_init_cfg_ptr, top_energy_state_reg);

    if (PMC_SUCCESS == rc)
    {
        rc = emac_handle_restart_init(enet_handle->emac_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }
    if (PMC_SUCCESS == rc)
    {
        rc = empei_e_handle_restart_init(enet_handle->empei_e_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }
    if (PMC_SUCCESS == rc)
    {
        rc = empei_i_handle_restart_init(enet_handle->empei_i_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    /* ETRANS is only instantiated in ENET_LINE */
    if (ENET_LINE == enet_type && PMC_SUCCESS == rc) {
        rc = etrans_handle_restart_init(enet_handle->etrans_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    /* 
       Recover context from device register space
    */
    if (PMC_SUCCESS == rc)
    {
       rc = enet_ctxt_reg_recover(enet_handle,
                                   enet_type, 
                                   top_energy_state_reg,
                                   enet_var_reg,
                                   &enet_energy_state_reg);
    }


    if (PMC_SUCCESS == rc)
    {
        enet_var_clean = (enet_var_t*) PMC_CALLOC(sizeof(enet_var_t));
        
        /* 
         * Audit register context and memory context 
         *   - Audit power and decide if we should just shut down the subsystem 
         *   - Create a new context that represents the intersection between memory and register space (enet_var_clean)
         */
        rc = enet_ctxt_audit(enet_handle, 
                             enet_var_reg,
                             &enet_energy_state_reg,
                             enet_var_clean); 
        
        PMC_LOG_TRACE("enet_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
        if (ENET_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
        {
            /* 
             * Cleanup 
             *   - At the end, enet_var_clean is the new context
             *    - We keep OPERATIONAL channel defined in enet_var_clean
             *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
             *       - they may have be partially provisionned
             *    - We ajust power setting based on enet_var_clean
             */                                    
            rc = enet_ctxt_cleanup(enet_handle, 
                                   enet_var_reg,
                                   &enet_energy_state_reg,
                                   enet_var_clean); 
        }
        PMC_FREE(&enet_var_clean);

        /* 
         * in second phase, we clean MPMO channels associated to OPERATIONAL channels 
         * that are unconnected to the wwith shall be cleaned. MPMO are provisionned when we connect 1

         * the segment the switch 
         *  
         */
        if (PMC_SUCCESS == rc && restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_SECOND)
        {
            enet_ctxt_mpmo_cleanup(enet_handle,restart_init_cfg_ptr);
        }
    }

    
    PMC_FREE(&enet_var_reg);
    PMC_RETURN(rc);
} /* enet_handle_restart_init */


/*******************************************************************************
* enet_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Compare given state againt RX or TX channel state. Only operationnal state is 
*    processed.
*
* INPUTS:
*    hndl                   - pointer to ENET handle instance
*    link                   - channel ID to be configured Valid range: 0 to 11
*    rx_chnl                - when TRUE query concerns RX channel state, 
*                             otherwise TX state state is returned
*    state                  - link state to compare against context
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfull otherwise a descriptive error 
*   code is returned.
*
* NOTES: 
*
*******************************************************************************/
PUBLIC BOOL8 enet_prov_get(enet_handle_t             *hndl,
                           UINT32                    link,
                           BOOL8                     rx_chnl,
                           util_global_chnl_state_t  state)
{
    BOOL8 rc = FALSE;
    enet_lnk_state_t lnk_state;

    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Check that link number is less than 12*/
    if (link < ENET_NUM_LINK_MAX) 
    {

        lnk_state = enet_util_global_state_convert(state);

        if (TRUE == rx_chnl)
        {            
            rc = (lnk_state == hndl->var.lnk_ctxt[link].rx_state);
        }
        else
        {
            rc = (lnk_state == hndl->var.lnk_ctxt[link].tx_state);
        }
    }

    PMC_RETURN(rc);
}/* enet_prov_get */

/*******************************************************************************
* enet_enhanced_pmon_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Return TRUE if the link is configured in Enhanced PMON mode 
*
* INPUTS:
*    hndl                   - pointer to ENET handle instance
*    link                   - channel ID to be configured Valid range: 0 to 11
*    etrans_path            - TRUE: etrans datapath
*                             FALSE: serdes datapath
*
* OUTPUTS:
*   *data_rate             - data_rate of the specified link
*
* RETURNS:
*   TRuE when the link is configured in enhanced PMON mode, otherwise FALSE
*
* NOTES: 
*
*******************************************************************************/
PUBLIC BOOL8 enet_enhanced_pmon_prov_get(enet_handle_t             *hndl,
                                         UINT32                    link,
                                         BOOL8                     etrans_path,
                                         enet_data_rate_t          *data_rate)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (hndl->var.lnk_ctxt[link].rx_state == ENET_LNK_OPERATIONAL)
    {
        if (TRUE == etrans_path)
        {
            switch (hndl->var.ch_datapath[link])
            {           
            case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
            case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
                result = TRUE;
                break;
            default:
                break;
            }
        } else 
        {
            switch (hndl->var.ch_datapath[link])
            {        
            case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_100GE_MAC_TX_PMON:
            case ENET_SLAVE_40GE_MAC_TX_PMON:
            case ENET_SLAVE_10GE_MAC_TX_PMON:
                result = TRUE;
                break;
            default:
                break;
            }
        }
    }
            
    if (result == TRUE)
    {
        *data_rate = enet_convert_datapath_to_enet_data_rate(hndl->var.ch_datapath[link]);
    }
    PMC_RETURN(result);

}/* enet_enhanced_pmon_prov_get */

/*
** Private Functions
*/

/*******************************************************************************
*  enet_convert_datapath_to_enet_data_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_data_rate_t - datapath rate
*
* NOTES:
*
*******************************************************************************/
PUBLIC enet_data_rate_t enet_convert_datapath_to_enet_data_rate( enet_datapath_t datapath)
{
    enet_data_rate_t rc;
    PMC_ENTRY();

    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    switch(datapath)
    {
    case ENET_SERDES_100GE_MAC:
    case ENET_SERDES_100GE_TRANSPARENT        :
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON :
    case ENET_SLAVE_100GE_MAC_TX_PMON         :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC :
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON :
    case ENET_MAPOTN_GMP_G709_100GE_MAC       :
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  :
        rc = ENET_LINK_100G;
        break;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    case ENET_SERDES_40GE_MAC                :
    case ENET_SERDES_40GE_TRANSPARENT        :
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON :
    case ENET_SLAVE_40GE_MAC_TX_PMON         :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC :
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON :
    case ENET_MAPOTN_GMP_G709_40GE_MAC       :
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  : 
       rc = ENET_LINK_40G;
        
        break;
    case ENET_SERDES_10GE_MAC:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        rc = ENET_LINK_10G;
        break;

    default:
        rc = LAST_ENET_LINK_DATA_RATE;
        break;
    }

    PMC_RETURN(rc);
    
}/*enet_convert_datapath_to_enet_data_rate*/

/*******************************************************************************
*  enet_convert_datapath_to_emac_data_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_data_rate_t - datapath rate 
*
* NOTES:
*
*******************************************************************************/
PRIVATE emac_data_rate_t enet_convert_datapath_to_emac_data_rate( enet_datapath_t datapath)
{
    emac_data_rate_t result;

    PMC_ENTRY();
    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {

        result = EMAC_LINK_100G;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        result = EMAC_LINK_40G;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {
        /* ENET_SERDES_10GE_MAC  */
        /* ENET_SERDES_10GE_TRANSPARENT */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT */
        /* ENET_SERDES_10GE_TRANSPARENT_RX_PMON */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON */
        /* ENET_SLAVE_10GE_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_MAC_TX_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON */
        result = EMAC_LINK_10G;
    }
    PMC_RETURN(result);
}

/*******************************************************************************
*  enet_convert_datapath_to_emac_rx_data_src
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_data_rate_t - datapath rate 
*
* NOTES:
*
*******************************************************************************/
PRIVATE emac_rx_data_src_t enet_convert_datapath_to_emac_rx_data_src( enet_datapath_t datapath)
{
    emac_rx_data_src_t d_path;

    PMC_ENTRY();

    /* EMAC Source */
    switch (datapath) 
    {
        /* SERDES Datapath configuration. */
    case ENET_SERDES_100GE_MAC:
    case ENET_SERDES_40GE_MAC:
    case ENET_SERDES_10GE_MAC: 
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
        /* Datapath with Ingress also routed to the Rx MAC for PMON */
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    {
        d_path = EMAC_RX_SRC_RX_SERDES;
        break;      
    }
    /* The configurations below are used for PMON only */
    /* CPB -> Tx EPMM -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
    {
        d_path = EMAC_RX_SRC_TX_EPMM;
        break;
    }
    /* CPB -> Tx MAC -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON: 
    {
        d_path = EMAC_RX_SRC_TX_EMAC_PMA;
        break;
    }
    /* MAPOTN Datapath configuration. Only valid for ENET_LINE. Not valid for ENET_SYS. */
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
        /* MAPOTN -> Rx ETRANS -> Rx MAC for PMON */
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    {
        d_path = EMAC_RX_SRC_RX_ETRANS;
        break;
    }
    default : 
    {
        d_path = EMAC_RX_SRC_UNKNOWN;
        break;
    }
    }

    PMC_RETURN(d_path);
}


/*******************************************************************************
*  enet_convert_datapath_to_emac_tx_data_dst
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_tx_data_dst_t - TX destination 
*
* NOTES:
*
*******************************************************************************/
PRIVATE emac_tx_data_dst_t enet_convert_datapath_to_emac_tx_data_dst( enet_datapath_t datapath)
{
    emac_tx_data_dst_t d_path;

    PMC_ENTRY();

    /* EMAC Source */
    switch (datapath) 
    {
        /* SERDES Datapath configuration. */
    case ENET_SERDES_100GE_MAC:
    case ENET_SERDES_40GE_MAC:
    case ENET_SERDES_10GE_MAC: 
        /* CPB -> Tx MAC -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON: 
    {
        d_path = EMAC_TX_DST_TX_PMA;
        break;                
    }
    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
        /* Datapath with Ingress also routed to the Rx MAC for PMON */
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    {
        d_path = EMAC_TX_DST_TX_EPMM;
        break;      
    }
    /* The configurations below are used for PMON only */
    /* CPB -> Tx EPMM -> loopback -> Rx MAC for PMON */
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: 
    {
        d_path = EMAC_TX_DST_RESERVED_1;
        break;
    }
    /* MAPOTN Datapath configuration. Only valid for ENET_LINE. Not valid for ENET_SYS. */
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
        /* MAPOTN -> Rx ETRANS -> Rx MAC for PMON */
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
    {
        d_path = EMAC_TX_DST_TX_ETRANS;
        break;
    }
    default : 
    {
        d_path = EMAC_TX_DST_UNKNOWN;
        break;
    }
    }


    PMC_RETURN(d_path);
}

/*******************************************************************************
*  enet_convert_datapath_to_etime_data_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   etime_data_rate_t - data rate 
*
* NOTES:
*
*******************************************************************************/
PRIVATE etime_data_rate_t enet_convert_datapath_to_etime_data_rate( enet_datapath_t datapath)
{
    etime_data_rate_t result;

    PMC_ENTRY();
    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {

        result = ETIME_LINK_100G;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        result = ETIME_LINK_40G;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {
        /* ENET_SERDES_10GE_MAC  */
        /* ENET_SERDES_10GE_TRANSPARENT */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT */
        /* ENET_SERDES_10GE_TRANSPARENT_RX_PMON */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON */
        /* ENET_SLAVE_10GE_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_MAC_TX_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON */
        result = ETIME_LINK_10G;
    }
    PMC_RETURN(result);
}


/*******************************************************************************
*  enet_convert_datapath_to_empei_i_data_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_data_rate_t - data rate
*
* NOTES:
*
*******************************************************************************/
PRIVATE empei_i_data_rate_enum_t enet_convert_datapath_to_empei_i_data_rate( enet_datapath_t datapath)
{
    empei_i_data_rate_enum_t result;

    PMC_ENTRY();
    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {

        result = EMPEI_I_LINK_100G;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        result = EMPEI_I_LINK_40G;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {
        /* ENET_SERDES_10GE_MAC  */
        /* ENET_SERDES_10GE_TRANSPARENT */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT */
        /* ENET_SERDES_10GE_TRANSPARENT_RX_PMON */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON */
        /* ENET_SLAVE_10GE_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_MAC_TX_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON */
        result = EMPEI_I_LINK_10G;
    }
    PMC_RETURN(result);
}

/*******************************************************************************
*  enet_convert_datapath_to_empei_e_data_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   emac_data_rate_t - data rate
*
* NOTES:
*
*******************************************************************************/
PRIVATE empei_e_data_rate_enum_t enet_convert_datapath_to_empei_e_data_rate( enet_datapath_t datapath)
{
    empei_e_data_rate_enum_t result;

    PMC_ENTRY();
    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {

        result = EMPEI_E_LINK_100G;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        result = EMPEI_E_LINK_40G;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {
        /* ENET_SERDES_10GE_MAC  */
        /* ENET_SERDES_10GE_TRANSPARENT */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT */
        /* ENET_SERDES_10GE_TRANSPARENT_RX_PMON */
        /* ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON */
        /* ENET_SLAVE_10GE_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON */
        /* ENET_SLAVE_10GE_MAC_TX_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON */
        result = EMPEI_E_LINK_10G;
    }
    PMC_RETURN(result);
}

/*******************************************************************************
*  enet_convert_datapath_to_empei_e_datapath_sel
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to convert
*
* OUTPUTS:
*
* RETURNS:
*  empei_e_datapath_type_enum_t - datapath type
*
* NOTES:
*
*******************************************************************************/
PRIVATE empei_e_datapath_type_enum_t enet_convert_datapath_to_empei_e_datapath_sel( enet_datapath_t datapath)
{
    empei_e_datapath_type_enum_t d_path = EMPEI_E_TRANSPARENT_DATAPATH;

    PMC_ENTRY();

    /* Configure EMPEI_I to select MAC or Transparent datapath to CPB */
    /* Default path is to MAC/PCS */
    if ((datapath == ENET_SERDES_10GE_MAC                    ) ||
        (datapath == ENET_SLAVE_10GE_MAC_TX_PMON             ) ||
        (datapath == ENET_SERDES_40GE_MAC                    ) ||
        (datapath == ENET_SLAVE_40GE_MAC_TX_PMON             ) ||
        (datapath == ENET_SERDES_100GE_MAC                   ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON            ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC     ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC          ) ||
        (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC           ) ||
        (datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC         ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON     ) ||
        (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON      ) ||
        (datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON    ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON)) {
        d_path = EMPEI_E_MAC_DATAPATH;
    } 
    PMC_RETURN(d_path);


}  /* enet_convert_datapath_to_empei_e_datapath_sel */

/*******************************************************************************
*  enet_convert_datapath_to_epmm_datapath
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   epmm_datapath_t  - EPMM datapath
*
* NOTES:
*
*******************************************************************************/
PRIVATE epmm_datapath_t enet_convert_datapath_to_epmm_datapath(enet_datapath_t datapath)
{
    epmm_datapath_t result;

    PMC_ENTRY();
    /* -------------- */
    /* 100GE datapaths */
    /* -------------- */
    if ((datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON)) {
        result = EPMM_100GE;
    }  
    else if ((datapath == ENET_SERDES_100GE_MAC        ) ||
               (datapath == ENET_SLAVE_100GE_MAC_TX_PMON)  ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON) ||
               (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC) ||
               (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON)) {
        result = EPMM_UNUSED_100G;
        
        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    }  
    else if ((datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON)) {      
        result = EPMM_40GE;
        
    } else if ((datapath == ENET_SERDES_40GE_MAC        ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON)) {
        result = EPMM_UNUSED_40G;
        
        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } 
    else if ((datapath == ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON)) {
        result = EPMM_10GE_GSUP43_73;
        
    } 
    else if ((datapath == ENET_SERDES_10GE_TRANSPARENT        ) ||
               (datapath == ENET_SLAVE_10GE_TRANSPARENT_TX_PMON)) {
        result = EPMM_10GE_CBR;
    } 
    else if ((datapath == ENET_SERDES_10GE_MAC))
    {
        result = EPMM_10GE_GSUP43_73_HI_BER_MON;
    } 
    else if ((datapath == ENET_SERDES_10GE_TRANSPARENT_RX_PMON))
    {
        result = EPMM_10GE_CBR_GSUP43_73_HI_BER_MON;
    } 
    else {
        /* ENET_SERDES_10GE_MAC  */
        /* ENET_SLAVE_10GE_MAC_TX_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC */
        /* ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC */
        /* ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON */
        /* ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON */
        result = EPMM_UNUSED_10G;
    }
    PMC_RETURN(result);
}

/*******************************************************************************
*  enet_is_epmm_gsup43_c73_used_as_hi_ber_mon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE if EPMM GSUP43_C73 block is used for some EMAC interrupts.
*   HI_BER, RX_LOC_FAULT
*
* INPUTS:
*   datapath - datapath to evaluate
*
* OUTPUTS:
*
* RETURNS:
*  TRUE when EPMM GSUP43_C73 block is used as EMAC interrupt monitor for some
*  interrupts.
*  FALSE otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 enet_should_emac_use_epmm_int_mon(enet_datapath_t datapath)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();

    if ((ENET_SERDES_10GE_MAC == datapath) ||
        (ENET_SERDES_10GE_TRANSPARENT_RX_PMON == datapath) ||
        (ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON == datapath))
    {
        rc = TRUE;
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
*  enet_convert_datapath_to_etrans_datapath
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   datapath - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   etrans_datapath_t  - ETRANS datapath
*
* NOTES:
*
*******************************************************************************/
PRIVATE etrans_datapath_t enet_convert_datapath_to_etrans_datapath( enet_datapath_t datapath)
{
    etrans_datapath_t d_path = ETRANS_UNUSED;

    PMC_ENTRY();

    if (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ||
        datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON) {
        d_path = ETRANS_GFP_GSUP43_C62_G709_100GE_MAC;
    } else if (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC) {
        d_path = ETRANS_GMP_G709_100GE_MAC;
    } else if (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON) {
        d_path = ETRANS_GMP_G709_100GE_MAC;
    } else if (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ||
               datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON) {
        d_path = ETRANS_GFP_GSUP43_C62_G709_40GE_MAC;
    } else if (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC) {
        d_path = ETRANS_GMP_G709_40GE_MAC;
    } else if (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON) {
        d_path = ETRANS_GMP_G709_40GE_MAC;
    } else if (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC ||
               datapath == ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON) {
        d_path = ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC;
    } else if (datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC ||
               datapath == ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON) {
        d_path = ETRANS_GFP_GSUP43_C73_10GE_MAC;
    } else if (datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC ||
               datapath == ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON) {
        d_path = ETRANS_BMP_GSUP43_C71_10GE_MAC;
    }

    PMC_RETURN(d_path);
}

/*******************************************************************************
*  enet_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes ENET var_t variables.
*
* INPUTS:
*   *var_ptr          - pointer to ENET handle variable configuration
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
PRIVATE void enet_var_default_init(enet_var_t *var_ptr)
{
    UINT32 link;
    
    PMC_ENTRY();
    
    PMC_ASSERT(var_ptr != NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    for(link = 0; link < ENET_NUM_LINK_MAX; link++)
    {
        var_ptr->lnk_ctxt[link].rx_state = ENET_LNK_START;
        var_ptr->lnk_ctxt[link].tx_state = ENET_LNK_START;
        var_ptr->ch_datapath[link]       = ENET_UNPROV_DATAPATH;
        var_ptr->dpi_sch_slot[link]      = 0xF;
        var_ptr->lnk_ctxt[link].empei_i_eclass_en = FALSE;
        var_ptr->lnk_ctxt[link].empei_e_eclass_en = FALSE;
        var_ptr->lnk_ctxt[link].ing_ts_en          = FALSE;
        var_ptr->lnk_ctxt[link].egr_ts_en         = FALSE;
    }
    
    var_ptr->lnks_prov_cnt      = 0;
    var_ptr->not_first_reset = FALSE;

    PMC_RETURN();
} /* enet_var_default_init */

/*******************************************************************************
*  enet_update_hndl_var
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   hndl                - handle to the ENET instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   datapath            - datapath to configure
*   ts_en               - TRUE: Enable the timestamp feature. False: Disable
*   config              - TRUE: Configure.
*
* OUTPUTS:
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_update_hndl_var( enet_handle_t *hndl, UINT32 link,  enet_datapath_t datapath, BOOL8 ts_en, BOOL8 config)
{
    UINT32 index;
    UINT32 jndex;

    PMC_ENTRY();

    /* --------------- */
    /* 100GE datapaths */
    /* --------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {

        if( config == TRUE ) {
            /* Configure all time slots to 0 */
            for (jndex = 0; jndex < ENET_NUM_LINK_100GE; jndex++) {
                hndl->var.dpi_sch_slot[jndex] = 0;
                hndl->var.ch_datapath[jndex] = datapath;
                if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                    enet_db_sch_slot_set(hndl, jndex, 0);
                }
            }      
        } else {
            /* Configure all time slots to 0xF */
            for (jndex = 0; jndex < ENET_NUM_LINK_100GE; jndex++) {
                hndl->var.dpi_sch_slot[jndex] = 0xF;
                hndl->var.ch_datapath[jndex] = ENET_UNPROV_DATAPATH;
                if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                    enet_db_entry_clear(hndl, link);
                }
            }      
        }


        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        if( config == TRUE ) {
            for ( jndex = 0; jndex < ENET_NUM_LINK_40GE; jndex++ ) {
                hndl->var.dpi_sch_slot[(jndex*3)+(link%3)] = link;
                hndl->var.ch_datapath[(link + jndex)] = datapath;
                if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                    enet_db_sch_slot_set(hndl, (jndex*3)+(link%3), link);
                }
            }
        } else {
            for ( jndex = 0; jndex < ENET_NUM_LINK_40GE; jndex++ ) {
                hndl->var.dpi_sch_slot[(jndex*3)+(link%3)] = 0xF;
                hndl->var.ch_datapath[(link + jndex)] = ENET_UNPROV_DATAPATH;
                if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                    enet_db_entry_clear(hndl, link);
                }
            }
        }

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {

        index = ((link%4)*3) + (link/4);

        if( config == TRUE ) {
            hndl->var.dpi_sch_slot[index] = link;
            hndl->var.ch_datapath[link] = datapath;
            if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                enet_db_sch_slot_set(hndl, index,link);
            }
        } else {
            hndl->var.dpi_sch_slot[index] = 0xF;
            hndl->var.ch_datapath[link] = ENET_UNPROV_DATAPATH;
            if(ENET_SCHD_POPULATE_LOCAL == hndl->cfg.schd_populate_mode)  {
                enet_db_entry_clear(hndl, link);
            }
        }
    }

    if( ts_en ) {
        hndl->var.lnk_ctxt[link].ing_ts_en = TRUE;
        hndl->var.lnk_ctxt[link].egr_ts_en = TRUE;
    }

    PMC_RETURN( PMC_SUCCESS );
} /* enet_update_hndl_var */

/*******************************************************************************
*  enet_validate_datapath_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert enums
*
* INPUTS:
*   hndl                - handle to the ENET instance to be operated on
*   link                - channel ID to be configured Valid range: 0 to 11
*   datapath            - datapath to configure
*   config              - TRUE: Configure.
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR - meaningful error messages
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_validate_datapath_cfg(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, BOOL8 config)
{
    PMC_ENTRY();

    /* Check that link number is less than 12*/
    if (link >= ENET_NUM_LINK_MAX) {
        PMC_RETURN(ENET_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Check for invalid datapath param*/
    if ((datapath >= LAST_ENET_DATAPATH) || (datapath == ENET_DATAPATH_LAST_SERDES_OR_SLAVE)) {
        PMC_RETURN(ENET_LOG_ERR_INVALID_PARAMETERS);
    }

    /* MAPOTN datapaths are only valid for ENET_LINE */
    if (ENET_SYS == hndl->cfg.enet_type_id) {
        if (datapath >= ENET_DATAPATH_LAST_SERDES_OR_SLAVE) {
            PMC_RETURN(ENET_LOG_ERR_INVALID_PARAMETERS);
        }
    }

    if( config == TRUE ) {
        /* Check to see if the link is already provisioned */
        if (hndl->var.ch_datapath[link] != ENET_UNPROV_DATAPATH) {
            PMC_RETURN(ENET_LOG_ERR_ALREADY_PROVISIONED);
        }
    }

    /* Check for valid link according to bw requested */
    /* --------------- */
    /* 100GE datapaths */
    /* --------------- */
    if ((datapath == ENET_SERDES_100GE_MAC                ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT        ) ||
        (datapath == ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ||
        (datapath == ENET_SLAVE_100GE_TRANSPARENT_TX_PMON ) ||
        (datapath == ENET_SLAVE_100GE_MAC_TX_PMON         ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC ) ||
        (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC       ) ||
        (datapath == ENET_MAPOTN_GMP_G709_100GE_MAC_PMON  )) {
        if (link != 0) {
            PMC_RETURN(ENET_LOG_ERR_INVALID_PARAMETERS);
        } 

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    } else if ((datapath == ENET_SERDES_40GE_MAC                ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT        ) ||
               (datapath == ENET_SERDES_40GE_TRANSPARENT_RX_PMON) ||
               (datapath == ENET_SLAVE_40GE_TRANSPARENT_TX_PMON ) ||
               (datapath == ENET_SLAVE_40GE_MAC_TX_PMON         ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC ) ||
               (datapath == ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC       ) ||
               (datapath == ENET_MAPOTN_GMP_G709_40GE_MAC_PMON  )) {

        if ((link != ENET_40GE_LINK_1) &&
            (link != ENET_40GE_LINK_2) &&
            (link != ENET_40GE_LINK_3)) {

            PMC_RETURN(ENET_LOG_ERR_INVALID_PARAMETERS);
        } 

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    } else {

    }


    PMC_RETURN(PMC_SUCCESS);
} /* enet_validate_datapath_cfg */

/*******************************************************************************
* enet_is_pmon_datapath_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return TRUE when datapath path is related to a PMON ENET, otherwise FALSE.
*
* INPUTS:
*   datapath            - datapath to configure
*
* OUTPUTS:
*
* RETURNS:
*   Return TRUE when datapath path is related to a PMON ENET, otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 enet_is_pmon_datapath_get(enet_datapath_t datapath)
{
    BOOL8 rc = FALSE;
    PMC_ENTRY();

    /* Check for valid link according to bw requested */
    /* --------------- */
    /* 100GE datapaths */
    /* --------------- */
    switch (datapath)
    {
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_SLAVE_10GE_MAC_TX_PMON:
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        rc = TRUE;
        break;
    default:
        break;
    }

    PMC_RETURN(rc);
} /* enet_is_pmon_datapath_get */


/*******************************************************************************
* enet_db_all_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the enet databases.
*
*
* INPUTS:
*   *enet_handle_ptr    - pointer to enet handle instance
*   enet_type_id        - instance of the ENET module
*   schd_populate_mode  - Array of enum defining the DCS scheduler mode for enet schedulers 
*                         Array index:
*                          0 : scheduler mode for scbs3
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void enet_db_all_init(enet_handle_t *enet_handle_ptr,
                              enet_type_id_t enet_type_id,
                              enet_schd_populate_t schd_populate_mode)
{    
    PMC_ENTRY();
    
    PMC_ASSERT( schd_populate_mode < LAST_ENET_SCHD_POPULATE, ENET_LOG_ERR_INVALID_PARAMETERS, 0 , 0);
        
    if(ENET_SCHD_POPULATE_LOCAL == schd_populate_mode || ENET_SCHD_POPULATE_DB == schd_populate_mode) {
        if( ENET_LINE == enet_type_id)
        {
            enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode = UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB;
            enet_handle_ptr->epmm_handle->mpma_db_alloc_mode = UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB;
            enet_handle_ptr->cfg.schd_use_ctxt.db_id = UTIL_GEN_DB_ENET_LINE_MPMA;                        
        } else 
        {
            enet_handle_ptr->cfg.schd_use_ctxt.alloc_mode = UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB;
            enet_handle_ptr->epmm_handle->mpma_db_alloc_mode = UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB;
            enet_handle_ptr->cfg.schd_use_ctxt.db_id = UTIL_GEN_DB_ENET_SYS_MPMA;
        }
        

        enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle = gen_db_attach(&(enet_handle_ptr->base),enet_handle_ptr->cfg.schd_use_ctxt.db_id);
        PMC_ASSERT(enet_handle_ptr->cfg.schd_use_ctxt.db_client_handle != 0, ENET_ERR_DB_ATTACH_FAIL, 0, 0); /* failure if data base attach failed */                 
    } else {
        /* unspecified schedular mode case, code error */
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0); /* failure if data base attach failed */
    }
    PMC_RETURN();
} /* enet_db_all_init */

/*******************************************************************************
*  enet_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the ENET.
*
* INPUTS:
*   hndl           - handle to the ENET instance
*   energy_state   - energy state of ENET\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_reg_SUBSYSTEM_CTRL_write     ( NULL, hndl, 0 );
        enet_field_EMAC120_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMAC120_RAM_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMAC120_SW_RST_set(NULL, hndl, 0);

        if (ENET_LINE == hndl->cfg.enet_type_id)
        {
            enet_reg_MTSB_LOW_POWER_MODE_write( NULL, hndl, 0 );
            enet_reg_MTSB_SW_RESET_write      ( NULL, hndl, 0 );
        }
        else
        {
            enet_reg_MTSB_LOW_POWER_MODE_write( NULL, hndl, (1 << ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_RAM_LOWPWR_OFF) | (1 << ENET120_TOP_REG_MTSB_LOW_POWER_MODE_BIT_ETRANS120_LOWPWR_OFF));
            enet_reg_MTSB_SW_RESET_write      ( NULL, hndl, 1 << ENET120_TOP_REG_MTSB_SW_RESET_BIT_ETRANS120_SW_RST_OFF);
        }
        break;            
    case PMC_ENERGY_STATE_RESET:
        enet_reg_SUBSYSTEM_CTRL_write     ( NULL, hndl, 1 );
        enet_reg_MTSB_SW_RESET_write      ( NULL, hndl, 1 );
        enet_reg_MTSB_LOW_POWER_MODE_write( NULL, hndl, 1 );
        enet_field_EMAC120_SW_RST_set(NULL, hndl, 1);
        enet_field_EMAC120_LOWPWR_set(NULL, hndl, 1);
        enet_field_EMAC120_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* enet_energy_state_reg_set */

/*******************************************************************************
*  enet_etrans_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the ETRANS module.
*
* INPUTS:
*   hndl           - handle to the ENET instance to be operated on
*   energy_state   - energy state of ETRANS\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_etrans_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_field_ETRANS120_LOWPWR_set(NULL, hndl, 0);
        enet_field_ETRANS120_RAM_LOWPWR_set(NULL, hndl, 0);
        enet_field_ETRANS120_SW_RST_set(NULL, hndl, 0);
        break;
    case PMC_ENERGY_STATE_RESET:
        enet_field_ETRANS120_SW_RST_set(NULL, hndl, 1);
        enet_field_ETRANS120_LOWPWR_set(NULL, hndl, 1);
        enet_field_ETRANS120_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* enet_etrans_energy_state_reg_set */

/*******************************************************************************
*  enet_epmm_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the EPMM module.
*
* INPUTS:
*   hndl           - handle to the ENET instance to be operated on
*   energy_state   - energy state of EPMM\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_epmm_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_field_EPMM120_LOWPWR_set(NULL, hndl, 0);
        enet_field_EPMM120_RAM_LOWPWR_set(NULL, hndl, 0);
        enet_field_EPMM120_SW_RST_set(NULL, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        enet_field_EPMM120_SW_RST_set(NULL, hndl, 1);
        enet_field_EPMM120_LOWPWR_set(NULL, hndl, 1);
        enet_field_EPMM120_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN();
} /* enet_epmm_energy_state_reg_set */

/*******************************************************************************
*  enet_empei_e_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the EMPEI_E module.
*
* INPUTS:
*   hndl           - handle to the ENET instance to be operated on
*   energy_state   - energy state of EMPEI_E\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_empei_e_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_field_EMPEI120_E_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMPEI120_E_RAM_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMPEI120_E_SW_RST_set(NULL, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        enet_field_EMPEI120_E_SW_RST_set(NULL, hndl, 1);
        enet_field_EMPEI120_E_LOWPWR_set(NULL, hndl, 1);
        enet_field_EMPEI120_E_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* enet_empei_e_energy_state_reg_set */

/*******************************************************************************
*  enet_empei_i_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the EMPEI_I module.
*
* INPUTS:
*   hndl           - handle to the ENET instance to be operated on
*   energy_state   - energy state of EMPEI_I\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_empei_i_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_field_EMPEI120_I_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMPEI120_I_RAM_LOWPWR_set(NULL, hndl, 0);
        enet_field_EMPEI120_I_SW_RST_set(NULL, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        enet_field_EMPEI120_I_SW_RST_set(NULL, hndl, 1);
        enet_field_EMPEI120_I_LOWPWR_set(NULL, hndl, 1);
        enet_field_EMPEI120_I_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* enet_empei_i_energy_state_reg_set */

/*******************************************************************************
*  enet_emac_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the EMAC module.
*
* INPUTS:
*   hndl           - handle to the ENET instance to be operated on
*   energy_state   - energy state of EMAC\n
*
* OUTPUTS:
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_emac_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state)
{

    PMC_ENTRY();
    
    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        enet_field_EMAC120_RAM_LOWPWR_set(NULL, hndl, 0);
        break;            
    case PMC_ENERGY_STATE_RESET:
        enet_field_EMAC120_RAM_LOWPWR_set(NULL, hndl, 1);
        break;
    default:
        PMC_ASSERT(FALSE, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN();
} /* enet_emac_energy_state_reg_set */

/*******************************************************************************
*  enet_convert_dir_to_epmm_dir
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Converts enet direction into epmm direction value.
*
* INPUTS:
*   direction  - enet direction
*
* OUTPUTS:
*
* RETURNS:
*   EPMM formatted direction.
*
* NOTES:
*
*******************************************************************************/
PRIVATE epmm_link_direction_t enet_convert_dir_to_epmm_dir(enet_dir_t direction)
{
    epmm_link_direction_t   epmm_dir;

    PMC_ENTRY();

    switch (direction)
    {
    case ENET_EGRESS:
        epmm_dir = EPMM_LINK_DIR_EGRESS;
        break;
    case ENET_INGRESS:
        epmm_dir = EPMM_LINK_DIR_INGRESS;
        break;
    default:
        epmm_dir = LAST_EPMM_LINK_DIR;
        break;
    }

    PMC_RETURN(epmm_dir);
}/* epmm_link_direction_t */


/*******************************************************************************
* FUNCTION: enet_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   enet_handle     - Pointer to enet handle
*   enable          - Enables/disables interrupt and status support.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_int_init(enet_handle_t    *enet_handle,
                                BOOL8             enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;

    PMC_ENTRY();

    for (i = 1; i < 3; i++)
    {
        
        if ( i==2 ) {
            enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, enet_handle, i, 0x1FFFFFD * enable);
        } else {
            enet_reg_SUBSTSEM_INT_EN_PAGE_0_array_write(NULL, enet_handle, i, 0xFFFFFFFF * enable);
        }
        enet_reg_SUBSTSEM_INT_EN_PAGE_1_array_write(NULL, enet_handle, i, 0xFFFFFFFF * enable);
    } 

    PMC_RETURN(result);
} /* enet_int_init */


/*******************************************************************************
*  enet_etrans_tx_lock_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return ETRANS TX PCS align or lock status. 
*
* INPUTS:
*   *hndl               - pointer to ENET handle instance to be operated on 
*   link                - channel ID to be polled
*   etrans_datapath     - ETRANS datapath to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ETRANS TX lock status.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 enet_etrans_tx_lock_status_get(enet_handle_t     *hndl,
                                             UINT32             link,
                                             etrans_datapath_t  etrans_datapath)
{
    BOOL8 rc = FALSE;
    PMC_ENTRY();
                
    rc = etrans_lock_status_get(hndl->etrans_handle, link, etrans_datapath);   
    
    PMC_RETURN(rc);
} /* enet_etrans_lock_poll */


/*******************************************************************************
*  enet_gfp_datapath_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return ETRANS TX PCS align or lock status. Function process 
*
* INPUTS:
*   etrans_datapath     - ETRANS datapath to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ETRANS TX lock status.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 enet_is_gfp_datapath_get(etrans_datapath_t  etrans_datapath)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();
    
    switch(etrans_datapath)
    {
    case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC :
    case ETRANS_GFP_GSUP43_C73_10GE_MAC :
    case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
    case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC :             
        rc = TRUE;   
        break;   
    default :
        break;
    }
    
    PMC_RETURN(rc);
} /* enet_gfp_datapath_get */


/*******************************************************************************
*  enet_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets all energy states from enet
*   
* INPUTS:
*   *hndl      - pointer to ENET handle instance to be operated on
*
* OUTPUTS:
*   *enet_energy_state - ENET energy states
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void enet_energy_state_reg_get(enet_handle_t       *hndl, 
                                       enet_energy_state_t *enet_energy_state)
{    
    UINT32 low_pwr, ram_low_pwr, soft_reset;
    
    PMC_ENTRY();    
    PMC_ASSERT(hndl != NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* ETRANS */
    soft_reset = enet_field_ETRANS120_SW_RST_get(NULL, hndl);
    low_pwr = enet_field_ETRANS120_LOWPWR_get(NULL, hndl);
    ram_low_pwr = enet_field_ETRANS120_RAM_LOWPWR_get(NULL, hndl);
    enet_energy_state->etrans = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* EMAC */
    soft_reset = enet_field_EMAC120_SW_RST_get(NULL, hndl);
    low_pwr = enet_field_EMAC120_LOWPWR_get(NULL, hndl);
    ram_low_pwr = enet_field_EMAC120_RAM_LOWPWR_get(NULL, hndl);
    enet_energy_state->emac = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* EPMM */
    soft_reset = enet_field_EPMM120_SW_RST_get(NULL, hndl);
    low_pwr = enet_field_EPMM120_LOWPWR_get(NULL, hndl);
    ram_low_pwr = enet_field_EPMM120_RAM_LOWPWR_get(NULL, hndl);
    enet_energy_state->epmm = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* exception for these ones */
    /* EMPEI_E */
    soft_reset = enet_field_EMPEI120_E_SW_RST_get(NULL, hndl);
    low_pwr = enet_field_EMPEI120_E_LOWPWR_get(NULL, hndl);
    ram_low_pwr = enet_field_EMPEI120_E_RAM_LOWPWR_get(NULL, hndl);
    enet_energy_state->empei_e = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);
  
    /* EMPEI_I */
    soft_reset = enet_field_EMPEI120_I_SW_RST_get(NULL, hndl);
    low_pwr = enet_field_EMPEI120_I_LOWPWR_get(NULL, hndl);
    ram_low_pwr = enet_field_EMPEI120_I_RAM_LOWPWR_get(NULL, hndl);
    enet_energy_state->empei_i = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

  

    PMC_RETURN();
} /* enet_energy_state_reg_get */

/*******************************************************************************
* enet_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a enet_recover_var_t context and energy state from register space.
*
*  This function is used by enet_handle_restart_init. It also recovers 
*  volatile information from enet_var_t context.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*   *enet_handle          - pointer to ENET handle instance to be operated on
*   enet_type             - ENET SS type: LINe or SYS   
*   top_energy_state_reg  - energy state from top-level
*   
*
* OUTPUTS:
*   *enet_var_reg          - A partial enet_var_t context + additional
*                              information to facilite audit phase
*   *enet_energy_state_reg - ENET energy states 
*
* RETURNS:
*  PMC_SUCCESS: 
*   recovery is a success 
*  ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT: 
*   Communal ressource energy states are incoherent. top will have
*    to shutdown the subsystem
*  ENET_ERR_CONTEXT_REG_INCOHERENT:
*   Register conetxt is incoherent. top will have
*    to shutdown the subsystem
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_ctxt_reg_recover(enet_handle_t        *enet_handle,
                                        enet_type_id_t        enet_type,
                                        pmc_energy_state_t    top_energy_state_reg,         
                                        enet_recover_var_t   *enet_var_reg,              
                                        enet_energy_state_t  *enet_energy_state_reg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr; 
    UINT32 link_itr; 
    UINT32 incr;
    scbs3_handle_t *mpma_scbs3_handle;
    scbs3_handle_t *mpmo_scbs3_handle;   
    BOOL8 epmm_rx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 epmm_tx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 emac_rx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 emac_tx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 etime_rx_actv;
    BOOL8 etime_tx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 etrans_rx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 etrans_tx_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 empei_i_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 empei_e_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 eclass_i_actv_link[ENET_NUM_LINK_MAX];
    BOOL8 eclass_e_actv_link[ENET_NUM_LINK_MAX];
    epmm_data_rate_t epmm_rx_rate[ENET_NUM_LINK_MAX];
    epmm_data_rate_t epmm_tx_rate[ENET_NUM_LINK_MAX];
    etrans_data_rate_t etrans_rx_rate[ENET_NUM_LINK_MAX];
    etrans_data_rate_t etrans_tx_rate[ENET_NUM_LINK_MAX];
    BOOL8 already_proc;
    emac_rx_data_src_t emac_rx_data_src;
    emac_tx_data_dst_t emac_tx_data_dst;

    PMC_ENTRY();


    /* GET SCBS3 handle */
    mpma_scbs3_handle   = mpma_scbs3_handle_get(enet_handle->epmm_handle->mpma_handle);
    mpmo_scbs3_handle   = mpmo_scbs3_handle_get(enet_handle->epmm_handle->mpmo_handle);
    
    /* 
       create a register context 
    */
    /* start from a clean context */
    enet_var_default_init(&enet_var_reg->var); 
    
    /* this information is coming from top-level register */
    enet_var_reg->var.enet_start_state = (top_energy_state_reg == PMC_ENERGY_STATE_RESET);
    
    PMC_MEMSET(empei_i_actv_link,0,sizeof(empei_i_actv_link));
    PMC_MEMSET(empei_e_actv_link,0,sizeof(empei_e_actv_link));

    /* 
       IF start_state is TRUE, the susbystem is completly in power down
       enet_register_var is completed (anyway, digi-layer should not call use with PMC_ENERGY_STATE_RESET)
    */
    if (FALSE == enet_var_reg->var.enet_start_state)
    {
        /* REGISTER:: ENET SS is not in power down */
        
        /* get energy state from enet-top register */
        enet_energy_state_reg_get(enet_handle, 
                                  enet_energy_state_reg);
                
        /* 
           check power state from communal blocks
        */
        if (PMC_SUCCESS == rc)
        {
            /* check EMAC state */
            enet_var_reg->has_emac_link = (enet_energy_state_reg->emac == PMC_ENERGY_STATE_RUNNING);

            /* check ETRANS state */
            enet_var_reg->has_etrans_link = (enet_energy_state_reg->etrans == PMC_ENERGY_STATE_RUNNING);

            /* check EPMM state */
            enet_var_reg->has_epmm_link = (enet_energy_state_reg->epmm == PMC_ENERGY_STATE_RUNNING);

            /* Check EMPEI_I state  */
            if (PMC_SUCCESS == rc)
            {
                enet_var_reg->has_empei_i_link = (enet_energy_state_reg->empei_i == PMC_ENERGY_STATE_RUNNING);
            }
            else
            {
                if (TRUE == enet_var_reg->has_emac_link || enet_var_reg->has_epmm_link)
                {                   
                    enet_var_reg->has_empei_i_link = (enet_energy_state_reg->empei_i == PMC_ENERGY_STATE_RUNNING);
                }
            }

            /* Check EMPEI_E state  */
            if (PMC_SUCCESS == rc &&
                enet_energy_state_reg->empei_e == PMC_ENERGY_STATE_RUNNING)
            {
                enet_var_reg->has_empei_e_link = (enet_energy_state_reg->empei_e == PMC_ENERGY_STATE_RUNNING);
            }
            else
            {
                if (TRUE == enet_var_reg->has_emac_link || TRUE == enet_var_reg->has_epmm_link)
                {
                   
                    enet_var_reg->has_empei_e_link = TRUE;
                }
            }

            /* empei_i cannot be out of reset alone */
            if (PMC_SUCCESS == rc &&
                ((TRUE == enet_var_reg->has_emac_link || TRUE == enet_var_reg->has_epmm_link) &&
                 (FALSE == enet_var_reg->has_empei_i_link || FALSE == enet_var_reg->has_empei_e_link)))
            {
                rc = ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
            }


            if (PMC_SUCCESS == rc &&
                ENET_SYS == enet_type &&
                TRUE == enet_var_reg->has_etrans_link)
            {
                rc = ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
            }
            /* when shared ressources are enabled, at least 1 channel based ressources shall be enabled */
            if (PMC_SUCCESS == rc &&
                FALSE == enet_var_reg->has_emac_link && 
                FALSE == enet_var_reg->has_etrans_link &&
                FALSE == enet_var_reg->has_epmm_link &&
                FALSE == enet_var_reg->has_empei_i_link &&
                FALSE == enet_var_reg->has_empei_e_link)
            {
                rc = ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
            } 
            else 
            {
                /* initialize variables  */
                PMC_MEMSET(&epmm_rx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&epmm_tx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&emac_rx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&emac_tx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&etrans_rx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&etrans_tx_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&eclass_i_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                PMC_MEMSET(&eclass_e_actv_link[0],0,sizeof(BOOL8)*ENET_NUM_LINK_MAX);
                

                for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                {
                    enet_var_reg->emac_link_rate[itr] = LAST_EMAC_DATA_RATE;
                    enet_var_reg->etrans_rx_link_datapath[itr] = ETRANS_UNUSED;
                    enet_var_reg->etrans_tx_link_datapath[itr] = ETRANS_UNUSED;
                    enet_var_reg->epmm_rx_link_datapath[itr] = EPMM_DATAPATH_UNUSED;
                    enet_var_reg->epmm_tx_link_datapath[itr] = EPMM_DATAPATH_UNUSED;
                }

                /* 
                   get channel information 
                   - channel status: activated or off
                   - channel rate
                */
                /* retrieve EMAC status */
                if (TRUE == enet_var_reg->has_emac_link)
                {
                    emac_activated_link_get(enet_handle->emac_handle,
                                            &emac_rx_actv_link[0], 
                                            &emac_tx_actv_link[0]);
                    emac_link_rate_get(enet_handle->emac_handle,
                                       &enet_var_reg->emac_link_rate[0]);

                    etime_activated_link_get(enet_handle->emac_handle->etime_handle,
                                             &etime_rx_actv,
                                             &etime_tx_actv_link[0]);
                    /* we could interpolate ETIME  RX and TX data rate */

                    for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                    {
                        if (etime_rx_actv)
                        {
                            enet_var_reg->var.lnk_ctxt[itr].ing_ts_en = etime_rx_actv;
                        }
                        enet_var_reg->var.lnk_ctxt[itr].egr_ts_en = etime_tx_actv_link[itr];
                    }
                }
                /* retrieve EPMM status */
                if (TRUE == enet_var_reg->has_epmm_link)
                {
                    epmm_activated_link_get(enet_handle->epmm_handle,
                                            &epmm_rx_actv_link[0], 
                                            &epmm_tx_actv_link[0]);
                    epmm_rx_link_rate_get(enet_handle->epmm_handle,
                                          &enet_var_reg->epmm_rx_link_datapath[0],
                                          &epmm_rx_rate[0]);
                    epmm_tx_link_rate_get(enet_handle->epmm_handle,
                                          &enet_var_reg->epmm_tx_link_datapath[0],
                                          &epmm_tx_rate[0]);
                }

                /* retrieve ETRANS status */
                if (TRUE == enet_var_reg->has_etrans_link)
                {
                    etrans_activated_link_get(enet_handle->etrans_handle,
                                              &etrans_rx_actv_link[0], 
                                              &etrans_tx_actv_link[0]);
                    etrans_rx_link_rate_get(enet_handle->etrans_handle,
                                            &enet_var_reg->etrans_rx_link_datapath[0],
                                            &etrans_rx_rate[0]);
                    etrans_tx_link_rate_get(enet_handle->etrans_handle,
                                            &enet_var_reg->etrans_tx_link_datapath[0],
                                            &etrans_tx_rate[0]);

                    for (itr = 0; itr < ENET_NUM_LINK_MAX && PMC_SUCCESS == rc; itr++)
                    {
                        if (ENET_SYS == enet_type &&
                            (TRUE == etrans_rx_actv_link[itr] ||
                             TRUE == etrans_tx_actv_link[itr] ||
                             LAST_ETRANS_DATA_RATE != etrans_rx_rate[itr] ||
                             LAST_ETRANS_DATA_RATE != etrans_tx_rate[itr]))
                        {
                            rc = ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;  
                        }
                    }
                }

                /* retrieve EMPEI_I status */
                if (PMC_SUCCESS == rc &&
                    TRUE == enet_var_reg->has_empei_i_link)
                {
                    empei_i_activated_link_get(enet_handle->empei_i_handle,
                                               &empei_i_actv_link[0]);

                    for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                    {
                        if (TRUE == empei_i_actv_link[itr])
                        {
                            eclass_activated_link_get(enet_handle->empei_i_handle->eclass_handle,
                                                      &eclass_i_actv_link[0]);
                            break;
                        }
                    }
                }

                /* retrieve EMPEI_E status */
                if (PMC_SUCCESS == rc &&
                    TRUE == enet_var_reg->has_empei_e_link)
                {
                    empei_e_activated_link_get(enet_handle->empei_e_handle,
                                               &empei_e_actv_link[0]);
                    for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                    {
                        if (TRUE == empei_e_actv_link[itr])
                        {
                            eclass_activated_link_get(enet_handle->empei_e_handle->eclass_handle,
                                                      &eclass_e_actv_link[0]);
                            break;
                        }
                    }
                }
            }
            
            /* careful: empei_i and empei_e have a register GFP_EN that allows to know link datapath */
            enet_var_default_init(&enet_var_reg->var);            

            /* populate linknel context */
            for (link_itr = 0; link_itr  < ENET_NUM_LINK_MAX && PMC_SUCCESS == rc; link_itr += incr)
            {
                incr = 1;
                already_proc = FALSE;
                /* check: MAX, TRANSPARENT, MAC_PMON, TRANSPARENT_PMON, [GFP, GMP]_MAC_PMON */
                /* transparent mode */
                if (EPMM_DATAPATH_UNUSED != enet_var_reg->epmm_rx_link_datapath[link_itr] &&
                    EPMM_DATAPATH_UNUSED != enet_var_reg->epmm_tx_link_datapath[link_itr] && 
                    LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr] &&
                    FALSE == already_proc)
                {
                    PMC_LOG_TRACE("trying to interpolate datapath using EPMM block - epmm activated\n");

                    if (((ETRANS_UNUSED != enet_var_reg->etrans_rx_link_datapath[link_itr] && TRUE == empei_i_actv_link[link_itr]) ||
                         (ETRANS_UNUSED != enet_var_reg->etrans_tx_link_datapath[link_itr] && TRUE == empei_e_actv_link[link_itr])))
                    {
                        /*     
                               ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
                               ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
                               ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
                               ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON: 
                               ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
                               ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
                               ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON: */

                        already_proc = TRUE;

                        PMC_LOG_TRACE("trying to interpolate ENET_MAPOTN_GMP_G709_100GE_MAC_PMON\n");
                        /* EPMM_UNUSED_[10,40,100]G should be used to kwon if we are in 
                           ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON */
                        if (ETRANS_UNUSED == enet_var_reg->etrans_rx_link_datapath[link_itr] && TRUE != empei_i_actv_link[link_itr])
                        {
                            enet_var_reg->etrans_rx_link_datapath[link_itr] = enet_var_reg->etrans_tx_link_datapath[link_itr];
                        }
                        switch(enet_var_reg->etrans_rx_link_datapath[link_itr])
                        {
                        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON;
                            break;
                        case ETRANS_GMP_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_100GE_MAC_PMON;
                            break;
                        case ETRANS_GMP_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_40GE_MAC_PMON;
                            break;
                        case ETRANS_BMP_GSUP43_C71_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON;
                            break;
                        default:
                            enet_var_reg->var.ch_datapath[link_itr] = LAST_ENET_DATAPATH;
                            break;
                        }    
                    }
                    else if (ETRANS_UNUSED == enet_var_reg->etrans_rx_link_datapath[link_itr] &&
                             ETRANS_UNUSED == enet_var_reg->etrans_tx_link_datapath[link_itr] )
                    {
                        /*     case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                               case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                               case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                               case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: */
                        
                        /* if empei_i active then it is a ENET_SERDES_[10, 40,100]GE_TRANSPARENT_TX_PMON datapath */
                        if (TRUE == empei_i_actv_link[link_itr] &&
                            TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, FALSE, LAST_EMAC_DATA_RATE)&&
                            FALSE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_ING, FALSE, LAST_EMAC_DATA_RATE) &&
                            FALSE == already_proc)
                        {                            

                            emac_rx_data_src = emac_link_rx_data_src_get(enet_handle->emac_handle, link_itr);
                            if (enet_convert_datapath_to_emac_rx_data_src(ENET_SLAVE_100GE_TRANSPARENT_TX_PMON) == emac_rx_data_src ||
                                enet_convert_datapath_to_emac_rx_data_src(ENET_SLAVE_40GE_TRANSPARENT_TX_PMON) ==  emac_rx_data_src||
                                enet_convert_datapath_to_emac_rx_data_src(ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON) == emac_rx_data_src ||
                                enet_convert_datapath_to_emac_rx_data_src(ENET_SLAVE_10GE_TRANSPARENT_TX_PMON) == emac_rx_data_src)
                            {                           
                              
                                already_proc = TRUE;
                                PMC_LOG_TRACE("trying to interpolate ENET_SLAVE_100GE_TRANSPARENT_TX_PMON\n");
                                /*     case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                                       case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                                       case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                                       case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON: */
                                switch(enet_var_reg->epmm_rx_link_datapath[link_itr])
                                {
                                case EPMM_10GE_GSUP43_73:
                                    enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON;
                                    break;
                                case EPMM_10GE_CBR:
                                    enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_10GE_TRANSPARENT_TX_PMON;
                                    break;
                                case EPMM_40GE:
                                    if (TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_40G))
                                    {
                                        /* check that all PMA lane are in low power mode */
                                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_40GE_TRANSPARENT_TX_PMON;
                                    }
                                    break;
                                case EPMM_100GE:
                                    if (TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_100G))
                                    {
                                        /* check that all PMA lane are in low power mode */
                                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_100GE_TRANSPARENT_TX_PMON;
                                    }
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                        
                        PMC_LOG_TRACE("empei_e_actv_link[link_itr] = %d  !empei_i_actv_link[link_itr]= %d EMAC_EGR_LOWPWR=%d !EMAC_ING_LOWPWR = %d enet_var_reg->emac_link_rate[link_itr] = %d already_proc = %d\n",
                                      empei_e_actv_link[link_itr], empei_i_actv_link[link_itr],
                                      emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, FALSE, LAST_EMAC_DATA_RATE),
                                      emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_ING, FALSE, LAST_EMAC_DATA_RATE),
                                      enet_var_reg->emac_link_rate[link_itr],
                                      already_proc);
                        if (TRUE == empei_e_actv_link[link_itr] &&
                            TRUE == empei_i_actv_link[link_itr] &&
                            TRUE == emac_rx_actv_link[link_itr] &&
                            (TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_10G) || 
                             TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_40G) ) &&
                            FALSE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_ING, FALSE, LAST_EMAC_DATA_RATE) &&
                            (LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr]) &&
                            FALSE == already_proc)
                        {
                            PMC_LOG_TRACE("empei_e_actv_link[link_itr]  !empei_i_actv_link[link_itr] EMAC_EGR_LOWPWR !EMAC_ING_LOWPWR\n");
                            emac_rx_data_src = emac_link_rx_data_src_get(enet_handle->emac_handle, link_itr);
                            emac_tx_data_dst = emac_link_tx_data_dst_get(enet_handle->emac_handle, link_itr);
                            if ((enet_convert_datapath_to_emac_rx_data_src(ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) == emac_rx_data_src ||
                                 enet_convert_datapath_to_emac_rx_data_src(ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ==  emac_rx_data_src||
                                 enet_convert_datapath_to_emac_rx_data_src(ENET_SERDES_40GE_TRANSPARENT_RX_PMON) == emac_rx_data_src ||
                                 enet_convert_datapath_to_emac_rx_data_src(ENET_SERDES_10GE_TRANSPARENT_RX_PMON) == emac_rx_data_src) &&
                                (enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) == emac_tx_data_dst ||
                                 enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ==  emac_tx_data_dst||
                                 enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_40GE_TRANSPARENT_RX_PMON) == emac_tx_data_dst ||
                                 enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_10GE_TRANSPARENT_RX_PMON) == emac_tx_data_dst))
                            {                
                                /*    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
                                      case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
                                      case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
                                      case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:*/           
                                already_proc = TRUE;
                                PMC_LOG_TRACE("trying to interpolate ENET_SERDES_100GE_TRANSPARENT_RX_PMON : emac_rate = %d\n", enet_var_reg->emac_link_rate[link_itr]);
                                switch(enet_var_reg->epmm_rx_link_datapath[link_itr])
                                {
                                case EPMM_10GE_GSUP43_73:
                                    enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
                                    break;
                                case EPMM_10GE_CBR:
                                    enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                                    break;
                                case EPMM_40GE:
                                    if (TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_40G))
                                    {
                                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                                    }
                                    break;
                                case EPMM_100GE:
                                    if (TRUE == emac_link_lowpwr_get(enet_handle->emac_handle, link_itr, EMAC_DIR_EGR, TRUE, EMAC_LINK_100G))
                                    {                                    
                                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
                                    }
                                    break;
                                default:
                                    break;
                                }                         
                            }
                        }
                        if (TRUE == already_proc)
                        {
                            if (TRUE == epmm_rx_actv_link[link_itr])
                            {
                                enet_var_reg->var.lnk_ctxt[link_itr].rx_state = ENET_LNK_OPERATIONAL;
                            }
                            if (TRUE == epmm_tx_actv_link[link_itr])
                            {
                                enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                            }
                        }
                    }
                }
                
                PMC_LOG_TRACE("enet_var_reg->emac_link_rate[link_itr] = %d %d %d %d %d %d\n", enet_var_reg->emac_link_rate[link_itr],
                              epmm_rx_actv_link[link_itr],
                              epmm_tx_actv_link[link_itr],
                              epmm_rx_actv_link[link_itr],
                              epmm_tx_actv_link[link_itr],
                              already_proc);
                emac_tx_data_dst = emac_link_tx_data_dst_get(enet_handle->emac_handle, link_itr);
                if ((EPMM_DATAPATH_UNUSED != enet_var_reg->epmm_rx_link_datapath[link_itr] ||
                     EPMM_DATAPATH_UNUSED != enet_var_reg->epmm_tx_link_datapath[link_itr]) &&
                    (LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr]) &&
                    FALSE == emac_rx_actv_link[link_itr] &&
                    FALSE == already_proc)
                {

                    /*    case ENET_SERDES_100GE_TRANSPARENT:
                          case ENET_SERDES_40GE_TRANSPARENT:
                          case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
                          case ENET_SERDES_10GE_TRANSPARENT_RX:*/
                    
                    PMC_LOG_TRACE("emac_tx_data_dst = %d -> %d/%d\n", emac_tx_data_dst, empei_i_actv_link[link_itr], empei_e_actv_link[link_itr]);
                    if (enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT) == emac_tx_data_dst ||
                        enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_100GE_TRANSPARENT) ==  emac_tx_data_dst ||
                        enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_40GE_TRANSPARENT) == emac_tx_data_dst ||
                        enet_convert_datapath_to_emac_tx_data_dst(ENET_SERDES_10GE_TRANSPARENT) == emac_tx_data_dst)
                    {                           
                        PMC_LOG_TRACE("trying to interpolate ENET_SERDES_XXXGE_TRANSPARENT link = %d %d %d\n", link_itr, empei_i_actv_link[link_itr], empei_e_actv_link[link_itr]);
                        /* ENET_SERDES_[10,40,100]GE_TRANSPARENT */
                        if (TRUE == epmm_rx_actv_link[link_itr] && TRUE == empei_i_actv_link[link_itr])
                        {
                            enet_var_reg->var.lnk_ctxt[link_itr].rx_state = ENET_LNK_OPERATIONAL;
                            already_proc = TRUE;
                        }
                        if (TRUE == epmm_tx_actv_link[link_itr] && TRUE == empei_e_actv_link[link_itr])
                        {
                            enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                            already_proc = TRUE;
                        }
                        if (EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link_itr])
                        {
                            enet_var_reg->epmm_rx_link_datapath[link_itr] = enet_var_reg->epmm_tx_link_datapath[link_itr];
                        }
                        switch(enet_var_reg->epmm_rx_link_datapath[link_itr])
                        {
                        case EPMM_10GE_GSUP43_73:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT;
                            break;
                        case EPMM_10GE_CBR:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_10GE_TRANSPARENT;
                            break;
                        case EPMM_40GE:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_40GE_TRANSPARENT;
                            break;
                        case EPMM_100GE:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_100GE_TRANSPARENT;
                            break;
                        default:
                            break;
                        }                      
                    }
                }

                if (LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr] &&
                    (TRUE == emac_rx_actv_link[link_itr] || TRUE == emac_tx_actv_link[link_itr]) &&
                    EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link_itr] &&    
                    ETRANS_UNUSED == enet_var_reg->etrans_rx_link_datapath[link_itr] &&
                    FALSE == already_proc)
/* TRUE == emac_ingr_data_sink_get(enet_handle->emac_handle, link_itr) && */
                {
                    /*
                     *   ENET_SLAVE_100GE_MAC_TX_PMON              - 100GE Tx MAC traffic looped back to Rx MAC for PMON only
                     *   ENET_SLAVE_40GE_MAC_TX_PMON               - 40GE Tx MAC traffic looped back to Rx MAC for PMON only
                     *   ENET_SLAVE_10GE_MAC_TX_PMON               - 10GE Tx MAC traffic looped back to Rx MAC for PMON only
                     */
                    
                    already_proc = TRUE;
                    if (TRUE == empei_i_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].rx_state = ENET_LNK_OPERATIONAL;
                    }
                    if (TRUE == empei_e_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                    }
                    switch(enet_var_reg->emac_link_rate[link_itr])
                    {
                    case EMAC_LINK_10G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_10GE_MAC_TX_PMON;
                        break;
                    case EMAC_LINK_40G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_40GE_MAC_TX_PMON;
                        break;
                    case EMAC_LINK_100G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SLAVE_100GE_MAC_TX_PMON;
                        break;
                    default:
                        break;
                    } 
                }

                if (LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr] &&
                    ((TRUE == emac_rx_actv_link[link_itr] && ETRANS_UNUSED != enet_var_reg->etrans_rx_link_datapath[link_itr]) || 
                     (TRUE == emac_tx_actv_link[link_itr] && ETRANS_UNUSED != enet_var_reg->etrans_tx_link_datapath[link_itr])) &&
                    (EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link_itr] &&
                     EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_tx_link_datapath[link_itr]) &&
                    FALSE == already_proc)
                {

                    /* ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
                       ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
                       ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
                       ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
                       ENET_MAPOTN_GMP_G709_100GE_MAC:
                       ENET_MAPOTN_GMP_G709_40GE_MAC:
                       ENET_MAPOTN_BMP_GSUP43_10GE_MAC:*/
                    PMC_LOG_TRACE("trying to interpolate ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC\n");
                    
                    already_proc = TRUE;   
                    if (TRUE == etrans_rx_actv_link[link_itr] && TRUE == empei_i_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].rx_state = ENET_LNK_OPERATIONAL;
                    }
                    if (TRUE == etrans_tx_actv_link[link_itr] && TRUE == empei_e_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                    }
                    if (TRUE != emac_rx_actv_link[link_itr])
                    {
                        enet_var_reg->etrans_rx_link_datapath[link_itr] = enet_var_reg->etrans_tx_link_datapath[link_itr];
                    }

                    if (enet_var_reg->var.lnk_ctxt[link_itr].tx_state != ENET_LNK_OPERATIONAL)
                    {
                        /* it is the PMON case where the egress scheduler is not enabled */

                        enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                        switch(enet_var_reg->etrans_rx_link_datapath[link_itr])
                        {
                        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON;
                            break;
                        case ETRANS_GFP_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON;
                            break;
                        case ETRANS_GMP_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_100GE_MAC_PMON;
                            break;
                        case ETRANS_GMP_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_40GE_MAC_PMON;
                            break;
                        case ETRANS_BMP_GSUP43_C71_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON;
                            break;
                        default:
                            enet_var_reg->var.ch_datapath[link_itr] = LAST_ENET_DATAPATH;
                            break;
                        }                        
                    } else 
                    {

                        switch(enet_var_reg->etrans_rx_link_datapath[link_itr])
                        {
                        case ETRANS_GFP_GSUP43_C62_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC;
                            break;
                        case ETRANS_GFP_GSUP43_C62_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC;
                            break;
                        case ETRANS_GFP_GSUP43_C62_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC;
                            break;
                        case ETRANS_GFP_GSUP43_C73_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC;
                            break;
                        case ETRANS_GMP_G709_100GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_100GE_MAC;
                            break;
                        case ETRANS_GMP_G709_40GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_GMP_G709_40GE_MAC;
                            break;
                        case ETRANS_BMP_GSUP43_C71_10GE_MAC:
                            enet_var_reg->var.ch_datapath[link_itr] = ENET_MAPOTN_BMP_GSUP43_10GE_MAC;
                            break;
                        default:
                            enet_var_reg->var.ch_datapath[link_itr] = LAST_ENET_DATAPATH;
                            break;
                        }
                    }                  
                }

                if (LAST_EMAC_DATA_RATE != enet_var_reg->emac_link_rate[link_itr] &&
                    (TRUE == emac_rx_actv_link[link_itr] || TRUE == emac_tx_actv_link[link_itr]) &&
                    ((EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link_itr] ||
                      EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_tx_link_datapath[link_itr])) &&
                    FALSE == already_proc)
                {
                    /* ENET_SERDES_10GE_MAC
                       ENET_SERDES_40GE_MAC
                       ENET_SERDES_100GE_MAC */
                                        
                    PMC_LOG_TRACE("trying to interpolate ENET_SERDES_XXXGE_MAC\n");
                    already_proc = TRUE;   
                    if (TRUE == empei_i_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].rx_state = ENET_LNK_OPERATIONAL;
                    }
                    if (TRUE == empei_e_actv_link[link_itr])
                    {
                        enet_var_reg->var.lnk_ctxt[link_itr].tx_state = ENET_LNK_OPERATIONAL;
                    }
                    switch(enet_var_reg->emac_link_rate[link_itr])
                    {
                    case EMAC_LINK_10G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_10GE_MAC;
                        break;
                    case EMAC_LINK_40G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_40GE_MAC;
                        break;
                    case EMAC_LINK_100G:
                        enet_var_reg->var.ch_datapath[link_itr] = ENET_SERDES_100GE_MAC;
                        break;
                    default:
                        break;
                    }     
                }                

                /* calculate EMPEI_I and EMPEI_E dpi scheduler calendar slots */
                if (TRUE == already_proc)
                {
                    /* recover EMPEI DPI SLOT */
                    PMC_LOG_TRACE("about to set dpi_slot link = %d\n", link_itr);
                    enet_empei_link_dpi_sch_slot_get(link_itr, enet_var_reg->var.ch_datapath[link_itr], enet_var_reg->var.dpi_sch_slot);
                }
                
                /* fulfill ECLASS state */
                if (TRUE == eclass_i_actv_link[link_itr])
                {
                    enet_var_reg->var.lnk_ctxt[link_itr].empei_i_eclass_en = TRUE;
                }

                if (TRUE == eclass_e_actv_link[link_itr])
                {
                    enet_var_reg->var.lnk_ctxt[link_itr].empei_e_eclass_en = TRUE;
                }
                

                /* if datapath is 40G or 100G fulfill following 3 or 9 links*/
                if (LAST_ENET_LINK_DATA_RATE != enet_convert_datapath_to_enet_data_rate(enet_var_reg->var.ch_datapath[link_itr]))
                {
                    switch(enet_convert_datapath_to_enet_data_rate(enet_var_reg->var.ch_datapath[link_itr]))
                    {
                    case ENET_LINK_40G: 
                        if ((TRUE == empei_i_actv_link[link_itr] ||
                             TRUE == empei_e_actv_link[link_itr]) ||
                            (TRUE == emac_rx_actv_link[link_itr] ||
                             TRUE == emac_tx_actv_link[link_itr]) ||
                            (TRUE == epmm_rx_actv_link[link_itr] ||
                             TRUE == epmm_tx_actv_link[link_itr]))
                        {
                            for (itr = 1; itr < ENET_NUM_LINK_40GE; itr++)
                            {
                                
                                enet_var_reg->var.lnk_ctxt[itr + link_itr].rx_state = (empei_i_actv_link[itr + link_itr] == TRUE) ? ENET_LNK_OPERATIONAL : ENET_LNK_START;
                                enet_var_reg->var.lnk_ctxt[itr + link_itr].tx_state = (empei_e_actv_link[itr + link_itr] == TRUE) ? ENET_LNK_OPERATIONAL : ENET_LNK_START;
                                enet_var_reg->var.ch_datapath[itr + link_itr] = enet_var_reg->var.ch_datapath[link_itr];
                                /* retrieve ECLASS state */
                                if (TRUE == eclass_i_actv_link[itr + link_itr])
                                {
                                    enet_var_reg->var.lnk_ctxt[itr + link_itr].empei_i_eclass_en = TRUE;
                                }
                                if (TRUE == eclass_e_actv_link[itr + link_itr])
                                {
                                    enet_var_reg->var.lnk_ctxt[itr + link_itr].empei_e_eclass_en = TRUE;
                                }
                            }
                        }
                        incr = ENET_NUM_LINK_40GE;
                        break;

                    case ENET_LINK_100G:
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    /* retrieve EPMM MPMA SCBS3 programmed calendar */        
    if (PMC_SUCCESS == rc)
    {
        if (TRUE == enet_var_reg->has_epmm_link)
        {
            scbs3_page_get(mpma_scbs3_handle,SCBS3_ACTIVE_MEM, &enet_var_reg->epmm_mpma_scbs3_page);

            PMC_LOG_TRACE("EPMM MPMA SCBS3 page retrieved\n");
        }
        if (TRUE == enet_var_reg->has_epmm_link)
        {
            scbs3_page_get(mpmo_scbs3_handle,SCBS3_ACTIVE_MEM, &enet_var_reg->epmm_mpmo_scbs3_page);
        }
    }

    PMC_RETURN(rc);
} /* enet_ctxt_reg_recover */

/*******************************************************************************
* enet_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Audit memory context with recovered register context. This function 
*   determines if a cleanup is required
*   
*   A clean context which represents the intersection between both context is also 
*   generated.
*
* INPUTS:
*   *enet_handle        - pointer to ENET handle instance to be operated on
*   *enet_var_reg       - A partial enet_var_t context + additional
*                           information to facilite audit phase
*   *enet_energy_state_reg - ENET energy states 
*
* OUTPUTS:
*   *enet_var_clean     - A clean context which represents the intersection
*                           between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS:
*    Memory & register context matches
*   ENET_ERR_CONTEXT_CLEANUP:
*    A cleanup is required.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_ctxt_audit(enet_handle_t        *enet_handle,
                                  enet_recover_var_t   *enet_var_reg,
                                  enet_energy_state_t  *enet_energy_state_reg,
                                  enet_var_t           *enet_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 link_itr;
    UINT32 itr;
    UINT32 start_itr;
    UINT32 incr;
    UINT32 slot_idx;
    PMC_ERROR loop_rc;
    UINT32               num_dpi_slot_entries = 0;
    UINT32               num_cal_entries = 0;
    PMC_ENTRY();
    
    /* init the new context  */
    enet_var_default_init(enet_var_clean);
    
    /* AUDIT POWER */
    
    PMC_LOG_TRACE("in %s -> step 0-1 - %d %d \n", __FUNCTION__, (UINT8)enet_var_reg->var.enet_start_state, (UINT8) enet_start_state_test(enet_handle));
    if (enet_var_reg->var.enet_start_state != enet_start_state_test(enet_handle))
    {       
        enet_var_clean->enet_start_state = TRUE;
        rc = ENET_ERR_CONTEXT_CLEANUP;
        PMC_LOG_TRACE("in %s -> step 0-0 - rc = %s %d %d\n", __FUNCTION__, pmc_log_string_get(rc), enet_var_reg->var.enet_start_state,  enet_start_state_test(enet_handle));
    } 
    else if (enet_var_reg->var.enet_start_state == FALSE) 
    {        
        PMC_LOG_TRACE("in %s -> step 0-1 - rc = %s\n", __FUNCTION__, pmc_log_string_get(rc));
        /* not sure */
        /* we are out-of-reset, now we check communal resources */
        if ((enet_energy_state_reg->emac    == PMC_ENERGY_STATE_RUNNING && emac_start_state_test(enet_handle->emac_handle) == FALSE) ||
            (enet_energy_state_reg->epmm  == PMC_ENERGY_STATE_RUNNING && epmm_start_state_test(enet_handle->epmm_handle) == FALSE) ||            
            (enet_energy_state_reg->etrans  == PMC_ENERGY_STATE_RUNNING && etrans_start_state_test(enet_handle->etrans_handle) == FALSE))
        {
            enet_var_clean->enet_start_state = enet_var_reg->var.enet_start_state;
        } 
        else 
        {
            enet_var_clean->enet_start_state = TRUE;
            rc = ENET_ERR_CONTEXT_CLEANUP;
            PMC_LOG_TRACE("in %s -> step 0-2 - rc = %s\n", __FUNCTION__, pmc_log_string_get(rc));
        }

        /* Channel context audit */
        for (link_itr = 0; link_itr  < ENET_NUM_LINK_MAX; link_itr += incr)
        {
            loop_rc = PMC_SUCCESS;
            incr = 1;
            /* 
               We only consider channel that are both in OPERATIONAL mode
               then we accept or reject the memory context based on information collected from 
               register space
            */
            
            /* AUDIT channels datapath type */            
            if (PMC_SUCCESS == loop_rc &&
                ((enet_var_reg->var.ch_datapath[link_itr] != enet_handle->var.ch_datapath[link_itr]) &&
                 (FALSE == ((ENET_LINK_100G == enet_convert_datapath_to_enet_data_rate(enet_var_reg->var.ch_datapath[link_itr])) && (ENET_LINK_100G == enet_convert_datapath_to_enet_data_rate(enet_handle->var.ch_datapath[link_itr]))))))
            {
                if (FALSE == ((ENET_SERDES_100GE_MAC == enet_handle->var.ch_datapath[link_itr] &&
                               ENET_SLAVE_100GE_MAC_TX_PMON == enet_var_reg->var.ch_datapath[link_itr]) ||
                              (ENET_SERDES_40GE_MAC == enet_handle->var.ch_datapath[link_itr] &&
                               ENET_SLAVE_40GE_MAC_TX_PMON == enet_var_reg->var.ch_datapath[link_itr]) ||
                              (ENET_SERDES_10GE_MAC == enet_handle->var.ch_datapath[link_itr] &&
                               ENET_SLAVE_10GE_MAC_TX_PMON == enet_var_reg->var.ch_datapath[link_itr]) ||
                              enet_check_rx_epmm_state(enet_handle,
                                                       enet_var_reg,
                                                       link_itr)))
                {                
                    loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                    PMC_LOG_TRACE("in %s -> step 2 - loop_rc = %s %d %d\n", __FUNCTION__, pmc_log_string_get(loop_rc), (UINT32)enet_var_reg->var.ch_datapath[link_itr], (UINT32)enet_handle->var.ch_datapath[link_itr]  );
                }
                else
                {
                    enet_var_clean->ch_datapath[link_itr] =  enet_handle->var.ch_datapath[link_itr];
                }
            } 
            else
            {
                enet_var_clean->ch_datapath[link_itr] =  enet_handle->var.ch_datapath[link_itr];
            }

            /* AUDIT RX channels */            
            PMC_LOG_TRACE("in %s -> step 4-0 - loop_rc = %s -> link = %d, %d/%d\n", __FUNCTION__, pmc_log_string_get(loop_rc), link_itr, enet_var_reg->var.lnk_ctxt[link_itr].rx_state, enet_handle->var.lnk_ctxt[link_itr].rx_state); 

            if (enet_var_reg->var.lnk_ctxt[link_itr].rx_state == enet_handle->var.lnk_ctxt[link_itr].rx_state) 
            {       
                enet_var_clean->lnk_ctxt[link_itr].rx_state = enet_handle->var.lnk_ctxt[link_itr].rx_state;
            }  else if (enet_handle->var.lnk_ctxt[link_itr].rx_state == ENET_LNK_EQUIPPED && 
                        enet_var_reg->var.lnk_ctxt[link_itr].rx_state == ENET_LNK_START)
            {
                enet_var_clean->lnk_ctxt[link_itr].rx_state = enet_handle->var.lnk_ctxt[link_itr].rx_state;
            }
            else
            {
                if (TRUE == enet_is_pmon_datapath_get(enet_var_clean->ch_datapath[link_itr]))
                {
                    enet_var_clean->lnk_ctxt[link_itr].rx_state = enet_handle->var.lnk_ctxt[link_itr].rx_state;
                }
                else if (PMC_SUCCESS == loop_rc)
                {
                    loop_rc = ENET_ERR_CONTEXT_CLEANUP;  
                    PMC_LOG_TRACE("in %s -> step 4 - loop_rc = %s\n", __FUNCTION__, pmc_log_string_get(loop_rc));              
                } 
            }


            /* AUDIT TX channels */
            
            PMC_LOG_TRACE("in %s -> step 5-0 - link = %d loop_rc = %s -> %d/%d\n", __FUNCTION__, link_itr, pmc_log_string_get(loop_rc), enet_var_reg->var.lnk_ctxt[link_itr].tx_state, enet_handle->var.lnk_ctxt[link_itr].tx_state);       
            if (enet_var_reg->var.lnk_ctxt[link_itr].tx_state == enet_handle->var.lnk_ctxt[link_itr].tx_state) 
            {       
                enet_var_clean->lnk_ctxt[link_itr].tx_state = enet_handle->var.lnk_ctxt[link_itr].tx_state;
            } else if (enet_handle->var.lnk_ctxt[link_itr].tx_state == ENET_LNK_EQUIPPED && 
                       enet_var_reg->var.lnk_ctxt[link_itr].tx_state == ENET_LNK_START)
            {
                enet_var_clean->lnk_ctxt[link_itr].tx_state = enet_handle->var.lnk_ctxt[link_itr].tx_state;
            } else
            {
                if (TRUE == enet_is_pmon_datapath_get(enet_var_clean->ch_datapath[link_itr]))
                {
                    enet_var_clean->lnk_ctxt[link_itr].tx_state = enet_handle->var.lnk_ctxt[link_itr].tx_state;
                }
                else if (PMC_SUCCESS == loop_rc)
                {
                    loop_rc = ENET_ERR_CONTEXT_CLEANUP;    
                    PMC_LOG_TRACE("in %s -> step 5 - link = %d loop_rc = %s\n", __FUNCTION__, link_itr,pmc_log_string_get(loop_rc));            
                }
            }
            
            /* AUDIT EMPEI DPI SLOT */
            /* cleanup if DPI slot are inconsistent */
            if (enet_var_reg->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])] == enet_handle->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])])
            {
                slot_idx = 0;
                switch(enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]))
                {
                    /* -------------- */
                    /* 100GE datapaths */
                    /* -------------- */
                case ENET_LINK_100G:
                    for (itr = 0; itr < 10; itr++)
                    {
                        enet_var_clean->dpi_sch_slot[itr] = enet_var_reg->var.dpi_sch_slot[itr];
                
                    }
                    break;
                        
                    /* -------------- */
                    /* 40GE datapaths */
                    /* -------------- */
                case ENET_LINK_40G :
                    for (itr = link_itr; itr < (link_itr + 4); itr++)
                    {
                        slot_idx = ((itr % 4) * 3) + ((itr / 4) % 3);
                        enet_var_clean->dpi_sch_slot[slot_idx] = enet_var_reg->var.dpi_sch_slot[slot_idx];                
                    }
                    break;
                    /* -------------- */
                    /* 10GE datapaths */
                    /* -------------- */
                case ENET_LINK_10G :
                    slot_idx = ((link_itr % 4) * 3) + (link_itr / 4);
                    enet_var_clean->dpi_sch_slot[slot_idx] = enet_var_reg->var.dpi_sch_slot[slot_idx];                
                    break;
                        
                default:
                    break;
                }    


            }
            else
            {
                if (PMC_SUCCESS == loop_rc)
                {
                    if (0xf == enet_var_reg->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])] &&
                        enet_handle->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])] < 12 &&
                        enet_var_reg->var.ch_datapath[enet_handle->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])]] != ENET_UNPROV_DATAPATH)
                    {
                    }
                    else
                    {
                        loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("in %s -> step 3 - loop_rc = %s link_itr = %d dpi_slot %d/%d\n", __FUNCTION__, pmc_log_string_get(loop_rc), link_itr, 
                                  enet_var_reg->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])],
                                  enet_handle->var.dpi_sch_slot[enet_empei_link_dpi_sch_slot_index_get(link_itr, enet_var_reg->var.ch_datapath[link_itr])]);
                    }
                }
            }

            /* check ETIME ING link status */
            if (TRUE == enet_handle->var.lnk_ctxt[link_itr].ing_ts_en)              
            {
                enet_var_clean->lnk_ctxt[link_itr].ing_ts_en = FALSE;
                /*copy enet_handle ing timestamp status even at least one link has it ING TS feature enabled */
                for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                {
                    if (TRUE == enet_handle->var.lnk_ctxt[itr].ing_ts_en)
                    {
                        enet_var_clean->lnk_ctxt[link_itr].ing_ts_en = enet_handle->var.lnk_ctxt[link_itr].ing_ts_en;
                        break;
                    }
                }
                if (ENET_NUM_LINK_MAX == itr)
                {
                    if (PMC_SUCCESS == loop_rc)
                    {
                        loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("in %s -> step 6 - loop_rc = %s\n", __FUNCTION__, pmc_log_string_get(loop_rc));
                    }
                }
            }


            /* check ETIME EGR link status */
            if (enet_var_reg->var.lnk_ctxt[link_itr].egr_ts_en == enet_handle->var.lnk_ctxt[link_itr].egr_ts_en)
            {
                enet_var_clean->lnk_ctxt[link_itr].egr_ts_en = enet_handle->var.lnk_ctxt[link_itr].egr_ts_en;
            }
            else
            {
                if (PMC_SUCCESS == loop_rc)
                {
                    loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                    PMC_LOG_TRACE("in %s -> step 7 - loop_rc = %s\n", __FUNCTION__, pmc_log_string_get(loop_rc));
                }
            }
            
            /* update number of provisioned links in context */
            if (ENET_LNK_OPERATIONAL == enet_var_clean->lnk_ctxt[link_itr].rx_state ||
                ENET_LNK_OPERATIONAL == enet_var_clean->lnk_ctxt[link_itr].tx_state )
            {
                if (ENET_UNPROV_DATAPATH != enet_var_clean->ch_datapath[link_itr])
                {
                        
                    switch(enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]))
                    {
                    case ENET_LINK_100G:
                        if (0 == link_itr)
                        {
                            enet_var_clean->lnks_prov_cnt += 1;  
                        }
                        break;
                    case ENET_LINK_40G:
                        if (0 == (link_itr % 4))
                        {
                            enet_var_clean->lnks_prov_cnt += 1;  
                        }
                        break;
                    case ENET_LINK_10G:
                        enet_var_clean->lnks_prov_cnt += 1;  
                        break;
                    default:
                        break;
                    }
                }              
            }
                
            /* AUDIT EPMM MPMA SCBS3 */
            if (ENET_LNK_OPERATIONAL == enet_var_clean->lnk_ctxt[link_itr].rx_state &&
                ENET_UNPROV_DATAPATH != enet_var_clean->ch_datapath[link_itr]
                && EPMM_UNUSED_10G != enet_convert_datapath_to_epmm_datapath(enet_var_clean->ch_datapath[link_itr]))
            {
                
                start_itr = 0xF;
                switch(enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]))
                {
                    /* -------------- */
                    /* 100GE datapaths */
                    /* -------------- */
                case ENET_LINK_100G:
                    if (0 == (link_itr % ENET_NUM_LINK_100GE))
                    {
                        start_itr = link_itr;
                    }
                    break;
                    
                    /* -------------- */
                    /* 40GE datapaths */
                    /* -------------- */
                case ENET_LINK_40G :
                    
                    if (0 == (link_itr % ENET_NUM_LINK_40GE))
                    {
                        start_itr = link_itr;
                    }
                    break;
                    
                    /* -------------- */
                    /* 10GE datapaths */
                    /* -------------- */
                case ENET_LINK_10G:
                    start_itr = link_itr;
                    break;
                    
                default:
                    break;
                }        

                /* AUDIT: SCBS3 */
                if (0xF != start_itr &&
                    PMC_SUCCESS != enet_db_chnl_entry_audit(enet_handle, enet_var_reg, start_itr))
                {
                    if (PMC_SUCCESS == loop_rc)
                    {        
                        PMC_LOG_TRACE("entry audit failed: link = %d, datapath = %d\n", link_itr, enet_var_clean->ch_datapath[link_itr]);
                        loop_rc = ENET_ERR_CONTEXT_CLEANUP;   
                    }
                }
            }

            incr = 0;
            slot_idx =0;
            start_itr =0;     
            /* when datapath is a 40G or a 100G: AUDIT remaining links belonging to the datapath */
            if (PMC_SUCCESS == loop_rc)
            {
                switch(enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]))
                {
                    /* -------------- */
                    /* 100GE datapaths */
                    /* -------------- */
                case ENET_LINK_100G:
                    start_itr = 0;
                    if (0 == (link_itr % ENET_NUM_LINK_100GE))
                    {
                        incr = 10;
                    }
                    break;
                    
                    /* -------------- */
                    /* 40GE datapaths */
                    /* -------------- */
                case ENET_LINK_40G :
                    start_itr = link_itr;
                    if (0 == (link_itr % ENET_NUM_LINK_40GE))
                    {
                        incr = 4;
                    }
                    break;
                    
                    /* -------------- */
                    /* 10GE datapaths */
                    /* -------------- */
                case ENET_LINK_10G:
                    start_itr = link_itr;
                    incr = 1;
                    break;
                    
                default:
                    start_itr = link_itr;
                    incr = 0;
                    break;
                }        
                
                
                for (itr = start_itr + 1; itr < (link_itr + incr) && PMC_SUCCESS == loop_rc && itr < ENET_NUM_LINK_MAX; itr++)
                {                      
                    PMC_LOG_TRACE("itr = %d, link_itr = %d incr = %d\n", itr, link_itr, incr);
                    slot_idx = 0;
                    switch(enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]))
                    {
                        /* -------------- */
                        /* 100GE datapaths */
                        /* -------------- */
                    case ENET_LINK_100G:
                        slot_idx = itr;
                        break;
                        
                        /* -------------- */
                        /* 40GE datapaths */
                        /* -------------- */
                    case ENET_LINK_40G :
                        slot_idx = ((itr-start_itr)*3)+(link_itr%3);
                        break;
                        
                    default:
                        break;
                    }    

                    if (slot_idx >=  ENET_NUM_LINK_MAX)
                    {
                        loop_rc = ENET_ERR_CONTEXT_CLEANUP; 
                    } else if (enet_var_reg->var.dpi_sch_slot[slot_idx] != enet_handle->var.dpi_sch_slot[slot_idx] ||
                        enet_var_reg->var.lnk_ctxt[itr].rx_state != enet_handle->var.lnk_ctxt[itr].rx_state ||
                        enet_var_reg->var.lnk_ctxt[itr].tx_state != enet_handle->var.lnk_ctxt[itr].tx_state ||
                        ENET_UNPROV_DATAPATH ==  enet_handle->var.ch_datapath[itr] ||
                        enet_var_reg->var.dpi_sch_slot[slot_idx] != enet_handle->var.dpi_sch_slot[slot_idx] ||
                        enet_var_reg->var.lnk_ctxt[itr].egr_ts_en != enet_handle->var.lnk_ctxt[itr].egr_ts_en)
                    {
                        PMC_LOG_TRACE("step 10 - link_itr = %d itr = %d slot_idx = %d %d %d %d %d %d %d %d\n",
                                      link_itr, itr, slot_idx,
                                      enet_var_reg->var.dpi_sch_slot[slot_idx] != enet_handle->var.dpi_sch_slot[slot_idx],
                                      enet_var_reg->var.lnk_ctxt[itr].rx_state != enet_handle->var.lnk_ctxt[itr].rx_state,
                                      enet_var_reg->var.lnk_ctxt[itr].tx_state != enet_handle->var.lnk_ctxt[itr].tx_state,
                                      ENET_UNPROV_DATAPATH ==  enet_handle->var.ch_datapath[itr],
                                      enet_var_reg->var.lnk_ctxt[itr].egr_ts_en != enet_handle->var.lnk_ctxt[itr].egr_ts_en,
                                      enet_var_reg->var.dpi_sch_slot[slot_idx], 
                                      enet_handle->var.dpi_sch_slot[slot_idx]);
                        loop_rc = ENET_ERR_CONTEXT_CLEANUP; 
                    }
                }  
            }

            if(PMC_SUCCESS == loop_rc)
            {
                num_dpi_slot_entries = 0;
                for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
                {
                    if (enet_var_reg->var.dpi_sch_slot[itr] == link_itr)
                    {
                        num_dpi_slot_entries += 8;
                    }
                }
                (void) enet_db_link_num_entries_get(enet_handle, link_itr, &num_cal_entries);
                
                if (num_cal_entries != num_dpi_slot_entries)
                {
                    PMC_LOG_TRACE("ENET DB link = %d num_entries = %d num_cal_entries = %d\n", link_itr, num_dpi_slot_entries, num_cal_entries);
                    loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                }                
            }
  
            incr = 1;
            if (PMC_SUCCESS != loop_rc)
            {
                start_itr = link_itr;
                /* MAJOR ISSUE cleanup link:
                   loop_rc = ENET_ERR_CONTEXT_CLEANUP;
                   - 100G -> disable change channel 0 to 9 status
                   - 40G  -> disable channel itr/4 to itr/4 + 3 */
                /* --------------- */
                switch(enet_convert_datapath_to_enet_data_rate(enet_handle->var.ch_datapath[link_itr]))
                {
                    /* -------------- */
                    /* 100GE datapaths */
                    /* -------------- */
                case ENET_LINK_100G:
                    if (0 == link_itr)
                    {
                        start_itr = 0;
                        incr = 10;                       
                        for (itr = start_itr; itr < link_itr + incr && itr < ENET_NUM_LINK_MAX; itr++)
                        {
                            enet_var_clean->dpi_sch_slot[itr] = 0xF;
                        }
                    }
                    break;
                
                    /* -------------- */
                    /* 40GE datapaths */
                    /* -------------- */
                case ENET_LINK_40G :
                    if (0 == (link_itr % 4))
                    {
                        start_itr = link_itr;
                        incr = 4;

                        for (itr = start_itr; itr < link_itr + incr && itr < ENET_NUM_LINK_MAX; itr++)
                        {
                            enet_var_clean->dpi_sch_slot[((itr % 4) * 3) + start_itr % 3] = 0xF;
                        }
                    }
                    break;
                    
                    /* -------------- */
                    /* 10GE datapaths */
                    /* -------------- */
                case ENET_LINK_10G:
                default:
                    start_itr = link_itr;
                    incr = 1;
                    enet_var_clean->dpi_sch_slot[((start_itr % 4) * 3) + start_itr / 4] = 0xF;
                    break;                    
                }        
                for (itr = start_itr; itr < link_itr + incr && itr < ENET_NUM_LINK_MAX; itr++)
                {                    
                    enet_var_clean->lnk_ctxt[itr].rx_state = ENET_LNK_START;
                    enet_var_clean->lnk_ctxt[itr].tx_state = ENET_LNK_START;
                    enet_var_clean->lnk_ctxt[itr].empei_i_eclass_en = FALSE;   
                    enet_var_clean->lnk_ctxt[itr].empei_e_eclass_en = FALSE;
                    enet_var_clean->lnk_ctxt[itr].egr_ts_en = FALSE;
                    enet_var_clean->ch_datapath[itr] = ENET_UNPROV_DATAPATH;
                    PMC_LOG_TRACE("set default value link = %d\n", itr);  
                }
                enet_var_clean->lnks_prov_cnt -= 1;  
            }

            if (PMC_SUCCESS == rc)
            {
                rc = loop_rc;
            }
        }        
    }

    /* if enet is already running */
    if (PMC_SUCCESS == rc && TRUE == enet_var_reg->var.enet_start_state)
    {
        enet_var_clean->not_first_reset = FALSE;
    }

    PMC_RETURN(rc);
} /* enet_ctxt_audit */


/*******************************************************************************
* enet_ing_ts_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return Ingress Timestamp feature status according to ENET configuration.
*
* INPUTS:
*   *hndl        - pointer to ENET handle instance to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when Ingress timestamp feature is enabled
*   FALSE otherwise.
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8  enet_ing_ts_en_get(enet_handle_t *hndl)
{
    BOOL8 rc = FALSE;
    UINT32 itr;

    PMC_ENTRY();

    for (itr = 0; itr < ENET_NUM_LINK_MAX; itr++)
    {
        if (hndl->var.lnk_ctxt[itr].ing_ts_en == TRUE)
        {
            rc = TRUE;
            break;
        }
    }

    PMC_RETURN(rc);
}/*enet_ing_ts_en_get*/

/*******************************************************************************
*  enet_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure ENET and its sub-block specified link.
*
* INPUTS:
*   *hndl                  - handle to the Enet instance to be operated on containing 
*                            variable configuraton retrieved from saved context
*   *enet_var_reg          - ENET variable configuration recovered from device registers
*   *enet_energy_state_reg - ENET energy state
*
* OUTPUTS:
*   *enet_var_clean        - new ENET variable configuration that matches device state.
*
* RETURNS:
*   PMC_SUCCESS if operation is successfully executed, 
*   otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_ctxt_cleanup(enet_handle_t         *hndl,
                                   enet_recover_var_t    *enet_var_reg,
                                   enet_energy_state_t   *enet_energy_state_reg,
                                   enet_var_t            *enet_var_clean)
{
    PMC_ERROR          rc = PMC_SUCCESS;
    UINT32             num_link_to_clean = 0;
    UINT32             link_to_clean[ENET_NUM_LINK_MAX];
    UINT32             link_itr;
    UINT32             j;

    PMC_ENTRY();

    for (link_itr = 0; link_itr < ENET_NUM_LINK_MAX; link_itr++)
    {
        link_to_clean[link_itr] = 0xf;
    }
    enet_var_clean->lnks_prov_cnt = 0;        
    
    /* Perform regiter cleanup and sub-blocks memory context cleanup */
    if (enet_start_state_test(hndl) == FALSE)
    {
        /* cleanup links that are not in operational mode */
        num_link_to_clean = 0;
        for (link_itr = 0; link_itr  < ENET_NUM_LINK_MAX && PMC_SUCCESS == rc; link_itr++)
        {
            if (ENET_LNK_START == enet_var_clean->lnk_ctxt[link_itr].rx_state ||
                ENET_LNK_START == enet_var_clean->lnk_ctxt[link_itr].tx_state 
                )
            {   
                /* if link belongs to a 40G datapath */  
                if ((0 != (link_itr % ENET_NUM_LINK_40GE)) && 
                    enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[(link_itr / ENET_NUM_LINK_40GE) * ENET_NUM_LINK_40GE]) == ENET_LINK_40G) 
                {
                    for (j = 0; j < num_link_to_clean; j++)
                    {
                        if (link_to_clean[j] == ((link_itr / ENET_NUM_LINK_40GE) * ENET_NUM_LINK_40GE))
                        {                            
                            link_to_clean[num_link_to_clean] = link_itr;
                            num_link_to_clean += 1;
                            PMC_LOG_TRACE("cleanup link = %d\n", link_itr);
                            break;
                        }
                    }
                }
                /* if link belongs to a 100G datapath */  
                else if ((0 != (link_itr % ENET_NUM_LINK_100GE)) && 
                         enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[(link_itr / ENET_NUM_LINK_100GE) * ENET_NUM_LINK_100GE]) == ENET_LINK_100G)
                {
                    for (j = 0; j < num_link_to_clean; j++)
                    {
                        if (link_to_clean[j] == ((link_itr / ENET_NUM_LINK_100GE) * ENET_NUM_LINK_100GE))
                        {                            
                            link_to_clean[num_link_to_clean] = (link_itr / ENET_NUM_LINK_100GE) * ENET_NUM_LINK_100GE;
                            num_link_to_clean += 1;
                            PMC_LOG_TRACE("cleanup link = %d\n", link_itr);
                            break;
                        }
                    }
                }
                /* if link belongs to a 10G link or is the link of a 40G or 100G datapath */
                else
                {
                    link_to_clean[num_link_to_clean] = link_itr;
                    num_link_to_clean += 1;
                    PMC_LOG_TRACE("cleanup link = %d\n", link_itr);
                }
            } else 
            {
                 /* if link belongs to a 40G datapath */  
                if ((0 == (link_itr % ENET_NUM_LINK_40GE)) && 
                    enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[(link_itr / ENET_NUM_LINK_40GE) * ENET_NUM_LINK_40GE]) == ENET_LINK_40G) 
                {
                    enet_var_clean->lnks_prov_cnt += 1;
                }
                /* if link belongs to a 100G datapath */  
                else if ((0 == (link_itr % ENET_NUM_LINK_100GE)) && 
                         enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[(link_itr / ENET_NUM_LINK_100GE) * ENET_NUM_LINK_100GE]) == ENET_LINK_100G)
                {
                    enet_var_clean->lnks_prov_cnt += 1;
                }
                /* if link belongs to a 10G link or is the link of a 40G or 100G datapath */
                else if (enet_convert_datapath_to_enet_data_rate(enet_var_clean->ch_datapath[link_itr]) == ENET_LINK_10G)
                {
                    enet_var_clean->lnks_prov_cnt += 1;
                }
                
            }
        }
    } 

    enet_var_clean->not_first_reset = hndl->var.not_first_reset;    
    /* the old enet var context is incoherent, we use the new one */
    PMC_MEMCPY(&hndl->var,enet_var_clean,sizeof(enet_var_t));    

    /* cleanup all sub-blocks */
    /* CLEANUP EMAC */ 
    if (PMC_SUCCESS == rc) 
    {
        rc = emac_cleanup(hndl->emac_handle, num_link_to_clean, link_to_clean, enet_ing_ts_en_get(hndl));
    }
    
    /* CLEANUP EPMM */ 
    if (PMC_SUCCESS == rc) 
    {
        epmm_cleanup(hndl->epmm_handle, num_link_to_clean, link_to_clean);
    }

    /* CLEANUP ETRANS */ 
    if (PMC_SUCCESS == rc)
    {        
        etrans_cleanup(hndl->etrans_handle, num_link_to_clean, link_to_clean);
    }

    /* CLEANUP EMPEI_I */ 
    if (PMC_SUCCESS == rc)
    {        
        empei_i_cleanup(hndl->empei_i_handle, num_link_to_clean, link_to_clean);
    }

    /* CLEANUP EMPEI_E */ 
    if (PMC_SUCCESS == rc)
    {        
        empei_e_cleanup(hndl->empei_e_handle, num_link_to_clean, link_to_clean);
    }

    /* CLEANUP ENET */ 
    if (PMC_SUCCESS == rc)
    {        
        enet_reg_cleanup(hndl, num_link_to_clean, link_to_clean);
    }    
    
    PMC_RETURN(rc);
    
} /* enet_ctxt_cleanup */

/*******************************************************************************
*  enet_ctxt_mpmo_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure ENET and its sub-block specified link.
*
* INPUTS:
*   *hndl                  - handle to the Enet instance to be operated on containing 
*                            variable configuraton retrieved from saved context
*   *restart_init_cfg_ptr  - pointer to restart init config
* 
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if operation is successfully executed, 
*   otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void enet_ctxt_mpmo_cleanup(enet_handle_t             *hndl,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{

    UINT32 link_itr = 0;
    UINT32 link_clean[ENET_NUM_LINK_MAX];
    UINT32 num_link_clean = 0;
    epmm_datapath_t    epmm_datapath = EPMM_10GE_GSUP43_73;
    UINT32 egress_port = 0;

    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(restart_init_cfg_ptr->dcpb_connect!=NULL, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
        
    egress_port = (hndl->cfg.enet_type_id == ENET_LINE ) ?(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE:(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_SYS;

    for(link_itr = 0;link_itr < ENET_NUM_LINK_MAX;link_itr++)
    {
        epmm_datapath     = enet_convert_datapath_to_epmm_datapath(hndl->var.ch_datapath[link_itr] );

        if (hndl->var.lnk_ctxt[link_itr].tx_state == ENET_LNK_OPERATIONAL && 
            (epmm_datapath == EPMM_10GE_CBR || epmm_datapath == EPMM_10GE_GSUP43_73 ||
             epmm_datapath == EPMM_40GE     || epmm_datapath == EPMM_100GE))
        {
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->dcpb_connect->chnl[egress_port][link_itr].mst.state)
            {
                link_clean[num_link_clean] = link_itr;
                num_link_clean += 1;
            }
        }
    }
    if (num_link_clean != 0)
    {
        epmm_mpmo_cleanup(hndl->epmm_handle, num_link_clean, &link_clean[0]);
    }
    
    PMC_RETURN();
    
} /* enet_ctxt_mpmo_cleanup */

/*******************************************************************************
*  enet_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable ENET specified links.
*
* INPUTS:
*   hndl         - handle to the ENET instance to be operated on
*   num_links    - link to disable
*   links        - indices of links to disable
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
PRIVATE void enet_reg_cleanup(enet_handle_t    *hndl,
                              UINT32            num_links, 
                              UINT32           *links)
{
    UINT32 link_itr;
    UINT32 link_idx;
    enet_buffer_t enet_buffer[1];
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    enet_buffer_init(enet_buffer, hndl);

    for (link_itr = 0; link_itr < num_links && PMC_SUCCESS == rc; link_itr++)
    {

        link_idx = links[link_itr];

        /* resync energy state */
        enet_init(hndl, link_idx, 
                  ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC, 
                  PMC_ENERGY_STATE_REQUEST_OPTIMAL);

        enet_field_range_LINK_10G_EN_set(NULL,hndl,link_idx,link_idx,0);
        
        if (0 == (link_idx % ENET_NUM_LINK_40GE))
        {
            enet_field_range_LINK_40G_EN_set(NULL,hndl,link_idx / ENET_NUM_LINK_40GE,link_idx / ENET_NUM_LINK_40GE,0);
        }

        if (0 == link_idx)
        {
            enet_field_LINK_100G_EN_set(NULL, hndl, 0);
        }
            

        /* If successful, then decrement the number of configured datapaths */
        
        hndl->var.lnk_ctxt[link_idx].rx_state = ENET_LNK_START;
        hndl->var.lnk_ctxt[link_idx].tx_state = ENET_LNK_START;
    }

    enet_buffer_flush(enet_buffer);

    PMC_RETURN();
    
}/*enet_reg_cleanup*/

/*******************************************************************************
*  enet_enhanced_pmon_num_links_set_up_gsup43_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if any enet link is set up as gsup43 TX Enhanced PMON.
*
* INPUTS:
*   hndl         - pointer to the ENET handle
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   Returns the number of datapaths running in
*   ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON mode
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 enet_enhanced_pmon_num_links_set_up_gsup43_pmon(enet_handle_t *hndl) 
{

    UINT32 result = 0;
    UINT32 i; /* counter */

    PMC_ENTRY();

    for (i = 0; i < ENET_NUM_LINK_MAX; i++) {

        if (hndl->var.ch_datapath[i] == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON) {

            result++;
        }
    }

    PMC_RETURN(result);
} /* enet_enhanced_pmon_num_links_set_up_gsup43_pmon */

/*******************************************************************************
*  enet_empei_link_dpi_sch_slot_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Fulfill dpi scheduler slot list for a given link.
*
* INPUTS:
*   link                - channel ID to be configured Valid range: 0 to 11
*   datapath            - datapath to configure
*
* OUTPUTS:
*  *dpi_sch_slot        - DPI scheduler slot table
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void  enet_empei_link_dpi_sch_slot_get(UINT32            link,  
                                               enet_datapath_t   datapath,
                                               UINT32           *dpi_sch_slot)
{
    UINT32 index;
    UINT32 jndex;
    enet_data_rate_t rate;

    PMC_ENTRY();

    rate = enet_convert_datapath_to_enet_data_rate(datapath);

    /* Fulfill dpi calendar slot table */
    switch(rate)
    {
        /* --------------- */
        /* 100GE datapaths */
        /* --------------- */
    case ENET_LINK_100G:        
        /* Configure all time slots to 0 */
        for (jndex = 0; jndex < ENET_NUM_LINK_100GE; jndex++) 
        {
            dpi_sch_slot[jndex] = 0;
        } 
        break;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    case ENET_LINK_40G:       
        for ( jndex = 0; jndex < ENET_NUM_LINK_40GE; jndex++ ) {
            dpi_sch_slot[(jndex*3)+(link%3)] = link;
        }
        break;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    case ENET_LINK_10G:    
        /* -------------- */
        /* 10GE datapaths */        
        index = ((link%4)*3) + (link/4);        
        dpi_sch_slot[index] = link;
        break;

    default:    
        index = link;
        dpi_sch_slot[index] = 0xF;
        break;
    }


    PMC_RETURN();
} /* enet_empei_link_dpi_sch_slot_get */


/*******************************************************************************
*  enet_empei_link_dpi_sch_slot_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns dpi slot index associated to a link.
*
* INPUTS:
*   link                - channel ID to be configured Valid range: 0 to 11
*   datapath            - datapath to configure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Returns dpi slot index associated to a link.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 enet_empei_link_dpi_sch_slot_index_get(UINT32            link,  
                                                      enet_datapath_t   datapath)
{
    UINT32 index;
    enet_data_rate_t rate;

    PMC_ENTRY();

    rate = enet_convert_datapath_to_enet_data_rate(datapath);

    /* according to ENET rate, compute dpi calendar slot value associated to link */
    switch(rate)
    {
        /* --------------- */
        /* 100GE datapaths */
        /* --------------- */
    case ENET_LINK_100G:        
        index = link;
        break;

        /* -------------- */
        /* 40GE datapaths */
        /* -------------- */
    case ENET_LINK_40G:     
        index = ((link % 4) * 3) + ((link / 4)  % 3);
        break;

        /* -------------- */
        /* 10GE datapaths */
        /* -------------- */
    case ENET_LINK_10G:    
        /* -------------- */
        /* 10GE datapaths */        
        index = ((link%4)*3) + (link/4);     
        break;

    default:    
        index = link;
        break;
    }

    PMC_RETURN(index);
} /* enet_empei_link_dpi_sch_slot_get */


/*******************************************************************************
* enet_db_chnl_entry_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function verifies EPMM MPMA SCBS3 calendar of a specified link.
*
* INPUTS:
*   *enet_handle            - pointer to ENET handle instance
*   *enet_var_reg           - ENET variable configuration retrieved from device registers
*    link                   - link identifier
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfull otherwise a descriptive error 
*   code is returned.
*
* NOTES: 
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_db_chnl_entry_audit(enet_handle_t        *enet_handle,
                                           enet_recover_var_t   *enet_var_reg,
                                           UINT32                link)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               num_of_recs = 0;
    mapotn_db_key        key;
    mapotn_db_query_res  *q_result;
    UINT32               itr;
    UINT32               cal_itr;
    UINT32               db_handle;
    UINT32               cal_entries[EPMM_NUM_DB_ENTRIES_MAX];
    UINT32               num_cal_entries = 0;
    scbs3_page_t        *scbs3_page_ptr;
    util_gen_db_id_t     db_id;
    
        
    PMC_ENTRY();
    
    /* argument checking */      
    db_handle = enet_handle->epmm_handle->mpma_handle->scbs3_handle->util_schd_handle->cfg.db_handle; 
    scbs3_page_ptr = &enet_var_reg->epmm_mpma_scbs3_page;
    db_id = enet_handle->epmm_handle->mpma_handle->cfg.default_param.db_id;
                   
    /* check that there are some calendar entries associated to link */
    key.calendar_entry = DO_NOT_USE_KEY;
    key.channel_num = link;
    num_cal_entries = gen_db_query_count(&(enet_handle->base),
                                   enet_handle->epmm_handle->mpma_handle->cfg.default_param.db_id, 
                                   enet_handle->epmm_handle->mpma_handle->scbs3_handle->util_schd_handle->cfg.db_handle, 
                                   (void *)&key);
    
    if (num_cal_entries == 0)
    {
        result = ENET_ERR_CONTEXT_CLEANUP;
    }

    num_cal_entries = 0;
    /* query all callendar entries belonging to link*/
    for (itr = 0; itr < EPMM_NUM_DB_ENTRIES_MAX; itr++)
    {
        /* query rd_channel_id data for the iteration */
        key.calendar_entry = itr;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs = gen_db_query(&(enet_handle->base), 
                                   db_id, /*enet_handle->cfg.schd_use_ctxt.*/
                                   db_handle,
                                   (void *)&key,
                                   (void **)&q_result);
        if(num_of_recs == 0)
        {
            PMC_RETURN(ENET_ERR_CONTEXT_CLEANUP);
        }
        else
        {
            if (q_result->p_recs[0]->channel_num == link)
            {
                cal_entries[num_cal_entries] = itr;
                num_cal_entries += 1;                
            }
        }
    }

    /* compare register based calendar slot against database entries */
    for (cal_itr = 0; cal_itr < num_cal_entries && PMC_SUCCESS == result; cal_itr++)
    {              
        if (link != scbs3_page_ptr->calendar[cal_entries[cal_itr]])
        {
            PMC_LOG_TRACE("%d - cal_entries[%d]= %d\n", num_cal_entries, cal_itr, cal_entries[cal_itr]);
            PMC_LOG_TRACE ("in %s, compare link(%d) == calendar(%d)\n", __FUNCTION__, link, scbs3_page_ptr->calendar[cal_entries[cal_itr]]);  
            result = ENET_ERR_CONTEXT_CLEANUP;
        }
    }

    PMC_RETURN(result);

} /* enet_db_chnl_entry_audit */


/*******************************************************************************
* enet_util_global_state_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Convert type util_global_chnl_state_t to enet_lnk_state_t.
*
* INPUTS:
*    
*    state                  - link state in util_global_chnl_state_t type
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   enet_lnk_state_t  - link state in enet_lnk_state_t type.
*
* NOTES: 
*
*******************************************************************************/
PRIVATE enet_lnk_state_t enet_util_global_state_convert(util_global_chnl_state_t  state)
{
    enet_lnk_state_t rc;

    PMC_ENTRY();

    switch(state)
    {
    case UTIL_GLOBAL_CHNL_START:
        rc = ENET_LNK_START;
        break;
    case UTIL_GLOBAL_CHNL_EQUIPPED:
        rc = ENET_LNK_EQUIPPED;
        break;
    case UTIL_GLOBAL_CHNL_OPERATIONAL:
        rc = ENET_LNK_OPERATIONAL;
        break;
    default:
        rc = LAST_ENET_LNK_STATE;
        break;
    }

    PMC_RETURN(rc);
} /*enet_util_global_state_convert*/

/*******************************************************************************
* enet_eclass_pkt_extract_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Returns ECLASS link extract status for a given configuration.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   dir                   - ENET datapath direction: ENET_INGRESS or ENET_EGRESS 
*   extract_cfg_ptr       - pointer to extraction configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is configured otherwise FALSE.
*
*******************************************************************************/
PUBLIC BOOL8 enet_eclass_pkt_extract_status_get(enet_handle_t            *hndl, 
                                                UINT32                    link, 
                                                enet_dir_t                dir,
                                                eclass_pkt_extract_cfg_t *extract_cfg_ptr)
{
    eclass_handle_t *eclass_handle_ptr;
    BOOL8 rc = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    eclass_handle_ptr = enet_eclass_handle_get(hndl, dir);

    if (NULL != eclass_handle_ptr)
    {
        rc = eclass_pkt_extract_status_get(eclass_handle_ptr, link, extract_cfg_ptr);
    }

    PMC_RETURN(rc);
}/*enet_eclass_pkt_extract_status_get*/


/*******************************************************************************
* enet_eclass_pkt_extract_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Returns ECLASS link extract configuration structure for the addressed
*    link and direction.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   dir                   - ENET datapath direction: ENET_INGRESS or ENET_EGRESS 
*
* OUTPUTS:
*   array_size_ptr        - size of extract_cfg_ptr array
*   array_used_size_ptr   - number of used elements in extract_cfg_array
*   extract_cfg_ptr       - pointer to extraction configuration
*
* RETURNS:
*   TRUE when extraction is configured otherwise FALSE.
*
*******************************************************************************/
PUBLIC void enet_eclass_pkt_extract_cfg_get(enet_handle_t            *hndl, 
                                            UINT32                    link, 
                                            enet_dir_t                dir,
                                            UINT32                   *array_size_ptr,
                                            UINT32                   *array_used_size_ptr,
                                            eclass_pkt_extract_cfg_t *extract_cfg_ptr)
{
    eclass_handle_t *eclass_handle_ptr;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    eclass_handle_ptr = enet_eclass_handle_get(hndl, dir);

    if (NULL != eclass_handle_ptr)
    {
        eclass_pkt_extract_cfg_get(eclass_handle_ptr, link, array_size_ptr, array_used_size_ptr, extract_cfg_ptr);
    }

    PMC_RETURN();
}/*enet_eclass_pkt_extract_cfg_get*/

/*******************************************************************************
* enet_eclass_pkt_extract_all_cfg_disabled_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE when extraction is not configured.
*
* INPUTS:
*   *hndl                 - pointer to ENET handle instance.
*    dir                  - ENET datapath direction: ENET_INGRESS or ENET_EGRESS.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is not configured otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 enet_eclass_pkt_extract_all_cfg_disabled_get(enet_handle_t *hndl,
                                                          enet_dir_t     dir)
{
    BOOL8 rc = TRUE;
    eclass_handle_t *eclass_handle_ptr;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    eclass_handle_ptr = enet_eclass_handle_get(hndl, dir);

    if (NULL != eclass_handle_ptr)
    {
        eclass_pkt_extract_all_cfg_disabled_get(eclass_handle_ptr);
    }

    PMC_RETURN(rc);
}/*enet_eclass_pkt_extract_all_cfg_disabled_get*/

/*******************************************************************************
* enet_eclass_pkt_extract_link_cfg_disabled_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE when no extraction is configured for a given link.
*    
* INPUTS:
*   *hndl                 - pointer to ENET handle instance.
*   link                  - the link to be operated on.
*    dir                  - ENET datapath direction: ENET_INGRESS or ENET_EGRESS.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is configured for a given link otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 enet_eclass_pkt_extract_link_cfg_disabled_get(enet_handle_t *hndl,
                                                           UINT32         link,
                                                           enet_dir_t     dir)
{   
    eclass_handle_t *eclass_handle_ptr;
    BOOL8 rc = TRUE;


    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ENET_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    eclass_handle_ptr = enet_eclass_handle_get(hndl, dir);

    if (NULL != eclass_handle_ptr)
    {
        rc = eclass_pkt_extract_link_cfg_disabled_get(eclass_handle_ptr, link);
    }
    
    PMC_RETURN(rc);
}/*enet_eclass_pkt_extract_link_cfg_disabled_get*/

/*******************************************************************************
* enet_eclass_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns ECLASS link extract status for a given configuration. 
*
* INPUTS:
*   *hndl        - pointer to ECLASS handle instance.
*    dir         - ENET datapath direction: ENET_INGRESS or ENET_EGRESS.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is configured otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC eclass_handle_t *enet_eclass_handle_get(enet_handle_t *hndl, 
                                               enet_dir_t     dir)
{
    eclass_handle_t *eclass_handle_ptr = NULL;

    PMC_ENTRY();

    switch (dir)        
    {
    case ENET_EGRESS:
        eclass_handle_ptr = hndl->empei_e_eclass_handle;
        break;
    case ENET_INGRESS:
        eclass_handle_ptr = hndl->empei_i_eclass_handle;
        break;
    default:
        break;
    }    
    PMC_RETURN(eclass_handle_ptr);
}/* enet_eclass_handle_get */


/*******************************************************************************
* enet_check_rx_epmm_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Checks whether the EPMM state may have been impacted by a consequential
*  action in FW.  This could be the case if the reg state indicates 
*  EPMM data path is not used however the enet_handle indicates that it is.
*  As well the data rates of the register and context space are the same 
*  and the ING RESET register is out of reset but is not enabled.
*
* INPUTS:
*  enet_handle        - pointer to ENET handle instance to be operated on
*  enet_var_reg       - Recovered enet_var_reg instance
*  link               - ENET link being operated on.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*  TRUE  - Datapath is valid
*  FALSE - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 enet_check_rx_epmm_state(enet_handle_t *enet_handle,
                                       enet_recover_var_t *enet_var_reg,
                                       UINT32 link)
{
    BOOL8 result = FALSE;
    UINT32 reg_value = 0xFF;

    PMC_ENTRY();
    /*
     * Handle one of 2 cases. If in here then the data paths at the enet
     * level do not match. If however it is set at the EPMM layer and the
     * register and context values match or is unused in the register space
     * but is set in the context and one of the 4 possible RX PMON cases
     * then check the register.
     */
    if (((EPMM_DATAPATH_UNUSED != enet_handle->epmm_handle->var.datapath[link] &&
        enet_var_reg->epmm_rx_link_datapath[link] == enet_handle->epmm_handle->var.datapath[link]) ||
         (EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link] &&
          EPMM_DATAPATH_UNUSED != enet_handle->epmm_handle->var.datapath[link])) &&
        (ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON == enet_handle->var.ch_datapath[link] ||
         ENET_SERDES_10GE_TRANSPARENT_RX_PMON == enet_handle->var.ch_datapath[link] ||
         ENET_SERDES_40GE_TRANSPARENT_RX_PMON == enet_handle->var.ch_datapath[link] ||
         ENET_SERDES_100GE_TRANSPARENT_RX_PMON == enet_handle->var.ch_datapath[link]))
    {
        /*
         * Confirm that the rates match and return an error if not.
         */
        enet_data_rate_t enet_reg_rate = enet_convert_datapath_to_enet_data_rate(enet_var_reg->var.ch_datapath[link]);
        enet_data_rate_t enet_rate = enet_convert_datapath_to_enet_data_rate(enet_handle->var.ch_datapath[link]);

        if (enet_reg_rate != enet_rate)
        {
            /*
             * Rates do not match. There is nothing to do but return false.
             */
            PMC_RETURN(FALSE);
        }
        /*
         * Retrieve the ING RESET register value.
         * NOTE: The enet_rate in this module and in epmm block have the 
         * same values making the cast safe..
         */
        reg_value = epmm_ing_reset_get(enet_handle->epmm_handle,
                                       link,
                                       (epmm_data_rate_t)enet_rate);
    }
    else if (ENET_UNPROV_DATAPATH == enet_var_reg->var.ch_datapath[link] &&
             ENET_LNK_START == enet_var_reg->var.lnk_ctxt[5].rx_state &&
             EPMM_DATAPATH_UNUSED == enet_var_reg->epmm_rx_link_datapath[link] &&
             ENET_LNK_START == enet_handle->var.lnk_ctxt[link].rx_state &&
             EPMM_DATAPATH_UNUSED == enet_handle->epmm_handle->var.datapath[5])
      {
        /*
         * Handle this one known special case where the enet_var_reg and
         * the context ch_datapath value do not match, which is why this
         * function was called. However everything else indicates that this
         * data path is not functional.
         */
        result = TRUE;
      }
    /*
     * Value of 0 indicates the register is out of RESET but is not enabled.
     * value of 2 indicates that it is out of RESET and is enabled. In 
     * either case this is good.
     */
    if (0 == reg_value || 0x2 == reg_value)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* enet_check_rx_epmm_state */


/*******************************************************************************
* FUNCTION: digi_enet_check_align_status_v_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check value of align_status_v
*
* INPUTS:
*   hndl     - pointer to ENET handle instance.
*
* RETURNS:
*   TRUE - align_status_v is set;
*   FALSE - otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL enet_100G_check_align_status_v_get(enet_handle_t *hndl)
{
    BOOL rc;

    PMC_ENTRY();
    rc = emac_100G_check_align_status_v_get(hndl->emac_handle);
    PMC_RETURN(rc);
}


/*******************************************************************************
* FUNCTION: digi_enet_disable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear align_status_e and rx_loc_fault_e
*
* INPUTS:
*   hndl     - pointer to ENET handle instance.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_100G_disable_align_status_loc_fault(enet_handle_t *hndl)
{
    PMC_ENTRY();
    emac_100G_disable_align_status_loc_fault(hndl->emac_handle);
    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: digi_enet_enable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set align_status_e and rx_loc_fault_e
*
* INPUTS:
*   hndl     - pointer to ENET handle instance.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_100G_enable_align_status_loc_fault(enet_handle_t *hndl)
{
    PMC_ENTRY();
    emac_100G_enable_align_status_loc_fault(hndl->emac_handle);
    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: enet_100G_clear_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write to clear align_status_i and rx_loc_fault_i
*
* INPUTS:
*   hndl     - pointer to ENET handle instance.
*
* OUTPUTS:
*   None: 
*
* RETURNS:
*   None:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_100G_clear_align_status_loc_fault(enet_handle_t *hndl)
{
    PMC_ENTRY();
    emac_100G_clear_align_status_loc_fault(hndl->emac_handle);
    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: enet_pcs_poll_count_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   hndl       - pointer to ENET handle instance.
*   poll_count - the polling counter to save in the ENET context
*
* RETURNS:
*
* NOTES:
*******************************************************************************/
PUBLIC void enet_pcs_poll_count_set(enet_handle_t *hndl, UINT8 poll_count)
{
    PMC_ENTRY();
    /* clear the old counter value */
    hndl->var.lnks_prov_cnt = hndl->var.lnks_prov_cnt & (~ENET_PCS_POLL_COUNT_MASK);
    /* set the new value */
    hndl->var.lnks_prov_cnt = hndl->var.lnks_prov_cnt | ((poll_count << ENET_PCS_POLL_COUNT_OFFSET) & ENET_PCS_POLL_COUNT_MASK);

    PMC_RETURN();
}


/*******************************************************************************
* FUNCTION: enet_pcs_poll_count_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
* INPUTS:
*   hndl       - pointer to ENET handle instance.
*
* OUTPUTS:
*   poll_count - the retrieved polling counter value
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PUBLIC void enet_pcs_poll_count_get(enet_handle_t *hndl, UINT8 *poll_count)
{
    UINT16 tmp_count;

    PMC_ENTRY();

    tmp_count = (hndl->var.lnks_prov_cnt & ENET_PCS_POLL_COUNT_MASK) >> ENET_PCS_POLL_COUNT_OFFSET;
    *poll_count = tmp_count;

    PMC_RETURN();
}

/*******************************************************************************
* FUNCTION: enet_emac_pcs_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets the ENET EMAC PCS block
*
* INPUTS:
*   hndl     - pointer to ENET handle instance.
*   link     - indice of link to reset
*
* OUTPUTS:
*   None: 
*
* RETURNS:
*   None:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void enet_emac_pcs_reset(enet_handle_t *hndl,
                                UINT32        link)
{
    PMC_ENTRY();
    
    emac_pcs_reset(hndl->emac_handle,
                   link);

    PMC_RETURN();

} /*enet_emac_pcs_reset */



/*
** End of file
*/
