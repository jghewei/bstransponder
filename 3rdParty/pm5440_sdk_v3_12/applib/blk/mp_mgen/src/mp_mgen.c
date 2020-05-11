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

#include "mp_mgen_loc.h"

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
const char MP_MGEN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    MP_MGEN_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char MP_MGEN_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE UINT32 mp_mgen_last_txr_byte_size_calc(mp_mgen_handle_t *mp_mgen_handle, 
                                               util_global_odukp_type_t server_type, 
                                               util_global_odukp_type_t client_type,
                                               util_global_mapping_mode_t mapping_mode,
                                               UINT32 ts_type,
                                               UINT32 trib_slot);

/*
** Public Functions
*/
/*******************************************************************************
* mp_mgen_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a MP_MGEN block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the RGFPF subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mp_mgen_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC mp_mgen_handle_t *mp_mgen_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    mp_mgen_handle_t *mp_mgen_handle;

    PMC_ENTRY();
    
    mp_mgen_handle = (mp_mgen_handle_t*)PMC_CTXT_CALLOC(sizeof(mp_mgen_handle_t), parent);
    
    pmc_handle_init(parent, mp_mgen_handle, sys_handle, PMC_MID_DIGI_MP_MGEN, tsb_name, base_address);
    
     /* Register log strings. */
    pmc_log_block_strings_register(MP_MGEN_LOG_ERR_STRINGS[0], MP_MGEN_LOG_ERR_TABLE_BASE, MP_MGEN_LOG_ERR_COUNT);
    
    PMC_RETURN(mp_mgen_handle);
} /* mp_mgen_ctxt_create */

 
/*******************************************************************************
* mp_mgen_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a MP_MGEN block instance.
*
*
* INPUTS:
*   *mp_mgen_handle        - pointer to MP_MGEN handle instance
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
PUBLIC void mp_mgen_ctxt_destroy(mp_mgen_handle_t *mp_mgen_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&mp_mgen_handle, mp_mgen_handle);
    
    PMC_RETURN();
} /* mp_mgen_ctxt_destroy */


/*******************************************************************************
* mp_mgen_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an MP_MGEN block handle instance.
*
*
* INPUTS:
*   *mp_mgen_handle         - pointer to MP_MGEN handle instance
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
PUBLIC void mp_mgen_handle_init(mp_mgen_handle_t *mp_mgen_handle)
{
    PMC_ENTRY();
    
    PMC_RETURN();
} /* mp_mgen_handle_init */

/*******************************************************************************
* mp_mgen_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on mp_mgen.
*
*
* INPUTS:
*   *mp_mgen_handle       - pointer to MP_MGEN handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. \n
*     WARM : register/context are coherent \n
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mp_mgen_handle_restart_init(mp_mgen_handle_t *mp_mgen_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                             pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(MP_MGEN_LOG_ERR_STRINGS[0], MP_MGEN_LOG_ERR_TABLE_BASE, MP_MGEN_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* mp_mgen_handle_restart_init */

/*******************************************************************************
* mp_mgen_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
*
* INPUTS:
*   *mp_mgen_handle         - pointer to MP_MGEN handle instance
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
PUBLIC PMC_ERROR mp_mgen_ram_init(mp_mgen_handle_t *mp_mgen_handle)
{
    UINT32 i;

    PMC_ENTRY();
    
    for (i = 0; i < 96; i++)
    {
        mp_mgen_reg_AMP_CFGS_array_write(NULL, mp_mgen_handle, i, 0x00000000);
        mp_mgen_reg_DIV_48_M_LAST_TXR_array_write(NULL, mp_mgen_handle, i, 0x00000000);
        mp_mgen_reg_IPT_ENTRY_array_write(NULL, mp_mgen_handle, i, 0x00007F00);
        
        mp_mgen_field_IPT_VALID_set(NULL, mp_mgen_handle, i, 0);
        mp_mgen_field_IPT_ID_set(NULL, mp_mgen_handle, i, 0x7F);
        mp_mgen_field_IPT_VA_set(NULL, mp_mgen_handle, i, 0);
    }        
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mp_mgen_ram_init */


/*******************************************************************************
* mp_mgen_ipt_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the IPT ID for mp_mgen.
*   
*
* INPUTS:
*   *mp_mgen_handle   - pointer to MP_MGEN handle instance                
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
PUBLIC PMC_ERROR mp_mgen_ipt_print(mp_mgen_handle_t *mp_mgen_handle)
{
    /* variable declaration */
    UINT32 i, chnl, valid, occr;
      
    PMC_ENTRY();
    
    for(i=0; i<MP_MGEN_MAX_CHNL_NUM; i++)
    {
        chnl = mp_mgen_field_IPT_ID_get(NULL, mp_mgen_handle, i);
        valid = mp_mgen_field_IPT_VALID_get(NULL, mp_mgen_handle, i);
        occr = mp_mgen_field_IPT_VA_get(NULL, mp_mgen_handle, i);
        PMC_LOG_TRACE("MPMGEN IPT ID loc:%d,  chnl_id:%d  valid:%d  occr:%d\n", i, chnl, valid, occr);
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* mp_mgen_ipt_print */ 


/*******************************************************************************
* mp_mgen_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the IPT configuration for tracking the calendar
*   entry assignment for the LO ODU channel. 
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   ts_loc              - calendar entry that belong to the LO ODU channel
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95   
*   occurrence          - occurrence number of the channel.
*                         Valid range: 0 - 8 \n
*                         When configuring for the first tributary slot of a 
*                         channel, set to 0. Every time the next tributary
*                         slot is added for the same channel, increment this
*                         argument by 1. The maximum occurrence for any one
*                         channel should equal to the number of tributary slots
*                         occupied by the channel.
*   valid               - indicates if the tributary slot is occupied or not. \n
*                         1 -- calendar entry is occupied (IPT entry is in use) \n
*                         0 -- calendar entry is unoccupied (IPT entry is not in use)               
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
PUBLIC PMC_ERROR mp_mgen_ipt_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                 UINT32 ts_loc, UINT32 chnl_id, 
                                 UINT32 occurrence, UINT32 valid)
{
    /* variable declaration */
    mp_mgen_buffer_t mp_mgen_buffer[1];

    PMC_ENTRY();
     
    mp_mgen_buffer_init(mp_mgen_buffer, mp_mgen_handle);

    /* If configuring IPT as a provisioning operation, write to valid last such
    ** that ID and VA are set when the entry is enabled
    */  
    if (1 == valid)
    {
        mp_mgen_field_IPT_ID_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, chnl_id);
        mp_mgen_field_IPT_VA_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, occurrence);

        mp_mgen_buffer_flush(mp_mgen_buffer);

        mp_mgen_field_IPT_ID_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, chnl_id);
        mp_mgen_field_IPT_VA_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, occurrence);
        mp_mgen_field_IPT_VALID_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, valid);
    }
    /* If configuring IPT as a deprovisioning operation, write to valid first such
    ** that ID and VA are set after the entry is disabled
    */
    else {

        mp_mgen_field_IPT_VALID_set(NULL, mp_mgen_handle, ts_loc, valid);

        mp_mgen_field_IPT_VALID_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, valid);
        mp_mgen_field_IPT_ID_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, chnl_id);
        mp_mgen_field_IPT_VA_set(mp_mgen_buffer, mp_mgen_handle, ts_loc, occurrence);
    }
        
    mp_mgen_buffer_flush(mp_mgen_buffer);

    PMC_RETURN(PMC_SUCCESS);
} /* mp_mgen_ipt_cfg */                                       

/*******************************************************************************
* mp_mgen_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clean a set of channels
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   *chnl               - set of channel ID to be cleaned
*   num_chnl            - number of channel ID to be cleaned
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
PUBLIC void mp_mgen_chnl_clean(mp_mgen_handle_t *mp_mgen_handle, 
                               UINT32 *chnl,
                               UINT32 num_chnl)                                   
{
    UINT32 chnl_itr, valid, id, ipt_itr;
    BOOL8 hit;

    PMC_ENTRY();  
    for(ipt_itr=0; ipt_itr<MP_MGEN_MAX_CHNL_NUM; ipt_itr++)
    {
        id    = mp_mgen_field_IPT_ID_get(NULL, mp_mgen_handle, ipt_itr);
        valid = mp_mgen_field_IPT_VALID_get(NULL, mp_mgen_handle, ipt_itr);
        hit = FALSE;
        for(chnl_itr = 0; chnl_itr < num_chnl && hit == FALSE;chnl_itr++)
        {  
            if (1 == valid && id == chnl[chnl_itr])
            {
                hit = TRUE;
                mp_mgen_field_IPT_VALID_set(NULL, mp_mgen_handle, ipt_itr, 0);
                mp_mgen_field_IPT_ID_set(NULL, mp_mgen_handle, ipt_itr,MP_MGEN_CHNL_ID_NIL);
                mp_mgen_field_IPT_VA_set(NULL, mp_mgen_handle, ipt_itr, 0);
                mp_mgen_amp_uncfg(mp_mgen_handle,ipt_itr);                
            }
        }
    }    
    for(chnl_itr = 0; chnl_itr < num_chnl;chnl_itr++)
    {
        mp_mgen_gmp_uncfg(mp_mgen_handle,chnl[chnl_itr]);
    }
    PMC_RETURN();        
} /* mp_mgen_chnl_clean */   

/*******************************************************************************
* mp_mgen_cycle_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clean a set of cycles
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   *cycle              - set of cycle ID to be cleaned
*   num_cycle           - number of cycle to be cleaned
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
PUBLIC void mp_mgen_cycle_clean(mp_mgen_handle_t *mp_mgen_handle, 
                                UINT32 *cycle,
                                UINT32 num_cycle)                                   
{
    UINT32 cycle_itr;


    PMC_ENTRY();
     
    for(cycle_itr = 0; cycle_itr < num_cycle ;cycle_itr++)
    {  
        mp_mgen_field_IPT_VALID_set(NULL, mp_mgen_handle, cycle[cycle_itr], 0);
        mp_mgen_field_IPT_ID_set(NULL, mp_mgen_handle, cycle[cycle_itr],MP_MGEN_CHNL_ID_NIL);
        mp_mgen_field_IPT_VA_set(NULL, mp_mgen_handle, cycle[cycle_itr], 0);
        mp_mgen_amp_uncfg(mp_mgen_handle,cycle[cycle_itr]); 
        mp_mgen_gmp_uncfg(mp_mgen_handle,cycle[cycle_itr]); 
    }

    PMC_RETURN();        
} /* mp_mgen_chnl_clean */


/*******************************************************************************
* mp_mgen_data_mask_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the quotient and remainder of data in one timeslot
*   to fit in one calendar cycle. 
*   
*   NOTE: This function is only required for GMP Processing.
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95   
*   num_tribslot        - M parameter in GMP. Number of tributary slots in 
*                         the LO channel               
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
PUBLIC PMC_ERROR mp_mgen_data_mask_gen_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                           UINT32 chnl_id, UINT32 num_tribslot)
{
    /* variable declaration */
    mp_mgen_cfg_t *cfg_ptr;
    UINT32 div, mod;
    mp_mgen_buffer_t mp_mgen_buffer[1];
    
    PMC_ENTRY();
    
    /* retrieve cfg struct */
    cfg_ptr = (mp_mgen_cfg_t *)(&mp_mgen_handle->cfg); 
    
    /* DIV_48_M where M = cfg_ptr->trib_slot_num*/
    /* MOD_48_M */
    PMC_ASSERT(num_tribslot != 0, MP_MGEN_ERR_CODE_ASSERT,0,0);
    div = 48 / (num_tribslot);
    mod = 48 % (num_tribslot);
    
    /* Set DIV and MOD values for GMP processing */
    mp_mgen_buffer_init(mp_mgen_buffer, mp_mgen_handle);
    mp_mgen_field_DIV_48_M_set(mp_mgen_buffer, mp_mgen_handle, chnl_id, div);
    mp_mgen_field_MOD_48_M_set(mp_mgen_buffer, mp_mgen_handle, chnl_id, mod);
    mp_mgen_buffer_flush(mp_mgen_buffer);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mp_mgen_data_mask_gen_cfg */

/*******************************************************************************
* mp_mgen_last_txr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the last transfer bytes which are the remaining 
*   bytes of data to be transferred in one calendar cycle. 
*
*   When this function is called from ODTU_MUX/DMX with the client type being
*   ODUFlex, the number of tributary slots the HO channel occupies must be 
*   known. For all the other cases, 'trib_slot' value will be ignored.
*   Similarly, tributary slot type ('ts_type') needs to be known in certain 
*   cases. These information is expected to be supplied by the calling function 
*   of the parent.
*   
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95 
*   server_type         - OPUk rate if used by TGMP/RGMP or
*                         HO ODUk rate if ODTU_MUX/DMX
*                         Refer to mp_mgen_oduk_enum.                              
*   client_type         - LO ODUj rate when used by ODTU_MUX/DMX
*                         For valid types, see mp_mgen_oduk_enum.
*                         This argument is ignored if used by TGMP/RGMP.
*   mapping_mode        - mapping procedure.  See mapping_mode_enum. 
*   ts_type             - tributary slot type \n
*                         Only used if ODTU_MUX/DMX is being used. Otherwise, 
*                         this argument is ignored. \n
*                         1 -- 1G25 \n
*                         2 -- 2G5 \n
*   trib_slot           - number of tributary slots in the LO channel if the 
*                         client_type is ODUFlex. Otherwise, this argument is
*                         ignored. 
*                         Valid range: 0 - 80 \n
*   m                   - number of tribslots used for the LO channel.
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
PUBLIC PMC_ERROR mp_mgen_last_txr_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                      UINT32 chnl_id, 
                                      util_global_odukp_type_t server_type, 
                                      util_global_odukp_type_t client_type,
                                      util_global_mapping_mode_t mapping_mode,
                                      UINT32 ts_type,
                                      UINT32 trib_slot,
                                      UINT32 m)
{
    /* variable declaration */
    UINT32 lst_bytes = 0;
    UINT32 div, mod;
    mp_mgen_buffer_t mp_mgen_buffer[1];
    
    PMC_ENTRY();
   
    lst_bytes = mp_mgen_last_txr_byte_size_calc(mp_mgen_handle, 
                                               server_type, 
                                               client_type,
                                               mapping_mode,
                                               ts_type,
                                               trib_slot);
    
    if (mapping_mode == UTIL_GLOBAL_GMP)
    {    
        /* DIV_48_M where M = cfg_ptr->trib_slot_num*/
        /* MOD_48_M */
        PMC_ASSERT(m != 0, MP_MGEN_ERR_CODE_ASSERT,0,0);
        div = 48 / (m);
        mod = 48 % (m);
    }
    else {
        div = 0;
        mod = 0;         
    }            
    
    /* Set DIV and MOD values for GMP processing */
    mp_mgen_buffer_init(mp_mgen_buffer, mp_mgen_handle);
    mp_mgen_field_DIV_48_M_set(mp_mgen_buffer, mp_mgen_handle, chnl_id, div);
    mp_mgen_field_MOD_48_M_set(mp_mgen_buffer, mp_mgen_handle, chnl_id, mod);
        
    /* write to the register field */ 
    mp_mgen_field_LAST_TXR_BYTES_set(mp_mgen_buffer, mp_mgen_handle, chnl_id, lst_bytes);
    mp_mgen_buffer_flush(mp_mgen_buffer); 
    
                          
    PMC_RETURN(PMC_SUCCESS);
    
} /* mp_mgen_last_txr_cfg */      

/*******************************************************************************
* mp_mgen_gmp_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function unsets the register fields used for GMP mapping and 
*   LAST_TXR_BYTES. 
*   
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   chnl_id             - channel ID to be configured
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
PUBLIC PMC_ERROR mp_mgen_gmp_uncfg(mp_mgen_handle_t *mp_mgen_handle, 
                                   UINT32 chnl_id)
{
    mp_mgen_buffer_t mp_mgen_buf[1];
    
    PMC_ENTRY();
    
    /* Set DIV and MOD values for GMP processing */
    mp_mgen_buffer_init(mp_mgen_buf, mp_mgen_handle);
    mp_mgen_field_LAST_TXR_BYTES_set(mp_mgen_buf, mp_mgen_handle, chnl_id, 0);
    mp_mgen_field_DIV_48_M_set(mp_mgen_buf, mp_mgen_handle, chnl_id, 0);
    mp_mgen_field_MOD_48_M_set(mp_mgen_buf, mp_mgen_handle, chnl_id, 0);
    mp_mgen_buffer_flush(mp_mgen_buf);
    
    PMC_RETURN(PMC_SUCCESS);
} /* mp_mgen_gmp_uncfg */ 

/*******************************************************************************
* mp_mgen_amp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the type of NJO opportunity, NJO position in a 
*   calendar cycle and NJO position in an ODTU/OPU container in 48 byte data
*   units.
*
*   This function is used only for AMP configuration.
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   cal_entry           - calendar entry to be configured
*                         Valid range: 0 - 95 
*   tribslot_pos        - tributary slot location. 
*                         Maximum valid range: 0 - 79 \n
*                         NOTE: the maximum valid range should be changed
*                               accordingly depending on the HO ODU type.
*   server_type         - HO ODUk type \n
*                         0 -- ODU1 \n
*                         1 -- ODU2 \n
*                         2 -- ODU3 \n
*                         3 -- ODU3E1 \n
*   client_type         - Client rate for TGMP/RGMP or LO ODUj type for ODTU_MUX/DMX \n 
*                         0 -- ODU0 \n
*                         1 -- ODU1 \n
*                         2 -- ODU2 \n
*                         3 -- ODU2E \n
*                         4 -- 10G SONET/SDH (ODU2P_OC_192_CBR_AMP) \n
*                         5 -- 40G SONET/SDH (ODU3P_OC_768_CB_AMP) \n
*   tribslot_type       - tributary slot type. See odu_line_payload_t.
*                         Only used for DMX/MUX. Set to 0 if this API is used
*                         for TGMP/RGMP. 
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
PUBLIC PMC_ERROR mp_mgen_amp_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                 UINT32 cal_entry, UINT32 tribslot_pos, 
                                 util_global_odukp_type_t server_type, 
                                 util_global_odukp_type_t client_type, 
                                 util_global_odu_line_payload_t tribslot_type)
{
    /* variable declaration */
    UINT32 njo_pos = 0;
    UINT32 col_size, real_njo_pos = 0;
    UINT32 njo_odtu_cycle = 0;
    UINT32 njo_type = 0;
    UINT32 frm_num = 0;
    mp_mgen_buffer_t mp_mgen_buffer[1];
    
    PMC_ENTRY();
        
    /* check if the requested tribslot is valid based on server/client types */
    switch (server_type)
    {
        case UTIL_GLOBAL_ODU1: /* ODU1 */
            if (client_type == UTIL_GLOBAL_ODU0) 
            {   /* ODU0 client */
                njo_type = 0;
                
                if (tribslot_pos > 1)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                
                col_size = ODU1_ODU0_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) - 1;     
            }
            break;
        case UTIL_GLOBAL_ODU2: /* ODU2 */
        case UTIL_GLOBAL_ODU2E:            
        case UTIL_GLOBAL_ODU2E_ETRANS:            
            if (client_type == UTIL_GLOBAL_ODU1 
                && tribslot_type == UTIL_GLOBAL_ODU_TS_1G25)
            {   /* ODU1 client with AMP 1.25G */
                njo_type = 1;
                
                if (tribslot_pos > 7)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                
                col_size = ODU2_ODU1_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) - 1;
            }
            else if (client_type == UTIL_GLOBAL_ODU1 
                     && tribslot_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   /* ODU1 client with AMP 2.5G */
                njo_type = 1;
                
                if (tribslot_pos > 3)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                
                col_size = ODU2_ODU1_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) - 1;
            }
            else if ((UINT32)client_type == (UINT32)UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP)
            {   /* 10G SONET/SDH client */
                if (tribslot_pos > 0)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                
                col_size = ODU2_10G_SONET_COL_SIZE;
                /* find the physical tributary slot location */
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) -1; 
            }    
            break;
        case UTIL_GLOBAL_ODU3: /* ODU3 */
            if (client_type == UTIL_GLOBAL_ODU1 
                && tribslot_type == UTIL_GLOBAL_ODU_TS_1G25)
            {   /* ODU1 client with AMP 1.25G */
                if (tribslot_pos > 31)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                njo_type = 1;
                col_size = ODU3_ODU1_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (tribslot_pos * 4 + 3) * col_size + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) -1; 
            }
            else if (client_type == UTIL_GLOBAL_ODU1 
                     && tribslot_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   /* ODU1 client with AMP2.5G */
                if (tribslot_pos > 15)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                njo_type = 1;
                col_size = ODU3_ODU1_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (tribslot_pos * 4 + 3) * col_size + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) -1; 
            }
            else if (client_type == UTIL_GLOBAL_ODU2 
                     && tribslot_type == UTIL_GLOBAL_ODU_TS_1G25)
            {   /* ODU2 client with AMP1.25G */
                if (tribslot_pos > 31)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                njo_type = 1;
                col_size = ODU3_ODU2_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (tribslot_pos * 4 + 3) * col_size + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) -1; 
            }
            else if (client_type == UTIL_GLOBAL_ODU2 
                     && tribslot_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   /* ODU2 client with AMP2.5G */
                if (tribslot_pos > 15)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                njo_type = 1;
                col_size = ODU3_ODU2_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) -1; 
            }
            else if ((UINT32)client_type == (UINT32)UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP)
            {   /* 40G SONET/SDH client */
                if (tribslot_pos > 0)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                njo_type = 0;
                col_size = ODU3_40G_SONET_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                njo_odtu_cycle = njo_pos / 48;
                real_njo_pos = (njo_pos % 48) - 1; 
            }  
            break;
       case UTIL_GLOBAL_ODU3E1: /* ODU3E1 */
            if ((client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                && tribslot_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (tribslot_pos > 15)
                {
                    PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
                }
                
                col_size = ODU3E1_ODU2E_COL_SIZE;
                
                PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "index for TS_position = %u\n", tribslot_pos);
                
                
                frm_num = mp_mgen_field_IPT_VA_get(NULL, mp_mgen_handle, cal_entry);
                
                /* query the database to get tribslot pos for */
                /* get all calendar entries for this LO channel ID */
                 
                
                njo_type = (frm_num % 2 == 0)? 2:1;
                
                if (njo_type == 2)
                {
                    njo_pos = (((tribslot_pos * 4) + 2) * col_size) + 1;
                    njo_odtu_cycle = njo_pos / 48;
                    real_njo_pos = (njo_pos % 48) - 1;
                }
                else 
                {
                    njo_pos = (((tribslot_pos * 4) + 3) * col_size) + 1;
                    njo_odtu_cycle = njo_pos / 48;
                    real_njo_pos = (njo_pos % 48) -1;
                }   
            }
            break;
        default: /* for all the other unsupported values */
            PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
    } /* end of switch */
    
    PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "njo_pos = %u\n", njo_pos);
    PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "njo_odtu_cycle = %u\n", njo_odtu_cycle);
    PMC_LOG(PMC_LOG_SEV_LOW, MP_MGEN_LOG_CODE_LOG, 0, 0, "real_njo_pos = %u\n", real_njo_pos);
    
    /* Configure NJO_POS, NJO_TYPE, and NJO_ODTU_CYCLE */
    mp_mgen_buffer_init(mp_mgen_buffer, mp_mgen_handle);
    mp_mgen_field_NJO_TYPE_set(mp_mgen_buffer, mp_mgen_handle, cal_entry, njo_type);
    mp_mgen_field_NJO_POS_set(mp_mgen_buffer, mp_mgen_handle, cal_entry, real_njo_pos);
    mp_mgen_field_NJO_ODTU_CYCLE_set(mp_mgen_buffer, mp_mgen_handle, cal_entry, njo_odtu_cycle);
    mp_mgen_buffer_flush(mp_mgen_buffer);
 
    PMC_RETURN(PMC_SUCCESS);   
} /* mp_mgen_amp_cfg */

/*******************************************************************************
* mp_mgen_ipt_va_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the IPT_VA for the specified calendar entry.
*
*   This function is used only for AMP configuration.
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   cal_entry           - calendar entry for this IPT 
*                         valid range: 0 - 95
*
* OUTPUTS:
*   ipt_va              - IPT virtual address for this calendar cycle
*                         Valid range: 0 - 95 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mp_mgen_ipt_va_get(mp_mgen_handle_t *mp_mgen_handle, 
                                    UINT32 cal_entry,
                                    UINT32 *ipt_va)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    *ipt_va = mp_mgen_field_IPT_VA_get(NULL, mp_mgen_handle, cal_entry);
    
    PMC_RETURN(result);
} /* mp_mgen_ipt_va_get */

/*******************************************************************************
* mp_mgen_amp_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures resets AMP related configurations for the specified
*   calendar entries of the LO channel.
*
*   This function is used only for AMP configuration.
*
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance 
*   cal_entry           - calendar entry to be configured
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
PUBLIC PMC_ERROR mp_mgen_amp_uncfg(mp_mgen_handle_t *mp_mgen_handle, 
                                   UINT32 cal_entry)
{
    mp_mgen_buffer_t mp_mgen_buf[1];

    PMC_ENTRY();
        
    /* Configure NJO_POS, NJO_TYPE, and NJO_ODTU_CYCLE */
    mp_mgen_buffer_init(mp_mgen_buf, mp_mgen_handle);
    mp_mgen_field_NJO_TYPE_set(mp_mgen_buf, mp_mgen_handle, cal_entry, 0);
    mp_mgen_field_NJO_POS_set(mp_mgen_buf, mp_mgen_handle, cal_entry, 0);
    mp_mgen_field_NJO_ODTU_CYCLE_set(mp_mgen_buf, mp_mgen_handle, cal_entry, 0);
    mp_mgen_buffer_flush(mp_mgen_buf);
     
    PMC_RETURN(PMC_SUCCESS);   
} /* mp_mgen_amp_uncfg */ 
  

/*
** Private Functions
*/
/*******************************************************************************
* mp_mgen_last_txr_byte_size_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the transfer byte size of ODTUjk/ODTUk.ts/OPUk
*   size in the last DCI transaction.
*   
* INPUTS:
*   *mp_mgen_handle     - pointer to MP_MGEN handle instance
*   server_type         - OPUk rate if used by TGMP/RGMP or
*                         HO ODUk rate if ODTU_MUX/DMX
*                         Refer to mp_mgen_oduk_enum.                              
*   client_type         - LO ODUj rate when used by ODTU_MUX/DMX
*                         For valid types, see mp_mgen_oduk_enum.
*                         This argument is ignored if used by TGMP/RGMP.
*   mapping_mode        - mapping procedure.  See mapping_mode_enum. 
*   ts_type             - tributary slot type \n
*                         Only used if ODTU_MUX/DMX is being used. Otherwise, 
*                         this argument is ignored. \n
*                         1 -- 1G25 \n
*                         2 -- 2G5 \n
*   trib_slot           - number of tributary slots in the LO channel if the 
*                         client_type is ODUFlex. Otherwise, this argument is
*                         ignored. 
*                         Valid range: 0 - 80 \n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32             - The transfer byte size
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 mp_mgen_last_txr_byte_size_calc(mp_mgen_handle_t *mp_mgen_handle, 
                                               util_global_odukp_type_t server_type, 
                                               util_global_odukp_type_t client_type,
                                               util_global_mapping_mode_t mapping_mode,
                                               UINT32 ts_type,
                                               UINT32 trib_slot)
{
    /* variable declaration */
    UINT32 lst_bytes = 0;
    UINT32 payload_size;
   
    PMC_ENTRY();
   
    /* last_txr_byte cfg is dependent on ODTU/OPUk container size */
    /* Need to get client type and server type */   
    /* TGMP/RGMP */
    switch (server_type)
    {
        case UTIL_GLOBAL_ODU0: /* ODU0 - only for TGMP/RGMP */
            if (mapping_mode == UTIL_GLOBAL_GMP) 
            {
                lst_bytes = 16; 
            }
            break;
        case UTIL_GLOBAL_ODU1: /* ODU1 */    
            if (mapping_mode == UTIL_GLOBAL_GMP) 
            {
                lst_bytes = 16; 
            }
            else if (client_type == UTIL_GLOBAL_ODU0 
                     && mapping_mode == UTIL_GLOBAL_AMP && ts_type == 1)
            { /* LO_HO mapping: ODU0 client with AMP 1.25G */
                lst_bytes = 16;
            }
            else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                     || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
            { /* LO_HO mapping: ODUFlex */
                payload_size = 15232 * trib_slot;
                lst_bytes = payload_size % 48;
            }
            break;
        case UTIL_GLOBAL_ODU1E: /* ODU1E */
        case UTIL_GLOBAL_ODU1E_ETRANS: /* ODU1E in MAPOTN sourced from ETRANS */
            if (mapping_mode == UTIL_GLOBAL_BMP)
            { 
                lst_bytes = 16;
            }
            break;
        case UTIL_GLOBAL_ODU1F: /* ODU1F */
            if (mapping_mode == UTIL_GLOBAL_BMP) /* ODU1FP_FC_1200_BMP */
            {
                lst_bytes = 16;
            }
            break;
        case UTIL_GLOBAL_ODU2: /* ODU2 */
            if ((UINT32)mapping_mode == (UINT32)UTIL_GLOBAL_GMP && (UINT32)client_type <= (UINT32)UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP) /* ODU2P_FC_800_GMP */  
            {
                lst_bytes = 16; 
            }
            else if ((mapping_mode == UTIL_GLOBAL_AMP || mapping_mode == UTIL_GLOBAL_BMP)
                    && (UINT32)client_type < (UINT32)UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
            {
                lst_bytes = 48;
            }
            else if ((UINT32)client_type == (UINT32)UTIL_GLOBAL_ODU0 
                     && mapping_mode == UTIL_GLOBAL_GMP && ts_type == 1) 
            { /* ODU0 with GMP 1.25G */
                lst_bytes = 16;
            }
            else if (client_type == UTIL_GLOBAL_ODU1 
                     && mapping_mode == UTIL_GLOBAL_AMP)
            {
                lst_bytes = (ts_type == 1)? 32:16;
            }
            else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                     || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
            {   /* LO_HO mapping: ODUFlex client with GMP1.25G */ 
                payload_size = 15232 * trib_slot; 
                lst_bytes = payload_size % 48;
                if (lst_bytes == 0) lst_bytes = 48;
            }
            break;
        case UTIL_GLOBAL_ODU2E: /* ODU2E */
        case UTIL_GLOBAL_ODU2E_ETRANS: /* ODU2E in MAPOTN from ENET */
             if (mapping_mode == UTIL_GLOBAL_GMP) 
             { /* client rate = Transcoded & GFP framed FC1200, 10GE, 
                                or 10G GDPS */
                lst_bytes = 16;
             }  
             else if(mapping_mode == UTIL_GLOBAL_BMP)
             {
                lst_bytes = 48;
             }
             break; 
        case UTIL_GLOBAL_ODU2F: /* ODU2F */
            if (mapping_mode == UTIL_GLOBAL_BMP)
            { /* ODU2FP_FC_1200_BMP */
                lst_bytes = 48;
            }
            break;
        case UTIL_GLOBAL_ODU3: /* ODU3 */
            if (mapping_mode == UTIL_GLOBAL_GMP && (UINT32)client_type < (UINT32)UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
            { /* ODU3P_40_GE_GMP or ODUJP_CBRX_GMP */
                lst_bytes = 16;
            }
            else if ((mapping_mode == UTIL_GLOBAL_AMP || mapping_mode == UTIL_GLOBAL_BMP)
                    && (UINT32)client_type < (UINT32)UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
            { /* ODU3P_OC_768_CB_BMP or ODU3P_OC_768_CB_AMP */
                lst_bytes = 32;
            }
            else if (mapping_mode == UTIL_GLOBAL_GMP && ts_type == 1 
                     && client_type == UTIL_GLOBAL_ODU0)
            {   
                lst_bytes = 16;
            }
            else if (client_type == UTIL_GLOBAL_ODU1 
                     && mapping_mode == UTIL_GLOBAL_AMP)
            {
                lst_bytes = (ts_type == 1)? 32:16;
            }
            else if (client_type == UTIL_GLOBAL_ODU2 
                     && mapping_mode == UTIL_GLOBAL_AMP)
            {
                trib_slot = (ts_type == 1)? 8:4;
                payload_size = 15232 * trib_slot; 
                lst_bytes = payload_size % 48;
            }
            else if ((client_type == UTIL_GLOBAL_ODU2E 
                       || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                       || client_type == UTIL_GLOBAL_ODU2F
                       || client_type == UTIL_GLOBAL_ODU1E 
                       || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                       || client_type == UTIL_GLOBAL_ODU1F) 
                     && (mapping_mode == UTIL_GLOBAL_GMP))
            {   /* LO_HO mapping: ODU2E client with GMP 1.25G */
                /* LO_HO mapping: ODU2F client with GMP 1.25G */
                /*lst_bytes = 0;*/
                payload_size = 15232 * trib_slot; 
                lst_bytes = payload_size % 48;
                if (lst_bytes == 0) lst_bytes = 48;
            }
            else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                     || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
            {
                payload_size = 15232 * trib_slot; 
                lst_bytes = payload_size % 48;
                if (lst_bytes == 0) lst_bytes = 48;
            }
            break;
        case UTIL_GLOBAL_ODU3E1: /* ODU3E1 */
            if (mapping_mode == UTIL_GLOBAL_AMP && ts_type == 2)
            {   /* LO_HO mapping: ODU2E client with AMP 2.5G */
                lst_bytes = 16;
            }
            break;
        case UTIL_GLOBAL_ODU3E2: /* ODU3E2 */
            
            if (mapping_mode == UTIL_GLOBAL_GMP && (UINT32)client_type == (UINT32)UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP)
            { /* TGMP/RGMP */
                lst_bytes = 16;
            }
            else
            {   /* LO_HO mapping: ODUFlex client with GMP 1.25G */
                payload_size = 15232 * trib_slot; 
                lst_bytes = payload_size % 48;
                if (lst_bytes == 0) lst_bytes = 48;
            }
            break;
        case UTIL_GLOBAL_ODU4: /* ODU4 */
            if (mapping_mode == UTIL_GLOBAL_GMP)
            {   /* TGMP/RGMP */ 
                lst_bytes = 32;
                
                if ((UINT32)client_type > (UINT32)UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
                {
                    payload_size = 15200 * trib_slot; 
                    lst_bytes = payload_size % 48;
                    if (lst_bytes == 0) lst_bytes = 48;
                }    
            }
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR: /* ODUFlex */
            if (mapping_mode == UTIL_GLOBAL_BMP)
            {
                lst_bytes = 16;
            }
            break;
        case UTIL_GLOBAL_ODUFLEX_GFP: /* ODUFlex */
            if (mapping_mode == UTIL_GLOBAL_BMP)
            {
                lst_bytes = 16;
            }
            break;
        default:
            PMC_RETURN(MP_MGEN_ERR_INVALID_PARAMETERS);
    }   

    PMC_LOG_TRACE("LAST_TXR %u", lst_bytes);
    
                                        
    PMC_RETURN(lst_bytes);
    
} /* mp_mgen_last_txr_byte_size_calc */     
       


/*
** End of file
*/
