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

#include "pmc_sys.h"
#include "util_global.h"
#include "otu_frm_loc.h"
#include "util_gen_db.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/* Maximum number of entries in the ODUKSC database */
#define OTU_FRM_ODUKSC_NUM_DB_ENTRIES_MAX   96

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
* The following creates the LOG/ERR strings from above by pulling out the text
* portion of the LOG_ERR_TABLE above
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char OTU_FRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    OTU_FRM_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char OTU_FRM_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */



/*
** Forward Declarations
*/
PRIVATE PMC_ERROR   otu_frm_dsm_set            (otu_frm_handle_t   *otu_frm_handle,
                                                UINT8               dsi_stream,
                                                otu_frm_stream_rate_t stream_rate );

PRIVATE PMC_ERROR   otu_frm_master_cfg_bits_set(otu_frm_handle_t         *otu_frm_handle,
                                                otu_frm_sw_pwr_cfg_bit_t sw_cfg_bit,
                                                otu_frm_bit_value_t      sw_cfg_set_value);

PRIVATE PMC_ERROR   otu_frm_sm_sk_glbl_cfg_set (otu_frm_handle_t   *otu_frm_handle,
                                                otu_frm_sm_sk_gc_t gc_field,
                                                otu_frm_bit_value_t value);

PRIVATE void        otu_frm_reset_ram_regs     (otu_frm_handle_t   *otu_frm_handle);

PRIVATE otu_frm_stream_rate_t   otu_frm_oduk_rate_get      (util_global_odukp_type_t oduk_type);

PRIVATE BOOL                    otu_frm_validate_rx_prov   (otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8                 dsi_stream);

PRIVATE BOOL                    otu_frm_validate_tx_prov   (otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8 dsi_stream);

PRIVATE void                    otu_frm_set_rx_stream_state(otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8                 dsi_stream,
                                                            otu_frm_str_state_t   new_state);

PRIVATE void                    otu_frm_set_tx_stream_state(otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8                 dsi_stream,
                                                            otu_frm_str_state_t    new_state);

PRIVATE otu_frm_str_state_t     otu_frm_get_rx_stream_state(otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8                 dsi_stream);

PRIVATE otu_frm_str_state_t     otu_frm_get_tx_stream_state(otu_frm_handle_t      *otu_frm_handle,
                                                            otu_frm_stream_rate_t otu_rate,
                                                            UINT8                 dsi_stream);

PRIVATE UINT8                   otu_frm_get_free_dci_ch_id (otu_frm_handle_t       *otu_frm_handle,
                                                            UINT8                  dsi_stream,
                                                            otu_frm_stream_rate_t  dsi_rate);

PRIVATE UINT8                   otu_frm_find_dci_ch_id     (otu_frm_handle_t       *otu_frm_handle,
                                                            UINT8                  dsi_stream,
                                                            otu_frm_stream_rate_t  dsi_rate);

PRIVATE BOOL                    otu_frm_validate_dsi_stream(UINT8                 dsi_stream,
                                                            otu_frm_stream_rate_t otu_rate);

PRIVATE UINT8                   otu_frm_get_base_10G_dsi_stream(UINT8                 dsi_stream,
                                                                otu_frm_stream_rate_t otu_rate);

PRIVATE PMC_ERROR               otu_frm_db_prov_update(otu_frm_handle_t *otu_frm_handle,
                                                       UINT8             dci_channel,
                                                       BOOL              provision_db);


PRIVATE void otu_frm_stream_fec_degap_reg_cleanup (otu_frm_handle_t     *otu_frm_handle,
                                                   UINT8                 dsi_stream,
                                                   otu_frm_stream_rate_t lane_rate);

PRIVATE void otu_frm_stream_cfg_reg_cleanup(otu_frm_handle_t           *otu_frm_handle,
                                            UINT32                      dsi_stream,
                                            otu_frm_stream_rate_t       stream_rate);
/*
** Public Functions
*/


/*******************************************************************************
* OTU FRM API functions
*******************************************************************************/


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/


/*******************************************************************************
* otu_frm_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an OTU_FRM block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent, then pass in NULL
*   base_address          - base address of the OTU_FRM subsystem relative to
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*
*   *tsb_name             - pointer to string, contains blk/ss name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   otu_frm_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC otu_frm_handle_t *otu_frm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    otu_frm_handle_t *otu_frm_handle;

    PMC_ENTRY();
    otu_frm_handle = (otu_frm_handle_t*)PMC_CTXT_CALLOC(sizeof(otu_frm_handle_t), parent);

    pmc_handle_init(parent, otu_frm_handle, sys_handle, PMC_MID_DIGI_OTU_FRM, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(OTU_FRM_LOG_ERR_STRINGS[0], OTU_FRM_LOG_ERR_TABLE_BASE, OTU_FRM_LOG_ERR_COUNT);

    /* Init children's handles */
    otu_frm_handle->oduksc_handle = oduksc_ctxt_create((pmc_handle_t *)otu_frm_handle, base_address+BASE_ADDR_PM55_82_20_OTU_FRM_ODUKSC_ODUKSC_CORE, sys_handle, "ODUKSC");
    PMC_CTXT_REGISTER_PTR(&otu_frm_handle->oduksc_handle, parent);

    PMC_RETURN(otu_frm_handle);
} /* otu_frm_ctxt_create */


/*******************************************************************************
* otu_frm_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an OTU_FRM block instance.
*
* INPUTS:
*   otu_frm_handle         - pointer to OTU_FRM handle instance
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
PUBLIC void otu_frm_ctxt_destroy(otu_frm_handle_t *otu_frm_handle)
{
    PMC_ENTRY();
    /* detach from db */
    gen_db_detach(&(otu_frm_handle->base),UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,otu_frm_handle->var.lineotn_db_handle_rx);
    gen_db_detach(&(otu_frm_handle->base),UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,otu_frm_handle->var.lineotn_db_handle_tx);

    gen_db_detach(&(otu_frm_handle->base), UTIL_GEN_DB_LINEOTN_SYSOTN_MODE, otu_frm_handle->var.lineotn_db_handle_sysotn_mode);
    
    /* Free Children's handles */
    oduksc_ctxt_destroy(otu_frm_handle->oduksc_handle);
    /* free otu_frm memory */
    PMC_CTXT_FREE(&otu_frm_handle, otu_frm_handle);

    PMC_RETURN();

} /* otu_frm_ctxt_destroy */

/*******************************************************************************
* otu_frm_var_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OTU_FRM handle var parameters.
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
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
PUBLIC void otu_frm_var_init(otu_frm_handle_t *otu_frm_handle)
{
    UINT8 i;

    PMC_ENTRY();

    /* initialize power modes as if we are after reset */
    otu_frm_handle->var.otu_frm_pwr_cfg.sw_power_mode = SW_LOWPWR_LS;

    /* Initialize all logical variables */
    otu_frm_handle->var.otudg_sfi51_lanes_bm = 0; /* reset otudg bit mask */

    /* 10G streams */
    for(i=0;i<MAX_10G_STREAM_NUM;i++)
    {
        otu_frm_handle->var.otu_frm_10G_stream_cfg[i].stream_lane = FREE_STREAM;
        PMC_MEMSET(&otu_frm_handle->var.otu_frm_10G_stream_cfg[i].stream_dsc,0x00,
                   sizeof(otu_frm_handle->var.otu_frm_10G_stream_cfg[i].stream_dsc));

        /* Init the dsi streams state variables */
        otu_frm_set_rx_stream_state(otu_frm_handle,RATE_10G,i,STREAM_STATE_START);
        otu_frm_set_tx_stream_state(otu_frm_handle,RATE_10G,i,STREAM_STATE_START);

        /* set dsi oduk rates to DONT CARE */
        otu_frm_handle->var.dsi_oduk_type[i] = UTIL_GLOBAL_ODUK_DONT_CARE;

        /* set data smoothing to disabled */
        otu_frm_handle->var.data_smoothing_enabled[i]=FALSE;
    }

    /* 40G streams */
    for(i=0;i<MAX_40G_STREAM_NUM;i++)
    {
        otu_frm_handle->var.otu_frm_40G_stream_cfg[i].stream_lane = FREE_STREAM;
        PMC_MEMSET(&otu_frm_handle->var.otu_frm_40G_stream_cfg[i].stream_dsc,0x00,
                   sizeof(otu_frm_handle->var.otu_frm_40G_stream_cfg[i].stream_dsc));

        /* Init the dsi streams state variables */
        otu_frm_set_rx_stream_state(otu_frm_handle,RATE_40G,i,STREAM_STATE_START);
        otu_frm_set_tx_stream_state(otu_frm_handle,RATE_40G,i,STREAM_STATE_START);
    }

    /* 100G streams */
    for(i=0;i<MAX_100G_STREAM_NUM;i++)
    {
        otu_frm_handle->var.otu_frm_100G_stream_cfg[i].stream_lane = FREE_STREAM;
        PMC_MEMSET(&otu_frm_handle->var.otu_frm_100G_stream_cfg[i].stream_dsc,0x00,
                   sizeof(otu_frm_handle->var.otu_frm_100G_stream_cfg[i].stream_dsc));

        /* Init the dsi streams state variables */
        otu_frm_set_rx_stream_state(otu_frm_handle,RATE_100G,i,STREAM_STATE_START);
        otu_frm_set_tx_stream_state(otu_frm_handle,RATE_100G,i,STREAM_STATE_START);
    }


    /* mark all dci channels as free */
    PMC_MEMSET(&otu_frm_handle->var.dci_to_dsi_chan_ids[0],FREE_STREAM,
               sizeof(otu_frm_handle->var.dci_to_dsi_chan_ids));

    /* CBR_INT variables */
    /* Initialize all channel descriptors */
    /* 10G Channels */
    for(i=0;i<MAX_10G_LANE_NUM;i++)
    {
        otu_frm_handle->var.cbr_int_channel_id_cfg[i].channel_lane = FREE_LANE;
        PMC_MEMSET(&otu_frm_handle->var.cbr_int_channel_id_cfg[i].channel_dsc,0x00,
                   sizeof(otu_frm_handle->var.cbr_int_channel_id_cfg[i].channel_dsc));

        otu_frm_handle->var.cbr_int_channel_id_cfg[i].channel_dsc.ch_id_lane = FREE_LANE;
    }


    /* clear multilane configuration */
    otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G = 0;
    otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G  = 0;
    otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G  = 0;

    /* CBR_DINT */
    /* Initialize all channel descriptors */

    /* 10G Channels */
    for(i=0;i<MAX_10G_LANE_NUM;i++)
    {
        otu_frm_handle->var.cbr_dint_channel_id_cfg[i].channel_lane = FREE_LANE;
        PMC_MEMSET(&otu_frm_handle->var.cbr_dint_channel_id_cfg[i].channel_dsc,0x00,
                   sizeof(otu_frm_handle->var.cbr_dint_channel_id_cfg[i].channel_dsc));

        otu_frm_handle->var.cbr_dint_channel_id_cfg[i].channel_dsc.ch_id_lane = FREE_LANE;
    }

    /* clear multilane configuration */
    otu_frm_handle->var.cbr_dint_multilane_cfg.multilane_100G = 0;
    otu_frm_handle->var.cbr_dint_multilane_cfg.multilane_40G  = 0;
    otu_frm_handle->var.cbr_dint_multilane_cfg.multilane_10G  = 0;
    PMC_RETURN();

}/* otu_frm_var_init */


/*******************************************************************************
* otu_frm_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OTU_FRM block handle instance.
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
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
PUBLIC void otu_frm_handle_init(otu_frm_handle_t *otu_frm_handle)
{
    PMC_ENTRY();

    /* First step: Set Constant and default values of this handle */
    /* Init OTU_FRM var and cfg values */

    otu_frm_var_init(otu_frm_handle);

    /*Context used to populate the HO database */
    otu_frm_handle->pop_ho_db = TRUE;

    /* Second step: init children handles           */
    /* ODUKSC blocks handle init */
    oduksc_handle_init(otu_frm_handle->oduksc_handle,ODUKSC_LINEOTN,0);

    /* now that all handles are init - init the OTU_FRM block and make it operational */    

    /* attach to line/core OTN db */

    otu_frm_handle->var.lineotn_db_handle_rx = gen_db_attach(&(otu_frm_handle->base),UTIL_GEN_DB_LINE_CORE_OTN_RX_DB);
    otu_frm_handle->var.lineotn_db_handle_tx = gen_db_attach(&(otu_frm_handle->base),UTIL_GEN_DB_LINE_CORE_OTN_TX_DB);

    otu_frm_handle->var.lineotn_db_handle_sysotn_mode = gen_db_attach(&(otu_frm_handle->base), UTIL_GEN_DB_LINEOTN_SYSOTN_MODE);

    PMC_RETURN();

} /* otu_frm_handle_init */


/*******************************************************************************
*  otu_frm_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  Allocate any local memory required (i.e. ODUKSC)
*
* INPUTS:
*   *otu_frm_handle             - pointer to otu_frm handle
*
* OUTPUTS:
*  NONE
*
* RETURNS:
*  NONE
*  
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_alloc_init(otu_frm_handle_t *otu_frm_handle)
{
    PMC_ENTRY();  

    oduksc_alloc_init(otu_frm_handle->oduksc_handle);

    PMC_RETURN();
} /* otu_frm_alloc_init */


/*******************************************************************************
*  otu_frm_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on OTU_FRM handle instance.
*
* INPUTS:
*   *otu_frm_handle             - pointer to otu_frm handle
*   *restart_init_cfg_ptr       - pointer to restart init config
*
* OUTPUTS:
*  NONE
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*  
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_handle_restart_init(otu_frm_handle_t *otu_frm_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();  

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(OTU_FRM_LOG_ERR_STRINGS[0], OTU_FRM_LOG_ERR_TABLE_BASE, OTU_FRM_LOG_ERR_COUNT);
    }

    result = oduksc_handle_restart_init(otu_frm_handle->oduksc_handle,restart_init_cfg_ptr);

    PMC_RETURN(result);
} /* otu_frm_handle_restart_init */




/*******************************************************************************
* otu_frm_blk_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OTU_FRM block with all its sub blocks as part of handle_init : \n
*       1. CBR_INT - channels disabled, ch id = 0x3f \n
*       2. CBR_DINT- channels disabled, ch id = 0x3f, set XOFF fifo threshold to 0x74 \n
*       3. OTU_FRM - Ram power active, all rx/system clocks power on/reset off \n
*       4. OTU_FRM_RX (SK) - put all channels at passthru,RAM R/W register set to 0 \n
*       5. OTU_FRM_TX (SO) - put all channels at passthru,RAM R/W register set to 0 \n
*       6. ODUKSC  - take out of reset and initialize.
*
*   This init takes the following actions: \n
*       1. Take out of low power state for SYS/355M clocks, RAMS \n
*       2. Rx line clocks out of reset but still in low power mode. \n
*       3. Remove all line loopbacks \n
*       4. Remove resets of different sub blocks \n
*       5. all channels should be in PASSTHRU mode \n
*       6. Disable dlom to sf for all rates and all streams \n
*       7. Initialize FEC degap block \n
*       8. RAM R/W register set to 0's 
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
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
PUBLIC void otu_frm_blk_init(otu_frm_handle_t *otu_frm_handle)
{
    UINT8 i;
    UINT8 start_lane,end_lane;
    int_dint_channel_cfg_dsc_t  init_channel_dscptr;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    /* Ram  and system  - take out of reset and low power mode */
    otu_frm_pwr_mode_config_set(otu_frm_handle,SW_ACTIVE);

    /* Clocks  held in reset until the end of configuration */
    otu_frm_master_cfg_bits_set(otu_frm_handle,SW_LOW_POWER,SET_BIT);     

    /* remove channels resets - CBR_INT/CBR_DINT channels */
    /* are taken out of reset as part of the handle init  */
    /* however they are still in a disabled mode at this  */
    /* point and will be enabled upon provisioning        */

    /* set all channels in passthru mode              */
    for(i=0;i<MAX_DCI_CHANNEL_NUM;i++)
    {
        otu_frm_passthru_set(otu_frm_handle,i,OTU_SK_PASSTHRU,SET_BIT);
        otu_frm_passthru_set(otu_frm_handle,i,OTU_SO_PASSTHRU,SET_BIT);

        /* force LCK both directions */
        otu_frm_sm_ctl_set(otu_frm_handle,i,OTU_SK_MAINT_SIG,2); /* 2-> LCK signal */
        otu_frm_sm_ctl_set(otu_frm_handle,i,OTU_SO_MAINT_SIG,2); /* 2-> LCK signal */  
    }

    /* CBR_INT hw init */
    /* take all channels out of reset,however, leave them in disabled mode */
    start_lane = 0;
    end_lane   = MAX_10G_LANE_NUM;

    /* disable cbr interleaver channels                              */
    PMC_MEMSET((void *)&init_channel_dscptr, 0, sizeof(int_dint_channel_cfg_dsc_t));
    init_channel_dscptr.channel_dsc.ch_id = INT_DINT_CAL_NULL_ID;
    init_channel_dscptr.channel_dsc.ch_enable = RESET_BIT;

    /* reseting 10G lanes */
    for(    ; start_lane < end_lane ; start_lane++ )
    {
        init_channel_dscptr.channel_lane = start_lane;
        cbr_int_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_10G);
        cbr_int_free_lanes(otu_frm_handle,start_lane,RATE_10G);
    }

    /* reseting 100G and 40G lanes */
    init_channel_dscptr.channel_lane = 0;
    cbr_int_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_100G);
    cbr_int_free_lanes(otu_frm_handle,0,RATE_100G);

    init_channel_dscptr.channel_lane = 0;
    cbr_int_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_int_free_lanes(otu_frm_handle,0,RATE_40G);

    init_channel_dscptr.channel_lane = 1;
    cbr_int_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_int_free_lanes(otu_frm_handle,1,RATE_40G);

    init_channel_dscptr.channel_lane = 2;
    cbr_int_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_int_free_lanes(otu_frm_handle,2,RATE_40G);

    /* Fec Degap init */
    otu_frm_fec_degap_init_set(otu_frm_handle,OTUDG_FIFO_FULL_THRS,OTUDG_FIFO_EMPTY_THRS);

    /* CBR_DINT hw init */
    /* take all channels out of reset,however, leave them in disabled mode */
    start_lane = 0;
    end_lane   = MAX_10G_LANE_NUM;

    /* disable cbr de-interleaver channels                              */
    init_channel_dscptr.channel_dsc.ch_id = INT_DINT_CAL_NULL_ID;
    init_channel_dscptr.channel_dsc.ch_enable = RESET_BIT;

    /* reseting 10G lanes */
    for(    ; start_lane < end_lane ; start_lane++ )
    {
        init_channel_dscptr.channel_lane = start_lane;
        cbr_dint_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_10G );
        cbr_dint_free_lanes(otu_frm_handle,start_lane,RATE_10G);
    }

    /* reseting 100G and 40G lanes */
    init_channel_dscptr.channel_lane = 0;
    cbr_dint_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_100G);
    cbr_dint_free_lanes(otu_frm_handle,0,RATE_100G);

    init_channel_dscptr.channel_lane = 0;
    cbr_dint_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_dint_free_lanes(otu_frm_handle,0,RATE_40G);

    init_channel_dscptr.channel_lane = 1;
    cbr_dint_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_dint_free_lanes(otu_frm_handle,1,RATE_40G);

    init_channel_dscptr.channel_lane = 2;
    cbr_dint_channel_dsc_set(otu_frm_handle, &init_channel_dscptr,RATE_40G);
    cbr_dint_free_lanes(otu_frm_handle,2,RATE_40G);

    /* take oduksc out of reset */
    oduksc_reset_cfg(otu_frm_handle->oduksc_handle, SET_BIT);
    oduksc_init(otu_frm_handle->oduksc_handle, PMC_ENERGY_STATE_RUNNING);
    oduksc_reset_cfg(otu_frm_handle->oduksc_handle, RESET_BIT);

    /* aSSF disabled for all streams */
    for( start_lane = 0 ; start_lane < end_lane ; start_lane++ )
    {
        otu_frm_dlom_to_sf_set(otu_frm_handle,RATE_10G,start_lane,(otu_frm_bit_value_t)0);
    }

    otu_frm_dlom_to_sf_set(otu_frm_handle,RATE_40G,0,(otu_frm_bit_value_t)0);
    otu_frm_dlom_to_sf_set(otu_frm_handle,RATE_40G,1,(otu_frm_bit_value_t)0);
    otu_frm_dlom_to_sf_set(otu_frm_handle,RATE_40G,2,(otu_frm_bit_value_t)0);

    otu_frm_dlom_to_sf_set(otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0);

    /* set all ram registers to 0 since they are not initialized by HW */
    otu_frm_reset_ram_regs(otu_frm_handle);

    /* rest of the registers are in their default values */
    /* Clocks out of reset, power on */
    /* moved the resets until the end of the configuration sequence */
    /* as they were triggering too many assertions when data flows  */
    /* through the unconfigured design                              */
    otu_frm_master_cfg_bits_set(otu_frm_handle,SW_LOW_POWER,RESET_BIT);     /* Clocks are working                   */

    /* Configure the following register to ensure SF stays asserted until the 
       FEC datapath is flushed after the SSF is de-asserted from the OTN_MLD.
       This is required as the SSF signal does not propagate through the FEC
       and thus does not account for the FEC decoder latency
       44348 -> 125 us / 2.8186 ns */
    otu_frm_field_OTU_FRM_SF_CNT_LIMIT_set(NULL, otu_frm_handle, 44348);

    PMC_RETURN();

} /* otu_frm_blk_init */


/*******************************************************************************
* OTU FRM Block functions
*******************************************************************************/

/*******************************************************************************
* otu_frm_reset_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the otu_frm into/out of reset, which includes the ODUKSC sub-block
*
* INPUTS:
*   *otu_frm_handle         - pointer to otu_frm handle instance.
*
*   reset_otu_frm           - boolean, requested reset mode:
*                             TRUE : put otu_frm block into reset mode; 
*                             FALSE : take otu_frm block out of reset mode
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_reset_config_set( otu_frm_handle_t     *otu_frm_handle,
                                           BOOL                 reset_otu_frm )

{
    PMC_ENTRY();
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    if( reset_otu_frm )
    {
        /* take oduksc into reset */
        oduksc_reset_cfg(otu_frm_handle->oduksc_handle, RESET_BIT);
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RESET,SET_BIT);         /* otu_frm Reset mode   */
    }
    else
    {
        /* take oduksc out of reset */
        oduksc_reset_cfg(otu_frm_handle->oduksc_handle, SET_BIT);
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RESET,RESET_BIT);         /* otu_frm No Reset mode   */
        /* rx line clks will be taken out of reset when provisioned */
    }

    PMC_RETURN(PMC_SUCCESS);
} /* otu_frm_reset_config_set */


/*******************************************************************************
* otu_frm_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring OTU_FRM block into configurable state 
*
* INPUTS:
*   otu_frm_handle       - handle to the otu frm instance to be operated on
*   energy_state         - Energy state to put OTU_FRM into
*
* OUTPUTS:
*    None.
* 
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_init(otu_frm_handle_t     *otu_frm_handle,
                         pmc_energy_state_t   energy_state)
{

    PMC_ENTRY();
  
    PMC_ASSERT(NULL != otu_frm_handle, OTU_FRM_ERR_INVALID_PTR, 0, 0);
    
    switch (energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        otu_frm_blk_init(otu_frm_handle);
        break;

    case PMC_ENERGY_STATE_RESET:
        /* configure otu_frm block */
        otu_frm_pwr_mode_config_set(otu_frm_handle,SW_LOWPWR_LS);
        otu_frm_reset_config_set(otu_frm_handle,TRUE);

        /* configure shared blocks */
        oduksc_init(otu_frm_handle->oduksc_handle, PMC_ENERGY_STATE_RESET);
        otu_frm_var_init(otu_frm_handle); /* reset context variables */
        break;
        
    default:
        PMC_ASSERT(FALSE, OTU_FRM_ERR_INVALID_PARAMETERS, energy_state, 0);
        break;
    } /* end switch */

    PMC_RETURN();
} /* otu_frm_init */


/*******************************************************************************
* otu_frm_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   otu frm instance.  It will test the resource state for all resources within
*   the otu frm instance if any are not in STREAM_STATE_START state then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the otu frm and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
* INPUTS:
*   otu_frm_handle        - handle to the otu frm instance to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
* 
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL otu_frm_start_state_test(otu_frm_handle_t    *otu_frm_handle) 
{
    UINT32 dsi_stream = 0;
    BOOL   rx_prov = TRUE;  /* rx no resources provisioned */
    BOOL   tx_prov = TRUE;  /* tx no resources provisioned */
    BOOL   rc = TRUE;

    PMC_ENTRY();
    PMC_ASSERT(otu_frm_handle!=NULL, OTU_FRM_ERR_INVALID_PTR, 0, 0);

    for (dsi_stream = 0; dsi_stream < MAX_10G_STREAM_NUM; dsi_stream++) 
    {
        /* check only for 10G- if ALL 10G are free --> no 40G or 100G is configured */
        rx_prov = otu_frm_validate_rx_prov(otu_frm_handle,RATE_10G,dsi_stream);    
        tx_prov = otu_frm_validate_tx_prov(otu_frm_handle,RATE_10G,dsi_stream);    
        
        /* if we failed on either rx or tx provision validation - exit - there are resources already provisioned */
        if( FALSE == tx_prov || FALSE==rx_prov )
        {
            rc = (tx_prov && rx_prov);
            break;
        }
    }
        
    PMC_RETURN( rc );

} /* otu_frm_start_state_test */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/

/*******************************************************************************
* otu_frm_blk_prov_rx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Full provisioning of a Rx stream per the user requested DSI lines and oduk type.
*   If successful: stream state changes from STREAM_STATE_START --> STREAM_STATE_EQUIPPED.
*   The following actions are taken: \n
*       1. Validation of the provisioning request from state/resources point of view \n
*       2. set the dci channel to passthru mode \n
*       3. DSI streams configuration:
*           a) FEC ins/removal - enabled;
*           b) dLom enabled;
*           c) sf squelch enabled;
*           d) scrambling/de-scrambling set per user configuration;
*           e) data smoothing - user defined \n
*       4. CBR_INT configuration:
*           a) dci channel allocation;
*           b) ch id is set to allocated dci channel;
*           c) dsi lanes to dci channel allocation (done in the CBR_INT module);
*           d) take channel out of reset;
*           e) channel is left in disabled mode;
*           f) ram_err_inject and ram_pg_override are set to 0 \n
*       5. oduk scheduler provisioning -->(scbs3 + util_schd modules) \n
*       6.  OTU_FRM_RX (SK) configuration:
*           a) maintenance signal disabled
*           b) No maintenance signal is forced
*           c) Degradation threshold are left in default values
*           d) Data smoothing configuration 
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
*   descrambling_enable       - boolean, TRUE : scrambling/descrambling enabled;
*                                        FALSE: scrambling/descrambling disabled
*   data_smooth_enable        - boolean, TRUE : data smoothing (otu degap) enabled;
*                                        FALSE: data smoothing (otu degap) disabled
*
* OUTPUTS:
*   dci_channel               - unsigned,allocated dci channel for the dsi_stream
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*   1. upon failure passthrough is removed \n
*   2. upon success channel will stay in passthrough until it reaches the activate function \n
*   3. this function sets dsi stream parameters both for rx and tx sides \n
*   4. dsi stream state must be STREAM_STATE_START when calling this function \n
*   5. returned dci channel is the same for rx and tx provisioning per dsi_channel. 
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_prov_rx_stream(otu_frm_handle_t    *otu_frm_handle,
                                            UINT8               dsi_stream,
                                            util_global_odukp_type_t     oduk_type,
                                            BOOL                descrambling_enable,
                                            BOOL                data_smooth_enable,
                                            UINT32              *dci_channel)
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    otu_frm_stream_cfg_dsc_t    stream_dsc;
    int_dint_channel_cfg_dsc_t  cbr_int_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_rx_ch_id;
    UINT8                       dsi_10g_stream;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(descrambling_enable <= TRUE,OTU_FRM_ERR_INVALID_PARAMETERS,descrambling_enable,0);
    PMC_ASSERT(data_smooth_enable <= TRUE,OTU_FRM_ERR_INVALID_PARAMETERS,data_smooth_enable,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check that we are in the right state and no other streams are already provisioned */
    if (FALSE == otu_frm_validate_rx_prov(otu_frm_handle,dsi_rate,dsi_stream))
    {
        PMC_RETURN (OTU_FRM_ERR_STREAM_PROV_FAILED);
    }

    /* Start provisioning - put channel to passthru                     */
    /* find a free dci channel id                                           */
    /* this channel id is going to be the same as the scheduler channel id */
    dci_rx_ch_id = otu_frm_get_free_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_rx_ch_id < MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_rx_ch_id,0);

    otu_frm_passthru_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_PASSTHRU,SET_BIT);

    /* save oduk rate per dsi stream */
    dsi_10g_stream = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
    PMC_ASSERT(dsi_10g_stream < MAX_10G_STREAM_NUM, OTU_FRM_ERR_INVALID_PARAMETERS, 0 ,0);
    otu_frm_handle->var.dsi_oduk_type[dsi_10g_stream] = oduk_type;

    /* Set defaults and user definitions per the requested stream */
    /* configure only rx side fields */
    PMC_MEMSET((void *)&stream_dsc, 0, sizeof(otu_frm_stream_cfg_dsc_t));    
    stream_dsc.stream_dsc.descrambling_enable = (UINT8)descrambling_enable;  /* user defined - user needs it because of different FEC's configuration */
    stream_dsc.stream_dsc.fec_col_rm_enable   = 1;      /* FEC removal - enabled  */
    stream_dsc.stream_dsc.dlom_enable         = 1;  /* dlom enabled         */
    stream_dsc.stream_dsc.sf_squelch_enable   = 1;  /* sf_squelch enabled   */
    stream_dsc.stream_dsc.dlom_to_sf_enable   = 1;  
    stream_dsc.stream_lane                    = dsi_stream;

    rc = otu_frm_stream_cfg_set(otu_frm_handle,dsi_rate,&stream_dsc,SK_DIR);

    /* interleaver configuration */
    if( PMC_SUCCESS == rc )
    {
        PMC_MEMSET((void *)&cbr_int_chan_dsc, 0, sizeof(int_dint_channel_cfg_dsc_t));
        /* set the interleaver lanes and channel id - find free channel id */
        cbr_int_chan_dsc.channel_lane           = dsi_stream;
        cbr_int_chan_dsc.channel_dsc.ch_id      = dci_rx_ch_id;
        cbr_int_chan_dsc.channel_dsc.ch_enable  = 0; /* disable channel */

        rc = cbr_int_channel_cfg_set(otu_frm_handle,dsi_rate,&cbr_int_chan_dsc,(otu_frm_bit_value_t)1);
    }

    /* oduksc configuration */
    if( PMC_SUCCESS == rc )
    {
        rc = oduksc_chnl_prov(otu_frm_handle->oduksc_handle,dci_rx_ch_id, UTIL_GLOBAL_ODUK_DONT_CARE, oduk_type, 0, UTIL_GLOBAL_NO_MAP, 0);
    }
    if( PMC_SUCCESS == rc )
    {
        rc = oduksc_chnl_update(otu_frm_handle->oduksc_handle, dci_rx_ch_id, TRUE); /* changes channel state in oduksc */
    }
    if( PMC_SUCCESS == rc )
    {
        rc = otu_frm_db_prov_update(otu_frm_handle,dci_rx_ch_id,TRUE);  /* update db */
    }

    /* otu_frm_rx configuration */
    if( PMC_SUCCESS == rc )
    {
        /* rx side */
        /* This is a good starting point - user can override later if he wants to */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MI_LCK,0); /* disable maintenance signal */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MAINT_SIG,0); /* No maintenance signal is forced */
    }

    /* Fec Degap and data smoothing parameters - user defined */
    if( PMC_SUCCESS == rc )
    {
        if(data_smooth_enable)
            rc = otu_frm_stream_fec_degap_set(otu_frm_handle, dsi_stream, dsi_rate,SET_BIT);
        else
            rc = otu_frm_stream_fec_degap_set(otu_frm_handle, dsi_stream, dsi_rate,RESET_BIT);
    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        (*dci_channel) = dci_rx_ch_id; /* let the user know what is the allocated dci channel */
        otu_frm_set_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_EQUIPPED);
    }
    else
    {
        /* since we failed - take channel out of passthru mode */
        otu_frm_passthru_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_PASSTHRU,RESET_BIT);

    }

    PMC_RETURN(rc);
}/* otu_frm_blk_prov_rx_stream */


/*******************************************************************************
* otu_frm_blk_deprov_rx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisioning of a provisioned Rx stream.
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_START.
*   The following actions are taken:\n
*       1. Validation of the deprovisioning request state point of view \n
*       2. set the dci channel to passthru mode \n
*       3. DSI (rx side only) streams configuration:
*           a) FEC removal disabled;
*           b) dLom disabled;
*           c) sf squelch disabled;
*           d) scrambling disabled;
*           e) reset data smoothing (FEC degap)\n
*       4. CBR_INT configuration:
*           a) dci channel deallocation;
*           b) ch id is set 0;
*           c) dsi lanes to dci stream deallocation (done in the CBR_INT module);
*           d) take channel stays out of reset;
*           e) channel is in disabled mode\n
*       5. oduk scheduler deprovisioning -->(scbs3 + util_schd modules)\n
*       6. OTU_FRM_RX (SK) configuration:
*           a) maintenance signal disabled;
*           b) No maintenance signal is forced;
*           c) Expected TTI data set to 0;
*           d) Degradation threshold are not changed;
*           e) Data smoothing configuration
*
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. if validation passes, passthru mode is set and not removed regardless
*      of function success or failure. \n
*   2. this function sets dsi stream parameters ONLY for rx side \n
*   3. dsi stream state must be STREAM_STATE_EQUIPPED or STREAM_STATE_OPERATIONAL
*     when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_deprov_rx_stream(otu_frm_handle_t      *otu_frm_handle,
                                              UINT8             dsi_stream,
                                              util_global_odukp_type_t   oduk_type)
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    otu_frm_stream_cfg_dsc_t    stream_dsc;
    int_dint_channel_cfg_dsc_t  cbr_int_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_rx_ch_id;
    otu_frm_stream_cfg_dsc_t * stream_dsc_ptr = &stream_dsc;
    UINT8                       dsi_10g_stream;

    PMC_ENTRY();
    
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    dci_rx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_rx_ch_id< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_rx_ch_id,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state - if we are in active state, deactivate on behalf of the user */
    if (otu_frm_get_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) == STREAM_STATE_OPERATIONAL )
    {
        /* call deactivate function first */
        rc = otu_frm_blk_deact_rx_stream(otu_frm_handle,dsi_stream,oduk_type);
    }

    if ( rc != PMC_SUCCESS ||
         otu_frm_get_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_EQUIPPED )
    {
        PMC_RETURN(OTU_FRM_ERR_STREAM_DEPROV_FAILED);
    }

    if( PMC_SUCCESS == rc )
    {
        /* put LCK Rx direction */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MAINT_SIG,2); /* 2-> LCK maint signal */
    }

    /* set to pass-through because we are about to touch registers that need pass-through */
    /* This is a precaution since passthru should have already been set by the rx */
    /* Provisioning function, however, this function can be called also when we are    */
    /* after deactivate function was called                                           */
    otu_frm_passthru_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_PASSTHRU,SET_BIT);

    /* Set defaults and user definitions per the requested stream */
    /* Read Modify Write */
    if(RATE_10G==dsi_rate)
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream],sizeof(stream_dsc));
    else if(RATE_40G==dsi_rate)
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream],sizeof(stream_dsc));
    else
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream],sizeof(stream_dsc));

    /* modify only rx side fields ONLY !!! */
    stream_dsc.stream_dsc.descrambling_enable = 0;   /* descrambling disable  */
    stream_dsc.stream_dsc.fec_col_rm_enable   = 0;      /* FEC removal disable  */
    stream_dsc.stream_dsc.dlom_enable         = 0;  /* dlom disable         */
    stream_dsc.stream_dsc.sf_squelch_enable   = 0;  /* sf_squelch disable   */
    stream_dsc.stream_dsc.dlom_to_sf_enable   = 0;  /* disable aSSF for non active stream   */
    stream_dsc.stream_lane                    = dsi_stream; /* needed for otu_frm_stream_cfg_set function */
    rc = otu_frm_stream_cfg_set(otu_frm_handle,dsi_rate,&stream_dsc,SK_DIR);

    /* interleaver configuration */
    if( PMC_SUCCESS == rc )
    {
        cbr_int_chan_dsc.channel_lane           = dsi_stream;
        cbr_int_chan_dsc.channel_dsc.ch_id      = INT_DINT_CAL_NULL_ID;
        cbr_int_chan_dsc.channel_dsc.ch_enable  = 0; /* disable channel */
        rc = cbr_int_channel_cfg_set(otu_frm_handle,dsi_rate,&cbr_int_chan_dsc,(otu_frm_bit_value_t)0);
        cbr_int_free_lanes(otu_frm_handle,dsi_stream,dsi_rate);
    }

    /* Free logical dsi/dci stream/channel */
    if( PMC_SUCCESS == rc )
    {
        BOOL free_resources = FALSE;

        if(RATE_10G == dsi_rate)
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].tx_stream_state)
            {
                otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }
        }
        else if(RATE_40G == dsi_rate)
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].tx_stream_state)
            {
                otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }
        }
        else
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].tx_stream_state)
            {
                otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }
        }

        /* mark the dsi stream_dsc as free only if Tx side is already freed */
        /* release dci_channel if both rx and tx are deprovisioned      */
        if ( free_resources )
        {
            otu_frm_handle->var.dci_to_dsi_chan_ids[dci_rx_ch_id] = FREE_STREAM;
            dsi_10g_stream = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
            PMC_ASSERT(dsi_10g_stream < MAX_10G_STREAM_NUM, OTU_FRM_ERR_INVALID_PARAMETERS, 0 ,0);
            otu_frm_handle->var.dsi_oduk_type[dsi_10g_stream] = UTIL_GLOBAL_ODUK_DONT_CARE;
        }

    }

    /* reset the FEC degap mechanism  if it was enabled */
    if( PMC_SUCCESS == rc )
    {
        UINT8 dsi_idx = 0;

        if(RATE_40G == dsi_rate) /* map dsi stream 0,1,2 -->0,4,8*/
            dsi_idx = (dsi_stream*4);
        else
            dsi_idx = dsi_stream;

        if(otu_frm_handle->var.data_smoothing_enabled[dsi_idx])
            /* disable FEC degap */
            rc = otu_frm_stream_fec_degap_set(otu_frm_handle,dsi_stream,dsi_rate,RESET_BIT);
    }

    /* oduksc configuration */
    if( PMC_SUCCESS == rc )
    {
        rc = oduksc_chnl_deprov(otu_frm_handle->oduksc_handle,dci_rx_ch_id);
    }
    if( PMC_SUCCESS == rc )
    {
        rc = oduksc_chnl_update(otu_frm_handle->oduksc_handle, dci_rx_ch_id, TRUE); /* changes channel state in oduksc */
    }
    if( PMC_SUCCESS == rc )
    {
        rc = otu_frm_db_prov_update(otu_frm_handle,dci_rx_ch_id,FALSE);  /* update db */
    }

    /* otu_frm_rx configuration */
    if( PMC_SUCCESS == rc )
    {
        UINT32 exp_tti_data[16];
        PMC_MEMSET (&exp_tti_data[0],0,sizeof(exp_tti_data));

        /* rx side */
        /* This is a good starting point - user can override later if he wants to */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MI_LCK,0); /* disable maintenance signal */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MAINT_SIG,0); /* No maintenance signal is forced */
        (void) otu_frm_sm_tti_set(otu_frm_handle,SK_DIR,dci_rx_ch_id, exp_tti_data); /* tti is set to all 0 */
    }

    /* data smoothing parameters - don't care will be overridden on next prov */

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        otu_frm_set_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_START);
    }

    PMC_RETURN(rc);
}/* otu_frm_blk_deprov_rx_stream */


/*******************************************************************************
* otu_frm_blk_act_rx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activation of a provisioned Rx stream.
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_OPERATIONAL.
*   The following actions are taken: \n
*       1. Validation of the activation request state point of view \n
*       2. DSI (rx side only) stream configuration:
*           a) stream rx line clock is taken out of low power mode;
*           b) data smoothing (FEC degap) is enabled\n
*       3. CBR_INT configuration:
*           a) channel is set to enabled mode\n
*       4. oduk scheduler page swap and activation -->(scbs3 + util_schd modules)
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. upon failure passthrough is NOT removed \n
*   2. upon success dci channel passthrough is removed \n
*   3. dsi stream state must be STREAM_STATE_EQUIPPED when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_act_rx_stream(otu_frm_handle_t         *otu_frm_handle,
                                           UINT8                dsi_stream,
                                           util_global_odukp_type_t      oduk_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    int_dint_channel_cfg_dsc_t  cbr_int_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_rx_ch_id;
    UINT8                       dsi_idx;

    PMC_ENTRY();
    int_dint_channel_cfg_dsc_t * cbr_int_chan_dsc_ptr = &cbr_int_chan_dsc;
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    dci_rx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_rx_ch_id< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_rx_ch_id,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state */
    if (otu_frm_get_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_EQUIPPED)
    {
        PMC_RETURN(OTU_FRM_ERR_STREAM_ACT_FAILED);
    }

    /* first thing - activate the calender to allow traffic flow before  */
    /* pass-through is removed                                            */
    if( PMC_SUCCESS == rc )
    {
        rc = oduksc_chnl_activate(otu_frm_handle->oduksc_handle, dci_rx_ch_id);
    }

    /* set dsc config array index based on dsi stream for 40G */
    /* 100G and 10G can use dsi stream directly              */
    if(RATE_40G==dsi_rate)
        dsi_idx = (dsi_stream * 4);
    else
        dsi_idx = dsi_stream;

    if(otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_idx].channel_lane != FREE_LANE)
    {
        /* read modify write */
        PMC_MEMCPY(cbr_int_chan_dsc_ptr,&otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_idx],sizeof(cbr_int_chan_dsc));
        cbr_int_chan_dsc.channel_dsc.ch_enable = 1; /* enable the channel */
        cbr_int_chan_dsc.channel_lane = dsi_stream; /* lane must be 0,1,2 */
        rc = cbr_int_lanes_cfg_set( otu_frm_handle,dsi_rate,&cbr_int_chan_dsc);
    }

    if( PMC_SUCCESS == rc  &&
        otu_frm_handle->var.data_smoothing_enabled[dsi_idx] )
    {
        rc = otu_frm_fec_degap_enable(otu_frm_handle,dsi_stream,dsi_rate,TRUE);
    }

    if( PMC_SUCCESS == rc )
    {
        /* remove LCK Rx direction */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MAINT_SIG,0); /* 0-> No maint signal */
    }

    if( PMC_SUCCESS == rc )
    {
        /* take channel out passthru mode             */
        otu_frm_passthru_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_PASSTHRU,RESET_BIT);
    }

    /* set next state */
    if( PMC_SUCCESS == rc )
    {
        otu_frm_set_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_OPERATIONAL);
    }

    PMC_RETURN(rc);

}/*  otu_frm_blk_act_rx_stream */


/*******************************************************************************
* otu_frm_blk_deact_rx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   De-activation of an active Rx stream.
*   If successful: channel  state changes from STREAM_STATE_OPERATIONAL --> STREAM_STATE_EQUIPPED.
*   The following actions are taken: \n
*       1. Validation of the deactivation request state point of view. \n
*       2. DSI (rx side only) streams configuration: 
*           a) stream rx line clock is set to low power mode;
*           b) data smoothing (FEC degap) is disabled \n
*       3. CBR_INT configuration:
*           a) channel is set to disabled mode \n
*       4. oduk scheduler is de-activated -->(scbs3 + util_schd modules)
*
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. if validation passes,passthru mode is set and not removed regardless
*     of function success or failure. \n
*   2. dsi stream state must be STREAM_STATE_OPERATIONAL when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_deact_rx_stream(otu_frm_handle_t       *otu_frm_handle,
                                             UINT8              dsi_stream,
                                             util_global_odukp_type_t    oduk_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    int_dint_channel_cfg_dsc_t  cbr_int_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_rx_ch_id;
    UINT8                       dsi_idx;

    PMC_ENTRY();
    int_dint_channel_cfg_dsc_t * cbr_int_chan_dsc_ptr = &cbr_int_chan_dsc;
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    dci_rx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_rx_ch_id < MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* check we are in the right state */
    if (otu_frm_get_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_OPERATIONAL)
    {
        PMC_RETURN (OTU_FRM_ERR_STREAM_DEACT_FAILED);
    }

    /* set passthrough mode */
    otu_frm_passthru_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_PASSTHRU,SET_BIT);

    /* set dsc config array index based on dsi stream for 40G */
    /* 100G and 10G can use dsi stream directly              */
    if(RATE_40G==dsi_rate)
        dsi_idx = (dsi_stream * 4);
    else
        dsi_idx = dsi_stream;

    /* stop the FEC degap mechanism */
    if( PMC_SUCCESS == rc  &&
        otu_frm_handle->var.data_smoothing_enabled[dsi_idx] )
    {
        rc = otu_frm_fec_degap_enable(otu_frm_handle,dsi_stream,dsi_rate,FALSE);
    }

    if(otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_idx].channel_lane != FREE_CHANNEL &&
       otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_idx].channel_dsc.ch_enable == SET_BIT)
    {
        /* read modify write */
        PMC_MEMCPY(cbr_int_chan_dsc_ptr,&otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_idx],sizeof(cbr_int_chan_dsc));
        cbr_int_chan_dsc.channel_dsc.ch_enable = 0; /* disable the channel */
        cbr_int_chan_dsc.channel_lane = dsi_stream; /* lane must be 0,1,2 in 40G */
        rc = cbr_int_lanes_cfg_set( otu_frm_handle,dsi_rate,&cbr_int_chan_dsc);
    }

    if( PMC_SUCCESS == rc)
    {
        /* stop the scheduler */
        rc = oduksc_chnl_deactivate(otu_frm_handle->oduksc_handle, dci_rx_ch_id);
    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        otu_frm_set_rx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_EQUIPPED);
    }

    PMC_RETURN(rc);
} /* otu_frm_blk_deact_rx_stream */


/*******************************************************************************
* otu_frm_blk_prov_tx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Full provisioning of a Tx stream per the user requested DSI lines and oduk type.
*   If successful: stream state changes from STREAM_STATE_START --> STREAM_STATE_EQUIPPED.
*   The following actions are taken:\n
*       1. Validation of the provisioning request from state/resources point of view \n
*       2. set the dci channel to passthru mode \n
*       3. DSI stream configuration: 
*           a) FEC ins/removal enabled;
*           b) dLom enabled;
*           c) sf squelch enabled;
*           d) scrambling/de-scrambling  set per user configuration\n
*       4. CBR_DINT configuration:
*           a) dci channel allocation;
*           b) ch id is set to allocated dci channel;
*           c) dsi lanes to dci channel allocation (done in the CBR_INT module);
*           d) take channel out of reset;
*           e) fifo threshold 0;
*           f) channel is left in disabled mode \n
*       5.  OTU_FRM_TX (SO) configuration:
*           a) set OTUk_So SM OH fields to 0; 
*           b) set RES8/RES7/RES13/RES14/GCCO/SM TTI to 0; 
*           c) set to disable the following fields: 
*               OTU_SO_SM_TTI_INS_CTRL (DAPI/SAPI/OP),
*               OTU_SO_SM_BIP8_INS_CTRL,
*               OTU_SO_SM_BEI_BIAE_INS_CTRL,
*               OTU_SO_SM_BDI_INS_CTRL,
*               OTU_SO_SM_IAE_INS_CTRL,
*               OTU_SO_SM_RES7_INS_CTRL,
*               OTU_SO_SM_RES8_INS_CTRL,
*               OTU_SO_GCC0_INS_CTRL,
*               OTU_SO_RES13_INS_CTRL,
*               OTU_SO_RES14_INS_CTRL
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
*   scrambling_enable         - boolean, TRUE : scrambling/descrambling enabled;
*                                        FALSE: scrambling/descrambling disabled
*
* OUTPUTS:
*   dci_channel               - pointer to unsigned,allocated dci channel for the dsi_stream
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. upon failure passthrough is removed\n
*   2. upon success channel will stay in passthrough until it reaches the activate function\n
*   3. this function sets dsi channel parameters both for rx and tx sides\n
*   4. dsi stream state must be STREAM_STATE_START when calling this function\n
*   5. returned dci channel is the same for rx and tx provisioning per dsi_channel.
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_prov_tx_stream(otu_frm_handle_t    *otu_frm_handle,
                                            UINT8               dsi_stream,
                                            util_global_odukp_type_t     oduk_type,
                                            BOOL                scrambling_enable,
                                            UINT32              *dci_channel)
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    otu_frm_stream_cfg_dsc_t    stream_dsc;
    int_dint_channel_cfg_dsc_t  cbr_dint_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_tx_ch_id;
    UINT8                       dsi_10g_stream;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(scrambling_enable <= TRUE,OTU_FRM_ERR_INVALID_PARAMETERS,scrambling_enable,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state */
    if (FALSE == otu_frm_validate_tx_prov(otu_frm_handle,dsi_rate,dsi_stream))
    {
        PMC_RETURN (OTU_FRM_ERR_STREAM_PROV_FAILED);
    }

    /* Start provisioning - put channel to passthru                     */
    /* find a free dci channel id                                           */
    /* this channel id is going to be the same as the scheduler channel id */
    dci_tx_ch_id = otu_frm_get_free_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_tx_ch_id < MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    otu_frm_passthru_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_PASSTHRU,SET_BIT);

    /* save oduk rate per dsi stream */
    dsi_10g_stream = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
    PMC_ASSERT(dsi_10g_stream < MAX_10G_STREAM_NUM, OTU_FRM_ERR_INVALID_PARAMETERS, 0 ,0);
    otu_frm_handle->var.dsi_oduk_type[dsi_10g_stream] = oduk_type;

    /* Set defaults and user definitions per the requested stream */
    /* Tx side fields */
    /* same values in relevant fields of rx to keep tx side symmetric */
    PMC_MEMSET((void *)&stream_dsc, 0, sizeof(otu_frm_stream_cfg_dsc_t));    
    stream_dsc.stream_dsc.fec_col_ins_enable = 1;       /* FEC insertion - enabled   */
    stream_dsc.stream_dsc.scarmbling_enable  = (UINT8)scrambling_enable;
    stream_dsc.stream_lane                   = dsi_stream;

    rc = otu_frm_stream_cfg_set(otu_frm_handle,dsi_rate,&stream_dsc,SO_DIR);

    /* de-interleaver configuration */
    if( PMC_SUCCESS == rc )
    {
        PMC_MEMSET((void *)&cbr_dint_chan_dsc, 0, sizeof(int_dint_channel_cfg_dsc_t));
        cbr_dint_chan_dsc.channel_lane          = dsi_stream;
        cbr_dint_chan_dsc.channel_dsc.ch_id     = dci_tx_ch_id;
        cbr_dint_chan_dsc.channel_dsc.ch_enable = 0; /* disable channel */
        rc = cbr_dint_channel_cfg_set(otu_frm_handle,dsi_rate,&cbr_dint_chan_dsc,(otu_frm_bit_value_t)1);
    }

    /* otu_frm_tx configuration */
    if( PMC_SUCCESS == rc )
    {
        otu_frm_sm_oh_t oh_field;
        otu_frm_ctl_field_t ctl_field;

        /* reset all OH and CTL registers */
        for(oh_field = OTU_SO_MFAS; oh_field < LAST_OH; oh_field = (otu_frm_sm_oh_t)((UINT32)oh_field + 1) )
        {
            (void) otu_frm_sm_oh_set(otu_frm_handle,dci_tx_ch_id,oh_field, 0); /* set RES8/RES7/RES13/RES14/GCCO to 0 */
        }

        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id, OTU_SO_FAS_INS_CTRL, 0);
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id, OTU_SO_MFAS_INS_CTRL, 0);
       
        /* now set all the recommended fields to enable */
        for(ctl_field = OTU_SO_SM_TTI_INS_CTRL; ctl_field <= OTU_SO_RES14_INS_CTRL; ctl_field = (otu_frm_ctl_field_t)((UINT32)ctl_field + 1) )
        {
            (void)otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id,ctl_field, 1);
        }

    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        (*dci_channel) = dci_tx_ch_id; /* let the user know what is the allocated dci channel */
        otu_frm_set_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_EQUIPPED);
    }
    else
    {
        /* since we failed - take channel out of passthru mode */
        otu_frm_passthru_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_PASSTHRU,RESET_BIT);
    }

    PMC_RETURN(rc);
}/* otu_frm_blk_prov_tx_stream */


/*******************************************************************************
* otu_frm_blk_deprov_tx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisioning of a provisioned Tx stream.
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_START.
*   The following actions are taken:\n
*       1. Validation of the deprovisioning request state point of view \n
*       2. set the dci channel to passthru mode \n
*       3. DSI (tx side only) stream configuration: 
*           a) FEC insertion disabled;
*           b) descrambling disabled \n
*       4. CBR_DINT configuration: 
*           a) dci channel deallocation;
*           b) ch id is set 0;
*           c) dsi lanes to dci channel deallocation (done in the CBR_INT module);
*           d) take channel stays out of reset;
*           e) channel is in disabled mode;\n
*       5.  OTU_FRM_TX (S0) configuration:
*           - set OTUk_So SM OH fields to 0
*           - set to disable the following fields:
*               OTU_SO_SM_TTI_INS_CTRL (DAPI/SAPI/OP),
*               OTU_SO_SM_BIP8_INS_CTRL,
*               OTU_SO_SM_BEI_BIAE_INS_CTRL,
*               OTU_SO_SM_BDI_INS_CTRL,
*               OTU_SO_SM_IAE_INS_CTRL,
*               OTU_SO_SM_RES7_INS_CTRL,
*               OTU_SO_SM_RES8_INS_CTRL,
*               OTU_SO_GCC0_INS_CTRL,
*               OTU_SO_RES13_INS_CTRL,
*               OTU_SO_RES14_INS_CTRL
*
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
* 
* NOTES:
*   1. if validation passes,passthru mode is set and not removed regardless
*     of function success or failure. \n
*   2. this function sets dsi stream parameters ONLY for tx side \n
*   3. dsi stream state must be STREAM_STATE_EQUIPPED or STREAM_STATE_OPERATIONAL
*     when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_deprov_tx_stream(otu_frm_handle_t      *otu_frm_handle,
                                              UINT8             dsi_stream,
                                              util_global_odukp_type_t   oduk_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    otu_frm_stream_cfg_dsc_t    stream_dsc;
    int_dint_channel_cfg_dsc_t  cbr_dint_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_tx_ch_id;
    UINT8                       dsi_10g_stream;

    PMC_ENTRY();
    otu_frm_stream_cfg_dsc_t * stream_dsc_ptr = &stream_dsc;
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    dci_tx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_tx_ch_id< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_tx_ch_id,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state - if we are in active state, deactivate on behalf of the user */
    if (otu_frm_get_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) == STREAM_STATE_OPERATIONAL )
    {
        /* call deactivate function first */
        rc = otu_frm_blk_deact_tx_stream(otu_frm_handle,dsi_stream,oduk_type);
    }

    if (rc != PMC_SUCCESS ||
        otu_frm_get_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_EQUIPPED )
    {
        PMC_RETURN (OTU_FRM_ERR_STREAM_DEPROV_FAILED);
    }

    if( PMC_SUCCESS == rc )
    {
        /* put LCK Tx direction */
        (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_MAINT_SIG,2); /* 2-> LCK maint signal */
    }

    /* set to pass-through because we are about to touch registers that need pass-through */
    /* This is a precaution since passthru should have already been set by the rx */
    /* Provisioning function, however, this function can be called also when we are    */
    /* after deactivate function was called                                           */
    otu_frm_passthru_set(otu_frm_handle,dci_tx_ch_id,OTU_SK_PASSTHRU,SET_BIT);

    /* Set defaults and user definitions per the requested stream */
    /* Read Modify Write */
    if(RATE_10G==dsi_rate)
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream],sizeof(stream_dsc));
    else if(RATE_40G==dsi_rate)
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream],sizeof(stream_dsc));
    else
        PMC_MEMCPY(stream_dsc_ptr,&otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream],sizeof(stream_dsc));

    /* Tx side fields  ONLY !!! */
    /* same values in relevant fields of rx to keep tx side symmetric */
    stream_dsc.stream_dsc.fec_col_ins_enable = 0;   /* FEC ins disable  */
    stream_dsc.stream_dsc.scarmbling_enable  = 0;   /* scrambling disable  */

    rc = otu_frm_stream_cfg_set(otu_frm_handle,dsi_rate,&stream_dsc,SO_DIR);

    /* de-interleaver configuration */
    if( PMC_SUCCESS == rc )
    {
        cbr_dint_chan_dsc.channel_lane          = dsi_stream;
        cbr_dint_chan_dsc.channel_dsc.ch_id     = INT_DINT_CAL_NULL_ID;
        cbr_dint_chan_dsc.channel_dsc.ch_enable = 0; /* disable channel */

        rc = cbr_dint_channel_cfg_set(otu_frm_handle,dsi_rate,&cbr_dint_chan_dsc,(otu_frm_bit_value_t)0);

        cbr_dint_free_lanes(otu_frm_handle,dsi_stream,dsi_rate);
    }

    /* Free logical dsi/dci channels */
    if ( PMC_SUCCESS == rc )
    {
        BOOL free_resources = FALSE;

        /* mark the dsi/dci channels as free only if Rx side is already freed */
        if(RATE_10G == dsi_rate)
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].rx_stream_state)
            {
                otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }
        }
        else if(RATE_40G == dsi_rate)
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].rx_stream_state)
            {
                otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }
        }
        else
        {
            if(STREAM_STATE_START == otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].rx_stream_state)
            {
                otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
                free_resources = TRUE;
            }

        }

        /* mark the dsi stream_dsc as free only if Tx side is already freed */
        /* release dci_channel if both rx and tx are deprovisioned      */
        if ( free_resources )
        {
            otu_frm_handle->var.dci_to_dsi_chan_ids[dci_tx_ch_id] = FREE_STREAM;
            dsi_10g_stream = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
            PMC_ASSERT(dsi_10g_stream < MAX_10G_STREAM_NUM, OTU_FRM_ERR_INVALID_PARAMETERS, 0 ,0);
            otu_frm_handle->var.dsi_oduk_type[dsi_10g_stream] = UTIL_GLOBAL_ODUK_DONT_CARE;
        }

    }

    /* otu_frm_tx configuration */
    if( PMC_SUCCESS == rc )
    {        
        otu_frm_ctl_field_t ctl_field;

        /* reset all CTL registers; no need to reset the OH fields  */
        for(ctl_field = OTU_SO_FAS_INS_CTRL; ctl_field <= OTU_SO_RES14_INS_CTRL; ctl_field = (otu_frm_ctl_field_t)((UINT32)ctl_field + 1) )
        {
            (void) otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id,ctl_field, 0);
        }

    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        (void) otu_frm_set_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_START);
    }

    PMC_RETURN(rc);
}/* otu_frm_blk_deprov_tx_stream */


/*******************************************************************************
* otu_frm_blk_act_tx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activation of a provisioned Tx stream.
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_OPERATIONAL.
*   The following actions are taken: \n
*       1. Validation of the activation request state point of view\n
*       2. CBR_DINT configuration:
*           a) channel is set to enabled mode
*

*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*   1. upon failure passthrough is NOT removed \n
*   2. upon success channel passthrough is removed \n
*   3. dsi stream state must be STREAM_STATE_EQUIPPED when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_act_tx_stream(otu_frm_handle_t         *otu_frm_handle,
                                           UINT8                dsi_stream,
                                           util_global_odukp_type_t      oduk_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    int_dint_channel_cfg_dsc_t  cbr_dint_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_tx_ch_id;
    UINT8                       dsi_idx = 0;

    PMC_ENTRY();
    int_dint_channel_cfg_dsc_t * cbr_dint_chan_dsc_ptr = &cbr_dint_chan_dsc;
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    dci_tx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_tx_ch_id< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_tx_ch_id,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state */
    if (otu_frm_get_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_EQUIPPED)
    {
        PMC_RETURN(OTU_FRM_ERR_STREAM_ACT_FAILED);
    }

    /* set dsc config array index based on dsi stream for 40G */
    /* 100G and 10G can use dsi stream directly              */
    if(RATE_40G==dsi_rate)
        dsi_idx = (dsi_stream * 4);
    else
        dsi_idx = dsi_stream;

    if(otu_frm_handle->var.cbr_dint_channel_id_cfg[dsi_idx].channel_lane != FREE_LANE)
    {
        /* read modify write */
        PMC_MEMCPY(cbr_dint_chan_dsc_ptr,&otu_frm_handle->var.cbr_dint_channel_id_cfg[dsi_idx],sizeof(cbr_dint_chan_dsc));
        cbr_dint_chan_dsc.channel_dsc.ch_enable = 1; /* enable the channel */
        cbr_dint_chan_dsc.channel_lane = dsi_stream; /* lane must be 0,1,2 in 40G */
        rc = cbr_dint_lanes_cfg_set( otu_frm_handle,dsi_rate,&cbr_dint_chan_dsc);
    }

    if( PMC_SUCCESS == rc )
    {
        /* remove LCK Tx direction */
        otu_frm_sm_ctl_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_MAINT_SIG,0); /* 0-> No maint signal */
    }

    if( PMC_SUCCESS == rc)
    {
        /* take channel out passthru mode             */
        otu_frm_passthru_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_PASSTHRU,RESET_BIT);
    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        otu_frm_set_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_OPERATIONAL);
    }

    PMC_RETURN(rc);

}/* otu_frm_blk_act_tx_stream */


/*******************************************************************************
* otu_frm_blk_deact_tx_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   De-activation of an active Tx stream.
*   If successful: stream state changes from STREAM_STATE_OPERATIONAL --> STREAM_STATE_EQUIPPED.
*   The following actions are taken: \n
*       1. Validation of the deactivation request state point of view. \n
*       2. CBR_DINT configuration:
*           a) channel is set to disabled mode
*
*
* INPUTS:
*   otu_frm_handle            - pointer to OTU_FRM handle instance
*   dsi_stream                - unsigned, for 10G configuration (0..11);
*                                         for 40G configuration (0..2);
*                                         for 100G configuration(0)
*   oduk_type                 - enum, (ODU1E ... ODU4)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. if validation passes,passthru mode is set and not removed regardless
*     of function success or failure. \n
*   2. dsi stream state must be STREAM_STATE_OPERATIONAL when calling this function
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_blk_deact_tx_stream(otu_frm_handle_t       *otu_frm_handle,
                                             UINT8              dsi_stream,
                                             util_global_odukp_type_t    oduk_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    int_dint_channel_cfg_dsc_t  cbr_dint_chan_dsc;
    otu_frm_stream_rate_t       dsi_rate;
    UINT8                       dci_tx_ch_id;
    UINT8                       dsi_idx;

    PMC_ENTRY();
    int_dint_channel_cfg_dsc_t * cbr_dint_chan_dsc_ptr = &cbr_dint_chan_dsc;
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(oduk_type < UTIL_GLOBAL_ODUFLEX_CBR ,OTU_FRM_ERR_INVALID_PARAMETERS,oduk_type,0);

    dsi_rate = otu_frm_oduk_rate_get(oduk_type);
    PMC_ASSERT(dsi_rate < LAST_RATE,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_rate,0);

    dci_tx_ch_id = otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream,dsi_rate);
    PMC_ASSERT(dci_tx_ch_id< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_tx_ch_id,0);

    /* check the dsi_stream range matches the requested rate */
    PMC_ASSERT(otu_frm_validate_dsi_stream(dsi_stream,dsi_rate),OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check we are in the right state */
    if (otu_frm_get_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream) != STREAM_STATE_OPERATIONAL)
    {
        PMC_RETURN(OTU_FRM_ERR_STREAM_DEACT_FAILED);
    }

    /* set pass-through mode */
    otu_frm_passthru_set(otu_frm_handle,dci_tx_ch_id,OTU_SO_PASSTHRU,SET_BIT);

    /* set dsc config array index based on dsi stream for 40G */
    /* 100G and 10G can use dsi stream directly              */
    if(RATE_40G==dsi_rate)
        dsi_idx = (dsi_stream * 4);
    else
        dsi_idx = dsi_stream;

    if(otu_frm_handle->var.cbr_dint_channel_id_cfg[dsi_idx].channel_lane != FREE_CHANNEL &&
       otu_frm_handle->var.cbr_dint_channel_id_cfg[dsi_idx].channel_dsc.ch_enable == SET_BIT)
    {
        /* read modify write */
        PMC_MEMCPY(cbr_dint_chan_dsc_ptr,&otu_frm_handle->var.cbr_dint_channel_id_cfg[dsi_idx],sizeof(cbr_dint_chan_dsc));
        cbr_dint_chan_dsc.channel_dsc.ch_enable = 0; /* disable the channel */
        cbr_dint_chan_dsc.channel_lane = dsi_stream; /* lane must be 0,1,2 in 40G */
        rc = cbr_dint_lanes_cfg_set( otu_frm_handle,dsi_rate,&cbr_dint_chan_dsc);
    }

    /* set next state */
    if(PMC_SUCCESS == rc)
    {
        otu_frm_set_tx_stream_state(otu_frm_handle,dsi_rate,dsi_stream,STREAM_STATE_EQUIPPED);
    }

    PMC_RETURN(rc);

}/*  otu_frm_blk_deact_tx_stream */


/*******************************************************************************
* otu_frm_pwr_mode_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   sets the otu_frm power mode into one of 2 predefined power modes.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   sw_pwr_cfg_mode         - enum, requested power mode:
*           SW_LOWPWR_LS        : low power, RAM in light sleep mode,clocks are working, not in reset;
*           SW_ACTIVE           : fully active mode, RAM active,clocks are working, not in reset
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*   1. refers to register OTU_FRM_MASTER_CFG \n
*   2. clocks (355/Line/Rx) are configured separately via the SW_LOW_POWER BIT
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_pwr_mode_config_set( otu_frm_handle_t                  *otu_frm_handle,
                                              otu_frm_sw_pwr_mode_cfg_t     sw_pwr_cfg_mode )

{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    /* set high order bits first */
    switch ( sw_pwr_cfg_mode )
    {
        /* Not in reset, LS mode, Clocks active */
    case SW_LOWPWR_LS:
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RESET,RESET_BIT);         /* No Reset mode                        */
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RAM_LOW_POWER,SET_BIT);   /* RAM low power effect                 */
        break;

        /* Ram and clocks are at full power consumption */
    case SW_ACTIVE:
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RESET,RESET_BIT);         /* No Reset mode                        */
        otu_frm_master_cfg_bits_set(otu_frm_handle,SW_RAM_LOW_POWER,RESET_BIT); /* No RAM low power effect              */
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    }

    if(PMC_SUCCESS == rc)
    {
        /* save power mode */
        otu_frm_handle->var.otu_frm_pwr_cfg.sw_power_mode = sw_pwr_cfg_mode;
    }

    PMC_RETURN(rc);
} /* otu_frm_pwr_mode_config_set */


/*******************************************************************************
* otu_frm_stream_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the scrambling,descrambling, FEC column removal/insertion,dLOM enable and
*   sf squelch enable disable bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   stream_rate             - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G,
*
*   stream_dscptr           - pointer to user stream descriptor structure:
*
*   rx_tx_dir               - enum, rx (SK) or tx (SO) direction specifier
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_stream_cfg_set(otu_frm_handle_t           *otu_frm_handle,
                                        otu_frm_stream_rate_t      stream_rate,
                                        otu_frm_stream_cfg_dsc_t   *stream_dscptr,
                                        otu_frm_direction_t        rx_tx_dir )

{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(stream_dscptr!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    switch (stream_rate)
    {
    case RATE_10G:
        if( stream_dscptr->stream_lane < MAX_10G_STREAM_NUM )
        {
            /* copy stream parameters to the handle configuration */
            PMC_MEMCPY(&otu_frm_handle->var.otu_frm_10G_stream_cfg[stream_dscptr->stream_lane].stream_dsc,
                       &stream_dscptr->stream_dsc,
                       sizeof(otu_frm_stream_cfg_t));

            otu_frm_handle->var.otu_frm_10G_stream_cfg[stream_dscptr->stream_lane].stream_lane = stream_dscptr ->stream_lane;

            /* update hw register by direction asked */
            if(SK_DIR==rx_tx_dir)/* rx side */
            {
                otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.descrambling_enable);
                otu_frm_field_OTU_FRM_10G_DLOM_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.dlom_enable);
                otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.sf_squelch_enable);
                otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.fec_col_rm_enable);
                otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane,stream_dscptr->stream_dsc.dlom_to_sf_enable);
            }
            else/* tx side */
            {
                otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.scarmbling_enable);
                otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.fec_col_ins_enable);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    case RATE_40G:
        if( stream_dscptr->stream_lane < MAX_40G_STREAM_NUM )
        {
            /* copy stream parameters to the handle configuration */
            PMC_MEMCPY(&otu_frm_handle->var.otu_frm_40G_stream_cfg[stream_dscptr->stream_lane].stream_dsc,
                       &stream_dscptr->stream_dsc,
                       sizeof(otu_frm_stream_cfg_t));

            otu_frm_handle->var.otu_frm_40G_stream_cfg[stream_dscptr->stream_lane].stream_lane = stream_dscptr ->stream_lane;

            /* update hw register by direction asked */
            if(SK_DIR==rx_tx_dir) /* rx side */
            {
                otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.descrambling_enable);
                otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.fec_col_rm_enable);
                otu_frm_field_OTU_FRM_40G_DLOM_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.dlom_enable);
                otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.sf_squelch_enable);
                otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane,stream_dscptr->stream_dsc.dlom_to_sf_enable);
            }
            else /* tx side */
            {
                otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.scarmbling_enable);
                otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_lane, stream_dscptr->stream_dsc.fec_col_ins_enable);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;


    case RATE_100G:
        if( stream_dscptr->stream_lane < MAX_100G_STREAM_NUM )
        {
            /* copy stream parameters to the handle configuration */
            PMC_MEMCPY(&otu_frm_handle->var.otu_frm_100G_stream_cfg[stream_dscptr->stream_lane].stream_dsc,
                       &stream_dscptr->stream_dsc,
                       sizeof(otu_frm_stream_cfg_t));

            otu_frm_handle->var.otu_frm_100G_stream_cfg[stream_dscptr->stream_lane].stream_lane = stream_dscptr ->stream_lane;

            /* update hw register by direction asked */
            if(SK_DIR==rx_tx_dir)/* rx side */
            {
                otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.descrambling_enable);
                otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set(NULL,otu_frm_handle, stream_dscptr->stream_dsc.fec_col_rm_enable);
                otu_frm_field_OTU_FRM_100G_DLOM_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.dlom_enable);
                otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.sf_squelch_enable);
                otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.dlom_to_sf_enable);
            }
            else /* tx side */
            {
                otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.scarmbling_enable);
                otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set(NULL,otu_frm_handle,stream_dscptr->stream_dsc.fec_col_ins_enable);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* otu_frm_stream_cfg_set */


/*******************************************************************************
* otu_frm_stream_scrambling_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the scrambling,descrambling,enable disable bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
*
* INPUTS:
*   otu_frm_handle    - pointer to OTU_FRM handle instance.
*
*   dsi_stream        - unsigned, for 10G configuration (0..11);
*                                 for 40G configuration (0..2);
*                                 for 100G configuration(0)
*
*   stream_rate       - enum, requested data rate:
*                       RATE_10G;
*                       RATE_40G;
*                       RATE_100G
*
*   scarmble_in_otu   - boolean,TRUE:scrambling/descrambling is done at the otu;
*                               FALSE:scrambling/descrambling is not done at the otu
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_stream_scrambling_set(otu_frm_handle_t            *otu_frm_handle,
                                               UINT8                       dsi_stream,
                                               otu_frm_stream_rate_t       stream_rate,
                                               BOOL                        scarmble_in_otu )

{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     otu_scramble = (scarmble_in_otu?1:0);

    PMC_ENTRY();
    
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    switch (stream_rate)
    {
    case RATE_10G:
        otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream, otu_scramble);
        otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream, otu_scramble);
        break;

    case RATE_40G:
        otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream, otu_scramble);
        otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream, otu_scramble);
        break;


    case RATE_100G:
        otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,otu_scramble);
        otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set(NULL,otu_frm_handle,otu_scramble);
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* otu_frm_stream_scrambling_set */


/*******************************************************************************
* otu_frm_dlom_to_sf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the dLOM to SF line bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   stream_rate             - enum, requested data rate:
*                             RATE_10G;
*                             RATE_40G;
*                             RATE_100G
*
*   stream_lane             - unsigned, for 10G (0..11);
*                                       for 40G (0,1,2);
*                                       for 100G (0)
*
*   enable_dlom_to_sf      - enum,requested clk power mode state:
*                             SET_BIT     : 1, enabled;
*                             RESET_BIT   : 0, disabled.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_dlom_to_sf_set( otu_frm_handle_t            *otu_frm_handle,
                                         otu_frm_stream_rate_t       stream_rate,
                                         UINT8                       stream_lane,
                                         otu_frm_bit_value_t         enable_dlom_to_sf )

{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    switch (stream_rate)
    {
    case RATE_10G:
        if( stream_lane < MAX_10G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,stream_lane,enable_dlom_to_sf);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    case RATE_40G:
        if( stream_lane < MAX_40G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,stream_lane,enable_dlom_to_sf);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;


    case RATE_100G:
        if( stream_lane < MAX_100G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,enable_dlom_to_sf);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* otu_frm_dlom_to_sf_set */


/*******************************************************************************
* otu_frm_dlom_to_sf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the dLOM to SF line bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   stream_rate             - enum, requested data rate:
*                             RATE_10G;
*                             RATE_40G;
*                             RATE_100G
*
*   stream_lane             - unsigned, for 10G (0..11);
*                                       for 40G (0,1,2);
*                                       for 100G (0)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 -  requested bit value as was set in the relevant hw register or
*   OTU_FRM_DEFECT_ERROR on failure.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 otu_frm_dlom_to_sf_get( otu_frm_handle_t         *otu_frm_handle,
                                      otu_frm_stream_rate_t    stream_rate,
                                      UINT8                    stream_lane )

{
    UINT32  dlom_to_sf_reg_value = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    switch (stream_rate)
    {
    case RATE_10G:
        if( stream_lane < MAX_10G_STREAM_NUM )
        {
            dlom_to_sf_reg_value =otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get(NULL,otu_frm_handle,stream_lane);
        }
        break;

    case RATE_40G:
        if( stream_lane < MAX_40G_STREAM_NUM )
        {
            dlom_to_sf_reg_value =otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get(NULL,otu_frm_handle,stream_lane);
        }
        break;


    case RATE_100G:
        if( stream_lane < MAX_100G_STREAM_NUM )
        {
            dlom_to_sf_reg_value =otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_get(NULL,otu_frm_handle);
        }
        break;

    default:
        dlom_to_sf_reg_value = OTU_FRM_DEFECT_ERROR;
        break;
    }

    PMC_RETURN(dlom_to_sf_reg_value);
} /* otu_frm_dlom_to_sf_get */


/*******************************************************************************
* otu_frm_mi_active_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the mi active bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   stream_rate             - enum, requested data rate:
*                             RATE_10G;
*                             RATE_40G;
*                             RATE_100G
*
*   stream_lane             - unsigned, for 10G (0..11);
*                                       for 40G (0,1,2);
*                                       for 100G (0)
*
*   enable_mi_active       - enum,requested clk power mode state:
*                             SET_BIT     : 1, enabled;
*                             RESET_BIT   : 0, disabled.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_mi_active_set( otu_frm_handle_t            *otu_frm_handle,
                                        otu_frm_stream_rate_t       stream_rate,
                                        UINT8                       stream_lane,
                                        otu_frm_bit_value_t         enable_mi_active )
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    switch (stream_rate)
    {
    case RATE_10G:
        if( stream_lane < MAX_10G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set(NULL,otu_frm_handle,stream_lane,enable_mi_active);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    case RATE_40G:
        if( stream_lane < MAX_40G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set(NULL,otu_frm_handle,stream_lane,enable_mi_active);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;


    case RATE_100G:
        if( stream_lane < MAX_100G_STREAM_NUM )
        {
            otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set(NULL,otu_frm_handle,enable_mi_active);
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* otu_frm_mi_active_set */


/*******************************************************************************
* OTU FRMR SK/SO functions
*******************************************************************************/

/*******************************************************************************
* otu_frm_sm_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function sets the all fields (SAPI/DAPI/OP) in the OTU_SK_SM_EXP_TTI/OTU_SO_SM_TTI
*   registers per the requested dci channel (0..11). The function works in a
*   read/modify/write of all the TTI values.
*   User is expected to provide ALL of the values to be written to the registers.
*
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   sk_so                   - enum, requested direction SO or SK:
*                             SK_DIR  : write to SK register;
*                             SO_DIR  : write to SO register.
*
*   dci_channel              - UINT8, requested channel (0..11)
*
*   tti_data                - 16xUINT32 unsigned array containing all
*                             tti all values:
*                             sapi_data is the first 16 bytes;
*                             dapi_data is the second 16 bytes;
*                             op_data is the last 32 bytes.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*  1. This function requires user to set the dci channel to passthrough mode
*     mode before function invocation. failure to do so will result in
*     unexpected failures and alarms triggered. \n
*  2. refers to registers OTU_SK_SM_EXP_TTI,OTU_SO_SM_TTI
*
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_tti_set( otu_frm_handle_t       *otu_frm_handle,
                                     otu_frm_direction_t    sk_so,
                                     UINT8                  dci_channel,
                                     UINT32                 tti_data[16] )

{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(sk_so < LAST_DIR,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(tti_data!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    if( SK_DIR == sk_so )
    {
        otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set(NULL,otu_frm_handle,dci_channel,tti_data);

    }
    else /* SO direction */
    {
        otu_frm_tx_lfield_OTU_SO_SM_TTI_set(NULL,otu_frm_handle,dci_channel,tti_data);
    }

    PMC_RETURN(PMC_SUCCESS);

}/* otu_frm_sm_tti_set */


/*******************************************************************************
* otu_frm_sm_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function gets the all fields (SAPI/DAPI/OP) in the OTU_SK_SM_EXP_TTI/OTU_SO_SM_TTI
*   registers per the requested dci channel (0..11). 
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   sk_so                   - enum, requested direction SO or SK:
*                             SK_DIR  : write to SK register;
*                             SO_DIR  : write to SO register.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
* OUTPUTS:
*   tti_data                - 16xUINT32 unsigned array containing all
*                             tti all values:
*                             sapi_data is the first 16 bytes;
*                             dapi_data is the second 16 bytes;
*                             op_data is the last 32 bytes.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*  1. refers to registers OTU_SK_SM_EXP_TTI,OTU_SO_SM_TTI \n
*  2. Invoking this function will cause a read into device registers.
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_tti_get( otu_frm_handle_t       *otu_frm_handle,
                                     otu_frm_direction_t    sk_so,
                                     UINT8                  dci_channel,
                                     UINT32                 tti_data[16] )

{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(sk_so < LAST_DIR,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(tti_data!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    if( SK_DIR == sk_so )
    {
        otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get(NULL,otu_frm_handle,dci_channel,tti_data);

    }
    else /* SO direction */
    {
        otu_frm_tx_lfield_OTU_SO_SM_TTI_get(NULL,otu_frm_handle,dci_channel,tti_data);

    }

    PMC_RETURN(PMC_SUCCESS);

}/* otu_frm_sm_tti_get */


/*******************************************************************************
* otu_frm_exp_tti_tim_det_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function sets OTU_SK_SM_EXP_TTI dTIM detection mode 
*   registers per the requested dci channel (0..11). 
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
*   exp_tti_tim_det_mode    - enum, dTIM detection mode:
*    OTU_SK_SM_DTIM_DET_DISABLED    : dTIM defect detection is disabled;
*    OTU_SK_SM_DTIM_DET_SAPI        : The expected SAPI field is used when determining the dTIM defect;
*    OTU_SK_SM_DTIM_DET_DAPI        : The expected DAPI field is used when determining the dTIM defect;
*    OTU_SK_SM_DTIM_DET_BOTH        : Both the expected SAPI field and the expected DAPI field are 
*                                     used when determining the dTIM defect 
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_exp_tti_tim_det_mode_set( otu_frm_handle_t           *otu_frm_handle,
                                              UINT8                      dci_channel,
                                              otu_frm_sk_tim_det_mode_t  exp_tti_tim_det_mode )
{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_channel,0);
    PMC_ASSERT(exp_tti_tim_det_mode < LAST_SK_SM_DTIM_DET  ,OTU_FRM_ERR_INVALID_PARAMETERS,exp_tti_tim_det_mode,0);

    otu_frm_rx_field_OTU_SK_TIM_DET_MO_set(NULL,otu_frm_handle,dci_channel,(UINT32)exp_tti_tim_det_mode);

    PMC_RETURN();
} /* otu_frm_exp_tti_tim_det_mode_set */


/*******************************************************************************
* otu_frm_exp_tti_tim_det_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function gets OTU_SK_SM_EXP_TTI dTIM detection mode 
*   registers per the requested dci channel (0..11). 
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
* OUTPUTS:
*   exp_tti_tim_det_mode    - pointer to pre allocated enum, dTIM detection mode:
*    OTU_SK_SM_DTIM_DET_DISABLED    : dTIM defect detection is disabled; 
*    OTU_SK_SM_DTIM_DET_SAPI        : The expected SAPI field is used when determining the dTIM defect;
*    OTU_SK_SM_DTIM_DET_DAPI        : The expected DAPI field is used when determining the dTIM defect;
*    OTU_SK_SM_DTIM_DET_BOTH        : Both the expected SAPI field and the expected DAPI field are 
*                                     used when determining the dTIM defect 
* RETURNS:
*   None
* 
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_exp_tti_tim_det_mode_get( otu_frm_handle_t           *otu_frm_handle,
                                              UINT8                      dci_channel,
                                              otu_frm_sk_tim_det_mode_t  *exp_tti_tim_det_mode )
{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_channel,0);
    PMC_ASSERT(exp_tti_tim_det_mode !=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    *exp_tti_tim_det_mode=(otu_frm_sk_tim_det_mode_t)otu_frm_rx_field_OTU_SK_TIM_DET_MO_get(NULL,otu_frm_handle,dci_channel);

    PMC_RETURN();
} /* otu_frm_exp_tti_tim_det_mode_get */


/*******************************************************************************
* otu_frm_ebc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   retrieve LINEOTN otu framer far/near end counters. 
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
* OUTPUTS:
*   *ebc   - pointer to LINEOTN otu framer far/near end counters\n
*
* RETURNS:
*   None
* 
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_ebc_get( otu_frm_handle_t           *otu_frm_handle,
                             UINT8                      dci_channel,
                             otu_frm_counters_t  *ebc )
{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_channel,0);
    PMC_ASSERT(ebc !=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    ebc->otu_frm_sk_sm_p_iae = otu_frm_rx_field_OTU_SK_SM_P_IAE_get(NULL,otu_frm_handle,dci_channel);
    ebc->otu_frm_sk_sm_p_n_ds = otu_frm_rx_field_OTU_SK_SM_P_N_DS_get(NULL,otu_frm_handle,dci_channel);
    ebc->otu_frm_sk_sm_p_n_ebc = otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get(NULL,otu_frm_handle,dci_channel);
    ebc->otu_frm_sk_sm_p_biae = otu_frm_rx_field_OTU_SK_SM_P_BIAE_get(NULL,otu_frm_handle,dci_channel);
    ebc->otu_frm_sk_sm_p_f_ds = otu_frm_rx_field_OTU_SK_SM_P_F_DS_get(NULL,otu_frm_handle,dci_channel);
    ebc->otu_frm_sk_sm_p_f_ebc = otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get(NULL,otu_frm_handle,dci_channel);

    PMC_RETURN();
} /* otu_frm_ebc_get */


/*******************************************************************************
* otu_frm_sm_accepted_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function reads the active TTI register per requested dci channel (0..11) back
*   to a user buffer. This is a SK only function.
*
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
*   tti_data_buf            - pointer to 16xUINT32 array containing user
*                             buffer where tti values are copied to.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*  refers to register OTU_SK_SM_AC_TTI
*
*
*******************************************************************************/
PUBLIC void otu_frm_sm_accepted_tti_get( otu_frm_handle_t      *otu_frm_handle,
                                         UINT8             dci_channel,
                                         UINT32            tti_data_buf[16] )

{
    PMC_ENTRY();
    
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* set the lock bit for master config register */
    otu_frm_sm_sk_glbl_cfg_set(otu_frm_handle,OTU_SK_TTI_LOCK, SET_BIT);

    /* wait 3 multi-frames before touching this register ~1 msec */
    PMC_Q_USLEEP(otu_frm_handle,1000);

    /* READ value  - this is a sk function only */
    otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get(NULL,otu_frm_handle,dci_channel,tti_data_buf);

    /* release the lock bit for master config register after reading */
    otu_frm_sm_sk_glbl_cfg_set(otu_frm_handle,OTU_SK_TTI_LOCK, RESET_BIT);

    PMC_RETURN();

}/* otu_frm_sm_accepted_tti_get */

/*******************************************************************************
* otu_frm_sm_oh_ext_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the a requested OH extracted field from rx side per requested dci
*   channel. The value returned contains the extracted SM overhead field for a
*   single OTUk frame.
*
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dci_channel             - UINT8, requested channel (0..11)
*
*   sk_field                - enum, requested sk (rx) field to retrieve:
*                             OTU_SK_SM_TTI;
*                             OTU_SK_SM_BIP8;
*                             OTU_SK_SM_BEI_BIAE;
*                             OTU_SK_SM_BDI;
*                             OTU_SK_SM_IAE;
*                             OTU_SK_SM_RES;
*                             OTU_SK_RES;
*                             OTU_SK_GCC0
*
* OUTPUTS:
*   extracted_oh_val        - pointer to UINT32 buffer where OH values are copied to.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
* 
* NOTES:
*  refers to register OTUk SK SM OH
*
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_oh_ext_get      ( otu_frm_handle_t  *otu_frm_handle,
                                              UINT8             dci_channel,
                                              otu_frm_sm_oh_t   sk_field,
                                              UINT32            *extracted_oh_val )

{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(extracted_oh_val!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(sk_field<= OTU_SK_GCC0,OTU_FRM_ERR_INVALID_PARAMETERS,sk_field,0);

    /* set the lock bit for master config register */
    otu_frm_sm_sk_glbl_cfg_set(otu_frm_handle,OTU_SK_LOCK, SET_BIT);

    /* READ values  - this is a sk function only */
    switch(sk_field)
    {
    case OTU_SK_SM_TTI:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_TTI_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_BIP8:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_BIP8_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_BEI_BIAE:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_BDI:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_BDI_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_IAE:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_IAE_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_RES:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_SM_RES_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_RES:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_RES_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_GCC0:
        (*extracted_oh_val) = otu_frm_rx_field_OTU_SK_GCC0_get(NULL,otu_frm_handle,dci_channel);
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    /* release the lock bit for master config register after reading */
    otu_frm_sm_sk_glbl_cfg_set(otu_frm_handle,OTU_SK_LOCK, RESET_BIT);

    PMC_RETURN(rc);

}/* otu_frm_sm_oh_ext_get */


/*******************************************************************************
* otu_frm_sm_ctl_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets SK/SO control fields per requested dci channel(0..11).
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   ctl_field           - enum,SK/SO control fields. Please see
*                         registers OTUk SK SM CTL,OTUk SK PASSTHRU,
*                         OTUk_So SM INS CTRL, OTUk_So CTRL, OTUk_So MASTER CTRL
*                         for details on each of the fields.\n
*
*                         SK Control fields:
*                         OTU_SK_BDI_DIS_FROM_TTI,
*                         OTU_SK_BDI_DIS_FROM_SSF,
*                         OTU_SK_BEI_DIS,
*                         OTU_SK_BIAE_DIS,
*                         OTU_SK_TSD_DIS,
*                         OTU_SK_TSF_DIS_FROM_SSF,
*                         OTU_SK_TIM_ACT_DIS,
*                         OTU_SK_ADAPT_TSD_DIS,
*                         OTU_SK_ADAPT_SSF_DIS,
*                         OTU_SK_ADAPT_AIS_DIS,
*                         OTU_SK_MI_LCK,
*                         OTU_SK_SSF_MI_DIS,
*                         OTU_SK_AIS_MI_DIS,
*                         OTU_SK_TSD_MI_DIS,
*                         OTU_SK_TIM_DET_MO,
*                         OTU_SK_MAINT_SIG\n
*
*                         SO Control fields:
*                         OTU_SO_FAS_INS_CTRL,
*                         OTU_SO_MFAS_INS_CTRL,
*                         OTU_SO_SM_TTI_INS_CTRL (DAPI/SAPI/OP),
*                         OTU_SO_SM_BIP8_INS_CTRL,
*                         OTU_SO_SM_BEI_BIAE_INS_CTRL,
*                         OTU_SO_SM_BDI_INS_CTRL,
*                         OTU_SO_SM_IAE_INS_CTRL,
*                         OTU_SO_SM_RES7_INS_CTRL,
*                         OTU_SO_SM_RES8_INS_CTRL,
*                         OTU_SO_GCC0_INS_CTRL,
*                         OTU_SO_RES13_INS_CTRL,
*                         OTU_SO_RES14_INS_CTRL,
*                         OTU_SO_MAINT_SIG,
*                         OTU_SO_MI_ADMINSTATE
*
*   value               - UINT8, 0..8 bit values per field bits.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*  1. This function requires user to set the dci channel to passthrough mode
*     mode before function invocation if configuring an SO control field. failure to do
*     so will result in unexpected failures and alarms triggered. \n
*  2. refers to registers OTUk_SK_SM_CTL,OTUk_SK_PASSTHRU,OTUk_So_SM_INS_CTRL,
*     OTUk_So_CTRL, OTUk_So_MASTER_CTRL
*
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_ctl_set( otu_frm_handle_t    *otu_frm_handle,
                                     UINT8               dci_channel,
                                     otu_frm_ctl_field_t ctl_field,
                                     UINT8               value )

{
    PMC_ERROR   rc = OTU_FRM_ERR_INVALID_PARAMETERS;
    PMC_ENTRY();
    
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(value < 8,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);  /* max 3 bit values are allowed */

    switch (ctl_field)
    {
    /**************** sk control ***************************/
    case OTU_SK_BDI_DIS_FROM_TTI:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_BDI_DIS_FROM_SSF:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_BEI_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_BEI_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_BIAE_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_BIAE_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_TSD_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_TSD_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_TSF_DIS_FROM_SSF:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_TIM_ACT_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_ADAPT_SSD_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_ADAPT_SSF_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_ADAPT_AIS_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_MI_LCK:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_MI_LCK_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_SSF_MI_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_AIS_MI_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_SSD_MI_DIS:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_TIM_DET_MO:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_rx_field_OTU_SK_TIM_DET_MO_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_MAINT_SIG:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_rx_field_OTU_SK_MAINT_SIG_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;


    /**************** so control ***************************/

    case OTU_SO_FAS_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_MFAS_INS_CTRL:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_TTI_INS_CTRL:   
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);

            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BIP8_INS_CTRL:
        if( value < 8 ) /* 3 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BEI_BIAE_INS_CTRL:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BDI_INS_CTRL:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_IAE_INS_CTRL:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_RES7_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_RES8_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_GCC0_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_RES13_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_RES14_INS_CTRL:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_MAINT_SIG:
        if( value < 4 ) /* 2 bit value */
        {
            otu_frm_tx_field_OTU_SO_MAINT_SIG_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_MI_ADMINSTATE:
        if( value < 2 ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;


    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);

}/* otu_frm_sm_ctl_set */


/*******************************************************************************
* otu_frm_sm_ctl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets SK/SO control fields per requested dci channel(0..11).
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   ctl_field           - enum,SK/SO control fields. Please see
*                         registers OTUk SK SM CTL,OTUk SK PASSTHRU,
*                         OTUk_So SM INS CTRL, OTUk_So CTRL, OTUk_So MASTER CTRL
*                         for details on each of the fields.\n
*
*                         SK Control fields:
*                         OTU_SK_BDI_DIS_FROM_TTI,
*                         OTU_SK_BDI_DIS_FROM_SSF,
*                         OTU_SK_BEI_DIS,
*                         OTU_SK_BIAE_DIS,
*                         OTU_SK_TSD_DIS,
*                         OTU_SK_TSF_DIS_FROM_SSF,
*                         OTU_SK_TIM_ACT_DIS,
*                         OTU_SK_ADAPT_TSD_DIS,
*                         OTU_SK_ADAPT_SSF_DIS,
*                         OTU_SK_ADAPT_AIS_DIS,
*                         OTU_SK_MI_LCK,
*                         OTU_SK_SSF_MI_DIS,
*                         OTU_SK_AIS_MI_DIS,
*                         OTU_SK_TSD_MI_DIS,
*                         OTU_SK_TIM_DET_MO,
*                         OTU_SK_MAINT_SIG\n
*
*                         SO Control fields:
*                         OTU_SO_FAS_INS_CTRL, 
*                         OTU_SO_MFAS_INS_CTRL,
*                         OTU_SO_SM_TTI_INS_CTRL (DAPI/SAPI/OP),
*                         OTU_SO_SM_BIP8_INS_CTRL,
*                         OTU_SO_SM_BEI_BIAE_INS_CTRL,
*                         OTU_SO_SM_BDI_INS_CTRL,
*                         OTU_SO_SM_IAE_INS_CTRL,
*                         OTU_SO_SM_RES7_INS_CTRL,
*                         OTU_SO_SM_RES8_INS_CTRL,
*                         OTU_SO_GCC0_INS_CTRL,
*                         OTU_SO_RES13_INS_CTRL,
*                         OTU_SO_RES14_INS_CTRL,
*                         OTU_SO_MAINT_SIG,
*                         OTU_SO_MI_ADMINSTATE
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - requested bit value as was set in the relevant hw register or
*   OTU_FRM_DEFECT_ERROR on failure.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 otu_frm_sm_ctl_get( otu_frm_handle_t    *otu_frm_handle,
                                  UINT8               dci_channel,
                                  otu_frm_ctl_field_t ctl_field )

{
    UINT32 sm_ctl_field_value = 0;
    
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (ctl_field)
    {
    /**************** sk control ***************************/
    case OTU_SK_BDI_DIS_FROM_TTI:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_BDI_DIS_FROM_SSF:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_BEI_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_BEI_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_BIAE_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_BIAE_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_TSD_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_TSD_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_TSF_DIS_FROM_SSF:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_TIM_ACT_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_ADAPT_SSD_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_ADAPT_SSF_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_ADAPT_AIS_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_MI_LCK:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_MI_LCK_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SSF_MI_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_AIS_MI_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SSD_MI_DIS:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_TIM_DET_MO:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_TIM_DET_MO_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_MAINT_SIG:
        sm_ctl_field_value = otu_frm_rx_field_OTU_SK_MAINT_SIG_get(NULL,otu_frm_handle,dci_channel);
        break;


    /**************** so control ***************************/

    case OTU_SO_FAS_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_MFAS_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_TTI_INS_CTRL:   
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;


    case OTU_SO_SM_BIP8_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_BEI_BIAE_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_BDI_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_IAE_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_RES7_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_RES8_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_GCC0_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_RES13_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_RES14_INS_CTRL:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_MAINT_SIG:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_MAINT_SIG_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_MI_ADMINSTATE:
        sm_ctl_field_value = otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get(NULL,otu_frm_handle,dci_channel);
        break;


    default:
        sm_ctl_field_value = OTU_FRM_DEFECT_ERROR;
        break;
    }

    PMC_RETURN(sm_ctl_field_value);

}/* otu_frm_sm_ctl_get */


/*******************************************************************************
* otu_frm_sm_oh_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets SO OH fields per requested dci channel(0..11).
*   This is a SO only function.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   oh_field            - enum,SO OH fields. Please see
*                         register  OTUk_So SM OH
*                         for details on each of the fields:
*                         OTU_SO_FAS_F6,
*                         OTU_SO_FAS_28,
*                         OTU_SO_MFAS,
*                         OTU_SO_SM_BIP8,
*                         OTU_SO_SM_BEI_BIAE,
*                         OTU_SO_SM_BDI,
*                         OTU_SO_SM_IAE,
*                         OTU_SO_SM_RES7,
*                         OTU_SO_SM_RES8,
*                         OTU_SO_GCC0,
*                         OTU_SO_RES13,
*                         OTU_SO_RES14
*
*   value               - UINT32, 0..32 bit values per field bits.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*  1. This function requires user to set the dci channel to passthrough mode
*     mode before function invocation if configuring an SO control field. failure to do
*     so will result in unexpected failures and alarms triggered. \n
*  2. refers to registers OTUk_SK_SM_CTL,OTUk_SK_PASSTHRU,OTUk_So_SM_INS_CTRL,
*     OTUk_So_CTRL, OTUk_So_MASTER_CTRL
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_oh_set( otu_frm_handle_t    *otu_frm_handle,
                                    UINT8               dci_channel,
                                    otu_frm_sm_oh_t     oh_field,
                                    UINT32              value )
{

    PMC_ERROR rc = OTU_FRM_ERR_INVALID_PARAMETERS;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* Set operation can only be done on so */
    switch (oh_field)
    {
    case OTU_SO_FAS_F6:
        if( 0 == (value >> 24) ) /* 24 bit value */
        {
            /* This register is shared across all channels */
            otu_frm_tx_reg_OTU_SO_FAS_F6_write(NULL,otu_frm_handle,value);
            rc = PMC_SUCCESS;
        }
        break;
    
    case OTU_SO_FAS_28:
        if( 0 == (value >> 24) ) /* 24 bit value */
        {
            /* This register is shared across all channels */
            otu_frm_tx_reg_OTU_SO_FAS_28_write(NULL,otu_frm_handle,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_MFAS:
        if( 0 == (value >> 8) ) /* 8 bit value */
        {
            otu_frm_tx_field_OTU_SO_MFAS_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BIP8:
        if( 0 == (value >> 8) ) /* 8 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BIP8_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BEI_BIAE:
        if( 0 == (value >> 4) ) /* 4 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_BDI:
        if( 0 == (value >> 1) ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_BDI_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_IAE:
        if( 0 == (value >> 1) ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_IAE_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_RES7:
        if( 0 == (value >> 1) ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_RES7_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_SM_RES8:
        if( 0 == (value >> 1) ) /* 1 bit value */
        {
            otu_frm_tx_field_OTU_SO_SM_RES8_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_GCC0:
        if( 0 == (value >> 16) ) /* 16 bit value */
        {
            otu_frm_tx_field_OTU_SO_GCC0_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_RES13:
        if( 0 == (value >> 8) ) /* 8 bit value */
        {
            otu_frm_tx_field_OTU_SO_RES13_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SO_RES14:
        if( 0 == (value >> 8) ) /* 8 bit value */
        {
            otu_frm_tx_field_OTU_SO_RES14_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);

}/* otu_frm_sm_oh_set */


/*******************************************************************************
* otu_frm_sm_oh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the SO OH fields per requested dci channel(0..11).
*   This is a SO only function.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   oh_field            - enum,SO OH fields. Please see
*                         register  OTUk_So SM OH
*                         for details on each of the fields:
*                         OTU_SO_FAS_F6,
*                         OTU_SO_FAS_28,
*                         OTU_SO_MFAS,
*                         OTU_SO_SM_BIP8,
*                         OTU_SO_SM_BEI_BIAE,
*                         OTU_SO_SM_BDI,
*                         OTU_SO_SM_IAE,
*                         OTU_SO_SM_RES7,
*                         OTU_SO_SM_RES8,
*                         OTU_SO_GCC0,
*                         OTU_SO_RES13,
*                         OTU_SO_RES14,
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*   
* NOTES:
*  1. This function requires user to set the dci channel to passthrough mode
*     mode before function invocation if configuring an SO control field. failure to do
*     so will result in unexpected failures and alarms triggered. \n
*  2. refers to registers OTUk_SK_SM_CTL,OTUk_SK_PASSTHRU,OTUk_So_SM_INS_CTRL,
*     OTUk_So_CTRL, OTUk_So_MASTER_CTRL
*
*******************************************************************************/
PUBLIC UINT32  otu_frm_sm_oh_get( otu_frm_handle_t    *otu_frm_handle,
                                  UINT8               dci_channel,
                                  otu_frm_sm_oh_t     oh_field)
{
    UINT32 sm_oh_field = OTU_FRM_DEFECT_ERROR;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* Set operation can only be done on SO */
    switch (oh_field)
    {
    case OTU_SO_FAS_F6:
        sm_oh_field = otu_frm_tx_reg_OTU_SO_FAS_F6_read(NULL,otu_frm_handle);
        break;
    
    case OTU_SO_FAS_28:
        sm_oh_field = otu_frm_tx_reg_OTU_SO_FAS_28_read(NULL,otu_frm_handle);
        break;

    case OTU_SO_MFAS:
        sm_oh_field = otu_frm_tx_field_OTU_SO_MFAS_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_BIP8:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_BIP8_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_BEI_BIAE:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_BDI:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_BDI_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_IAE:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_IAE_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_RES7:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_RES7_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_SM_RES8:
        sm_oh_field = otu_frm_tx_field_OTU_SO_SM_RES8_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_GCC0:
        sm_oh_field = otu_frm_tx_field_OTU_SO_GCC0_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_RES13:
        sm_oh_field = otu_frm_tx_field_OTU_SO_RES13_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SO_RES14:
        sm_oh_field = otu_frm_tx_field_OTU_SO_RES14_get(NULL,otu_frm_handle,dci_channel);
        break;

    default:
        sm_oh_field = OTU_FRM_DEFECT_ERROR;
        break;

    }

    PMC_RETURN(sm_oh_field);

}/* otu_frm_sm_oh_get */


/*******************************************************************************
* otu_frm_sm_deg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets SK SM DEG fields per requested dci channel(0..11).
*   This is a SK only function.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   deg_field           - enum,SK DEG fields. Please see
*                         register OTUk_SK - OTUk SK SM DEG THR
*                         for details on each of the fields:
*                         OTU_SK_SM_DEG_M,
*                         OTU_SK_SM_DEG_THR
*
*   value               - UINT32, 0..32 bit values per field bits.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*  refers to register OTUk SK SM DEG THR
*
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_deg_set( otu_frm_handle_t       *otu_frm_handle,
                                     UINT8               dci_channel,
                                     otu_frm_sk_sm_deg_t deg_field,
                                     UINT32              value )
{
    PMC_ERROR rc = OTU_FRM_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* Set operation can only be done on so */
    switch (deg_field)
    {
    case OTU_SK_SM_DEG_M:
        if( 0 == (value >> 4) ) /* 4 bit value */
        {
            otu_frm_rx_field_OTU_SK_SM_DEG_M_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    case OTU_SK_SM_DEG_THR:
        if( 0 == (value >> 20) ) /* 20 bit value */
        {
            otu_frm_rx_field_OTU_SK_SM_DEG_THR_set(NULL,otu_frm_handle,dci_channel,value);
            rc = PMC_SUCCESS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/*  otu_frm_sm_deg_set */


/*******************************************************************************
* otu_frm_sm_deg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets SK SM DEG fields per requested dci channel(0..11).
*   This is a SK only function.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   deg_field           - enum,SK DEG fields. Please see
*                         register   OTUk_SK - OTUk SK SM DEG THR
*                         for details on each of the fields:
*                         OTU_SK_SM_DEG_M,
*                         OTU_SK_SM_DEG_THR,
*
* OUTPUTS:
*   *value              - pointer to pre allocated UINT32, 0..32 bit values per field bits.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
* 
* NOTES:
*  refers to register OTUk SK SM DEG THR
*
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_sm_deg_get( otu_frm_handle_t       *otu_frm_handle,
                                     UINT8               dci_channel,
                                     otu_frm_sk_sm_deg_t deg_field,
                                     UINT32              *value )
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(value!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* Set operation can only be done on so */
    switch (deg_field)
    {
    case OTU_SK_SM_DEG_M:
        *value = otu_frm_rx_field_OTU_SK_SM_DEG_M_get(NULL,otu_frm_handle,dci_channel);
        break;

    case OTU_SK_SM_DEG_THR:
        *value = otu_frm_rx_field_OTU_SK_SM_DEG_THR_get(NULL,otu_frm_handle,dci_channel);
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/*  otu_frm_sm_deg_get */


/*******************************************************************************
* otu_frm_FEC_degap_init_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the FEC degap registers for the first time after reset.
*   The function will set the following parameters per all dsi streams:
*   - Fifo Empty/Full thresholds
*   - Initial Delta Sigma Numerator/Denominator for a 12x10G configuration
*   - Smoothing filter activation per user configuration\n
*   The function will enable all slices, wait for 2 msec for the configuration
*   to take affect and then disable all slices.
*   Slices will be enabled one by one when the user will provision relevant
*   dsi streams.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   fifo_full_thresh    - unsigned, (0..133), user set value
*
*   fifo_empty_thresh   - unsigned, (35..fifo_full_thresh), user set value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*  1. This function should be used only once after reset to set the initial
*     register values for the FEC degap block.
*
*******************************************************************************/
PUBLIC void otu_frm_fec_degap_init_set ( otu_frm_handle_t *otu_frm_handle,
                                         UINT8 fifo_full_thresh,
                                         UINT8 fifo_empty_thresh )
{
    UINT8 i;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fifo_full_thresh < OTUDG_FIFO_SIZE,OTU_FRM_ERR_INVALID_PARAMETERS,fifo_full_thresh,0);
    PMC_ASSERT(fifo_empty_thresh >= OTUDG_FIFO_EMPTY_THRS,OTU_FRM_ERR_INVALID_PARAMETERS,fifo_empty_thresh,0);
    PMC_ASSERT(fifo_empty_thresh < fifo_full_thresh,OTU_FRM_ERR_INVALID_PARAMETERS,fifo_empty_thresh,0);

    /* Disable all slices  */
    for( i=0 ; i < MAX_10G_STREAM_NUM ; i++ )
    {
        otudg_field_SLICE_EN_set(NULL,otu_frm_handle,i,0);
    }

    /* Set FIFO threshold values */
    otudg_field_CENTER_THRES_set(NULL,otu_frm_handle,fifo_empty_thresh);
    otudg_field_A_FULL_THRES_set(NULL,otu_frm_handle,fifo_full_thresh);

    /* set filter enable and delta sigma parameters per slice    */
    /* Initial setting is of 12x10G                             */
    for( i=0 ; i < MAX_10G_STREAM_NUM ; i++ )
    {
        otu_frm_dsm_set(otu_frm_handle,i,RATE_10G);
    }


    PMC_RETURN();

}/* otu_frm_fec_degap_init_set */


/*******************************************************************************
* otu_frm_stream_fec_degap_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the FEC degap parameters per
*   dsi stream during stream provisioning/deprovisioning.
*   Values set are the delta sigma numerator/denominator and the smoothing
*   filter enable/disable bit.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dsi_stream          - UINT8, requested stream number (0..11)
*
*   lane_rate           - enum, requested data rate:
*                         RATE_10G,
*                         RATE_40G,
*                         RATE_100G,
*
*   enable_smoothing    - enum, SET_BIT : enables smoothing filter;
*                              RESET_BIT : disables smoothing filter
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. This function configures the FEC degap register values however it leaves
*      the FEC degap in a disabled state. to enable FEC degap mechanism please
*      use the otu_frm_FEC_degap_enable function.
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_stream_fec_degap_set ( otu_frm_handle_t *otu_frm_handle,
                                                UINT8 dsi_stream,
                                                otu_frm_stream_rate_t lane_rate,
                                                otu_frm_bit_value_t enable_smoothing )
{
    PMC_ERROR rc = PMC_SUCCESS;

    UINT8 rx_clk_src = 0;
    UINT8 i = 0;
    UINT8 hw_adapt_mask = 0;
    UINT8 cfg_lane = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /*
      The simplest software vision is
      Internal RX_LINECLK[11]=RX_LINECLK[11] in 10G mode
      Internal RX_LINECLK[11]=RX_LINECLK[8] in 100G mode (this meets 100G OTL, 100G SFIS(N=10), 100G 2xSFIS(N=5))
      Internal RX_LINECLK[11]=RX_LINECLK_40G_SFI51[0] in 40G mode.

      The 100G 2xSFIS(N=5) mode may also use the RX_LINECLK[11], but I think to use RX_LINECLK[8]
      for any 100G mode is simpler for software.

      The overall clock table looks like this (take care of the column alignment):
      RX_LINECLK[11:0]     0  1  2  3  4  5  6  7  8  9  10  11
      10G                  0  1  2  3  4  5  6  7  8  9  10  11
      40G_OTL              0==1==2==3  4==5==6==7  8==9==10==11
      40G SFIS(N=4)        0==1==2==3==4  5==6==7==8==9
      100G OTL             0==1==2==3==4==5==6==7==8==9
      100G SFIS(N=10)      0==1==2==3==4==5==6==7==8==9==10
      100G 2xSFIS(N=5)     0==1==2==3==4==5==6==7==8==9==10==11
      40G SFI5.1           ====0=====  ====1=====  =====0======  Here the index refers to RX_LINECLK40G_SFI51[1:0]
    */

    /* map dsi lanes to FEC degap lanes */
    switch (lane_rate)
    {
    case RATE_10G:
        /* 10G - 1:1 mapping */
        cfg_lane = dsi_stream;
        hw_adapt_mask = (1<<(dsi_stream/4));

        /* configure rx line clk mux for streams 3,7,11             */
        /* all other lanes do not require mux output configuration  */
        /* in 10G mode. select source #0                            */
        if(3==dsi_stream || 7==dsi_stream || 11==dsi_stream)
        {
            otu_frm_rx_lineclk_mux_cfg(otu_frm_handle,dsi_stream,0);
            /* do not power on or take our of reset - will be done at the otu_frm_fec_degap_enable function */
        }

        otu_frm_handle->var.data_smoothing_enabled[cfg_lane]=(BOOL)enable_smoothing;
        break;

    case RATE_40G:
        /* 40G - dsi stream(0,1,2)--> lanes(3,7,11) mapping */
        PMC_ASSERT(dsi_stream< 3,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

        cfg_lane = dsi_stream*4 + 3;
        hw_adapt_mask = (1<<dsi_stream);

        /* configure rx line clk mux for streams 3,7,11                 */
        /* for 40G OTL/SFIS configurations we use lanes 3,7,11          */
        /* with source (#0).                                            */
        /* for SFI51 configuration we use lanes 3,7,11 with source (#1) */
        if(otu_frm_handle->var.otudg_sfi51_lanes_bm & (1<<dsi_stream))
            rx_clk_src = 1; /* SFI51 SRC */
        else
            rx_clk_src = 0; /* rx clk SRC */

        otu_frm_rx_lineclk_mux_cfg(otu_frm_handle,cfg_lane,rx_clk_src);

        /* do not power on or take our of reset - will be done at the otu_frm_fec_degap_enable function */

        for(i =(cfg_lane-3); i<=cfg_lane; i++)
            otu_frm_handle->var.data_smoothing_enabled[i]=(BOOL)enable_smoothing;
        break;


    case RATE_100G:
        /* 100G - dsi stream(0)--> lane(11) mapping */
        cfg_lane = 11;
        hw_adapt_mask = (1<<4); /*bit 4 is set for 100G cases */

        /* configure rx line clk mux for streams 11                     */
        /* for all 100G configurations we use lane 11 with rx_lineclk[8]*/
        /* as source (#2)                                               */

        otu_frm_rx_lineclk_mux_cfg(otu_frm_handle,11,2);

        /* do not power on or take our of reset - will be done at the otu_frm_fec_degap_enable function */

        for(i = 0; i<12 ; i++)
            otu_frm_handle->var.data_smoothing_enabled[i]=(BOOL)enable_smoothing;
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    /* re configure the FEC degap hw */
    if ( PMC_SUCCESS == rc )
    {
        /* first - disable slice */
        otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
        
        /* check if hw adapt is set for this dsi stream */
        otu_frm_dsm_set(otu_frm_handle,dsi_stream,lane_rate);

        /* slice will be enabled by otu_frm_FEC_degap_enable function */
    }

    PMC_RETURN(rc);
}/* otu_frm_stream_fec_degap_set */


/*******************************************************************************
* otu_frm_fec_degap_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables FEC degap hw per requested dsi stream.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dsi_stream          - UINT8, requested stream number (0..11)
*
*   lane_rate           - enum, requested data rate:
*                         RATE_10G,
*                         RATE_40G,
*                         RATE_100G
*
*   enable_fec_degap    - boolean, TRUE : enable FEC degap;
*                                  FALSE : disable FEC degap
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. User should call this function with enable_FEC_degap = TRUE only after
*      Configuring FEC degap parameters.
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_fec_degap_enable ( otu_frm_handle_t *otu_frm_handle,
                                            UINT8 dsi_stream,
                                            otu_frm_stream_rate_t lane_rate,
                                            BOOL enable_fec_degap )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 cfg_lane = 0;
    UINT8 rx_clk_src = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /*
      The simplest software vision is
      Internal RX_LINECLK[11]=RX_LINECLK[11] in 10G mode
      Internal RX_LINECLK[11]=RX_LINECLK[8] in 100G mode (this meets 100G OTL, 100G SFIS(N=10), 100G 2xSFIS(N=5))
      Internal RX_LINECLK[11]=RX_LINECLK_40G_SFI51[0] in 40G mode.

      The 100G 2xSFIS(N=5) mode may also use the RX_LINECLK[11], but I think to use RX_LINECLK[8]
      for any 100G mode is simpler for software.

      The overall clock table looks like this (take care of the column alignment):
      RX_LINECLK[11:0]     0  1  2  3  4  5  6  7  8  9  10  11
      10G                  0  1  2  3  4  5  6  7  8  9  10  11
      40G_OTL              0==1==2==3  4==5==6==7  8==9==10==11
      40G SFIS(N=4)        0==1==2==3==4  5==6==7==8==9
      100G OTL             0==1==2==3==4==5==6==7==8==9
      100G SFIS(N=10)      0==1==2==3==4==5==6==7==8==9==10
      100G 2xSFIS(N=5)     0==1==2==3==4==5==6==7==8==9==10==11
      40G SFI5.1           ====0=====  ====1=====  =====0======  Here the index refers to RX_LINECLK40G_SFI51[1:0]
    */

    /* map dsi lanes to FEC degap lanes */
    switch (lane_rate)
    {
    case RATE_10G:
        /* 10G - 1:1 mapping */
        cfg_lane = dsi_stream;

        /* configure rx line clk mux for streams 3,7,11             */
        /* all other lanes do not require mux output configuration  */
        /* in 10G mode                                              */
        if(enable_fec_degap)
        {
            if(3==dsi_stream || 7==dsi_stream || 11==dsi_stream)
            {
                otu_frm_rx_lineclk_mux_enable(otu_frm_handle,dsi_stream,0,SET_BIT);
            }
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,1);
        }
        else
        {
            if(3==dsi_stream || 7==dsi_stream || 11==dsi_stream)
            {
                otu_frm_rx_lineclk_mux_enable(otu_frm_handle,dsi_stream,0,RESET_BIT);
            }
            /* stay in low power */
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
        }
        break;

    case RATE_40G:
        /* 40G - dsi stream(0,1,2)--> lanes(3,7,11) mapping */
        cfg_lane = dsi_stream*4 + 3;

        /* configure rx line clk mux for streams 3,7,11                 */
        /* for 40G OTL/SFIS configurations we use lanes 3,7,11          */
        /* with source (#0).                                            */
        /* for SFI51 configuration we use lanes 3,7,11 with source (#1) */
        if(otu_frm_handle->var.otudg_sfi51_lanes_bm & (1<<dsi_stream))
            rx_clk_src = 1; /* SFI51 SRC */
        else
            rx_clk_src = 0; /* rx clk SRC */

        if(enable_fec_degap)
        {
            /* start the rx line clock for the dsi stream */
            otu_frm_rx_lineclk_mux_enable(otu_frm_handle,cfg_lane,rx_clk_src,SET_BIT);
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,1);
        }
        else
        {
            otu_frm_rx_lineclk_mux_enable(otu_frm_handle,cfg_lane,rx_clk_src,RESET_BIT);
            /* stay in low power */
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
        }
        break;

    case RATE_100G:
        /* 100G - dsi stream(0)--> lane(11) mapping */
        cfg_lane = 11;

        /* configure rx line clk mux for streams 11                     */
        /* for all 100G configurations we use lane 11 with rx_lineclk[8]*/
        /* as source (#2)                                               */
        if(enable_fec_degap)
        {
            /* start the rx line clock for the dsi stream */
            otu_frm_rx_lineclk_mux_enable(otu_frm_handle,11,2,SET_BIT);
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,1);
        }
        else
        {
            otu_frm_rx_lineclk_mux_enable(otu_frm_handle,11,2,RESET_BIT);
            /* stay in low power */
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/* otu_frm_fec_degap_enable */


/*******************************************************************************
* otu_frm_rx_lineclk_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the rx line clock mux per selected lane.
*   Configuration sets the OTU_FRM_RX_LINECLK_X_SEL field to the requested
*   Source while leaving all OTU_FRM_RX_LINECLK_x_SRCy_EN bits in a disabled
*   mode. To enable the OTU_FRM_RX_LINECLK_x_SRCy_EN please use the
*   otu_frm_rx_lineclk_mux_enable function.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*
*   rx_lineclk_sel      - UINT8, requested mux selector lane (3,7 or 11)
*
*   rx_lineclk_src      - UINT8, requested source:\n
*                         for lane #3 - 2 possible sources:
*                              0 : RX_LINECLK[3] input pin;
*                              1 : RX_LINECLK_40G_SFI51[0] input pin\n
*
*                         for lane #7 - 2 possible sources:
*                              0 : RX_LINECLK[7] input pin;
*                              1 : RX_LINECLK_40G_SFI51[1] input pin \n
*
*                         for lane #11 - 3 possible sources: \n
*                              0 : RX_LINECLK[11] input pin;
*                              1 : RX_LINECLK_40G_SFI51[0] input pin;
*                              2 : RX_LINECLK[8] input pin input pin
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   1. refers to register: OTU_FRM RX_LINECLK MUX \n
*   2. To enable the OTU_FRM_RX_LINECLK_x_SRCy_EN please use the
*      otu_frm_rx_lineclk_mux_enable function.
* 
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_rx_lineclk_mux_cfg(otu_frm_handle_t *otu_frm_handle,
                                            UINT8 rx_lineclk_sel,
                                            UINT8 rx_lineclk_src)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT( (rx_lineclk_sel == 3 ||
                 rx_lineclk_sel == 7 ||
                 rx_lineclk_sel == 11),OTU_FRM_ERR_INVALID_PARAMETERS,rx_lineclk_sel,0);

    PMC_ASSERT( rx_lineclk_src < 3 ,OTU_FRM_ERR_INVALID_PARAMETERS,rx_lineclk_src,0);

    switch(rx_lineclk_sel)
    {
    case 3:
        if( rx_lineclk_src < 2 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set(NULL,otu_frm_handle,rx_lineclk_src);
        }
        else
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    case 7:
        if( rx_lineclk_src < 2 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set(NULL,otu_frm_handle,rx_lineclk_src);
        }
        else
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    case 11:
        if( rx_lineclk_src < 3 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set(NULL,otu_frm_handle,rx_lineclk_src);
        }
        else
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* otu_frm_rx_lineclk_mux_cfg */


/*******************************************************************************
* otu_frm_rx_lineclk_mux_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables the rx line clock mux per selected
*   source and lane.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   rx_lineclk_sel      - UINT8, requested mux selector lane (3,7 or 11)
*
*   rx_lineclk_src      - UINT8, requested source: \n
*                         for lane #3 : 2 possible sources:
*                              0 : RX_LINECLK[3] input pin;
*                              1 : RX_LINECLK_40G_SFI51[0] input pin\n
*
*                         for lane #7 - 2 possible sources:
*                              0 : RX_LINECLK[7] input pin;
*                              1 : RX_LINECLK_40G_SFI51[1] input pin\n
*
*                         for lane #11 - 3 possible sources:
*                              0 : RX_LINECLK[11] input pin;
*                              1 : RX_LINECLK_40G_SFI51[0] input pin;
*                              2 : RX_LINECLK[8] input pin input pin
*
*   enable_source       - enum, SET_BIT : enables selected source;
*                               RESET_BIT : disables selected source
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   refers to register: OTU_FRM RX_LINECLK MUX
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_rx_lineclk_mux_enable(otu_frm_handle_t *otu_frm_handle,
                                               UINT8 rx_lineclk_sel,
                                               UINT8 rx_lineclk_src,
                                               otu_frm_bit_value_t enable_source)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    
    PMC_ASSERT( (rx_lineclk_sel == 3 ||
                 rx_lineclk_sel == 7 ||
                 rx_lineclk_sel == 11),OTU_FRM_ERR_INVALID_PARAMETERS,rx_lineclk_sel,0);

    PMC_ASSERT( rx_lineclk_src < 3 ,OTU_FRM_ERR_INVALID_PARAMETERS,rx_lineclk_src,0);

    switch(rx_lineclk_sel)
    {
    case 3:
        if( rx_lineclk_src < 2 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set(NULL,otu_frm_handle,0);

            /* enable the requested source */
            if(enable_source)
            {
                if(0 == rx_lineclk_src)
                    otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set(NULL,otu_frm_handle,1);
                else
                    otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set(NULL,otu_frm_handle,1);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    case 7:
        if( rx_lineclk_src < 2 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set(NULL,otu_frm_handle,0);

            /* enable the requested source */
            if(enable_source)
            {
                if(0 == rx_lineclk_src)
                    otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set(NULL,otu_frm_handle,1);
                else
                    otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set(NULL,otu_frm_handle,1);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    case 11:
        if( rx_lineclk_src < 3 )
        {
            /* shut down all sources */
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set(NULL,otu_frm_handle,0);
            otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set(NULL,otu_frm_handle,0);

            /* enable the requested source */
            if(enable_source)
            {
                if(0 == rx_lineclk_src)
                    otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set(NULL,otu_frm_handle,1);
                else if(1 == rx_lineclk_src)
                    otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set(NULL,otu_frm_handle,1);
                else
                    otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set(NULL,otu_frm_handle,1);
            }
        }
        else
        {
            rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        }
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* otu_frm_rx_lineclk_mux_enable */


/*******************************************************************************
* otu_frm_channel_dsc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns arrays with RX/TX channel status, lane rate, and DCI channel.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*   
* OUTPUTS:
*   *rx_act                 - list of RX channel status: ON or OFF
*   *tx_act                 - list of TX channel status: ON or OFF
*   *lane_rate              - list of channel rate
*   *dci_channel            - list of active DCI channels 
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_channel_dsc_get( otu_frm_handle_t             *otu_frm_handle,
                                          BOOL8                        *rx_act,
                                          BOOL8                        *tx_act,
                                          otu_frm_stream_rate_t        *lane_rate,
                                          UINT8                        *dci_channel)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 dsi_stream;   
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);

    if (0 != otu_frm_field_OTU_FRM_SW_RESET_get(NULL, otu_frm_handle) ||
        0 != otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_get(NULL, otu_frm_handle) ||
        0 != otu_frm_field_OTU_FRM_SW_LOWPWR_get(NULL, otu_frm_handle))        
    {
        rc = OTU_FRM_ERR_FAIL;
    }
    
    if (PMC_SUCCESS == rc)
    {
        for (itr = 0; itr < MAX_10G_STREAM_NUM; itr++)
        {
            lane_rate[itr] = LAST_RATE;
            dci_channel[itr] = 0x7F;
            rx_act[itr] = FALSE;
            tx_act[itr] = FALSE;
            dsi_stream = itr;

            /* retrieve interleaver/deinterleaver status */
            if (dsi_stream == 0)
            {            
                if (1 == otu_frm_field_OTU_FRM_10G_INTLV_EN_get(NULL, otu_frm_handle, 0) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream))
                {
                    lane_rate[itr] = RATE_10G; 
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 0);
                    rx_act[dsi_stream] = TRUE;
                }

                if (1 == otu_frm_field_OTU_FRM_40G_INTLV_EN_get(NULL,otu_frm_handle, 0) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream + 3))
                {
                    lane_rate[itr] = RATE_40G;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream + 3);
                    rx_act[dsi_stream] = TRUE;
                }
                if (1 == otu_frm_field_OTU_FRM_100G_INTLV_EN_get(NULL,otu_frm_handle) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 11))
                {
                    lane_rate[itr] = RATE_100G;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 11);
                    rx_act[dsi_stream] = TRUE;
                }

                if (1 == otu_frm_field_OTU_FRM_10G_DINTLV_EN_get(NULL, otu_frm_handle, 0) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream))
                {                
                    tx_act[dsi_stream] = TRUE;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 0);
                }
                if (1 == otu_frm_field_OTU_FRM_40G_DINTLV_EN_get(NULL, otu_frm_handle, 0) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream + 3))
                {                
                    tx_act[dsi_stream] = TRUE;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream + 3);
                }   
                if (1 == otu_frm_field_OTU_FRM_100G_DINTLV_EN_get(NULL, otu_frm_handle) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 11))
                {                
                    tx_act[dsi_stream] = TRUE;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 11);
                }
            }
            else if (0 == (dsi_stream % 4))
            { 
                if (1 == otu_frm_field_OTU_FRM_10G_INTLV_EN_get(NULL,otu_frm_handle, dsi_stream) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream))
                {
                    lane_rate[itr] = RATE_10G;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
                    rx_act[dsi_stream] = TRUE;
                }   
                if (1 == otu_frm_field_OTU_FRM_40G_INTLV_EN_get(NULL,otu_frm_handle, dsi_stream / 4) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 3 + dsi_stream))
                {
                    lane_rate[itr] = RATE_40G;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 3 + dsi_stream);
                    rx_act[dsi_stream] = TRUE;
                }

                if (1 == otu_frm_field_OTU_FRM_10G_DINTLV_EN_get(NULL, otu_frm_handle, dsi_stream)&&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream))
                {                
                    tx_act[dsi_stream] = TRUE;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
                }
                if (1 == otu_frm_field_OTU_FRM_40G_DINTLV_EN_get(NULL, otu_frm_handle, dsi_stream / 4) &&
                    FREE_CHANNEL != otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 3 + dsi_stream))
                {                
                    tx_act[dsi_stream] = TRUE;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, 3 + dsi_stream);
                }   
            }
            else
            {
                if (1 == otu_frm_field_OTU_FRM_10G_INTLV_EN_get(NULL,otu_frm_handle, dsi_stream))
                {
                    lane_rate[itr] = RATE_10G;
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
                    rx_act[dsi_stream] = TRUE;
                }
                if (1 == otu_frm_field_OTU_FRM_10G_DINTLV_EN_get(NULL, otu_frm_handle, dsi_stream))
                {                
                    dci_channel[itr] = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
                    tx_act[dsi_stream] = TRUE;
                }   
            }        
        }
    }

    PMC_RETURN(rc);
}/* otu_frm_channel_dsc_get */


/*******************************************************************************
* otu_frm_db_chnl_entry_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function audit ODUKSC SCBS3 calendar of a specified link.
*
* INPUTS:
*   *otu_frm_handle         - pointer to OTU FRAMER handle instance
*   *scbs3_page_ptr         - SCBS3 page to audit.
*    chnl                   - channel identifier
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_db_chnl_entry_audit(otu_frm_handle_t     *otu_frm_handle,
                                             scbs3_page_t         *scbs3_page_ptr,
                                             UINT32                chnl)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               num_of_recs = 0;
    line_core_otn_db_key lineotn_db_key;
    mapotn_db_key        sysotn_mode_db_key;
    void                *key;
    line_core_otn_db_query_res  *lineotn_q_result;
    mapotn_db_query_res         *sysotn_q_result;
    void                        *q_result;
    UINT32               itr;
    UINT32               cal_itr;
    UINT32               db_handle;
    UINT32               cal_entries[OTU_FRM_ODUKSC_NUM_DB_ENTRIES_MAX];
    UINT32               num_cal_entries = 0;
    util_gen_db_id_t     db_id;
            
    PMC_ENTRY();
    
    if (TRUE == otu_frm_handle->pop_ho_db)
    {
        lineotn_db_key.calendar_entry = DO_NOT_USE_KEY;
        lineotn_db_key.ho_channel = chnl;
        key = (void *)&lineotn_db_key;
        db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
        db_handle = otu_frm_handle->var.lineotn_db_handle_rx;
    }
    else
    {
        sysotn_mode_db_key.calendar_entry = DO_NOT_USE_KEY;
        sysotn_mode_db_key.channel_num = chnl;
        key = (void *)&sysotn_mode_db_key;
        db_id = UTIL_GEN_DB_LINEOTN_SYSOTN_MODE;
        db_handle = otu_frm_handle->var.lineotn_db_handle_sysotn_mode;
    }

    /* check that there are some calendar entries associated to chnl */
    num_cal_entries = gen_db_query_count(&(otu_frm_handle->base),
                                         db_id,
                                         db_handle,
                                         (void *)key);
    
    if (num_cal_entries == 0)
    {
        result = OTU_FRM_ERR_CONTEXT_CLEANUP;
    }
                  
    num_cal_entries = 0;
    /* query all calendar entries belonging to chnl*/
    for (itr = 0; itr < OTU_FRM_ODUKSC_NUM_DB_ENTRIES_MAX && PMC_SUCCESS == result; itr++)
    {
        /* query rd_channel_id data for the iteration */
        lineotn_db_key.calendar_entry = itr;
        lineotn_db_key.ho_channel = DO_NOT_USE_KEY;
        sysotn_mode_db_key.calendar_entry = itr;
        sysotn_mode_db_key.channel_num = DO_NOT_USE_KEY;

        num_of_recs = gen_db_query(&(otu_frm_handle->base),
                                   db_id,
                                   db_handle,
                                   (void *)key,
                                   (void **)&q_result);
        if (num_of_recs > 0)
        {
            if (TRUE == otu_frm_handle->pop_ho_db)
            {
                lineotn_q_result = (line_core_otn_db_query_res*)q_result;
                if (lineotn_q_result->p_recs[0]->ho_channel == chnl)
                {
                    cal_entries[num_cal_entries] = itr;
                    num_cal_entries += 1;
                }
            }
            else
            {
                sysotn_q_result = (mapotn_db_query_res*)q_result;
                if (sysotn_q_result->p_recs[0]->channel_num == chnl)
                {
                    cal_entries[num_cal_entries] = itr;
                    num_cal_entries += 1;
                }
            }
        }
        else
        {
            result = OTU_FRM_ERR_CONTEXT_CLEANUP;
        }
    }


    /* compare register based calendar slot against database entries */
    for (cal_itr = 0; cal_itr < num_cal_entries && PMC_SUCCESS == result; cal_itr++)
    {
        if (chnl != scbs3_page_ptr->calendar[cal_entries[cal_itr]])
        {
            PMC_LOG_TRACE("%d - cal_entries[%d]= %d\n", num_cal_entries, cal_itr, cal_entries[cal_itr]);
            PMC_LOG_TRACE("in %s, compare chnl(%d) == calendar(%d)\n", __FUNCTION__, chnl, scbs3_page_ptr->calendar[cal_entries[cal_itr]]);
            result = OTU_FRM_ERR_CONTEXT_CLEANUP;
        }
    }

    PMC_RETURN(result);

} /* otu_frm_db_chnl_entry_audit */


/*******************************************************************************
* otu_frm_db_dci_chnl_num_entries_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of calendar entries for a channel and a 
*   database 
*
* INPUTS:
*   *otu_frm_handle          - pointer to OTU_FRM handle instance
*                              it is a SYSOTN SS.
*   dci_chnl                 - DCI channel ID
*   is_rx_chnl               - when TRUE Rx database is checked otherwise TX 
*
* OUTPUTS:
*   *num_of_recs_ptr  - number of calendar entries required by the channel
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_db_dci_chnl_num_entries_get(otu_frm_handle_t    *otu_frm_handle,
                                                     UINT32               dci_chnl,
                                                     BOOL8                is_rx_chnl,
                                                     UINT32              *num_of_recs_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    line_core_otn_db_key    key;
    util_gen_db_id_t   db_id;
    UINT32 db_handle;

    PMC_ENTRY();
 
    PMC_ASSERT(otu_frm_handle != NULL, OTU_FRM_ERR_INVALID_PTR, 0 , 0);
    PMC_ASSERT(num_of_recs_ptr != NULL, OTU_FRM_ERR_INVALID_PTR, 0 , 0);

    /* if data base does not exist return an error */
    if (TRUE == otu_frm_handle->pop_ho_db)
    {
        if (TRUE == is_rx_chnl)
        {
            db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            db_handle = otu_frm_handle->var.lineotn_db_handle_rx;
        }
        else
        {        
            db_id = UTIL_GEN_DB_LINE_CORE_OTN_TX_DB;
            db_handle = otu_frm_handle->var.lineotn_db_handle_tx;
        }

        /* query rd_channel_id data for the schd_addr */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = dci_chnl;
        *num_of_recs_ptr = gen_db_query_count(&(otu_frm_handle->base),
                                              db_id, 
                                              db_handle, 
                                              (void *)&key);
        
        if (*num_of_recs_ptr == 0)
        {
            PMC_RETURN(OTU_FRM_DB_ENTRY_NOT_FOUND);
        }
    }
    else
    {
        db_id = UTIL_GEN_DB_LINEOTN_SYSOTN_MODE;
        db_handle = otu_frm_handle->var.lineotn_db_handle_sysotn_mode;
        
        /* query rd_channel_id data for the schd_addr */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = dci_chnl;
        *num_of_recs_ptr = gen_db_query_count(&(otu_frm_handle->base),
                                              db_id, 
                                              db_handle, 
                                              (void *)&key);
        
        if (*num_of_recs_ptr == 0)
        {
            PMC_RETURN(OTU_FRM_DB_ENTRY_NOT_FOUND);
        }
         
    }

    PMC_RETURN(result);
} /* otu_frm_db_dci_chnl_num_entries_get */


/*******************************************************************************
*  otu_frm_ssf_assertion_on_intlv_fifo_empty_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the hardware function which causes SSF to be
*   generated when the Interleaver FIFO goes empty.  Note that this control
*   is common to all channels.
*
* INPUTS:
*   *otu_frm_handle          - pointer to OTU_FRM handle instance
*   enable                   - Enable (TRUE) or disable SSF assertion on FIFO empty
*
* OUTPUTS:
*   None
* 
* RETURNS:
*   None
* 
* NOTES:
*
*******************************************************************************/


PUBLIC void otu_frm_ssf_assertion_on_intlv_fifo_empty_set (otu_frm_handle_t *otu_frm_handle, 
                                                                BOOL8 enable) 
{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle != NULL, OTU_FRM_ERR_INVALID_PTR, 0 , 0);

    if (TRUE == enable)
    {
        otu_frm_field_range_OTU_FRM_SPARE_set(NULL, otu_frm_handle, 31, 31, 1);
    } 
    else 
    {
        otu_frm_field_range_OTU_FRM_SPARE_set(NULL, otu_frm_handle, 31, 31, 0);
    }
    PMC_RETURN();

} /* otu_frm_ssf_assertion_on_intlv_fifo_empty_set */


/*
** Private Functions
*/

/*******************************************************************************
* otu_frm_master_cfg_bits_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to OTU_FRM_MASTER_CFG register bits.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   sw_cfg_bit              - enum, requested OTU_FRM_MASTER_CFG bit:
*           SW_RESET            : bit 31;
*           SW_LOW_POWER        : bit 0;
*           SW_RAM_LOW_POWER    : bit 1
*
*   sw_cfg_set_value        - enum,requested bit value:
*           SET_BIT             : 1;
*           RESET_BIT           : 0
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   refers to register OTU_FRM_MASTER_CFG
*
*******************************************************************************/
PRIVATE PMC_ERROR otu_frm_master_cfg_bits_set( otu_frm_handle_t                 *otu_frm_handle,
                                               otu_frm_sw_pwr_cfg_bit_t     sw_cfg_bit,
                                               otu_frm_bit_value_t          sw_cfg_set_value)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL, OTU_FRM_ERR_INVALID_PTR,0,0);

    /* set high order bits first */
    switch ( sw_cfg_bit )
    {
    case SW_RESET:
        otu_frm_field_OTU_FRM_SW_RESET_set(NULL,otu_frm_handle,sw_cfg_set_value);
        break;

    case SW_LOW_POWER:
        otu_frm_field_OTU_FRM_SW_LOWPWR_set(NULL,otu_frm_handle,sw_cfg_set_value);
        break;

    case SW_RAM_LOW_POWER:
        otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set(NULL,otu_frm_handle,sw_cfg_set_value);
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
} /* otu_frm_master_cfg_bits_set */


/*******************************************************************************
* otu_frm_sm_sk_glbl_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to set bits in OTUk_SK_CFG register.
*   This is a SK only function.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   gc_field                - enum, requested global config field:
*                             OTU_SK_AC_TTI_FIELD   :   bit 2;
*                             OTU_SK_TTI_LOCK       :   bit 1;
*                             OTU_SK_LOCK           :   bit 0
*
*   value                   - enum,requested bit value:
*                             SET_BIT               : 1;
*                             RESET_BIT             : 0
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   refers to register OTUk_SK_CFG
*
*******************************************************************************/
PRIVATE PMC_ERROR otu_frm_sm_sk_glbl_cfg_set( otu_frm_handle_t    *otu_frm_handle,
                                              otu_frm_sm_sk_gc_t  gc_field,
                                              otu_frm_bit_value_t value )

{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    switch(gc_field)
    {
    case OTU_SK_AC_TTI_FIELD:
        otu_frm_rx_field_OTU_SK_AC_TTI_set(NULL,otu_frm_handle,value);
        break;

    case OTU_SK_TTI_LOCK:
        otu_frm_rx_field_OTU_SK_TTI_LOCK_set(NULL,otu_frm_handle,value);
        break;

    case OTU_SK_LOCK:
        otu_frm_rx_field_OTU_SK_LOCK_set(NULL,otu_frm_handle,value);
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);

}/* otu_frm_sm_sk_glbl_cfg_set */


/*******************************************************************************
* otu_frm_dsm_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   sets the otu_frm data smoothing Delta Sigma modulation numerator and denominator
*   values per stream (0..11).
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dsi_stream              - UINT8, requested stream number (0..11)
*
*   stream_rate             - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   refers to registers: OTU_FRM_DS_DSM_M,  OTU_FRM_DS_DSM_N
*
*******************************************************************************/
PRIVATE PMC_ERROR otu_frm_dsm_set( otu_frm_handle_t      *otu_frm_handle,
                                   UINT8                  dsi_stream,
                                   otu_frm_stream_rate_t  stream_rate )

{
    UINT8       dsm_lane    = 0;
    PMC_ERROR   rc          = PMC_SUCCESS;
    UINT32      factor_N    = 1;
    UINT32      factor_M    = 1;
    UINT32      factor_N1   = 1;
    UINT32      factor_M1   = 1;
    UINT32      ds_den      = 255;
    UINT32      ds_num      = 239;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* ODUk rate/RX line rate= ds_num/ds_den=(ODUk columns/OTUk columns)*(RX word size/DCI word size)*(OTUk rate/RX line rate)  */
    /* ds_num = (ODUk columns) * (RX word size) * (OTUk rate)                                                                   */
    /* ds_den = (OTUk columns) * (DCI word size) * (RX line rate)                                                               */
    /* (OTUk rate/RX line rate) ==> (hw_factor_N/hw_factor_M) ;                                                                 */
    switch(stream_rate)
    {
    case RATE_10G:
        /* hw factor is always 1 here for both M and N*/
        dsm_lane = dsi_stream;      /* maps 1:1     */
        ds_num = (239*1)* factor_N ;
        ds_den = (255*12)*factor_M ;

        break;

    case RATE_40G:

        dsm_lane = (dsi_stream*4) + 3;  /* dsi ch 0 --> dsm lane 3,dsi ch 1 --> dsm lane 7,dsi ch 2 --> dsm lane 11 */
        ds_num = (239*1)* factor_N * factor_N1;
        ds_den = (255*3)* factor_M * factor_M1;
        break;

    case RATE_100G:

        dsm_lane = 11;  /* hardcoded map to lane 11     */
        ds_num = (239*320)* factor_N ;
        ds_den = (255*384)* factor_M ;
        break;

    default:
        rc = OTU_FRM_ERR_INVALID_PARAMETERS;
        break;

    }

    if(PMC_SUCCESS == rc )
    {        
        otudg_field_DSM_DEN_set(NULL, otu_frm_handle, dsm_lane, ds_den);
        otudg_field_DSM_NUM_set(NULL, otu_frm_handle, dsm_lane, ds_num);
    }

    PMC_RETURN(rc);
} /* otu_frm_dsm_set */



/*******************************************************************************
* otu_frm_passthru_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets a dci channel(0..11) into/out of passthru mode.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   sk_so_passtru       - enum,SK or SO passthru mode
*
*   value               - UINT8, 0 : set mode out of passthru;
*                                1 : set mode in passthru.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*  refers to registers OTUk_SK_PASSTHRU,OTUk_SO_PASSTHRU
*
*
*******************************************************************************/
PUBLIC void otu_frm_passthru_set( otu_frm_handle_t       *otu_frm_handle,
                                  UINT8               dci_channel,
                                  otu_frm_ctl_field_t sk_so_passtru,
                                  UINT8               value )

{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(value < 2,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);  /* max 1 bit values are allowed */

    /* check if passthru is need*/
    if ( OTU_SO_PASSTHRU == sk_so_passtru )
    {
        otu_frm_tx_field_OTU_SO_PASSTHRU_set(NULL,otu_frm_handle,dci_channel,value);
    }
    else if ( OTU_SK_PASSTHRU == sk_so_passtru )
    {
        otu_frm_rx_field_OTU_SK_PASSTHRU_set(NULL,otu_frm_handle,dci_channel,value);
    }

    PMC_RETURN();

}/* otu_frm_passthru_set */


/*******************************************************************************
* otu_frm_get_free_dci_ch_id
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Private function to find the next free dci channel.
*   associates a dsi channel to dci channel. If the dsi channel was
*   already allocated a dci channel, the allocated dci channel will be
*   returned.\n
*   Allocation is done based on the following policy:\n
*   1. 100G --> always allocated with dci channel 0\n
*   2. 40G dsi streams 0,1,2 --> always allocated with dci channel 0,1,2\n
*   3. 10G dsi streams:\n
*      -dsi streams 0,1,2,3  --> dci channels 0,3,6,9
*      -dsi streams 4,5,6,7  --> dci channels 1,4,7,10
*      -dsi streams 8,9,10,11--> dci channels 2,5,8,11
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dsi_stream              - unsigned, dsi channel to be associated with
*                             the found dci channel.
*
*   dsi_rate                - enum, dsi data rate (10/40/100G)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT8 - free tx dci channel (0..11)
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT8 otu_frm_get_free_dci_ch_id ( otu_frm_handle_t    *otu_frm_handle,
                                           UINT8                dsi_stream,
                                           otu_frm_stream_rate_t  dsi_rate)
{
    UINT8 i;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* check that the dsi channel is not already allocated */
    i = otu_frm_find_dci_ch_id (otu_frm_handle,dsi_stream,dsi_rate);

    if( FREE_CHANNEL == i )
    {
        if(RATE_100G == dsi_rate)
        {
            /* always allocate 0 */
            i=0;
            if (FREE_STREAM == otu_frm_handle->var.dci_to_dsi_chan_ids[i])
            {
                otu_frm_handle->var.dci_to_dsi_chan_ids[i] = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
            }
            else
            {   /* blow up */
                PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,otu_frm_handle->var.dci_to_dsi_chan_ids[i],0);
            }
        }
        else if (RATE_40G == dsi_rate)
        {
            /* always allocate 0,1,2 to 40G 0,1,2 respectively */
            i = dsi_stream;
            if (FREE_STREAM == otu_frm_handle->var.dci_to_dsi_chan_ids[i])
            {
                otu_frm_handle->var.dci_to_dsi_chan_ids[i] = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
            }
            else
            {   /* blow up */
                PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,otu_frm_handle->var.dci_to_dsi_chan_ids[i],0);
            }
        }
        else /* 10G  */
        {
            /* dsi streams 0,1,2,3  --> dci channels 0,3,6,9     */
            /* dsi streams 4,5,6,7  --> dci channels 1,4,7,10    */
            /* dsi streams 8,9,10,11--> dci channels 2,5,8,11    */
            i = (dsi_stream/4) + (dsi_stream%4)*3;
            if(FREE_STREAM == otu_frm_handle->var.dci_to_dsi_chan_ids[i])
            {
                otu_frm_handle->var.dci_to_dsi_chan_ids[i] = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);
            }
            else
            {   /* blow up */
                PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,otu_frm_handle->var.dci_to_dsi_chan_ids[i],0);
            }
        }
    }

    PMC_RETURN(i);
}/* otu_frm_get_free_dci_ch_id */


/*******************************************************************************
* otu_frm_find_dci_ch_id
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to find dci channel based on a dsi channel associated with it.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   dsi_stream              - unsigned, dsi channel associated with
*                             the found dci channel.
*
*   dsi_rate                - enum, dsi data rate (10/40/100G)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT8 - dci channel (0..11) or FREE_CHANNEL (0xFF) if not found.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT8 otu_frm_find_dci_ch_id (otu_frm_handle_t *otu_frm_handle,
                                      UINT8 dsi_stream,
                                      otu_frm_stream_rate_t  dsi_rate)
{
    UINT8 i;
    UINT8 dci_chan = FREE_CHANNEL;
    UINT8 dsi_stream_to_find;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    /* translate dsi stream to 10G based dsi stream */
    dsi_stream_to_find = otu_frm_get_base_10G_dsi_stream(dsi_stream,dsi_rate);

    for(i=0;i<MAX_DCI_CHANNEL_NUM;i++)
    {
        if( dsi_stream_to_find == otu_frm_handle->var.dci_to_dsi_chan_ids[i] )
        {
            break;
        }
    }

    if( i < MAX_DCI_CHANNEL_NUM )
        dci_chan = i;

    PMC_RETURN(dci_chan);
}/* otu_frm_find_dci_ch_id */


/*******************************************************************************
* otu_frm_get_dci_channel_params
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private helper function to return several parameters associated with the dci channel
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*   dci_channel              - unsigned, dci_channel
*
* OUTPUTS:
*   dsi_rate               - enum, dsi data rate (10/40/100G)
*   oduk_type              - enum, ODUk Type
*
* RETURNS:
*   UINT8 - dsi stream (0..11) or FREE_CHANNEL if unprovisioned
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT8 otu_frm_get_dci_channel_params (otu_frm_handle_t *otu_frm_handle,
                                              UINT8 dci_channel,
                                              otu_frm_stream_rate_t  *dsi_rate,
                                              util_global_odukp_type_t *oduk_type)
{
    UINT8 dsi_stream;

    PMC_ENTRY();
    
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    
    dsi_stream = otu_frm_handle->var.dci_to_dsi_chan_ids[dci_channel];
    (*oduk_type) =  otu_frm_handle->var.dsi_oduk_type[dsi_stream];    
    (*dsi_rate) = otu_frm_oduk_rate_get(*oduk_type);
 
    PMC_RETURN(dsi_stream);
} /* otu_frm_get_dci_channel_params */


/*******************************************************************************
* otu_frm_reset_ram_regs
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to set all RAM based R/W registers to 0.
*   called upon block init to make sure we start RAM registers at a known state.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
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
PRIVATE void otu_frm_reset_ram_regs(otu_frm_handle_t    *otu_frm_handle)
{
    UINT8 i;
    UINT32 tti_data[16];

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    PMC_MEMSET(&tti_data[0],0,sizeof(tti_data));

    for(i=0;i<MAX_DCI_CHANNEL_NUM;i++)
    {
        otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set(NULL,otu_frm_handle,i,tti_data);

        otu_frm_tx_lfield_OTU_SO_SM_TTI_set(NULL,otu_frm_handle,i,tti_data);

        otu_frm_tx_field_OTU_SO_MFAS_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BIP8_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BDI_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_IAE_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_RES7_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_RES8_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_GCC0_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_RES13_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_RES14_set(NULL,otu_frm_handle,i,0);

        otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set(NULL,otu_frm_handle,i,0);
        otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set(NULL,otu_frm_handle,i,0);
    }

    PMC_RETURN();

}/* otu_frm_reset_ram_regs */


/*******************************************************************************
* otu_frm_oduk_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to translate oduk type to raw (10G,40G 100g) data rate.
*
* INPUTS:
*   oduk_type               - enum, oduk type definition
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   otu_frm_stream_rate_t   - enum: RATE_10G, RATE_40G, RATE_100G or
*                             LAST_RATE on error
*
* NOTES:
*
*******************************************************************************/
PRIVATE otu_frm_stream_rate_t otu_frm_oduk_rate_get(util_global_odukp_type_t oduk_type)
{
    otu_frm_stream_rate_t rate;

    PMC_ENTRY();

    switch(oduk_type)
    {
    case UTIL_GLOBAL_ODU1E:
    case UTIL_GLOBAL_ODU1E_ETRANS:
    case UTIL_GLOBAL_ODU1F:
    case UTIL_GLOBAL_ODU2:
    case UTIL_GLOBAL_ODU2E:
    case UTIL_GLOBAL_ODU2E_ETRANS:
    case UTIL_GLOBAL_ODU2F:
        rate = RATE_10G;
        break;

    case UTIL_GLOBAL_ODU3:
    case UTIL_GLOBAL_ODU3E1:
    case UTIL_GLOBAL_ODU3E2:
        rate = RATE_40G;
        break;

    case UTIL_GLOBAL_ODU4:
        rate = RATE_100G;
        break;

    default:
        rate= LAST_RATE;
        break;
    }

    PMC_RETURN(rate);
}/* otu_frm_oduk_rate_get */


/*******************************************************************************
* otu_frm_validate_rx_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to check if a rx dsi stream at a given rate can be provisioned.
*   the function checks the data against already configured dsi streams to make sure
*   we do not "over book" our resources.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   otu_rate                - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream              - unsigned, dsi stream to be provisioned.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE : the dsi stream can be provisioned as requested or 
*         FALSE : the dsi stream can not be provisioned as requested.
* 
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL otu_frm_validate_rx_prov(otu_frm_handle_t *otu_frm_handle,otu_frm_stream_rate_t otu_rate,UINT8 dsi_stream)
{
    BOOL    rc = TRUE;
    UINT8   mapped_40G_lane=0;
    UINT8   i;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
    case RATE_10G:
        /* Find the relevant 40G lane                   */
        /* 10G lanes 0..3  map to 40G lane 0            */
        /* 10G lanes 4..7  map to 40G lane 1            */
        /* 10G lanes 8..11 map to 40G lane 2            */
        mapped_40G_lane = dsi_stream / (MAX_10G_STREAM_NUM/MAX_40G_STREAM_NUM);

        /* check that 100G lane or the relevant 40G lanes or the requested 10G lane are not already configured */
        if((otu_frm_handle->var.otu_frm_100G_stream_cfg[0].rx_stream_state != STREAM_STATE_START) ||
           (otu_frm_handle->var.otu_frm_40G_stream_cfg[mapped_40G_lane].rx_stream_state != STREAM_STATE_START) ||  /* 40G lane */
           (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].rx_stream_state != STREAM_STATE_START) ) /* 10G lanes */
        {
            rc = FALSE;  /* error - disable other configuration before setting new one */
        }
        break;


    case RATE_40G:
        if( dsi_stream < MAX_40G_STREAM_NUM )
        {
            /* check that 100G lane or the requested 40G or separate 10G lanes are not already configured */
            /* dsi_stream is now 0..2 */
            if((otu_frm_handle->var.otu_frm_100G_stream_cfg[0].rx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].rx_stream_state != STREAM_STATE_START) ||  /* 40G lane */
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 0].rx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 1].rx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 2].rx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 3].rx_stream_state != STREAM_STATE_START) ) /* 10G lanes */
            {
                rc = FALSE;  /* error - disable other configuration before setting new one */
            }
        }
        else
        {
            rc = FALSE;
        }
        break;


    case RATE_100G:
        /* if at least one of the 10G 40G or 100G is in a state different from init --> fail */
        if (otu_frm_handle->var.otu_frm_100G_stream_cfg[0].rx_stream_state != STREAM_STATE_START)
            PMC_RETURN(FALSE);

        for(i=0; i < MAX_40G_STREAM_NUM; i++ )
            if( otu_frm_handle->var.otu_frm_40G_stream_cfg[i].rx_stream_state != STREAM_STATE_START )
                PMC_RETURN(FALSE);

        for(i=0; i < MAX_10G_STREAM_NUM; i++ )
            if( otu_frm_handle->var.otu_frm_10G_stream_cfg[i].rx_stream_state != STREAM_STATE_START )
                PMC_RETURN(FALSE);
        break;

    default:
        rc = FALSE;
        break;
    }

    PMC_RETURN(rc);
}/* otu_frm_validate_rx_prov */


/*******************************************************************************
* otu_frm_validate_tx_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to check if a tx dsi stream at a given rate can be provisioned.
*   the function checks the data against already configured dsi streams to make sure
*   we do not "over book" our resources.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   otu_rate                - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream              - unsigned, dsi stream to be provisioned.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE : the dsi stream can be provisioned as requested; or, 
*          FALSE : the dsi stream can not be provisioned as requested.
* 
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL otu_frm_validate_tx_prov(otu_frm_handle_t *otu_frm_handle,otu_frm_stream_rate_t otu_rate,UINT8 dsi_stream)
{
    BOOL    rc = TRUE;
    UINT8   mapped_40G_lane=0;
    UINT8   i;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
        /* 10G -> no multilane configuration register        */
        /* just update configuration storage                */
    case RATE_10G:
        /* Find the relevant 40G lane                   */
        /* 10G lanes 0..3  map to 40G lane 0            */
        /* 10G lanes 4..7  map to 40G lane 1            */
        /* 10G lanes 8..11 map to 40G lane 2            */
        mapped_40G_lane = dsi_stream / (MAX_10G_STREAM_NUM/MAX_40G_STREAM_NUM);

        /* check that 100G lane or the relevant 40G lanes or the requested 10G lane are not already configured */
        if((otu_frm_handle->var.otu_frm_100G_stream_cfg[0].tx_stream_state != STREAM_STATE_START) ||
           (otu_frm_handle->var.otu_frm_40G_stream_cfg[mapped_40G_lane].tx_stream_state != STREAM_STATE_START) ||  /* 40G lane */
           (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].tx_stream_state != STREAM_STATE_START) ) /* 10G lanes */
        {
            rc = FALSE;  /* error - disable other configuration before setting new one */
        }
        break;


    case RATE_40G:
        if( dsi_stream < MAX_40G_STREAM_NUM )
        {
            /* check that 100G lane or the requested 40G or separate 10G lanes are not already configured */
            /* dsi_stream is now 0..2 */
            if((otu_frm_handle->var.otu_frm_100G_stream_cfg[0].tx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].tx_stream_state != STREAM_STATE_START) ||  /* 40G lane */
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 0].tx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 1].tx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 2].tx_stream_state != STREAM_STATE_START) ||
               (otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream*4 + 3].tx_stream_state != STREAM_STATE_START) ) /* 10G lanes */
            {
                rc = FALSE;  /* error - disable other configuration before setting new one */
            }
        }
        else
        {
            rc = FALSE;
        }
        break;


    case RATE_100G:
        /* if at least one of the 10G 40G or 100G is in a state different from init --> fail */
        if (otu_frm_handle->var.otu_frm_100G_stream_cfg[0].tx_stream_state != STREAM_STATE_START)
            PMC_RETURN(FALSE);

        for(i=0; i < MAX_40G_STREAM_NUM; i++ )
            if( otu_frm_handle->var.otu_frm_40G_stream_cfg[i].tx_stream_state != STREAM_STATE_START )
                PMC_RETURN(FALSE);

        for(i=0; i < MAX_10G_STREAM_NUM; i++ )
            if( otu_frm_handle->var.otu_frm_10G_stream_cfg[i].tx_stream_state != STREAM_STATE_START )
                PMC_RETURN(FALSE);
        break;

    default:
        rc = FALSE;
        break;
    }

    PMC_RETURN(rc);
}/* otu_frm_validate_tx_prov */


/*******************************************************************************
* otu_frm_set_rx_stream_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to set rx dsi stream state
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream          - unsigned, dsi stream to change state
*
*   new_state           - enum, new state to set (STREAM_STATE_START,STREAM_STATE_EQUIPPED,
*                         STREAM_STATE_OPERATIONAL)
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
PRIVATE void otu_frm_set_rx_stream_state(otu_frm_handle_t           *otu_frm_handle,
                                         otu_frm_stream_rate_t  otu_rate,
                                         UINT8                  dsi_stream,
                                         otu_frm_str_state_t    new_state)
{
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(new_state< LAST_STR_STATE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
    case RATE_10G:
        otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].rx_stream_state = new_state;
        break;


    case RATE_40G:
        if(dsi_stream < MAX_40G_LANE_NUM)
        {
            otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].rx_stream_state = new_state;
        }
        else
            PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;


    case RATE_100G:
        otu_frm_handle->var.otu_frm_100G_stream_cfg[0].rx_stream_state = new_state;
        break;

    default:
        PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;
    }

    PMC_RETURN();
}/* otu_frm_set_rx_stream_state */


/*******************************************************************************
* otu_frm_set_tx_stream_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to set tx dsi stream state
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream          - unsigned, dsi stream to change state
*
*   new_state           - enum, new state to set (STREAM_STATE_START,STREAM_STATE_EQUIPPED,
*                         STREAM_STATE_OPERATIONAL)
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
PRIVATE void otu_frm_set_tx_stream_state(otu_frm_handle_t           *otu_frm_handle,
                                         otu_frm_stream_rate_t  otu_rate,
                                         UINT8                  dsi_stream,
                                         otu_frm_str_state_t    new_state)
{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(new_state< LAST_STR_STATE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
    case RATE_10G:
        otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].tx_stream_state = new_state;
        break;

    case RATE_40G:
        if(dsi_stream < MAX_40G_LANE_NUM)
        {
            otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].tx_stream_state = new_state;
        }
        else
        {
            PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        }
        break;

    case RATE_100G:
        otu_frm_handle->var.otu_frm_100G_stream_cfg[0].tx_stream_state = new_state;
        break;

    default:
        PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;
    }

    PMC_RETURN();
}/* otu_frm_set_tx_stream_state */


/*******************************************************************************
* otu_frm_get_rx_stream_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to retrieve rx dsi stream state
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream          - unsigned, dsi stream to change state
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   state               - enum, current state (STREAM_STATE_START,STREAM_STATE_EQUIPPED,
*                         STREAM_STATE_OPERATIONAL)
*
* NOTES:
*
*******************************************************************************/
PRIVATE otu_frm_str_state_t otu_frm_get_rx_stream_state(otu_frm_handle_t            *otu_frm_handle,
                                                        otu_frm_stream_rate_t   otu_rate,
                                                        UINT8                   dsi_stream)
{
    otu_frm_str_state_t state = LAST_STR_STATE;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
    case RATE_10G:
        state = otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].rx_stream_state;
        break;

    case RATE_40G:
        if(dsi_stream < MAX_40G_LANE_NUM)
            state = otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].rx_stream_state;
        else
            PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */

        break;

    case RATE_100G:
        state = otu_frm_handle->var.otu_frm_100G_stream_cfg[0].rx_stream_state;
        break;

    default:
        PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;
    }

    PMC_RETURN(state);
}/* otu_frm_get_rx_stream_state */


/*******************************************************************************
* otu_frm_get_tx_stream_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to retrieve tx dsi stream state
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*   dsi_stream          - unsigned, dsi stream to retrieve state for
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   state               - enum, current state (STREAM_STATE_START,STREAM_STATE_EQUIPPED,
*                         STREAM_STATE_OPERATIONAL)
*
* NOTES:
*
*******************************************************************************/
PRIVATE otu_frm_str_state_t otu_frm_get_tx_stream_state(otu_frm_handle_t            *otu_frm_handle,
                                                        otu_frm_stream_rate_t   otu_rate,
                                                        UINT8                   dsi_stream)
{
    otu_frm_str_state_t state = LAST_STR_STATE;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    switch (otu_rate)
    {
    case RATE_10G:
        state = otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].tx_stream_state;
        break;

    case RATE_40G:
        if(dsi_stream < MAX_40G_LANE_NUM)
            state = otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream].tx_stream_state;
        else
            PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */

        break;

    case RATE_100G:
        state = otu_frm_handle->var.otu_frm_100G_stream_cfg[0].tx_stream_state;
        break;

    default:
        PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;
    }

    PMC_RETURN(state);
}/* otu_frm_get_tx_stream_state */


/*******************************************************************************
* otu_frm_validate_dsi_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function validate dsi stream number with the associated rate.
*
* INPUTS:
*
*   dsi_stream          - unsigned, dsi stream to verify
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - TRUE : stream is within the required range; or
*          FALSE : stream is outside of the required range.
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL otu_frm_validate_dsi_stream(UINT8 dsi_stream,otu_frm_stream_rate_t otu_rate)
{
    BOOL rc = TRUE;
    PMC_ENTRY();

    switch (otu_rate)
    {
    case RATE_10G:
        if( dsi_stream >= MAX_10G_STREAM_NUM ) rc = FALSE;
        break;

    case RATE_40G:
        if( dsi_stream >= MAX_40G_STREAM_NUM ) rc = FALSE;
        break;

    case RATE_100G:
        if( dsi_stream >= MAX_100G_STREAM_NUM ) rc = FALSE;
        break;

    default:
        rc = FALSE;
        break;
    }

    PMC_RETURN(rc);
}/* otu_frm_validate_dsi_stream */


/*******************************************************************************
* otu_frm_get_base_10G_dsi_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to translate dsi stream number to 10G base dsi stream.
*
* INPUTS:
*
*   dsi_stream          - unsigned, dsi stream to translate
*
*   otu_rate            - enum, dsi stream rate (RATE_10G, RATE_40G, RATE_100G)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT8 - 10G based stream number:
*       for 10G rate --> dsi_stream \n
*       for 40G rate --> dsi_stream (0,1,2) --> (0,4,8) \n
*       for 100G rate --> 0
* 
* NOTES:
*
*******************************************************************************/
PRIVATE UINT8 otu_frm_get_base_10G_dsi_stream(UINT8 dsi_stream,otu_frm_stream_rate_t otu_rate)
{
    UINT8 dsi_stream_10G = FREE_LANE;
    PMC_ENTRY();

    switch (otu_rate)
    {
    case RATE_10G:
        if( dsi_stream < MAX_10G_STREAM_NUM ) dsi_stream_10G = dsi_stream;
        break;

    case RATE_40G:
        if( dsi_stream < MAX_40G_STREAM_NUM ) dsi_stream_10G = dsi_stream*4;
        break;

    case RATE_100G:
        dsi_stream_10G = 0;
        break;

    default:
        PMC_ASSERT(FALSE,OTU_FRM_ERR_INVALID_PARAMETERS,0,0); /* blow up !!! */
        break;
    }

    PMC_RETURN(dsi_stream_10G);
}/* otu_frm_validate_dsi_stream */


/*******************************************************************************
* otu_frm_db_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private function to update line/core OTN db
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   dci_channel         - unsigned,allocated high order channel (HO).
*
*   provision_db        - boolean, TRUE : function was called for provision purposes
*                                  FALSE : function was called for deprovision purposes
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR otu_frm_db_prov_update(otu_frm_handle_t *otu_frm_handle,
                                         UINT8            dci_channel,
                                         BOOL             provision_db)
{
    line_core_otn_db_rec db_record;
    line_core_otn_db_key db_key;
    mapotn_db_rec        sysotn_mode_db_record;
    mapotn_db_key        sysotn_mode_db_key;  
    UINT8                i = 0;
    PMC_ERROR            rc = PMC_SUCCESS;
    line_core_otn_db_rec * db_record_ptr = &db_record;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel<MAX_10G_LANE_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dci_channel,0);

    if(provision_db) /* add new records */
    {
        PMC_MEMSET(db_record_ptr,0xff,sizeof(db_record));

        for(i = 0; i<CALENDAR_ENTRIES_NUM;i++)
        {
            if(otu_frm_handle->oduksc_handle->scbs3_handle->util_schd_handle->var.schd_ctxt->chnl_ctxt[i].allocated_ho_channel == dci_channel)
            {
                if(otu_frm_handle->pop_ho_db)
                {
                    db_record.calendar_entry = i;
                    db_record.ho_channel = dci_channel;
                    rc = gen_db_add_record(&(otu_frm_handle->base),
                                           UTIL_GEN_DB_LINE_CORE_OTN_RX_DB ,
                                           otu_frm_handle->var.lineotn_db_handle_rx,
                                           (void *)&db_record);
                    if( PMC_SUCCESS == rc )
                        rc = gen_db_add_record(&(otu_frm_handle->base),
                                               UTIL_GEN_DB_LINE_CORE_OTN_TX_DB ,
                                               otu_frm_handle->var.lineotn_db_handle_tx,
                                               (void *)&db_record);
                }
                else
                {
                    sysotn_mode_db_record.calendar_entry = i;
                    sysotn_mode_db_record.channel_num = dci_channel;

                    rc = gen_db_add_record(&(otu_frm_handle->base),
                                           UTIL_GEN_DB_LINEOTN_SYSOTN_MODE,
                                           otu_frm_handle->var.lineotn_db_handle_sysotn_mode,
                                           (void *)&sysotn_mode_db_record);


                }

                if( rc != PMC_SUCCESS )
                    break;
            }
        }
    }
    else /* remove deprovisioned records */
    {

        if(otu_frm_handle->pop_ho_db)
        {
            db_key.ho_channel = dci_channel;
            db_key.lo_channel = DO_NOT_USE_KEY;
            db_key.mo_channel = DO_NOT_USE_KEY;
            db_key.calendar_entry = DO_NOT_USE_KEY;
            db_key.trib_slot = DO_NOT_USE_KEY;
            rc = gen_db_rem_record(&(otu_frm_handle->base),
                                   UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                   otu_frm_handle->var.lineotn_db_handle_rx, (void *)&db_key);
            if ( PMC_SUCCESS == rc )
                rc = gen_db_rem_record(&(otu_frm_handle->base),
                                       UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                       otu_frm_handle->var.lineotn_db_handle_tx, (void *)&db_key);
        }
        else
        {
            sysotn_mode_db_key.calendar_entry = DO_NOT_USE_KEY;
            sysotn_mode_db_key.channel_num = dci_channel;
            
            rc = gen_db_rem_record(&(otu_frm_handle->base),
                                   UTIL_GEN_DB_LINEOTN_SYSOTN_MODE,
                                   otu_frm_handle->var.lineotn_db_handle_sysotn_mode, (void *)&sysotn_mode_db_key);
        }


    }

    PMC_RETURN(rc);
}/* otu_frm_db_prov_update */


/*******************************************************************************
* FUNCTION: otu_frm_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   dci_channel        - DSI stream
*   int_table_ptr      - pointer to aggregation or channel interrupt table with
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_validate(otu_frm_handle_t     *otu_frm_handle,
                                      UINT32                 dci_channel,
                                      void                  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != otu_frm_handle, OTU_FRM_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (dci_channel >= MAX_DCI_CHANNEL_NUM)
    {
        result = OTU_FRM_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* otu_frm_int_validate */


/*******************************************************************************
* otu_frm_get_dsi_master_lane_num
* ______________________________________________________________________________
*
* DESCRIPTION:
*   private helper function to return the master DSI lane number associated
*   with the specified DSI stream; this is associated with the non-channelized
*   logic in the OTU_FRM (eg. the dLOM defect).
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*   dsi_stream              - unsigned (0..11)
*   dsi_rate                - enum, dsi data rate (10/40/100G)
*
* OUTPUTS:
*
* RETURNS:
*   UINT8 - dsi stream (0..11) or FREE_CHANNEL if unprovisioned
*
* NOTES:
*
*******************************************************************************/

PRIVATE UINT8 otu_frm_get_dsi_master_lane_num(otu_frm_handle_t *otu_frm_handle, 
                                              UINT8 dsi_stream, 
                                              otu_frm_stream_rate_t dsi_rate) 
{
    PMC_ENTRY();
    UINT8 dsi_master_lane_num = dsi_stream;
    if (dsi_rate == RATE_40G) 
    {
        dsi_master_lane_num +=3;
    }
    else if (dsi_rate == RATE_100G)
    {
        dsi_master_lane_num = 11;
    }

    PMC_RETURN(dsi_master_lane_num);
}


/*******************************************************************************
* FUNCTION: otu_frm_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the interrupt table.
*
* INPUTS:
*   otu_frm_handle    - Pointer to subsystem handle
*   int_table_ptr     - pointer to interrupt table with bits set to enable
*                       interrupts.
*                       If NULL, enables/disables all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_enable(otu_frm_handle_t    *otu_frm_handle,
                                    otu_frm_int_t       *int_table_ptr,
                                    otu_frm_int_t       *int_en_table_ptr,
                                    BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_enable(otu_frm_handle->oduksc_handle,
                                   UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                   &int_en_table_ptr->oduksc,
                                   enable);
    }

    PMC_RETURN(result);
} /* otu_frm_int_enable */


/*******************************************************************************
* FUNCTION: otu_frm_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   otu_frm_handle    - Pointer to subsystem handle
*   int_table_ptr     - pointer to interrupt table with bits set to clear
*                       interrupts.
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_clear(otu_frm_handle_t   *otu_frm_handle,
                                   otu_frm_int_t      *int_table_ptr,
                                   otu_frm_int_t      *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_clear(otu_frm_handle->oduksc_handle,
                                  UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                  UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, oduksc));
    }

    PMC_RETURN(result);
} /* otu_frm_int_clear */


/*******************************************************************************
* FUNCTION: otu_frm_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the interrupt table.
*   
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   filt_table_ptr     - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be retrieved
*                        to the output table. NULL to retrieve all 
*                        interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_retrieve(otu_frm_handle_t   *otu_frm_handle,
                                      otu_frm_int_t      *filt_table_ptr,
                                      otu_frm_int_t      *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* ODUKSC */
    result = oduksc_int_retrieve(otu_frm_handle->oduksc_handle,
                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, oduksc),
                                 &int_table_ptr->oduksc);

    PMC_RETURN(result);
} /* otu_frm_int_retrieve */


/*******************************************************************************
* FUNCTION: otu_frm_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   int_en_table_ptr   - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_enabled_check(otu_frm_handle_t   *otu_frm_handle,
                                           otu_frm_int_t      *int_en_table_ptr,
                                           BOOL               *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    BOOL8           int_found = FALSE;

    PMC_ENTRY();

    /* ODUKSC */
    if (PMC_SUCCESS == result &&
        (FALSE == *int_found_ptr))
    {
        result = oduksc_int_enabled_check(otu_frm_handle->oduksc_handle,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, oduksc),
                                          &int_found);
    }
    if (TRUE == int_found)
    {
        *int_found_ptr = TRUE;
    }

    PMC_RETURN(result);
} /* otu_frm_int_enabled_check */


/*******************************************************************************
* FUNCTION: otu_frm_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
* INPUTS:
*   otu_frm_handle    - Pointer to subsystem handle
*   dci_channel       - DSI stream
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_chnl_enable(otu_frm_handle_t   *otu_frm_handle,
                                         UINT32               dci_channel,
                                         otu_frm_int_chnl_t  *int_table_ptr,
                                         otu_frm_int_chnl_t  *int_en_table_ptr,
                                         BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1;
    UINT32          link2;
    UINT8           dsi_stream;
    UINT8           dsi_master_lane;        
    otu_frm_stream_rate_t    dsi_rate;
    util_global_odukp_type_t oduk_type;

    PMC_ENTRY();

    result = otu_frm_int_validate(otu_frm_handle, dci_channel, int_en_table_ptr);

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_enable(otu_frm_handle->oduksc_handle,
                                        dci_channel,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                        &int_en_table_ptr->oduksc,
                                        enable);
    }

    /* Look-up the DSI Master lane number which is what the dLOM defect is based upon;
       the rest of the interrupts are based on the dci_channel number */
    dsi_stream = otu_frm_get_dci_channel_params(otu_frm_handle, dci_channel, &dsi_rate, &oduk_type);
    dsi_master_lane = otu_frm_get_dsi_master_lane_num(otu_frm_handle, dsi_stream, dsi_rate);

#ifdef OTU_FRM_INT
#undef OTU_FRM_INT
#endif
#define OTU_FRM_INT UTIL_GLOBAL_INT_ENABLE_RANGE
    
#ifdef OTU_FRM_INT_NOOP
#undef OTU_FRM_INT_NOOP
#endif
#define OTU_FRM_INT_NOOP UTIL_GLOBAL_INT_NOOP


    OTU_FRM_INT_TABLE_DEFINE(dsi_master_lane,dci_channel);

    PMC_RETURN(result);
} /* otu_frm_int_chnl_enable */


/*******************************************************************************
* FUNCTION: otu_frm_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   otu_frm_handle    - Pointer to subsystem handle
*   dci_channel       - DSI stream
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_chnl_clear(otu_frm_handle_t    *otu_frm_handle,
                                        UINT32               dci_channel,
                                        otu_frm_int_chnl_t  *int_en_table_ptr,
                                        otu_frm_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = dci_channel;
    UINT32          link2 = dci_channel;
    UINT8           dsi_stream;
    UINT8           dsi_master_lane;        
    otu_frm_stream_rate_t    dsi_rate;
    util_global_odukp_type_t oduk_type;

    PMC_ENTRY();

    result = otu_frm_int_validate(otu_frm_handle, dci_channel, int_en_table_ptr);

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_clear(otu_frm_handle->oduksc_handle,
                                       dci_channel,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                       &int_en_table_ptr->oduksc);
    }

    /* Look-up the DSI Master lane number which is what the dLOM defect is based upon;
       the rest of the interrupts are based on the dci_channel number */    
    dsi_stream = otu_frm_get_dci_channel_params(otu_frm_handle, dci_channel, &dsi_rate, &oduk_type);
    dsi_master_lane = otu_frm_get_dsi_master_lane_num(otu_frm_handle, dsi_stream, dsi_rate);
    
#ifdef OTU_FRM_INT
#undef OTU_FRM_INT
#endif
#define OTU_FRM_INT UTIL_GLOBAL_INT_CLEAR_RANGE
    
#ifdef OTU_FRM_INT_NOOP
#undef OTU_FRM_INT_NOOP
#endif
#define OTU_FRM_INT_NOOP UTIL_GLOBAL_INT_NOOP

    OTU_FRM_INT_TABLE_DEFINE(dsi_master_lane,dci_channel);

    PMC_RETURN(result);
} /* otu_frm_int_chnl_clear */


/*******************************************************************************
* FUNCTION: otu_frm_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   dci_channel        - DSI stream
*   filt_table_ptr     - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be retrieved
*                        to the output table. NULL to retrieve all 
*                        interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_chnl_retrieve(otu_frm_handle_t    *otu_frm_handle,
                                           UINT32               dci_channel,
                                           otu_frm_int_chnl_t  *filt_table_ptr,
                                           otu_frm_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = dci_channel;
    UINT32          link2 = dci_channel;
    UINT8           dsi_stream;
    UINT8           dsi_master_lane;        
    otu_frm_stream_rate_t    dsi_rate;
    util_global_odukp_type_t oduk_type;

    PMC_ENTRY();

    result = otu_frm_int_validate(otu_frm_handle, dci_channel, int_table_ptr);

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_retrieve(otu_frm_handle->oduksc_handle,
                                          dci_channel,
                                          &filt_table_ptr->oduksc,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc));
    }

    /* Look-up the DSI Master lane number which is what the dLOM defect is based upon;
       the rest of the interrupts are based on the dci_channel number */    
    dsi_stream = otu_frm_get_dci_channel_params(otu_frm_handle, dci_channel, &dsi_rate, &oduk_type);
    dsi_master_lane = otu_frm_get_dsi_master_lane_num(otu_frm_handle, dsi_stream, dsi_rate);
    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(otu_frm_handle, otu_frm, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(otu_frm_handle, otudg, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(otu_frm_handle, otu_frm_rx, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(otu_frm_handle, otu_frm_tx, UTIL_GLOBAL_INT_OPTIMIZE);

#ifdef OTU_FRM_INT
#undef OTU_FRM_INT
#endif
#define OTU_FRM_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE
        
#ifdef OTU_FRM_INT_NOOP
#undef OTU_FRM_INT_NOOP
#endif
#define OTU_FRM_INT_NOOP UTIL_GLOBAL_INT_NOOP

    OTU_FRM_INT_TABLE_DEFINE(dsi_master_lane,dci_channel);
        
#ifdef OTU_FRM_STATUS
#undef OTU_FRM_STATUS
#endif
#define OTU_FRM_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
        
#ifdef OTU_FRM_STATUS_NOOP
#undef OTU_FRM_STATUS_NOOP
#endif
#define OTU_FRM_STATUS_NOOP UTIL_GLOBAL_INT_NOOP


    OTU_FRM_STATUS_TABLE_DEFINE(dsi_master_lane,dci_channel);

    UTIL_GLOBAL_INT_BUFFER_FLUSH(otu_frm_handle, otu_frm);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(otu_frm_handle, otudg);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(otu_frm_handle, otu_frm_rx);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(otu_frm_handle, otu_frm_tx);

    PMC_RETURN(result);
} /* otu_frm_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: otu_frm_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   dci_channel        - DSI stream
*   int_en_table_ptr   - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr      - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_int_chnl_enabled_check(otu_frm_handle_t   *otu_frm_handle,
                                                UINT32              dci_channel,
                                                otu_frm_int_chnl_t *int_en_table_ptr,
                                                BOOL               *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = dci_channel;
    UINT32          link2 = dci_channel;
    UINT8           dsi_stream;
    UINT8           dsi_master_lane;        
    otu_frm_stream_rate_t    dsi_rate;
    util_global_odukp_type_t oduk_type;
    BOOL8           int_found = FALSE;

    PMC_ENTRY();

    result = otu_frm_int_validate(otu_frm_handle, dci_channel, int_en_table_ptr);

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_enabled_check(otu_frm_handle->oduksc_handle,
                                               dci_channel,
                                               &int_en_table_ptr->oduksc,
                                               &int_found);
    }
    if (TRUE == int_found)
    {
        *int_found_ptr = TRUE;
    }

    /* Look-up the DSI Master lane number which is what the dLOM defect is based upon;
       the rest of the interrupts are based on the dci_channel number */    
    dsi_stream = otu_frm_get_dci_channel_params(otu_frm_handle, dci_channel, &dsi_rate, &oduk_type);
    dsi_master_lane = otu_frm_get_dsi_master_lane_num(otu_frm_handle, dsi_stream, dsi_rate);

#ifdef OTU_FRM_INT
#undef OTU_FRM_INT
#endif
#define OTU_FRM_INT UTIL_GLOBAL_INT_CHECK_RANGE
    
#ifdef OTU_FRM_INT_NOOP
#undef OTU_FRM_INT_NOOP
#endif
#define OTU_FRM_INT_NOOP UTIL_GLOBAL_INT_NOOP

    OTU_FRM_INT_TABLE_DEFINE(dsi_master_lane,dci_channel);


    PMC_RETURN(result);
} /* otu_frm_int_chnl_enabled_check */


/*******************************************************************************
* FUNCTION: otu_frm_client_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns ODUK packet type associated to dsi stream.
*   
* INPUTS:
*   otu_frm_handle     - Pointer to subsystem handle
*   dsi_stream         - DSI stream identifier
*
* OUTPUTS:
*   *client_type       - oduk type
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void otu_frm_client_rate_get(otu_frm_handle_t           *otu_frm_handle,
                                    UINT32                       dsi_stream,
                                    util_global_odukp_type_t    *client_type)
{
    util_global_odukp_type_t server_rate;
    UINT32                   client_rate;
    UINT32                   rate_ramp;

    PMC_ENTRY();

    oduksc_cgen_chnl_cfg_get(otu_frm_handle->oduksc_handle, 
                             dsi_stream, 
                             &server_rate,
                             client_type,
                             &client_rate,
                             &rate_ramp);    
                             
    PMC_RETURN();
}


/*******************************************************************************
*  otu_frm_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset OTU_FRM registers associated to given dsi_streams.
*
* INPUTS:
*   otu_frm_handle          - handle to the OTU_FRM instance to be operated on
*   num_dsi_streams         - stream to disable
*   dsi_streams             - indices of dsi streams to disable
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
PUBLIC void otu_frm_reg_cleanup(otu_frm_handle_t    *otu_frm_handle,
                                UINT32               num_dsi_streams, 
                                UINT32              *dsi_streams)
{
    UINT32 itr;
    UINT32 jtr;
    UINT32                      dsi_stream;
    BOOL8                       is_40G;
    BOOL8                       is_100G;
    otu_frm_stream_rate_t       dsi_rate = LAST_RATE;
    UINT8                       dci_rx_ch_id = FREE_CHANNEL;
    UINT8                       dci_tx_ch_id = FREE_CHANNEL;
    UINT8                       dci_rx_ch_id_to_keep[12];
    UINT8                       dci_rx_ch_id_to_keep_num = 0;
    otu_frm_sm_oh_t oh_field;
    otu_frm_ctl_field_t ctl_field;
    UINT32 exp_tti_data[16];

    PMC_ENTRY();

    for (itr = 0; itr < num_dsi_streams; itr++)
    {
        dsi_stream = dsi_streams[itr];
        dsi_rate = LAST_RATE;
        dci_rx_ch_id = FREE_CHANNEL;
        dci_tx_ch_id = FREE_CHANNEL;
        is_40G = FALSE;
        is_100G = FALSE;
        
        if (0 == dsi_stream)
        {
            is_100G = TRUE;
        }
        if (0 == (dsi_stream % 4))
        {
            is_40G = TRUE;
        }

        /* retrieve DCI channel from register space*/
        if (0 != otu_frm_field_OTU_FRM_10G_INTLV_EN_get(NULL, otu_frm_handle, dsi_stream))
        {
            dsi_rate = RATE_10G;
            PMC_LOG_TRACE("RX: 10G stream %d %d %d\n", dci_rx_ch_id, MAX_DCI_CHANNEL_NUM, otu_frm_find_dci_ch_id(otu_frm_handle,dsi_stream, RATE_40G));
            dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
        }
        
        if (dci_rx_ch_id == FREE_CHANNEL)
        {            
            if ((TRUE == is_40G) && 0 != otu_frm_field_OTU_FRM_40G_INTLV_EN_get(NULL, otu_frm_handle, dsi_stream / 4))
            {
                dsi_rate = RATE_40G;
                PMC_LOG_TRACE("40G stream \n");
                dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream + 3);
            }
        }

        if (dci_rx_ch_id == FREE_CHANNEL)
        {
            if ((TRUE == is_100G) && 0 != otu_frm_field_OTU_FRM_100G_INTLV_EN_get(NULL, otu_frm_handle))
            {
                dsi_rate = RATE_100G;
                PMC_LOG_TRACE("100G stream \n");
                dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream);
            }
        }
   
        
        if (dci_rx_ch_id < MAX_DCI_CHANNEL_NUM)
        {                
            /* set passthrough mode */
            otu_frm_passthru_set(otu_frm_handle, dci_rx_ch_id, OTU_SK_PASSTHRU, SET_BIT);

        }

        /* clean interleaver registers and context fields */
        cbr_int_channel_reg_cleanup( otu_frm_handle, dsi_stream);

        /* beginning of TX cleanup */       
        dci_tx_ch_id = dci_rx_ch_id;
        
        /* otu_frm_tx configuration */
        if (MAX_DCI_CHANNEL_NUM > dci_tx_ch_id)
        {
            /* reset all OH and CTL registers */
            for(oh_field = OTU_SO_MFAS; oh_field < LAST_OH; oh_field = (otu_frm_sm_oh_t)((UINT32)oh_field + 1) )
            {
                (void)otu_frm_sm_oh_set(otu_frm_handle, dci_tx_ch_id, oh_field, 0);
            }
            
            for(ctl_field = OTU_SO_FAS_INS_CTRL; ctl_field <= OTU_SO_RES14_INS_CTRL; ctl_field = (otu_frm_ctl_field_t)((UINT32)ctl_field + 1) )
            {
                (void)otu_frm_sm_ctl_set(otu_frm_handle, dci_tx_ch_id, ctl_field, 0);
            }
        }
    
        /* clean de-interleaver registers and context fields */
        cbr_dint_channel_reg_cleanup( otu_frm_handle, dsi_stream); 
        
        /* end of TX chnl cleanup */

        /* cleanup common registers used by RX and TX datapath */
        
        /* stop the FEC degap mechanism */   
        otu_frm_stream_fec_degap_reg_cleanup(otu_frm_handle, dsi_stream, dsi_rate);

        /* cleanup stream features: scrambling, FEC, dlom, etc. */
        otu_frm_stream_cfg_reg_cleanup(otu_frm_handle, dsi_stream, dsi_rate);

        /* oduksc configuration */
        if (MAX_DCI_CHANNEL_NUM > dci_rx_ch_id)
        {
            (void)oduksc_chnl_cleanup(otu_frm_handle->oduksc_handle, dci_rx_ch_id);
            (void)oduksc_chnl_update(otu_frm_handle->oduksc_handle, dci_rx_ch_id, TRUE); 
        }
           
        /* otu_frm_rx configuration */
        if (MAX_DCI_CHANNEL_NUM > dci_rx_ch_id)
        {            
            PMC_MEMSET (&exp_tti_data[0],0,sizeof(exp_tti_data));
                    
            /* rx side */
            /* This is a good starting point - user can override later if he wants to */
            (void)otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MI_LCK,0); /* disable maintenance signal */
            (void)otu_frm_sm_ctl_set(otu_frm_handle,dci_rx_ch_id,OTU_SK_MAINT_SIG,0); /* No maintenance signal is forced */
            (void)otu_frm_sm_tti_set(otu_frm_handle,SK_DIR,dci_rx_ch_id, exp_tti_data); /* tti is set to all 0 */
        }
        
        /* last register to clean up */
        switch(dsi_rate)
        {
            /* passthru */
        case RATE_10G: 
        case LAST_RATE:
            otu_frm_field_OTU_FRM_CH_ID_set(NULL, otu_frm_handle, dsi_stream, FREE_CHANNEL);
            break;
            /* passthru */
        case RATE_40G: 
            for (jtr = 0; jtr < 4; jtr++)
            {
                otu_frm_field_OTU_FRM_CH_ID_set(NULL, otu_frm_handle, dsi_stream + jtr, FREE_CHANNEL);
            }
            break;
        case RATE_100G: 
            for (jtr = 0; jtr < 12; jtr++)
            {
                otu_frm_field_OTU_FRM_CH_ID_set(NULL, otu_frm_handle, jtr, FREE_CHANNEL);
            }
            break;
        default:
            break;
        }

        /* mark the dsi stream_dsc as free */   
        otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].stream_lane = FREE_STREAM;
            
        if(0 == (dsi_stream % 4))
        {            
            otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream / 4].stream_lane = FREE_STREAM;
        }
            
        if(0 == dsi_stream)
        {
            otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].stream_lane=FREE_STREAM;
        }
        
        for (jtr = 0; jtr < MAX_DCI_CHANNEL_NUM; jtr++)
        {
            if (dsi_stream == otu_frm_handle->var.dci_to_dsi_chan_ids[jtr])
            {
                if (MAX_DCI_CHANNEL_NUM > dci_rx_ch_id)
                {
                    otu_frm_handle->var.dci_to_dsi_chan_ids[dci_rx_ch_id] = FREE_STREAM;
                }
            }
        }
        otu_frm_handle->var.dsi_oduk_type[dsi_stream] = UTIL_GLOBAL_ODUK_DONT_CARE;

        /* set rx steam next state */
        PMC_LOG_TRACE("otu_frm_set_rx_stream_state for 10G set to START \n");
        otu_frm_set_rx_stream_state(otu_frm_handle, RATE_10G, dsi_stream, STREAM_STATE_START);
        
        if (0 == dsi_stream)
        {
            otu_frm_set_rx_stream_state(otu_frm_handle, RATE_100G, dsi_stream, STREAM_STATE_START);
        }
        if (0 == (dsi_stream % 4))
        { 
            otu_frm_set_rx_stream_state(otu_frm_handle, RATE_40G, dsi_stream/4, STREAM_STATE_START);
        }
        
        /* set tx stream next state */
        PMC_LOG_TRACE("otu_frm_set_tx_stream_state for 10G set to START \n");
        otu_frm_set_tx_stream_state(otu_frm_handle, RATE_10G, dsi_stream, STREAM_STATE_START);
        
        if (0 == dsi_stream)
        {
            otu_frm_set_tx_stream_state(otu_frm_handle, RATE_100G, dsi_stream, STREAM_STATE_START);
        }
        if (0 == (dsi_stream % 4))
        { 
            otu_frm_set_tx_stream_state(otu_frm_handle, RATE_40G, dsi_stream/4, STREAM_STATE_START);
        }
    }

    /* identify RX DCI channel to keep */
    {
        UINT32 dsi_stream_itr = 0;
        
        for (dsi_stream_itr = 0; dsi_stream_itr < 12; dsi_stream_itr++)
        {
            is_40G = FALSE;
            is_100G = FALSE;
            
            if (0 == dsi_stream_itr)
            {
                is_100G = TRUE;
            }
            if (0 == (dsi_stream_itr % 4))
            {
                is_40G = TRUE;
            }

            dci_rx_ch_id = FREE_CHANNEL;
            if (1 == otu_frm_field_OTU_FRM_10G_INTLV_EN_get(NULL, otu_frm_handle, dsi_stream_itr))
            {
                dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream_itr);
            }
        
            if (dci_rx_ch_id == FREE_CHANNEL)
            {            
                if ((TRUE == is_40G) && 0 != otu_frm_field_OTU_FRM_40G_INTLV_EN_get(NULL, otu_frm_handle, dsi_stream_itr / 4))
                {
                    dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream_itr + 3);
                }
            }

            if (dci_rx_ch_id == FREE_CHANNEL)
            {
                if ((TRUE == is_100G) && 0 != otu_frm_field_OTU_FRM_100G_INTLV_EN_get(NULL, otu_frm_handle))
                {
                    dsi_rate = RATE_100G;
                    PMC_LOG_TRACE("100G stream \n");
                    dci_rx_ch_id = otu_frm_field_OTU_FRM_CH_ID_get(NULL, otu_frm_handle, dsi_stream_itr);
                }
            }
        
            if (FREE_CHANNEL != dci_rx_ch_id)
            {
                dci_rx_ch_id_to_keep[dci_rx_ch_id_to_keep_num] = dci_rx_ch_id;
                dci_rx_ch_id_to_keep_num += 1;
            }   
        }

        for (itr = 0; itr < 12; itr++)
        {
            dci_rx_ch_id = FREE_CHANNEL;
            /* check if we keep it */
            for (jtr = 0; jtr < dci_rx_ch_id_to_keep_num && dci_rx_ch_id == FREE_CHANNEL; jtr++)
            {
                if (itr == dci_rx_ch_id_to_keep[jtr])
                {
                    dci_rx_ch_id = itr;
                }
            }
                        
            if (dci_rx_ch_id == FREE_CHANNEL)
            {
                /* oduksc configuration */
                (void)oduksc_chnl_cleanup(otu_frm_handle->oduksc_handle, itr);
                (void)oduksc_chnl_update(otu_frm_handle->oduksc_handle, itr, TRUE);                 
            }
        }
    }

    PMC_RETURN();
}/*otu_frm_reg_cleanup*/


/*******************************************************************************
* otu_frm_stream_cfg_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   resets the scrambling,descrambling,FEC column removal/insertion,dLOM enable and
*   sf squelch enable disable bits per data rate and stream.
*   Data rates are 10G (12 streams),40G (3 streams), 10G (1 stream).
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*   dsi_stream              - DSI stream
*   stream_rate             - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void otu_frm_stream_cfg_reg_cleanup(otu_frm_handle_t           *otu_frm_handle,
                                            UINT32                      dsi_stream,
                                            otu_frm_stream_rate_t       stream_rate)
{
    UINT32 dsi_stream_40G_base = dsi_stream / 4;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);

    /* copy stream parameters to the handle configuration */
    PMC_MEMSET(&otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].stream_dsc,
               0,
               sizeof(otu_frm_stream_cfg_t));
    
    otu_frm_handle->var.otu_frm_10G_stream_cfg[dsi_stream].stream_lane = FREE_LANE;
    
    /* update hw register by direction asked */
    otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set(NULL,otu_frm_handle, dsi_stream, FALSE);
    otu_frm_field_OTU_FRM_10G_DLOM_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);
    otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);
    otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);
    otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);
    
    otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);
    otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set(NULL,otu_frm_handle,dsi_stream, FALSE);

    /* reset potential 40G stream */
    if (0 == (dsi_stream % 4))
    {
        /* copy stream parameters to the handle configuration */
        PMC_MEMSET(&otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream_40G_base].stream_dsc,
                   0,
                   sizeof(otu_frm_stream_cfg_t));

        otu_frm_handle->var.otu_frm_40G_stream_cfg[dsi_stream_40G_base].stream_lane = FREE_LANE;
        
        otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
        otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
        otu_frm_field_OTU_FRM_40G_DLOM_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
        otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
        otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
    
        otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
        otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set(NULL,otu_frm_handle,dsi_stream_40G_base, FALSE);
    }

    /* reset potential 100G stream */
    if (0 == dsi_stream)
    {
        /* copy stream parameters to the handle configuration */
        PMC_MEMSET(&otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].stream_dsc,
                   0,
                   sizeof(otu_frm_stream_cfg_t));
        
        otu_frm_handle->var.otu_frm_100G_stream_cfg[dsi_stream].stream_lane = FREE_LANE;

        otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set(NULL,otu_frm_handle, FALSE);
        otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set(NULL,otu_frm_handle, FALSE);
        otu_frm_field_OTU_FRM_100G_DLOM_EN_set(NULL,otu_frm_handle, FALSE);
        otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set(NULL,otu_frm_handle, FALSE);
        otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set(NULL,otu_frm_handle, FALSE);

        otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set(NULL,otu_frm_handle, FALSE);
        otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set(NULL,otu_frm_handle, FALSE);        
    }
    
    PMC_RETURN();
} /* otu_frm_stream_cfg_reg_cleanup */


/*******************************************************************************
* otu_frm_stream_fec_degap_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the FEC degap parameters per
*   dsi stream during stream provisioning/deprovisioning.
*   values set are the delta sigma numerator/denominator and the smoothing
*   filter enable/disable bit.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*
*   dsi_stream          - UINT8, requested stream number (0..11)
*
*   lane_rate           - enum, requested data rate:
*                         RATE_10G,
*                         RATE_40G,
*                         RATE_100G
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void otu_frm_stream_fec_degap_reg_cleanup(otu_frm_handle_t     *otu_frm_handle,
                                                  UINT8                 dsi_stream,
                                                  otu_frm_stream_rate_t lane_rate)
{
    UINT8 cfg_lane;
    UINT8 i = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream< MAX_10G_STREAM_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /*
      The simplest software vision is
      Internal RX_LINECLK[11]=RX_LINECLK[11] in 10G mode
      Internal RX_LINECLK[11]=RX_LINECLK[8] in 100G mode (this meets 100G OTL, 100G SFIS(N=10), 100G 2xSFIS(N=5))
      Internal RX_LINECLK[11]=RX_LINECLK_40G_SFI51[0] in 40G mode.

      The 100G 2xSFIS(N=5) mode may also use the RX_LINECLK[11], but I think to use RX_LINECLK[8]
      for any 100G mode is simpler for software.

      The overall clock table looks like this (take care of the column alignment):
      RX_LINECLK[11:0]     0  1  2  3  4  5  6  7  8  9  10  11
      10G                  0  1  2  3  4  5  6  7  8  9  10  11
      40G_OTL              0==1==2==3  4==5==6==7  8==9==10==11
      40G SFIS(N=4)        0==1==2==3==4  5==6==7==8==9
      100G OTL             0==1==2==3==4==5==6==7==8==9
      100G SFIS(N=10)      0==1==2==3==4==5==6==7==8==9==10
      100G 2xSFIS(N=5)     0==1==2==3==4==5==6==7==8==9==10==11
      40G SFI5.1           ====0=====  ====1=====  =====0======  Here the index refers to RX_LINECLK40G_SFI51[1:0]
    */
    
    if (LAST_RATE == lane_rate ||
        RATE_10G == lane_rate)
    {
        /* map dsi lanes to FEC degap lanes */
        /* 10G - 1:1 mapping */
        cfg_lane = dsi_stream;
        
        /* configure rx line clk mux for streams 3,7,11             */
        /* all other lanes do not require mux output configuration  */
        /* in 10G mode. select source #0                            */
        if(3 == dsi_stream || 7 == dsi_stream || 11 == dsi_stream)
        {
            otu_frm_rx_lineclk_mux_enable(otu_frm_handle, dsi_stream, 0, RESET_BIT);
            otu_frm_rx_lineclk_mux_cfg(otu_frm_handle, dsi_stream,0);
            
            /* first - disable slice */
            otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
            
            /* do not power on or take our of reset - will be done at the otu_frm_fec_degap_enable function */
        }
        
        /* reset smoothing delta sigma modulation parameters */
        if (RATE_10G == lane_rate)
        {
            (void)otu_frm_dsm_set(otu_frm_handle, dsi_stream, lane_rate);
        }
    
        otu_frm_handle->var.data_smoothing_enabled[cfg_lane]=FALSE;    
    }
        
    if (0 == (dsi_stream % 4) && RATE_40G == lane_rate)
    {
        /* 40G - dsi stream(0,1,2)--> lanes(3,7,11) mapping */
        cfg_lane = dsi_stream + 3;
                
        otu_frm_rx_lineclk_mux_enable(otu_frm_handle, cfg_lane, 0, RESET_BIT);
        otu_frm_rx_lineclk_mux_cfg(otu_frm_handle,cfg_lane, 0);
        
        /* first - disable slice */
        otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);

        /* do not power on or take our of reset - will be done at the otu_frm_fec_degap_enable function */
        
        for(i =(cfg_lane-3); i<=cfg_lane; i++)
            otu_frm_handle->var.data_smoothing_enabled[i]=FALSE;
        
        /* reset smoothing delta sigma modulation parameters */
        otu_frm_dsm_set(otu_frm_handle, dsi_stream / 4, lane_rate);
    }

    if (0 == dsi_stream && RATE_100G == lane_rate)
    {
        /* 100G - dsi stream(0)--> lane(11) mapping */
        cfg_lane = 11;
        
        /* configure rx line clk mux for streams 11                     */
        /* for all 100G configurations we use lane 11 with rx_lineclk[8]*/
        /* as source (#2)                                               */
        
        otu_frm_rx_lineclk_mux_enable(otu_frm_handle, cfg_lane, 0, RESET_BIT);
        otu_frm_rx_lineclk_mux_cfg(otu_frm_handle,11,2);

        /* first - disable slice */
        otudg_field_SLICE_EN_set(NULL,otu_frm_handle,cfg_lane,0);
        
        for(i = 0; i<12 ; i++)
            otu_frm_handle->var.data_smoothing_enabled[i]= FALSE;

        /* check if hw adapt is set for this dsi stream */
        otu_frm_dsm_set(otu_frm_handle, dsi_stream, lane_rate);
    }
        
    /* update context */
    otu_frm_handle->var.data_smoothing_enabled[dsi_stream] = FALSE;

    PMC_RETURN();
}/* otu_frm_stream_fec_degap_reg_cleanup */


/*******************************************************************************
* otu_frm_dintlv_xoff_thres_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets a dci channel(0..11) into/out of passthru mode.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*
*   dci_channel         - UINT8, requested channel (0..11)
*
*   dintlv_xoff         - Desired deinterleaver fifo xoff threshold
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otu_frm_dintlv_xoff_thres_set(otu_frm_handle_t  *otu_frm_handle,
                                               UINT32            dci_channel,
                                               UINT32            dintlv_xoff)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,OTU_FRM_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel< MAX_DCI_CHANNEL_NUM,OTU_FRM_ERR_INVALID_PARAMETERS,0,0);

    otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set(NULL,otu_frm_handle,dci_channel,dintlv_xoff);

    PMC_RETURN(rc);

}/* otu_frm_dintlv_xoff_thres_set */


/*
** End of file
*/
