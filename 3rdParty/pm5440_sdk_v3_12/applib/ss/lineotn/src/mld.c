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
#include "sfi51_io.h"
#include "sfis_rx_io.h"
#include "sfis_tx_io.h"
#include "opsm4_io.h"
#include "opsm23_io.h"
#include "otn_mld_io.h"
#include "lineotn.h"

#include "sysotn_io.h"
#include "sysotn_opsm4_io.h"
#include "sysotn_sfis_rx_io.h"
#include "sysotn_sfis_tx_io.h"

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
** Forward declarations
*/

PRIVATE PMC_ERROR mld_otl_prov(lineotn_handle_t   *lineotn_handle,
                               lineotn_cfg_type_t lineotn_cfg,
                               mld_ifc_type_t     interface_type,
                               UINT8              dsi_stream);

PRIVATE void mld_otl_deprov(lineotn_handle_t *lineotn_handle,
                            mld_ifc_type_t   interface_type,
                            UINT8            dsi_stream);

PRIVATE PMC_ERROR mld_otl_100G_prov(lineotn_handle_t *lineotn_handle);

PRIVATE void mld_otl_100G_deprov(lineotn_handle_t *lineotn_handle);

PRIVATE PMC_ERROR mld_sfis_n_10_100G_prov(lineotn_handle_t *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type);

PRIVATE void mld_sfis_n_10_100G_deprov(lineotn_handle_t *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type);

PRIVATE PMC_ERROR mld_2x_sfis_sfi51_prov(lineotn_handle_t   *lineotn_handle,
                                         lineotn_deskew_type_t deskew_type,
                                         mld_ifc_type_t     interface_type,
                                         UINT8              dsi_stream);
PRIVATE PMC_ERROR mld_2x_sfis_sfi51_deprov(lineotn_handle_t   *lineotn_handle,
                                           lineotn_deskew_type_t deskew_type,
                                           mld_ifc_type_t     interface_type,
                                           UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfi51_otl34_prov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfi51_otl34_deprov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfi51_4x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream);


PRIVATE PMC_ERROR mld_2x_sfi51_4x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_n_4_7x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_n_4_7x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_otl34_3x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_otl34_3x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                                lineotn_deskew_type_t deskew_type,
                                                mld_ifc_type_t     interface_type,
                                                UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_1x_sfi51_otl_prov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        lineotn_cfg_type_t lineotn_cfg,
                                        UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_1x_sfi51_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          lineotn_cfg_type_t lineotn_cfg,
                                          UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_sfi51_otl_prov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          lineotn_cfg_type_t lineotn_cfg,
                                          UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfis_sfi51_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            lineotn_cfg_type_t lineotn_cfg,
                                            UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfis_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfis_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfi51_sfis_prov(lineotn_handle_t   *lineotn_handle,
                                         lineotn_deskew_type_t deskew_type,
                                         mld_ifc_type_t     interface_type,
                                         UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_2x_sfi51_sfis_deprov(lineotn_handle_t   *lineotn_handle,
                                           lineotn_deskew_type_t deskew_type,
                                           mld_ifc_type_t     interface_type,
                                           UINT8              dsi_stream);

PRIVATE void mld_sfis_40G_prov(lineotn_handle_t   *lineotn_handle,
                               UINT8              dsi_stream);

PRIVATE void mld_sfis_40G_deprov(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_sfi51_prov(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream,
                                 lineotn_cfg_type_t lineotn_cfg);

PRIVATE PMC_ERROR mld_sfi51_deprov(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream,
                                   lineotn_cfg_type_t lineotn_cfg );


PRIVATE PMC_ERROR mld_opsm23_sfi_prov(lineotn_handle_t   *lineotn_handle,
                                      lineotn_deskew_type_t deskew_type,
                                      UINT8              opsm23_idx);

PRIVATE PMC_ERROR mld_opsm23_sfi_deprov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        UINT8              opsm23_idx);

PRIVATE PMC_ERROR mld_opsm23_otl_prov(lineotn_handle_t   *lineotn_handle,
                                      BOOL               opsm_mode_otl_34,
                                      UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_opsm23_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                        BOOL               opsm_mode_otl_34,
                                        UINT8              dsi_stream);

PRIVATE PMC_ERROR mld_opsm4_prov(lineotn_handle_t   *lineotn_handle,
                                 lineotn_deskew_type_t deskew_type );

PRIVATE void mld_opsm4_deprov(lineotn_handle_t   *lineotn_handle);


PRIVATE void mld_opsm23_common_cfg(lineotn_handle_t *lineotn_handle,
                                   UINT8             opsm23_idx);

PRIVATE void mld_rcp_defect_cfg(lineotn_handle_t    *lineotn_handle,
                                UINT8                dsi_stream,
                                lineotn_frame_rate_t frame_rate,
                                BOOL                 rcp_defect_enable);



PRIVATE void mld_sfis_n_reg_cleanup(lineotn_handle_t *lineotn_handle,
                                    UINT32            dsi_stream);

PRIVATE void mld_sfi51_reg_cleanup(lineotn_handle_t *lineotn_handle,
                                   UINT32            dsi_stream);


PRIVATE void mld_sfis_n_rx_alarm_cfg(lineotn_handle_t *lineotn_handle,
                                     UINT32            opsm_idx,
                                     BOOL8             enable);

PRIVATE PMC_ERROR lineotn_mld_sfis_slice_get(lineotn_handle_t  *lineotn_handle,
                                             UINT8              dsi_stream,
                                             UINT8             *slice_id_ptr);

PRIVATE void lineotn_mld_sfis_odd_parity_reset(lineotn_handle_t    *lineotn_handle,
                                               UINT8                slice);

/*
** Public Functions
*/

/*******************************************************************************
* mld_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring MLD block into configurable state
*
* INPUTS:
*   lineotn_handle       - handle to the MLD instance to be operated on
*   energy_state         - Energy state to put MLD into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success or a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mld_init(lineotn_handle_t     *lineotn_handle,
                     pmc_energy_state_t   energy_state)
{

    PMC_ENTRY();

    /* Check Arguments */
    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PTR, 0, 0);

    switch (energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        mld_blk_init(lineotn_handle);
        break;

    case PMC_ENERGY_STATE_RESET:
        if(lineotn_handle->var.lineotn_mode)
        {
            mld_pwr_mode_config_set( lineotn_handle, MLD_BLOCK_HW,TRUE );
            mld_reset_config_set(lineotn_handle,TRUE);
            mld_var_init(&lineotn_handle->var); /* reset context variables */
        }
        break;

    default:
        PMC_ASSERT(FALSE, LINEOTN_ERR_INVALID_PARAMETERS, energy_state, 0);
        break;
    } /* end switch */

    PMC_RETURN();
} /* mld_init */


/*******************************************************************************
* mld_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the
*   MLD instance.  It will test the resource state for all resources within
*   the MLD instance if any are not in LAST_MLD_IFC_TYPE (start state equivalent)
*   state then FALSE is returned, else TRUE.  Should TRUE be returned it can be
*   assumed that no resources are provisioned against the MLD and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
* INPUTS:
*   lineotn_handle        - handle to the MLD instance to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state;
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL mld_start_state_test(lineotn_handle_t *lineotn_handle)
{
    UINT32 dsi_stream = 0;
    BOOL   rc = TRUE;  /* no resources provisioned */

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL, OTU_FRM_ERR_INVALID_PTR, 0, 0);

    for (dsi_stream = 0; dsi_stream < 12; dsi_stream++)
    {
        if(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc != LAST_MLD_IFC_TYPE)
        {
            rc = FALSE; /* found at least one resource that is allocated ! */
            break;
        }
    }

    PMC_RETURN( rc );

} /* mld_start_state_test */


/*******************************************************************************
* mld_var_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes mld block logical variables.
*
* INPUTS:
*   *var_ptr            - pointer to LINEOTN variable configuration
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
PUBLIC void mld_var_init(lineotn_var_t *var_ptr)
{
    UINT8 i;
    /* var_ptr->mld_curr_num_otu_configured bit 31-30 was re-used for sysotn 
       interrupt type, mask 0xC0000000 to ignore bit 31-30 */
    var_ptr->mld_curr_num_otu_configured &= SYSOTN_INT_TYPE_STORAGE_MASK;

    PMC_ENTRY();

    for(i = 0; i < 12; i++)
    {
        /* set scrambling by default to be at the OTU_FRM and not the OPSM   */
        /* this means that from a fec point of view we are scrambling before */
        /* the encoder and after decoder                                     */
        var_ptr->lineotn_dsi_dist[i].scramble_in_opsm = FALSE;
        var_ptr->lineotn_dsi_dist[i].bergen_enabled = FALSE;
    }
    PMC_RETURN();
}


/*******************************************************************************
* mld_blk_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the MLD block as part of handle_init, by performing
*   the following actions:\n
*       1. Take the mld block out of reset \n
*       2. Take the mld block out of low power \n
*       3. Leave all HW modules (SFIS,SFI51,OPSM ...) at low power \n
*       4. Set all precoders/bergens to bypass mode
*
* INPUTS:
*   lineotn_handle            - pointer to LINEOTM handle instance
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
PUBLIC void mld_blk_init(lineotn_handle_t *lineotn_handle)
{
    mld_hw_module_t hw_module;
    UINT8 i;
    UINT8 lane;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* initialize mld block variables */
    mld_var_init(&lineotn_handle->var);

    /* power up mld block */
    mld_pwr_mode_config_set( lineotn_handle, MLD_BLOCK_HW,FALSE );

    /* take mld block out of reset */
    mld_reset_config_set( lineotn_handle, FALSE );

    /* set all hw modules to low power */
    for(hw_module = MLD_SFI_51_0; hw_module < MLD_BLOCK_HW ; hw_module = (mld_hw_module_t)((UINT32)hw_module + 1) )
    {
        mld_pwr_mode_config_set( lineotn_handle, hw_module,TRUE );
    }

    if(lineotn_handle->var.lineotn_mode)
    {
        /* set all bergen and precoders to bypass mode and disable*/
        for(i=0; i<12;i++)
        {
            otn_mld_field_PRECODER_32_TX_BYPASS_set(NULL,lineotn_handle,i,1);
            otn_mld_field_PRECODER_32_RX_BYPASS_set(NULL,lineotn_handle,i,1);
            otn_mld_field_BERGEN_32_BYPASS_set(NULL,lineotn_handle,i,1);
            otn_mld_field_BERGEN32_ENABLE_set(NULL,lineotn_handle,i,0);
            otn_mld_field_LANE_10G_TX_SRC_set(NULL,lineotn_handle,i,0);
        }

        /* set the output channels that will be used to report defects of */
        /* an internal OTN MLD channel to be the same as the internal mld */
        /* channels                                                       */
        for(i=0; i<12;i++)
        {
            otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,i,i);
        }

        otn_mld_field_PRECODER_128_TX_BYPASS_set(NULL,lineotn_handle,0,1);
        otn_mld_field_PRECODER_128_TX_BYPASS_set(NULL,lineotn_handle,1,1);
        otn_mld_field_PRECODER_128_TX_BYPASS_set(NULL,lineotn_handle,2,1);
        otn_mld_field_PRECODER_128_RX_BYPASS_set(NULL,lineotn_handle,0,1);
        otn_mld_field_PRECODER_128_RX_BYPASS_set(NULL,lineotn_handle,1,1);
        otn_mld_field_PRECODER_128_RX_BYPASS_set(NULL,lineotn_handle,2,1);

        /* disable all 36 bergen32 ring ocilators */
        for(i=0; i<36;i++)
        {
            otn_mld_field_RINGOSC_32_ENABLE_set(NULL,lineotn_handle,i,0);
        }
    }

    /* remove all opsm loopbacks */
    if(lineotn_handle->var.lineotn_mode)
    {
        opsm4_field_RX_SYS_LOOPBACK_set(NULL, lineotn_handle, 0);
        opsm4_field_TX_SYS_LOOPBACK_set(NULL, lineotn_handle, 0);

        /* OTL-AIS can be falsely triggered under high bit error rates, so disable it's contribution to SSF */
        opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
        opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);

    }
    else
    {
        sysotn_opsm4_field_RX_SYS_LOOPBACK_set(NULL, lineotn_handle, 0);
        sysotn_opsm4_field_TX_SYS_LOOPBACK_set(NULL, lineotn_handle, 0);

        /* OTL-AIS can be falsely triggered under high bit error rates, so disable it's contribution to SSF */
        sysotn_opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
        sysotn_opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
    }


    if(lineotn_handle->var.lineotn_mode)
    {
        for(i=0; i<3;i++)
        {
            opsm23_field_range_RX_SYS_LOOPBACK_set(NULL, lineotn_handle,i,0,3,0);
            opsm23_field_range_TX_SYS_LOOPBACK_set(NULL, lineotn_handle,i,0,3,0);
            /* OTL-AIS can be falsely triggered under high bit error rates, so disable it's contribution to SSF */
            for (lane=0; lane<4; lane++) {
                opsm23_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,i, lane, 0);
                opsm23_field_RX_FAIL_LANE_OTL_AIS_set(NULL,lineotn_handle,i, lane, 0);
            }
        }


    } /*end if lineotn_mode */

    PMC_RETURN();

} /* mld_blk_init */


/*******************************************************************************
* mld_pwr_mode_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the mld power mode into one of 2 predefined power modes per requested
*   hw module or the mld block in general.
*
* INPUTS:
*   lineotn_handle          - pointer to Lineotn handle instance.
*
*   hw_type                 - enum, requested hw module.
*
*   low_power               - boolean,TRUE: set to low power;
*                                     FALSE: take out of low power
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/

PUBLIC PMC_ERROR mld_pwr_mode_config_set( lineotn_handle_t  *lineotn_handle,
                                          mld_hw_module_t    hw_type,
                                          BOOL               low_power)

{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       low_power_val = (low_power?1:0);

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(hw_type < LAST_MLD_HW_MODULE,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

    switch ( hw_type )
    {
    case MLD_SFI_51_0:
        if(low_power_val != otn_mld_field_range_SFI51_LOWPWR_get(NULL,lineotn_handle,0,0))
            otn_mld_field_range_SFI51_LOWPWR_set(NULL,lineotn_handle,0,0,low_power_val);
        break;

    case MLD_SFI_51_1:
        if(low_power_val != otn_mld_field_range_SFI51_LOWPWR_get(NULL,lineotn_handle,1,1))
            otn_mld_field_range_SFI51_LOWPWR_set(NULL,lineotn_handle,1,1,low_power_val);
        break;

    case MLD_SFIS_N_10:
        /* 100G SFIS */

        if(lineotn_handle->var.lineotn_mode)
        {
            if(low_power_val != otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle,0,0))
                otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,0,0,low_power_val);
        }
        else
        {
            if(low_power_val != sysotn_field_SFIS_LOWPWR_get(NULL,lineotn_handle))
                sysotn_field_SFIS_LOWPWR_set(NULL,lineotn_handle,low_power_val);
        }

        break;
    case MLD_SFIS_N_4_0:
        if(low_power_val != otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle,2,2))
            otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,2,2,low_power_val);
        break;

    case MLD_SFIS_N_4_1:
        if(low_power_val != otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle,3,3))
            otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,3,3,low_power_val);
        break;

    case MLD_OPSM23_0:
        if(low_power_val != otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,0,0))
            otn_mld_field_range_OPSM23_LOWPWR_set(NULL,lineotn_handle,0,0,low_power_val);
        break;

    case MLD_OPSM23_1:
        if(low_power_val != otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,1,1))
            otn_mld_field_range_OPSM23_LOWPWR_set(NULL,lineotn_handle,1,1,low_power_val);
        break;

    case MLD_OPSM23_2:
        if(low_power_val != otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,2,2))
            otn_mld_field_range_OPSM23_LOWPWR_set(NULL,lineotn_handle,2,2,low_power_val);
        break;

    case MLD_OPSM4:

        if(lineotn_handle->var.lineotn_mode)
        {
            if(low_power_val != otn_mld_field_OPSM4_LOWPWR_get(NULL,lineotn_handle))
                otn_mld_field_OPSM4_LOWPWR_set(NULL,lineotn_handle,low_power_val);
        }
        else
        {
            if(low_power_val != sysotn_field_OPSM4_LOWPWR_get(NULL,lineotn_handle))
                sysotn_field_OPSM4_LOWPWR_set(NULL,lineotn_handle,low_power_val);
        }
        break;


    case MLD_BLOCK_HW:
        if(low_power_val != otn_mld_field_MLD_LOWPWR_get(NULL,lineotn_handle))
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                /* set the power gating and the power */
                otn_mld_field_MLD_LOWPWR_set(NULL,lineotn_handle,low_power_val);
            }
        }
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* mld_pwr_mode_config_set */


/*******************************************************************************
* mld_reset_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the mld into/out of reset.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   reset_mld               - boolean, requested reset mode:
*                                      TRUE: put mld block into reset mode;
*                                      FALSE: take mld block out of reset mode
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void mld_reset_config_set( lineotn_handle_t *lineotn_handle,
                                  BOOL             reset_mld )

{
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        if( reset_mld )
            otn_mld_field_MLD_TOP_RESET_set(NULL,lineotn_handle,1);
        else
            otn_mld_field_MLD_TOP_RESET_set(NULL,lineotn_handle,0);
    }

    PMC_RETURN();
} /* mld_reset_config_set */


/*******************************************************************************
* mld_blk_prov_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will call functions to allocate and set all HW modules for a dsi
*   stream per requested configuration.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s:
*                             LineOTN has 19 supported configuration modes.
*                             Some configurations can have either a HW
*                             resources routing via SFIS or OTL HW modules.
*                             In the case of SFIS routing user need to configure the type of
*                             dsi lane deskew required. For OTL3.4/OTU2 configurations
*                             routing no deskew is required
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8);
*                             for 40G SFIS configurations (0,5);
*                             for 40G SFI5.1 configurations (0,17);
*                             for 100G configurations (0)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PUBLIC PMC_ERROR mld_blk_prov_stream     ( lineotn_handle_t   *lineotn_handle,
                                           lineotn_cfg_type_t lineotn_cfg,
                                           lineotn_deskew_type_t deskew_type,
                                           mld_ifc_type_t     interface_type,
                                           UINT8              dsi_stream )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    switch ( lineotn_cfg )
    {
    /* 100GB, 1x100GB , 1xOTL4.10  */
    case LINE_OTN_CFG_0:
        rc = mld_otl_100G_prov(lineotn_handle);
        break;

    /* 100GB, 1x100GB , 1xSFI-S(n=10+1) */
    case LINE_OTN_CFG_1:
        rc = mld_sfis_n_10_100G_prov(lineotn_handle,deskew_type);
        break;


    /* all OTL configurations that are not 100G have the same config */
    case LINE_OTN_CFG_3:/* 120GB , 12x10GB , 12xOTU2  */
      rc = mld_otl_prov(lineotn_handle, lineotn_cfg, interface_type,
                        dsi_stream);
        break;

    case LINE_OTN_CFG_4:/* 120GB , 1x40GB + 8x10GB , 1xOTL3.4 + 8xOTU2 */
      rc = mld_otl_prov(lineotn_handle,lineotn_cfg, interface_type,
                        dsi_stream);
        break;

    /* 110GB , 1x40GB + 7x10GB , 1xSFI-S(n=4+1) + 7xOTU2 */
    case LINE_OTN_CFG_5:
        rc = mld_sfis_n_4_7x_otu2_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 110GB , 1x40GB + 1x40GB + 3x10GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  */
    case LINE_OTN_CFG_6:
        rc = mld_sfis_otl34_3x_otu2_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;


    case LINE_OTN_CFG_7:/* 120GB , 1x40GB + 8x10GB , 1xSFI5.1 + 8xOTU2 */
    case LINE_OTN_CFG_8:/* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xOTL3.4 + 1xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_12:/* 120GB , 2x40GB + 1x40GB , 2xOTL3.4 + 1xSFI5.1  */
        rc = mld_1x_sfi51_otl_prov(lineotn_handle,deskew_type,interface_type,lineotn_cfg,dsi_stream);
        break;


    case LINE_OTN_CFG_9: /* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_10:/* 120GB , 1x40GB + 1x40GB + 1x40GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1*/
        rc = mld_sfis_sfi51_otl_prov(lineotn_handle,deskew_type,interface_type,lineotn_cfg,dsi_stream);
        break;

    case LINE_OTN_CFG_11:/* 120GB , 2x40GB + 4x10GB , 2xOTL3.4 + 4xOTU2 */
      rc = mld_otl_prov(lineotn_handle, lineotn_cfg, interface_type,
                        dsi_stream);
        break;

        /* 100GB , 2x40GB + 2x10GB , 2xSFI-S(n=4+1) + 2xOTU2 */
    case LINE_OTN_CFG_13:
        rc = mld_2x_sfis_otu2_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI-S(n=4+1) + 1xSFI5.1  */
    case LINE_OTN_CFG_14:
        rc = mld_2x_sfis_sfi51_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 4x10GB , 2xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_15:
        rc = mld_2x_sfi51_4x_otu2_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xOTL3.4 */
    case LINE_OTN_CFG_16:
        rc = mld_2x_sfi51_otl34_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xSFI-S(n=4+1)   */
    case LINE_OTN_CFG_17:
        rc = mld_2x_sfi51_sfis_prov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    case LINE_OTN_CFG_18:/* 120GB , 3x40GB , 3xOTL3.4  */
      rc = mld_otl_prov(lineotn_handle, lineotn_cfg, interface_type,
                        dsi_stream);
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* mld_blk_prov_stream */


/*******************************************************************************
* mld_blk_deprov_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will call functions to de-allocate and reset all HW modules for a dsi
*   stream per requested configuration.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s:
*                             LineOTN has 19 supported configuration modes.
*                             Some configurations can have either a HW
*                             resources routing via SFIS or OTL HW modules.
*                             In the case of SFIS routing user need to configure the type of
*                             dsi lane deskew required. for OTL3.4/OTU2 configurations
*                             routing no deskew is required.
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8);
*                             for 40G SFIS configurations (0,5);
*                             for 40G SFI5.1 configurations (0,17);
*                             for 100G configurations (0)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/
PUBLIC PMC_ERROR mld_blk_deprov_stream( lineotn_handle_t   *lineotn_handle,
                                        lineotn_cfg_type_t lineotn_cfg,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        UINT8              dsi_stream )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    switch ( lineotn_cfg )
    {
    /* 100GB, 1x100GB , 1xOTL4.10  */
    case LINE_OTN_CFG_0:
        mld_otl_100G_deprov(lineotn_handle);
        break;

    /* 100GB, 1x100GB , 1xSFI-S(n=10+1) */
    case LINE_OTN_CFG_1:
        mld_sfis_n_10_100G_deprov(lineotn_handle,deskew_type);
        break;


    /* all OTL configurations that are not 100G have the same config */
    case LINE_OTN_CFG_3:/* 120GB , 12x10GB , 12xOTU2  */
    case LINE_OTN_CFG_4:/* 120GB , 1x40GB + 8x10GB , 1xOTL3.4 + 8xOTU2 */
    case LINE_OTN_CFG_11:/* 120GB , 2x40GB + 4x10GB , 2xOTL3.4 + 4xOTU2 */
    case LINE_OTN_CFG_18:/* 120GB , 3x40GB , 3xOTL3.4  */
        mld_otl_deprov(lineotn_handle, interface_type, dsi_stream);
        break;

    /* 110GB , 1x40GB + 7x10GB , 1xSFI-S(n=4+1) + 7xOTU2 */
    case LINE_OTN_CFG_5:
        rc = mld_sfis_n_4_7x_otu2_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 110GB , 1x40GB + 1x40GB + 3x10GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  */
    case LINE_OTN_CFG_6:
        rc = mld_sfis_otl34_3x_otu2_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    case LINE_OTN_CFG_7:/* 120GB , 1x40GB + 8x10GB , 1xSFI5.1 + 8xOTU2 */
    case LINE_OTN_CFG_8:/* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xOTL3.4 + 1xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_12:        /* 120GB , 2x40GB + 1x40GB , 2xOTL3.4 + 1xSFI5.1  */
        rc = mld_1x_sfi51_otl_deprov(lineotn_handle,deskew_type,interface_type,lineotn_cfg,dsi_stream);
        break;

    case LINE_OTN_CFG_9: /* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_10:/* 120GB , 1x40GB + 1x40GB + 1x40GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1*/
        rc = mld_sfis_sfi51_otl_deprov(lineotn_handle,deskew_type,interface_type,lineotn_cfg,dsi_stream);
        break;

    /* 100GB , 2x40GB + 2x10GB , 2xSFI-S(n=4+1) + 2xOTU2 */
    case LINE_OTN_CFG_13:
        rc = mld_2x_sfis_otu2_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI-S(n=4+1) + 1xSFI5.1  */
    case LINE_OTN_CFG_14:
        rc = mld_2x_sfis_sfi51_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 4x10GB , 2xSFI5.1 + 4xOTU2  */
    case LINE_OTN_CFG_15:
        rc = mld_2x_sfi51_4x_otu2_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xOTL3.4 */
    case LINE_OTN_CFG_16:
        rc = mld_2x_sfi51_otl34_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xSFI-S(n=4+1)   */
    case LINE_OTN_CFG_17:
        rc = mld_2x_sfi51_sfis_deprov(lineotn_handle,deskew_type,interface_type,dsi_stream);
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* mld_blk_deprov_stream */


/*******************************************************************************
*  mld_opsm23_scramble_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the scrambling/descrambling mode for opsm23.
*
* INPUTS:
*   lineotn_handle    - pointer to lineotn handle instance.
*
*   opsm_idx          - unsigned, opsm23 to configure (0,1,2)
*
*   dsi_stream        - unsigned,relevant only for opsm23 for 10G configurations
*                       (0..11)
*
*   scramble_in_opsm  - boolean,TRUE:scrambling/descrambling is done at the opsm;
*                               FALSE:scrambling/descrambling is not done at the opsm
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
PUBLIC void mld_opsm23_scramble_set     (lineotn_handle_t   *lineotn_handle,
                                         UINT8              opsm_idx,
                                         UINT8              dsi_stream,
                                         BOOL               scramble_in_opsm)
{
    UINT8 dsi_stream_idx = (dsi_stream%4);

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(opsm_idx < 3,LINEOTN_ERR_INVALID_PARAMETERS,opsm_idx,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if(scramble_in_opsm)
    {
        if( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 &&
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2)  /* 40G */
        {
            /* configure bit 3 to set scrambling for all 40G stream */
            opsm23_field_range_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,0,3,0);
            opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,0,3,0xf);/* data scramble in opsm */
            opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,0,3,0);/* data coming from the fibre is always scrambled*/
            opsm23_field_range_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,0,3,0xf);/* data is de-scrambled in opsm */
            opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm_idx,0,3,0);
        }
        else /* 10G */
        {
            /* configure bits per dsi stream to set scrambling for all 10G stream */
            opsm23_field_range_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);
            opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,1);/* data scramble in opsm */
            opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);/* data coming from the fibre is always scrambled */
            opsm23_field_range_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,1);/* data is scramble in opsm */
            opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);

        }
    }
    else
    {
        if( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 &&
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2)  /* 40G */
        {
            /* configure bit 3 to set scrambling for all 40G stream */
            opsm23_field_range_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,0,3,0xf);/* data is coming in scrambled */
            opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,0,3,0);/* data scramble in otu_frm */
            opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,0,3,0);/* data is coming in scrambled */
            opsm23_field_range_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,0,3,0);/* data is scramble in OTU_FRM */
            opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm_idx,0,3,0xf);

        }
        else /* 10G */
        {
            /* configure bits per dsi stream to set scrambling for all 10G stream */
            opsm23_field_range_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,1);/* data is coming in scrambled */
            opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);/* data scramble in otu_frm */
            opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);/* data is coming in scrambled */
            opsm23_field_range_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,0);/* data is scramble in OTU_FRM */
            opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm_idx,dsi_stream_idx,dsi_stream_idx,1);
        }

    }

    PMC_RETURN();

}/* mld_opsm23_scramble_set */


/*******************************************************************************
*  mld_opsm4_scramble_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the scrambling/descrambling mode for opsm4.
*
* INPUTS:
*   lineotn_handle    - pointer to lineotn handle instance.
*
*   scramble_in_opsm  - boolean,TRUE:scrambling/descrambling is done at the opsm;
*                               FALSE:scrambling/descrambling is not done at the opsm
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
PUBLIC void mld_opsm4_scramble_set(lineotn_handle_t   *lineotn_handle,
                                   BOOL               scramble_in_opsm)
{
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(scramble_in_opsm)
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in not scrambled */
            opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data scramble in opsm */
            opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data coming from the fibre is always scrambled */
            opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data is scramble in opsm */
            opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,0);
        }
        else
        {
            sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in not scrambled */
            sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data scramble in opsm */
            sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data coming from the fibre is always scrambled */
            sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data is scramble in opsm */
            sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,0); 
        }
    }
    else
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,1);/* data is coming in scrambled */
            opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data scramble in otu_frm */
            opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in scrambled */
            opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data is scramble in OTU_FRM */
            opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,1); /* should be set when descramber is either bypassed or not enabled, 
                                                                             in order to provide "unscrambled" data to OTU MFAS generator.*/
        }
        else
        {
            sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,1);/* data is coming in scrambled */
            sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data scramble in otu_frm */
            sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in scrambled */
            sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data is scramble in OTU_FRM */
            sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,1); /* should be set when descramber is either bypassed or not enabled, 
                                                                                    in order to provide "unscrambled" data to OTU MFAS generator.*/
        }
    }

    PMC_RETURN();
}/* mld_opsm4_scramble_set */


/*******************************************************************************
* mld_sfis_n_rx_alarm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables SFIS RX alarms.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   opsm_idx                - OPSM slice index.
*   enable                  - when TRUE alarms are enabled, otherwise
*                             they are disabled.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void mld_sfis_n_rx_alarm_cfg(lineotn_handle_t *lineotn_handle,
                                     UINT32            opsm_idx,
                                     BOOL8             enable)
{
    UINT32 reg_val;
    sfis_rx_buffer_t buf[1];
    sysotn_sfis_rx_buffer_t sysotn_buf[1];
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    reg_val = TRUE == enable ? 1 : 0;

    /* set RX alarms: OOA, OOF, LOS */
    if (TRUE == lineotn_handle->var.lineotn_mode)
    {
        sfis_rx_buffer_init(buf, lineotn_handle, opsm_idx);

        sfis_rx_field_RXOOA_ALARM_EN_set(buf, lineotn_handle, opsm_idx, reg_val);
        sfis_rx_field_RXOOF_ALARM_EN_set(buf, lineotn_handle, opsm_idx, reg_val);
        sfis_rx_field_RXLOS_ALARM_EN_set(buf, lineotn_handle, opsm_idx, reg_val);
        sfis_rx_field_RX_FORCE_LOS_set(buf, lineotn_handle, opsm_idx, 0);

        sfis_rx_buffer_flush(buf);
    }
    else
    {
        sysotn_sfis_rx_buffer_init(sysotn_buf, lineotn_handle);

        /* set RX alarms: OOA, OOF, LOS */
        sysotn_sfis_rx_field_RXOOA_ALARM_EN_set(sysotn_buf, lineotn_handle, reg_val);
        sysotn_sfis_rx_field_RXOOF_ALARM_EN_set(sysotn_buf, lineotn_handle, reg_val);
        sysotn_sfis_rx_field_RXLOS_ALARM_EN_set(sysotn_buf, lineotn_handle, reg_val);
        sysotn_sfis_rx_field_RX_FORCE_LOS_set(sysotn_buf, lineotn_handle, 0);

        sysotn_sfis_rx_buffer_flush(sysotn_buf);
    }

    PMC_RETURN();

}/* mld_sfis_n_rx_alarm_cfg */


/*******************************************************************************
* mld_rcp_defect_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables a rcp defects broadcast.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned,for 10G configurations (0..11);
*                                      for 40G configurations (0,4,8);
*                                      for 100G configurations (0)
*   frame_rate              - enum, OTU frame rate provisioned for this DSI stream
*                             (LINE_OTN_RATE_OTU1E ... LINE_OTN_RATE_OTU4). 
*
*   rcp_defect_enable       - boolean, TRUE: enable the rcp defect broadcast;
*                                      FALSE: the rcp defect broadcast disabled
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void mld_rcp_defect_cfg(lineotn_handle_t    *lineotn_handle,
                                     UINT8                dsi_stream,
                                     lineotn_frame_rate_t frame_rate,
                                     BOOL                 rcp_defect_enable)
{
    UINT8       i = 0;
    UINT8       dci_chnl;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(frame_rate<LAST_LINE_OTN_FRAME_RATE,LINEOTN_ERR_INVALID_PARAMETERS,frame_rate,0);

    if( rcp_defect_enable )
    {
        if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G config */
        {
            /* error prevention - normalize dsi stream to 0,4,8  */
            dsi_stream = ((dsi_stream/4)*4);

            /* dsi streams 0-3 will broadcast to rcp channel 3   */
            /* dsi streams 4-7 will broadcast to rcp channel 7   */
            /* dsi streams 8-11 will broadcast to rcp channel 11 */
            for(i = dsi_stream;i<dsi_stream+4;i++)
            {
                dci_chnl = lineotn_handle->var.lineotn_dsi_dist[i].dci_channel;
                otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,dci_chnl,dsi_stream+3);
            }
        }
        else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G config */
        {
            dci_chnl = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dci_channel;
            otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,dci_chnl,dsi_stream); /* set rcp channels to 1:1 from dsi stream (source) to dci_chnl (output) */
        }
        else /* 100G */
        {
            for(i=0;i<11;i++) /* set all rcp channels to broadcast to channel 11 */
                otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,i,11);
        }

    }
    else /* disable rcp channel broadcast */
    {
        if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G config  */
        {
            /* error prevention - normalize dsi stream to 0,4,8  */
            dsi_stream = ((dsi_stream/4)*4);

            /* set as rda default value per 40G 4 lanes */
            for(i = dsi_stream;i<dsi_stream+4;i++)
            {
                dci_chnl = lineotn_handle->var.lineotn_dsi_dist[i].dci_channel;
                otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,dci_chnl,0);
            }
        }
        else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G config */
        {
            dci_chnl = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dci_channel;
            otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,dci_chnl,0); /* set as rda default value */
        }
        else /* 100G */
        {
            for(i=0;i<11;i++) /* set as rda default value */
                otn_mld_field_DEFECT_RCP_CHNL_set(NULL,lineotn_handle,i,0);
        }
    }

    PMC_RETURN();
}/* mld_rcp_defect_cfg */


/*******************************************************************************
* mld_otl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following OTL configurations:\n
*   - LINE_OTN_CFG_3 - 120GB , 12x10GB , 12xOTU2                   (0..11)
*   - LINE_OTN_CFG_4 - 120GB , 1x40GB + 8x10GB , 1xOTL3.4 + 8xOTU2 (0-3,4,5,6,7,8,9,10,11)
*   - LINE_OTN_CFG_11- 120GB , 2x40GB + 4x10GB ,2xOTL3.4 + 4xOTU2  (0-3,4-7,8,9,10,11)
*   - LINE_OTN_CFG_18- 120GB , 3x40GB , 3xOTL3.4                   (0-3,4-7,8-11)
*
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 4 possible OTL configurations for
*                             LineOTN SS
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*          
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8);
*                             for 40G SFIS configurations (0,5);
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/
PRIVATE PMC_ERROR mld_otl_prov(lineotn_handle_t   *lineotn_handle,
                               lineotn_cfg_type_t lineotn_cfg,
                               mld_ifc_type_t     interface_type,
                               UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    BOOL        opsm_mode_otl_34=FALSE;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(lineotn_cfg == LINE_OTN_CFG_3 ||
               lineotn_cfg == LINE_OTN_CFG_4 ||
               lineotn_cfg == LINE_OTN_CFG_11 ||
               lineotn_cfg == LINE_OTN_CFG_18,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE, 
               LINEOTN_ERR_INVALID_PARAMETERS, interface_type, 0);

    if (MLD_OTL == interface_type) 
    { 
        opsm_mode_otl_34 = TRUE; 
        
        if(dsi_stream < 4) /* dsi streams 0-3 */
        {
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);
        } 
        else if (dsi_stream >=4 && dsi_stream < 8) /* dsi streams 4-7 */
        {
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
        }
        else /* dsi streams 8-11 */
        {
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);
        }
    }
    else 
    { 
        opsm_mode_otl_34 = FALSE; 
        lineotn_handle->var.mld_curr_num_otu_configured |= (1<<dsi_stream); 
		mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);
    } 

    if( PMC_SUCCESS == rc )
    {
        mld_opsm23_otl_prov(lineotn_handle,opsm_mode_otl_34,dsi_stream);
    }

    PMC_RETURN(rc);
}/* mld_otl_prov */

/*******************************************************************************
* mld_otl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following OTL configurations:
*   - LINE_OTN_CFG_3 - 120GB , 12x10GB , 12xOTU2                   (0..11)
*   - LINE_OTN_CFG_4 - 120GB , 1x40GB + 8x10GB , 1xOTL3.4 + 8xOTU2 (0-3,4,5,6,7,8,9,10,11)
*   - LINE_OTN_CFG_11- 120GB , 2x40GB + 4x10GB ,2xOTL3.4 + 4xOTU2  (0-3,4-7,8,9,10,11)
*   - LINE_OTN_CFG_18- 120GB , 3x40GB , 3xOTL3.4                   (0-3,4-7,8-11)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   interface_type          - enum mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8);
*                             for 40G SFIS configurations (0,5);
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/
PRIVATE void mld_otl_deprov(lineotn_handle_t   *lineotn_handle,
                            mld_ifc_type_t     interface_type,
                            UINT8              dsi_stream)
{
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if(dsi_stream < 4) /* dsi stream 0-3 */
    {
        /* otu2 configured */
        if(MLD_OTU2 == interface_type)
        {
            /* update number of otu2 configured */
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else  /* this is a 40G configuration - deprov all 0-3 lanes from OPSM#0*/
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }

    }
    else if (dsi_stream >= 4 && dsi_stream < 8) /* dsi stream 4-7 */
    {
        /* otu2 configured */
        if(MLD_OTU2 == interface_type)
        {
            /* update number of otu2 configured */
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else  /* this is a 40G configuration - deprov all 4-7 lanes from OPSM#1*/
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }

    }
    else /* dsi streams 8-11 */
    {
        /* otu2 configured */
        if(MLD_OTU2 == interface_type)
        {
            /* update number of otu2 configured */
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else /* this is a 40G configuration - deprov all 8-11 lanes from OPSM#2 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }

    }

    PMC_RETURN();
}


/*******************************************************************************
* mld_otl_100G_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*   - LINE_OTN_CFG_0 - 100GB   1x100GB  1xOTL4.10  (0-9)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/
PRIVATE PMC_ERROR mld_otl_100G_prov(lineotn_handle_t *lineotn_handle)
{
    UINT8 i;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        /* set distribution mux on all dsi streams 0-9 to OPSM4 */
        for(i=0;i<10;i++)
        {
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,i,0,0,1);
        }

        /* set mux 0 line stream_10x32_0 is connected */
        otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,1);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU4,TRUE);
    }

    /* provision opsm4 for OTL */
    rc = mld_opsm4_prov(lineotn_handle,LINE_OTN_NO_DESKEW);

    PMC_RETURN(rc);

}/* mld_otl_100G_prov */


/*******************************************************************************
* lineotn_mld_deskew_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover MLD deskew type from LINEOTN register-space for a specific slice.
*
* INPUTS:
*   *lineotn_handle          - pointer to LINEOTN handle instance to be operated on
*    is_100g                 - when TRUE slice concerns a 100G stream
*    opsm_23_idx             - OTU slice number
*
* OUTPUTS:
*   *deskew_type             - stream deskew type
*
* RETURNS:
*   TRUE - when returned deskew_type is valid, otherwise FALSE
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 lineotn_mld_deskew_type_get(lineotn_handle_t        *lineotn_handle,
                                          BOOL8                    is_100g,
                                          UINT32                   opsm_23_idx,
                                          lineotn_deskew_type_t   *deskew_type)
{
    BOOL8 rc = TRUE;

    PMC_ENTRY();

    *deskew_type = LAST_LINE_OTN_DESKEW_TYPE;

    /* interpolate deskew type according to MLD mux configuration */
    if (TRUE == is_100g)
    {
        /* find SFIS related deskew type */
        if (TRUE == (lineotn_handle->var.lineotn_mode))
        {
            if (0x1 == otn_mld_field_MUX2_SYS_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_OIF_SFIS_DESKEW;
            }
            else if (0x2 == otn_mld_field_MUX2_SYS_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX1_LINE_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX0_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_ENHANCED_SFIS_DESKEW;
            }
        }
        else
        {
            if ( 1 == sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_get(NULL,lineotn_handle))
            {
                if (0 ==  opsm4_field_TX_LINE_DEQUEUE_EN_get(NULL,lineotn_handle))
                {                
                    *deskew_type = LINE_OTN_OIF_SFIS_DESKEW;
                }             
            }
        }
    }
    else
    {
        switch (opsm_23_idx)
        {
        case 0:
            /* find SFIS related deskew type associated to SLICE 0 */
            if (0x2 == otn_mld_field_MUX7_SYS_STREAM_get(NULL,lineotn_handle) &&
                0x2 == otn_mld_field_MUX6_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_OIF_SFIS_DESKEW;
            }
            else if (0x1 == otn_mld_field_MUX7_SYS_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX5_LINE_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX4_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_ENHANCED_SFIS_DESKEW;
            }
            break;
        case 1:
            /* find SFIS related deskew type associated to SLICE 1 */
            if (0x2 == otn_mld_field_MUX11_SYS_STREAM_get(NULL,lineotn_handle) &&
                0x2 == otn_mld_field_MUX10_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_OIF_SFIS_DESKEW;
            }
            else if (0x1 == otn_mld_field_MUX11_SYS_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX9_LINE_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX8_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_ENHANCED_SFIS_DESKEW;
            }
            break;   
        case 2:
            /* find SFIS related deskew type associated to SLICE 1 */
            if (0x4 == otn_mld_field_MUX13_SYS_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_OIF_SFIS_DESKEW;
            }
            else if (0x2 == otn_mld_field_MUX13_SYS_STREAM_get(NULL,lineotn_handle) &&
                     0x2 == otn_mld_field_MUX12_LINE_STREAM_get(NULL,lineotn_handle))
            {
                *deskew_type = LINE_OTN_ENHANCED_SFIS_DESKEW;
            }
            break;
        default:
            rc = FALSE;
            break;
        }
    }

    PMC_RETURN(rc);
}/*lineotn_mld_deskew_type_get*/


/*******************************************************************************
* lineotn_mld_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover MLD related configuration parameters from LINEOTN register-space.
*
* INPUTS:
*   *lineotn_handle           - pointer to LINEOTN handle instance to be operated on
*   top_energy_state_reg      - energy state from top-level
*
*
* OUTPUTS:
*   *lineotn_var_reg          - A partial lineotn_var_t context + additional
*                              information to facilite audit phase
*   *lineotn_energy_state_reg - LINEOTN energy states
*
* RETURNS:
*   TRUE - when returned deskew_type is valid, otherwise FALSE
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_mld_ctxt_reg_recover(lineotn_handle_t        *lineotn_handle,
                                         pmc_energy_state_t       top_energy_state_reg,
                                         lineotn_recover_var_t   *lineotn_var_reg,
                                         lineotn_energy_state_t  *lineotn_energy_state_reg)
{
    UINT32    tx_src_val;
    UINT32 itr;
    UINT32 num_100g_lanes = 0;
    UINT32 num_sfis_100g_lanes = 0;
    UINT32 num_23_0_lanes = 0;
    UINT32 num_23_1_lanes = 0;
    UINT32 num_23_2_lanes = 0;
    UINT32 num_sfis_40g_0_lanes = 0;
    UINT32 num_sfis_40g_1_lanes = 0;
    UINT32 dsi_start;
    UINT32 dsi_end;
    UINT32 lowpwr;
    lineotn_deskew_type_t   deskew_type;

    PMC_ENTRY();

    /* check any discrepancies*/

    /* depending on enabled sub blocks, determine which datapaths are operational :*/
    /* fill up
       lineotn_dsi_cfg_t       lineotn_dsi_dist[LINE_OTN_MAX_DSI_STREAMS];

       lineotn_frame_rate_t    dsi_rate;
       BOOL                    dsi_provisioned;
       BOOL                    dsi_activated_rx;
       BOOL                    dsi_activated_tx;
       BOOL                    scramble_in_opsm; --- MLD var
       mld_ifc_type_t          dsi_hw_ifc;
       lineotn_deskew_type_t   deskew_type;
       UINT8                   dci_channel;
       BOOL8                   bergen_enabled    --- MLD var
       lineotn_frame_rate_t    lineotn_sfi51_0_frame_rate;
       lineotn_frame_rate_t    lineotn_sfi51_1_frame_rate;
    */

    /* recover DSI lanes / MUX-DEMUX status lineotn_dsi_dist */

    /* determine how many 10G, 40G and 100G DSI lanes are configured */

    if(lineotn_handle->var.lineotn_mode)
    {
        for (itr = 0; itr < LINE_OTN_MAX_DSI_STREAMS; itr++)
        {
            tx_src_val = otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, itr);
            PMC_LOG_TRACE("LANE_10G_TX_SRC itr = %d tx_src_val = %d\n", itr, tx_src_val);
            
            if (0 == (tx_src_val & 0xFC) && 0 != (tx_src_val & 0x2))
            {
                num_sfis_100g_lanes += 1;
            }
            
            if (0 == (tx_src_val & 0xFC) && 0 != (tx_src_val & 0x1))
            {
                num_100g_lanes += 1;
            }
            
            if (0 != (tx_src_val & 0x8))
            {
                num_23_0_lanes += 1;
            }
            if (0 != (tx_src_val & 0x20))
            {
                num_23_1_lanes += 1;
            }
            if (0 != (tx_src_val & 0x80))
            {
                num_23_2_lanes += 1;
            }
            if (0 != (tx_src_val & 0x10))
            {
                num_sfis_40g_0_lanes += 1;
            }
            if (0 != (tx_src_val & 0x40))
            {
                num_sfis_40g_1_lanes += 1;
            }
        }
    }

    /*  OPSM4  - slice 0 */
    PMC_LOG_TRACE("num 100g lanes = %d %d\n", num_100g_lanes, opsm4_field_RX_OTL_4_10_ENABLE_get(NULL,lineotn_handle));
    if (((10 == num_100g_lanes) &&
         TRUE == lineotn_handle->var.lineotn_mode &&
         0x1 == opsm4_field_RX_OTL_4_10_ENABLE_get(NULL,lineotn_handle))
        || (FALSE == lineotn_handle->var.lineotn_mode &&
            0x1 == sysotn_opsm4_field_RX_OTL_4_10_ENABLE_get(NULL,lineotn_handle)))
    {

        lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_hw_ifc  = MLD_OTL;

        if (TRUE == lineotn_handle->var.lineotn_mode)
        {
            /*define if lane is scrambled */
            lineotn_var_reg->var.lineotn_dsi_dist[0].scramble_in_opsm = (opsm4_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1 ||
                                                                         opsm4_field_RX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1)? TRUE : FALSE;

            if (0 == otn_mld_field_OPSM4_LOWPWR_get(NULL,lineotn_handle))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_provisioned = TRUE;
            }
        }
        else
        {
            /*define if lane is scrambled */
            lineotn_var_reg->var.lineotn_dsi_dist[0].scramble_in_opsm = (sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1 ||
                                                                         sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1)? TRUE : FALSE;

            if (0 == sysotn_field_OPSM4_LOWPWR_get(NULL,lineotn_handle))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_provisioned = TRUE;
            }
        }
        lineotn_var_reg->var.lineotn_dsi_dist[0].deskew_type = LINE_OTN_NO_DESKEW;

    }

    /*  SFIS (n = 10 + 1)  for SYSOTN */
    if (FALSE == lineotn_handle->var.lineotn_mode)
    {
        if (0x0 == sysotn_sfis_rx_field_RX_SW_RST_get(NULL,lineotn_handle) &&
            0x0 == sysotn_sfis_tx_field_TX_SW_RST_get(NULL,lineotn_handle) &&
            10 == sysotn_sfis_rx_field_RX_NUM_LANES_get(NULL,lineotn_handle) &&
            10 == sysotn_sfis_rx_field_RX_NUM_LANES_get(NULL,lineotn_handle))
        {
            lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_hw_ifc  = MLD_SFIS;
            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, TRUE, 0, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].deskew_type = deskew_type;
            }

            if (0 == sysotn_field_OPSM4_LOWPWR_get(NULL,lineotn_handle))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_provisioned = TRUE;
            }

            /*define if lane is scrambled */
            lineotn_var_reg->var.lineotn_dsi_dist[0].scramble_in_opsm = (sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1 ||
                                                                         sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1)? TRUE : FALSE;
            
        }
    }
    
    /* consult LINOTN specific registers */
    if (TRUE == lineotn_handle->var.lineotn_mode)
    {
        /*  SFIS (n = 10 + 1)  - slice 0 */
        if (11 == num_sfis_100g_lanes)
        {
            lineotn_var_reg->var.lineotn_dsi_dist[0].scramble_in_opsm = opsm4_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle) == 1 ? TRUE : FALSE;
            
            if (0 == otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle, 0, 0))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_provisioned = TRUE;
            }
            
            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, TRUE, 0, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].deskew_type = deskew_type;
            }
            
            lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_hw_ifc  = MLD_SFIS;
        }

        /* SFI51 slice 0 */
        if (1 == otn_mld_field_MUX13_SYS_STREAM_get(NULL,lineotn_handle) &&
            0x2 == otn_mld_field_MUX7_LINE_STREAM_get(NULL,lineotn_handle))
        {

            if (num_sfis_40g_0_lanes == 5 || num_sfis_40g_1_lanes == 5)
            {
                dsi_start = 8;
                dsi_end = 12;
            }
            else
            {
                dsi_start = 0;
                dsi_end = 4;
            }

            for (itr = dsi_start; itr < dsi_end; itr++)
            {
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_SFI_51;
                lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = (0 != ((1 << (itr % 4)) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4))) ? TRUE : FALSE;
            }

            if(0x0 == otn_mld_field_range_SFI51_LOWPWR_get(NULL,lineotn_handle,0,0))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[dsi_start].dsi_provisioned = TRUE;
            }

            /* retrieve deskew type */
            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, FALSE, 0, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[dsi_start].deskew_type = deskew_type;
            }
        }

        /* SFI51 slice 1 */
        if (0x2 == otn_mld_field_MUX11_LINE_STREAM_get(NULL,lineotn_handle))
        {
            dsi_start = 4;
            dsi_end = 8;
            for (itr = dsi_start; itr < dsi_end; itr++)
            {
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_SFI_51;
                lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = (0 != ((1 << (itr % 4)) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4))) ? TRUE : FALSE;
            }

            if(0x0 == otn_mld_field_range_SFI51_LOWPWR_get(NULL,lineotn_handle, 1,1))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[dsi_start].dsi_provisioned = TRUE;
            }

            /* is it a sfis? */
            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, FALSE, 1, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[dsi_start].deskew_type = deskew_type;
            }
        }

        /*  OPSM23 slice 0*/
        PMC_LOG_TRACE("num_23_0_lanes = %d\n", num_23_0_lanes);
        if (0 != num_23_0_lanes)
        {
            lowpwr = otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,0,0);
            PMC_LOG_TRACE("about to process : %d\n", num_23_0_lanes);
            for (itr = 0; itr < 4; itr++)
            {
                if (1 == otn_mld_field_MUX4_LINE_STREAM_get(NULL,lineotn_handle) &&
                    0 != (otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, itr) & 0x08))
                {
                    /* update all 4 lanes*/
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = (0 != ((1 << itr) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4))) ? TRUE : FALSE;
                    PMC_LOG_TRACE("OTL itr = %d %d %d %d %d %d %x %x %d\n", itr,
                                  opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4),
                                  opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4),
                                  opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4),
                                  opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4),
                                  otn_mld_field_MUX4_LINE_STREAM_get(NULL,lineotn_handle),
                                  opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4),
                                  (1 << itr) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4),
                                  lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm);

                    if (4 == num_23_0_lanes &&
                        1 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        1 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTL;
                    }
                    else if (0 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             0 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTU2;
                    }

                    if (0 == lowpwr &&  LAST_MLD_IFC_TYPE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc)
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned = TRUE;
                    }
                }
            }
        }

        /*  OPSM23 slice 1*/
        if (0 != num_23_1_lanes)
        {
            lowpwr = otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,1,1);
            for (itr = 4; itr < 8; itr++)
            {
                /* Note: If you have a SFI-S in the previous slice, it occupies lanes 0 through 4,
                   thus, you need to make sure LANE_10G_TX_SRC indicates OPSM instead of SFIS */  
                if (0 != otn_mld_field_MUX8_LINE_STREAM_get(NULL,lineotn_handle) &&
                    0 != (otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, itr) & 0x20))
                {
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = (0 != (((1 << (itr % 4)) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4)))) ? TRUE : FALSE;

                    if (4 == num_23_1_lanes &&
                        1 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        1 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTL;
                    }
                    else if (0 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             0 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTU2;
                    }
                }
                /* check low power */
                if (0 == lowpwr &&  LAST_MLD_IFC_TYPE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc)
                {
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned = TRUE;
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type = LINE_OTN_NO_DESKEW;
                }
            }
        }

        /*  OPSM23 slice 2 */
        PMC_LOG_TRACE("num_23_2_lanes = %d\n", num_23_2_lanes);
        if (0 != num_23_2_lanes)
        {
            lowpwr = otn_mld_field_range_OPSM23_LOWPWR_get(NULL,lineotn_handle,2,2);
            PMC_LOG_TRACE("about to process : %d\n", num_23_0_lanes);
            for (itr = 8; itr < 12; itr++)
            {
                /* Note: If you have a SFI-S in the previous slice, it occupies lanes 5 through 9,
                   thus, you need to make sure LANE_10G_TX_SRC indicates OPSM instead of SFIS */  
                if (0 != otn_mld_field_MUX12_LINE_STREAM_get(NULL,lineotn_handle) &&
                    0 != (otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, itr) & 0x80))
                {
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = (0 != ((1 << itr % 4) & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, itr / 4))) ? TRUE : FALSE;

                    if (4 == num_23_2_lanes &&
                        1 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        1 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                        0 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTL;
                    }

                    else if (0 == opsm23_field_TX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             0 == opsm23_field_RX_OTL_3_4_ENABLE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_TX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4) &&
                             1 == opsm23_field_RX_DATA_4X10G_MODE_get(NULL,lineotn_handle, itr / 4))
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_OTU2;
                    }
                    if (0 == lowpwr &&  LAST_MLD_IFC_TYPE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc)
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned = TRUE;
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type = LINE_OTN_NO_DESKEW;
                    }
                }
            }
        }


        /*  SFIS (n = 4 +1) - slice 0 */
        if (5 == num_sfis_40g_0_lanes)
        {
            lineotn_var_reg->var.lineotn_dsi_dist[3].scramble_in_opsm = (0 != (0x8 & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle,0))) ? TRUE : FALSE;

            if (0 == otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle, 2, 2))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_provisioned = TRUE;
            }

            for (itr = 0; itr < 4; itr++)
            {
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_SFIS;
            }


            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, FALSE, 0, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[0].deskew_type = deskew_type;
            }
        }

        /*  SFIS (n = 4 +1) - slice 1 */
        if (5 == num_sfis_40g_1_lanes)
        {
            lineotn_var_reg->var.lineotn_dsi_dist[7].scramble_in_opsm = (0 != (0x8 & opsm23_field_TX_SCRAMBLE_ENABLE_get(NULL, lineotn_handle, 1))) ? TRUE : FALSE;
            if (0 == otn_mld_field_range_SFIS_LOWPWR_get(NULL,lineotn_handle, 3, 3))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[4].dsi_provisioned = TRUE;
            }
            for (itr = 4; itr < 8; itr++)
            {
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc  = MLD_SFIS;
            }


            if (TRUE == lineotn_mld_deskew_type_get(lineotn_handle, FALSE, 1, &deskew_type))
            {
                lineotn_var_reg->var.lineotn_dsi_dist[4].deskew_type = deskew_type;
            }
        }

        /* retrieve bergen state */
        for (itr = 0; itr < lineotn_handle->cfg.max_dsi_streams; itr++)
        {
            lineotn_var_reg->var.lineotn_dsi_dist[itr].bergen_enabled = (0 != otn_mld_field_BERGEN32_ENABLE_get(NULL,lineotn_handle, itr)) ? TRUE : FALSE;
        }
    }


    PMC_RETURN();
} /* lineotn_mld_ctxt_reg_recover */


/*******************************************************************************
* mld_otl_100G_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*   LINE_OTN_CFG_0 - 100GB   1x100GB  1xOTL4.10  (0-9)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/

PRIVATE void mld_otl_100G_deprov(lineotn_handle_t *lineotn_handle)
{
    UINT8 i;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        /* reset distribution mux on all dsi streams 0-9 */
        for(i=0;i<10;i++)
        {
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,i,0,0,0);
        }

        /* reset mux 0 line stream_10x32_0 is disconnected */
        otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,0);

        /* reset rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU4,FALSE);
    }

    /* disable OPSM4 */
    mld_opsm4_deprov(lineotn_handle);

    PMC_RETURN();

}/* mld_otl_100G_deprov */


/*******************************************************************************
* mld_otl_100G_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleanup ressouces associated to 100G datatpath
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
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
PRIVATE void mld_otl_100G_cleanup(lineotn_handle_t *lineotn_handle)
{
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,0,0,0,0);
        /* reset mux 0 line stream_10x32_0 is disconnected */
        otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,0);

        /* reset rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU4,FALSE);
    }

    /* disable OPSM4 */
    mld_opsm4_deprov(lineotn_handle);

    PMC_RETURN();

}/* mld_otl_100G_cleanup */


/*******************************************************************************
* mld_sfis_n_10_100G_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*   - LINE_OTN_CFG_1 - 100GB   1x100GB 1xSFI-S(n=10+1) \n
*    (lanes used: 0..10)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_sfis_n_10_100G_prov(lineotn_handle_t *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type)
{
    UINT8 i;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    /* deskew_type parameter is checked below for illegal values*/

    /* set all common resources */
    /* take required modules out of low power */
    mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_10,FALSE );

    /* set distribution mux on all dsi streams 0-10 to SFIS#0 */
    if(lineotn_handle->var.lineotn_mode)
    {
        for(i=0;i<11;i++)
        {
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,i,1,1,1);
        }
    }

    /* Configure SFIS#0 n=10 */
    /* put to reset to configure */
    if(lineotn_handle->var.lineotn_mode)
    {
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,0,1);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,0,1);

        /* set # of lines to 10 */
        sfis_rx_field_RX_NUM_LANES_set(NULL,lineotn_handle,0,10);
        sfis_tx_field_TX_NUM_LANES_set(NULL,lineotn_handle,0,10);

        /* reset odd parity */
        lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 0);

        /* take out of reset once configured */
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,0,0);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,0,0);
    }
    else
    {
        sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,1);
        sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,1);

        /* set # of lines to 10 */
        sysotn_sfis_rx_field_RX_NUM_LANES_set(NULL,lineotn_handle,10);
        sysotn_sfis_tx_field_TX_NUM_LANES_set(NULL,lineotn_handle,10);

        /* reset odd parity */
        lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 0);

        /* take out of reset once configured */
        sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,0);
        sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,0);
    }


    /* set rcp defects broadcasts */
    if(lineotn_handle->var.lineotn_mode)
    {
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU4,TRUE);
    }


    if (LINE_OTN_OIF_SFIS_DESKEW == deskew_type)
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            /* set mux 2 sys stream_1x320_1 is connected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,1);
        }

        /* OPSM4 SFI mode */
        rc= mld_opsm4_prov(lineotn_handle, LINE_OTN_OIF_SFIS_DESKEW);
    }
    else if(LINE_OTN_ENHANCED_SFIS_DESKEW == deskew_type)
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            /* set mux 2 sys stream_1x320_2 is connected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 1 line stream_10x32_4 is connected. */
            otn_mld_field_MUX1_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 0 line stream_10x32_2 is connected. */
            otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,2);
        }

        /* OPSM4 OTL 4x10 mode */
        rc= mld_opsm4_prov(lineotn_handle, LINE_OTN_NO_DESKEW);

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    /* Configure RX alarm propagation */
    if (PMC_SUCCESS == rc)
    {
        mld_sfis_n_rx_alarm_cfg(lineotn_handle, 0, TRUE);
    }

    PMC_RETURN(rc);

}/* mld_sfis_n_10_100G_prov */


/*******************************************************************************
* mld_sfis_n_10_100G_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*   - LINE_OTN_CFG_1 - 100GB   1x100GB 1xSFI-S(n=10+1)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE void mld_sfis_n_10_100G_deprov(lineotn_handle_t *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type)
{
    UINT8 i;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* reset distribution mux on all dsi streams 0-10 */
    if(lineotn_handle->var.lineotn_mode)
    {
        for(i=0;i<11;i++)
        {
            otn_mld_field_LANE_10G_TX_SRC_set(NULL,lineotn_handle, i, 0); 
        }
    }

    /* Configure SFIS#0 n=10 */
    /* put to reset */
    if(lineotn_handle->var.lineotn_mode)
    {
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,0,1);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,0,1);
    }
    else
    {
        sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,1);
        sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,1);
    }

    /* set rcp defects broadcasts */
    if(lineotn_handle->var.lineotn_mode)
    {
        /* these registers are only used in LINEOTN. No action to
        perform for SYSOTN*/

        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU4,FALSE);

        if (LINE_OTN_OIF_SFIS_DESKEW == deskew_type)
        {
            /* reset mux 2 sys stream_1x320_1 disconnected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,0);
        }
        else if(LINE_OTN_ENHANCED_SFIS_DESKEW == deskew_type)
        {
            /* reset mux 2 sys stream_1x320_2 disconnected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 1 line stream_10x32_4 is diconnected. */
            otn_mld_field_MUX1_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 0 line stream_10x32_2 disconnected. */
            otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,0);

        }
        else
        {
            /* reset mux 2 sys stream_1x320_2 disconnected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,0);
        }
    
    } /* (lineotn_handle->var.lineotn_mode) */

    /* Disable RX alarm propagation */    
    mld_sfis_n_rx_alarm_cfg(lineotn_handle, 0, FALSE);
    
    /* reset all common resources */   
    /* disable OPSM4 */
    mld_opsm4_deprov(lineotn_handle);

    /* put required modules into low power */
    (void) mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_10,TRUE );
    
    PMC_RETURN();

}/* mld_sfis_n_10_100G_deprov */


/*******************************************************************************
* mld_sfis_n_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function SFIS related registers of a given stream
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - unsigned, dsi stream to clean
*                             for 40G configurations (0,4,8), 
*                             for 100G (0)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void mld_sfis_n_reg_cleanup(lineotn_handle_t *lineotn_handle,
                                    UINT32            dsi_stream)
{
    UINT32 opsm_idx = dsi_stream / 4;
    UINT32 jtr;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* reset distribution mux on all dsi streams 0-10 */
    if(lineotn_handle->var.lineotn_mode)
    {
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL, lineotn_handle, dsi_stream, 1, 1, 0);
    }

    if (0 == dsi_stream)
    {
        /* Configure SFIS#0 n=10 */
        /* put to reset */
        if(FALSE == lineotn_handle->var.lineotn_mode)
        {
            sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,1);
            sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,1);
            lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 0);
            sysotn_field_SFIS_LOWPWR_set(NULL,lineotn_handle, 1);
            mld_sfis_n_rx_alarm_cfg(lineotn_handle, 0, FALSE);
        }
        else
        {
            sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,0,1);
            sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,0,1);
            lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 0);
            sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,1,1);
            sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,1,1);
            lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 1);
            sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,2,1);
            sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,2,1);
            lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 2);

            /* reset mux 2 sys stream_1x320_2 disconnected. */
            otn_mld_field_MUX2_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 1 line stream_10x32_4 is diconnected. */
            otn_mld_field_MUX1_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 0 line stream_10x32_2 disconnected. */
            otn_mld_field_MUX0_LINE_STREAM_set(NULL,lineotn_handle,0);

        }
    }
    else if(4 == dsi_stream)
    {
        if(TRUE == lineotn_handle->var.lineotn_mode)
        {
            sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,3,1);
            sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,3,1);
        }
    }

    if (TRUE == lineotn_handle->var.lineotn_mode &&
        0 == (dsi_stream%4))
    {
            /* check that all stream of the slice are unprovisioned */
        for (jtr = 0; jtr < 4; jtr++)
        {
            if (TRUE == lineotn_handle->var.lineotn_dsi_dist[((dsi_stream / 4) * 4) + jtr].dsi_provisioned)
            {
                break;
            }
        }
        /* if all streams of the slice are un-prov, then disable mux driving data to OPSM23 slice */
        if (4 <= jtr)
        {
            switch (opsm_idx)
            {
            case 0:
                otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,0);
                break;
            case 1:
                otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,0);
                break;
            case 2:
                otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,0);
                break;
            default:
                break;
            }
        }

        switch (opsm_idx)
        {
        case 0:
            /* 100G SFIS */
            /* set mux 7 sys stream_1x128_2 is connected */
            otn_mld_field_MUX7_SYS_STREAM_set(NULL,lineotn_handle,0);
                
            /* set mux 6 line stream_1x128_2 is connected */
            otn_mld_field_MUX6_LINE_STREAM_set(NULL,lineotn_handle,0);
                
            /* set mux 5 line stream_4x32_4 is connected */
            otn_mld_field_MUX5_LINE_STREAM_set(NULL,lineotn_handle,0);
                
            /* Disable RX alarm propagation */
            mld_sfis_n_rx_alarm_cfg(lineotn_handle, opsm_idx, FALSE);
            otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,0,0,1);
            otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,2,2,1);
            break;
        case 1:
            /* set mux 11 sys stream_1x128_11 is connected */
            otn_mld_field_MUX11_SYS_STREAM_set(NULL,lineotn_handle, 0);

            /* set mux 9 line stream_4x32_9 is connected */
            otn_mld_field_MUX9_LINE_STREAM_set(NULL,lineotn_handle, 0);


            /* Disable RX alarm propagation */
            mld_sfis_n_rx_alarm_cfg(lineotn_handle, opsm_idx, FALSE);
            otn_mld_field_range_SFIS_LOWPWR_set(NULL,lineotn_handle,3,3, 1);
            break;
        default:
            break;
        }    
    }

    PMC_RETURN();

}/* mld_sfis_n_reg_cleanup */

/*******************************************************************************
* mld_sfi51_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function SFI51 related registers of a given stream
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - unsigned, dsi stream to clean
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
PRIVATE void mld_sfi51_reg_cleanup(lineotn_handle_t *lineotn_handle,
                                   UINT32            dsi_stream)
{
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    switch (dsi_stream)
    {
    case 0:
        otn_mld_field_MUX7_LINE_STREAM_set(NULL,lineotn_handle, 0);
        otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle, 0);
        otn_mld_field_range_SFI51_LOWPWR_set(NULL,lineotn_handle,0,0,1);
        break;
    case 4:
        if (FALSE == lineotn_handle->var.lineotn_dsi_dist[8].dsi_provisioned ||
            MLD_SFI_51 != lineotn_handle->var.lineotn_dsi_dist[8].dsi_hw_ifc)
        {
            otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle, 0);
            otn_mld_field_range_SFI51_LOWPWR_set(NULL,lineotn_handle,1,1,1);
        }
        break;
    case 8:
        if (FALSE == lineotn_handle->var.lineotn_dsi_dist[4].dsi_provisioned ||
            MLD_SFI_51 != lineotn_handle->var.lineotn_dsi_dist[4].dsi_hw_ifc)
        {
            otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle, 0);
            otn_mld_field_range_SFI51_LOWPWR_set(NULL,lineotn_handle,1,1,1);
        }
        break;
    default:
        break;
    }

    PMC_RETURN();
}/* mld_sfi51_reg_cleanup */




/*******************************************************************************
* mld_2x_sfis_sfi51_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_14 - 120GB  2x40GB + 1x40GB  2xSFI-S(n=4+1) + 1xSFI5.1 \n
*      (lanes used: 0..4,5..9 and SFI5.1: 0..16)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfis_sfi51_prov(lineotn_handle_t   *lineotn_handle,
                                         lineotn_deskew_type_t deskew_type,
                                         mld_ifc_type_t     interface_type,
                                         UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 10,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        /* normalize dsi streams 0,5 to 0,4 */
        mld_rcp_defect_cfg(lineotn_handle,((dsi_stream/4)*4),LINE_OTN_RATE_OTU3,TRUE);

        if(0 == dsi_stream)
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
        else
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */

    }
    else if ( 0 == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 0-16 SFI5.1#0 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,LINE_OTN_CFG_14);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,2); /* set opsm23 #2 */
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfis_sfi51_prov */


/*******************************************************************************
* mld_2x_sfis_sfi51_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_14 - 120GB  2x40GB + 1x40GB  2xSFI-S(n=4+1) + 1xSFI5.1
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfis_sfi51_deprov(lineotn_handle_t   *lineotn_handle,
                                           lineotn_deskew_type_t deskew_type,
                                           mld_ifc_type_t     interface_type,
                                           UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 10,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        /* normalize dsi streams 0,5 to 0,4 */
        mld_rcp_defect_cfg(lineotn_handle,((dsi_stream/4)*4),LINE_OTN_RATE_OTU3,FALSE);

        if(0 == dsi_stream)
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
        else
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */

    }
    else if ( 0 == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 0-16 SFI5.1#0 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,LINE_OTN_CFG_14);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,2); /* reset opsm23 #2 */
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

}/* mld_2x_sfis_sfi51_deprov */


/*******************************************************************************
* mld_2x_sfi51_otl34_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_16 - 120GB  2x40GB + 1x40GB  2xSFI5.1 + 1xOTL3.4 \n
*      (lanes used: SFI5.1: 0..16,17..33 and OTL 0..3 or 8..11)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/
PRIVATE PMC_ERROR mld_2x_sfi51_otl34_prov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( 8 == dsi_stream && MLD_OTL == interface_type ) /* dsi streams 8-11 OTL3.4 */
    {
        if ( deskew_type != LINE_OTN_NO_DESKEW )
            PMC_RETURN ( LINEOTN_ERR_INVALID_PARAMETERS );

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_otl_prov(lineotn_handle,TRUE,dsi_stream);
    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,LINE_OTN_CFG_16);

        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
* mld_2x_sfi51_otl34_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_16 - 120GB  2x40GB + 1x40GB  2xSFI5.1 + 1xOTL3.4
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfi51_otl34_deprov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( 8 == dsi_stream && MLD_OTL == interface_type ) /* dsi streams 8-11 OTL3.4 */
    {
        if ( deskew_type != LINE_OTN_NO_DESKEW )
            PMC_RETURN ( LINEOTN_ERR_INVALID_PARAMETERS );

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);

    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,LINE_OTN_CFG_16);

        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
} /* mld_2x_sfi51_otl34_deprov */


/*******************************************************************************
* mld_2x_sfi51_4x_otu2_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_15 - 120GB   2x40GB + 4x10GB    2xSFI5.1 + 4xOTU2 \n
*      (lanes used: SFI5.1: 0..16,17..33 ; 10G: 0,1,2,3, 8,9,10,11)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8);
*                             for 40G SFIS configurations (0,5);
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfi51_4x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( dsi_stream >= 8 && MLD_OTU2 == interface_type ) /* dsi streams 8-11 OTU2 */
    {
        if ( deskew_type != LINE_OTN_NO_DESKEW )
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else {
            /* update number of otu2 resources */
            lineotn_handle->var.mld_curr_num_otu_configured |= (1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

            mld_opsm23_otl_prov(lineotn_handle,FALSE,dsi_stream);
        }
    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,LINE_OTN_CFG_15);

        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfi51_4x_otu2_prov */


/*******************************************************************************
* mld_2x_sfi51_4x_otu2_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_15 - 120GB   2x40GB + 4x10GB    2xSFI5.1 + 4xOTU2 \n
*      (SERDES lanes used: SFI5.1: 0..16,17..33; 10G: 0,1,2,3, 8,9,10,11)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfi51_4x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( dsi_stream >= 8  && MLD_OTU2 == interface_type ) /* dsi streams 8-11 OTU2 */
    {
        if ( deskew_type != LINE_OTN_NO_DESKEW )
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else
        {  
            /* update number of otu2 resources */
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,LINE_OTN_CFG_15);

        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfi51_4x_otu2_deprov */


/*******************************************************************************
* mld_sfis_n_4_7x_otu2_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_5 - 110GB  1x40GB + 7x10GB  1xSFI-S(n=4+1) + 7x10G \n
*      (10G SERDES lanes used: 0..4, 5,6,7,8,9,10,11)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_sfis_n_4_7x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
    }
    else if (dsi_stream >=5 && dsi_stream < 8) /* dsi streams 5-7 */
    {
        lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

        /* set opsm23#1 to 4x10G mode */
        mld_opsm23_otl_prov(lineotn_handle,FALSE,dsi_stream);

    }
    else if (dsi_stream >=8 && dsi_stream < 12)/* dsi streams 8-11 */
    {
        lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

        /* set OPSM23#2 on the distribution mux */
        mld_opsm23_otl_prov(lineotn_handle,FALSE,dsi_stream);

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_sfis_n_4_7x_otu2_prov */


/*******************************************************************************
* mld_sfis_n_4_7x_otu2_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_5 - 110GB  1x40GB + 7x10GB  1xSFI-S(n=4+1) + 7x10G

*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_sfis_n_4_7x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
    }
    else if (dsi_stream >=5 && dsi_stream < 8) /* dsi stream 5-7 */
    {
        /* update number of otu2 configured */
        lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

        /* reset OPSM23#1 on the distribution mux */
        mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
    }
    else if ( dsi_stream >=8 && dsi_stream < 12 )/* dsi streams 8-11 */
    {
        /* update number of otu2 configured */
        lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

        /* reset OPSM23#2 on the distribution mux */
        mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

}/* mld_sfis_n_4_7x_otu2_deprov */


/*******************************************************************************
* mld_sfis_otl34_3x_otu2_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_6 - 110GB  1x40GB + 1x40GB + 3x10GB \n
*      1xSFI-S(n=4+1) + 1x40G OTL 3.4 + 3x10G \n
*      (10G SERDES lanes used: 0..4, 8..11, 5,6,7)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s:
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/


PRIVATE PMC_ERROR mld_sfis_otl34_3x_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                              lineotn_deskew_type_t deskew_type,
                                              mld_ifc_type_t     interface_type,
                                              UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
    }
    else if (dsi_stream >=5 && dsi_stream < 8) /* dsi streams 5-7 */
    {
        lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

        /* set opsm23#1 to 4x10G mode */
        mld_opsm23_otl_prov(lineotn_handle,FALSE,dsi_stream);

    }
    else if (8 == dsi_stream )/* 40G dsi streams 8-11 */
    {
        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,FALSE );

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);

        /* 1X40G */
        /* set OPSM23#2 on the distribution mux for all 8-11 lanes */
        mld_opsm23_otl_prov(lineotn_handle,TRUE,dsi_stream);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_sfis_otl34_3x_otu2_prov */


/*******************************************************************************
* mld_sfis_otl34_3x_otu2_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_6 - 110GB  1x40GB + 1x40GB + 3x10GB
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
* 
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_sfis_otl34_3x_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                                lineotn_deskew_type_t deskew_type,
                                                mld_ifc_type_t     interface_type,
                                                UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
    }
    else if (dsi_stream >=5 && dsi_stream < 8) /* dsi stream 5-7 */
    {
        /* update number of otu2 configured */
        lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

        /* reset OPSM23#1 on the distribution mux */
        mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);

    }
    else if ( 8 == dsi_stream )/* 40G dsi streams 8-11 */
    {
        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);

        /* reset OPSM23#20 on the distribution mux */
        mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

}/* mld_sfis_otl34_3x_otu2_prov */


/*******************************************************************************
* mld_1x_sfi51_otl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_7 -  120GB  1x40GB + 8x10GB           1xSFI5.1 + 8xOTU2 \n
*      (SFI5.1 SERDES lanes used: 0..16; 10G SERDES lanes used: 4,5,6,7,8,9,10,11)
*    - LINE_OTN_CFG_8 -  120GB  1x40GB + 1x40GB + 4x10GB  1xOTL3.4 + 1xSFI5.1 + 4xOTU2\n
*      (SFI5.1 SERDES lanes used: 0..16; 10G SERDES lanes used: 4..7, 8,9,10,11)
*    - LINE_OTN_CFG_12 - 120GB  2x40GB + 1x40GB           2xOTL3.4 + 1xSFI5.1\n
*      (SFI5.1 SERDES lanes used: 0..16; 10G SERDES lanes used: 4..7, 8..11)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   lineotn_cfg             - enum, 1 of 3 possible configurations for
*                             LineOTN SS
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_1x_sfi51_otl_prov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        lineotn_cfg_type_t lineotn_cfg,
                                        UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    BOOL        opsm_mode_otl_34=FALSE;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);
    PMC_ASSERT(lineotn_cfg == LINE_OTN_CFG_7 ||
               lineotn_cfg == LINE_OTN_CFG_8 ||
               lineotn_cfg == LINE_OTN_CFG_12,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);

    if ( 0 == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 0-16 SFI5.1#0 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,lineotn_cfg);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
    }
    else if (dsi_stream >=4 && dsi_stream < 8) /* dsi streams 4-7 */
    {
        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_1 ,FALSE );

        /* configure OPSM23#1 streams 4-7 */
        switch(lineotn_cfg)
        {
        case LINE_OTN_CFG_7:/* 120GB  1x40GB + 8x10GB (0-16),(4,5,6,7,8,9,10,11)  */
            opsm_mode_otl_34 = FALSE;
            lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

            break;

        case LINE_OTN_CFG_8: /* 120GB  1x40GB + 1x40GB + 4x10GB (4-7),(0-16),(8,9,10,11)  */
        case LINE_OTN_CFG_12:/* 120GB  2x40GB + 1x40GB  (4-7,8-11) (0-16)  */
            opsm_mode_otl_34 = TRUE;
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

        if ( PMC_SUCCESS == rc )
        {
            mld_opsm23_otl_prov(lineotn_handle,opsm_mode_otl_34,dsi_stream);
        }
    }
    else if (dsi_stream >=8 && dsi_stream < 12)/* dsi streams 8-11 */
    {
        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,FALSE );

        /* configure OPSM23#1 streams 8-11 */
        switch(lineotn_cfg)
        {
        case LINE_OTN_CFG_7:/* 120GB  1x40GB + 8x10GB (0-16),(4,5,6,7,8,9,10,11)  */
        case LINE_OTN_CFG_8: /* 120GB  1x40GB + 1x40GB + 4x10GB (4-7),(0-16),(8,9,10,11)  */
            opsm_mode_otl_34 = FALSE;
            lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);
            break;

        case LINE_OTN_CFG_12:/* 120GB  2x40GB + 1x40GB  (4-7,8-11) (0-16)  */
            opsm_mode_otl_34 = TRUE;
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);

            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

        if( PMC_SUCCESS == rc )
        {
            mld_opsm23_otl_prov(lineotn_handle,opsm_mode_otl_34,dsi_stream);
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_1x_sfi51_otl_prov */


/*******************************************************************************
* mld_1x_sfi51_otl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_7 -  120GB  1x40GB + 8x10GB           1xSFI5.1 + 8xOTU2
*    - LINE_OTN_CFG_8 -  120GB  1x40GB + 1x40GB + 4x10GB  1xOTL3.4 + 1xSFI5.1 + 4xOTU2
*    - LINE_OTN_CFG_12 - 120GB  2x40GB + 1x40GB           2xOTL3.4 + 1xSFI5.1
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   lineotn_cfg             - enum, 1 of 3 possible configurations for
*                             LineOTN SS
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_1x_sfi51_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          lineotn_cfg_type_t lineotn_cfg,
                                          UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);
    PMC_ASSERT(lineotn_cfg == LINE_OTN_CFG_7 ||
               lineotn_cfg == LINE_OTN_CFG_8 ||
               lineotn_cfg == LINE_OTN_CFG_12,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);


    if ( 0 == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 0-16 SFI5.1#0 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,lineotn_cfg);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
    }
    else if (dsi_stream >=4 && dsi_stream < 8) /* dsi stream 4-7 */
    {
        /* update number of otu2 configured */
        if( LINE_OTN_CFG_7 == lineotn_cfg )
        {
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

            /* reset OPSM23#1 on the distribution mux */
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else  /* this is a 40G configuration - deprov all 4-7 lanes from OPSM#1*/
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);

            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }

    }
    else if (dsi_stream >=8 && dsi_stream < 12) /* dsi streams 8-11 */
    {
        /* update number of otu2 configured */
        if( LINE_OTN_CFG_7 == lineotn_cfg ||
            LINE_OTN_CFG_8 == lineotn_cfg )
        {
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

            /* reset OPSM23#2 on the distribution mux */
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else /* this is a 40G configuration - deprov all 8-11 lanes from OPSM#2 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);

            /* reset OPSM23# 2 on the distribution mux */
            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
} /* mld_1x_sfi51_otl_deprov */


/*******************************************************************************
* mld_sfis_sfi51_otl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_9   120GB   1x40GB + 1x40GB + 4x10GB-1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2 \n
*      (SFI5.1 SERDES lanes used: 17..33; 10G SERDES lanes used: 0..4, 8,9,10,11)
*    - LINE_OTN_CFG_10  120GB   1x40GB + 1x40GB + 1x40GB-1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1\n
*      (SFI5.1 SERDES lanes used: 17..33; 10G SERDES lanes used: 0..4, 8..11)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   lineotn_cfg             - enum, 1 of 3 possible configurations for
*                             LineOTN SS
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_sfis_sfi51_otl_prov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          lineotn_cfg_type_t lineotn_cfg,
                                          UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    BOOL opsm_mode_otl_34 = FALSE;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);
    PMC_ASSERT(lineotn_cfg == LINE_OTN_CFG_9 ||
               lineotn_cfg == LINE_OTN_CFG_10,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);

    if ( LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,lineotn_cfg);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */
    }

    else if( (0 == dsi_stream ) && (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
    }
    else if (dsi_stream >= 8 && dsi_stream < 12)/* dsi streams 8-11 */
    {
        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,FALSE );

        /* configure OPSM23#1 streams 8-11 */
        switch(lineotn_cfg)
        {
        case LINE_OTN_CFG_9:/* 120GB,1x40GB + 1x40GB + 4x10GB,(0-4)(17-33)(8,9,10,11) */
            opsm_mode_otl_34 = FALSE;
            lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);
            break;

        case LINE_OTN_CFG_10:/* 120GB,1x40GB + 1x40GB + 1x40GB,(0-4)(17-33)(8-11) */
            opsm_mode_otl_34 = TRUE;
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

        if( PMC_SUCCESS == rc )
        {
            mld_opsm23_otl_prov(lineotn_handle,opsm_mode_otl_34,dsi_stream);
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_sfis_sfi51_otl_prov */


/*******************************************************************************
* mld_sfis_sfi51_otl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_9   120GB   1x40GB + 1x40GB + 4x10GB-1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2
*    - LINE_OTN_CFG_10  120GB   1x40GB + 1x40GB + 1x40GB-1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   lineotn_cfg             - enum, 1 of 19 possible configurations for
*                             LineOTN SS
*   deskew_type             - enum,configuration mode for dsi stream/s:
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   lineotn_cfg             - enum, 1 of 3 possible configurations for
*                             LineOTN SS
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/


PRIVATE PMC_ERROR mld_sfis_sfi51_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                            lineotn_deskew_type_t deskew_type,
                                            mld_ifc_type_t     interface_type,
                                            lineotn_cfg_type_t lineotn_cfg,
                                            UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);
    PMC_ASSERT(lineotn_cfg == LINE_OTN_CFG_9 ||
               lineotn_cfg == LINE_OTN_CFG_10,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);

    if ( LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream && MLD_SFI_51 == interface_type ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,lineotn_cfg);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */

    }
    else if( (0 == dsi_stream)&&(MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);
        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
    }
    else if (dsi_stream >=8 && dsi_stream < 12) /* dsi streams 8-11 */
    {
        /* update number of otu2 configured */
        if( LINE_OTN_CFG_9 == lineotn_cfg)
        {
            lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

            /* reset OPSM23#2 on the distribution mux */
            mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
        }
        else /* this is a 40G configuration - deprov all 8-11 lanes from OPSM#2 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);

            /* reset OPSM23#2 on the distribution mux */
            mld_opsm23_otl_deprov(lineotn_handle,TRUE,dsi_stream);
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
} /* mld_sfis_sfi51_otl_deprov */


/*******************************************************************************
* mld_2x_sfis_otu2_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_13 - 100GB 2x40GB + 2x10GB  2xSFI-S(n=4+1) + 2xOTU2\n
*      (10G SERDES lanes used: 0..4,5..9, 10,11)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*   
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51,
*                             MLD_SFIS,
*                             MLD_OTL,
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G OTL configurations (0,4,8),
*                             for 40G SFIS configurations (0,5),
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
* 
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfis_otu2_prov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        if(0 == dsi_stream)
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */
        }
    }
    else if (dsi_stream >=10 && dsi_stream < 12)/* dsi streams 10,11 */
    {
        lineotn_handle->var.mld_curr_num_otu_configured|=(1<<dsi_stream);

        /* 4X10G */
        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,TRUE);

        /* set OPSM23#2 on the distribution mux */
        mld_opsm23_otl_prov(lineotn_handle,FALSE,dsi_stream);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfis_otu2_prov */


/*******************************************************************************
* mld_2x_sfis_otu2_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_13 - 100GB 2x40GB + 2x10GB  2xSFI-S(n=4+1) + 2xOTU2
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51
*                             MLD_SFIS
*                             MLD_OTL
*                             MLD_OTU2
*
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11)
*                             for 40G OTL configurations (0,4,8)
*                             for 40G SFIS configurations (0,5)
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfis_otu2_deprov(lineotn_handle_t   *lineotn_handle,
                                          lineotn_deskew_type_t deskew_type,
                                          mld_ifc_type_t     interface_type,
                                          UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);

        if(0 == dsi_stream)
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */
        }

    }
    else if ( dsi_stream >=10 && dsi_stream < 12 )/* dsi streams 10,11 */
    {
        /* update number of otu2 configured */
        lineotn_handle->var.mld_curr_num_otu_configured &= ~(1<<dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,dsi_stream,LINE_OTN_RATE_OTU2,FALSE);

        /* reset OPSM23#2 on the distribution mux */
        mld_opsm23_otl_deprov(lineotn_handle,FALSE,dsi_stream);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

}/* mld_2x_sfis_otu2_deprov */


/*******************************************************************************
* mld_2x_sfi51_sfis_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions resources for the following configuration:
*    - LINE_OTN_CFG_17 - 120GB   2x40GB + 1x40GB    2xSFI5.1 + 1xSFI-S(n=4+1)\n
*      (SFI5.1 SERDES lanes used: 0..16,17..33; 10G SERDES lanes used: 0..4)
* 
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51
*                             MLD_SFIS
*                             MLD_OTL
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11)
*                             for 40G OTL configurations (0,4,8)
*                             for 40G SFIS configurations (0,5)
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfi51_sfis_prov(lineotn_handle_t   *lineotn_handle,
                                         lineotn_deskew_type_t deskew_type,
                                         mld_ifc_type_t     interface_type,
                                         UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_prov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,TRUE);

        mld_opsm23_sfi_prov(lineotn_handle,deskew_type,0); /* set opsm23 #0 */
    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_prov(lineotn_handle,dsi_stream,LINE_OTN_CFG_17);

        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #2 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,2); /* set opsm23 #2 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,TRUE);
            mld_opsm23_sfi_prov(lineotn_handle,deskew_type,1); /* set opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfi51_sfis_prov */


/*******************************************************************************
* mld_2x_sfi51_sfis_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions resources for the following configuration:
*    - LINE_OTN_CFG_17 - 120GB   2x40GB + 1x40GB    2xSFI5.1 + 1xSFI-S(n=4+1)\n
*      (SFI5.1 SERDES lanes used: 0..16,17..33, 10G SERDES lanes used: 0..4)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   interface_type          - enum,mld HW interface for a dsi stream:
*                             MLD_SFI_51
*                             MLD_SFIS
*                             MLD_OTL
*                             MLD_OTU2
*
*   dsi_stream              - unsigned, dsi stream to configure
*                             for 10G configurations (0..11)
*                             for 40G OTL configurations (0,4,8)
*                             for 40G SFIS configurations (0,5)
*                             for 40G SFI5.1 configurations (0,17)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this stream.
*******************************************************************************/

PRIVATE PMC_ERROR mld_2x_sfi51_sfis_deprov(lineotn_handle_t   *lineotn_handle,
                                           lineotn_deskew_type_t deskew_type,
                                           mld_ifc_type_t     interface_type,
                                           UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 18,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deskew_type < LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,deskew_type,0);
    PMC_ASSERT(interface_type < LAST_MLD_IFC_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,interface_type,0);

    if( (0 == dsi_stream ) &&
        (MLD_SFIS == interface_type) ) /* dsi streams 0-4 SFIS#3 */
    {
        mld_sfis_40G_deprov(lineotn_handle,dsi_stream);

        /* set rcp defects broadcasts */
        mld_rcp_defect_cfg(lineotn_handle,0,LINE_OTN_RATE_OTU3,FALSE);

        mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,0); /* reset opsm23 #0 */
    }
    else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
              (MLD_SFI_51 == interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
    {
        mld_sfi51_deprov(lineotn_handle,dsi_stream,LINE_OTN_CFG_17);
        if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #2 */
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,8,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,2); /* reset opsm23 #2 */
        }
        else
        {
            /* set rcp defects broadcasts */
            mld_rcp_defect_cfg(lineotn_handle,4,LINE_OTN_RATE_OTU3,FALSE);
            mld_opsm23_sfi_deprov(lineotn_handle,deskew_type,1); /* reset opsm23 #1 */
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_2x_sfi51_sfis_deprov */


/*******************************************************************************
*  mld_opsm23_common_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets common configuration registers for opsm23.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   opsm23_idx              - unsigned, opsm23 to configure (0,1,2)
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
PRIVATE void mld_opsm23_common_cfg (lineotn_handle_t *lineotn_handle,
                                    UINT8             opsm23_idx  )
{
    UINT8 dsi_stream = 0;
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(opsm23_idx<3,LINEOTN_ERR_INVALID_PTR,opsm23_idx,0);

    dsi_stream = opsm23_idx*4; /* first dsi stream in every opsm23 */

    if(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm)
    {
        opsm23_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm23_idx,0);/* data is coming in not scrambled */
        opsm23_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm23_idx,1);/* data scramble in opsm */
        opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm23_idx,0);/* data coming from the fibre is always scrambled */
        opsm23_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm23_idx,1);/* data is scramble in opsm */
        opsm23_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm23_idx,0);
    }
    else
    {
        opsm23_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,opsm23_idx,1);/* data is coming in scrambled */
        opsm23_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm23_idx,0);/* data scramble in otu_frm */
        opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,opsm23_idx,0);/* data is coming in scrambled */
        opsm23_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,opsm23_idx,0);/* data is scramble in OTU_FRM */
        opsm23_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,opsm23_idx,1);
    }

    PMC_RETURN();

}/* mld_opsm23_common_cfg */


/*******************************************************************************
*  mld_opsm23_otl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions opsm23 for OTL3.4/OTU2 configurations
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   opsm_mode_otl_34        - boolean,TRUE : OTL3.4 (1x40G) mode;
*                                     FALSE : OTU2 (4x10G) mode
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_opsm23_otl_prov(lineotn_handle_t   *lineotn_handle,
                                      BOOL               opsm_mode_otl_34,
                                      UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* set mld top level otn selector to OPSM23 */
    otn_mld_field_OTU4_SEL_set(NULL,lineotn_handle,0);

    if( dsi_stream < 4 ) /* dsi streams 0-3 */
    {
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_0 ,FALSE );
        mld_opsm23_scramble_set(lineotn_handle,0,dsi_stream,lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm);

        /* set opsm23#0 to OTL34 or 4x10G mode streams 0-3 */
        if(opsm_mode_otl_34)
        {
            /* set OPSM23#0 on the distribution mux for all 0-3 lanes */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,0,3,3,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,1,3,3,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,2,3,3,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,3,3,3,1);

            /* set mux 4 line stream_4x32_0 is connected */
            otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,1);

            opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,0,0);
            opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,0,0);
            opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,0,1);
            opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,0,1);
        }
        else  /* 4x10G */
        {
            /* set OPSM23#0 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,3,3,1);

            /* configure muxes only once on the first otu2 provisioned */
            if( 0 == otn_mld_field_MUX4_LINE_STREAM_get(NULL,lineotn_handle) )
            {
                /* set mux 4 line stream_4x32_0 is connected */
                otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,1);

                opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,0,0);
                opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,0,0);
                opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,0,1);
                opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,0,1);
            }
        }

    }
    else if ( dsi_stream >= 4 && dsi_stream < 8 ) /* dsi streams 4-7 */
    {
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,1,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_1 ,FALSE );
        mld_opsm23_scramble_set(lineotn_handle,1,dsi_stream,lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm);

        /* set opsm23#1 to OTL34 or 4x10G mode */
        if(opsm_mode_otl_34)
        {
            /* set OPSM23#1 on the distribution mux for all 4-7 lanes */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,4,5,5,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,5,5,5,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,6,5,5,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,7,5,5,1);

            /* set mux 8 line stream_4x32_5 is connected */
            otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,1);

            opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,1,0);
            opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,1,0);
            opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,1,1);
            opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,1,1);

        }
        else /* 4X10G */
        {
            /* set OPSM23#1 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,5,5,1);

            /* configure muxes only once on the first otu2 provisioned */
            if( 0 == otn_mld_field_MUX8_LINE_STREAM_get(NULL,lineotn_handle) )
            {
                /* set mux 8 line stream_4x32_5 is connected */
                otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,1);

                opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,1,0);
                opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,1,0);
                opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,1,1);
                opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,1,1);
            }
        }

    }
    else if ( dsi_stream >= 8 )/* dsi streams 8-11 */
    {
        /* configure OPSM23#2 streams 8-11 */
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,2,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,FALSE );
        mld_opsm23_scramble_set(lineotn_handle,2,dsi_stream,lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm);

        if(opsm_mode_otl_34)
        {
            /* set OPSM23#2 on the distribution mux for all 8-11 lanes */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,8,7,7,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,9,7,7,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,10,7,7,1);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,11,7,7,1);

            /* set mux 12 line stream_4x32_10 is connected */
            otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,1);

            opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,2,0);
            opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,2,0);
            opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,2,1);
            opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,2,1);

        }
        else /* 4X10G */
        {
            /* set OPSM23#2 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,7,7,1);

            /* configure muxes only once on the first otu2 provisioned */
            if( 0 == otn_mld_field_MUX12_LINE_STREAM_get(NULL,lineotn_handle) )
            {
                /* set mux 12 line stream_4x32_10 is connected */
                otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,1);

                opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,2,0);
                opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,2,0);
                opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,2,1);
                opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,2,1);
            }

        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_opsm23_otl_prov */


/*******************************************************************************
*  mld_opsm23_otl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions opsm23 for SFIS/SFI5.1 configurations
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   opsm_mode_otl_34        - boolean,TRUE : OTL3.4 (1x40G) mode;
*                                     FALSE : OTU2 (4x10G) mode
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G OTL configurations (0,4,8)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_opsm23_otl_deprov(lineotn_handle_t   *lineotn_handle,
                                        BOOL               opsm_mode_otl_34,
                                        UINT8              dsi_stream)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(dsi_stream < 4) /* dsi stream 0-3 */
    {
        if( FALSE==opsm_mode_otl_34 )
        {
            /* reset OPSM23#0 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,3,3,0);
        }
        else  /* this is a 40G configuration - deprov all 0-3 lanes from OPSM#0*/
        {
            /* reset OPSM23#0 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,0,3,3,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,1,3,3,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,2,3,3,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,3,3,3,0);

        }

        /* deprovision only if we have no OTU2 configured */
        if( opsm_mode_otl_34 ||  /* 40G prov */
            ( FALSE==opsm_mode_otl_34 && /* 10G prov and all 10G cleared */
              0 == (lineotn_handle->var.mld_curr_num_otu_configured & 0x0000000f) )
            )
        {
            /* reset mux 4 line stream_4x32_0  */
            otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* toggle reset OPSM23#0 in order to clear data pata */
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0,1);
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0,0);

            /* put required modules into low power */
            mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_0 ,TRUE );
        }
    }
    else if (dsi_stream >=4 && dsi_stream < 8) /* dsi stream 4-7 */
    {
        if( FALSE==opsm_mode_otl_34 )
        {
            /* reset OPSM23#1 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,5,5,0);
        }
        else  /* this is a 40G configuration - deprov all 4-7 lanes from OPSM#1*/
        {
            /* reset OPSM23#1 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,4,5,5,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,5,5,5,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,6,5,5,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,7,5,5,0);
        }

        /* deprovision only if we have no OTU2 configured */
        if( opsm_mode_otl_34 ||  /* 40G prov */
            (FALSE==opsm_mode_otl_34 && 0 == (lineotn_handle->var.mld_curr_num_otu_configured & 0x000000f0)) )
        {
            /* reset mux 8 line stream_4x32_5  */
            otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* toggle reset OPSM23#1 in order to clear data pata */
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1,1);
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1,0);

            /* put required modules into low power */
            mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_1 ,TRUE );
        }
    }
    else if ( dsi_stream >= 8  && dsi_stream < 12)/* dsi streams 8-11 */
    {
        if( FALSE==opsm_mode_otl_34 )
        {
            /* reset OPSM23#2 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,dsi_stream,7,7,0);
        }
        else /* this is a 40G configuration - deprov all 8-11 lanes from OPSM#2 */
        {
            /* reset OPSM23#20 on the distribution mux */
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,8,7,7,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,9,7,7,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,10,7,7,0);
            otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,11,7,7,0);
        }

        /* deprovision only if we have no OTU2 configured */
        if( opsm_mode_otl_34 || /* 40G prov */
            (FALSE==opsm_mode_otl_34 && 0 == (lineotn_handle->var.mld_curr_num_otu_configured & 0x00000f00))  )
        {
            /* reset mux 12 line stream_4x32_10  */
            otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* toggle reset OPSM23#2 in order to clear data pata */
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,2,1);
            opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,2,0);

            /* put required modules into low power */
            mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,TRUE );
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle, dsi_stream / 4, dsi_stream%4, dsi_stream%4, 1);/* data scramble in opsm */
    }

    PMC_RETURN(rc);
}/* mld_opsm23_otl_deprov */


/*******************************************************************************
*  mld_opsm23_sfi_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions opsm23 for SFIS/SFI5.1 configurations
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   opsm23_idx              - unsigned, opsm23 to configure (0,1,2)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/

PRIVATE PMC_ERROR mld_opsm23_sfi_prov(lineotn_handle_t   *lineotn_handle,
                                      lineotn_deskew_type_t deskew_type,
                                      UINT8              opsm23_idx)
{
    BOOL      opsm_mode_otl_34 = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    /* Deskew_type and opsm23_idx are checked below */
    
    /* set mld top level otn selector to OPSM23 */
    otn_mld_field_OTU4_SEL_set(NULL,lineotn_handle,0);

    if( 0 == opsm23_idx ) /* OPSM#23 #0 */
    {
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_0 ,FALSE );
        mld_opsm23_common_cfg (lineotn_handle,opsm23_idx);

        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* set mux 7 sys stream_1x128_2 is connected */
            otn_mld_field_MUX7_SYS_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 6 line stream_1x128_2 is connected */
            otn_mld_field_MUX6_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* configure OPSM23#0 */
            opsm_mode_otl_34=FALSE;
            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* set mux 7 sys stream_1x128_4 is connected */
            otn_mld_field_MUX7_SYS_STREAM_set(NULL,lineotn_handle,1);

            /* set mux 5 line stream_4x32_4 is connected */
            otn_mld_field_MUX5_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 4 line stream_4x32_2 is connected */
            otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* configure OPSM23#0 */
            opsm_mode_otl_34=TRUE;
            break;


        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else if ( 1 == opsm23_idx ) /* OPSM#23 #1 */
    {
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,1,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,1,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_1 ,FALSE );
        mld_opsm23_common_cfg (lineotn_handle,opsm23_idx);

        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* set mux 11 sys stream_1x128_9 is connected */
            otn_mld_field_MUX11_SYS_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 10 line stream_1x128_9 is connected */
            otn_mld_field_MUX10_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* configure OPSM23#1 */
            opsm_mode_otl_34=FALSE;
            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* set mux 11 sys stream_1x128_11 is connected */
            otn_mld_field_MUX11_SYS_STREAM_set(NULL,lineotn_handle,1);

            /* set mux 9 line stream_4x32_9 is connected */
            otn_mld_field_MUX9_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 8 line stream_4x32_7 is connected */
            otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* configure OPSM23#1 */
            opsm_mode_otl_34=TRUE;
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else if( 2 == opsm23_idx ) /* OPSM#23 #2 */
    {
        /* make sure we are not in reset */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,2,0);
        opsm23_field_RX_LINE_SOFT_RESET_set(NULL,lineotn_handle,2,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM23_2 ,FALSE );
        mld_opsm23_common_cfg (lineotn_handle,opsm23_idx);

        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* set mux 13 sys stream_1x128_17 is connected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,4);

            /* configure OPSM23#2 */
            opsm_mode_otl_34=FALSE;
            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* set mux 13 sys stream_1x128_15 is connected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,2);

            /* set mux 12 line stream_4x32_12 is connected */
            otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,2);

            /* configure OPSM23#2 */
            opsm_mode_otl_34=TRUE;
            break;


        default: /* opsm23 #2 does not have a reserved mode */
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    if( PMC_SUCCESS == rc )
    {
        /* set opsm23# n to OTL34 over SFI or SFI mode */
        if(opsm_mode_otl_34)
        {   /* OTL over SFI */
            opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,opsm23_idx,0);
            opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,opsm23_idx,0);
            opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,opsm23_idx,1);
            opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,opsm23_idx,1);
        }
        else
        {   /* SFI */
            opsm23_field_TX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,opsm23_idx,0);
            opsm23_field_RX_OTL_3_4_ENABLE_set(NULL,lineotn_handle,opsm23_idx,0);
            opsm23_field_TX_DATA_4X10G_MODE_set(NULL,lineotn_handle,opsm23_idx,0);
            opsm23_field_RX_DATA_4X10G_MODE_set(NULL,lineotn_handle,opsm23_idx,0);
        }
    }

    PMC_RETURN(rc);
}/* mld_opsm23_sfi_prov */

/*******************************************************************************
*  mld_opsm23_sfi_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions opsm23 for SFIS/SFI5.1 configurations
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
*   opsm23_idx              - unsigned, opsm23 to configure (0,1,2)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_opsm23_sfi_deprov(lineotn_handle_t   *lineotn_handle,
                                        lineotn_deskew_type_t deskew_type,
                                        UINT8              opsm23_idx)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    /* deskew_type and opsm23_idx are checked below for valid values */

    if( 0 == opsm23_idx ) /* opsm23 #0 */
    {
        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* reset mux 7 sys stream_1x128_2 disconnected */
            otn_mld_field_MUX7_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 6 line stream_1x128_2 disconnected */
            otn_mld_field_MUX6_LINE_STREAM_set(NULL,lineotn_handle,0);

            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* reset mux 7 sys stream_1x128_4 disconnected */
            otn_mld_field_MUX7_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 5 line stream_4x32_4 disconnected */
            otn_mld_field_MUX5_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 4 line stream_4x32_2 disconnected */
            otn_mld_field_MUX4_LINE_STREAM_set(NULL,lineotn_handle,0);
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }
    }
    else if ( 1 == opsm23_idx ) /* opsm23 #1 */
    {
        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* reset mux 11 sys stream_1x128_9 disconnected */
            otn_mld_field_MUX11_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 10 line stream_1x128_9 disconnected */
            otn_mld_field_MUX10_LINE_STREAM_set(NULL,lineotn_handle,0);
            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* reset mux 11 sys stream_1x128_11 disconnected */
            otn_mld_field_MUX11_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 9 line stream_4x32_9 disconnected */
            otn_mld_field_MUX9_LINE_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 8 line stream_4x32_7 disconnected */
            otn_mld_field_MUX8_LINE_STREAM_set(NULL,lineotn_handle,0);
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else if( 2 == opsm23_idx ) /* opsm23 #2 */
    {
        /*  No configuration for SFI5.1(n=17)#0 on the distribution mux */
        switch(deskew_type)
        {
        case LINE_OTN_OIF_SFIS_DESKEW:
            /* reset mux 13 sys stream_1x128_17 disconnected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,0);
            break;

        case LINE_OTN_ENHANCED_SFIS_DESKEW:
            /* reset mux 13 sys stream_1x128_15 disconnected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 12 line stream_4x32_12 disconnected */
            otn_mld_field_MUX12_LINE_STREAM_set(NULL,lineotn_handle,0);
            break;

        default:/* opsm23 #2 does not have a reserved mode */
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    if( PMC_SUCCESS == rc )
    {
        /* toggle reset OPSM23#0 in order to clear data pata */
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,opsm23_idx,1);
        opsm23_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,opsm23_idx,0);

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, (mld_hw_module_t)(MLD_OPSM23_0 + opsm23_idx) ,FALSE );
    }

    PMC_RETURN(rc);
}/* mld_opsm23_sfi_deprov */


/*******************************************************************************
*  mld_opsm4_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions opsm4 for OTL4.10/SFI configurations
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   deskew_type             - enum,configuration mode for dsi stream/s
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_opsm4_prov(lineotn_handle_t   *lineotn_handle,
                                 lineotn_deskew_type_t deskew_type )
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* make sure we are not in reset */
    if(lineotn_handle->var.lineotn_mode)
    {
        opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        opsm4_field_range_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,9,0);
    }
    else
    {
        sysotn_opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        sysotn_opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        sysotn_opsm4_field_range_TX_LINE_SOFT_RESET_set(NULL,lineotn_handle,0,9,0);
    }

    /* take required modules out of low power */
    (void) mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM4 ,FALSE );

    /* set mld top level otn selector to OPSM4 */
    otn_mld_field_OTU4_SEL_set(NULL,lineotn_handle,1);

    /* set mld top level otn selector to OPSM4 */
    if (lineotn_handle->var.lineotn_mode)
    {
        otn_mld_field_OTU4_SEL_set(NULL,lineotn_handle,1);
    }

    /* in 100G configuration we check for scrambling config on the first dsi stream (0) */
    if (lineotn_handle->var.lineotn_dsi_dist[0].scramble_in_opsm)
    {
        if (lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in not scrambled */
            opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data scramble in opsm */
            opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data coming from the fibre is always scrambled */
            opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data is scramble in opsm */
            opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,0);
        }
        else 
        {
            sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in not scrambled */
            sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data scramble in opsm */
            sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data coming from the fibre is always scrambled */
            sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,1);/* data is scramble in opsm */   
            sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,0); 
        }
    }
    else
    {
        if (lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,1);/* data is coming in scrambled */
            opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data scramble in otu_frm */
            opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in scrambled */
            opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data is scramble in OTU_FRM */
            opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,1); /* should be set when descramber is either bypassed or not enabled, 
                                                                              in order to provide "unscrambled" data to OTU MFAS generator.*/
        }
        else
        {
            sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_set(NULL,lineotn_handle,1);/* data is coming in scrambled */
            sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data scramble in otu_frm */
            sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set(NULL,lineotn_handle,0);/* data is coming in scrambled */
            sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle,0);/* data is scramble in OTU_FRM */
            sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_set(NULL,lineotn_handle,1); /* should be set when descramber is either bypassed or not enabled, 
                                                                                    in order to provide "unscrambled" data to OTU MFAS generator.*/
        }
    }

    switch (deskew_type)
    {
    case LINE_OTN_NO_DESKEW:
        if (lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0xf); /*--> missing from current RDA*/ /* enable tx OTL */
            opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,1); /* enable rx */
            opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable tx OTL */
            opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
            opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
            opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,1); /* set to otl mode */
        }
        else
        {
            sysotn_opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0xf); /*--> missing from current RDA*/ /* enable tx OTL */
            sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable tx OTL */
            sysotn_opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
            sysotn_opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
            sysotn_opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,1); /* enable rx */
            sysotn_opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,1); /* set to otl mode */
        }
        break;

    case LINE_OTN_OIF_SFIS_DESKEW:

        if (lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0);/*--> missing from current RDA*/ /* disable tx OTL */
            opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,0); /* disable rx OTL*/
            opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable tx */
            opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable SFI direction */
            opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable SFI direction */
            opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,0); /* reset otl mode */
        }
        else
        {
            sysotn_opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0);/*--> missing from current RDA*/ /* disable tx OTL */
            sysotn_opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,0); /* disable rx OTL*/
            sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable tx */
            sysotn_opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable SFI direction */
            sysotn_opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,1); /* enable SFI direction */
            sysotn_opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,0); /* reset otl mode */
        }
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    if (rc == PMC_SUCCESS && pmc_enable_otu4_clock_cfg_in_fw(&lineotn_handle->base) == FALSE)
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set(NULL,lineotn_handle,26);
            opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set(NULL,lineotn_handle,26);
        }
        else
        {
            sysotn_opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set(NULL,lineotn_handle,26);
            sysotn_opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set(NULL,lineotn_handle,26);
        }
    }

    PMC_RETURN(rc);
}/* mld_opsm4_prov */


/*******************************************************************************
*  mld_opsm4_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions opsm4 and puts it into low power
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
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
PRIVATE void mld_opsm4_deprov(lineotn_handle_t   *lineotn_handle )
{

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        /* stop tx/rx */
        opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable tx */
        opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0);/*--> missing from current RDA*/ /* disable tx OTL*/
        opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,0); /* disable rx OTL */
        opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
        opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */

        opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,0); /* remove otl mode */

        /* reset HW to clear all gabage */
        opsm4_field_TX_FIFO_RESET_set(NULL,lineotn_handle,1);
        opsm4_field_RX_FIFO_RESET_set(NULL,lineotn_handle,1);
        opsm4_field_TX_FIFO_RESET_set(NULL,lineotn_handle,0);
        opsm4_field_RX_FIFO_RESET_set(NULL,lineotn_handle,0);

        opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1);
        opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,1);
        opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
    }
    else
    {
        /* stop tx/rx */
        sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable tx */
        sysotn_opsm4_field_TX_LINE_DEQUEUE_EN_set(NULL,lineotn_handle,0);/*--> missing from current RDA*/ /* disable tx OTL*/
        sysotn_opsm4_field_RX_LINE_EN_set(NULL,lineotn_handle,0); /* disable rx OTL */
        sysotn_opsm4_field_SFI_TX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */
        sysotn_opsm4_field_SFI_RX_DEQUEUE_EN_set(NULL,lineotn_handle,0); /* disable SFI direction */

        sysotn_opsm4_field_RX_OTL_4_10_ENABLE_set(NULL,lineotn_handle,0); /* remove otl mode */

        /* reset HW to clear all gabage */
        sysotn_opsm4_field_TX_FIFO_RESET_set(NULL,lineotn_handle,1);
        sysotn_opsm4_field_RX_FIFO_RESET_set(NULL,lineotn_handle,1);
        sysotn_opsm4_field_TX_FIFO_RESET_set(NULL,lineotn_handle,0);
        sysotn_opsm4_field_RX_FIFO_RESET_set(NULL,lineotn_handle,0);

        sysotn_opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,1);
        sysotn_opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,1);
        sysotn_opsm4_field_TX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
        sysotn_opsm4_field_RX_HARDWARE_RESET_set(NULL,lineotn_handle,0);
    }

    /* put required modules into low power */
    (void) mld_pwr_mode_config_set( lineotn_handle, MLD_OPSM4 ,TRUE );

    PMC_RETURN();
}/* mld_opsm4_deprov */


/*******************************************************************************
*  mld_sfis_40G_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions SFIS n=4+1 resources
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G SFIS configurations (0,5)
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void mld_sfis_40G_prov(lineotn_handle_t   *lineotn_handle,
                               UINT8              dsi_stream)
{
    UINT32 opsm_idx = 0;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT((dsi_stream == 0 || dsi_stream == 5),LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if( 0 == dsi_stream ) /* dsi streams 0-4 SFIS#3 */
    {
        /* take required modules out of low power */
        (void) mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_4_0,FALSE );

        /* set SFIS(n=4)#3 (0-4) on the distribution mux */
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,0,4,4,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,1,4,4,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,2,4,4,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,3,4,4,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,4,4,4,1);

        /* Configure SFIS#3 n=4 */
        /* put to reset to configure */
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,2,1);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,2,1);

        /* set # of lines to 4+1 */
        sfis_rx_field_RX_NUM_LANES_set(NULL,lineotn_handle,2,4);
        sfis_tx_field_TX_NUM_LANES_set(NULL,lineotn_handle,2,4);
        
        /* reset odd parity */
        lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 2);

        /* take out of reset once configured */
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,2,0);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,2,0);

        /* set mux 7 line stream_1x128_5 is connected */
        otn_mld_field_MUX7_LINE_STREAM_set(NULL,lineotn_handle,1);

        opsm_idx = 0;
    }
    else if ( 5 == dsi_stream ) /* dsi streams 5-9 SFIS#4 */
    {
        /* take required modules out of low power */
        (void) mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_4_1,FALSE );

        /* set SFIS(n=4)#4 (5-9) on the distribution mux */
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,5,6,6,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,6,6,6,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,7,6,6,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,8,6,6,1);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,9,6,6,1);

        /* Configure SFIS#4 n=4 */
        /* put to reset to configure */
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,3,1);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,3,1);

        /* set # of lines to 4+1 */
        sfis_rx_field_RX_NUM_LANES_set(NULL,lineotn_handle,3,4);
        sfis_tx_field_TX_NUM_LANES_set(NULL,lineotn_handle,3,4);

        /* reset odd parity */
        lineotn_mld_sfis_odd_parity_reset(lineotn_handle, 3);

        /* take out of reset once configured */
        sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle,3,0);
        sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle,3,0);


        /* set mux 11 line stream_1x128_12 is connected */
        otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle,1);

        opsm_idx = 1;
    }
    
    /* Configure RX alarm propagation */   
    mld_sfis_n_rx_alarm_cfg(lineotn_handle, opsm_idx, TRUE);

    PMC_RETURN();
}/* mld_sfis_40G_prov */


/*******************************************************************************
*  mld_sfis_40G_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions SFIS n=4+1 resources
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G SFIS configurations (0,5)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void mld_sfis_40G_deprov(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream)
{
    UINT32 opsm_idx = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 || dsi_stream == 5,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if( 0 == dsi_stream ) /* dsi streams 0-4 SFIS#3 */
    {
        /* reset SFIS(n=4)#3 0-4 on the distribution mux */
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,0,4,4,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,1,4,4,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,2,4,4,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,3,4,4,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,4,4,4,0);

        /* reset mux 7 line stream_1x128_5 disconnected */
        otn_mld_field_MUX7_LINE_STREAM_set(NULL,lineotn_handle,0);

        mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_4_0,TRUE );
        opsm_idx = 0;

    }
    else if ( 5 == dsi_stream ) /* dsi streams 5-9 SFIS#4 */
    {
        /* reset SFIS(n=4)#4 5-9 on the distribution mux */
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,5,6,6,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,6,6,6,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,7,6,6,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,8,6,6,0);
        otn_mld_field_range_LANE_10G_TX_SRC_set(NULL,lineotn_handle,9,6,6,0);

        /* reset mux 11 line stream_1x128_12 disconnected */
        otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle,0);

        mld_pwr_mode_config_set( lineotn_handle, MLD_SFIS_N_4_1,TRUE );
        opsm_idx = 1;
    }

    mld_sfis_n_rx_alarm_cfg(lineotn_handle, opsm_idx, FALSE);

    PMC_RETURN();
}


/*******************************************************************************
*  mld_sfi51_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions SFI5.1 resources
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G SFI5.1 configurations (0,17)
*
*   lineotn_cfg             - enum, 1 of 18 possible configurations for
*                             LineOTN SS
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_sfi51_prov(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream,
                                 lineotn_cfg_type_t lineotn_cfg )
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 || dsi_stream == LINEOTN_SFI51_1_DSI_STREAM_IDX,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if ( 0 == dsi_stream ) /* dsi streams 0-16 SFI5.1#0 */
    {
        /* take required modules out of low power */
        (void) mld_pwr_mode_config_set( lineotn_handle, MLD_SFI_51_0,FALSE );

        /* The routing of the first SFI5.1 interface depends on the configuration */
        switch (lineotn_cfg)
        {
        case LINE_OTN_CFG_7:
        case LINE_OTN_CFG_8:
        case LINE_OTN_CFG_12:
        case LINE_OTN_CFG_15:
        case LINE_OTN_CFG_16:
            /* SFI5.1 (0-16) routed to OPSM23 #0 */
            /* set mux 13 sys stream_1x128_6 is connected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,1);

            /* set mux 7 line stream_1x128_6 connected */
            otn_mld_field_MUX7_LINE_STREAM_set(NULL,lineotn_handle,2);
            break;

        case LINE_OTN_CFG_9:
        case LINE_OTN_CFG_10:
        case LINE_OTN_CFG_14:
        case LINE_OTN_CFG_17:
            /* SFI5.1 (0-16) routed to OPSM23 #2 */
            /* Configuration muxing is handled in the mld_opsm23_sfi_prov function */
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else if ( LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream ) /* dsi streams 17-33 SFI5.1#1 */
    {

        /* take required modules out of low power */
        mld_pwr_mode_config_set( lineotn_handle, MLD_SFI_51_1 ,FALSE );

        /*  No congiguration for SFI5.1(n=17)#1 on the distribution mux */

        /* set mux 11 line stream_1x128_13 is connected */
        otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle,2);

    }

    PMC_RETURN(rc);
}


/*******************************************************************************
*  mld_sfi51_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions SFI5.1 resources
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G SFI5.1 configurations (0,17)
*
*   lineotn_cfg             - enum, 1 of 18 possible configurations for
*                             LineOTN SS
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mld_sfi51_deprov(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream,
                                   lineotn_cfg_type_t lineotn_cfg )
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 || dsi_stream == LINEOTN_SFI51_1_DSI_STREAM_IDX,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if ( 0 == dsi_stream ) /* dsi streams 0-16 SFI5.1#0 */
    {
        /*  No congiguration for SFI5.1(n=17)#0 on the distribution mux */
        (void) mld_pwr_mode_config_set( lineotn_handle, MLD_SFI_51_0 ,TRUE );

        /*  Lineotn configuraton 17 routes to opsm 2 while all other configurations route to opsm 0 */
        /*  Configuration 17 muxing is handled in the mld_opsm23_sfi_prov(opsm=2) function          */
        if(LINE_OTN_CFG_17 != lineotn_cfg)
        {
            /* reset mux 13 sys stream_1x128_6 is disconnected */
            otn_mld_field_MUX13_SYS_STREAM_set(NULL,lineotn_handle,0);

            /* reset mux 7 line stream_1x128_6 disconnected */
            otn_mld_field_MUX7_LINE_STREAM_set(NULL,lineotn_handle,0);
        }

    }
    else if ( LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream ) /* dsi streams 17-33 SFI5.1#1 */
    {
        /*  No congiguration for SFI5.1(n=17)#1 on the distribution mux */

        /* reset mux 11 line stream_1x128_13 disconnected */
        otn_mld_field_MUX11_LINE_STREAM_set(NULL,lineotn_handle,0);

        mld_pwr_mode_config_set( lineotn_handle, MLD_SFI_51_1 ,TRUE );
    }

    PMC_RETURN(rc);
}



/*******************************************************************************
* mld_sfi_51_dsi_idx_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to help calculate dsi streams post mld range
*   for sfi 5.1 mld hw interface .
*
* INPUTS:
*
*   lineotn_cfg            - enum,lineOTN overwhole configuration mode.
*                            this enum specifies the lineOTN context in which we are
*                            the current dsi stream is being provisioned.
*                            There are 19 possible lineOTN configurations specified within
*                            line_otn_cfg enum.
*
*   dsi_type              - enum, sfi 5.1 dsi stream to configure:
*                            LINE_OTN_DSI_TYPE_SFI_51_A : lanes 0,16;
*                            LINE_OTN_DSI_TYPE_SFI_51_B : lanes 17,33
*
* OUTPUTS:
*   dsi_start              - pointer to unsigned,post mld dsi stream start range
*
*   dsi_end                - pointer to unsigned,post mld dsi stream end range
*
* RETURNS:
*   PMC_SUCCESS - on success, or LINEOTN_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mld_sfi_51_dsi_idx_get(lineotn_cfg_type_t lineotn_cfg,lineotn_dsi_type_t dsi_type,UINT32 *dsi_start,UINT32 *dsi_end )
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(dsi_start!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_end!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_type < LAST_LINE_OTN_DSI_TYPE ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    if( LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type ) /* only one option here 17-33 is always mapped to dsi streams 4-7 */
    {
        (*dsi_start) = 4;
        (*dsi_end)   = 7;
    }
    else if( LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )/* 16 can be either 8-11 or 0-3 depending on the configuration chosen */
    {
        switch(lineotn_cfg)
        {
        case LINE_OTN_CFG_7: /* (0-16) (4,5,6,7,8,9,10,11) */
        case LINE_OTN_CFG_8: /* (0-16) (4-7) (8,9,10,11)   */
        case LINE_OTN_CFG_12:/* (0-16) (4-7,8-11)          */
        case LINE_OTN_CFG_15: /* (0-16,17-33) (8,9,10,11) */
        case LINE_OTN_CFG_16:/* (0-16,17-33) (8-11) */
            (*dsi_start) = 0;
            (*dsi_end)   = 3;
            break;

        case LINE_OTN_CFG_14: /* (0-4,5-9)  (0-16) */
        case LINE_OTN_CFG_17: /* (0-16,17-33)   (0-4) */
            (*dsi_start) = 8;
            (*dsi_end)   = 11;
            break;

        default:
            (*dsi_start)= (*dsi_end) = 0;
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }
    }
    else
    {
        (*dsi_start)= (*dsi_end) = 0;
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

}/* mld_sfi_51_dsi_idx_get */


/*******************************************************************************
* mld_opsm_ssf_defect_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set/resets a defect bit on OPSM23/OPSM4 on an already provisioned
*   dsi stream for the SSF paths.
*   In order to make sure that user configuration is the one that is driving
*   the SSF output OPSM MI active bit is left in its default mode --> active mode.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0)
*
*   defect_bit              - unsigned,one bit value to represent the defect to set/reset:
*                             LINE_OTN_D_LOS_P,
*                             LINE_OTN_D_LOL_P,
*                             LINE_OTN_D_LOFLANE,
*                             LINE_OTN_D_AIS,
*                             LINE_OTN_D_LOF,
*
*   set_defect_bit          - boolean, TRUE: set the defect to active mode;
*                                      FALSE: reset defect to non-active mode
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. dsi stream must be provisioned before invoking this function otherwise
*      an error status will be returned.
* 
*******************************************************************************/
PUBLIC PMC_ERROR mld_opsm_ssf_defect_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         UINT32             defect_bit,
                                         BOOL               set_defect_bit)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       set_defect = (set_defect_bit?1:0);
    UINT8       i = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
            ) /* configure opsm4 */
        {
            /* For the SSF Defect Control registers, only instance #0 is used*/
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOL_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_SSF_LOL_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_SSF_LOL_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOFLANE:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_AIS:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOF:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_SSF_OTU4_LOF_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_SSF_OTU4_LOF_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else /* opsm23 to be configured */
        {
            UINT8 opsm23_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
            BOOL  OTU3_config  = ( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                                   lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 ? TRUE:FALSE);
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOL_P:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_SSF_LOL_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_SSF_LOL_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOFLANE:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_AIS:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOF:
                if(OTU3_config)
                {
                    for(i=0;i<4;i++)
                        opsm23_field_RX_SSF_OTU3_LOF_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                }
                else /* 10G - alias D_LOF to D_LOFLANE as the H/W uses LOFLANE */
                {
                    opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                }
                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }

        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_opsm_ssf_defect_set */


/*******************************************************************************
* mld_opsm_ssf_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets a defect bit value on OPSM23/OPSM4 on an already provisioned
*   dsi stream for the SSF paths.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0)
*
*   defect_bit              - unsigned,one bit value to retrieve:
*                             LINE_OTN_D_LOS_P,
*                             LINE_OTN_D_LOL_P,
*                             LINE_OTN_D_LOFLANE,
*                             LINE_OTN_D_AIS,
*                             LINE_OTN_D_LOF
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - requested bit value as was set in the relevant hw register or
*   LINE_OTN_DEFECT_ERROR on failure.
*
* NOTES:
*   1. dsi stream must be provisioned before invoking this function otherwise
*      an error status will be returned.
* 
*******************************************************************************/
PUBLIC UINT32 mld_opsm_ssf_defect_get(lineotn_handle_t   *lineotn_handle,
                                      UINT8              dsi_stream,
                                      UINT32             defect_bit)
{
    UINT32       defect_reg_value = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
            ) /* configure opsm4 */
        {
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_SSF_LANE_LOS_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_SSF_LANE_LOS_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOL_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_SSF_LOL_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_SSF_LOL_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOFLANE:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_AIS:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_SSF_LANE_OTUK_AIS_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_SSF_LANE_OTUK_AIS_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOF:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_SSF_OTU4_LOF_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_SSF_OTU4_LOF_get(NULL,lineotn_handle,0);
                }
                break;

            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }
        }
        else /* opsm23 to be configured */
        {
            UINT8 opsm23_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
            BOOL  OTU3_config  = ( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                                   lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 ? TRUE:FALSE);
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_SSF_LANE_LOS_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_SSF_LANE_LOS_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOL_P:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_SSF_LOL_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_SSF_LOL_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOFLANE:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_AIS:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_SSF_LANE_OTUK_AIS_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_SSF_LANE_OTUK_AIS_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOF:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_SSF_OTU3_LOF_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - alias D_LOF to D_LOFLANE as the H/W uses LOFLANE */
                    defect_reg_value =opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }

        }
    }

    PMC_RETURN(defect_reg_value);
}/* mld_opsm_ssf_defect_get */


/*******************************************************************************
* mld_opsm_defect_value_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets a defect bit value on OPSM23/OPSM4 on an already provisioned
*   dsi stream
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0)
*
*   defect_bit              - unsigned,one bit value to retrieve:
*                             LINE_OTN_D_LOS_P,
*                             LINE_OTN_D_LOFLANE,
*                             LINE_OTN_D_AIS
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - requested bit value as was set in the relevant hw register or
*   LINE_OTN_DEFECT_ERROR on failure.
*
* NOTES:
*   1. dsi stream must be provisioned before invoking this function otherwise
*      an error status will be returned.
* 
*******************************************************************************/
PUBLIC UINT32 mld_opsm_defect_value_get(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream,
                                        UINT32             defect_bit)
{
    UINT32       defect_reg_value = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
            ) /* configure opsm4 */
        {
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_LANE_LOS_INT_V_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_LANE_LOS_INT_V_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOFLANE:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_AIS:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value =opsm4_field_RX_LANE_OTUK_AIS_INT_V_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value =sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_V_get(NULL,lineotn_handle,0);
                }
                break;

            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }
        }
        else /* opsm23 to be configured */
        {
            UINT8 opsm23_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
            BOOL  OTU3_config  = ( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                                   lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 ? TRUE:FALSE);
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_LANE_LOS_INT_V_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_LANE_LOS_INT_V_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOFLANE:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_AIS:
                if(OTU3_config)
                    defect_reg_value =opsm23_field_RX_LANE_OTUK_AIS_INT_V_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value =opsm23_field_RX_LANE_OTUK_AIS_INT_V_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;


            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }

        }
    }

    PMC_RETURN(defect_reg_value);
}/* mld_opsm_defect_value_get */


/*******************************************************************************
* mld_opsm_ffail_defect_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set/resets a a defect bit on OPSM23/OPSM4 on an already provisioned
*   dsi stream for the FFAIL paths.
*   In order to make sure that user configuration is the one that is driving
*   the SSF output OPSM MI active bit is left in its default mode --> active mode.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0)
*
*   defect_bit              - unsigned,one bit value to represent the defect to set/reset:
*                             LINE_OTN_D_LOS_P,
*                             LINE_OTN_D_LOL_P,
*                             LINE_OTN_D_LOFLANE,
*                             LINE_OTN_D_AIS,
*                             LINE_OTN_D_LOF
**
*   set_defect_bit          - boolean, TRUE: set the defect to active mode
*                                      FALSE: reset defect to non-active mode
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. dsi stream must be provisioned before invoking this function otherwise
*      an error status will be returned.
* 
*******************************************************************************/
PUBLIC PMC_ERROR mld_opsm_ffail_defect_set(lineotn_handle_t   *lineotn_handle,
                                           UINT8              dsi_stream,
                                           UINT32             defect_bit,
                                           BOOL               set_defect_bit)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       set_defect = (set_defect_bit?1:0);
    UINT8       i = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
            ) /* configure opsm4 */
        {
            /* For the SSF Defect Control registers, only instance #0 is used*/
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOL_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_FAIL_LOL_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_FAIL_LOL_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOFLANE:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_AIS:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOF:
                if(lineotn_handle->var.lineotn_mode)
                {
                    opsm4_field_RX_FAIL_OTU4_LOF_set(NULL,lineotn_handle,i,set_defect);
                }
                else
                {
                    sysotn_opsm4_field_RX_FAIL_OTU4_LOF_set(NULL,lineotn_handle,i,set_defect);
                }
                break;

            case LINE_OTN_D_LOM: /* configure only mld - all 12 lanes */
                for(i=0;i<12;i++)
                {
                    if(lineotn_handle->var.lineotn_mode)
                    {
                        otn_mld_field_DLOM_TO_FFAIL_MAPPING_set(NULL,lineotn_handle,i,set_defect);
                    }
                    else
                    {
                        rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    }
                }
                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
        else /* opsm23 to be configured */
        {
            UINT8 opsm23_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
            BOOL  OTU3_config  = ( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                                   lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 ? TRUE:FALSE);

            /* Do not enable FFAIL when 10G I7FEC is selected */
            BOOL block_otu2_ffail_en = ( (OTU3_config==FALSE &&
                                          lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_I7 &&
                                          set_defect_bit == TRUE) ? TRUE : FALSE);

            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else if (block_otu2_ffail_en == FALSE) /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOL_P:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_FAIL_LOL_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else if (block_otu2_ffail_en == FALSE) /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_FAIL_LOL_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOFLANE:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else if (block_otu2_ffail_en == FALSE) /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_AIS:
                if(OTU3_config)
                    for(i=0;i<4;i++)
                        opsm23_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                else if (block_otu2_ffail_en == FALSE) /* 10G - configure one lane --> dsi_stream modulo 4*/
                    opsm23_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                break;

            case LINE_OTN_D_LOF:
                if(OTU3_config)
                {
                    for(i=0;i<4;i++)
                        opsm23_field_RX_FAIL_OTU3_LOF_set(NULL,lineotn_handle,opsm23_idx,i,set_defect);
                }
                else /* 10G - alias it to LOFLANE as the H/W uses the lane based LOF for 10G */
                {
                    opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4),set_defect);
                }
                break;

            case LINE_OTN_D_LOM: /* configure only mld */
                if(OTU3_config)
                    for(i=(opsm23_idx*4) ;i< (opsm23_idx*4)+4 ;i++) /* 0-3,4-7,8-11 */
                        otn_mld_field_DLOM_TO_FFAIL_MAPPING_set(NULL,lineotn_handle,i,set_defect);
                else if (block_otu2_ffail_en == FALSE)
                    otn_mld_field_DLOM_TO_FFAIL_MAPPING_set(NULL,lineotn_handle,dsi_stream,set_defect);
                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }

        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* mld_opsm_ffail_defect_set */


/*******************************************************************************
* mld_opsm_ffail_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets a defect bit value on OPSM23/OPSM4 on an already provisioned
*   dsi stream for the FFAIL paths.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0)
*
*   defect_bit              - unsigned,one bit value to represent the defect to set/reset:
*                             LINE_OTN_D_LOS_P,
*                             LINE_OTN_D_LOL_P,
*                             LINE_OTN_D_LOFLANE,
*                             LINE_OTN_D_AIS,
*                             LINE_OTN_D_LOF
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - requested bit value as was set in the relevant hw register or
*   LINE_OTN_DEFECT_ERROR on failure.
*
* NOTES:
*   1. dsi stream must be provisioned before invoking this function otherwise
*      an error status will be returned.
* 
*******************************************************************************/
PUBLIC UINT32 mld_opsm_ffail_defect_get(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream,
                                        UINT32             defect_bit)
{

    UINT32       defect_reg_value = 0;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
            ) /* configure opsm4 */
        {
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = opsm4_field_RX_FAIL_LANE_LOS_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = sysotn_opsm4_field_RX_FAIL_LANE_LOS_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOL_P:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = opsm4_field_RX_FAIL_LOL_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = sysotn_opsm4_field_RX_FAIL_LOL_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOFLANE:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = sysotn_opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_AIS:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = opsm4_field_RX_FAIL_LANE_OTUK_AIS_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = sysotn_opsm4_field_RX_FAIL_LANE_OTUK_AIS_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOF:
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = opsm4_field_RX_FAIL_OTU4_LOF_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = sysotn_opsm4_field_RX_FAIL_OTU4_LOF_get(NULL,lineotn_handle,0);
                }
                break;

            case LINE_OTN_D_LOM: /* configure only mld - all 12 lanes */
                if(lineotn_handle->var.lineotn_mode)
                {
                    defect_reg_value = otn_mld_field_DLOM_TO_FFAIL_MAPPING_get(NULL,lineotn_handle,0);
                }
                else
                {
                    defect_reg_value = LINE_OTN_DEFECT_ERROR;
                }
                break;

            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }
        }
        else /* opsm23 to be configured */
        {
            UINT8 opsm23_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
            BOOL  OTU3_config  = ( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                                   lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 ? TRUE:FALSE);
            switch(defect_bit)
            {
            case LINE_OTN_D_LOS_P:
                if(OTU3_config)
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_LOS_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_LOS_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOL_P:
                if(OTU3_config)
                    defect_reg_value = opsm23_field_RX_FAIL_LOL_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value = opsm23_field_RX_FAIL_LOL_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOFLANE:
                if(OTU3_config)
                    defect_reg_value =  opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_AIS:
                if(OTU3_config)
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_OTUK_AIS_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - configure one lane --> dsi_stream modulo 4*/
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_OTUK_AIS_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOF:
                if(OTU3_config)
                    defect_reg_value = opsm23_field_RX_FAIL_OTU3_LOF_get(NULL,lineotn_handle,opsm23_idx,0);
                else /* 10G - alias it to LOFLANE as the H/W uses the lane based LOF for 10G */
                    defect_reg_value = opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get(NULL,lineotn_handle,opsm23_idx,(dsi_stream%4));
                break;

            case LINE_OTN_D_LOM: /* configure only mld */
                if(OTU3_config)
                    defect_reg_value = otn_mld_field_DLOM_TO_FFAIL_MAPPING_get(NULL,lineotn_handle,0);
                else
                    defect_reg_value = otn_mld_field_DLOM_TO_FFAIL_MAPPING_get(NULL,lineotn_handle,dsi_stream);
                break;

            default:
                defect_reg_value = LINE_OTN_DEFECT_ERROR;
                break;
            }

        }
    }

    PMC_RETURN(defect_reg_value);
}/* mld_opsm_ffail_defect_get */


/*******************************************************************************
* mld_opsm_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables the system loopback on OPSM23/OPSM4.
*   In the case of the TX direction, the TX system input on the OPSM4 is sent
*   to the RX system output. Note: this function supports the system loopback only
*   (RX or TX)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   opsm                    - enum:
*                             MLD_OPSM23_0,
*                             MLD_OPSM23_1,
*                             MLD_OPSM23_2,
*                             MLD_OPSM4
*
*   dsi_stream              - unsigned,relevant only for opsm23 for 10G configurations
*                             (0..11)
*
*   frame_rate              - enum, OTU frame rate provisioned for this DSI stream:
*                             (LINE_OTN_RATE_OTU1E ... LINE_OTN_RATE_OTU4). 
*
*   rx_tx_dir               - enum, Rx or Tx direction
*
*   loopback_enable         - boolean, TRUE: enable the loopback;
*                                      FALSE: disable loopback
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   When the TX SYS Loopback is activiated, the propagation of the various
*   lane or OTU defects to SSF is disabled.  When the TX SYS Loopback is
*   de-activated, the register fields (eg. RX_SSF_LANE_LOS) that enable the
*   defect contribution to SSF are set back to their default values. This avoids
*   having to save the context, which is an acceptable restriction given that this
*   feature is for diagnostic purposes.
*
*******************************************************************************/
PUBLIC PMC_ERROR mld_opsm_loopback_set(lineotn_handle_t   *lineotn_handle,
                                       mld_hw_module_t    opsm,
                                       UINT8              dsi_stream,
                                       lineotn_frame_rate_t frame_rate,
                                       lineotn_dir_t      rx_tx_dir,
                                       BOOL               loopback_enable)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    UINT32       set_loopback = (loopback_enable?1:0);
    UINT32       enable_sfail = (loopback_enable?0:1);
    INT32 loopback_lane=0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(rx_tx_dir < LAST_LINE_OTN_DIR ,LINEOTN_ERR_INVALID_PARAMETERS,rx_tx_dir,0);
    PMC_ASSERT(opsm <= MLD_OPSM4  && opsm >= MLD_OPSM23_0,LINEOTN_ERR_INVALID_PARAMETERS,opsm,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(frame_rate<LAST_LINE_OTN_FRAME_RATE,LINEOTN_ERR_INVALID_PARAMETERS,frame_rate,0);

    if(MLD_OPSM4 == opsm) /* configure opsm4 */
    {
        if( LINE_OTN_RX_DIR == rx_tx_dir )
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                opsm4_field_RX_SYS_LOOPBACK_set(NULL, lineotn_handle, set_loopback);
            }
            else
            {
                sysotn_opsm4_field_RX_SYS_LOOPBACK_set(NULL, lineotn_handle, set_loopback);
            }
        }
        else
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                opsm4_field_TX_SYS_LOOPBACK_set(NULL, lineotn_handle, set_loopback);
                /* Enable or disable the SFAIL output depending on whether the loopback
                   is enabled. See note above for explanation */
                opsm4_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle, 0, enable_sfail);
                opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
                opsm4_field_RX_SSF_LOL_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_SSF_OTU4_LOF_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_FAIL_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
                opsm4_field_RX_FAIL_LOL_set(NULL,lineotn_handle,0, enable_sfail);
                opsm4_field_RX_FAIL_OTU4_LOF_set(NULL,lineotn_handle,0, enable_sfail);
            }
            else
            {
                sysotn_opsm4_field_TX_SYS_LOOPBACK_set(NULL, lineotn_handle, set_loopback);
                /* Enable or disable the SFAIL output depending on whether the loopback
                   is enabled. See note above for explanation */
                sysotn_opsm4_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle, 0, enable_sfail);
                sysotn_opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
                sysotn_opsm4_field_RX_SSF_LOL_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_SSF_OTU4_LOF_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_FAIL_LANE_OTL_AIS_set(NULL,lineotn_handle,0, 0);
                sysotn_opsm4_field_RX_FAIL_LOL_set(NULL,lineotn_handle,0, enable_sfail);
                sysotn_opsm4_field_RX_FAIL_OTU4_LOF_set(NULL,lineotn_handle,0, enable_sfail);
            }
        }
    }
    else /* opsm23 to be configured */
    {
        UINT8 opsm23_idx =(UINT8)(opsm - MLD_OPSM23_0); /* set the opsm idx to configure (0,1,2) */
        UINT8 dsi_idx = (dsi_stream%4);

        if( LINE_OTN_RX_DIR == rx_tx_dir )
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G loopback config - set only bit 3 of the relevant opsm */
            {
                opsm23_field_range_RX_SYS_LOOPBACK_set(NULL, lineotn_handle,opsm23_idx,3,3, set_loopback);
            }
            else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G loopback config - set only bit based on dsi stream  */
            {
                opsm23_field_range_RX_SYS_LOOPBACK_set(NULL, lineotn_handle,opsm23_idx,dsi_idx,dsi_idx, set_loopback);
            }
            else
            {
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }

        }
        else
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G loopback config - set only bit 3 of the relevant opsm */
            {
                opsm23_field_range_TX_SYS_LOOPBACK_set(NULL, lineotn_handle,opsm23_idx,3,3,set_loopback);
                loopback_lane = 3; /* for clearing the rx lane defects - see below */
            }
            else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G loopback config - set only bit based on dsi stream  */
            {
                opsm23_field_range_TX_SYS_LOOPBACK_set(NULL, lineotn_handle,opsm23_idx,dsi_idx,dsi_idx,set_loopback);
                loopback_lane = dsi_idx % 4; /* for clearing the rx lane defects - see below */
            }
            else
            {
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            if (rc == PMC_SUCCESS)
            {
                /* Enable or disable the SFAIL output depending on whether the loopback
                   is enabled. See note above for explanation */
                opsm23_field_RX_SSF_LANE_LOS_set(NULL,lineotn_handle, opsm23_idx,loopback_lane, enable_sfail);
                opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_SSF_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_SSF_LOL_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_SSF_OTU3_LOF_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_FAIL_LOL_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                opsm23_field_RX_FAIL_OTU3_LOF_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, enable_sfail);
                /* OTL-AIS can be falsely triggered under high bit error rates */
                opsm23_field_RX_SSF_LANE_OTL_AIS_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, 0);
                opsm23_field_RX_FAIL_LANE_OTL_AIS_set(NULL,lineotn_handle,opsm23_idx, loopback_lane, 0);
            }
        }
    }

    PMC_RETURN(rc);
}/* mld_opsm_loopback_set */


/*******************************************************************************
* mld_opsm_loopback_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns whether a system loopback on OPSM23/OPSM4 is enabled.
*   Note: this function supports the system loopback only (RX or TX)
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   opsm                    - enum:
*                             MLD_OPSM23_0,
*                             MLD_OPSM23_1,
*                             MLD_OPSM23_2,
*                             MLD_OPSM4
*
*   dsi_stream              - unsigned,relevant only for opsm23 for 10G configurations
*                             (0..11)
*
*   frame_rate              - enum, OTU frame rate provisioned for this DSI stream:
*                             (LINE_OTN_RATE_OTU1E ... LINE_OTN_RATE_OTU4). 
*
*   rx_tx_dir               - enum, Rx or Tx direction
*
* OUTPUTS:
*   *loopback_enable         - boolean, TRUE: enable the loopback;
*                                       FALSE: disable loopback

*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   When the TX SYS Loopback is activiated, the propagation of the various
*   lane or OTU defects to SSF is disabled.  When the TX SYS Loopback is
*   de-activated, the register fields (eg. RX_SSF_LANE_LOS) that enable the
*   defect contribution to SSF are set back to their default values. This avoids
*   having to save the context, which is an acceptable restriction given that this
*   feature is for diagnostic purposes.
*
*******************************************************************************/
PUBLIC PMC_ERROR mld_opsm_loopback_get(lineotn_handle_t   *lineotn_handle,
                                       mld_hw_module_t    opsm,
                                       UINT8              dsi_stream,
                                       lineotn_frame_rate_t frame_rate,
                                       lineotn_dir_t      rx_tx_dir,
                                       BOOL               *loopback_enable)
{

    PMC_ERROR   rc = PMC_SUCCESS;
    UINT32 lb_en = 0;
    *loopback_enable = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(rx_tx_dir < LAST_LINE_OTN_DIR ,LINEOTN_ERR_INVALID_PARAMETERS,rx_tx_dir,0);
    PMC_ASSERT(opsm <= MLD_OPSM4  && opsm >= MLD_OPSM23_0,LINEOTN_ERR_INVALID_PARAMETERS,opsm,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(frame_rate<LAST_LINE_OTN_FRAME_RATE,LINEOTN_ERR_INVALID_PARAMETERS,frame_rate,0);

    if(MLD_OPSM4 == opsm) /* configure opsm4 */
    {
        if( LINE_OTN_RX_DIR == rx_tx_dir )
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                lb_en = opsm4_field_RX_SYS_LOOPBACK_get(NULL, lineotn_handle);
            }
            else
            {
                lb_en = sysotn_opsm4_field_RX_SYS_LOOPBACK_get(NULL, lineotn_handle);
            }
        }
        else
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                lb_en = opsm4_field_TX_SYS_LOOPBACK_get(NULL, lineotn_handle);
            }
            else
            {
                lb_en = sysotn_opsm4_field_TX_SYS_LOOPBACK_get(NULL, lineotn_handle);
            }
        }
    }
    else /* opsm23 to be configured */
    {
        UINT8 opsm23_idx =(UINT8)(opsm - MLD_OPSM23_0); /* set the opsm idx to configure (0,1,2) */
        UINT8 dsi_idx = (dsi_stream%4);

        if( LINE_OTN_RX_DIR == rx_tx_dir )
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G loopback config - set only bit 3 of the relevant opsm */
            {
                lb_en = opsm23_field_range_RX_SYS_LOOPBACK_get(NULL, lineotn_handle,opsm23_idx,3,3);
            }
            else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G loopback config - set only bit based on dsi stream  */
            {
                lb_en = opsm23_field_range_RX_SYS_LOOPBACK_get(NULL, lineotn_handle,opsm23_idx,dsi_idx,dsi_idx);
            }
            else
            {
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
        }
        else
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* this is a 40G loopback config - set only bit 3 of the relevant opsm */
            {
                lb_en = opsm23_field_range_TX_SYS_LOOPBACK_get(NULL, lineotn_handle,opsm23_idx,3,3);
            }
            else if(frame_rate >= LINE_OTN_RATE_OTU2F && frame_rate <= LINE_OTN_RATE_OTU1E) /* this is a 10G loopback config - set only bit based on dsi stream  */
            {
                lb_en = opsm23_field_range_TX_SYS_LOOPBACK_get(NULL, lineotn_handle,opsm23_idx,dsi_idx,dsi_idx);
            }
            else
            {
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
        }
    }
    if(lb_en > 0)
    {
        *loopback_enable = TRUE;
    }

    PMC_RETURN(rc);
}/* mld_opsm_loopback_get */


/*******************************************************************************
* mld_bergen_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables a bergen block and its bit error insertion
*   per user requested error rate and error mode per single dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure (0..11) \n
*
*   error_rate - The bit error density. You can specify a value between 0 and
*                1.0. This field has a precision of 24 bits.
*
*   ringosc_enable   - Enables or disables the bergen Ring Oscillator. The 32
*                      bit Bit Error Rate Generator (BERGEN32) blocks have the
*                      ability to use clocks created by free running ring 
*                      oscillators to create asynchronous events that better 
*                      randomize the distribution of bit errors inserted in 
*                      the transmit line data.
*
*   bergen_mode              - enum, error insertion mode: \n
*                             LINE_OTN_BERGEN_NORMAL : Inverts the input bit in order to
*                                                      insert an erroneous bit.\n
*                             LINE_OTN_BERGEN_0_INSERTION : Inverts the input bit only
*                                                           if the input bit is 1
*                                                           in order to insert an erroneous 0.\n
*                             LINE_OTN_BERGEN_1_INSERTION : Inverts the input bit only
*                                                           if the input bit is 0
*                                                           in order to insert an erroneous 1.
*
*   bergen_enable           - boolean, TRUE: enable the bergen;
*                                      FALSE: disable bergen 
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mld_bergen_cfg_set(lineotn_handle_t            *lineotn_handle,
                                    UINT8                       dsi_stream,
                                    FLOAT                       error_rate,
                                    BOOL8                       ringosc_enable,
                                    lineotn_bergen_err_mode     bergen_mode,
                                    BOOL8                        bergen_enable)
{

    UINT32 normalized_err_rate = 0;
    UINT32 itr = 0;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(bergen_mode < LAST_LINE_OTN_BERGEN_ERROR_MODE,LINEOTN_ERR_INVALID_PARAMETERS,bergen_mode,0);

    if (error_rate < 0.0 || error_rate > 1.0)
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == rc)
    {
        /* do not use error limit mecahnism */
        otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set(NULL,lineotn_handle,dsi_stream,0);

        if(TRUE == bergen_enable)
        {
            normalized_err_rate = (UINT32) (pow(2,24)-1.0) * error_rate;  /* 0x1000000 --> 2^24 is the bit range from RDA */

            /* set bergen out of bypass mode */
            otn_mld_field_BERGEN_32_BYPASS_set(NULL,lineotn_handle,dsi_stream,0);

            for (itr= 0;itr<3;itr++)
            {
                /* setup bergen clock */
                otn_mld_field_RINGOSC_32_ENABLE_set(NULL,lineotn_handle, dsi_stream*3+itr,ringosc_enable);
                otn_mld_field_RINGOSC_32_BYPASS_set(NULL,lineotn_handle,dsi_stream*3+itr,
                                                    ringosc_enable ==TRUE?FALSE:TRUE);
            }

            /* set requested error rate */
            otn_mld_field_BERGEN32_BER_IN_set(NULL,lineotn_handle,dsi_stream,normalized_err_rate);

            /* Note: Word Start/End is not supported */

            /* inject errors on all 32 bus bits  lanes*/
            otn_mld_field_BERGEN32_ENABLE_BITLANE_set(NULL,lineotn_handle,dsi_stream,0xffffffff);

            /* set the error mode */
            otn_mld_field_BERGEN32_ERR_MODE_set(NULL,lineotn_handle,dsi_stream,(UINT32)bergen_mode);

            /* enable the bergen */
            otn_mld_field_BERGEN32_ENABLE_set(NULL,lineotn_handle,dsi_stream,1);
        }
        else
        {
            /* set bergen to be in bypass mode */
            otn_mld_field_BERGEN_32_BYPASS_set(NULL,lineotn_handle,dsi_stream,1);

            /* set error rate to 0 */
            otn_mld_field_BERGEN32_BER_IN_set(NULL,lineotn_handle,dsi_stream,0);

            /* do not inject errors on 32 bus bits  lanes*/
            otn_mld_field_BERGEN32_ENABLE_BITLANE_set(NULL,lineotn_handle,dsi_stream,0x00000000);

            for (itr= 0;itr<3;itr++)
            {
                /* clear clock configuration */
                otn_mld_field_RINGOSC_32_ENABLE_set(NULL,lineotn_handle,dsi_stream*3+itr,FALSE);
                otn_mld_field_RINGOSC_32_BYPASS_set(NULL,lineotn_handle,dsi_stream*3+itr,FALSE);
            }
            /* disable the bergen */
            otn_mld_field_BERGEN32_ENABLE_set(NULL,lineotn_handle,dsi_stream,0);
        }
    }

    if (PMC_SUCCESS == rc)
    {
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].bergen_enabled = bergen_enable;
    }

    PMC_RETURN(rc);
}/* mld_bergen_cfg_set */

/*******************************************************************************
* mld_bergen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function the bergen configuration.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G (0) 
*
* OUTPUTS:
*   *error_rate - The bit error density. You can specify a value between 0 and
*                1.0. This field has a precision of 24 bits. 
*                Only valid when enable is TRUE.
*
*   *ringosc_enable   - Enables or disables the bergen Ring Oscillator. The 32
*                      bit Bit Error Rate Generator (BERGEN32) blocks have the
*                      ability to use clocks created by free running ring 
*                      oscillators to create asynchronous events that better 
*                      randomize the distribution of bit errors inserted in 
*                      the transmit line data. *Only valid when enable is TRUE.
*
*   *bergen_mode   - enum, error insertion mode:\n
*                   LINE_OTN_BERGEN_NORMAL : Inverts the input bit in order to
*                                            insert an erroneous bit.\n
*                   LINE_OTN_BERGEN_0_INSERTION : Inverts the input bit only
*                                                 if the input bit is 1
*                                                 in order to insert an erroneous 0.\n
*                   LINE_OTN_BERGEN_1_INSERTION : Inverts the input bit only
*                                                 if the input bit is 0
*                                                 in order to insert an erroneous 1.\n
*                    *Only valid when enable is TRUE
* 
*   *bergen_enable         - boolean, TRUE  --BERGEN is enabled;
*                                     FALSE -- BERGEN is disabled
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mld_bergen_cfg_get(lineotn_handle_t            *lineotn_handle,
                               UINT8                       dsi_stream,
                               FLOAT                       *error_rate,
                               BOOL8                       *ringosc_enable,
                               lineotn_bergen_err_mode     *bergen_mode,
                               BOOL8                        *bergen_enable)
{
    UINT32 error_rate_N;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    *bergen_enable = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].bergen_enabled;

    if (TRUE == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].bergen_enabled)
    {
        error_rate_N = otn_mld_field_BERGEN32_BER_IN_get(NULL,lineotn_handle,dsi_stream);
        *error_rate  = (DOUBLE) error_rate_N / (pow(2,24)-1.0);
        *ringosc_enable = (BOOL)  otn_mld_field_RINGOSC_32_ENABLE_get(NULL,lineotn_handle,dsi_stream*3);
        *bergen_mode = (lineotn_bergen_err_mode) otn_mld_field_BERGEN32_ERR_MODE_get(NULL,lineotn_handle,dsi_stream);
    }

    PMC_RETURN() ;
}/* mld_bergen_cfg_get */


/*******************************************************************************
* mld_otl_logical_lane_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns RX logical lane identifier from the lane aggregator
*   (OPSM) associated to the specified stream or the physical lane associated
*   with the logical lane
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - unsigned, dsi stream to configure:\n
*                             for 10G configurations (0..11);\n
*                             for 40G configurations (0,4,8);\n
*                             for 100G (0)
*  get_phy_lane             - If true, returns the physical lane associated with
*                             the logical lane; if false, returns the logical lane
*
* OUTPUTS:
*  logical_lane_id         - an array of logical/phy lane identifier associated to the dsi stream:\n
*                             for 10G  : item 0 is valid;\n
*                             for 40G  : item 0 to 3 are valids;\n
*                             for 100G : item 0 to 19 are valids
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mld_otl_logical_lane_id_get(lineotn_handle_t  *lineotn_handle,
                                        UINT8  dsi_stream,
                                        UINT32 logical_lane_id[LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE],
                                        BOOL   get_phy_lane)
{
    UINT8 itr = 0;
    UINT8 opsm23_inst_idx = 0;
    UINT8 llid = 0;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned == TRUE,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

    if(LINE_OTN_RATE_OTU4==lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
        ) /* get data from opsm4 */
    {
        for (itr = 0;itr < LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE;itr++)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                llid = opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get(NULL,lineotn_handle,itr);
                if (TRUE == get_phy_lane)
                {
                    logical_lane_id[llid % LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE] = itr/2;
                }
                else
                {
                    logical_lane_id[itr] = llid;
                }
            }
        }
    }
    else /* opsm23 to be configured */
    {
        opsm23_inst_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
        if (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 )
        {
            for (itr = 0;itr < LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE;itr++)
            {
                llid = opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get(NULL,lineotn_handle, opsm23_inst_idx,itr);
                if (TRUE == get_phy_lane)
                {
                    logical_lane_id[llid % LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE] = itr + opsm23_inst_idx*4;
                }
                else
                {
                    logical_lane_id[itr] = llid;
                }
            }

        }
        else
        {
            logical_lane_id[0] = 0;
            if (TRUE == get_phy_lane)
            {
                logical_lane_id[0] = dsi_stream;
            }
        }
    }

    PMC_RETURN();
}/* mld_otl_logical_lane_id_get */


/*******************************************************************************
*  lineotn_mld_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable LINEOTN MLD specified dsi streams.
*
* INPUTS:
*   lineotn_handle          - handle to the LINEOTN instance to be operated on
*   num_dsi_streams         - link to disable
*   dsi_streams             - indices of dsi streams to disable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - when operation is successful otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_mld_reg_cleanup(lineotn_handle_t    *lineotn_handle,
                                    UINT32               num_dsi_streams,
                                    UINT32              *dsi_streams)
{
    UINT32 itr;
    UINT32 jtr;
    UINT32 dsi_stream;

    PMC_ENTRY();

    for (itr = 0; itr < num_dsi_streams; itr++)
    {
        dsi_stream = dsi_streams[itr];

        /* clear up SFIS registers*/
        mld_sfis_n_reg_cleanup(lineotn_handle, dsi_stream);

        /* clear up SFIS51 register*/
        if(TRUE == lineotn_handle->var.lineotn_mode)
        {
            mld_sfi51_reg_cleanup(lineotn_handle, dsi_stream);
        }

        /*disabe potential 100G or 40G stream */
        switch(dsi_stream)
        {
        case 0:
            /*disable 100G */
            opsm4_field_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle, 1);/* data scramble in opsm */
            mld_otl_100G_cleanup(lineotn_handle);
            if(TRUE == lineotn_handle->var.lineotn_mode)
            {
                mld_rcp_defect_cfg(lineotn_handle, dsi_stream,LINE_OTN_RATE_OTU3,FALSE);
            }
            break;
        case 4:
            mld_rcp_defect_cfg(lineotn_handle, dsi_stream,LINE_OTN_RATE_OTU3,FALSE);
            break;
        case 8:
            mld_rcp_defect_cfg(lineotn_handle, dsi_stream,LINE_OTN_RATE_OTU3,FALSE);
            break;
        default:
            break;
        }

        if ((TRUE == lineotn_handle->var.lineotn_mode))
        {
           if (0 == (dsi_stream % 4))
           {
               /* check that all stream of the slice are unprovisioned */
               for (jtr = 0; jtr < 4; jtr++)
               {
                   if (TRUE == lineotn_handle->var.lineotn_dsi_dist[((dsi_stream / 4) * 4) + jtr].dsi_provisioned)
                   {
                       break;
                }
               }
               if (4 <= jtr)
               {
                   mld_opsm23_otl_deprov(lineotn_handle, TRUE, dsi_stream);
               }
           }
           
           /* clean up possible 10G stream */
           mld_otl_deprov(lineotn_handle, MLD_OTU2, dsi_stream);
           

           /*disable scrambling*/
           opsm23_field_range_TX_SCRAMBLE_ENABLE_set(NULL,lineotn_handle, dsi_stream / 4, dsi_stream%4, dsi_stream%4, 1);/* data scramble in opsm */

           /* disable bergen */
           otn_mld_field_BERGEN32_ENABLE_set(NULL, lineotn_handle, dsi_stream, 0);
        }
    }

    PMC_RETURN();
}/*lineotn_mld_reg_cleanup*/

/*******************************************************************************
* lineotn_mld_sfis_odd_parity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures SFIS bits of odd parity.
*
* INPUTS:
*   lineotn_handle     - pointer to lineotn handle instance.
*   dsi_stream         - dsi stream to configure.
*   dir                - direction concerned by the configuration: RX, TX or both.
*   inv                - when TRUE all bits of odd parity are inverted
*                        when FALSE all bits of odd parity are processed normally
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mld_sfis_odd_parity_set(lineotn_handle_t            *lineotn_handle,
                                                 UINT8                       dsi_stream,
                                                 util_global_direction_t     dir,
                                                 BOOL8                       inv)
{
    BOOL8 up_rx_odd = FALSE;
    BOOL8 up_tx_odd = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice;
    const UINT32 reg_val =  TRUE ==  inv ? 1 : 0;
   
    PMC_ENTRY();

    /* Determine which interface is concerned by the configuration: RX or TX, or both */
    switch(dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        up_rx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_TX:
        up_tx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        up_rx_odd = TRUE;
        up_tx_odd = TRUE;
        break;
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    /* retrieve SFIS slice id */
    if (PMC_SUCCESS == rc)
    {
        rc = lineotn_mld_sfis_slice_get(lineotn_handle, dsi_stream, &slice);
    }

    /* Configure registers */
    if (PMC_SUCCESS == rc)
    {        
        if (TRUE == up_rx_odd)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle, slice, 1);
                sfis_rx_field_RX_ODD_INV_set(NULL,lineotn_handle, slice, reg_val);
                sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle, slice, 0);
            }
            else
            {
                sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle, 1); 
                sysotn_sfis_rx_field_RX_ODD_INV_set(NULL,lineotn_handle, reg_val);        
                sysotn_sfis_rx_field_RX_SW_RST_set(NULL,lineotn_handle, 0);         
            }
        }
        
        if (TRUE == up_tx_odd)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle, slice, 1);
                sfis_tx_field_TX_ODD_INV_set(NULL,lineotn_handle, slice, reg_val);
                sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle, slice, 0);
            }
            else
            {
                sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle, 1); 
                sysotn_sfis_tx_field_TX_ODD_INV_set(NULL,lineotn_handle, reg_val);        
                sysotn_sfis_tx_field_TX_SW_RST_set(NULL,lineotn_handle, 0);         
            }
        }
    }

    PMC_RETURN(rc);
}/* lineotn_mld_sfis_odd_parity_set */

/*******************************************************************************
* lineotn_mld_sfis_odd_parity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns status of SFIS bits of odd parity.
*
* INPUTS:
*   lineotn_handle     - pointer to lineotn handle instance.
*   dsi_stream         - dsi stream to configure
*   dir                - direction concerned by the configuration: RX, TX only.
*
* OUTPUTS:
*   *inv_ptr           - when TRUE all bits of odd parity are inverted
*                        when FALSE all bits of odd parity are processed normally
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mld_sfis_odd_parity_get(lineotn_handle_t           *lineotn_handle,
                                                 UINT8                       dsi_stream,
                                                 util_global_direction_t     dir,
                                                 BOOL8                      *inv_ptr)
{
    BOOL8 rd_rx_odd = FALSE;
    BOOL8 rd_tx_odd = FALSE;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 reg_val = 0;
    UINT8 slice;
   
    PMC_ENTRY();
    
    /* Determine which interface is concerned by the configuration: RX or TX, or both */
    switch(dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        rd_rx_odd = TRUE;
        break;
    case UTIL_GLOBAL_DIR_TX:
        rd_tx_odd = TRUE;
        break;
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    /* retrieve SFIS slice id */
    if (PMC_SUCCESS == rc)
    {
        rc = lineotn_mld_sfis_slice_get(lineotn_handle, dsi_stream, &slice);
    }

    /* read registers */
    if (PMC_SUCCESS == rc)
    {        
        if (TRUE == rd_rx_odd)
        {
            /* access LINEOTN or SYSOTN registers */
            if(lineotn_handle->var.lineotn_mode)
            {
                reg_val = sfis_rx_field_RX_ODD_INV_get(NULL,lineotn_handle, slice);
            }
            else
            {
                reg_val = sysotn_sfis_rx_field_RX_ODD_INV_get(NULL,lineotn_handle);
            }
        }
        
        if (TRUE == rd_tx_odd)
        {
            /* access LINEOTN or SYSOTN registers */
            if(lineotn_handle->var.lineotn_mode)
            {
                reg_val = sfis_tx_field_TX_ODD_INV_get(NULL,lineotn_handle, slice);
            }
            else
            {
                reg_val = sysotn_sfis_tx_field_TX_ODD_INV_get(NULL,lineotn_handle); 
            }
        }
    }

    if (PMC_SUCCESS == rc)
    {        
        *inv_ptr = (reg_val == 0) ? FALSE : TRUE;
    }

    PMC_RETURN(rc);
}/* lineotn_mld_sfis_odd_parity_get */

/*******************************************************************************
* lineotn_mld_dlom_to_ffail_mapping_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set dLOM to ffail mapping bit.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - unsigned, dsi stream to configure:
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            - for 10G configurations valid values are: (0..11) 
*                            - for 40G configurations valid values are: (3,7,11)
*                            - for 100G configurations valid values are: (11) 
*   enable                  - whether enable or disable dLOM to ffail mapping bit
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mld_dlom_to_ffail_mapping_set(lineotn_handle_t    *lineotn_handle,
                                                       UINT32 dsi_stream,
                                                       BOOL8 enable)
{
    lineotn_frame_rate_t frame_rate = LAST_LINE_OTN_FRAME_RATE; 
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle != NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams, 
               LINEOTN_ERR_INVALID_PARAMETERS, dsi_stream, 0);

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
      frame_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;
      PMC_ASSERT(frame_rate < LAST_LINE_OTN_FRAME_RATE,
               LINEOTN_ERR_INVALID_PARAMETERS, frame_rate, 0);
      switch (lineotn_stream_rate_get(frame_rate))
        {
           case LINE_OTN_RATE_100G:
            if (dsi_stream != 11)
            {
              rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;
           case LINE_OTN_RATE_40G:
            if(dsi_stream != 3 && dsi_stream != 7 && dsi_stream != 11)
            {
              rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;
           case LINE_OTN_RATE_10G:
            break;
           default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }
    
      if (PMC_SUCCESS == rc) {
          otn_mld_field_DLOM_TO_FFAIL_MAPPING_set(NULL,lineotn_handle,dsi_stream,enable);
      }

    } 
    else 
    {
          sysotn_field_FEC_FAIL_DLOM_ENABLE_set(NULL,lineotn_handle,enable);

    }
    
    PMC_RETURN(rc);
}


/*******************************************************************************
* lineotn_mld_sfis_slice_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns SFIS slice identifier associated to dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to configure.
*
* OUTPUTS:
*   *slice_id_ptr           - SFIS slice id associated to given  dsi stream
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_mld_sfis_slice_get(lineotn_handle_t  *lineotn_handle,
                                             UINT8              dsi_stream,
                                             UINT8             *slice_id_ptr)
{
    PMC_ERROR rc = LINEOTN_ERR_INVALID_PARAMETERS;

    PMC_ENTRY();

    if(lineotn_handle->var.lineotn_mode)
    {
        switch (lineotn_handle->var.current_lineotn_cfg)
        {            
            /* 100GB, 1x100GB , 1xSFI-S(n=10+1) */
        case LINE_OTN_CFG_1:
            if (0 == dsi_stream &&
                MLD_SFIS == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                *slice_id_ptr = 0;
                rc = PMC_SUCCESS;
            }
            break;
            
            
            /* 110GB , 1x40GB + 7x10GB , 1xSFI-S(n=4+1) + 7xOTU2 */
        case LINE_OTN_CFG_5:
            /* 110GB , 1x40GB + 1x40GB + 3x10GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  */
        case LINE_OTN_CFG_6:
        case LINE_OTN_CFG_9: /* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  */
        case LINE_OTN_CFG_10:/* 120GB , 1x40GB + 1x40GB + 1x40GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1*/
            if (0 == dsi_stream &&
                MLD_SFIS == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                *slice_id_ptr = 2;
                rc = PMC_SUCCESS;
            } 
            break;
            
            /* 100GB , 2x40GB + 2x10GB , 2xSFI-S(n=4+1) + 2xOTU2 */
        case LINE_OTN_CFG_13: 
            /* 120GB , 2x40GB + 1x40GB , 2xSFI-S(n=4+1) + 1xSFI5.1  */
        case LINE_OTN_CFG_14:
            /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xSFI-S(n=4+1)   */
        case LINE_OTN_CFG_17:
            if (0 == dsi_stream &&
                MLD_SFIS == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                *slice_id_ptr = 2;
                rc = PMC_SUCCESS;
            } 
            if (5 == dsi_stream &&
                MLD_SFIS == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                *slice_id_ptr = 3;
                rc = PMC_SUCCESS;
            } 
            
        break;
        default:
            break;
            
        }        
    }
    else
    {  
        /* sysotn */
        if (0 == dsi_stream &&
            MLD_SFIS == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
        {
            *slice_id_ptr = 0;
            rc = PMC_SUCCESS;
        }         
    }

    PMC_RETURN(rc);
}/* lineotn_mld_sfis_slice_get */

/*******************************************************************************
* lineotn_mld_sfis_odd_parity_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function resets SFIS bits of odd parity.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   slice                   - SFIS slice to configure
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
PRIVATE void lineotn_mld_sfis_odd_parity_reset(lineotn_handle_t    *lineotn_handle,
                                               UINT8                slice)
{
    PMC_ENTRY();

    if(lineotn_handle->var.lineotn_mode)
    {
        sfis_rx_field_RX_ODD_INV_set(NULL,lineotn_handle, slice, 0);
        sfis_tx_field_TX_ODD_INV_set(NULL,lineotn_handle, slice, 0);
    }
    else
    {
        sysotn_sfis_rx_field_RX_ODD_INV_set(NULL,lineotn_handle, 0);
        sysotn_sfis_tx_field_TX_ODD_INV_set(NULL,lineotn_handle, 0);
    }
    
    PMC_RETURN();
}

/*
** end of file
*/

