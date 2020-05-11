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
#include "lifd_loc.h"

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

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char LIFD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    LIFD_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char LIFD_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/

PRIVATE PMC_ERROR lifd_internal_to_serdes_param_check(UINT32 i_lane,
                                                      UINT32 s_lane,
                                                      UINT32* ret_value);
PRIVATE PMC_ERROR lifd_serdes_to_internal_param_check(UINT32 s_lane,
                                                      UINT32 i_lane,
                                                      UINT32* ret_value_ptr);
PRIVATE PMC_ERROR lifd_internal_lane_param_check(UINT32 i_lane);
PRIVATE PMC_ERROR lifd_serdes_lane_param_check(UINT32 s_lane);
PRIVATE PMC_ERROR lifd_internal_lane_configure(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_rx_lane, 
                                               UINT32 serdes_tx_lane, 
                                               UINT32 internal_rx_lane, 
                                               UINT32 internal_tx_lane);
PRIVATE PMC_ERROR lifd_serdes_subsystem_set(lifd_handle_t *lifd_handle, 
                                            UINT32 serdes_lane, 
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable);
PRIVATE PMC_ERROR lifd_subsytem_lane_enable(lifd_handle_t *lifd_handle, 
                                            UINT32 serdes_lane, 
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable);

/*
** Public Functions
*/

/*******************************************************************************
* lifd_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an LIFD block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the LIFD subsystem relative to
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - the name of this TSB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   lifd_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC lifd_handle_t *lifd_ctxt_create(pmc_handle_t *parent,
                                       UINT32 base_address,
                                       pmc_sys_handle_t *sys_handle,
                                       const char *tsb_name)
{
    lifd_handle_t *lifd_handle;

    PMC_ENTRY();


#ifdef USE_PMC_CTXT_CALLOC
        if (parent == NULL) {
            void *c = NULL;
            c = pmc_ctxt_mem_create(sys_handle, FALSE);
            pmc_ctxt_mem_init(c);
            lifd_handle = (lifd_handle_t*)(((pmc_ctxt_mgmt_ctxt_t*)c)->header.digi_hdl);
            lifd_handle->base.ctxt = c;
            lifd_handle = (lifd_handle_t*)PMC_CTXT_CALLOC(sizeof(lifd_handle_t), lifd_handle);
        } else {
            lifd_handle = (lifd_handle_t*)PMC_CTXT_CALLOC(sizeof(lifd_handle_t), parent);
        };
#else
        lifd_handle = (lifd_handle_t*)PMC_CTXT_CALLOC(sizeof(lifd_handle_t), parent);
#endif

    pmc_log_block_strings_register(LIFD_LOG_ERR_STRINGS[0], LIFD_LOG_ERR_TABLE_BASE, LIFD_LOG_ERR_COUNT);

    pmc_handle_init(parent, lifd_handle, sys_handle, PMC_MID_DIGI_LIFD,
                    tsb_name, base_address);

    PMC_RETURN(lifd_handle);
} /* lifd_ctxt_create */

/*******************************************************************************
* lifd_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an LIFD block instance.
*
*
* INPUTS:
*   *lifd_handle           - pointer to LIFD handle instance
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
PUBLIC void lifd_ctxt_destroy(lifd_handle_t *lifd_handle)
{
    PMC_ENTRY();

    pmc_handle_mid_verify(lifd_handle, PMC_MID_DIGI_LIFD);
    lifd_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&lifd_handle, lifd_handle);
    PMC_RETURN();
} /* lifd_ctxt_destroy */

/*******************************************************************************
* lifd_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an LIFD block handle instance.
*
*
* INPUTS:
*   *lifd_handle           - pointer to LIFD handle instance
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
PUBLIC void lifd_handle_init(lifd_handle_t *lifd_handle)
{
    PMC_ENTRY();

    UINT8 i = 0;

    PMC_ASSERT((lifd_handle != NULL), LIFD_ERR_CODE_ASSERT, 0, 0);

    for (i = 0; i < LIFD_SERDES_DSIS_LANES_MAX; ++i) {

        if (i > LIFD_MAXIMUM_INTERNAL_LANE_NUM) {

            lifd_handle->var.internal_lane_tx[i] = LIFD_SERDES_UNASSIGNED;
            lifd_handle->var.internal_lane_rx[i] = LIFD_SERDES_UNASSIGNED;
            lifd_handle->var.los_ctxt[i].active = FALSE;

        } else {
            lifd_internal_lane_configure(lifd_handle, i, i, i, i);
        }
    }

    for (i = 0; i < (LIFD_MAXIMUM_INTERNAL_LANE_NUM+1); ++i) {
            lifd_handle->var.serdes_los_lane[i] = LIFD_SERDES_UNASSIGNED;
    }
    for (i = 0; i < (LIFD_MAXIMUM_INTERNAL_LANE_NUM+1); ++i) {
            lifd_handle->var.serdes_lane[i] = LIFD_SERDES_UNASSIGNED;
    }


    PMC_RETURN();
} /* lifd_handle_init */

/*******************************************************************************
* lifd_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on lifd.\n\n
*
*   In warm restart scenario return of PMC_SUCCESS indicates hardware register
*   space and context are coherent.\n\n
*
*   In crash restart scenario return of PMC_SUCCESS indicates hardware register
*   space and context are coherent.  A cleanup may have been performed modifying
*   the state of hardware register space and or context as part of this process.
*   
*
* INPUTS:
*   *lifd_handle          - pointer to LIFD handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_handle_restart_init(lifd_handle_t *lifd_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                          pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT((lifd_handle != NULL), LIFD_ERR_CODE_ASSERT, 0, 0);

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(LIFD_LOG_ERR_STRINGS[0], LIFD_LOG_ERR_TABLE_BASE, LIFD_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* lifd_handle_restart_init */

/*******************************************************************************
* lifd_serdes_subsystem_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Selects the internal lane to/from a SERDES lane.  The internal lane can
*   be swizzled from a specified subsystem lane.  The SERDES lane will be
*   enabled automatically.\n\n
*
*   When CBRC/OTN is used in a MLD configuration (i.e. multi-lane), a master
*   needs to be specified for the clock source.  Both the master internal lane
*   and its matching master SERDES lane need to be specified.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.\n\n
*
*   Multicasting several lanes to the same subsystem may result in
*   undetermined behaviour and is not supported. It is up to the user to
*   ensure that these configurations do not occur.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   serdes_rx_lane        - RX SERDES lane number
*   serdes_tx_lane        - TX SERDES lane number
*   subsystem_type        - direct the lane to this subsystem
*   single_lane_otn_cbrc  - single lane CBRC/OTN (TRUE) or MLD configuration
*                           (FALSE) that derives its clock from the master lane
*   master_serdes_lane    - master clock SERDES lane for TX FIFO
*   is_cfp_application    - TRUE: This is a CFP product application
*                         - FALSE: This is not a CFP product application
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Certain SERDES -> internal lane combinations are not possible.
*   Following is a diagram indicating which SERDES lanes may be mapped
*   to which internal lanes:\n
*\n
*   SERDES Lane [11:00]  -> Internal Lane [11:00]\n
*\code
*   Internal Lane:     [11][10][09][08][07][06][05][04][03][02][01][00]\n
*   SERDES Lane [12]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [13]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [14]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [15]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*   SERDES Lane [16]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [17]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [18]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [19]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*\endcode
*
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_subsystem_configure(lifd_handle_t *lifd_handle,
                                                 UINT32 subsystem_lane,
                                                 UINT32 serdes_rx_lane,
                                                 UINT32 serdes_tx_lane,
                                                 xifd_dsis_type_t subsystem_type,
                                                 BOOL single_lane_otn_cbrc,
                                                 UINT32 master_serdes_lane,
                                                 BOOL is_cfp_application)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL valid_sub = FALSE;
    UINT32 clock_enable = 0;
    UINT32 ret_value_data = 3;
    UINT32 ret_value_clock = 3;
    UINT32 ret_value_serdes = 3;
    UINT32 i;

    /*UINT32 lane_offset, group_offset;*/

    UINT32 internal_rx_lane = LIFD_SERDES_UNASSIGNED;
    UINT32 internal_tx_lane = LIFD_SERDES_UNASSIGNED;

    UINT32 master_serdes = 0;
    UINT32 master_lane = 0;

    PMC_ENTRY();

    if(is_cfp_application)
    {
        /* The internal lanes were chosen in lifd_internal_context_set */
        for(i=0;i<12;i++)
        {
            if(lifd_handle->var.serdes_lane[i] == serdes_rx_lane)
            {
                internal_rx_lane = i;
                lifd_handle->var.internal_lane_rx[serdes_rx_lane] = i;
            }
            if(lifd_handle->var.serdes_lane[i] == serdes_tx_lane)
            {
                internal_tx_lane = i;
                lifd_handle->var.internal_lane_tx[serdes_tx_lane] = i;
            }
            if(subsystem_type == XIFD_DSIS_ENET)
            {
                master_serdes = serdes_tx_lane;                 /* default master */
                master_lane = internal_tx_lane;                 /* default master */
            }
            else
            {
                if(lifd_handle->var.serdes_lane[i] == master_serdes_lane)
                {
                    master_lane = i;
                    master_serdes = master_serdes_lane;
                }
            }
        }
        if(internal_rx_lane == LIFD_SERDES_UNASSIGNED ||
           internal_tx_lane == LIFD_SERDES_UNASSIGNED)
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
/*        else
            master_lane = internal_tx_lane;*/

        PMC_LOG_TRACE("lifd cfp: serdes_tx_lane: %u, internal_tx_lane: %u, subsystem_lane = %u\n",serdes_tx_lane,internal_tx_lane, subsystem_lane);
        PMC_LOG_TRACE("lifd cfp: serdes_rx_lane: %u, internal_rx_lane: %u, subsystem_lane = %u\n",serdes_rx_lane,internal_rx_lane, subsystem_lane);
    }
    else
    {
        internal_rx_lane = lifd_handle->var.internal_lane_rx[serdes_rx_lane];
        internal_tx_lane = lifd_handle->var.internal_lane_tx[serdes_tx_lane];

        master_serdes = serdes_tx_lane;                 /* default master */
        master_lane = internal_tx_lane;                 /* default master */

    }
    lifd_handle->var.internal_lane_rx[serdes_rx_lane] = internal_rx_lane;
    lifd_handle->var.internal_lane_tx[serdes_tx_lane] = internal_tx_lane;


    if (rc == PMC_SUCCESS) 
    {
        switch (subsystem_type) {
        case XIFD_DSIS_CBRC:
            clock_enable = LIFD_CBRC_CLK_ENABLE;               /* enable CBRC clock */
            valid_sub = TRUE;
            if (!single_lane_otn_cbrc && !is_cfp_application) {                       /* master on another lane? */
                master_lane = lifd_handle->var.internal_lane_tx[master_serdes_lane];
                master_serdes = master_serdes_lane;
            }
            break;
        case XIFD_DSIS_OTN:
            clock_enable = LIFD_OTN_CLK_ENABLE;                /* enable OTN clock */
            valid_sub = TRUE;
            if (!single_lane_otn_cbrc && !is_cfp_application) {                       /* master on another lane? */
                master_lane = lifd_handle->var.internal_lane_tx[master_serdes_lane];
                master_serdes = master_serdes_lane;
            }
            break;
        case XIFD_DSIS_ENET:
            clock_enable = LIFD_ENET_CLK_ENABLE;               /* enable ENET clock */
            valid_sub = TRUE;
            break;
        default:
            break;
        }
    }

    PMC_LOG_TRACE("lifd cfp: master_serdes: %u, master_lane: %u, master_serdes_lane: %u\n",master_serdes,master_lane,master_serdes_lane);
    
    if(!valid_sub)
        rc = PMC_ERR_INVALID_PARAMETERS;
    
    if(rc == PMC_SUCCESS)
        rc = lifd_internal_lane_param_check(subsystem_lane);
    
    if(rc == PMC_SUCCESS)
        rc = lifd_serdes_to_internal_param_check(serdes_rx_lane, internal_rx_lane, &ret_value_data);
    
    if(rc == PMC_SUCCESS)
        rc = lifd_serdes_to_internal_param_check(master_serdes, master_lane, &ret_value_clock);
    

    if(rc == PMC_SUCCESS) 
    {
        
        /* select SERDES receive clock & data for internal lane */
        line_dsis_field_RX_I_SEL_set(NULL, lifd_handle, internal_rx_lane, ret_value_data);
        
        /* select internal receive lane clock & data for subsystem lanes (i.e. swizzle) */
        line_dsis_field_RX_SEL_set(NULL, lifd_handle, subsystem_lane, internal_rx_lane);

        /* select SERDES transmit clock for internal transmit lane (which may be the master clock) */
        line_dsis_field_TX_I_CLK_SEL_set(NULL,
                                         lifd_handle,
                                         internal_tx_lane, /* DSIS Internal Lane N */
                                         ret_value_clock);  /* TX_I_CLK_SEL value */

        /* select internal transmit clock for subsystem lanes
           - multi-lane interfaces on CBRC & OTN use a master clock - all lanes
           are set to the same value */
        line_dsis_field_TX_CLK_SEL_set(NULL, lifd_handle, subsystem_lane, master_lane);

        if (subsystem_type == XIFD_DSIS_ENET) {
            /* select default transmit clock for ENET subsystem to be the internal lane (SERDES) clock*/
            line_dsis_field_range_ENET_TX_CLK_SEL_set(NULL, lifd_handle, subsystem_lane, subsystem_lane, 0);
        }

        /* select subsystem that drives internal transmit lane before reordering */
        rc = lifd_serdes_tx_if_sel_update(lifd_handle, subsystem_lane, subsystem_type);
    }


    if (PMC_SUCCESS == rc) 
    {
        if(is_cfp_application)
        {
            rc = lifd_internal_to_serdes_param_check(internal_tx_lane, serdes_tx_lane, &ret_value_serdes);
            if(rc == PMC_SUCCESS)
            {
                /*line_dsis_field_TX_SEL_set(NULL, lifd_handle, serdes_tx_lane - group_offset + lane_offset, internal_tx_lane);     */
                /*line_dsis_field_TX_SEL_set(NULL, lifd_handle, subsystem_lane, internal_tx_lane);*/
                line_dsis_field_TX_SEL_set(NULL, lifd_handle, internal_tx_lane, subsystem_lane);
                if(serdes_tx_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM)
                {
                    line_dsis_field_TX_O_SEL_set(NULL, lifd_handle,
                                                 (serdes_tx_lane - (LIFD_MAXIMUM_INTERNAL_LANE_NUM + 1)),
                                                 ret_value_serdes);
                }
            }
        }
        else
            line_dsis_field_TX_SEL_set(NULL, lifd_handle, serdes_tx_lane, subsystem_lane);
            
        /* reset TX FIFO */
        line_dsis_field_range_TX_FIFO_RESET_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 1);
            
        /* clock for FIFO write port - same as transmit clock of master SERDES port */
        line_dsis_field_TX_FIFO_WRITE_CLK_SEL_set(NULL, lifd_handle, serdes_tx_lane, master_serdes);
            
        switch (subsystem_type) {
        case XIFD_DSIS_ENET:
            /* set FIFO to master mode */
            line_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 0);
            break;
                
        case XIFD_DSIS_CBRC:
        case XIFD_DSIS_OTN:
            if (single_lane_otn_cbrc || (serdes_tx_lane == master_serdes_lane)) {
                /* set FIFO to master mode */
                line_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 0);
            }
            else {
                /* set FIFO to slave mode */
                line_dsis_field_range_TX_FIFO_MODE_SEL_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 1);
            }
            break;
        default:
            break;
        }
            
        /* set FIFO for auto-centering */
        line_dsis_field_range_TX_FIFO_AUTO_CENTER_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 1);
            
        /* release reset of TX FIFO */
        line_dsis_field_range_TX_FIFO_RESET_set(NULL, lifd_handle, serdes_tx_lane, serdes_tx_lane, 0);
    }

    if (rc == PMC_SUCCESS)
    {
        rc = lifd_subsytem_lane_enable(lifd_handle, serdes_tx_lane, subsystem_lane, clock_enable);        
    }

    PMC_RETURN(rc);
} /* lifd_serdes_subsystem_configure */

/*******************************************************************************
* lifd_serdes_subsystem_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Disables the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_subsystem_disable(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_lane,
                                               UINT32 subsystem_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 clock = LIFD_CLK_DISABLE;   /* disable all subsystems */

    PMC_ENTRY();

    if ((serdes_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM) ||
        (subsystem_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM)) {

        rc = LIFD_ERR_NO_SW_CONTENT;

    } else {

        rc = lifd_serdes_subsystem_set(lifd_handle,
                                       serdes_lane,
                                       subsystem_lane,
                                       clock);
    }

    PMC_RETURN(rc);
} /* lifd_serdes_subsystem_disable */

/*******************************************************************************
* lifd_internal_context_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets up the LIFD context to map serdes lanes from the line or system
*   side to LIFD internal lanes.
*
*
* INPUTS:
*   *lifd_handle          - Pointer to lifd handle instance to be operated on
*   serdes_pin            - Serdes pin index
*   is_line_side          - LOS pin is comming from the line or system side:\n
*                           TRUE: line side\n
*                           FALSE: system side
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_internal_context_set(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_pin,
                                               BOOL is_line_side)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();

    if(is_line_side)
    {
        if(serdes_pin > LIFD_MAXIMUM_INTERNAL_LANE_NUM)
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);

        if(lifd_handle->var.serdes_lane[serdes_pin] != LIFD_SERDES_UNASSIGNED)
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);

        lifd_handle->var.serdes_lane[serdes_pin] = serdes_pin;

    }
    else /* System side */
    {
        if(serdes_pin < 4)
        {
        /* range 0-3 */
        if(lifd_handle->var.serdes_lane[0+serdes_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[0+serdes_pin] = 12+serdes_pin;
        else if (lifd_handle->var.serdes_lane[4+serdes_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[4+serdes_pin] = 12+serdes_pin;
        else if (lifd_handle->var.serdes_lane[8+serdes_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[8+serdes_pin] = 12+serdes_pin;
        else
            PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
        }
        else if (serdes_pin < 8)
        {
        /* range 4-7 */
        if(lifd_handle->var.serdes_lane[0+serdes_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[0+serdes_pin-4] = 12+serdes_pin;
        else if (lifd_handle->var.serdes_lane[4+serdes_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[4+serdes_pin-4] = 12+serdes_pin;
        else if (lifd_handle->var.serdes_lane[8+serdes_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_lane[8+serdes_pin-4] = 12+serdes_pin;
        else
            PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
        }
        else
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG_TRACE("lifd serdes lanes:\n");
    for(i=0;i<12;i++)
    {
        PMC_LOG_TRACE("    i = %u, serdes_lane[i] == %u\n",i,lifd_handle->var.serdes_lane[i]);
    }

    PMC_RETURN(rc);

} /* lifd_internal_context_set */


/*******************************************************************************
* lifd_los_internal_context_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets up the LIFD context to map serdes LOS lanes from the line or system
*   side to LIFD internal lanes.
*
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   los_pin               - LOS serdes pin index
*   los_pin_val           - LOS serdes pin index
*   is_line_side          - LOS pin is comming from the line or system side:\n
*                           TRUE: line side\n
*                           FALSE: system side
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_los_internal_context_set(lifd_handle_t *lifd_handle,
                                               UINT32 los_pin,
                                               UINT32 los_pin_val,
                                               BOOL is_line_side)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if(is_line_side)
    {


        if (los_pin > LIFD_MAXIMUM_INTERNAL_LANE_NUM) {

            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);

        } else if (los_pin_val > LIFD_MAXIMUM_INTERNAL_LANE_NUM && los_pin_val !=  LIFD_SERDES_PORT_NO_LOS_SOURCE) {

            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);

        } else if (lifd_handle->var.serdes_los_lane[los_pin] != LIFD_SERDES_UNASSIGNED) {

            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        lifd_handle->var.serdes_los_lane[los_pin] = los_pin_val;

    }
    else /* System side */
    {
        if(los_pin < 4)
        {
        /* range 0-3 */
        if(lifd_handle->var.serdes_los_lane[0+los_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[0+los_pin] = 12+los_pin;
        else if (lifd_handle->var.serdes_los_lane[4+los_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[4+los_pin] = 12+los_pin;
        else if (lifd_handle->var.serdes_los_lane[8+los_pin] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[8+los_pin] = 12+los_pin;
        else
            PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
        }
        else if (los_pin < 8)
        {
        /* range 4-7 */
        if(lifd_handle->var.serdes_los_lane[0+los_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[0+los_pin-4] = 12+los_pin;
        else if (lifd_handle->var.serdes_los_lane[4+los_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[4+los_pin-4] = 12+los_pin;
        else if (lifd_handle->var.serdes_los_lane[8+los_pin-4] == LIFD_SERDES_UNASSIGNED)
            lifd_handle->var.serdes_los_lane[8+los_pin-4] = 12+los_pin;
        else
            PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
        }
        else
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(rc);

} /* lifd_los_internal_context_set */

/*******************************************************************************
* lifd_los_internal_context_unset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Tears down the LIFD context to map serdes LOS lanes from the line or system
*   side to LIFD internal lanes.
*
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   los_pin               - LOS serdes pin
*   is_line_side          - LOS pin is comming from the line or system side:\n
*                           TRUE: line side\n
*                           FALSE: system side
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_los_internal_context_unset(lifd_handle_t *lifd_handle,
                                                 UINT32 los_pin,
                                                 BOOL is_line_side)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if(is_line_side) {

        if(los_pin > LIFD_MAXIMUM_INTERNAL_LANE_NUM) {

            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        lifd_handle->var.serdes_los_lane[los_pin] = LIFD_SERDES_UNASSIGNED;

    /* System side */
    } else {

        if(los_pin < 4) {

            /* range 0-3 */
            if(lifd_handle->var.serdes_los_lane[0+los_pin] != LIFD_SERDES_UNASSIGNED) {

                lifd_handle->var.serdes_los_lane[0+los_pin] = LIFD_SERDES_UNASSIGNED;

            } else if (lifd_handle->var.serdes_los_lane[4+los_pin] != LIFD_SERDES_UNASSIGNED) {

                lifd_handle->var.serdes_los_lane[4+los_pin] = LIFD_SERDES_UNASSIGNED;

            } else if (lifd_handle->var.serdes_los_lane[8+los_pin] != LIFD_SERDES_UNASSIGNED) {

                lifd_handle->var.serdes_los_lane[8+los_pin] = LIFD_SERDES_UNASSIGNED;

            } else {
                PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
            }

        } else if (los_pin < 8) {

            /* range 4-7 */
            if(lifd_handle->var.serdes_los_lane[0+los_pin-4] != LIFD_SERDES_UNASSIGNED) {

                lifd_handle->var.serdes_los_lane[0+los_pin-4] = LIFD_SERDES_UNASSIGNED;
            } else if (lifd_handle->var.serdes_los_lane[4+los_pin-4] != LIFD_SERDES_UNASSIGNED) {
                lifd_handle->var.serdes_los_lane[4+los_pin-4] = LIFD_SERDES_UNASSIGNED;

            } else if (lifd_handle->var.serdes_los_lane[8+los_pin-4] != LIFD_SERDES_UNASSIGNED) {
                lifd_handle->var.serdes_los_lane[8+los_pin-4] = LIFD_SERDES_UNASSIGNED;

            } else {
                PMC_RETURN(LIFD_LOG_ERR_NO_MUX);
            }

        } else {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    PMC_RETURN(rc);

} /* lifd_los_internal_context_unset */

/*******************************************************************************
* lifd_internal_los_context_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Clears context associated with internal los lanes.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
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
PUBLIC void lifd_internal_los_context_clear(lifd_handle_t *lifd_handle)
{
    PMC_ENTRY();
    UINT32 i;

    for(i=0; i <= LIFD_MAXIMUM_INTERNAL_LANE_NUM; i++) {

        lifd_handle->var.serdes_los_lane[i] = LIFD_SERDES_UNASSIGNED;
    }

    PMC_RETURN();
} /* lifd_internal_los_context_clear */

/*******************************************************************************
* lifd_internal_context_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears the LIFD context which map serdes lanes from the line or system
*   side to LIFD internal lanes.
*
*
* INPUTS:
*   *lifd_handle          - Pointer to lifd handle instance to be operated on
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
PUBLIC void lifd_internal_context_clear(lifd_handle_t *lifd_handle)
{
    PMC_ENTRY();

    UINT32 i;

    for(i=0; i <= LIFD_MAXIMUM_INTERNAL_LANE_NUM; i++) {

        lifd_handle->var.serdes_lane[i] = LIFD_SERDES_UNASSIGNED;
    }

    PMC_RETURN();
} /* lifd_internal_context_clear */

/*******************************************************************************
* lifd_dsis_los_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure LIFD muxing for:\n
*       1) SERDES LOS lane <-> LIFD internal lane\n
*       2) LIFD internal lane <-> Subsystem lane
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   los_pin               - SERDES LOS pin
*   los_inv               - Is the SERDES LOS pin inverted\n
*                           0: No\n
*                           1: Yes
*   post_mux_los_mask     - Mask indicating which subsystem lanes use this LOS pin
*   enable                - Enable or disable the LOS pin
*   los_cfg               - LOS config structure. See lifd_los_cfg_t.
*   use_dlos_los_source   - Route internally generated DLOS signal to subsystem
*                           instead of external LOS pin.\n
*                           TRUE: Use DLOS\n
*                           FALSE: Use LOS
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_dsis_los_cfg(lifd_handle_t *lifd_handle,
                                        UINT32 los_pin,
                                        UINT32 los_inv,
                                        UINT32 post_mux_los_mask,
                                        BOOL8 enable,
                                        lifd_los_cfg_t *los_cfg,
                                        BOOL8 use_dlos_los_source)
{
    PMC_ENTRY();
    UINT32 i;
    UINT32 internal_los_lane = LIFD_SERDES_UNASSIGNED;
    UINT32 enable_int;
    UINT32 use_dlos_int;
    UINT32 pin_number_int = 0;
    BOOL8 is_los_source = TRUE;


    use_dlos_int = LIFD_DSIS_DLOS_DISABLE;
    is_los_source = TRUE;

    if (los_pin == LIFD_SERDES_PORT_NO_LOS_SOURCE) {

        enable_int = LIFD_DSIS_LOS_DISABLE;
        use_dlos_int = LIFD_DSIS_DLOS_DISABLE;
        is_los_source = FALSE;
    }
    else if (use_dlos_los_source == TRUE) {

        use_dlos_int = TRUE == enable ? LIFD_DSIS_DLOS_ENABLE : LIFD_DSIS_DLOS_DISABLE;
        enable_int = LIFD_DSIS_LOS_DISABLE;

    }
    else
    {
        enable_int = TRUE == enable ? LIFD_DSIS_LOS_ENABLE : LIFD_DSIS_LOS_DISABLE;
    }



    for(i=0;i<(LIFD_MAXIMUM_INTERNAL_LANE_NUM+1);i++)
    {
        if ((los_pin == LIFD_SERDES_PORT_NO_LOS_SOURCE) &&
            (lifd_handle->var.serdes_los_lane[i] == los_pin)) {

            pin_number_int = i;

        } else if (lifd_handle->var.serdes_los_lane[i] == los_pin) {

            pin_number_int = los_pin;
        }

        if (((los_pin == LIFD_SERDES_PORT_NO_LOS_SOURCE) &&
             (lifd_handle->var.serdes_los_lane[i] == los_pin)) ||
             (lifd_handle->var.serdes_los_lane[i] == los_pin)) {

                lifd_mux_field_range_DLOS_INV_set(NULL,
                                                  lifd_handle,
                                                  pin_number_int,
                                                  pin_number_int,
                                                  los_inv);

                lifd_mux_field_range_PIN_LOS_INV_set(NULL,
                                                     lifd_handle,
                                                     pin_number_int,
                                                     pin_number_int,
                                                     los_inv);

                lifd_mux_field_range_DLOS_EN_set(NULL,
                                                 lifd_handle,
                                                 pin_number_int,
                                                 pin_number_int,
                                                 use_dlos_int);

                lifd_mux_field_range_PIN_LOS_EN_set(NULL,
                                                    lifd_handle,
                                                    pin_number_int,
                                                    pin_number_int,
                                                    enable_int);
        }

        if (los_pin != LIFD_SERDES_PORT_NO_LOS_SOURCE) {

            /* write internal lane */
            if(lifd_handle->var.serdes_los_lane[i] == i &&
               lifd_handle->var.serdes_los_lane[i] == los_pin)
            {
                line_dsis_field_RX_I_LSEL_set(NULL, lifd_handle, i, 0);
                internal_los_lane = i;
            }
            else if (lifd_handle->var.serdes_los_lane[i] >= 12 &&
                     lifd_handle->var.serdes_los_lane[i] <= 15 &&
                     lifd_handle->var.serdes_los_lane[i] == los_pin)
            {

                line_dsis_field_RX_I_LSEL_set(NULL, lifd_handle, i, 1);
                internal_los_lane = i;
            }
            else if (lifd_handle->var.serdes_los_lane[i] >= 16 &&
                     lifd_handle->var.serdes_los_lane[i] <= 19 &&
                     lifd_handle->var.serdes_los_lane[i] == los_pin)
            {
                line_dsis_field_RX_I_LSEL_set(NULL, lifd_handle, i, 2);
                internal_los_lane = i;
            }
        }
    }

    if (los_pin == LIFD_SERDES_PORT_NO_LOS_SOURCE) {

        /* This is intentionally left as a no-op as we do not want
         * to return ERR_NO_MUX or set RX_LSEL since we aren't
         * using LOS. We just entered this function to explicitly turn it off */

    } else if (internal_los_lane == LIFD_SERDES_UNASSIGNED) {

        PMC_RETURN(LIFD_LOG_ERR_NO_MUX);

    } else {

        for(i=0;i<(LIFD_MAXIMUM_SUBSYSTEM_LANE+1);i++)
        {
            /* now i represents post_mux (subsystem) lane */
            if( post_mux_los_mask & (1<<i) )
            {
                line_dsis_field_RX_LSEL_set(NULL, lifd_handle, i, internal_los_lane);
            }

        }
    }

    /* Deal with the LOS config structure */
    if(los_pin < LIFD_SERDES_DSIS_LANES_MAX)
    {
        lifd_handle->var.los_ctxt[los_pin].active = is_los_source;
        PMC_MEMCPY(&lifd_handle->var.los_ctxt[los_pin].los_cfg, los_cfg, sizeof(lifd_los_cfg_t));
    }

    PMC_RETURN(PMC_SUCCESS);
} /* lifd_dsis_los_cfg */

/*******************************************************************************
* lifd_enet_clock_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects source for ENET clock - whether the source is a mate DSIS
*   (e.g. SYS ENET DSIS for LINE ENET) or internal lane (i.e. one of the
*   SERDES ports).\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   enet_clk_enable       - enable transmit clock from mate DSIS (TRUE) or
*                           from internal lane (FALSE)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_enet_clock_select(lifd_handle_t *lifd_handle,
                                        UINT32 subsystem_lane,
                                        BOOL enet_clk_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if (lifd_internal_lane_param_check(subsystem_lane) == PMC_SUCCESS) {

        /* select transmit clock for ENET subsystem */
        line_dsis_field_range_ENET_TX_CLK_SEL_set(NULL, lifd_handle, subsystem_lane, subsystem_lane, enet_clk_enable);

        /* en/disable transmit clock */
        line_dsis_field_TX_CLK_EN_set(NULL, lifd_handle, subsystem_lane, LIFD_ENET_CLK_ENABLE);

        rc = PMC_SUCCESS;
    }
    else
        rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_RETURN(rc);
} /* lifd_enet_clock_select */

/*******************************************************************************
* lifd_serdes_c_ckctl_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Selects the C_CKCTL source (i.e. subsystem) for each internal lane.
*   This also drives C_CKCTL on the equivalent SERDES lanes 0 to 19.
*   Note that lane re-ordering is not possible for C_CKCTL (ACB) selection.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane (0 to 19)
*   clk_source            - source block for the ACB signal
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Certain internal -> SERDES lane combinations are not possible.
*   Following is a diagram indicating which internal lanes may be
*   mapped to which SERDES lanes:\n
*\n
*   Internal Lane [11:00]  -> SERDES Lane [11:00]\n
*\code
*   Internal Lane:     [11][10][09][08][07][06][05][04][03][02][01][00]\n
*   SERDES Lane [12]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [13]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [14]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [15]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*   SERDES Lane [16]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [17]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [18]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [19]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*\endcode
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_c_ckctl_configure(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_lane,
                                               lifd_sig_src_type_t clk_source)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    UINT32 internal_lane = lifd_handle->var.internal_lane_tx[serdes_lane];
    UINT32 set_value = 0;
    UINT32 ret_value;

    PMC_ENTRY();

    switch (clk_source) {
    case LIFD_SRC_CBRC:
        set_value = 0;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_IPM_CBRC:
        set_value = 1;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_ENET:
        set_value = 2;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_LPM:
        set_value = 3;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_EPM:
        set_value = 4;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_DIG_M1:
        set_value = 5;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_IPM_ENET:
        set_value = 6;
        rc = PMC_SUCCESS;
        break;
    default:                 /* all other sources are not valid */
        break;
    }

    if (rc == PMC_SUCCESS) {

        if ((lifd_serdes_lane_param_check(serdes_lane) == PMC_SUCCESS) &&
            ((rc = lifd_internal_lane_param_check(internal_lane)) == PMC_SUCCESS)) {

            line_dsis_field_ACB_IF_SEL_set(NULL, lifd_handle, internal_lane, set_value);

            /* select for SERDES lane 12 to 19 */
            if(serdes_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM)
            {
                /* check failure is interpreted as no SERDES lane (12 to 19) is configured */
                if (lifd_internal_to_serdes_param_check(internal_lane, serdes_lane, &ret_value) ==
                    PMC_SUCCESS) {
                                                      /* SERDES lane offset is 0 to 7 */
                    line_dsis_field_ACB_O_SEL_set(NULL, lifd_handle,
                                            (serdes_lane - (LIFD_MAXIMUM_INTERNAL_LANE_NUM + 1)),
                                            ret_value);
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* lifd_serdes_c_ckctl_configure */

/*******************************************************************************
*  lifd_serdes_c_ckctl_source_lane_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recovers the internal lane used for an ACB CKCTL signal.\n\n
*
*   There are 20 Serdes Lanes driven out.  SERDES LANES 11:0 route to
*   LINE_S16 and SERDES LANES 19:12 route to SYS_S16 lanes.\n\n
*
*   This function uses the same subfunction as lifd_serdes_c_ckctl_configure()
*   to determine what value of ACB_O_SEL is going to be applied.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane (0 to 19)
*
* OUTPUTS:
*   internal_acb_lane_ptr - internal ACB lane driving the data source.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   Certain internal -> SERDES lane combinations are not possible.
*   Following is a diagram indicating which internal lanes may be
*   mapped to which SERDES lanes:\n
*\n
*   Internal Lane [11:00]  -> SERDES Lane [11:00]\n
*\code
*   Internal Lane:     [11][10][09][08][07][06][05][04][03][02][01][00]\n
*   SERDES Lane [12]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [13]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [14]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [15]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*   SERDES Lane [16]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [17]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [18]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [19]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*\endcode
*******************************************************************************/
PUBLIC void lifd_serdes_c_ckctl_source_lane_get(lifd_handle_t *lifd_handle,
                                                UINT32 serdes_lane,
                                                UINT32 *internal_acb_lane_ptr)
{
    UINT32 internal_lane = 99;
    UINT32 acb_o_sel_val = 99;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(serdes_lane < LIFD_SERDES_DSIS_LANES_MAX, LIFD_ERR_CODE_ASSERT, 0, 0);

    internal_lane = lifd_handle->var.internal_lane_tx[serdes_lane];

    /* select for SERDES lane 12 to 19 */
    if(serdes_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM)    {
        result = lifd_internal_to_serdes_param_check(internal_lane, serdes_lane, &acb_o_sel_val);
        PMC_ASSERT(result == PMC_SUCCESS, LIFD_ERR_CODE_ASSERT, 0, 0);
        PMC_ASSERT(acb_o_sel_val <= LIFD_ACB_O_SEL_MAX_VALUE, LIFD_ERR_CODE_ASSERT, 0, 0);
        if(serdes_lane < 16) {
            *internal_acb_lane_ptr = acb_o_sel_val * LIFD_ACB_ON_SERDES_MULT + (serdes_lane - (LIFD_MAXIMUM_INTERNAL_LANE_NUM + 1));
        } else if(serdes_lane < LIFD_SERDES_DSIS_LANES_MAX) {
            *internal_acb_lane_ptr = acb_o_sel_val * LIFD_ACB_ON_SERDES_MULT + (serdes_lane - (LIFD_MAXIMUM_INTERNAL_LANE_NUM + 1 + 4));
        } else {
            PMC_ASSERT(FALSE, LIFD_ERR_CODE_ASSERT, 0, 0);
        }
    } else {
        *internal_acb_lane_ptr = internal_lane;
    }

    PMC_RETURN();
} /* lifd_serdes_c_ckctl_source_lane_get */


/*******************************************************************************
* lifd_sfi_c_ckctl_source_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects the C_CKCTL source (i.e. subsystem) for each SFI 5.1 interface.\n\n
*
*   If more than one interface requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   clk_source            - source block for the ACB signal
*   sfi_num               - number of the SFI5.1 interface to configure
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_sfi_c_ckctl_source_select(lifd_handle_t *lifd_handle,
                                                lifd_sig_src_type_t clk_source,
                                                lifd_sfi5point1_int_t sfi_num)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;
    UINT32 set_value;

    PMC_ENTRY();

    switch (clk_source) {
    case LIFD_SRC_CBRC:
        set_value = 0;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_IPM_CBRC:
        set_value = 1;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_LPM:
        set_value = 2;
        rc = PMC_SUCCESS;
        break;
    case LIFD_SRC_OTN_EPM:
        set_value = 3;
        rc = PMC_SUCCESS;
        break;
    default:                 /* all other sources are not valid */
        break;
    }

    if (rc == PMC_SUCCESS) {
        switch (sfi_num) {

        case LIFD_SFI5POINT1_ONE:
            lifd_mux_field_SFI1_ACB_IF_SEL_set( NULL, lifd_handle, set_value);
            break;

        case LIFD_SFI5POINT1_TWO:
            lifd_mux_field_SFI2_ACB_IF_SEL_set( NULL, lifd_handle, set_value);
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
        }
    }

    PMC_RETURN(rc);
} /* lifd_sfi_c_ckctl_source_select */

/*******************************************************************************
* lifd_sfi_subsystem_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Selects which SFI5.1 interface routes to which subsystem (CBRC or OTN)
*   Selects whether SFI5.1 - 1 and SFI5.1 - 2 should be swapped.
*   Selects whether the RXS signal is inverted and enabled/disabled.
*   Enables/disables SFI5.1 (1 or 2) SERDES interface\n\n
*
*   If more than one interface requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   sfi_num               - number of the SFI5.1 interface to be configured
*   swap_sfi_one_and_two  - if TRUE, then SFI5.1 interfaces are swapped
*   subsystem_type        - subsystem this SFI5.1 interface should be routed to
*   enable                - enable (TRUE) or disable (FALSE) the specified SFI5.1
*                           SERDES interface
*   rxs_config            - RXS configuration structure to be applied to SFI5.1
*                           interface
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   Only the XIFD_DSIS_OTN and XIFD_DSIS_CBRC subsystems may be specifed.
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_sfi_subsystem_select(lifd_handle_t *lifd_handle,
                                           lifd_sfi5point1_int_t sfi_num,
                                           BOOL swap_sfi_one_and_two,
                                           xifd_dsis_type_t subsystem_type,
                                           BOOL enable,
                                           lifd_rxs_cfg_t rxs_config)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;
    UINT32 tx_connect_path = LIFD_SFI5POINT1_TO_CBRC;
    UINT32 cbrc_connect_status = LIFD_SFI5POINT1_DISABLED;
    UINT32 otn_connect_status = LIFD_SFI5POINT1_DISABLED;

    PMC_ENTRY();

    if (enable == TRUE) {

        switch (subsystem_type) {
           case XIFD_DSIS_CBRC:
                tx_connect_path = LIFD_SFI5POINT1_TO_CBRC;
                cbrc_connect_status = LIFD_SFI5POINT1_ENABLED;
                rc = PMC_SUCCESS;
                break;
            case XIFD_DSIS_OTN:
                tx_connect_path = LIFD_SFI5POINT1_TO_OTN;
                otn_connect_status = LIFD_SFI5POINT1_ENABLED;
                rc = PMC_SUCCESS;
                break;
            default:
                rc = PMC_ERR_INVALID_PARAMETERS;
                break;
        }

    } else {
        /* We are disabling. Use above defaults and proceed */
        rc = PMC_SUCCESS;
    }

    if (rc == PMC_SUCCESS) {
        switch (sfi_num) {

        case LIFD_SFI5POINT1_ONE:

            lifd_mux_field_SFI_IF_SWAP_EN_set( NULL, lifd_handle, swap_sfi_one_and_two);
      	   if (TRUE == swap_sfi_one_and_two) {
      	      lifd_mux_field_SFI2_TX_IFSEL_set( NULL, lifd_handle, tx_connect_path);
         		lifd_mux_field_SFI2_OTN_EN_set( NULL, lifd_handle, otn_connect_status);
         		lifd_mux_field_SFI2_CBRC_EN_set( NULL, lifd_handle, cbrc_connect_status);
      	   } else {
      	      lifd_mux_field_SFI1_TX_IFSEL_set( NULL, lifd_handle, tx_connect_path);
         		lifd_mux_field_SFI1_OTN_EN_set( NULL, lifd_handle, otn_connect_status);
         		lifd_mux_field_SFI1_CBRC_EN_set( NULL, lifd_handle, cbrc_connect_status);
      	   }

            lifd_mux_field_SFI1_RXS_INV_set( NULL, lifd_handle, rxs_config.rxs_inv);
            lifd_mux_field_SFI1_RXS_EN_set( NULL, lifd_handle, rxs_config.rxs_en);

            lifd_mux_field_SFI1_EN_set( NULL, lifd_handle, enable);

            break;

        case LIFD_SFI5POINT1_TWO:

            lifd_mux_field_SFI_IF_SWAP_EN_set( NULL, lifd_handle, swap_sfi_one_and_two);
      	   if (TRUE == swap_sfi_one_and_two) {
               lifd_mux_field_SFI1_TX_IFSEL_set( NULL, lifd_handle, tx_connect_path);
         		lifd_mux_field_SFI1_OTN_EN_set( NULL, lifd_handle, otn_connect_status);
         		lifd_mux_field_SFI1_CBRC_EN_set( NULL, lifd_handle, cbrc_connect_status);
      	   } else {
               lifd_mux_field_SFI2_TX_IFSEL_set( NULL, lifd_handle, tx_connect_path);
         		lifd_mux_field_SFI2_OTN_EN_set( NULL, lifd_handle, otn_connect_status);
         		lifd_mux_field_SFI2_CBRC_EN_set( NULL, lifd_handle, cbrc_connect_status);
      	   }
            lifd_mux_field_SFI2_RXS_INV_set( NULL, lifd_handle, rxs_config.rxs_inv);
            lifd_mux_field_SFI2_RXS_EN_set( NULL, lifd_handle, rxs_config.rxs_en);

            lifd_mux_field_SFI2_EN_set( NULL, lifd_handle, enable);

            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
        }
    }

    PMC_RETURN(rc);
} /* lifd_sfi_subsystem_select */

/*
** Private Functions
*/

/*******************************************************************************
* lifd_serdes_los_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LIFD PIN LOS.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - serdes lane: Physical LOS pin (0 to 19)
*   *los_cfg              - LOS configuration structurePoine
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_los_cfg_get(lifd_handle_t   *lifd_handle,
                                         UINT32           serdes_lane,
                                         lifd_los_cfg_t  *los_cfg)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    PMC_ASSERT(lifd_handle != NULL, LIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(los_cfg != NULL, LIFD_LOG_ERR_INVALID_PARAMETERS,0,0);
    PMC_ASSERT(serdes_lane <= LIFD_MAXIMUM_SERDES_LANE_NUM, LIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    if (TRUE == lifd_handle->var.los_ctxt[serdes_lane].active)
    {
        PMC_MEMCPY(los_cfg, &lifd_handle->var.los_ctxt[serdes_lane].los_cfg, sizeof(lifd_los_cfg_t));
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* lifd_serdes_los_cfg_get */

/*******************************************************************************
* lifd_serdes_los_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is called by digi_serdes_los_force to force a LOS state in 
*   the subsystem connected with LIFD.
*
* INPUTS:
*   *lifd_handle   - pointer to lifd handle instance to be operated on
*   serdes_lane    - serdes lane id (0 to 19)
*   dlos_en        - dlos enable
*   los_pol_inv    - los polarity invert
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_los_force(lifd_handle_t *lifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 dlos_en, BOOL8 los_pol_inv)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(lifd_handle != NULL, LIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    lifd_mux_field_range_DLOS_EN_set(NULL, lifd_handle, serdes_lane, serdes_lane, dlos_en);
    lifd_mux_field_range_LOS_POL_INV_set(NULL, lifd_handle, serdes_lane, serdes_lane, los_pol_inv);

    lifd_handle->var.los_ctxt[serdes_lane].los_cfg.los_pol_inv = los_pol_inv;

    PMC_RETURN(rc);
} /* lifd_serdes_los_force */

/*******************************************************************************
* lifd_serdes_los_force_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is called by digi_serdes_los_force_get to get LOS force config in 
*   the subsystem connected with LIFD.
*
* INPUTS:
*   *lifd_handle   - pointer to lifd handle instance to be operated on
*   serdes_lane    - serdes lane id (0 to 19)
*
* OUTPUTS:
*   *dlos_en       - dlos enable
*   *los_pol_inv   - los polarity invert
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_los_force_get(lifd_handle_t *lifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 *dlos_en, BOOL8 *los_pol_inv)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(lifd_handle != NULL, LIFD_LOG_ERR_INVALID_PARAMETERS,0,0);

    *dlos_en = lifd_mux_field_range_DLOS_EN_get(NULL, lifd_handle, serdes_lane, serdes_lane);
    *los_pol_inv = lifd_mux_field_range_LOS_POL_INV_get(NULL, lifd_handle, serdes_lane, serdes_lane);

    PMC_RETURN(rc);
} /* lifd_serdes_los_force_get */

/* LCOV_EXCL_START */
/*******************************************************************************
* lifd_serdes_dsis_rx_sel_and_rx_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the rx clock and data lines per subsystem lane and
*   then en/disable the lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to sifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   internal_rx_lane      - the internal rx lane to use.
*   clock_enable          - set to one of:\n
*                           LIFD_CBRC_CLK_ENABLE\n
*                           LIFD_OTN_CLK_ENABLE\n
*                           LIFD_ENET_CLK_ENABLE\n
*                           or to LIFD_CLK_DISABLE in order to disable.
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
PUBLIC void lifd_serdes_dsis_rx_sel_and_rx_en_set(lifd_handle_t *lifd_handle,
                                                  UINT32 subsystem_lane,
                                                  UINT32 internal_rx_lane,
                                                  UINT32 clock_enable)
{
    PMC_ENTRY();

    line_dsis_field_RX_SEL_set(NULL, lifd_handle, subsystem_lane, internal_rx_lane);
    line_dsis_field_RX_EN_set(NULL, lifd_handle, subsystem_lane, clock_enable);

    PMC_RETURN();
} /* lifd_serdes_dsis_rx_sel_and_rx_en_set */
/* LCOV_EXCL_STOP */
/*******************************************************************************
* lifd_serdes_to_internal_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*             This function validates lane parameters and maps SERDES lanes to an
*             internal index (after validating the map).
*
* INPUTS:
*   s_lane                 - SERDES lane (0 to 19)
*   i_lane                 - internal lane (0 to 11)
*
* OUTPUTS:
*   *ret_value_ptr         - pointer to SERDES to internal mapping index\n
*                            0 -- direct map internal to SERDES lanes 0 to 11\n
*                            1 -- internal to SERDES lanes 12 to 15\n
*                            2 -- internal to SERDES lanes 16 to 19
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_serdes_to_internal_param_check(UINT32 s_lane,
                                                      UINT32 i_lane,
                                                      UINT32* ret_value_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if(rc == PMC_SUCCESS)
        rc = lifd_serdes_lane_param_check(s_lane);

    if(rc == PMC_SUCCESS)
        rc = lifd_internal_lane_param_check(i_lane);

    if(rc == PMC_SUCCESS)
    {
        if (s_lane == i_lane) {

            *ret_value_ptr = 0;                  /* direct map for SERDES lanes 0 to 11 */
            rc = PMC_SUCCESS;

        } else if ( s_lane > LIFD_MAXIMUM_INTERNAL_LANE_NUM ) {

            UINT32 s_lane_rem = s_lane % 4;
            UINT32 i_lane_rem = i_lane % 4;

            if (s_lane_rem == i_lane_rem) {      /* check validity of mapping */

                if ((i_lane_rem + LIFD_MAXIMUM_INTERNAL_LANE_NUM + 1) == s_lane) {

                    *ret_value_ptr = 1;          /* SERDES lanes 12 to 15 */
                    rc = PMC_SUCCESS;

                } else {

                    *ret_value_ptr = 2;          /* SERDES lanes 16 to 19 */
                    rc = PMC_SUCCESS;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* lifd_serdes_to_internal_param_check */

/*******************************************************************************
* lifd_internal_to_serdes_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*             This function validates lane parameters and maps SERDES lanes
*             12 to 19 to an internal index (after validating the map).
*
* INPUTS:
*   i_lane                 - internal lane (0 to 11)
*   s_lane                 - SERDES lane (12 to 19)
*
* OUTPUTS:
*   *ret_value_ptr         - SERDES to internal mapping index\n
*                            0 -- internal lanes 0 to 3\n
*                            1 -- internal lanes 4 to 7\n
*                            2 -- internal lanes 8 to 11
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_internal_to_serdes_param_check(UINT32 i_lane,
                                                      UINT32 s_lane,
                                                      UINT32 *ret_value_ptr)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((lifd_internal_lane_param_check(i_lane) == PMC_SUCCESS) &&
        (s_lane >= LIFD_MINIMUM_INTERNAL_LANE_NUM) &&
        (s_lane <= LIFD_MAXIMUM_SERDES_LANE_NUM)) {

        UINT32 i_lane_rem = i_lane % 4;
        UINT32 s_lane_rem = s_lane % 4;

        if (i_lane_rem == s_lane_rem) {

            if (i_lane <= 3)
                *ret_value_ptr = 0;
            else if (i_lane <= 7)
                *ret_value_ptr = 1;
            else
                *ret_value_ptr = 2;

            rc = PMC_SUCCESS;
        }
    }

    PMC_RETURN(rc);
} /* lifd_internal_to_serdes_param_check */

/*******************************************************************************
* lifd_internal_lane_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the internal lane number (i.e. between 0 & 11).
*
* INPUTS:
*   i_lane                 - internal lane
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_internal_lane_param_check(UINT32 i_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((i_lane >= LIFD_MINIMUM_INTERNAL_LANE_NUM) &&
        (i_lane <= LIFD_MAXIMUM_INTERNAL_LANE_NUM)) {

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* lifd_internal_lane_param_check */

/*******************************************************************************
* lifd_serdes_lane_param_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the SERDES lane number (i.e. between 0 & 19).
*
* INPUTS:
*   s_lane                 - SERDES lane
*
* OUTPUTS:
*   none
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_serdes_lane_param_check(UINT32 s_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((s_lane >= LIFD_MINIMUM_SERDES_LANE_NUM) &&
        (s_lane <= LIFD_MAXIMUM_SERDES_LANE_NUM)) {

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* lifd_serdes_lane_param_check */

/*******************************************************************************
* lifd_internal_lane_configure
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   This sets up the lookup table for the internal lane which is used by
*   lifd_serdes_subsystem_configure, to determine the correct internal lane
*   and then set up the data pather.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.\n
*
*   Multicasting several lanes to the same subsystem may result in
*   undetermined behaviour and is not supported. It is up to the user to
*   ensure that these configurations do not occur.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_rx_lane        - RX SERDES lane number
*   serdes_tx_lane        - TX SERDES lane number
*   internal_rx_lane      - RX internal lane number to map to.
*   internal_tx_lane      - TX internal lane number to map to.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*   Certain SERDES -> internal lane combinations are not possible.
*   Following is a diagram indicating which SERDES lanes may be mapped
*   to which internal lanes:\n
*\n
*   SERDES Lane [11:00]  -> Internal Lane [11:00]\n
*\code
*   Internal Lane:     [11][10][09][08][07][06][05][04][03][02][01][00]\n
*   SERDES Lane [12]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [13]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [14]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [15]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*   SERDES Lane [16]:: [  ][  ][  ][08][  ][  ][  ][04][  ][  ][  ][00]\n
*   SERDES Lane [17]:: [  ][  ][09][  ][  ][  ][05][  ][  ][  ][01][  ]\n
*   SERDES Lane [18]:: [  ][10][  ][  ][  ][06][  ][  ][  ][02][  ][  ]\n
*   SERDES Lane [19]:: [11][  ][  ][  ][07][  ][  ][  ][03][  ][  ][  ]\n
*\endcode
*******************************************************************************/
PRIVATE PMC_ERROR lifd_internal_lane_configure(lifd_handle_t *lifd_handle,
                                               UINT32 serdes_rx_lane,
                                               UINT32 serdes_tx_lane,
                                               UINT32 internal_rx_lane,
                                               UINT32 internal_tx_lane)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;
    PMC_ENTRY();
    UINT32 ret_val;

    PMC_ASSERT((lifd_handle != NULL), LIFD_ERR_CODE_ASSERT, 0, 0);

    if (lifd_serdes_to_internal_param_check(serdes_rx_lane, internal_rx_lane, &ret_val) != PMC_SUCCESS) {

    } else if (lifd_serdes_to_internal_param_check(serdes_tx_lane, internal_tx_lane, &ret_val) != PMC_SUCCESS) {

    } else {

        lifd_handle->var.internal_lane_rx[serdes_rx_lane] = internal_rx_lane;
        lifd_handle->var.internal_lane_tx[serdes_tx_lane] = internal_tx_lane;
        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* lifd_internal_lane_configure */


/*******************************************************************************
* lifd_serdes_subsystem_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   clock_enable          - set to one of:\n
*                           LIFD_CBRC_CLK_ENABLE\n
*                           LIFD_OTN_CLK_ENABLE\n
*                           LIFD_ENET_CLK_ENABLE\n
*                           or to LIFD_CLK_DISABLE in order to disable.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_serdes_subsystem_set(lifd_handle_t *lifd_handle,
                                            UINT32 serdes_lane,
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if ((lifd_serdes_lane_param_check(serdes_lane) == PMC_SUCCESS) &&
        (lifd_internal_lane_param_check(subsystem_lane) == PMC_SUCCESS)) {

        /* en/disable receive clock */
        line_dsis_field_RX_EN_set(NULL, lifd_handle, subsystem_lane, clock_enable);

        /* en/disable transmit clock */
        line_dsis_field_TX_CLK_EN_set(NULL, lifd_handle, subsystem_lane, clock_enable);

        /* en/disable transmit on SERDES lane */
        if(clock_enable != LIFD_CLK_DISABLE)
        {
            line_dsis_field_range_TX_LANE_ENABLE_set(NULL, lifd_handle, serdes_lane, serdes_lane, 1);
        } else {
            line_dsis_field_range_TX_LANE_ENABLE_set(NULL, lifd_handle, serdes_lane, serdes_lane, 0);
        }

        rc = PMC_SUCCESS;
    }

    PMC_RETURN(rc);
} /* lifd_serdes_subsystem_set */

/*******************************************************************************
* lifd_subsytem_lane_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Sets the SERDES lane plus all clocks on the subsystem lane.\n\n
*
*   If more than one lane requires configuration, this function needs to be
*   called multiple times.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   serdes_lane           - TX SERDES lane number
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   clock_enable          - set to one of:\n
*                           LIFD_CBRC_CLK_ENABLE\n
*                           LIFD_OTN_CLK_ENABLE\n
*                           LIFD_ENET_CLK_ENABLE
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lifd_subsytem_lane_enable(lifd_handle_t *lifd_handle,
                                            UINT32 serdes_lane,
                                            UINT32 subsystem_lane,
                                            UINT32 clock_enable)
{
    PMC_ERROR rc = PMC_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    rc = lifd_serdes_subsystem_set(lifd_handle,
                                   serdes_lane,
                                   subsystem_lane,
                                   clock_enable);

    PMC_RETURN(rc);
} /* lifd_serdes_subsystem_enable */

/*******************************************************************************
* lifd_serdes_tx_if_sel_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Updates TX interface select according to given subsystem type.
*
* INPUTS:
*   *lifd_handle          - pointer to lifd handle instance to be operated on
*   subsystem_lane        - subsystem lane that internal lane is mapped to
*   subsystem_type        - direct the lane to this subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR lifd_serdes_tx_if_sel_update(lifd_handle_t    *lifd_handle, 
                                              UINT32            subsystem_lane, 
                                              xifd_dsis_type_t  subsystem_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 data_enable = 3;

    PMC_ENTRY();

    switch (subsystem_type) {
    case XIFD_DSIS_CBRC:
        data_enable = LIFD_CBRC_DATA_ENABLE;               /* enable CBRC data */
        break;
    case XIFD_DSIS_OTN:
        data_enable = LIFD_OTN_DATA_ENABLE;                /* enable OTN data */
        break;
    case XIFD_DSIS_ENET:
        data_enable = LIFD_ENET_DATA_ENABLE;               /* enable ENET data */
        break;
    default:
        rc = PMC_ERR_INVALID_PARAMETERS;
        break;
    }
    
    if (PMC_SUCCESS == rc) 
    {
        /* select subsystem that drives internal transmit lane before reordering */
        line_dsis_field_TX_IF_SEL_set(NULL, lifd_handle, subsystem_lane, data_enable);
    }

    PMC_RETURN(rc);
}/*lifd_serdes_tx_if_sel_update*/



/*
** End of file
*/
