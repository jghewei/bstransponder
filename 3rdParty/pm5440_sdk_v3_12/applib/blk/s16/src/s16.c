/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*       Main S16 source file.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "s16_loc.h"
#include "s16_channel_pcbi_io.h"
#include "s16_csu_pcbi_io.h"
#include "acb_ft_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define S16_T_PISO_LOOKUP_TBL_SIZE_MAX 37


#define S16_PISO_LOOKUP_TBL_ENTRIES { \
{    S16_PORT_MODE_2G488    , 2    }, \
{    S16_PORT_MODE_3G125    , 2    }, \
{    S16_PORT_MODE_4G250    , 1    }, \
{    S16_PORT_MODE_4G915    , 1    }, \
{    S16_PORT_MODE_5G000    , 1    }, \
{    S16_PORT_MODE_6G144    , 1    }, \
{    S16_PORT_MODE_6G250    , 1    }, \
{    S16_PORT_MODE_8G500    , 0    }, \
{    S16_PORT_MODE_9G830    , 0    }, \
{    S16_PORT_MODE_9G953    , 0    }, \
{    S16_PORT_MODE_10G000   , 0    }, \
{    S16_PORT_MODE_10G037   , 0    }, \
{    S16_PORT_MODE_10G312   , 0    }, \
{    S16_PORT_MODE_10G355   , 0    }, \
{    S16_PORT_MODE_10G399   , 0    }, \
{    S16_PORT_MODE_10G518   , 0    }, \
{    S16_PORT_MODE_10G562   , 0    }, \
{    S16_PORT_MODE_10G607   , 0    }, \
{    S16_PORT_MODE_10G709   , 0    }, \
{    S16_PORT_MODE_10G754   , 0    }, \
{    S16_PORT_MODE_10G925   , 0    }, \
{    S16_PORT_MODE_11G049   , 0    }, \
{    S16_PORT_MODE_11G095   , 0    }, \
{    S16_PORT_MODE_11G142   , 0    }, \
{    S16_PORT_MODE_11G145   , 0    }, \
{    S16_PORT_MODE_11G180   , 0    }, \
{    S16_PORT_MODE_11G182   , 0    }, \
{    S16_PORT_MODE_11G200   , 0    }, \
{    S16_PORT_MODE_11G270   , 0    }, \
{    S16_PORT_MODE_11G317   , 0    }, \
{    S16_PORT_MODE_11G320   , 0    }, \
{    S16_PORT_MODE_11G319   , 0    }, \
{    S16_PORT_MODE_11G32252 , 0    }, \
{    S16_PORT_MODE_11G32259 , 0    }, \
{    S16_PORT_MODE_11G32275 , 0    }, \
{    S16_PORT_MODE_11G500   , 0    }, \
{    S16_PORT_MODE_12G500   , 0    }} \

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: piso_loopback_tbl_elem_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PISO loopback table
*
* ELEMENTS:
*   See Typedef.
*
*******************************************************************************/
typedef struct piso_lookup_tbl_elem_struct_t
{
    s16_port_mode_t port_mode;
    UINT8 piso_val;
} piso_lookup_tbl_elem_t;

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
const char S16_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    S16_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char S16_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void s16_var_default_init(s16_var_t   *var_ptr);

PRIVATE void s16_csu_cfg(s16_handle_t *s16_handle, 
                         UINT8 lane, 
                         UINT8 master_lane, 
                         BOOL8 enable,
                         BOOL8 cleanup);

PRIVATE void s16_csu_lpbk_cfg(s16_handle_t *s16_handle,
                              UINT8 lane, 
                              acb_ft_prov_cfg_t *acb_cfg_ptr, 
                              BOOL8 enable);

PRIVATE void s16_pmm_cfg(s16_handle_t *s16_handle,UINT8 lane, 
                         acb_ft_prov_cfg_t *acb_cfg_ptr, 
                         BOOL8 enable);

PRIVATE void s16_csu_acb_reset(s16_handle_t *s16_handle,UINT8 lane, BOOL8 reset);

PRIVATE PMC_ERROR s16_t_piso_ctrl_val_get(s16_port_mode_t port_mode, 
                                          UINT8 *t_piso_ctrl);

PRIVATE void s16_acb_ft_loopback_cfg_get(s16_handle_t *handle,
                                         UINT8 serdes_id,
                                         s16_loopback_cfg_t *cfg_ptr,
                                         acb_ft_prov_cfg_t *acb_ft_cfg_ptr);

PRIVATE void s16_acb_ft_loop_timing_compute_pkt_cfg(s16_handle_t *handle,
                                                    UINT8 serdes_id,
                                                    acb_ft_prov_cfg_t *acb_ft_cfg_ptr);

PRIVATE void s16_loopback_cfg(s16_handle_t *handle,
                              UINT8 serdes_id,
                              BOOL8 enable);

PRIVATE PMC_ERROR s16_tx_ctxt_push(s16_handle_t *s16_handle, 
                                   UINT8 serdes_id);

PRIVATE PMC_ERROR s16_tx_ctxt_pop(s16_handle_t *s16_handle, 
                                  UINT8 serdes_id);

PRIVATE BOOL8 s16_serdes_id_validate(s16_handle_t *handle,
                                     UINT8 serdes_id);

PRIVATE void s16_tx_squelch(s16_handle_t *handle,
                            UINT8 serdes_id,
                            BOOL8 enable);

PRIVATE PMC_ERROR s16_internal_loopback_activate(s16_handle_t       *handle,
                                                 UINT8               serdes_id,
                                                 s16_loopback_cfg_t *cfg_ptr,
                                                 BOOL8               check_state);

PRIVATE PMC_ERROR s16_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg);
PRIVATE PMC_ERROR util_patt_mode_to_s16_mode_conv(util_patt_mode_t util_patt_mode,            
                                      s16_patt_mode_t *s16_mode);

PRIVATE PMC_ERROR s16_ctxt_audit(s16_handle_t *s16_handle,
                                 s16_var_t    *s16_var_clean,
                                 UINT32        s16_reset_msk,
                                 UINT32        skip_msk);

PRIVATE PMC_ERROR s16_ctxt_cleanup(s16_handle_t *s16_handle,
                                   s16_var_t    *s16_var_clean,
                                   UINT32        skip_msk);

PRIVATE acb_ft_serdes_if_t s16_serdes_if_get(s16_handle_t        *s16_handle, 
                                             UINT8                serdes_id);

/*
** Public Functions
*/
/*******************************************************************************
* s16_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a S16 block instance.
*
*
* INPUTS:
*   *parent           - pointer to parent handle.  If no parent set to NULL .
*   base_address      - base address of the S16 block relative to the device 
*                       memory space
*   *sys_handle       - pointer to user system handle
*   *tsb_name         - pointer to TSB name
*    s16_type         - define the instance TYPE
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   s16_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC s16_handle_t *s16_ctxt_create(pmc_handle_t *parent, 
                                     UINT32 base_address, 
                                     pmc_sys_handle_t *sys_handle, 
                                     const char *tsb_name , 
                                     s16_type_t s16_type)
{
    UINT8 itr;
    s16_handle_t *s16_handle;
    /* Define ACB_FT instances name's prefix */
    const char* ACB_FT_HANDLE_NAME = "ACB_FT";

    PMC_ENTRY();    

    s16_handle = (s16_handle_t*)PMC_CTXT_CALLOC(sizeof(s16_handle_t), parent);
    
    pmc_handle_init( parent, s16_handle, sys_handle, PMC_MID_DIGI_S16_SERDES, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(S16_LOG_ERR_STRINGS[0], S16_LOG_ERR_TABLE_BASE, S16_LOG_ERR_COUNT);

    /* create sub component context */
    s16_handle->cfg.num_serdes_instance =  (s16_type == S16_LINE) ? S16_LINE_NUM_INSTANCE: S16_SYSTEM_NUM_INSTANCE;
    s16_handle->cfg.s16_type = s16_type;

    s16_handle->acb_ft_handle = (acb_ft_handle_t **) PMC_CTXT_CALLOC(sizeof(acb_ft_handle_t*)*s16_handle->cfg.num_serdes_instance, parent); 
    PMC_CTXT_REGISTER_PTR(&s16_handle->acb_ft_handle, parent);
    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance ; itr++) 
    { 
        s16_handle->acb_ft_handle[itr] = acb_ft_ctxt_create(
            (pmc_handle_t *) s16_handle,
            base_address+BASE_ADDR_PM55_82_50_BRIDGE_ACB_PCBI_ACB_FT + (itr * S16_INSTANCE_ADDR_SPACE),
            sys_handle ,
            ACB_FT_HANDLE_NAME);
        PMC_CTXT_REGISTER_PTR(&s16_handle->acb_ft_handle[itr], parent);
    } 
    /* allocate var context */
    s16_handle->var =  (s16_var_t *) PMC_CTXT_CALLOC(sizeof(s16_var_t) * s16_handle->cfg.num_serdes_instance, parent); 
    PMC_CTXT_REGISTER_PTR(&s16_handle->var, parent);
    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance; itr++)
    {
        s16_handle->var[itr].s16_ctxt.active = FALSE;
    }

        
    PMC_RETURN(s16_handle);
} /* s16_ctxt_create */  
 
/*******************************************************************************
* s16_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a S16 block instance.
*
*
* INPUTS:
*   *s16_handle        - pointer to S16 handle instance
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
PUBLIC void s16_ctxt_destroy(s16_handle_t *s16_handle)
{
    UINT8 itr;
    PMC_ENTRY();

    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance ; itr++) 
    { 
        acb_ft_ctxt_destroy(s16_handle->acb_ft_handle[itr]); 
    } 
   
    PMC_CTXT_FREE(&s16_handle->acb_ft_handle, s16_handle);
    
    PMC_CTXT_FREE(&s16_handle->var, s16_handle);           
    PMC_CTXT_FREE(&s16_handle, s16_handle); 
    PMC_RETURN();
} /* s16_ctxt_destroy */

/*******************************************************************************
* s16_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a S16 block handle instance.
*
*
* INPUTS:
*   *s16_handle        - pointer to S16 handle instance
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
PUBLIC void s16_handle_init(s16_handle_t *s16_handle)
{
    UINT8 itr;
   
    PMC_ENTRY();

    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance ; itr++)
    {
        acb_ft_handle_init(s16_handle->acb_ft_handle[itr],ACB_FT_S16);        
        s16_var_default_init(&s16_handle->var[itr]);
    }

    PMC_RETURN();

} /* s16_handle_init */

/*******************************************************************************
* s16_ext_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set s16 lanes state in reset or start mode.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane_msk              - lanes mask
*   out_of_reset          - When TRUE state of S16 lane is set to START mode.
*                           when FALSE state is set back to RESET mode.
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation executes succesfully otherwise
*   a descriptive error code will be returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_ext_ctxt_state_set(s16_handle_t      *s16_handle, 
                                        UINT16             lane_msk,
                                        BOOL8              out_of_reset) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_itr;

    PMC_ENTRY();    

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* for each lane, set update state */
    for (lane_itr = 0; lane_itr < s16_handle->cfg.num_serdes_instance; lane_itr += 1)
    {
        if (0x1 == (0x1 & (lane_msk >> lane_itr)))
        {
            if (TRUE == out_of_reset)
            {
                /* this function should be call only once and before loopback activation*/
                if (S16_PORT_RESET == s16_handle->var[lane_itr].state)
                {
                    s16_handle->var[lane_itr].state = S16_PORT_START;
                }
                else
                {
                    rc = S16_STATE_NOT_RESET;
                    break;
                }
            }
            else
            {
                s16_handle->var[lane_itr].state = S16_PORT_RESET;
                s16_var_default_init(&s16_handle->var[lane_itr]);
                /* cleanup ACB register space and context */
                acb_ft_cleanup(s16_handle->acb_ft_handle[lane_itr]);
            }
        }
    }
        
    PMC_RETURN(rc);
}/*s16_ext_ctxt_state_set*/

/*******************************************************************************
* s16_reset_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return serdes lane reset mode status.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - lane index.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE when serdes is in reset mode, otherwise TRUE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 s16_reset_state_get(s16_handle_t      *s16_handle, 
                                 UINT16             lane) 
{
    BOOL8 rc;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    rc = (S16_PORT_RESET == s16_handle->var[lane].state) ? TRUE : FALSE;
    
    PMC_RETURN(rc);
}/*s16_reset_state_get*/

/*******************************************************************************
* s16_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions a S16 serdes instance. S16 is mainly provision by FW. 
*   This function handle ACB, multi-lane aligner (in multi-lane configuration) and 
*   PMM configuration in loopback (RX to TX) configuration.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - the lane to configure 
*   master_lane           - the master lane in multi-lane configuration
*   *acb_cfg_ptr          - pointer to ACB_FT provisioning configuration
*   clk_cfg               - Indicate that the serdes insatcne is used as a \n
*                           a clock and if it should be configured as a slave 
*                           or a master lane.
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_prov(s16_handle_t      *s16_handle, 
                          UINT8              lane,
                          UINT8              master_lane, 
                          acb_ft_prov_cfg_t *acb_cfg_ptr,
                          s16_clk_cfg_t      clk_cfg) 
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 csu_r;
    UINT64 csu_n;

    PMC_ENTRY();    

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(master_lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(acb_cfg_ptr != NULL ,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (S16_PORT_START != s16_handle->var[lane].state &&
        S16_PORT_OPERATIONAL_LOOPBACK != s16_handle->var[lane].state)
    {
        PMC_RETURN(S16_STATE_NOT_START);
    }

    if (s16_handle->var[lane].state == S16_PORT_OPERATIONAL_LOOPBACK)
    {
        /* force acb_ft deprov */
        (void) acb_ft_deactivate(s16_handle->acb_ft_handle[lane]);
        (void) acb_ft_deprov(s16_handle->acb_ft_handle[lane]);
        s16_handle->var[lane].has_acb = FALSE;
    }

    /* update var context */
    s16_handle->var[lane].mld_mode = (lane == master_lane)?S16_MASTER:S16_SLAVE;
    s16_handle->var[lane].has_acb = TRUE;    
    s16_handle->var[lane].clk_cfg = (UINT8)clk_cfg;
    
        
    /* retrieve master lane bus width when serdes will be used as a clock and configured as a master. */
    if (S16_CLK_CFG_AS_MASTER == clk_cfg) {
        /* when the serdes is a slave in a multi-lane configure, we configure it as a slave in the CSU */
        s16_csu_cfg(s16_handle,lane,lane,TRUE, FALSE);
        acb_cfg_ptr->master_serdes_if = s16_serdes_if_get(s16_handle, master_lane);
    }
    else
    {
        /* when the serdes is a slave in a multi-lane configure, we configure it as a slave in the CSU */
        s16_csu_cfg(s16_handle,lane, master_lane,TRUE, FALSE);
        acb_cfg_ptr->master_serdes_if = LAST_ACB_FT_SERDES_IF;
    }

    if (TRUE == s16_handle->var[lane].has_acb)
    {
        if (acb_cfg_ptr->mode == ACB_FT_MODE_LOOP_TIMING)
        {
            /* compute packet size */
            s16_acb_ft_loop_timing_compute_pkt_cfg(s16_handle,lane,acb_cfg_ptr);
            /* configure PMM */
            s16_pmm_cfg(s16_handle, lane, acb_cfg_ptr, TRUE);
        }

        /* extract required ACB_FT configuration field from CSU */
        s16_csu_div_val_get(s16_handle,lane,&acb_cfg_ptr->csu_div, &csu_n, &acb_cfg_ptr->csu_denomin, (UINT32 *)&csu_r);
        /* Release reset to ACB_FT prior to use it */
        s16_csu_acb_reset(s16_handle, lane, FALSE);
        /* provision ACB_FT block */
        rc = acb_ft_prov(s16_handle->acb_ft_handle[lane], acb_cfg_ptr);

        if (PMC_SUCCESS == rc) 
        {       
            acb_ft_pll_s_max_cfg(s16_handle->acb_ft_handle[lane], acb_cfg_ptr->max_ppm, csu_n, acb_cfg_ptr->csu_denomin, csu_r);
        } 
    }

    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        s16_handle->var[lane].state = S16_PORT_EQUIPPED;
    }


    PMC_RETURN(rc);
} /* s16_prov */

/*******************************************************************************
* s16_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions a S16 serdes instance.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - the lane to deprovision
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
PUBLIC PMC_ERROR s16_deprov(s16_handle_t *s16_handle, UINT8 lane)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (S16_PORT_EQUIPPED != s16_handle->var[lane].state) 
    {
        PMC_RETURN(S16_STATE_NOT_EQUIPPED);
    }

    /* configure CSU */
    s16_csu_cfg(s16_handle,lane, lane, FALSE, FALSE);

    if (TRUE == s16_handle->var[lane].has_acb)
    {
        /* deprovision PMM */
        s16_pmm_cfg(s16_handle, lane, NULL, FALSE);
       
        /* deprovision ACB_FT block */ 
        rc = acb_ft_deprov(s16_handle->acb_ft_handle[lane]);
        /* Put ACB in reset mode */
        s16_csu_acb_reset(s16_handle, lane, TRUE);
    }

    if (PMC_SUCCESS == rc) 
    {  
        /* update state */
        s16_handle->var[lane].state = S16_PORT_START;
        s16_handle->var[lane].has_acb = FALSE;
    }

    /* configure S16 loopback if it was activated */
    if (PMC_SUCCESS == rc && TRUE == s16_handle->var[lane].s16_ctxt.lpbk_cfg_valid)
    {
        /* if we have a loopback, we just flush the ACB_FT context */
        acb_ft_flush_context(s16_handle->acb_ft_handle[lane], UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX);
        s16_handle->var[lane].has_lpbk_acb_ft_backup = FALSE;
        rc = s16_internal_loopback_activate(s16_handle, lane, &s16_handle->var[lane].s16_ctxt.lpbk_cfg, FALSE);
    }

    PMC_RETURN(rc);    
} /* s16_deprov */

/*******************************************************************************
* s16_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activate a S16 serdes instance.
*
* INPUTS:
*  *s16_handle           - pointer to S16 handle instance.
*  lane                  - the lane to activate
*  activate_acb          - Flag indicating whether to activate acb. If FALSE
*                          and lane is master lane then no acb_ft activation
*                          is done. If TRUE then acb_ft is activated 
*                          regardless of the master lane state.
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
PUBLIC PMC_ERROR s16_activate(s16_handle_t *s16_handle, 
                              UINT8 lane,
                              BOOL8 activate_acb) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (S16_PORT_EQUIPPED != s16_handle->var[lane].state &&
        TRUE != s16_handle->var[lane].s16_ctxt.lpbk_cfg_valid) 
    {
        PMC_RETURN(S16_STATE_NOT_EQUIPPED);
    }
    if (TRUE == s16_handle->var[lane].has_acb && 
        (TRUE == activate_acb ||
          S16_MASTER != s16_handle->var[lane].mld_mode))
    {
        rc = acb_ft_activate(s16_handle->acb_ft_handle[lane]);        
    }
    
    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        s16_handle->var[lane].state = S16_PORT_OPERATIONAL;
    }

    /* configure S16 loopback if it was activated */
    if (PMC_SUCCESS == rc && TRUE == s16_handle->var[lane].s16_ctxt.lpbk_cfg_valid)
    {
        rc = s16_internal_loopback_activate(s16_handle, lane, &s16_handle->var[lane].s16_ctxt.lpbk_cfg, FALSE);
    }

    PMC_RETURN(rc);
} /* s16_activate */

/*******************************************************************************
* s16_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deactivate a S16 serdes instance.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - the lane to activate
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
PUBLIC PMC_ERROR s16_deactivate(s16_handle_t *s16_handle, 
                                UINT8         lane) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (S16_PORT_OPERATIONAL != s16_handle->var[lane].state &&
        S16_PORT_OPERATIONAL_LOOPBACK != s16_handle->var[lane].state &&
        S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL != s16_handle->var[lane].state &&
        S16_PORT_OPERATIONAL_FORCE_NOMINAL != s16_handle->var[lane].state) 
    {
        PMC_RETURN(S16_STATE_NOT_OPERATIONAL);
    }

    if (TRUE == s16_handle->var[lane].has_acb)
    {
        rc = acb_ft_deactivate(s16_handle->acb_ft_handle[lane]);
    }

    if (PMC_SUCCESS == rc) 
    {
        /* update state */
        s16_handle->var[lane].state = S16_PORT_EQUIPPED;
    }

    PMC_RETURN(rc);
} /* s16_deactivate */

/*******************************************************************************
* s16_restart
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function puts back S16 serdes in start mode or loopback.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - the lane to restart
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
PUBLIC PMC_ERROR s16_restart(s16_handle_t *s16_handle, 
                             UINT8         lane) 
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    if (S16_PORT_RESET != s16_handle->var[lane].state)
    {
        acb_ft_restart(s16_handle->acb_ft_handle[lane], TRUE);

        /* configure CSU */
        s16_csu_cfg(s16_handle,lane, lane, FALSE, FALSE);

        /* deprovision PMM */
        s16_pmm_cfg(s16_handle, lane, NULL, FALSE);
    
        /* Put ACB in reset mode */
        s16_csu_acb_reset(s16_handle, lane, TRUE);

        s16_handle->var[lane].has_acb = FALSE;
        /* update state */
        s16_handle->var[lane].state = S16_PORT_START;
 
        /* configure S16 loopback if it was activated */
        if (TRUE == s16_handle->var[lane].s16_ctxt.lpbk_cfg_valid)
        {
            /* if we have a loopback, we just flush the ACB_FT context */
            acb_ft_flush_context(s16_handle->acb_ft_handle[lane], UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX);
            s16_handle->var[lane].has_lpbk_acb_ft_backup = FALSE;
            rc = s16_internal_loopback_activate(s16_handle, lane, &s16_handle->var[lane].s16_ctxt.lpbk_cfg, FALSE);
        }
    }

    PMC_RETURN(rc);
} /* s16_restart */

/*******************************************************************************
* s16_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on s16 handle instance.
*
* INPUTS:
*   *s16_handle             - pointer to s16 handle instance.
*   *restart_init_cfg_ptr   - pointer to restart init config.
*    s16_reset_msk          - S16 serdes reset bits mask.
*    skip_msk               - serdes not concerned by operation bit mask.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   S16_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put S16 in RESET mode
*   
*
* NOTES:
*   subsystem shall be put in reset mode 
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_handle_restart_init(s16_handle_t                   *s16_handle,
                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                         UINT32                          s16_reset_msk,
                                         UINT32                          skip_msk)
{
    PMC_ERROR           rc = PMC_SUCCESS;
    s16_var_t           *s16_var_clean;
    UINT32              itr;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(S16_LOG_ERR_STRINGS[0], S16_LOG_ERR_TABLE_BASE, S16_LOG_ERR_COUNT);
    }

    s16_var_clean =  (s16_var_t *) PMC_CALLOC(sizeof(s16_var_t) * s16_handle->cfg.num_serdes_instance); 

    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance && PMC_SUCCESS == rc; itr++)
    {
        rc = acb_ft_handle_restart_init(s16_handle->acb_ft_handle[itr], restart_init_cfg_ptr, PMC_ENERGY_STATE_RUNNING);
    }

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only OPERATIONAL channel configruation are recovered
       - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
       - detailed energy states are recovered
       - incoherency in register context means that the subsystem shall be shutdown
    */

    /* 
     * Audit register context and memory context 
     *   - Audit power and decide if we should just shut down the subsystem 
     *   - Create a new context that represents the intersection between memory and register space (s16_var_clean)
     */
    if (PMC_SUCCESS == rc)
    {
        rc = s16_ctxt_audit(s16_handle,
                            s16_var_clean,
                            s16_reset_msk,
                            skip_msk); 
    }
    
    PMC_LOG_TRACE("s16_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    if (S16_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, s16_var_clean is the new context
         *    - We keep OPERATIONAL channel defined in s16_var_clean
         *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
         *       - they may have be partially provisionned
         *    - We ajust power setting based on s16_var_clean
         */                                    
        rc = s16_ctxt_cleanup(s16_handle,
                              s16_var_clean,
                              skip_msk); 
    }

    PMC_FREE(&s16_var_clean);

    PMC_RETURN(rc);
} /* s16_handle_restart_init */

/*******************************************************************************
* s16_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits S16 handle context variable against register space content.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   *s16_var_clean        - pointer to S16 lanes context.
*    s16_reset_msk        - S16 serdes reset bits mask.
*    skip_msk             - serdes not concerned by operation bit mask.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if auditing is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_ctxt_audit(s16_handle_t *s16_handle,
                                 s16_var_t    *s16_var_clean,
                                 UINT32        s16_reset_msk,
                                 UINT32        skip_msk)
{
    UINT32 itr;
    BOOL8 serdes_ctxt_match;
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 serdes_reset;
    s16_loopback_mode_t lpbk_mode;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    

    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance; itr += 1)
    {
        if (0x1 & (skip_msk >> itr))
        {
            continue;
        }

        serdes_ctxt_match = TRUE;
        serdes_reset = (0x1 == (0x1 & (s16_reset_msk >> itr))) ? TRUE : FALSE;

        if (FALSE == serdes_reset)
        {
            /* check s16 provisioning & activation status */
            if (S16_PORT_RESET == s16_handle->var[itr].state ||
                LAST_S16_PORT_STATE == s16_handle->var[itr].state)
            {
                serdes_ctxt_match = FALSE;
            }
            
            /* audit acb_ft activation && prov status if necessary */
            if (TRUE == serdes_ctxt_match)
            {
                if (S16_PORT_EQUIPPED != s16_handle->var[itr].state)
                {
                    /* check loopback status */
                    if (TRUE == serdes_ctxt_match)
                    {
                        lpbk_mode =  s16_loopback_cfg_get(s16_handle,itr);
                        if (TRUE == s16_handle->var[itr].s16_ctxt.lpbk_cfg_valid)
                        {
                            if (s16_handle->var[itr].s16_ctxt.lpbk_cfg.loopback_mode !=
                                lpbk_mode)
                            {                        
                                if (FALSE == ((S16_DIAGNOSTIC_WITH_TX_LOOPBACK == lpbk_mode && 
                                     S16_DIAGNOSTIC_LOOPBACK == s16_handle->var[itr].s16_ctxt.lpbk_cfg.loopback_mode) ||
                                    (S16_DIAGNOSTIC_WITH_TX_LOOPBACK == s16_handle->var[itr].s16_ctxt.lpbk_cfg.loopback_mode && 
                                     S16_DIAGNOSTIC_LOOPBACK == lpbk_mode)))
                                {
                                    serdes_ctxt_match = FALSE;
                                }
                            }
                        }
                    }
                    if (TRUE == serdes_ctxt_match)
                    {
                        /* loopback mode should not be set when valid is FALSE */
                        if (FALSE == s16_handle->var[itr].s16_ctxt.lpbk_cfg_valid &&
                            S16_LOOPBACK_MODE_UNUSED != lpbk_mode)
                        {
                            serdes_ctxt_match = FALSE;
                        }   
                    }
                    if (TRUE == serdes_ctxt_match)
                    {
                        /* loopback mode should not be set when S16 state is not in loopback or force_nominal_loopback */
                        if ((S16_LOOPBACK_MODE_UNUSED != lpbk_mode) &&
                            (S16_PORT_OPERATIONAL_LOOPBACK != s16_handle->var[itr].state) &&
                            (S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL != s16_handle->var[itr].state))
                        {
                            serdes_ctxt_match = FALSE;
                        }
                    }
                }
            }
        }
        else
        {
            /* check s16 provisioning & activation status */
            if (S16_PORT_RESET != s16_handle->var[itr].state &&
                LAST_S16_PORT_STATE != s16_handle->var[itr].state)
            {
                serdes_ctxt_match = FALSE;
            }
        }
        /* update returned error code anf var clean (used to cleanup the context) */
        if (FALSE == serdes_ctxt_match)
        {
            if (PMC_SUCCESS == rc)
            {
                rc = S16_ERR_CONTEXT_CLEANUP;
            }
            s16_var_default_init(&s16_var_clean[itr]);
        }   
        else
        {
            PMC_MEMCPY((void*)&s16_var_clean[itr], (void*)&s16_handle->var[itr], sizeof(s16_var_t));
        } 
    }

    PMC_RETURN(rc);
} /* s16_ctxt_audit */

/*******************************************************************************
* s16_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up S16 handle context. 
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   *s16_var_clean        - pointer to S16 lanes clean context.
*    skip_msk             - serdes not concerned by operation bit mask.
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
PRIVATE PMC_ERROR s16_ctxt_cleanup(s16_handle_t  *s16_handle,
                                   s16_var_t     *s16_var_clean,
                                   UINT32         skip_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    for (itr = 0; itr < s16_handle->cfg.num_serdes_instance; itr += 1)
    {     
        if (0x1 & (skip_msk >> itr))
        {
            continue;
        }
        if (S16_PORT_RESET == s16_var_clean[itr].state)
        {
            s16_cleanup(s16_handle, itr);
        }
    }

    PMC_RETURN(rc);
} /* s16_ctxt_cleanup */

/*******************************************************************************
* s16_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleanup S16 serdes lane context.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   lane                  - the lane to cleanup
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
PUBLIC void s16_cleanup(s16_handle_t *s16_handle, 
                        UINT8         lane) 
{
    PMC_ENTRY();

    PMC_ASSERT(s16_handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(lane < s16_handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    /* cleanup ACB register space and context */
    acb_ft_cleanup(s16_handle->acb_ft_handle[lane]);
    

    /* update state */
    s16_var_default_init(&s16_handle->var[lane]);

    PMC_RETURN();
} /* s16_cleanup */

/*******************************************************************************
* s16_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function activates a loopback. It can be called when the SERDES is in 
*   S16_PORT_START or S16_PORT_OPERATIONAL or S16_PORT_OPERATIONAL_FORCE_NOMINAL state.
*   
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   *cfg_ptr        - pointer to a s16 loopbck configuration
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_loopback_activate(s16_handle_t       *handle,
                                       UINT8               serdes_id,
                                       s16_loopback_cfg_t *cfg_ptr)
{
    PMC_ERROR     rc;

    PMC_ENTRY();

    rc = s16_internal_loopback_activate(handle, serdes_id, cfg_ptr, TRUE);

    PMC_RETURN(rc);

}

/*******************************************************************************
* s16_internal_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function activates a loopback. Serdes state check can be skipped or not.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   *cfg_ptr        - pointer to a s16 loopbck configuration
*    check_state    - when TRUE, serdes state is checked, otherwise
*                     loopback configuration is blindly done
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_internal_loopback_activate(s16_handle_t       *handle,
                                                 UINT8               serdes_id,
                                                 s16_loopback_cfg_t *cfg_ptr,
                                                 BOOL8               check_state)
{
    acb_ft_prov_cfg_t acb_ft_cfg;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(cfg_ptr != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(cfg_ptr->loopback_mode != S16_LOOPBACK_MODE_UNUSED,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (TRUE == check_state)
    {
        if (!(S16_MASTER == handle->var[serdes_id].mld_mode && S16_PORT_EQUIPPED == handle->var[serdes_id].state) &&
            S16_PORT_OPERATIONAL != handle->var[serdes_id].state && 
            S16_PORT_START != handle->var[serdes_id].state &&
            S16_PORT_OPERATIONAL_FORCE_NOMINAL != handle->var[serdes_id].state)
        {
            PMC_RETURN(S16_STATE_NOT_OPERATIONAL);
        }
    }
        
    /* update var state */
    handle->var[serdes_id].loopback_mode = cfg_ptr->loopback_mode;    
    
    /* check if we have a conflict between pattern generator and FACILITY loopback */
    if (S16_FACILITY_LOOPBACK == handle->var[serdes_id].loopback_mode && 
        TRUE == handle->var[serdes_id].patt_gen_enabled)
    {
        PMC_RETURN(S16_PATT_GEN_FACILITY_LOOPBACK_CONFLICT);
    }

    /* handle PMM/ACB configuration */

    if (S16_FACILITY_LOOPBACK == handle->var[serdes_id].loopback_mode ||
        S16_ACB_ONLY_LOOPBACK == handle->var[serdes_id].loopback_mode) 
    {
        /* Initialize acb ft configuration*/
        PMC_MEMSET((void *) &acb_ft_cfg, 0, sizeof(acb_ft_prov_cfg_t));

        /* get the acb_ft configuration for the loopback */ 
        s16_acb_ft_loopback_cfg_get(handle,serdes_id,cfg_ptr,&acb_ft_cfg);

        /* configure PMM */
        s16_csu_lpbk_cfg(handle,serdes_id, &acb_ft_cfg, TRUE);    
        if (TRUE == acb_ft_provisionned(handle->acb_ft_handle[serdes_id]) ||
            (S16_MASTER == handle->var[serdes_id].mld_mode &&
             S16_PORT_OPERATIONAL == handle->var[serdes_id].state))
        {
            if (PMC_SUCCESS == rc &&
                TRUE == handle->var[serdes_id].has_acb)
            { 
                rc = acb_ft_deactivate(handle->acb_ft_handle[serdes_id]);
            }

            /* 
               when ACB_FT is already provisionned we store the context,
               it is restored when we deactivated the loopback
            */
            if (PMC_SUCCESS == rc &&
                S16_PORT_OPERATIONAL_FORCE_NOMINAL != handle->var[serdes_id].state)
            {
                handle->var[serdes_id].has_lpbk_acb_ft_backup = TRUE;
                rc = acb_ft_context_push(handle->acb_ft_handle[serdes_id], UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX);
            }
            
            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_deprov(handle->acb_ft_handle[serdes_id]);
                /* Put ACB in reset mode */
                s16_csu_acb_reset(handle, serdes_id, TRUE);
            }
        }
        if (PMC_SUCCESS == rc) 
        {
            /* remove ACB in reset mode */
            s16_csu_acb_reset(handle, serdes_id, FALSE);
            rc = acb_ft_prov(handle->acb_ft_handle[serdes_id],&acb_ft_cfg);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_s16_loop_timing_activate(handle->acb_ft_handle[serdes_id]);
            if (PMC_SUCCESS == rc)
            {
                handle->var[serdes_id].has_acb = TRUE;
            }
        }
    } 

    if (TRUE == handle->var[serdes_id].has_force_nominal_acb_ft_backup && PMC_SUCCESS == rc)
    {        
        acb_ft_cfg.mode = ACB_FT_MODE_NOMINAL;
        
        /* remove ACB in reset mode */
        s16_csu_acb_reset(handle, serdes_id, FALSE);
        if (TRUE == handle->var[serdes_id].has_acb)
        {
            rc = acb_ft_deactivate(handle->acb_ft_handle[serdes_id]);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_deprov(handle->acb_ft_handle[serdes_id]);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_prov(handle->acb_ft_handle[serdes_id],&acb_ft_cfg);
        }
        if (PMC_SUCCESS == rc) 
        {
            rc = acb_ft_activate(handle->acb_ft_handle[serdes_id]);

            if (PMC_SUCCESS == rc)
            {
                handle->var[serdes_id].has_acb = TRUE;
            }
        }        
    }
    if (PMC_SUCCESS == rc) 
    {        
        if (S16_ACB_ONLY_LOOPBACK != handle->var[serdes_id].loopback_mode)
        {
            /* configure loopback */
            s16_loopback_cfg(handle,serdes_id, TRUE);
        }
        
        /* update state */
        handle->var[serdes_id].prev_prev_state = handle->var[serdes_id].prev_state;
        handle->var[serdes_id].prev_state = handle->var[serdes_id].state; 

        if (TRUE == handle->var[serdes_id].has_force_nominal_acb_ft_backup)
        { 
            handle->var[serdes_id].state = S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL;
        }
        else
        {
            handle->var[serdes_id].state = S16_PORT_OPERATIONAL_LOOPBACK;
        }        
    }
    if (PMC_SUCCESS == rc)
    {
        handle->var[serdes_id].s16_ctxt.lpbk_cfg_valid = TRUE;
        if ((s16_loopback_cfg_t *)(&handle->var[serdes_id].s16_ctxt.lpbk_cfg) != cfg_ptr)
        {
            PMC_MEMCPY(&handle->var[serdes_id].s16_ctxt.lpbk_cfg, cfg_ptr, sizeof(s16_loopback_cfg_t));
        }
    }

    PMC_RETURN(rc);
}/* s16_internal_loopback_activate */

/*******************************************************************************
* s16_as_a_clock_facilty_loopback_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function reconfigures S16 as a clock to support facility loopback.
*   This function should be called only when master lane bus width is 32 bits 
*   and lane used as a clock bus width is 20 bits. 
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   master_lane     - master lane index.
*   enable          - when TRUE, facility loopback is enabled and S16 as a clock
*                                should be configured as slave lane
*                   - when FALSE, facility loopback is enabled and S16 as a clock
*                                 should be configured as master lane
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_as_a_clock_facilty_loopback_cfg(s16_handle_t       *handle,
                                                     UINT8               serdes_id,
                                                     UINT8               master_lane,
                                                     BOOL8               enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    s16_csu_pcbi_buffer_t s16_csu_pcbi_buffer[1];
    acb_ft_serdes_if_t master_serdes_if;
    acb_ft_serdes_if_t lane_serdes_if;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    if (S16_PORT_START != handle->var[serdes_id].state &&
        S16_PORT_OPERATIONAL != handle->var[serdes_id].state)
    {
        PMC_RETURN(S16_STATE_NOT_START);
    }
    
    master_serdes_if = s16_serdes_if_get(handle, master_lane);
    lane_serdes_if = s16_serdes_if_get(handle, serdes_id);

    if (ACB_FT_SERDES_32b == master_serdes_if &&
        ACB_FT_SERDES_20b == lane_serdes_if)
    {
        /* configure PMM */
        s16_csu_pcbi_buffer_init(s16_csu_pcbi_buffer,handle, serdes_id);
        if (TRUE == enable)
        {
            /* reconfigure lane as a slave */
            s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set(s16_csu_pcbi_buffer,handle,serdes_id,master_lane);
            s16_csu_pcbi_field_PDM_TCLK_SEL_set(s16_csu_pcbi_buffer,handle,serdes_id,master_lane);
            s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set(s16_csu_pcbi_buffer,handle,serdes_id, 0x0);
        }
        else
        {
            /* reconfigure lane as a master */
            s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set(s16_csu_pcbi_buffer,handle,serdes_id, serdes_id);
            s16_csu_pcbi_field_PDM_TCLK_SEL_set(s16_csu_pcbi_buffer,handle,serdes_id, serdes_id);
            s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set(s16_csu_pcbi_buffer,handle,serdes_id, 0);
            
        }    
        s16_csu_pcbi_field_PDM_EN_set(s16_csu_pcbi_buffer, handle, serdes_id, enable);
        s16_csu_pcbi_buffer_flush(s16_csu_pcbi_buffer);
        
        /* if s16 lane is used as a clock, save ACB_FT config and configured default configuration */
        if (TRUE == handle->var[serdes_id].has_acb)
        {
            rc = acb_ft_deactivate(handle->acb_ft_handle[serdes_id]);
        }
        
        if (TRUE == enable)
        {
            acb_ft_cntn_cntd_recalculate_and_cfg(handle->acb_ft_handle[serdes_id],
                                                 32,
                                                 20);
        }
        else
        {
            acb_ft_cntn_cntd_recalculate_and_cfg(handle->acb_ft_handle[serdes_id],
                                                 20,
                                                 32);
        }
        
        rc = acb_ft_activate(handle->acb_ft_handle[serdes_id]); 
        if (PMC_SUCCESS == rc)
        {
            handle->var[serdes_id].has_acb = TRUE;
        }
    }
    else
    {
        rc = S16_LOG_ERR_INVALID_PARAMETERS;
    }
    
    PMC_RETURN(rc);
}/* s16_as_a_clock_facilty_loopback_cfg */

/*******************************************************************************
* s16_loopback_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Deactivate a loopback. This function may be called when the serdes is in 
*   S16_PORT_START or S16_PORT_OPERATIONAL state.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_loopback_deactivate(s16_handle_t        *handle,
                                         UINT8                serdes_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    acb_ft_prov_cfg_t acb_ft_cfg;
    BOOL8 acb_deprov = TRUE;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (S16_PORT_OPERATIONAL_LOOPBACK != handle->var[serdes_id].state &&
        S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL != handle->var[serdes_id].state)
    {
        PMC_RETURN(S16_STATE_NOT_OPERATIONAL_LOOPBACK);
    }
    
    /* restore ACB_FT configuration when it is required */
    if (S16_FACILITY_LOOPBACK == handle->var[serdes_id].loopback_mode ||
        S16_ACB_ONLY_LOOPBACK == handle->var[serdes_id].loopback_mode) 
    {
        /* configure PMM */
        if (PMC_SUCCESS == rc) 
          {
            acb_ft_prov_cfg_t acb_ft_cfg;
            PMC_MEMSET((void *) &acb_ft_cfg, 0, sizeof(acb_ft_prov_cfg_t));

            /* get the acb_ft configuration for the loopback */ 
            s16_acb_ft_loopback_cfg_get(handle, serdes_id, 
                                        &handle->var[serdes_id].s16_ctxt.lpbk_cfg,
                                        &acb_ft_cfg);
            s16_csu_lpbk_cfg(handle, serdes_id, &acb_ft_cfg, FALSE);    
        }
        if (PMC_SUCCESS == rc &&
            TRUE == handle->var[serdes_id].has_acb)
        {               
            rc = acb_ft_s16_loop_timing_deactivate(handle->acb_ft_handle[serdes_id]);
        } 
        if (PMC_SUCCESS == rc && 
            TRUE == handle->var[serdes_id].has_lpbk_acb_ft_backup)
        {
            /* put ACB in reset */
            s16_csu_acb_reset(handle, serdes_id, TRUE);
            /* put ACB in reset */
            s16_csu_acb_reset(handle, serdes_id, FALSE);           

            /* ACB FT is already in nominal mode, just copy loopback acb ft context in force nominal context backup */
            if (S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL == handle->var[serdes_id].state)
            {        
                handle->var[serdes_id].has_force_nominal_acb_ft_backup = TRUE;
                acb_ft_context_copy(handle->acb_ft_handle[serdes_id], S16_FORCE_NOMINAL_ACB_FT_CTXT_IDX, S16_FORCE_NOMINAL_ACB_FT_CTXT_IDX);
            }


            /* restore context saved before loopback activate*/
            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_context_pop(handle->acb_ft_handle[serdes_id], UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX);
            }

            if (S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL != handle->var[serdes_id].state)
            { 
                if (PMC_SUCCESS == rc &&
                    S16_SLAVE == handle->var[serdes_id].mld_mode)
                {
                    rc = acb_ft_activate(handle->acb_ft_handle[serdes_id]);

                    if (PMC_SUCCESS == rc)
                    {
                        handle->var[serdes_id].has_acb = TRUE;
                    }
                }
            }
            acb_deprov = FALSE;

            handle->var[serdes_id].has_lpbk_acb_ft_backup = FALSE;
        } 
        else 
        {
            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_deprov(handle->acb_ft_handle[serdes_id]);
                /* put ACB in reset */
                s16_csu_acb_reset(handle, serdes_id, TRUE);
            }
        }    
    }
    else
    {
        acb_deprov = FALSE;
    }

    if (PMC_SUCCESS == rc && S16_ACB_ONLY_LOOPBACK != handle->var[serdes_id].loopback_mode)    
    {
        s16_loopback_cfg(handle,serdes_id,FALSE);
    }

    /* reconfigure ACB FT in force nominal to make sure everything restarts cleanly after loopback deactivation*/
    if (PMC_SUCCESS == rc)
    {
        /* restore previous state */
        if (S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL == handle->var[serdes_id].state)
        {        
            handle->var[serdes_id].state = S16_PORT_OPERATIONAL_FORCE_NOMINAL;
            handle->var[serdes_id].prev_state = handle->var[serdes_id].prev_prev_state;
            acb_ft_cfg.mode = ACB_FT_MODE_NOMINAL;
        
            /* remove ACB in reset mode */
            s16_csu_acb_reset(handle, serdes_id, FALSE);

            if (TRUE == handle->var[serdes_id].has_acb)
            {
                rc = acb_ft_deactivate(handle->acb_ft_handle[serdes_id]);
            }
            if (FALSE == acb_deprov && PMC_SUCCESS == rc)
            {
                rc = acb_ft_deprov(handle->acb_ft_handle[serdes_id]);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_prov(handle->acb_ft_handle[serdes_id],&acb_ft_cfg);
            }
            if (PMC_SUCCESS == rc) 
            {
                rc = acb_ft_activate(handle->acb_ft_handle[serdes_id]);

                if (PMC_SUCCESS == rc)
                {
                    handle->var[serdes_id].has_acb = TRUE;
                }
            }
        }    
        else
        {
            handle->var[serdes_id].state = handle->var[serdes_id].prev_state;
        }        

        handle->var[serdes_id].prev_prev_state = LAST_S16_PORT_STATE;
    }
    if (PMC_SUCCESS == rc)
    {
        handle->var[serdes_id].s16_ctxt.lpbk_cfg_valid = FALSE;
    }
    PMC_RETURN(rc);
} /*s16_loopback_deactivate*/

/*******************************************************************************
* s16_loopback_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns the status of the S16 SERDES loopback.
*   
*
* INPUTS:
*   *handle              - pointer to s16 handle.
*   serdes_id            - serdes index.
*
* OUTPUTS:
*  *loopback_mode_ptr    - loopback mode
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_loopback_status_get(s16_handle_t *handle,
                                         UINT8 serdes_id,
                                         s16_loopback_mode_t *loopback_mode_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,
               S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    switch(handle->var[serdes_id].state)
    {
        case S16_PORT_OPERATIONAL_LOOPBACK:
        case S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL:
            *loopback_mode_ptr = handle->var[serdes_id].loopback_mode; 
            break;
        default:
            *loopback_mode_ptr = S16_LOOPBACK_MODE_UNUSED;
    }
    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* s16_tx_squelch_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables the TX squelch signal for the given serdes index.
*   
*
* INPUTS:
*   *handle              - pointer to s16 handle.
*   serdes_id            - serdes index.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void s16_tx_squelch_enable(s16_handle_t *handle,
                                  UINT8 serdes_id)
{

    PMC_ENTRY();

    s16_tx_squelch(handle, serdes_id, TRUE);

    PMC_RETURN();
} /* s16_tx_squelch_enable */

/*******************************************************************************
* s16_tx_squelch_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function disables the TX squelch signal for the given serdes index.
*   
*
* INPUTS:
*   *handle              - pointer to s16 handle.
*   serdes_id            - serdes index.
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
PUBLIC void s16_tx_squelch_disable(s16_handle_t *handle,
                                   UINT8 serdes_id)
{

    PMC_ENTRY();

    s16_tx_squelch(handle, serdes_id, FALSE);

    PMC_RETURN();
} /* s16_tx_squelch_disable */


/*******************************************************************************
* s16_tx_squelch_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function returns TX squelch status for the given serdes index.
*   
*
* INPUTS:
*   *handle              - pointer to s16 handle.
*   serdes_id            - serdes index.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when serdes tx squelch is enabled, otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 s16_tx_squelch_enable_status_get(s16_handle_t *handle,
                                             UINT8 serdes_id)
{
    BOOL8 rc;
    PMC_ENTRY();

    rc = (0x1 == ((s16_channel_pcbi_field_TPD_CTRL_OVR_EN_get(NULL, handle, serdes_id) & 
                   s16_channel_pcbi_field_TPD_CTRL_OVR_get(NULL, handle, serdes_id)))) 
        ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* s16_tx_squelch_enable_status_get */

/*******************************************************************************
* s16_max_ppm_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Configure the ACB FT maximum PPM.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   max_ppm         - maximum PPM.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void s16_max_ppm_set(s16_handle_t      *handle,
                                 UINT8         serdes_id,
                                 UINT32        max_ppm)
{
    UINT64 csu_n;
    UINT64 csu_d;
    UINT32 csu_r;
    DOUBLE csu_div;

    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    s16_csu_div_val_get(handle, serdes_id, &csu_div, &csu_n, &csu_d, &csu_r);
    PMC_LOG_TRACE("max_ppm = %d, csu_n=%lld, csu_d=%lld, csu_r=%d\n", max_ppm, csu_n, csu_d, csu_r);
    acb_ft_pll_s_max_cfg(handle->acb_ft_handle[serdes_id], max_ppm, csu_n, csu_d, csu_r);
    
    PMC_RETURN();
}/* s16_max_ppm_set */


/*******************************************************************************
* s16_tx_squelch
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function enables or disables the TX signal for the given 
*   serdes index.
*   
*
* INPUTS:
*   *handle              - pointer to s16 handle.
*   serdes_id            - serdes index.
*   enable               - TRUE/FALSE
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
PRIVATE void s16_tx_squelch(s16_handle_t *handle,
                            UINT8 serdes_id,
                            BOOL8 enable)
{

    UINT32 value;
    PMC_ENTRY();

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance,
               S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (enable == TRUE) {

        value = 1;

    } else {

        value = 0;
    }

    s16_channel_pcbi_field_TPD_CTRL_OVR_EN_set(NULL, handle, serdes_id, value); 
    s16_channel_pcbi_field_TPD_CTRL_OVR_set(NULL, handle, serdes_id, value);
    s16_channel_pcbi_reg_CHANNEL_PCBI_LOS_CFG_1_field_set(NULL, handle, serdes_id, 
                                                          PCBI_REG_CHANNEL_PCBI_LOS_CFG_1_BIT_TX_SQUELCH_OVR_MSK, 
                                                          PCBI_REG_CHANNEL_PCBI_LOS_CFG_1_BIT_TX_SQUELCH_OVR_OFF, 
                                                          value);

    PMC_RETURN();
}

/*******************************************************************************
* s16_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Enable or disable loopback.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   enable          - TRUE to enable loopback, FALSE to disable it
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void s16_loopback_cfg(s16_handle_t *handle, UINT8 serdes_id, BOOL8 enable) 
{
    UINT32 tx_oenb;

    PMC_ENTRY();

    if (S16_DIAGNOSTIC_LOOPBACK == handle->var[serdes_id].loopback_mode ||
        S16_DIAGNOSTIC_WITH_TX_LOOPBACK == handle->var[serdes_id].loopback_mode) 
    {
        /* TX -> RX loopback */
        s16_channel_pcbi_field_LOOPBACK_TX_RX_EN_set(NULL,handle,serdes_id,(enable ? 1 : 0));
        s16_channel_pcbi_field_RXLOS_EXT_LOOPBACK_EN_set(NULL,handle,serdes_id,(enable ? 1 : 0)); 
        tx_oenb  = (S16_DIAGNOSTIC_WITH_TX_LOOPBACK == handle->var[serdes_id].loopback_mode) ? 0 : 1;
        if (FALSE == enable)
        {
            tx_oenb = 0;
        }
        s16_mtsb_ctrl_field_T_TX_OENB_set(NULL,handle, serdes_id, tx_oenb);             
    } 
    else 
    {
        /* RX -> TX Loopback */
        UINT32 tx_source;
        tx_source = (enable ? S16_TX_SOURCE_INTERNAL_LOOPBACK : S16_TX_SOURCE_FUNCTIONAL_MODE); 
        s16_channel_pcbi_field_TX_SOURCE_set(NULL,handle,serdes_id,tx_source);

        s16_channel_pcbi_field_RXLOS_INT_LOOPBACK_EN_set(NULL,handle,serdes_id, 0);
        s16_channel_pcbi_field_RXLOS_EXT_LOOPBACK_EN_set(NULL,handle,serdes_id, 0);
    }
    PMC_RETURN();
}/*s16_loopback_cfg*/

/*******************************************************************************
* s16_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the S16 instance configured loopback mode.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   s16_loopback_mode_t - loopback mode.
*
* NOTES:
*
*******************************************************************************/
PUBLIC s16_loopback_mode_t s16_loopback_cfg_get(s16_handle_t *handle, 
                                                UINT8         serdes_id) 
{
    s16_loopback_mode_t lpbk_mode = S16_LOOPBACK_MODE_UNUSED;
    UINT32 rxlos_ext_lpbk;
    acb_ft_prov_cfg_t acb_ft_cfg;

    PMC_ENTRY();
 
    /* read few registers values to interpret configured loopback mode */  
    if (serdes_id < handle->cfg.num_serdes_instance &&
        S16_PORT_RESET != handle->var[serdes_id].state)
    {
        rxlos_ext_lpbk = s16_channel_pcbi_field_LOOPBACK_TX_RX_EN_get(NULL,handle,serdes_id);
        
        if (rxlos_ext_lpbk)
        {
            if (0x1 == s16_channel_pcbi_field_RXLOS_EXT_LOOPBACK_EN_get(NULL,handle,serdes_id))
            {
                if (0x1 == s16_mtsb_ctrl_field_T_TX_OENB_get(NULL,handle,serdes_id))
                {
                    lpbk_mode = S16_DIAGNOSTIC_LOOPBACK;
                }
                else
                {
                    lpbk_mode = S16_DIAGNOSTIC_WITH_TX_LOOPBACK;
                }
            }
        }
        else 
        {
            if ((UINT32)S16_TX_SOURCE_INTERNAL_LOOPBACK == s16_channel_pcbi_field_TX_SOURCE_get(NULL,handle,serdes_id) && 
                0x0 == s16_channel_pcbi_field_RXLOS_INT_LOOPBACK_EN_get(NULL,handle,serdes_id))
            {
                lpbk_mode = S16_FACILITY_LOOPBACK;
            }
            else
            {
                if (TRUE == handle->var[serdes_id].s16_ctxt.lpbk_cfg_valid)
                {
                    /* check if acb_ft only loopback is activate */
                    s16_acb_ft_loopback_cfg_get(handle, serdes_id,
                                                &handle->var[serdes_id].s16_ctxt.lpbk_cfg,
                                                &acb_ft_cfg);
                    if (acb_ft_cfg.packet_size_C == s16_csu_pcbi_field_PMM_PKT_SIZE_get(NULL, handle, serdes_id))
                    {
                        lpbk_mode = S16_ACB_ONLY_LOOPBACK;                
                }
                }
            }
        }
    }

        
    PMC_RETURN(lpbk_mode);
}/*s16_loopback_cfg_get*/
    
    
/*******************************************************************************
* s16_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the var context to its default values
*
* INPUTS:
*  None.
*
* OUTPUTS:
*   *var_ptr            - initialized variable
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void s16_var_default_init(s16_var_t    *var_ptr)
{
    PMC_ENTRY();

    var_ptr->mld_mode = LAST_S16_MLD_MODE;    
    var_ptr->state = S16_PORT_RESET;
    var_ptr->has_acb = FALSE;
    var_ptr->patt_gen_enabled = FALSE;
    var_ptr->loopback_mode = S16_LOOPBACK_MODE_UNUSED;
    var_ptr->prev_state = S16_PORT_RESET;
    var_ptr->has_lpbk_acb_ft_backup = FALSE;
    var_ptr->has_force_nominal_acb_ft_backup = FALSE;
    var_ptr->clk_cfg = (UINT8)S16_CLK_CFG_NOT_CLK_LANE;
    PMC_MEMSET((void*)&var_ptr->s16_ctxt, 0, sizeof(s16_ctxt_t));
    var_ptr->s16_ctxt.polarity_inv = LAST_UTIL_GLOBAL_DIR;
    PMC_RETURN();
}

/*******************************************************************************
* s16_csu_div_val_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extract csu divider values
*
* INPUTS:
*   *handle           - pointer to S16 handle instance.
*  lane               - extract CSU values from this lane
*
* OUTPUTS:
*   *csu_div          - CSU divider in DOUBLE notation
*   *csu_r            - CSU PLL Integer Multiplication
*   *csu_denomin      - CSU divider denominator
*   *csu_n            - CSU divider nominator
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void s16_csu_div_val_get(s16_handle_t *handle, 
                                 UINT8 lane, 
                                 DOUBLE *csu_div, 
                                 UINT64 *csu_n, 
                                 UINT64 *csu_denomin, 
                                 UINT32 *csu_r)
{
    UINT32 CSU0_DENOMIN_39_32;
    UINT32 CSU0_DENOMIN_31_0;
    UINT32 CSU0_FRAC_DIV_31_0;
    UINT32 CSU0_FRAC_DIV_39_32;
    
    PMC_ENTRY();
       
    *csu_r               = s16_csu_pcbi_field_CSU0_INT_DIV_get(NULL,handle,lane);
    CSU0_DENOMIN_39_32  = s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get(NULL,handle,lane);
    CSU0_DENOMIN_31_0   = s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get(NULL,handle,lane);
    CSU0_FRAC_DIV_31_0  = s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get(NULL,handle,lane);
    CSU0_FRAC_DIV_39_32 = s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get(NULL,handle,lane);
    *csu_n = ((UINT64) CSU0_FRAC_DIV_39_32 << 32) + CSU0_FRAC_DIV_31_0;
    *csu_denomin = ((UINT64) CSU0_DENOMIN_39_32 << 32) + CSU0_DENOMIN_31_0;
    PMC_ASSERT(*csu_denomin != 0, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    if (!digi_use_float32_get()) {
        *csu_div = (DOUBLE) *csu_r + ( (DOUBLE) *csu_n / (DOUBLE) *csu_denomin );
    } else {
        *csu_div = (DOUBLE)((FLOAT) *csu_r + ( (FLOAT) *csu_n / (FLOAT) *csu_denomin ));
    }
    PMC_RETURN();
}  /*s16_csu_div_val_get*/

/*******************************************************************************
* s16_csu_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CSU 
*
* INPUTS:
*   *handle           - pointer to S16 handle instance.
*   lane              - the lane to configure 
*   master_lane       - the master lane in multi-lane configuration
*   enable            - TRUE or FALSE to enable it 
*   cleanup           - when cleanup is TRUE, s16 state or mode are not 
*                       check before writing over register bits. Otherwise
*                       they are checked.
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
PRIVATE void s16_csu_cfg(s16_handle_t *handle,
                         UINT8 lane, 
                         UINT8 master_lane,
                         BOOL8 enable,
                         BOOL8 cleanup)
{
    s16_csu_pcbi_buffer_t s16_csu_pcbi_buffer[1];

    PMC_ENTRY();

    if (TRUE == cleanup)
    {
        enable = FALSE;
    }

    s16_csu_pcbi_buffer_init(s16_csu_pcbi_buffer,handle,lane);
    
    if (TRUE == enable)
    {
        s16_csu_pcbi_field_PDM_UPDATE_RATE_set(s16_csu_pcbi_buffer,handle,lane,S16_PDM_UPDATE_RATE);    
    }

    if (S16_SLAVE ==  handle->var[lane].mld_mode ||
        TRUE == cleanup)    
    {
        if (TRUE == enable)
        {            
            s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set(s16_csu_pcbi_buffer,handle,lane,master_lane);
            s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set(s16_csu_pcbi_buffer,handle,lane,S16_SLAVE_PDM_ACCUM_CYCLES);
            s16_csu_pcbi_field_PDM_GAIN_set(s16_csu_pcbi_buffer,handle,lane,S16_SLAVE_PDM_GAIN);
            s16_csu_pcbi_field_PDM_TCLK_SEL_set(s16_csu_pcbi_buffer,handle,lane,master_lane ); 
        }
        if (S16_CLK_CFG_AS_MASTER != (s16_clk_cfg_t)handle->var[lane].clk_cfg) 
        {
            s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set(s16_csu_pcbi_buffer,handle,lane,enable);
        }
        else
        {
            s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set(s16_csu_pcbi_buffer,handle,lane, 0);
        }

        if ((S16_CLK_CFG_NOT_CLK_LANE != (s16_clk_cfg_t)handle->var[lane].clk_cfg &&
             s16_channel_pcbi_field_MTSB_BUS_WIDTH_get(NULL, handle, master_lane) != 0x1) ||
            TRUE == cleanup ||
            S16_CLK_CFG_AS_MASTER == (s16_clk_cfg_t)handle->var[lane].clk_cfg)
        {
            s16_csu_pcbi_field_PDM_EN_set(s16_csu_pcbi_buffer,handle,lane,FALSE);
        }
        else 
        {
            s16_csu_pcbi_field_PDM_EN_set(s16_csu_pcbi_buffer,handle,lane,enable);
        }
    }
            
    s16_csu_pcbi_buffer_flush(s16_csu_pcbi_buffer);
        
    PMC_RETURN();
} /*s16_csu_cfg*/

/*******************************************************************************
* s16_csu_lpbk_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CSU PMM 
*
* INPUTS:
*   *handle           - pointer to S16 handle instance.
*   lane              - the lane to configure 
*   *acb_cfg_ptr      - pointer to ACB_FT provisioning configuration
*   enable            - TRUE or FALSE to enable it 
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
PRIVATE void s16_csu_lpbk_cfg(s16_handle_t *handle,
                              UINT8 lane, 
                              acb_ft_prov_cfg_t *acb_cfg_ptr, 
                              BOOL8 enable)
{
    s16_csu_pcbi_buffer_t s16_csu_pcbi_buffer[1];
    BOOL8 inv_enable = enable == TRUE?FALSE:TRUE;
    PMC_ENTRY();
    
         
    /* this function is used in loopback mode */
    /* when we are a slave, we disable PDM_EN in loopback mode since we will get the information 
       from the local ACB, we have to enable it when we disable the loopback
    */
    if (S16_SLAVE ==  handle->var[lane].mld_mode)    
    {
        s16_csu_pcbi_buffer_init(s16_csu_pcbi_buffer,handle,lane);
        /* For S16 lane used as a clock only, CSU0_FRAC_DIV_MOD_EXT_EN should be set to 0 in normal mode */
        s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set(s16_csu_pcbi_buffer,handle,lane, inv_enable);

        if (S16_CLK_CFG_NOT_CLK_LANE != (s16_clk_cfg_t)handle->var[lane].clk_cfg && acb_cfg_ptr->serdes_if != ACB_FT_SERDES_20b)
        {
            s16_csu_pcbi_field_PDM_EN_set(s16_csu_pcbi_buffer,handle,lane,FALSE);        
        }
        else 
        {
            s16_csu_pcbi_field_PDM_EN_set(s16_csu_pcbi_buffer,handle,lane,inv_enable);        
        }

        s16_csu_pcbi_buffer_flush(s16_csu_pcbi_buffer);
    }

    s16_pmm_cfg(handle, lane, acb_cfg_ptr,  enable);
            
    PMC_RETURN();
} /*s16_csu_lpbk_cfg*/

/*******************************************************************************
* s16_pmm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CSU PMM 
*
* INPUTS:
*   *handle           - pointer to S16 handle instance.
*   lane              - the lane to configure 
*   *acb_cfg_ptr      - pointer to ACB_FT provisioning configuration
*   enable            - TRUE to enable PMM, FALSE to disable it 
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
PRIVATE void s16_pmm_cfg(s16_handle_t *handle,
                             UINT8 lane, 
                             acb_ft_prov_cfg_t *acb_cfg_ptr, 
                             BOOL8 enable)
{
    s16_csu_pcbi_buffer_t s16_csu_pcbi_buffer[1];
    PMC_ENTRY();
    
    s16_csu_pcbi_buffer_init(s16_csu_pcbi_buffer,handle,lane);
  
    if (TRUE == enable)
    {
        s16_csu_pcbi_field_PMM_DSM_DENOM_set(s16_csu_pcbi_buffer,handle,lane,S16_PMM_DSM_DENOM);
        s16_csu_pcbi_field_PMM_PKT_PERIOD_set(s16_csu_pcbi_buffer,handle,lane,S16_PMM_PACKET_PERIOD);
        /* Keep PMM disabled for this reg setting. */
        s16_csu_pcbi_field_PMM_ENABLE_set(s16_csu_pcbi_buffer,handle,lane,FALSE);    
        s16_csu_pcbi_field_PMM_PKT_SIZE_set(s16_csu_pcbi_buffer,handle,lane, acb_cfg_ptr->packet_size_C);
        /* Standard Second Order DSM */
        s16_csu_pcbi_field_PMM_DSM_TYPE_set(s16_csu_pcbi_buffer,handle,lane,S16_PMM_DSM_TYPE);
        /* Flush buffer since we re-enable PMMM*/
        s16_csu_pcbi_buffer_flush(s16_csu_pcbi_buffer);
    }    
    s16_csu_pcbi_field_PMM_ENABLE_set(NULL,handle,lane,enable);    
            
    PMC_RETURN();
} /*s16_pmm_cfg*/


/*******************************************************************************
* s16_csu_acb_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CSU ACB reset
*
* INPUTS:
*   *handle           - pointer to S16 handle instance.
*   lane              - the lane to configure 
*   reset             - control ACB reset
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
PRIVATE void s16_csu_acb_reset(s16_handle_t *handle,
                               UINT8 lane, 
                               BOOL8 reset)
{    
    PMC_ENTRY();
    
    s16_csu_pcbi_field_ACB_RSTB_set(NULL,handle,lane,(reset == TRUE?0:1));
                
    PMC_RETURN();
} /*s16_csu_acb_reset*/

/*******************************************************************************
* s16_patt_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes S16 pattern monitor: 
*    - It sets pattern mode,
*    - It enables/disables pattern inversion check. 
*    - It activates/deactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - s16 handle.
*   serdes_id              - serdes index.
*   port_mode              - serdes port rate.
*   *patt_mon_cfg_ptr      - pattern monitor configuration.

** OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_patt_mon_cfg(s16_handle_t *handle,
                                  UINT8 serdes_id,
                                  s16_port_mode_t port_mode,
                                  util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr)
{
    s16_channel_pcbi_buffer_t s16_buf;
    s16_patt_mode_t s16_mode;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 usr_patt_size;
    UINT32 i;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(patt_mon_cfg_ptr != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    } 

    if (PMC_SUCCESS == result && patt_mon_cfg_ptr->patt_cfg.enable == TRUE)
    {           

        /*Reset the checker before starting. Hold active until lane aligner resets      PM55_82_50_CHANNEL      CHANNEL_PCBI_RESET_CFG_1        PATT_CHK_SW_RESET*/
        s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 1);       

        /*enable the checker    PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_EN*/
        s16_channel_pcbi_field_CHK_EN_set(NULL, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.enable);

        /*    Reset the Rate adapt FIFO pointers so that correct data alignment occurs  PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 1*/
        s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 1);

        /*      PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 0 */
        s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 0);                

        result = util_patt_mode_to_s16_mode_conv(patt_mon_cfg_ptr->patt_cfg.mode, &s16_mode);
        
        if (PMC_SUCCESS == result)
        {       
            /*  set monitor user-defined pattern when mode matches */
            if (S16_PATT_MODE_USER_DEF == s16_mode ||
                S16_PATT_MODE_CJAT == s16_mode)
            {
                result = s16_validate_usr_patt(&patt_mon_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                usr_patt_size = patt_mon_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_mon_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
                
                for (i = 0; i < usr_patt_size; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        s16_channel_pcbi_field_CHK_USR_PAT_31_TO_0_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        s16_channel_pcbi_field_CHK_USR_PAT_63_TO_32_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 2:
                        s16_channel_pcbi_field_CHK_USR_PAT_95_TO_64_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 3:
                        s16_channel_pcbi_field_CHK_USR_PAT_127_TO_96_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 4:
                        s16_channel_pcbi_field_CHK_USR_PAT_159_TO_128_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 5:
                        s16_channel_pcbi_field_CHK_USR_PAT_191_TO_160_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 6:
                        s16_channel_pcbi_field_CHK_USR_PAT_223_TO_192_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 7:
                        s16_channel_pcbi_field_CHK_USR_PAT_255_TO_224_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 8:
                        s16_channel_pcbi_field_CHK_USR_PAT_287_TO_256_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 9:
                        s16_channel_pcbi_field_CHK_USR_PAT_319_TO_288_set(NULL, handle, serdes_id, patt_mon_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = S16_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }
            }
        
            if (PMC_SUCCESS == result)
            {  
                /* initialize s16 buffer */     
                s16_channel_pcbi_buffer_init(&s16_buf, handle, (UINT32)serdes_id);  

                /* set check inv, pattern mode and enable/disable */
                s16_channel_pcbi_field_CHK_INV_set(&s16_buf, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.inv_cfg);         
                s16_channel_pcbi_field_CHK_MODE_set(&s16_buf, handle, serdes_id, (UINT32) s16_mode); 
                

                /* flush s16 buffer */ 
                s16_channel_pcbi_buffer_flush(&s16_buf);

                /* Clear Pattern checker reset  PM55_82_50_CHANNEL      CHANNEL_PCBI_RESET_CFG_1        PATT_CHK_SW_RESET */
                s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 0);
        
                /*      Issue one checker status read to clear old state.       PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_READ        W       b1 */ 
                s16_channel_pcbi_field_CHK_READ_set(NULL, handle, serdes_id, 1);
                /*      Wait 10 PCLK cycles                             sleep(50ns)     */

                /*              PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_READ        W       b0 */
                s16_channel_pcbi_field_CHK_READ_set(NULL, handle, serdes_id, 0);

            }
        }       
    }
    else
    {
        s16_channel_pcbi_field_CHK_EN_set(NULL, handle, serdes_id, (UINT32)patt_mon_cfg_ptr->patt_cfg.enable);
    }


    PMC_RETURN(result);
}/*s16_patt_mon_cfg*/

/*******************************************************************************
* s16_all_ones_zeroes_detection_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes S16 pattern monitor: 
*    - It sets pattern mode,
*    - It enables/disables pattern inversion check. 
*    - It activates/deactivates pattern monitor.
*
*
* INPUTS:
*   *handle                - s16 handle.
*   serdes_id              - serdes index.
*   enable                 - when TRUE dectection is enabled, otherwise it is disabled.  
*   detect_all_ones        - when TRUE all ones detection is configured, 
*                            otherwise all zeroes detection is configured.
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_all_ones_zeroes_detection_cfg(s16_handle_t *handle,
                                                   UINT8         serdes_id,
                                                   BOOL8         enable,
                                                   BOOL8         detect_all_ones)
{
    s16_channel_pcbi_buffer_t s16_buf;
    const s16_patt_mode_t s16_mode = S16_PATT_MODE_USER_DEF;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 patt_val;
    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);

    
    /* check serdes ID */ 
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    } 
    
    patt_val = 0xFFFFFFFF;
    if (TRUE == enable)
    {
        s16_channel_pcbi_field_CHK_MODE_set(NULL, handle, serdes_id, (UINT32) s16_mode); 

        /*  set monitor user-defined pattern when mode matches */
        s16_channel_pcbi_field_CHK_USR_PAT_31_TO_0_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_63_TO_32_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_95_TO_64_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_127_TO_96_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_159_TO_128_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_191_TO_160_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_223_TO_192_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_255_TO_224_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_287_TO_256_set(NULL, handle, serdes_id, patt_val);
        s16_channel_pcbi_field_CHK_USR_PAT_319_TO_288_set(NULL, handle, serdes_id, patt_val);
        
        /* initialize s16 buffer */     
        s16_channel_pcbi_buffer_init(&s16_buf, handle, (UINT32)serdes_id);  
        
        /* set check inv, pattern mode and enable/disable */
        s16_channel_pcbi_field_CHK_INV_set(NULL, handle, serdes_id, TRUE == detect_all_ones ? FALSE : TRUE);         
                
        /* flush s16 buffer */ 
        s16_channel_pcbi_buffer_flush(&s16_buf);                        
    }


    /*Reset the checker before starting. Hold active until lane aligner resets      PM55_82_50_CHANNEL      CHANNEL_PCBI_RESET_CFG_1        PATT_CHK_SW_RESET*/
    s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 1);
    
    /*enable the checker    PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_EN*/
    s16_channel_pcbi_field_CHK_EN_set(NULL, handle, serdes_id, (UINT32)enable);
    
    /*    Reset the Rate adapt FIFO pointers so that correct data alignment occurs  PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 1*/
    s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 1);
    
    /*      PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 0 */
    s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 0);
    

    /* Reset the checker before starting. Hold active until lane aligner resets      PM55_82_50_CHANNEL      CHANNEL_PCBI_RESET_CFG_1        PATT_CHK_SW_RESET */
    s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 0);
    
    /*      Issue one checker status read to clear old state.       PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_READ        W       b1 */
    s16_channel_pcbi_field_CHK_READ_set(NULL, handle, serdes_id, 1);
    /*      Wait 10 PCLK cycles                             sleep(50ns)     */
    
    /*              PM55_82_50_CHANNEL      CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1    CHK_READ        W       b0 */
    s16_channel_pcbi_field_CHK_READ_set(NULL, handle, serdes_id, 0);
    
    PMC_RETURN(result);
}/*s16_all_ones_zeroes_detection_cfg*/


/*******************************************************************************
* s16_patt_mon_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function enables pattern monitor check. s16_patt_mon_cfg function shall 
*    be called prior to this function. This function shall be called after
*    device starts receiving pattern. 
*
*
* INPUTS:
*   *handle                - s16 handle.
*   serdes_id              - serdes index.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_patt_mon_resync(s16_handle_t *handle,
                                     UINT8 serdes_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 patt_en;

    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);

    /* check serdes ID */
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    }

    patt_en = s16_channel_pcbi_field_CHK_EN_get(NULL, handle, serdes_id);

    if (0 != patt_en)
    {
        /* enable pattern monitor */
        s16_channel_pcbi_field_CHK_EN_set(NULL, handle, serdes_id, 0);
        s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 1);
        s16_channel_pcbi_field_CHK_EN_set(NULL, handle, serdes_id, 1);
        /*    Reset the Rate adapt FIFO pointers so that correct data alignment occurs  PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 1*/
        s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 1);
        
        /*      PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_LANE_N_CFG_REG        FIFO_HW_RESET = 0 */
        s16_mtsb_ctrl_field_FIFO_HW_RESET_set(NULL, handle, serdes_id, 0);               
        s16_channel_pcbi_field_PATT_CHK_SW_RESET_set(NULL, handle, serdes_id, 0); 
        
        
        /* activate pattern monitor error counter read and update in mon_err_cnt register  */
        /* emulate faling-edge to allow future read */
        s16_channel_pcbi_field_CHK_READ_set (NULL, handle, serdes_id, 1);
        s16_channel_pcbi_field_CHK_READ_set (NULL, handle, serdes_id, 0);
    }
    else
    {
        result = S16_LOG_ERR_PATT_MON_DISABLED;
    }
    
    PMC_RETURN(result);
}

/*******************************************************************************
* s16_patt_gen_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function configures one serdes S16 pattern generator: 
*    - It sets pattern mode.
*    - It enables/disables pattern inversion.
*    - It enables/disables error injection. 
*    - It activates/deactivates pattern generator.
*    - It sets pattern mode.
*
*
* INPUTS:
*   *handle                - s16 handle.
*   serdes_id              - serdes index.
*   port_mode              - serdes port rate.
*   *patt_gen_cfg_ptr      - pointer to pattern generator configuration.
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_patt_gen_cfg(s16_handle_t *handle,
                                  UINT8 serdes_id,
                                  s16_port_mode_t port_mode,
                                  util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr)
{
    s16_channel_pcbi_buffer_t s16_buf;
    s16_patt_mode_t s16_mode;
    UINT32 i;
    UINT32 usr_patt_size;
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 t_piso_ctrl;

    PMC_ENTRY();

    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0); 
    PMC_ASSERT(patt_gen_cfg_ptr != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    } 
    
    /* check if we have a conflict between pattern generator and FACILITY loopback */
    if (S16_PORT_OPERATIONAL_LOOPBACK == handle->var[serdes_id].state)
    {
        if (S16_FACILITY_LOOPBACK == handle->var[serdes_id].loopback_mode)
        {
            PMC_RETURN(S16_PATT_GEN_FACILITY_LOOPBACK_CONFLICT);
        }
    }

    if (patt_gen_cfg_ptr->patt_cfg.enable == TRUE)
    {
        /* save s16 context */
        s16_tx_ctxt_push(handle, serdes_id);

        t_piso_ctrl = s16_mtsb_ctrl_field_T_PISO_CTRL_get(NULL, handle, serdes_id);
        t_piso_ctrl &= 0xfffffffB;
        s16_mtsb_ctrl_field_T_PISO_CTRL_set(NULL, handle, serdes_id, t_piso_ctrl);

        /* Config from S16 configuration guide */
        /* 
           Set MTSB Bus width to 20bits. Rx data out of SERDES not valid for normally 16-bit or 32-bit MTSB bus width rates.    
           PM55_82_50_CHANNEL      CHANNEL_PCBI_RESET_CFG_1        MTSB_BUS_WIDTH 
        */
        s16_channel_pcbi_field_MTSB_BUS_WIDTH_set(NULL, handle, serdes_id, 0x1);
   
        /* Select the Pattern Generator to drive the MABC TX parallel interface.        PM55_82_50_CHANNEL      CHANNEL_PCBI_TX_CFG_1   TX_SOURCE */     
        s16_channel_pcbi_field_TX_SOURCE_set(NULL, handle, serdes_id, S16_TX_SOURCE_PATTERN_GENERATOR);
        
        result = util_patt_mode_to_s16_mode_conv(patt_gen_cfg_ptr->patt_cfg.mode, &s16_mode);
        if (PMC_SUCCESS == result)
        {
        
            /*  set generator user-defined pattern when mode matches */
            if (S16_PATT_MODE_USER_DEF == s16_mode ||
                S16_PATT_MODE_CJAT == s16_mode)
            {
                result = s16_validate_usr_patt(&patt_gen_cfg_ptr->patt_cfg);
                if (PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
                
                usr_patt_size = patt_gen_cfg_ptr->patt_cfg.usr_patt_size / 32;
                if ((patt_gen_cfg_ptr->patt_cfg.usr_patt_size % 32) > 0)
                {
                    usr_patt_size += 1;
                }
                
                for (i = 0; i < usr_patt_size; i ++)
                {
                    switch (i)
                    {
                    case 0:
                        s16_channel_pcbi_field_GEN_USR_PAT_31_TO_0_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 1:
                        s16_channel_pcbi_field_GEN_USR_PAT_63_TO_32_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 2:
                        s16_channel_pcbi_field_GEN_USR_PAT_95_TO_64_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 3:
                        s16_channel_pcbi_field_GEN_USR_PAT_127_TO_96_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 4:
                        s16_channel_pcbi_field_GEN_USR_PAT_159_TO_128_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 5:
                        s16_channel_pcbi_field_GEN_USR_PAT_191_TO_160_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 6:
                        s16_channel_pcbi_field_GEN_USR_PAT_223_TO_192_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 7:
                        s16_channel_pcbi_field_GEN_USR_PAT_255_TO_224_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 8:
                        s16_channel_pcbi_field_GEN_USR_PAT_287_TO_256_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    case 9:
                        s16_channel_pcbi_field_GEN_USR_PAT_319_TO_288_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.usr_patt[i]);
                        break;
                    default: 
                        result = S16_LOG_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }       
            }
            
            if (PMC_SUCCESS == result)
            {
                /* initialize s16 buffer */     
                s16_channel_pcbi_buffer_init(&s16_buf, handle, (UINT32)serdes_id);  
                                
                /* set check inv, pattern mode and enable/disable */
                s16_channel_pcbi_field_GEN_INV_set(&s16_buf, handle, serdes_id, (UINT32)patt_gen_cfg_ptr->patt_cfg.inv_cfg);
                s16_channel_pcbi_field_GEN_MODE_set(&s16_buf, handle, serdes_id, (UINT32)s16_mode);

                /* flush s16 buffer */ 
                s16_channel_pcbi_buffer_flush(&s16_buf);

                /* Setup the PISO divider for correct mode      PM55_82_50_MTSB_CTRL    DIGI120_MTSB_CTRL_GLOBAL_PISO_CFG_1     T_PISO_CTRL */
 
                result = s16_t_piso_ctrl_val_get(port_mode, &t_piso_ctrl);
                if (PMC_SUCCESS == result)
                {
                    s16_mtsb_ctrl_field_T_PISO_CTRL_set(NULL, handle, serdes_id, t_piso_ctrl);
                    
                    s16_channel_pcbi_field_GEN_EN_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.enable);
                }
            }
        }
    }
    else
    {
        /* set default value */
        s16_channel_pcbi_field_GEN_EN_set(NULL, handle, serdes_id, patt_gen_cfg_ptr->patt_cfg.enable);

        /* restore s16 TX registers */
        s16_tx_ctxt_pop(handle, serdes_id);        
    }       


    /* update status */
    handle->var[serdes_id].patt_gen_enabled = patt_gen_cfg_ptr->patt_cfg.enable;

    PMC_RETURN(result);
} /*s16_patt_gen_cfg*/

/*******************************************************************************
* s16_patt_gen_err_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function enables or disables error insertion in generated pattern.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_patt_gen_err_insert_cfg(s16_handle_t *handle,
                                             UINT8 serdes_id)
 
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    }

    /* emulate rising edge on GEN_ERR_INJ register bit */
    /* in case bit is already high */
    s16_channel_pcbi_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, (UINT32)0);
    s16_channel_pcbi_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, (UINT32)1);

    /* lower register bit  */
    s16_channel_pcbi_field_GEN_ERR_INJ_set(NULL, handle, serdes_id, (UINT32)0);
    
    PMC_RETURN(PMC_SUCCESS);
}/*s16_patt_gen_err_insert_cfg*/


/*******************************************************************************
* s16_patt_mon_err_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function reads and returns pattern monitor status and errors counter value.
*
* INPUTS:
*   *handle                - s16 handle
*   serdes_id              - Serdes index
* OUTPUTS:
*   *status                - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_count             - Number of errors detected by pattern monitor. 
*                            The value shall be ignored when pattern monitor
*                            is not in locked state.
*
* RETURNS:
*   PMC_SUCCESS            - when counter value is successfully read 
*                            otherwise an error code is returned
*
* NOTES:
*
*******************************************************************************/   
PUBLIC PMC_ERROR s16_patt_mon_err_count_get(s16_handle_t *handle,
                                            UINT8 serdes_id,
                                            util_patt_status_t    *status,
                                            UINT32 *err_count)
{

    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(status != NULL, S16_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(err_count != NULL, S16_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_ENTRY();

    /* activate pattern monitor error counter read and update in mon_err_cnt register  */
    s16_channel_pcbi_field_CHK_READ_set (NULL, handle, serdes_id, 1);

    /* emulate faling-edge to allow future read */
    s16_channel_pcbi_field_CHK_READ_set (NULL, handle, serdes_id, 0);

    /* Wait 10 PCLK cycles */
    *err_count = s16_channel_pcbi_field_CHK_ERR_CNT_get (NULL, handle, serdes_id);

    switch(*err_count)
    {
    case 0xFFFFFFFF: 
        *status = UTIL_PATT_STATUS_DISABLED;
        break;
    case 0xFFFFFFFE: 
        *status = UTIL_PATT_STATUS_TRYING_TO_LOCK;
        break;
    case 0xFFFFFFFD:
        *status = UTIL_PATT_STATUS_ZERO_DET;
        break;
    default:
        *status = UTIL_PATT_STATUS_LOCKED;
        break;
    }

    PMC_RETURN(PMC_SUCCESS);
}  


/*******************************************************************************
* s16_validate_usr_patt 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function verifies that the length of user-defined pattern is compatible 
*   to s16 serdes.
*
* INPUTS:
*  *patt_cfg         - pointer to util global pattern mode enum
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS      - when user-defined pattern is correct is successful 
*                      otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_validate_usr_patt(util_serdes_patt_cfg_t *patt_cfg)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != patt_cfg,S16_LOG_ERR_NULL_HANDLE,0,0);
    if (patt_cfg->usr_patt_size != S16_USR_PATT_BIT_SIZE)
    {
        result = S16_LOG_ERR_USR_PATT_SIZE_INVALID;
    }

    PMC_RETURN(result);
} /* util_patt_mode_to_s16_mode_conv */

/*******************************************************************************
* util_patt_mode_to_s16_mode_conv 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert pattern mode defined defined in util global to s16 pattern mode enum.
*
*
* INPUTS:
*  util_patt_mode         - util global pattern mode enum
*
* OUTPUTS:
*   *s16_mode              - s16 pattern mode enum
*
* RETURNS:
*   PMC_SUCCESS            - when conversion is done 
*                            otherwise a descriptive error is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_patt_mode_to_s16_mode_conv(util_patt_mode_t util_patt_mode,
                                                  s16_patt_mode_t *s16_mode)
{
    PMC_ENTRY();

    switch (util_patt_mode)
    {
    case UTIL_PATT_MODE_PRBS_5:
        *s16_mode = S16_PATT_MODE_PRBS_5;
        break;  
    case UTIL_PATT_MODE_PRBS_7:
        *s16_mode = S16_PATT_MODE_PRBS_7;
        break;  
    case UTIL_PATT_MODE_PRBS_11:
        *s16_mode = S16_PATT_MODE_PRBS_11;
        break;
    case UTIL_PATT_MODE_PRBS_15:
        *s16_mode = S16_PATT_MODE_PRBS_15;
        break;
    case UTIL_PATT_MODE_PRBS_20:
        *s16_mode = S16_PATT_MODE_PRBS_20;
        break;
    case UTIL_PATT_MODE_PRBS_23:
        *s16_mode = S16_PATT_MODE_PRBS_23;
        break;
    case UTIL_PATT_MODE_PRBS_31:
        *s16_mode = S16_PATT_MODE_PRBS_31;
        break;
    case UTIL_PATT_MODE_PRBS_9:
        *s16_mode = S16_PATT_MODE_PRBS_9;
        break; 
    case UTIL_PATT_MODE_CJAT:
        *s16_mode = S16_PATT_MODE_CJAT;
        break;   ;
    case UTIL_PATT_MODE_USER_DEF:
        *s16_mode = S16_PATT_MODE_USER_DEF;
        break;
    default:
        PMC_RETURN(S16_LOG_ERR_MODE_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
        
}/*util_patt_mode_to_s16_mode_conv*/

/*******************************************************************************
* s16_t_piso_ctrl_val_get 
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function is called by pattern generator function to map S16 port mode
*    and returns T_PISO_CTRL value associated to mapped mode.
*
* INPUTS:
*   port_mode              - S16 port mode value.
*
* OUTPUTS:
*  *t_piso_ctrl            - T_PIS_CTRL value corresponding to s16 port mode.
*
* RETURNS:
*   PMC_SUCCESS            - when piso_ctrl is found, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_t_piso_ctrl_val_get(s16_port_mode_t port_mode,
                                          UINT8 *t_piso_ctrl)
{
    piso_lookup_tbl_elem_t s16_piso_lookup_tbl[S16_T_PISO_LOOKUP_TBL_SIZE_MAX] = S16_PISO_LOOKUP_TBL_ENTRIES;
    UINT32 i;
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != t_piso_ctrl, S16_LOG_ERR_INVALID_PARAMETERS, 0 ,0);

    /* lookup table PISO value  */
    for (i = 0; i < S16_T_PISO_LOOKUP_TBL_SIZE_MAX; i++) 
    {
        if (s16_piso_lookup_tbl[i].port_mode == port_mode)
        {
            *t_piso_ctrl = s16_piso_lookup_tbl[i].piso_val;
            PMC_RETURN(PMC_SUCCESS);
        }
    }

    PMC_RETURN(S16_LOG_ERR_INVALID_PARAMETERS);
}/* s16_t_piso_ctrl_val_get  */


/*******************************************************************************
* s16_acb_ft_loopback_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configure the ACB configuration structure for loopback  mode.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*   *cfg_ptr        - A pointer to a s16 loopback configuration
*
* OUTPUTS:
*  *acb_ft_cfg_ptr  - an ACB_FT configuration for facility loopback
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void s16_acb_ft_loopback_cfg_get(s16_handle_t *handle,
                                         UINT8 serdes_id,
                                         s16_loopback_cfg_t *cfg_ptr,
                                         acb_ft_prov_cfg_t *acb_ft_cfg_ptr)
{
    UINT32 bus_width;
    UINT64 csu_n;
    UINT32 csu_r;

    PMC_ENTRY();

    
    acb_ft_cfg_ptr->mode             = ACB_FT_MODE_LOOP_TIMING;      
    acb_ft_cfg_ptr->packet_per_int_C = S16_PMM_PACKET_PERIOD;
    acb_ft_cfg_ptr->packet_per_n_C   = 0;
    acb_ft_cfg_ptr->packet_per_d_C   = 1;
    acb_ft_cfg_ptr->packet_size_B    = 0;
    acb_ft_cfg_ptr->packet_per_int_B = 0;
    acb_ft_cfg_ptr->packet_per_n_B   = 0;
    acb_ft_cfg_ptr->packet_per_d_B   = 0;
    
    acb_ft_cfg_ptr->capture_rate  = cfg_ptr->serdes_rate;
    acb_ft_cfg_ptr->transmit_rate = cfg_ptr->serdes_rate;
    acb_ft_cfg_ptr->server_rate   = cfg_ptr->serdes_rate;
    acb_ft_cfg_ptr->client_type   = cfg_ptr->is_otu_rate == TRUE?ACB_FT_OTN:ACB_FT_DEMAPPED;
    acb_ft_cfg_ptr->num_lane      = 1;
    acb_ft_cfg_ptr->ref_clk       = UTIL_GLOBAL_REF_FREQ/2;
    switch (cfg_ptr->serdes_if)
    {
    case ACB_FT_SERDES_20b:
        bus_width = 20;
        break;
    case ACB_FT_SERDES_8b:
        bus_width = 8;
        break;
    default:
        bus_width = 32;
        break;
    }

    /* compute packet size */
    acb_ft_cfg_ptr->packet_size_C    = (UINT32) PMC_ROUND(((DOUBLE) acb_ft_cfg_ptr->packet_per_int_C * (acb_ft_cfg_ptr->server_rate / bus_width))/ ( acb_ft_cfg_ptr->ref_clk*2));
    
    /* get CSU configuration */
    s16_csu_div_val_get(handle,serdes_id,&acb_ft_cfg_ptr->csu_div, &csu_n, &acb_ft_cfg_ptr->csu_denomin, &csu_r);
    
    /* we don't care about the next fields, we just set a value */
    acb_ft_cfg_ptr->serdes_if     = ACB_FT_SERDES_32b;
    
    PMC_RETURN();
}/* s16_acb_ft_loopback_cfg_get */

/*******************************************************************************
*  s16_acb_ft_loop_timing_compute_pkt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*    The function compute PMM packet period and size 
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  *acb_ft_cfg_ptr  - an ACB_FT configuration for Facility loopback
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void s16_acb_ft_loop_timing_compute_pkt_cfg(s16_handle_t *handle,
                                                    UINT8 serdes_id,
                                                    acb_ft_prov_cfg_t *acb_ft_cfg_ptr)
{
    UINT32 bus_width;

    PMC_ENTRY();

    /* compute packet period */
    acb_ft_cfg_ptr->packet_per_int_C = S16_PMM_PACKET_PERIOD;
    acb_ft_cfg_ptr->packet_per_n_C   = 0;
    acb_ft_cfg_ptr->packet_per_d_C   = 1;
    acb_ft_cfg_ptr->packet_size_B    = 0;
    acb_ft_cfg_ptr->packet_per_int_B = 0;
    acb_ft_cfg_ptr->packet_per_n_B   = 0;
    acb_ft_cfg_ptr->packet_per_d_B   = 0;

    switch (acb_ft_cfg_ptr->serdes_if)
    {
    case ACB_FT_SERDES_20b:
        bus_width = 20;
        break;
    case ACB_FT_SERDES_8b:
        bus_width = 8;
        break;
    default:
        bus_width = 32;
        break;
    }

    /* compute packet size */
    acb_ft_cfg_ptr->packet_size_C    = (UINT32) PMC_ROUND(((DOUBLE) acb_ft_cfg_ptr->packet_per_int_C * (acb_ft_cfg_ptr->transmit_rate / bus_width))/ ( acb_ft_cfg_ptr->ref_clk*2));
    
    PMC_RETURN();
}/* s16_acb_ft_loop_timing_compute_pkt_cfg */

/*******************************************************************************
* s16_tx_ctxt_push
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function reads device registers to save context variables.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_tx_ctxt_push(s16_handle_t *handle, 
                                   UINT8 serdes_id)
{
    
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    } 

    if (FALSE != handle->var[serdes_id].s16_ctxt.active)
    {
        PMC_RETURN(S16_CONTEXT_ALREADY_STORED);
    }

    handle->var[serdes_id].s16_ctxt.t_piso_ctrl = s16_mtsb_ctrl_field_T_PISO_CTRL_get(NULL, handle, serdes_id);
    handle->var[serdes_id].s16_ctxt.tx_source = s16_channel_pcbi_field_TX_SOURCE_get(NULL, handle, serdes_id);
    handle->var[serdes_id].s16_ctxt.mtsb_bus_width = s16_channel_pcbi_field_MTSB_BUS_WIDTH_get(NULL, handle, serdes_id);
  
    handle->var[serdes_id].s16_ctxt.active = TRUE;

    PMC_RETURN(PMC_SUCCESS);
}/* s16_tx_ctxt_push */


/*******************************************************************************
* s16_tx_ctxt_pop
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This function restores TX registers value saved before pattern generator or 
*   loopback provisioning.
*
* INPUTS:
*   *handle         - pointer to s16 handle.
*   serdes_id       - serdes index.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
* 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR s16_tx_ctxt_pop(s16_handle_t *handle, 
                                  UINT8 serdes_id)
{
    PMC_ENTRY();
    
    /* check parameters */
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE, 0, 0);
    
    /* check serdes ID */ 
    if (handle->cfg.num_serdes_instance <= serdes_id)
    {
        PMC_RETURN(S16_LOG_ERR_SERDES_ID_INVALID);
    } 

    if (FALSE == handle->var[serdes_id].s16_ctxt.active)
    {
        PMC_RETURN(S16_LOG_ERR_NULL_CTXT);
    }

    s16_mtsb_ctrl_field_T_PISO_CTRL_set(NULL, handle, serdes_id, 
                                        handle->var[serdes_id].s16_ctxt.t_piso_ctrl);
    s16_channel_pcbi_field_TX_SOURCE_set(NULL, handle, serdes_id, 
                                         handle->var[serdes_id].s16_ctxt.tx_source);
    s16_channel_pcbi_field_MTSB_BUS_WIDTH_set(NULL, handle, serdes_id, 
                                              handle->var[serdes_id].s16_ctxt.mtsb_bus_width);
  
    handle->var[serdes_id].s16_ctxt.active = FALSE;

    PMC_RETURN(PMC_SUCCESS);
}/* s16_tx_ctxt_pop */

/*******************************************************************************
* s16_polarity_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configure a S16 instance to enable/disable lane polarity inversion
*
* INPUTS:
*   *handle         - pointer to S16 handle.
*   serdes_id       - serdes identifier.
*   dir             - serdes logical lane direction.
*   enable          - TRUE to enable polarity inversion, FALSE to disable it
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void s16_polarity_cfg(s16_handle_t             *handle,
                             UINT8                     serdes_id,
                             util_global_direction_t   dir,
                             BOOL8                     enable)
{
    BOOL cfg_rx = FALSE;
    BOOL cfg_tx = FALSE;
    s16_channel_pcbi_buffer_t b[1];
    
    PMC_ENTRY();
       
    s16_channel_pcbi_buffer_init(b,handle,serdes_id);


    switch (dir)
    {
    case UTIL_GLOBAL_DIR_TX:
        cfg_tx = TRUE;    
        break;
    case UTIL_GLOBAL_DIR_RX:
        cfg_rx = TRUE;
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        cfg_rx = TRUE;
        cfg_tx = TRUE;
        break;
    default:
        break;
    }
                            
    if (TRUE == cfg_tx)
    {
        s16_channel_pcbi_field_TX_LANE_INVERT_set(b,handle,serdes_id, TRUE == enable ? 0x1 : 0x0);
    }

    if (TRUE == cfg_rx)
    {
        s16_channel_pcbi_field_RX_LANE_INVERT_set(b,handle,serdes_id, TRUE == enable ? 0x1 : 0x0);
    }

    s16_channel_pcbi_buffer_flush(b);
    
    /* call polarity new value */
    handle->var[serdes_id].s16_ctxt.polarity_inv = util_global_direction_value_compute(handle->var[serdes_id].s16_ctxt.polarity_inv, dir, enable);

    PMC_RETURN();    
} /* s16_polarity_cfg */


/*******************************************************************************
* s16_polarity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Returns serdes polarity.
*
* INPUTS:
*   *handle         - pointer to S16 handle.
*   serdes_id       - serdes identifier.
*   dir             - serdes logical lane direction.
*
* OUTPUTS:
*   *polarity_inv   - TRUE when polarity is inverted, otherwise FALSE.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_polarity_get(s16_handle_t            *handle,
                                  UINT8                    serdes_id,
                                  util_global_direction_t  dir,
                                  BOOL8                   *polarity_inv)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
       
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE,0,0);
    PMC_ASSERT(serdes_id < handle->cfg.num_serdes_instance, S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(NULL != polarity_inv, S16_LOG_ERR_INVALID_PARAMETERS,0,0);           

    switch (handle->var[serdes_id].s16_ctxt.polarity_inv & dir)
    {
    case UTIL_GLOBAL_DIR_TX:
        *polarity_inv = TRUE;    
        break;
    case UTIL_GLOBAL_DIR_RX:
        *polarity_inv = TRUE;
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        *polarity_inv = TRUE;
        break;
    default:
        *polarity_inv = FALSE;
        break;
    }
        
    PMC_RETURN(rc);
} /* s16_polarity_get */

/*******************************************************************************
* s16_tx_refclk_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures a S16 instance to enable/disable TX reference clock. 
*   
*
* INPUTS:
*   *handle               - pointer to S16 handle.
*   serdes_id             - serdes identifier.
*   div16_enable          - TRUE to enable DIV16 clock divider, 
*                           otherwise DIV64 clock is configured
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void s16_tx_refclk_cfg(s16_handle_t *handle,
                              UINT8         serdes_id,
                              BOOL8         div16_enable)
{
    PMC_ENTRY();    

    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE,0,0);
    PMC_ASSERT(s16_serdes_id_validate(handle, serdes_id), S16_LOG_ERR_INVALID_PARAMETERS,0,0);
    
    s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set(NULL, handle, serdes_id, 
                                               ~((UINT8)div16_enable) & 0x1);
                                            
    PMC_RETURN();
 
} /* s16_tx_refclk_cfg */

/*******************************************************************************
* s16_tx_swing_emphasis_levels_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configure S16 tx swing and de-emphasis levels.
*
* INPUTS:
*   *handle          - pointer to S16 handle.
*   serdes_id        - serdes identifier.
*   levels_cfg_ptr   - pointer to TX swing and de-emphasis levels to
*                      set in device
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_tx_swing_emphasis_levels_set(s16_handle_t           *handle,
                                                  UINT8                   serdes_id,
                                                  s16_tx_swing_levels_t  *levels_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    s16_mtsb_ctrl_buffer_t b[1];
    
    PMC_ENTRY();
    
    PMC_ASSERT(handle != NULL, S16_LOG_ERR_NULL_HANDLE,0,0);
    PMC_ASSERT(levels_cfg_ptr != NULL, S16_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (FALSE == s16_serdes_id_validate(handle, serdes_id))
    {
        rc = S16_LOG_ERR_SERDES_ID_INVALID;
    }
    else 
    {
        s16_mtsb_ctrl_buffer_init(b,handle,serdes_id);
        
        s16_mtsb_ctrl_field_AMPLITUDE_set(b, handle, serdes_id, levels_cfg_ptr->amplitude);
        s16_mtsb_ctrl_field_POSTCURSOR_set(b, handle, serdes_id, levels_cfg_ptr->post_cursor);
        s16_mtsb_ctrl_field_PRECURSOR_set(b, handle, serdes_id, levels_cfg_ptr->pre_cursor);
        s16_mtsb_ctrl_field_T_PISO_PRE2_SEL_set(b, handle, serdes_id, 1);

        s16_mtsb_ctrl_buffer_flush(b);    
    }

    PMC_RETURN(rc);    
} /* s16_tx_swing_emphasis_levels_set */
 
/*******************************************************************************
* s16_dc_coupling_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configure a S16 serdes in DC coupling mode or put it back in in AC coupling 
*  mode.
*
* INPUTS:
*   *handle              - pointer to S16 handle.
*   serdes_id            - serdes identifier.
*   dc_coupling_enabled  - when TRUE, DC coupling is enabled otherwise AC 
*                          coupling is enabled
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_dc_coupling_set(s16_handle_t   *handle,
                                     UINT8           serdes_id,
                                     BOOL8           dc_coupling_enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();

    PMC_ASSERT(NULL != handle, S16_LOG_ERR_NULL_HANDLE,0,0);
       
    if (FALSE == s16_serdes_id_validate(handle, serdes_id))
    {
        rc = S16_LOG_ERR_SERDES_ID_INVALID;
    }
    else 
    {

        if (TRUE == dc_coupling_enabled)
        {
            s16_mtsb_ctrl_field_RX_CM_TERM_set(NULL, handle, serdes_id, 0x16);
            s16_channel_pcbi_field_TRS_CTRL_0_set(NULL, handle, serdes_id, 0x1);
        }
        else
        {
            s16_mtsb_ctrl_field_RX_CM_TERM_set(NULL, handle, serdes_id, 0x16);
            s16_mtsb_ctrl_field_RX_CM_TERM_SEL_set(NULL, handle, serdes_id, 0x1);
            s16_channel_pcbi_field_TRS_CTRL_0_set(NULL, handle, serdes_id, 0);
        }
    }

    PMC_RETURN(rc);
} /* s16_dc_coupling_set */

/*******************************************************************************
* s16_serdes_id_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Validates that given serdes id is in range of values.
*
* INPUTS:
*   *handle          - pointer to S16 handle.
*   serdes_id        - serdes identifier.
*
* OUTPUTS:
*  None.
*
* RETURNS:
*   TRUE when serdes id is in range of values otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 s16_serdes_id_validate(s16_handle_t *handle,
                                     UINT8 serdes_id)
{
    BOOL8 rc;
    PMC_ENTRY();

    rc = handle->cfg.num_serdes_instance <= serdes_id ? FALSE : TRUE;

    PMC_RETURN(rc);
}/*serdes_id_validate*/


/*******************************************************************************
* FUNCTION: s16_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   s16_handle        - Pointer to subsystem handle
*   serdes_id         - Serdes identifier
*   non_null_ptr      - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_int_validate(s16_handle_t    *s16_handle,
                                  UINT32           serdes_id,
                                  void            *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != s16_handle, S16_LOG_ERR_NULL_HANDLE, 0, 0);
   
    if (serdes_id >= s16_handle->cfg.num_serdes_instance)
    {
        result = S16_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = S16_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* s16_int_validate */


/*******************************************************************************
* FUNCTION: s16_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel 
*   interrupt table.
*   
*
* INPUTS:
*   s16_handle        - Pointer to subsystem handle
*   serdes_id         - Serdes identifier
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
PUBLIC PMC_ERROR s16_int_chnl_enable(s16_handle_t    *s16_handle,
                                     UINT32           serdes_id,
                                     s16_int_pin_t   *int_table_ptr,
                                     s16_int_pin_t   *int_en_table_ptr,
                                     BOOL             enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = serdes_id;
    acb_ft_handle_t *acb_ft_handle;

    PMC_ENTRY();

    if (S16_PORT_RESET == s16_handle->var[serdes_id].state)
    {
        PMC_RETURN(result);
    }

    result = s16_int_validate(s16_handle, serdes_id, int_en_table_ptr);

    acb_ft_handle = s16_handle->acb_ft_handle[link];

    #ifdef S16_INT
    #undef S16_INT
    #endif
    #define S16_INT    UTIL_GLOBAL_INT_ENABLE_SINGLE
    

    S16_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* s16_int_chnl_enable */




/*******************************************************************************
* FUNCTION: s16_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   s16_handle       - Pointer to subsystem handle
*   serdes_id              - Serdes identifier
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
PUBLIC PMC_ERROR s16_int_chnl_clear(s16_handle_t   *s16_handle,
                                    UINT32          serdes_id,
                                    s16_int_pin_t  *int_table_ptr,
                                    s16_int_pin_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = serdes_id;
    acb_ft_handle_t *acb_ft_handle;

    PMC_ENTRY();

    if (S16_PORT_RESET == s16_handle->var[serdes_id].state)
    {
        PMC_RETURN(result);
    }

    result = s16_int_validate(s16_handle, serdes_id, int_en_table_ptr);

    acb_ft_handle = s16_handle->acb_ft_handle[link];

    #ifdef S16_INT
    #undef S16_INT
    #endif
    #define S16_INT UTIL_GLOBAL_INT_CLEAR_SINGLE
    

    S16_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* s16_int_chnl_clear */


/*******************************************************************************
* FUNCTION: s16_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   s16_handle        - Pointer to subsystem handle
*   serdes_id         - Serdes identifier
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within 
*                       this table to indicate that the interrupt/status will 
*                       be retrieved to the output table. NULL to retrieve all 
*                       interrupt/status.
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
PUBLIC PMC_ERROR s16_int_chnl_retrieve(s16_handle_t   *s16_handle,
                                       UINT32          serdes_id,
                                       s16_int_pin_t  *filt_table_ptr,
                                       s16_int_pin_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = serdes_id;
    acb_ft_handle_t *acb_ft_handle;

    PMC_ENTRY();

    if (S16_PORT_RESET == s16_handle->var[serdes_id].state)
    {
        PMC_RETURN(result);
    }

    if (serdes_id >= s16_handle->cfg.num_serdes_instance)
    {
        result = S16_LOG_ERR_INVALID_PARAMETERS;
    }

    acb_ft_handle = s16_handle->acb_ft_handle[link];

    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(acb_ft_handle, acb_ft, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef S16_INT
    #undef S16_INT
    #endif
    #define S16_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    

    S16_INT_TABLE_DEFINE();
    
    
    #ifdef S16_STATUS
    #undef S16_STATUS
    #endif
    #define S16_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    

    S16_STATUS_TABLE_DEFINE();
    
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(acb_ft_handle, acb_ft);


    PMC_RETURN(result);
} /* s16_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: s16_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   s16_handle       - Pointer to subsystem handle
*   serdes_id              - Serdes identifier
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR s16_int_chnl_enabled_check(s16_handle_t     *s16_handle,
                                             UINT32           serdes_id,
                                             s16_int_pin_t   *int_en_table_ptr,
                                             BOOL            *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link = serdes_id;
    acb_ft_handle_t *acb_ft_handle;

    PMC_ENTRY();

    if (S16_PORT_RESET == s16_handle->var[serdes_id].state)
    {
        PMC_RETURN(result);
    }

    result = s16_int_validate(s16_handle, serdes_id, int_found_ptr);

    acb_ft_handle = s16_handle->acb_ft_handle[link];


    #ifdef S16_INT
    #undef S16_INT
    #endif
    #define S16_INT UTIL_GLOBAL_INT_CHECK_SINGLE
    

    S16_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* s16_int_chnl_enabled_check */

/*******************************************************************************
* FUNCTION: s16_vert_hist_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves s16 vertical histogram information.
*   
*
* INPUTS:
*   *s16_handle     - Pointer to S16 handle.
*   lane_0_19_msk   - Bitmask of lanes 0-19 to configure. 
*                     Bit 0 corresponds to lane 0.
*   test_length     - Enum,requested the number of samples to capture
*                     UTIL_VHIST_DEBUG
*                     UTIL_VHIST_SHORT
*                     UTIL_VHIST_LONG
*                     UTIL_VHIST_USERDEF
*   userdef_length  - The vertical histgram counter-timer limit value 
*                     that is defined by user.
*                     This parameter is only valid if test_lenth is set to 
*                     UTIL_VHIST_USERDEF.
*
* OUTPUTS:
*   bin_buffer      - Pointer to vertical histogram value
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR  s16_vert_hist_get(s16_handle_t     *s16_handle,
                                    UINT32            lane_0_19_msk,
                                    util_vhist_test_length_t  test_length,
                                    UINT64            userdef_length,
                                    UINT64            bin_buffer[][64]) 
{

    UINT32 i = 0, itr = 0;
    UINT32 j = 0;
    UINT32 capture_running = 0;
    UINT32 captCount = 0;
    UINT32 index = 0;
    UINT32 readback_addr;
    UINT32 eye_index = 0; 
    UINT64 value_31_0 = 0;
    UINT64 value_39_32 = 0;
    UINT32 max_serdes = 0;
    UINT32 serdes_id = 0;
    UINT32 msk = 0;
    UINT32 cnt_tim_limit_31_0 = 0, cnt_tim_limit_39_32 = 0;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != s16_handle), S16_LOG_ERR_NULL_HANDLE, 0, 0);

    switch(test_length) 
    {
    case UTIL_VHIST_LONG:
        cnt_tim_limit_31_0 = 0xFFFFFFFF;
        cnt_tim_limit_39_32 = 0x0F;
        break;
    case UTIL_VHIST_SHORT:
        cnt_tim_limit_31_0 = 0xFFFFFFFF;
        cnt_tim_limit_39_32 = 0x0;
        break;
    case UTIL_VHIST_USERDEF:
        cnt_tim_limit_31_0 = (UINT32)(userdef_length & 0xFFFFFFFF);
        cnt_tim_limit_39_32 = (UINT32)((userdef_length >> 32) & 0xFF);
        break;
    case UTIL_VHIST_DEBUG:
    default:
        cnt_tim_limit_31_0 = 0x0FFFFFFF;
        cnt_tim_limit_39_32 = 0x0;
        break;
    }

    max_serdes = (s16_handle->cfg.s16_type == S16_LINE) ? S16_LINE_NUM_INSTANCE: S16_SYSTEM_NUM_INSTANCE;
    for(itr = 0; itr < max_serdes; itr++)
    {
        msk = 0x1 << itr;
        if (0 != (lane_0_19_msk & msk))
        {
            serdes_id = itr;
            /* Set VHIST_CNT_TIM_LIMIT_31_0 to 0xFFFFFFFF */
            s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_31_0_set(NULL, s16_handle, serdes_id, cnt_tim_limit_31_0);

            /* Set VHIST_CNT_TIM_LIMIT_39_32 to 0 */
            s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_39_32_set(NULL, s16_handle, serdes_id, cnt_tim_limit_39_32);

            /* Set VHIST_PERBIN_VALUE_RD_ENB to 1*/
            s16_channel_pcbi_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, s16_handle, serdes_id, 0x1);
            
            /* Set VHIST_CAPT_ENB to 1 */
            s16_channel_pcbi_field_VHIST_CAPT_ENB_set(NULL, s16_handle, serdes_id, 0x1);

            /* Set VHIST_TITAN_TARGET_AMP_WR_EN to 0b1 */
            s16_channel_pcbi_field_VHIST_TITAN_TARGET_AMP_WR_EN_set(NULL, s16_handle, serdes_id, 0x1);

            /* Set VHIST_CAPT_START to 0 */
            s16_channel_pcbi_field_VHIST_CAPT_START_set(NULL, s16_handle, serdes_id, 0x0);

            /* Set VHIST_TITAN_TARGET_AMP_WR_EN to 0 */
            s16_channel_pcbi_field_VHIST_TITAN_TARGET_AMP_WR_EN_set(NULL, s16_handle, serdes_id, 0x0);

            /* Set VHIST_CAPT_ENB to 0 */
            s16_channel_pcbi_field_VHIST_CAPT_ENB_set(NULL, s16_handle, serdes_id, 0x0);

            /* Set VHIST_CAPT_START to 1 */
            s16_channel_pcbi_field_VHIST_CAPT_START_set(NULL, s16_handle, serdes_id, 0x1);
        }
    }
    
    index  = 0;
    for (i = 0; i < 8; i++)
    {
        
        for(itr = 0; itr < max_serdes; itr++)
        {

            msk = 0x1 << itr;
            
            if (0 != (lane_0_19_msk & msk))
            {
                serdes_id = itr;
                /* Wait for captures to complete */
                captCount = 0;
                do
                {
                    /* Read VHIST_CAPT_RUNNING bit */
                    capture_running = s16_channel_pcbi_field_VHIST_CAPT_RUNNING_get(NULL, s16_handle, serdes_id);
                    captCount++;
                    PMC_ATOMIC_YIELD(s16_handle->base.parent_handle, 10000);

                } 
                while (capture_running & 0x1);
            }
        }
        
        for (j = 0; j < 8; j++)
        {

            for(itr = 0; itr < max_serdes; itr++)
            {
                msk = 0x1 << itr;
                if (0 != (lane_0_19_msk & msk))
                {
                    serdes_id = itr;
                    /* Read VHIST_PERBIN_VALUE_31_0 */
                    value_31_0  = s16_channel_pcbi_field_VHIST_PERBIN_VALUE_31_0_get(NULL, s16_handle, serdes_id);

                    /* Read VHIST_PERBIN_VALUE_39_32 */
                    value_39_32  = s16_channel_pcbi_field_VHIST_PERBIN_VALUE_39_32_get(NULL, s16_handle, serdes_id);

                    /* Read VHIST_READBACK_ADDR */
                    readback_addr  = s16_channel_pcbi_field_VHIST_READBACK_ADDR_get(NULL, s16_handle, serdes_id);
                    /* Check the bin address */
                    if (readback_addr != index)
                    {
                         PMC_LOG_TRACE("ERROR: read back address not equal to bin number for bin: %d read back addr: %d\n", index ,readback_addr);
                    }


                    if  (index < 32) 
                    { 
                        eye_index = 32 + index; 
                    } 
                    else 
                    { 
                        eye_index = index - 32; 
                    } 
                                                                    
                    PMC_LOG_TRACE("DEBUG:  index: %d,  capture polls: %d,  value: 0x%llx\n", eye_index, captCount, value_31_0 );
                    bin_buffer[itr][eye_index] = value_31_0 + ((value_39_32 & 0xff) << 32);  

                    /* Set VHIST_PERBIN_VALUE_RD_ENB to 0*/
                    s16_channel_pcbi_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, s16_handle, serdes_id, 0x0);

                    /* Set VHIST_PERBIN_VALUE_RD_ENB to 1*/
                    s16_channel_pcbi_field_VHIST_PERBIN_VALUE_RD_ENB_set(NULL, s16_handle, serdes_id, 0x1);

                }
            }
            index++;
        }
    }

    for(itr = 0; itr < max_serdes; itr++)
    {
        msk = 0x1 << itr;
        if (0 != (lane_0_19_msk & msk))
        {
            serdes_id = itr;
            
            /* Set VHIST_CAPT_START to 0 */
            s16_channel_pcbi_field_VHIST_CAPT_START_set(NULL, s16_handle, serdes_id, 0x0);

            /* Set VHIST_CAPT_ENB to 1 */
            s16_channel_pcbi_field_VHIST_CAPT_ENB_set(NULL, s16_handle, serdes_id, 0x1);
        }
    }
    
    PMC_RETURN(rc);
} /* s16_vert_hist_get */

/*******************************************************************************
* s16_serdes_if_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the serdes interface type associated to a given lane.
*
* INPUTS:
*   *s16_handle           - pointer to S16 handle instance.
*   serdes_id             - serdes index.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   serdes_if             - serdes interface type: 20b, 32b, etc.
*
* NOTES:
*
*******************************************************************************/
PRIVATE acb_ft_serdes_if_t s16_serdes_if_get(s16_handle_t        *s16_handle, 
                                             UINT8                serdes_id) 
{
    acb_ft_serdes_if_t serdes_if;
    UINT32 lane_bus_width;

    PMC_ENTRY();


    lane_bus_width = s16_channel_pcbi_field_MTSB_BUS_WIDTH_get(NULL, s16_handle, serdes_id);

    switch (lane_bus_width) 
    {
    case 2 :
        serdes_if = ACB_FT_SERDES_32b;
        break;
    case 1:
        serdes_if = ACB_FT_SERDES_20b;
        break;
    case 0:
        serdes_if = ACB_FT_SERDES_8b;
        break;
    default:
        serdes_if = LAST_ACB_FT_SERDES_IF;
        break;
    }

    PMC_RETURN(serdes_if);
}/* s16_serdes_if_get */


/*
** End of file
*/



