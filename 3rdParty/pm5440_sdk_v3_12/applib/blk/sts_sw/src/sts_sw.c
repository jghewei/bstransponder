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

#include "sts_sw_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/** @brief Field offsets for the CMD_CFG_STS_SW_Set_Config host message */
#define HOSTMSG_MSGCFG_SET_STS_SW_CFG_NUM_TRIBSLOT_BIT_OFF          24
#define HOSTMSG_MSGCFG_SET_STS_SW_CFG_LO_CHNL_ID_OFFSET             4
#define HOSTMSG_MSGCFG_SET_STS_SW_CFG_CAL_ENTRY_OFFSET              24
#define HOSTMSG_MSGCFG_SET_STS_SW_CFG_TARGET_REG_MASK_BIT_OFFSET    16
#define HOSTMSG_MSGCFG_SET_STS_SW_CFG_HO_CAL_ENTRY_OFFSET           4

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
const char STS_SW_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    STS_SW_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char STS_SW_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void sts_sw_var_default_init(sts_sw_handle_t *sts_sw_handle);
 
PRIVATE PMC_ERROR sts_sw_eomf_cnt_calc(sts_sw_handle_t *sts_sw_handle, 
                                       UINT32 chnl_id,
                                       UINT32 *eomf_cnt);

/*
** Public Functions
*/

/*******************************************************************************
* sts_sw_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an STS_SW block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the STS_SW subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifer of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   sts_sw_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC sts_sw_handle_t *sts_sw_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    PMC_ENTRY();
    
    sts_sw_handle_t *sts_sw_handle;

    sts_sw_handle = (sts_sw_handle_t*)PMC_CTXT_CALLOC(sizeof(sts_sw_handle_t), parent);
    
    pmc_handle_init(parent, sts_sw_handle, sys_handle, PMC_MID_DIGI_STS_SW, tsb_name, base_address);
    
    /* Register log strings */
    pmc_log_block_strings_register(STS_SW_LOG_ERR_STRINGS[0], STS_SW_LOG_ERR_TABLE_BASE, STS_SW_LOG_ERR_COUNT);
    
    PMC_RETURN(sts_sw_handle);
} /* sts_sw_ctxt_create */

 
/*******************************************************************************
* sts_sw_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a STS_SW block instance.
*
*
* INPUTS:
*   *sts_sw_handle         - pointer to STS_SW handle instance
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
PUBLIC void sts_sw_ctxt_destroy(sts_sw_handle_t *sts_sw_handle)
{
    UINT32 i;
    PMC_ENTRY();
    
    for (i = 0; i < 96; i++)
    {
        sts_sw_chnl_ctxt_destroy(sts_sw_handle, i); 
    }
    
    PMC_CTXT_FREE(&sts_sw_handle, sts_sw_handle);
    
    PMC_RETURN();
} /* sts_sw_ctxt_destroy */



/*******************************************************************************
* sts_sw_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an STS_SW block handle instance.
*
*
* INPUTS:
*   *sts_sw_handle         - pointer to STS_SW handle instance
*   sts_sw_instance        - enum for sts_sw instace. See sts_sw_inst_type_t.
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
PUBLIC void sts_sw_handle_init(sts_sw_handle_t *sts_sw_handle, 
                               sts_sw_inst_type_t sts_sw_instance)
{    
    PMC_ENTRY();

    sts_sw_var_default_init(sts_sw_handle);
    sts_sw_handle->var.inst_type = sts_sw_instance;
    
    PMC_RETURN();    
} /* sts_sw_handle_init */

/*******************************************************************************
* sts_sw_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on sts_sw.
*
*
* INPUTS:
*   *sts_sw_handle        - pointer to STS_SW handle instance
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
PUBLIC PMC_ERROR sts_sw_handle_restart_init(sts_sw_handle_t *sts_sw_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(STS_SW_LOG_ERR_STRINGS[0], STS_SW_LOG_ERR_TABLE_BASE, STS_SW_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* sts_sw_handle_restart_init */

/*******************************************************************************
* sts_sw_chnl_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys channel related context information in the STS_SW.
*
*
* INPUTS:
*   *sts_sw_handle           - pointer to STS_SW handle instance
*   chnl_id                  - HO channel ID
*                              Legal range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_chnl_ctxt_destroy(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id)
{
    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    PMC_RETURN(PMC_SUCCESS); 
} /* sts_sw_chnl_ctxt_destroy */

/*******************************************************************************
* sts_sw_output_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a STS_SW block instance.
*
*
* INPUTS:
*   *sts_sw_handle         - pointer to STS_SW handle instance
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
PUBLIC PMC_ERROR sts_sw_output_ctxt_destroy(sts_sw_handle_t *sts_sw_handle)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    PMC_RETURN(PMC_SUCCESS);
    
} /* sts_sw_output_ctxt_destroy */

/*******************************************************************************
* sts_sw_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an STS_SW block instance.
*
*
* INPUTS:
*   *sts_sw_handle         - pointer to STS_SW handle instance.
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
PUBLIC UINT32 sts_sw_base_address_get(sts_sw_handle_t *sts_sw_handle)
{
    PMC_ENTRY();
    
    PMC_RETURN(sts_sw_handle->base.base_address);
} /* sts_sw_base_address_get */


/*******************************************************************************
* sts_sw_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables STS_SW block and configures its  
*   operational mode. This function configures the switch to be in MUX or DEMUX
*   mode.
*
*   When STS_SW is disabled, the state-machines will remain in initial state and
*   all counters will reset.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   mode                - operational mode for STS_SW \n
*                         0 -- MUX mode; LO to HO multiplexing \n
*                         1 -- DEMUX mode; HO to LO multiplexing
*   enbl                - enables or disables STS_SW block. \n
*                         0 -- disable STS_SW; all counters will not run. \n
*                         1 -- enables STS_SW    
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
PUBLIC PMC_ERROR sts_sw_init(sts_sw_handle_t *sts_sw_handle, UINT32 mode, 
                             UINT32 enbl)
{
    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    
    if (pmc_is_digi_rev_a_revision(&sts_sw_handle->base) == TRUE)
    {
        /* Configure STS_SW_EN bit */
        sts_sw_reg_CONFIG_write(NULL, sts_sw_handle, enbl);
    }

    /* Configure MXDX_MODE_EN bit */
    sts_sw_field_MXDX_MODE_EN_set(NULL, sts_sw_handle, mode);

     
    PMC_RETURN(PMC_SUCCESS); 
} /* sts_sw_init */ 

/*******************************************************************************
* sts_sw_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ram based configuration.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
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
PUBLIC PMC_ERROR sts_sw_ram_init(sts_sw_handle_t *sts_sw_handle)
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
   
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_CAL_ENTRY; i++)
    { 
        sts_sw_reg_CALENDAR_CHANNEL_ID_array_write( NULL, sts_sw_handle, i, 0x0000007F);
        sts_sw_reg_IPT_array_write( NULL, sts_sw_handle, i, 0x00007F00);
        sts_sw_reg_HO_CFG_array_write(NULL, sts_sw_handle, i, 0x00000000);
        sts_sw_reg_HO_CYCLE_CFG_array_write(NULL, sts_sw_handle, i, 0x00000000);
    }
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_CAL_ENTRY; i++)
    { 
        sts_sw_reg_IPT_array_write(NULL, sts_sw_handle, i, 0x00007F00);
    }

    /* initialize all ram registers to 0x0 */
    sts_sw_calendar_init(sts_sw_handle);
    
    PMC_RETURN(PMC_SUCCESS); 
} /* sts_sw_ram_init */ 
                               
/*******************************************************************************
* sts_sw_ho_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the switch settings for each HO channel. When the
*   switch is configured as MUX, this function works on the egress HO channel.
*   When the switch is configured as DEMUX, this function works on the ingress
*   HO channel.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_ho_chnl_cfg(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    sts_sw_buffer_t sts_sw_buf[1];
    UINT32 num_ts;
    UINT32 eomf_cnt;
    
    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id > 95)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
   
    /* number of tribslots in the HO channel */
    num_ts = sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot;
    
    result = sts_sw_eomf_cnt_calc(sts_sw_handle, chnl_id, &eomf_cnt);
    
    if (PMC_SUCCESS == result)
    {
        sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
        /* Set EOMF_CNT */ 
        sts_sw_field_EOMF_CNT_set(sts_sw_buf, sts_sw_handle, chnl_id, eomf_cnt);
    
        /* Set LAST_CNT field*/
        sts_sw_field_LAST_CNT_set(sts_sw_buf, sts_sw_handle, chnl_id, num_ts);
        
        /* Set HO_CNT_TOT field */
        sts_sw_field_HO_CNT_TOT_set(sts_sw_buf, sts_sw_handle, chnl_id, num_ts);
        sts_sw_buffer_flush(sts_sw_buf); 
    }
    
    PMC_RETURN(result);
} /* sts_sw_ho_chnl_cfg */

/*******************************************************************************
* sts_sw_ho_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function unprovisions the HO channel related configurations in STS_SW.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_ho_chnl_deprov(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    sts_sw_buffer_t sts_sw_buf[1];
    PMC_ENTRY();
    
    sts_sw_field_EOMF_CNT_set(NULL, sts_sw_handle, chnl_id, 0);
    sts_sw_field_LAST_CNT_set(NULL, sts_sw_handle, chnl_id, 0);
    sts_sw_field_HO_CNT_TOT_set(NULL, sts_sw_handle, chnl_id, 0);  
   
    
    sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
    sts_sw_field_EOMF_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 0); 
    sts_sw_field_LAST_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 0); 
    sts_sw_field_FIRST_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 0);
    sts_sw_buffer_flush(sts_sw_buf);


    PMC_RETURN(result);
} /* sts_sw_ho_chnl_deprov */




/*******************************************************************************
* sts_sw_ho_calendar_cfg_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function unprovisions the HO channel related configurations in STS_SW.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cal_entry           - calendar entry to be deprovisioned
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_ho_calendar_cfg_deprov(sts_sw_handle_t *sts_sw_handle, 
                                               UINT32 cal_entry)
{
    sts_sw_buffer_t sts_sw_buf[1];
    PMC_ENTRY();
    
    sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
    sts_sw_field_IPT_VALID_set(sts_sw_buf, sts_sw_handle, cal_entry, 0);
    sts_sw_field_IPT_ID_set(sts_sw_buf, sts_sw_handle, cal_entry, 0x7F);
    sts_sw_field_IPT_VA_set(sts_sw_buf, sts_sw_handle, cal_entry, 0);
    sts_sw_buffer_flush(sts_sw_buf);
                               
    PMC_RETURN(PMC_SUCCESS);
} /* sts_sw_ho_calendar_cfg_deprov */




/*******************************************************************************
* sts_sw_ho_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function unprovisions the HO channel related configurations in STS_SW.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   *ho_chnl            - set of HO channel ID to be cleaned
*   num_ho_chnl         - number of channel ID to be cleaned
*   *lo_chnl            - set of channel ID to be cleaned
*   num_lo_chnl         - number of channel ID to be cleaned
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
PUBLIC PMC_ERROR sts_sw_ho_clean(sts_sw_handle_t *sts_sw_handle, 
                                 UINT32           *ho_chnl,
                                 UINT32           num_ho_chnl,
                                 UINT32           *lo_chnl,
                                 UINT32           num_lo_chnl)
{
    UINT32 lo_chnl_itr, cal_entry_itr, ho_chnl_itr, ipt_entry_itr;
    BOOL8 hit, valid, id; 
    sts_sw_buffer_t sts_sw_buf[1];

    PMC_ENTRY();
     
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl;ho_chnl_itr++)
    {
        sts_sw_field_EOMF_CNT_set(NULL, sts_sw_handle, ho_chnl[ho_chnl_itr], 0);
        sts_sw_field_LAST_CNT_set(NULL, sts_sw_handle, ho_chnl[ho_chnl_itr], 0);
        sts_sw_field_HO_CNT_TOT_set(NULL, sts_sw_handle, ho_chnl[ho_chnl_itr], 0);
        /* clean up context */
        sts_sw_handle->var.ho_chnl_cfg[ho_chnl[ho_chnl_itr]].oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
        sts_sw_handle->var.ho_chnl_cfg[ho_chnl[ho_chnl_itr]].chnl_id = 0xFF;
        sts_sw_handle->var.ho_chnl_cfg[ho_chnl[ho_chnl_itr]].num_tribslot = 0;
        for (lo_chnl_itr = 0; lo_chnl_itr < 96; lo_chnl_itr++)
        {
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl[ho_chnl_itr]].cal_entry[lo_chnl_itr] = 0x7F;
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl[ho_chnl_itr]].lo_chnl_ids_ptr[lo_chnl_itr] = 0x7F;
        }
    }
    
    /* clean IPT entries */        
    for (ipt_entry_itr = 0; ipt_entry_itr < NUM_STS_SW_CHNL;ipt_entry_itr++)
    {        
        valid = sts_sw_field_IPT_VALID_get(NULL, sts_sw_handle, ipt_entry_itr);
        if (valid == TRUE) 
        {
            id = sts_sw_field_IPT_ID_get(NULL, sts_sw_handle, ipt_entry_itr);
            hit = FALSE;
            for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {
                if (id == ho_chnl[ho_chnl_itr])
                {
                    sts_sw_field_IPT_VALID_set(NULL, sts_sw_handle, ipt_entry_itr, 0);
                    sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
                    sts_sw_field_IPT_VALID_set(sts_sw_buf, sts_sw_handle, ipt_entry_itr, 0);
                    sts_sw_field_IPT_ID_set(sts_sw_buf, sts_sw_handle, ipt_entry_itr, 0x7F);
                    sts_sw_field_IPT_VA_set(sts_sw_buf, sts_sw_handle, ipt_entry_itr, 0);
                    sts_sw_buffer_flush(sts_sw_buf);
                    
                    hit = TRUE;
                } 
            }
        }
    }
    /* clean lo context */
    for (ho_chnl_itr = 0; ho_chnl_itr < 95;ho_chnl_itr++)
    {
        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot != 0)
        {
            for (cal_entry_itr = 0; cal_entry_itr< sts_sw_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot; cal_entry_itr++)
            {
                if ( sts_sw_handle->var.ho_chnl_cfg[ho_chnl_itr].lo_chnl_ids_ptr[cal_entry_itr] != 0x7F)
                {
                    hit = FALSE;
                    for(lo_chnl_itr= 0; lo_chnl_itr < num_lo_chnl && hit == FALSE; lo_chnl_itr++)
                    {
                        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_itr].lo_chnl_ids_ptr[cal_entry_itr] == lo_chnl[lo_chnl_itr])
                        {
                            hit = TRUE;
                            sts_sw_handle->var.ho_chnl_cfg[ho_chnl_itr].lo_chnl_ids_ptr[cal_entry_itr] = 0x7f;                          
                        }
                    }
                }
            }
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* sts_sw_ho_clean */


/*******************************************************************************
* sts_sw_ho_ipt_valid_reset 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clears IPT_VALID for all calendar entries of HO channel.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cal_entry           - calendar entry of HO ODUk channel
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_ho_ipt_valid_reset(sts_sw_handle_t *sts_sw_handle, 
                                           UINT32 cal_entry)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    sts_sw_field_IPT_VALID_set(NULL, sts_sw_handle, cal_entry, 0);
    
    PMC_RETURN(result);
} /* sts_sw_ho_ipt_valid_reset */

/*******************************************************************************
* sts_sw_ho_chnl_ts_cfg 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the tributary slot context information and HO 
*   channel ID carried in the tributary slot.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_ho_chnl_ts_cfg(sts_sw_handle_t *sts_sw_handle, 
                                       UINT32 chnl_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 ts_num;
    UINT32 i, eomf_cnt, idx;
    UINT32 ts_loc = 0;
    sts_sw_buffer_t sts_sw_buf[1];

    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id > 95)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    ts_num = sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot;
    
    /* set IPT_VALID and IPT_ID for the HO channel */
    
    for (i = 0; i < ts_num; i++)
    {
        ts_loc = sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i];
        
        sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
        sts_sw_field_IPT_ID_set(sts_sw_buf, sts_sw_handle, 
                                sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i], 
                                chnl_id);
        sts_sw_field_IPT_VA_set(sts_sw_buf, sts_sw_handle, 
                                sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i], i);
        sts_sw_buffer_flush(sts_sw_buf);
        sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
        sts_sw_field_IPT_VALID_set(sts_sw_buf, sts_sw_handle, 
                                   sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i], 
                                   1);       
        sts_sw_field_IPT_ID_set(sts_sw_buf, sts_sw_handle, 
                                sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i], 
                                chnl_id);
        sts_sw_field_IPT_VA_set(sts_sw_buf, sts_sw_handle, 
                                sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[i], i);
        sts_sw_buffer_flush(sts_sw_buf);
    }
    
    /* read EOMF_CNT and EOMF_CYCLE should be the calendar entry of EOMF_CNT-1 */
    result = sts_sw_eomf_cnt_calc(sts_sw_handle, chnl_id, &eomf_cnt);
     
    
    if (PMC_SUCCESS == result)
    {
        if (eomf_cnt == 0)
        {
            idx = ts_num-1;
        }
        else {
            idx = eomf_cnt - 1;
        }
    
        PMC_LOG_TRACE("eomf_cycle = %u\n", sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[idx]);
        sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
        sts_sw_field_LAST_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 
                                    sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[ts_num-1]);                                     
        sts_sw_field_FIRST_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 
                                     sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[0]);
        sts_sw_field_EOMF_CYCLE_set(sts_sw_buf, sts_sw_handle, chnl_id, 
                                    sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry[idx]);    
        sts_sw_buffer_flush(sts_sw_buf);
    }
           
    PMC_RETURN(result);
} /* sts_sw_ho_chnl_ts_cfg */    

/*******************************************************************************
* sts_sw_ho_last_cycle_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the last cycle used for the HO. 
*   This function is used to set the last cycle of MO ODU3e2 or MO ODU3e1.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel
*                         Valid range: 0 - 95
*   last_cycle          - Last cycle of the HO channel
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
PUBLIC PMC_ERROR sts_sw_ho_last_cycle_set(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 chnl_id, UINT32 last_cycle)
{
    PMC_ENTRY();
    
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id > 95 || last_cycle > 95)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    sts_sw_field_LAST_CYCLE_set(NULL, sts_sw_handle, chnl_id, last_cycle);
                                
    PMC_RETURN(result);                                
    
} /* sts_sw_ho_last_cycle_set */    

/*******************************************************************************
* sts_sw_lo_chnl_ids_get 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves an array of all LO channel IDs in a HO.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   ho_chnl_id          - HO ODUk channel
*                         Valid range: 0 - 95
*  
* OUTPUTS:
*   *lo_chnl_ids        - output array to store all LO channel IDs within the HO
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_lo_chnl_ids_get(sts_sw_handle_t *sts_sw_handle, 
                                        UINT32 ho_chnl_id, UINT32 *lo_chnl_ids)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 idx, cnt;
    BOOL8 unique_array[96];
    
    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (ho_chnl_id > 95)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    /* initialize output array */
    for (i = 0; i < 96; i++)
    {
        lo_chnl_ids[i] = 0x7F;
        unique_array[i] = FALSE;
    }
    
    idx = 0;
    cnt = 0;
    
    for (i = 0; i < 96; i++)
    {
        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i] != 0x7F)
        {
            unique_array[sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i]] = TRUE;
            idx++;
        }
    }

    /* copy over to the return array */
    for (i = 0; i < 96; i++)
    {
        if (unique_array[i] == TRUE)
        {
            lo_chnl_ids[cnt] = i;
            cnt++;
        }

        if (cnt == idx)
        {
            break;
        }
    }
             
                                
    PMC_RETURN(result);                                
    
} /* sts_sw_lo_chnl_ids_get */    
   

/*******************************************************************************
* sts_sw_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the switch configuration and applies the output to
*   the registers. 
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel \n
*                         Valid range: 0 - 95
*   cfg_all             - boolean to indicate if both DMX and MUX STS_SW will
*                         will be configured in all call.
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
PUBLIC PMC_ERROR sts_sw_cfg(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id, BOOL8 cfg_all)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* 
     * Call firmware host message to generate STS_SW configuration and apply
     * the configuration to the device.
     */
    if (cfg_all == TRUE)
    {
        result = sts_sw_cfg_all_by_fw(sts_sw_handle, chnl_id);
    }
    else {
        result = sts_sw_cfg_by_fw(sts_sw_handle, chnl_id);
    }
    
    PMC_RETURN(result);    

} /* End: sts_sw_cfg() */

/*******************************************************************************
* sts_sw_cfg_by_fw
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Triggers firmware to generate configuration data for STS_SW and write the 
*  configuration data to the STS_SW registers by calling the 
*  CMD_CFG_CoreOTN_Set_STS_SW_Config host message.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel \n
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function uses buffers and services provided by the host 
*   message module. The host message context must be created and initialized 
*   (using hostmsg_ctxt_create() and hostmsg_handle_init()) before calling 
*   this function. 
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[42];
    UINT32 i;
    UINT8 *lo_chnl_src_ptr;
    UINT8 *cal_src_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sts_sw_handle, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl_id < NUM_STS_SW_CHNL, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    switch (sts_sw_handle->base.base_address)
    {
    case 0xC50000:
    case 0xC70000:
    case 0xD50000:
    case 0xD70000:
        break;
    default:
        result = HOSTMSG_ERR_INVALID_PARAMETERS;
        break;
    }

    if (PMC_SUCCESS == result && 0 < sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot)
    {
        /* 
         * Base address and number of tributary slots.
         * The following base addresses are valid:
         * - FMF1 Demux STS_SW - 0xC50000
         * - FMF1 Mux STS_SW   - 0xC70000
         * - FMF2 Demux STS_SW - 0xD50000
         * - FMF2 Mux STS_SW   - 0xD70000
         */
        PMC_MEMSET(data, 0, sizeof(UINT32) * 42);
        data[0] = sts_sw_handle->base.base_address & 0xffffff; 
        data[1] = sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot << HOSTMSG_MSGCFG_SET_STS_SW_CFG_NUM_TRIBSLOT_BIT_OFF; 
        
        /* Copy LO channel and calendar information into the host message. */
        lo_chnl_src_ptr  = sts_sw_handle->var.ho_chnl_cfg[chnl_id].lo_chnl_ids_ptr;
        cal_src_ptr      = sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry;
        
        for (i = 0; i < sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot; i = i + sizeof(UINT32)) {
            data[i/4+2] = ((((UINT32)lo_chnl_src_ptr[i]) << 24) & 0xff000000) |
                ((((UINT32)lo_chnl_src_ptr[i+1]) << 16) & 0x00ff0000) |
                ((((UINT32)lo_chnl_src_ptr[i+2]) << 8) & 0x0000ff00) |
                (((UINT32)lo_chnl_src_ptr[i+3]) & 0x000000ff);
            data[i/4+22] = ((((UINT32)cal_src_ptr[i]) << 24) & 0xff000000) |
                ((((UINT32)cal_src_ptr[i+1]) << 16) & 0x00ff0000) |
                ((((UINT32)cal_src_ptr[i+2]) << 8) & 0x0000ff00) |
                (((UINT32)cal_src_ptr[i+3]) & 0x000000ff);
        } /* end for */

        PMC_LOG_TRACE("Channel ID: %d\n", chnl_id);
        PMC_LOG_TRACE("sts_sw_cfg_by_fw host message contents:\n");
        for (i = 0; i < 42; i++) {
            PMC_LOG_TRACE("Word %d: 0x%x\n", i, data[i]);
        }
        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_start_cfg_message((pmc_handle_t *)sts_sw_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_CONFIG_ST,data,42,stat);
        }
        
    }

    PMC_RETURN(result);

} /* End: sts_sw_cfg_by_fw() */


/*******************************************************************************
* sts_sw_cfg_all_by_fw
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Triggers firmware to generate configuration data for STS_SW and write the 
*  configuration data to the STS_SW registers by calling the 
*  CMD_CFG_CoreOTN_Set_STS_SW_Config host message.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel \n
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function uses buffers and services provided by the host 
*   message module. The host message context must be created and initialized 
*   (using hostmsg_ctxt_create() and hostmsg_handle_init()) before calling 
*   this function. 
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_cfg_all_by_fw(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sts_sw_handle, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl_id < NUM_STS_SW_CHNL, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    switch (sts_sw_handle->base.base_address)
    {
    case 0xC50000:
    case 0xC70000:
    case 0xD50000:
    case 0xD70000:
        break;
    default:
        result = HOSTMSG_ERR_INVALID_PARAMETERS;
        break;
    }

    if (PMC_SUCCESS == result && 0 < sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot)
    {
        UINT32 i;
        UINT8 *lo_chnl_src_ptr;
        UINT8 *cal_src_ptr;
        UINT32 data[42];
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

        /* 
         * Base address and number of tributary slots.
         * The following base addresses are valid:
         * - FMF1 Demux STS_SW - 0xC50000
         * - FMF1 Mux STS_SW   - 0xC70000
         * - FMF2 Demux STS_SW - 0xD50000
         * - FMF2 Mux STS_SW   - 0xD70000
         */
        data[0] = sts_sw_handle->base.base_address & 0xffffff; 
        data[1] = sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot << HOSTMSG_MSGCFG_SET_STS_SW_CFG_NUM_TRIBSLOT_BIT_OFF; 
        
        /* Copy LO channel and calendar information into the host message. */
        lo_chnl_src_ptr  = sts_sw_handle->var.ho_chnl_cfg[chnl_id].lo_chnl_ids_ptr;
        cal_src_ptr      = sts_sw_handle->var.ho_chnl_cfg[chnl_id].cal_entry;
        
        for (i = 0; i < sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot; i = i + sizeof(UINT32)) 
        {
            data[2+i/4] = ((((UINT32)lo_chnl_src_ptr[i+3]) << 24) & 0xff000000) | \
                ((((UINT32)lo_chnl_src_ptr[i+2]) << 16) & 0x00ff0000) | \
                ((((UINT32)lo_chnl_src_ptr[i+1]) << 8) & 0x0000ff00) |  \
                (((UINT32)lo_chnl_src_ptr[i]) & 0x000000ff);
            data[22+i/4] = ((((UINT32)cal_src_ptr[i+3]) << 24) & 0xff000000) | \
                ((((UINT32)cal_src_ptr[i+2]) << 16) & 0x00ff0000) |     \
                ((((UINT32)cal_src_ptr[i+1]) << 8) & 0x0000ff00) |      \
                (((UINT32)cal_src_ptr[i]) & 0x000000ff);
        } /* end for */

        PMC_LOG_TRACE("Channel ID: %d\n", chnl_id);

        result = hostmsg_start_cfg_message((pmc_handle_t *)sts_sw_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_CONFIG_ALL_ST,data,42,stat);
    }

    PMC_RETURN(result);

} /* End: sts_sw_cfg_all_by_fw() */


/*******************************************************************************
* sts_sw_calendar_null_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function writes 0 to STS_SW ram registers that are associated with the
*   specified calendar entry.  
*   Used for ODU3e1/2 into a ODU4 HO configuration.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cal_entry           - calendar entry that needs to be nullified in STS config
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_calendar_null_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cal_entry)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);

    /* 
     * Call firmware host message to nullify the specified calendar entry
     */
    result = sts_sw_calendar_null_cfg_by_fw(sts_sw_handle, cal_entry);
    PMC_RETURN(result);    

} /* End: sts_sw_calendar_null_cfg() */
                                 
/*******************************************************************************
* sts_sw_calendar_null_cfg_by_fw
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Triggers firmware to write 0 to STS_SW ram registers that are 
*  associated with the specified calendar entry.  
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cal_entry           - calendar entry that needs to be nullified in STS config
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function uses buffers and services provided by the host 
*   message module. The host message context must be created and initialized 
*   (using hostmsg_ctxt_create() and hostmsg_handle_init()) before calling 
*   this function. 
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_calendar_null_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, 
                                                UINT32 cal_entry)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 data[2];

    PMC_ENTRY();

    PMC_ASSERT(NULL != sts_sw_handle, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cal_entry < NUM_STS_SW_CAL_ENTRY, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Base address and number of tributary slots */
    data[0] = sts_sw_handle->base.base_address; 
    data[1] = cal_entry << 24; 

    PMC_LOG_TRACE("sts_sw_calendar_null_cfg_by_fw host message contents:\n");
    for (i = 0; i < 2; i++) {
        PMC_LOG_TRACE("Word %d: 0x%x\n", i, data[i]);
    }

    /* Send message. */
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)sts_sw_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_NULL_CONFIG_ST,data,2,stat);
    }

    PMC_RETURN(result);

} /* End: sts_sw_calendar_null_cfg_by_fw() */


/*******************************************************************************
* sts_sw_calendar_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function writes 0 to the entire STS_SW ram registers.  
*   Used for each ODTU_MUX and ODTU_DMX block in the start up of the COREOTN
*   subsystem
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
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
PUBLIC PMC_ERROR sts_sw_calendar_init(sts_sw_handle_t *sts_sw_handle)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);

    /* 
     * Call firmware host message to nullify the specified calendar entry
     */
    result = sts_sw_calendar_init_by_fw(sts_sw_handle);
    PMC_RETURN(result);    

} /* End: sts_sw_calendar_init() */
                                 
/*******************************************************************************
* sts_sw_calendar_init_by_fw
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Triggers firmware to write 0 to the entire STS_SW ram registers.  
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function uses buffers and services provided by the host 
*   message module. The host message context must be created and initialized 
*   (using hostmsg_ctxt_create() and hostmsg_handle_init()) before calling 
*   this function. 
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_calendar_init_by_fw(sts_sw_handle_t *sts_sw_handle)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1];

    PMC_ENTRY();

    PMC_ASSERT(NULL != sts_sw_handle, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* Base address and number of tributary slots */
    data[0] = sts_sw_handle->base.base_address; 
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)sts_sw_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_INIT_ST,data,1,stat);
    }

    PMC_RETURN(result);

} /* End: sts_sw_calendar_init_by_fw() */


/*******************************************************************************
* sts_sw_ho_calendar_null_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function writes 0 to STS_SW ram registers that are associated with the
*   specified calendar entry.  
*   Used for ODU3e1/2 into a ODU4 HO configuration.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO channel ID
*                         Valid range: 0 - 95
*   num_tribslot        - number of tribslots in the HO container
*   cal_entry           - an array of calendar entries of HO
*   target_reg_mask     - bit mask to indicate the specific register of STS_SW
*                          
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_ho_calendar_null_cfg(sts_sw_handle_t *sts_sw_handle, 
                                             UINT8 chnl_id,
                                             UINT8 num_tribslot,
                                             UINT32 cal_entry[UTIL_GLOBAL_MAX_NUM_CAL_ENTRY],
                                             UINT8 target_reg_mask)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);

    /* 
     * Call firmware host message to nullify the specified calendar entry
     */
    result = sts_sw_ho_calendar_null_cfg_by_fw(sts_sw_handle, chnl_id, 
                                               num_tribslot, cal_entry,
                                               target_reg_mask);
    PMC_RETURN(result);    

} /* End: sts_sw_ho_calendar_null_cfg() */
                                 
/*******************************************************************************
* sts_sw_ho_calendar_null_cfg_by_fw
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Triggers firmware to write 0 to STS_SW ram registers that are 
*  associated with the specified calendar entry.  
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO channel ID
*                         Valid range: 0 - 95
*   num_tribslot        - number of tribslots in the HO container
*   cal_entry           - an array of calendar entries of HO
*   target_reg_mask     - bit mask to indicate the specific register of STS_SW
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This function uses buffers and services provided by the host 
*   message module. The host message context must be created and initialized 
*   (using hostmsg_ctxt_create() and hostmsg_handle_init()) before calling 
*   this function. 
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_ho_calendar_null_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, 
                                                   UINT8 chnl_id,
                                                   UINT8 num_tribslot,
                                                   UINT32 cal_entry[UTIL_GLOBAL_MAX_NUM_CAL_ENTRY],
                                                   UINT8 target_reg_mask)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 i;
    UINT32 msg_len = NUM_STS_SW_CAL_ENTRY/4+2;
    UINT32 data[NUM_STS_SW_CAL_ENTRY/4+2];

    PMC_ENTRY();

    PMC_ASSERT(NULL != sts_sw_handle, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl_id < NUM_STS_SW_CHNL, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(num_tribslot > 1 || num_tribslot < NUM_STS_SW_CAL_ENTRY, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);


    /* Base address and number of tributary slots */
    data[0] = sts_sw_handle->base.base_address; 
    data[1] = (num_tribslot << HOSTMSG_MSGCFG_SET_STS_SW_CFG_NUM_TRIBSLOT_BIT_OFF)
                + (target_reg_mask << HOSTMSG_MSGCFG_SET_STS_SW_CFG_TARGET_REG_MASK_BIT_OFFSET);

    /* Copy calendar information into the host message. */
    for (i = 0; i < num_tribslot; i = i + sizeof(UINT32))
    {
        data[(i/4)+2] = (((UINT32)cal_entry[i]) |
                         ((((UINT32)cal_entry[i+1]) << 8) & 0x0000ff00) |
                         ((((UINT32)cal_entry[i+2]) << 16) & 0x00ff0000) |
                         ((((UINT32)cal_entry[i+3]) << 24) & 0xff000000));
    } /* end for */

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_start_cfg_message((pmc_handle_t *)sts_sw_handle,MODID_COREOTN,HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_HO_CAL_NULL_CONFIG_ST,data,msg_len,stat);
    }

#ifdef PMC_SW_SIMULATION
    /* Do not send host message in simulation */
    PMC_RETURN(PMC_SUCCESS);
#endif /* PMC_SW_SIMULATION */

    PMC_RETURN(result);

} /* End: sts_sw_ho_calendar_null_cfg_by_fw() */


/*******************************************************************************
* sts_sw_ho_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the context information needed for a HO channel.
*   Number of tributary slots occupied by HO channel and their locations will be
*   specified in this function. 
*
*   The size of lo_chnl_ids_val[] array will be initialized to the number of 
*   tributary slots. The contents of this array will be filled in by 
*   sts_sw_lo_chnl_assign function.  
*
* INPUTS:
*   *sts_sw_handle          - pointer to STS_SW handle instance.
*   ho_chnl_id              - HO Channel ID
*                             Legal range: 0 - 95
*   oduk_type               - HO ODUk rate. See util_global_odukp_type_t.
*   ts_type                 - enum for tribslot size. See util_global_odu_line_payload_t.
*   num_trib_slot           - number of tributary slots in the HO channel
*   cal_array               - an array of the calendar offsets for the HO channel.
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
PUBLIC PMC_ERROR sts_sw_ho_chnl_init(sts_sw_handle_t *sts_sw_handle, 
                                     UINT32 ho_chnl_id,
                                     util_global_odukp_type_t oduk_type,
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_trib_slot,
                                     UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS])
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(num_trib_slot <= 80, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].chnl_id = ho_chnl_id;
    sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type = oduk_type;
    sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type = ts_type;
    sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot = num_trib_slot;
   
    /* retrieve calendar slot assignment for the HO channel */
    for (i = 0; i < 96; i++)
    {
        sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i] = 0x7f;
        if (i < num_trib_slot)
        {
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i] = cal_array[i];
            PMC_LOG_TRACE("cal_array[%u] = %u, sts_sw_handle->var.ho_chnl_cfg[%u].cal_entry[%u] = %u\n", i, cal_array[i], ho_chnl_id, i, sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i] );
        } else 
        {
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i] = 0x7f;
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* sts_sw_ho_chnl_init */

/*******************************************************************************
* sts_sw_lo_chnl_cfg_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function assigns LO channel ID to a tributary slot port of a HO channel
*   and sets the first cycle and EOMF cycle.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   ho_chnl_id          - HO Channel ID
*                         Valid range: 0 - 95
*   cycle               - calendar slot 
*                         Valid range: 0 - 95
*   lo_chnl_id          - LO channel ID 
*   eomf_cycle          - end of multi-frame cycle 
*   first_cycle         - First calendar cycle of the LO channel 
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
PUBLIC PMC_ERROR sts_sw_lo_chnl_cfg_all(sts_sw_handle_t *sts_sw_handle, 
                                        UINT32 ho_chnl_id,
                                        UINT32 cycle, 
                                        UINT32 lo_chnl_id, 
                                        UINT32 eomf_cycle,
                                        UINT32 first_cycle)
{
    /* variable declaration */
    UINT32 loc = 0;
    UINT32 i;
    sts_sw_buffer_t sts_sw_buf[1];

    PMC_ENTRY();
        
    /* argument checking */ 
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (ho_chnl_id > 95 || cycle > 95 || lo_chnl_id > 0x7F || first_cycle > 1)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG_TRACE("HO num_tribslot = %u\n", sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot);

    PMC_LOG_TRACE("Assign lo channel ID\n");
    
    for (i = 0; i < sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot; i++)
    {
       if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i] == cycle)
       {
           loc = i;
           break;
       }
    }
    
    if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] == 0x7F)
    {
        /* first LO channel for the HO channel */
        sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] = lo_chnl_id;
    }
            
    sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
    sts_sw_field_CHAN_ID_set(sts_sw_buf, sts_sw_handle, cycle, lo_chnl_id);
    sts_sw_field_LO_EOMF_CYCLE_set(sts_sw_buf, sts_sw_handle, cycle, eomf_cycle);
    sts_sw_field_LO_FIRST_CYCLE_set(sts_sw_buf, sts_sw_handle, cycle, first_cycle);   
    sts_sw_buffer_flush(sts_sw_buf);


    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_lo_chnl_cfg_all */

/*******************************************************************************
* sts_sw_lo_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function assigns LO channel ID to a tributary slot port of a HO channel.
*   This function will store all LO channel IDs associated with a HO channel in
*   an array pointed by lo_chnl_ids_prt in sts_sw_ho_cfg_t struct.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   ho_chnl_id          - HO Channel ID
*                         Valid range: 0 - 95
*   cycle               - calendar slot 
*                         Valid range: 0 - 95
*   lo_chnl_id          - LO channel ID 
*   unset               - indicates if the API is for deprovisioning or provisioning
*                         0 -- prov
*                         1 -- deprov
*   super_deprov        - boolean to indicate that this function is used for
*                         super deprov. lo_chnl_ids_ptr array will not be 
*                         updated until the last LO and all LO from offline page
*                         are removed.
*                         TRUE: super deprov
*                         FALSE: non-super deprov
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
PUBLIC PMC_ERROR sts_sw_lo_chnl_cfg(sts_sw_handle_t *sts_sw_handle, 
                                    UINT32 ho_chnl_id,
                                    UINT32 cycle, 
                                    UINT32 lo_chnl_id, 
                                    UINT32 unset,
                                    BOOL8 super_deprov)
{
    /* variable declaration */
    UINT32 loc = 0;
    UINT32 i;
    
    PMC_ENTRY();
        
    /* argument checking */ 
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (ho_chnl_id > 95 || cycle > 95 || lo_chnl_id > 0x7F)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG_TRACE("HO num_tribslot = %u\n", sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot);

    PMC_LOG_TRACE("Assign lo channel ID\n");
    
    for (i = 0; i < sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot; i++)
    {
       if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i] == cycle)
       {
           loc = i;
           break;
       }
    }
    
    if (unset == 0)
    {
        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] == 0x7F)
        {
            /* first LO channel for the HO channel */
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] = lo_chnl_id;
        }
    }
    else {
        PMC_LOG_TRACE("1) unset STS_SW.CHAN_ID...\n"); 
        
        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] == lo_chnl_id)
        {     
            PMC_LOG_TRACE("2) unset STS_SW.CHAN_ID...\n");    
            PMC_LOG_TRACE("sts_sw_handle->var.ho_chnl_cfg[%u].lo_chnl_ids_ptr[%u] = %u\n", ho_chnl_id, loc, sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc]);
            
            if (super_deprov == FALSE)
            {
                sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] = 0x7F;
            }
            lo_chnl_id = 0x7F;
        }
    }    
    
    if (sts_sw_handle->var.inst_type == INSTANCE_DEMUX_STS_SW)
    {       
        /* Set CHAN_ID */ 
        sts_sw_field_CHAN_ID_set(NULL, sts_sw_handle, cycle, lo_chnl_id);
    }
    
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_lo_chnl_cfg */

/*******************************************************************************
* sts_sw_lo_chnl_ids_ptr_ctxt_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function resets the lo chnanle IDs array of a particular HO to 0x7F.
*   This function is used only when operating in Super deprov mode.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   ho_chnl_id          - HO Channel ID
*                         Valid range: 0 - 95
*   lo_chnl_id          - LO channel ID
*                         Valid range: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_lo_chnl_ids_ptr_ctxt_reset(sts_sw_handle_t *sts_sw_handle, 
                                                   UINT32 ho_chnl_id,
                                                   UINT32 lo_chnl_id)
{
    /* variable declaration */
    UINT32 i;
    
    PMC_ENTRY();
        
    /* argument checking */ 
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; i++)
    {
        if (sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i] == lo_chnl_id)
        {
            sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i] = 0x7F;
        }            
    }
    
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_lo_chnl_ids_ptr_ctxt_reset */

/*******************************************************************************
* sts_sw_lo_eomf_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures EOMF cycle of the LO channel.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cycle               - calendar slot. Valid rage: 0 - 95
*   eomf_cycle          - end of multi-frame cycle 
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
PUBLIC PMC_ERROR sts_sw_lo_eomf_cycle_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cycle, UINT32 eomf_cycle)
{
    PMC_ENTRY();
        
    /* argument checking */   
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    /* Set LO_EOMF_CYCLE */
    sts_sw_field_LO_EOMF_CYCLE_set(NULL, sts_sw_handle, cycle, eomf_cycle);
       
    
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_lo_eomf_cycle_cfg */

/*******************************************************************************
* sts_sw_lo_calendar_cfg_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API clears any LO calendar entry based configurations in STS_SW when
*   deprovisioning a LO channel from ODTU_DMX.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cal_entry           - calendar entry. Valid rage: 0 - 95
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
PUBLIC PMC_ERROR sts_sw_lo_calendar_cfg_deprov(sts_sw_handle_t *sts_sw_handle, 
                                               UINT32 cal_entry)
{   
    PMC_ERROR result = PMC_SUCCESS;
    sts_sw_buffer_t sts_sw_buf[1];

    PMC_ENTRY();
    
    sts_sw_buffer_init(sts_sw_buf, sts_sw_handle);
    sts_sw_field_LO_EOMF_CYCLE_set(sts_sw_buf, sts_sw_handle, cal_entry, 0);
    sts_sw_field_LO_FIRST_CYCLE_set(sts_sw_buf, sts_sw_handle, cal_entry, 0);
    sts_sw_field_CHAN_ID_set(sts_sw_buf, sts_sw_handle, cal_entry, 0x7F);
    sts_sw_buffer_flush(sts_sw_buf);
    
    PMC_RETURN(result);
} /* sts_sw_lo_calendar_cfg_deprov */ 

/*******************************************************************************
* sts_sw_lo_calendar_cfg_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API clears any LO calendar entry based configurations in STS_SW when
*   deprovisioning a LO channel from ODTU_DMX.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   *cycle              - an array of calendar entry
*   num_cycle           - number of calendar to clean 
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
PUBLIC PMC_ERROR sts_sw_lo_calendar_cfg_clean(sts_sw_handle_t *sts_sw_handle, 
                                              UINT32 *cycle,
                                              UINT32 num_cycle)
{   
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 cycle_itr;

    PMC_ENTRY();
    for (cycle_itr = 0; cycle_itr < num_cycle; cycle_itr++)
    {
        sts_sw_field_MAP_DMSIM_TO_SSF_set(NULL, sts_sw_handle, cycle[cycle_itr], 0);
        sts_sw_field_LO_EOMF_CYCLE_set(NULL, sts_sw_handle, cycle[cycle_itr], 0);
        sts_sw_field_LO_FIRST_CYCLE_set(NULL, sts_sw_handle, cycle[cycle_itr], 0);
        sts_sw_field_CHAN_ID_set(NULL, sts_sw_handle, cycle[cycle_itr], 0x7F);
    }
    PMC_RETURN(result);
} /* sts_sw_lo_calendar_cfg_clean */ 

/*******************************************************************************
* sts_sw_lo_first_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the first calendar cycle of the LO channel.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   cycle               - calendar cycle 
*   first_cycle         - First calendar cycle of the LO channel 
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
PUBLIC PMC_ERROR sts_sw_lo_first_cycle_cfg(sts_sw_handle_t *sts_sw_handle, 
                                          UINT32 cycle, UINT32 first_cycle)
{
    PMC_ENTRY();
        
    /* argument checking */
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    if (cycle > 95 || first_cycle > 1)
    {
        PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    /* LO_FIRST_CYCLE */
    sts_sw_field_LO_FIRST_CYCLE_set(NULL, sts_sw_handle, cycle, first_cycle);   
    
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_lo_first_cycle_cfg */




/*******************************************************************************
* sts_sw_map_dmsim_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the consequential action for an MSI mistmatch
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   map_dmsim_to_ssf    - Maps MSI mismatch to SSF
*                         FALSE -- MSI mistmatch does not result in SSF
*                         TRUE -- MSI mismatch results in SSF
*                         (G.709 defined default is 0)
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
PUBLIC PMC_ERROR sts_sw_map_dmsim_to_ssf_set(sts_sw_handle_t *sts_sw_handle, 
                                             BOOL8 map_dmsim_to_ssf)
{
    PMC_ENTRY();
        
    /* argument checking */
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    sts_sw_field_MXDX_MODE_EN_set(NULL, sts_sw_handle, (UINT32)map_dmsim_to_ssf);
    
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_map_dmsim_to_ssf_set */


/*******************************************************************************
* sts_sw_map_dmsim_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the consequential action for an MSI mistmatch
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*
* OUTPUTS:
*   *map_dmsim_to_ssf    - Maps MSI mismatch to SSF
*                         0 -- MSI mistmatch does not result in SSF
*                         1 -- MSI mismatch results in SSF
*                         (G.709 defined default is 0)
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sts_sw_map_dmsim_to_ssf_get(sts_sw_handle_t *sts_sw_handle, 
                                             BOOL8 *map_dmsim_to_ssf)
{
    PMC_ENTRY();
    UINT32 result = 0;
    /* argument checking */
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);

    result = sts_sw_field_MXDX_MODE_EN_get(NULL, sts_sw_handle);
    *map_dmsim_to_ssf = (BOOL8) result;

       
    PMC_RETURN(PMC_SUCCESS);    
} /* sts_sw_map_dmsim_to_ssf_get */





/*
** Private Functions
*/
/*******************************************************************************
* sts_sw_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a STS_SW block instance.
*
*
* INPUTS:
*   *sts_sw_handle          - pointer to STS_SW handle instance
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
PRIVATE void sts_sw_var_default_init(sts_sw_handle_t *sts_sw_handle)
{
     /* variable declaration */  
    UINT32 i, j;

    PMC_ENTRY();
    
    PMC_ASSERT(sts_sw_handle != NULL, STS_SW_ERR_INVALID_PTR, 0, 0);
    
    /* configure the number of HO channels */
    for (i = 0; i < NUM_STS_SW_CHNL; i++)
    {
        sts_sw_handle->var.ho_chnl_cfg[i].oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
        sts_sw_handle->var.ho_chnl_cfg[i].chnl_id = 0xFF;
        sts_sw_handle->var.ho_chnl_cfg[i].num_tribslot = 0;

        for (j = 0; j < 96; j++)
        {
            sts_sw_handle->var.ho_chnl_cfg[i].cal_entry[j] = 0x7F;
            sts_sw_handle->var.ho_chnl_cfg[i].lo_chnl_ids_ptr[j] = 0x7F;
        }
    }

    PMC_RETURN();
} /* sts_sw_var_default_init */  

 
/*******************************************************************************
* sts_sw_eomf_cnt_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the end of multi-frame count of the HO channel
*   based on the HO ODU payload size, number of tribslots and the tributary
*   slot size.
*
* INPUTS:
*   *sts_sw_handle      - pointer to STS_SW handle instance.
*   chnl_id             - HO ODUk channel
*                         Valid range: 0 - 95
*   
* OUTPUTS:
*   eomf_cnt            - end of multi frame count
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sts_sw_eomf_cnt_calc(sts_sw_handle_t *sts_sw_handle, 
                                       UINT32 chnl_id,
                                       UINT32 *eomf_cnt)
{
    /* variable declaration */
    UINT32 num_ts;
    util_global_odu_line_payload_t ts_type;
    
    PMC_ENTRY();
    
    /* number of tribslots in the HO channel */
    num_ts = sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot;
    ts_type = sts_sw_handle->var.ho_chnl_cfg[chnl_id].ts_type;
    
    switch(sts_sw_handle->var.ho_chnl_cfg[chnl_id].oduk_type)
    {
        case UTIL_GLOBAL_ODU1:
            num_ts = 2;
            *eomf_cnt = ((ODU1_PAYLOAD_BYTES / 48) % num_ts) + 1;
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1F:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
        case UTIL_GLOBAL_ODU2F:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (ODU2_2G5_PAYLOAD_BYTES % 48 == 0)
                {
                    *eomf_cnt = (ODU2_2G5_PAYLOAD_BYTES / 48) % num_ts;
                }
                else {
                    *eomf_cnt = ((ODU2_2G5_PAYLOAD_BYTES / 48) + 1) % num_ts;
                }        
            }
            else { 
                if (ODU2_PAYLOAD_BYTES % 48 == 0)
                {        
                    *eomf_cnt = (ODU2_PAYLOAD_BYTES / 48) % num_ts;    
                }
                else {
                    *eomf_cnt = ((ODU2_PAYLOAD_BYTES / 48) + 1) % num_ts;    
                }
            }
            
            break;
        case UTIL_GLOBAL_ODU3:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (ODU3_2G5_PAYLOAD_BYTES % 48 == 0)
                {
                    *eomf_cnt = (ODU3_2G5_PAYLOAD_BYTES / 48) % num_ts;
                }
                else {
                    *eomf_cnt = ((ODU3_2G5_PAYLOAD_BYTES / 48) + 1) % num_ts;
                }        
            }
               else { 
                   if (ODU3_PAYLOAD_BYTES % 48 == 0)
                   {        
                    *eomf_cnt = (ODU3_PAYLOAD_BYTES / 48) % num_ts;    
                }
                else {
                    *eomf_cnt = ((ODU3_PAYLOAD_BYTES / 48) + 1) % num_ts;    
                }
            }
            break;
        case UTIL_GLOBAL_ODU3E1:
            num_ts = 32;
            
            /* if unchannelized MO, then we need 33 */
            if (sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot == 33
                && ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 33;
            }
     
            *eomf_cnt = ((ODU3E1_PAYLOAD_BYTES / 48) % num_ts) + 1;
            break;
        case UTIL_GLOBAL_ODU3E2:
            num_ts = 32;
            
            /* if unchannelized MO, then we need 33 */
            if (sts_sw_handle->var.ho_chnl_cfg[chnl_id].num_tribslot == 33
                && ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 33;
            }
            *eomf_cnt = ((ODU3E2_PAYLOAD_BYTES / 48) % num_ts) + 1;
            break;
        case UTIL_GLOBAL_ODU4:
            num_ts = 80;
            *eomf_cnt = ((ODU4_PAYLOAD_BYTES / 48) % num_ts) + 1;
            break;
        case UTIL_GLOBAL_ODU0:
            num_ts = 1;
            *eomf_cnt = ((ODU0_PAYLOAD_BYTES / 48) % num_ts) + 1;
            break;            
        case UTIL_GLOBAL_ODUFLEX_GFP:
        case UTIL_GLOBAL_ODUFLEX_CBR: 
            *eomf_cnt = 0;
            break;
        default:
            PMC_RETURN(STS_SW_ERR_INVALID_PARAMETERS);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
} /* sts_sw_eomf_cnt_calc */


/*
** End of file
*/
