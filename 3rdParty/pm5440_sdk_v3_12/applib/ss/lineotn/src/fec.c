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

#include "mfec_io.h"
#include "fecstat_io.h"
#include "shared_ram_io.h"
#include "swizzle7_40g_io.h"
#include "i4_fec_io.h"
#include "i7_fec_io.h"
#include "gfec_io.h"
#include "lineotn.h"

#include "sysotn_io.h"
#include "sysotn_gfec_io.h"

#include "opsm23_io.h"
#include "otn_mld_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

#define FEC_SLICE(dsi_stream)       (dsi_stream/4) /* need to verify slice allocation - assumption here - 0-3--> 0,4-7-->1,8-11-->2 */


#define LINEOTN_MAX_NUM_40G_SLICES   3

/*
** Local Structures and Unions
*/


/*
** Local Variables
*/


/*
** Forward declarations
*/

PRIVATE PMC_ERROR fec_shared_ram_select_set(lineotn_handle_t   *lineotn_handle,
                                            UINT8              dsi_stream,
                                            lineotn_fec_t      fec_type);

PRIVATE PMC_ERROR fec_shared_ram_pwr_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         lineotn_fec_t      fec_type,
                                         BOOL               low_power);

PRIVATE PMC_ERROR fec_validate_prov(lineotn_handle_t   *lineotn_handle,
                                    UINT8              dsi_stream,
                                    lineotn_fec_t      fec_type);

PRIVATE PMC_ERROR fec_validate_deprov(lineotn_handle_t   *lineotn_handle,
                                      UINT8              dsi_stream,
                                      lineotn_fec_t      fec_type);

PRIVATE BOOL fec_is_slice_prov(lineotn_handle_t   *lineotn_handle,
                               UINT8              dsi_stream,
                               lineotn_fec_t      fec_type);

PRIVATE PMC_ERROR fec_mfec_enc_set(lineotn_handle_t *lineotn_handle,
                                   UINT8            dsi_stream,
                                   lineotn_fec_t    fec_type);

PRIVATE PMC_ERROR fec_mfec_dec_set(lineotn_handle_t *lineotn_handle,
                                   UINT8            dsi_stream,
                                   lineotn_fec_t    fec_type);

PRIVATE PMC_ERROR fec_i4_cfg_set(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_i4_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_swizzle40_cfg_set(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_swizzle40_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                          UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_i7_cfg_set(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_i7_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream);



PRIVATE PMC_ERROR fec_gfec_cfg_set(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_gfec_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                     UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_swizzle100_cfg_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream);

PRIVATE PMC_ERROR fec_swizzle100_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                           UINT8              dsi_stream);

PRIVATE UINT8 fec_gfec_enc_dec_type_get(lineotn_handle_t   *lineotn_handle);

PRIVATE PMC_ERROR fec_set_scrambling_mode(lineotn_handle_t *lineotn_handle,
                                          UINT8            dsi_stream,
                                          BOOL             scarmble_in_opsm);

PRIVATE PMC_ERROR fec_stat_cfg(lineotn_handle_t *lineotn_handle,                               
                               digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr,
                               BOOL8            enable);

PRIVATE void fec_stat_logic_table_cfg(lineotn_handle_t *h,
                                      UINT8 entry_id,
                                      UINT8 num_fec_source,
                                      UINT8 fec_source_addr,
                                      UINT8 llid,
                                      UINT8 phy_id);

PRIVATE void fec_i4_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                UINT8              dsi_stream);

PRIVATE void fec_swizzle40_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                       UINT8              dsi_stream);

PRIVATE void fec_i7_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                UINT8              dsi_stream);



PRIVATE void fec_gfec_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                  UINT8              dsi_stream);

PRIVATE void fec_swizzle100_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream);

PRIVATE void fec_pwr_mode_cleanup(lineotn_handle_t  *lineotn_handle,
                                  UINT8             dsi_stream);

PRIVATE void fec_mfec_cleanup(lineotn_handle_t   *lineotn_handle,
                              UINT8               dsi_stream);

PRIVATE void fec_mfec_sel_cleanup(lineotn_handle_t *lineotn_handle);

PRIVATE void fec_stat_reg_cleanup(lineotn_handle_t        *h);

/*
** Public Functions
*/

/*******************************************************************************
* fec_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring FEC block into configurable state 
*
* INPUTS:
*   lineotn_handle       - handle to the FEC instance to be operated on
*   energy_state         - Energy state to put FEC into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success or a meaningful error code is returned 
*
* NOTES:
*
*******************************************************************************/
PUBLIC void fec_init(lineotn_handle_t     *lineotn_handle,
                     pmc_energy_state_t   energy_state)
{
    UINT8 i;
    lineotn_fec_t fec_type;
    PMC_ENTRY();
  
    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PTR, 0, 0);
    
    switch (energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        fec_blk_init(lineotn_handle);
        break;

    case PMC_ENERGY_STATE_RESET:
        if(lineotn_handle->var.lineotn_mode)
        {
            for(fec_type=LINE_OTN_FEC_NONE;fec_type<LAST_LINE_OTN_FEC;fec_type = (lineotn_fec_t)((UINT32)fec_type + 1))
            {   
                for(i=0;i<3;i++) /* for each FEC slice */
                {                
                    fec_pwr_mode_config_set( lineotn_handle,i * 4,fec_type,TRUE);/* all possible fec combinations to low power */
                    if ((i == 0) && (fec_type == LINE_OTN_FEC_SWIZZLE_100G || fec_type == LINE_OTN_FEC_G709))
                    {
                        /* Only need to configure one instance of Swizzle 100G & G709 FEC */
                        break;
                    }
                }
            }
        }
        else 
        {
            fec_pwr_mode_config_set( lineotn_handle,0,LINE_OTN_FEC_G709,TRUE);/* Only G709 FEC is valid in SYSOTN mode */            
        }   

        fec_var_init(&lineotn_handle->var);   /* reset context variables */
        break;
        
    default:
        PMC_ASSERT(FALSE, LINEOTN_ERR_INVALID_PARAMETERS, energy_state, 0);
        break;
    } /* end switch */

    PMC_RETURN();
} /* fec_init */


/*******************************************************************************
* fec_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   FEC instance.  It will test the resource state for all resources within
*   the FEC instance if any are not in LINE_OTN_FEC_NONE with power set to 
*   LINE_OTN_PWR_OPERATIONAL (start state equivalent), 
*   state then FALSE is returned, else TRUE. \n
* 
*   Should TRUE be returned it can be assumed that no resources are
*   provisioned against the FEC and it would be possible for the higher
*   layer module to put the module in lowpower standby mode of operation.
*
*
* INPUTS:
*   lineotn_handle        - handle to the FEC instance to be operated on
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
PUBLIC BOOL fec_start_state_test(lineotn_handle_t *lineotn_handle) 
{
    UINT32 dsi_stream = 0;
    BOOL   rc = TRUE;  /* no resources provisioned */

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL, LINEOTN_ERR_INVALID_PTR, 0, 0);

    for (dsi_stream = 0; dsi_stream < lineotn_handle->cfg.max_dsi_streams ; dsi_stream++) 
    {
        if(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type != LAST_LINE_OTN_FEC || 
           lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ) /* when dsi stream is provisioned we want to keep fec in bypass mode */
        {
            rc = FALSE; /* found at least one resource that is allocated ! */    
            break;
        }
    }

    if (TRUE == rc)
    {
        /* check if the FEC STAT is initialized */
        if (TRUE == lineotn_handle->var.fec_stat_cfg.enable) 
        {
            rc = FALSE;
        }
    }
       
    PMC_RETURN( rc );

} /* fec_start_state_test */


/*******************************************************************************
* fec_var_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the block's logical variables.
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
PUBLIC void fec_var_init(lineotn_var_t *var_ptr)
{
    UINT8 i;
    PMC_ENTRY();

    for(i = 0; i < LINE_OTN_MAX_DSI_STREAMS; i++)
    {
        var_ptr->lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
        var_ptr->lineotn_dsi_dist[i].dsi_fec_type = LAST_LINE_OTN_FEC;
        var_ptr->lineotn_dsi_dist[i].fec_bypassed = TRUE;
        var_ptr->lineotn_dsi_dist[i].fec_stat_enabled = FALSE;
        var_ptr->lineotn_dsi_dist[i].fec_dec_passthru = TRUE;
    }

    var_ptr->fec_stat_cfg.enable = FALSE;
    for(i = 0; i<LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES; i++)
    {
        var_ptr->fec_stat_cfg.fsl_table[i] = LINEOTN_FEC_STAT_NULL_ENTRY;
    }

    PMC_RETURN();
}


/*******************************************************************************
* fec_blk_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes the FEC block as part of handle_init:\n
*
*   This init takes the following actions:\n
*       1. Take the fec block out of reset\n
*       2. put the fec block to low power until provisioning is done\n
*       3. bypass fec
*
* INPUTS:
*   lineotn_handle            - pointer to LINEOTN handle instance
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
PUBLIC void fec_blk_init(lineotn_handle_t *lineotn_handle)
{
    UINT8 slice = 0;
    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        /* shared ram all slices (0,1,2) out of reset / shutdown */
        for(slice=0;slice<3;slice++)
        {
            shared_ram_field_RESET_set(NULL,lineotn_handle,slice,0);

            shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x3);
            shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
            shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
            shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
        }

        /* mfec glue - out of reset/high power, bypass */
        mfec_field_RESET_set(NULL,lineotn_handle,0);
        mfec_field_MFEC_GLUE_LOWPWR_set(NULL,lineotn_handle,0); /* block level is set to high power */
        mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,0x0fff);
        mfec_field_ENCODER_BYPASS_set(NULL,lineotn_handle,0x0fff);
        
        /* clear mfec enc/dec fec selections */                    
        mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0);
        mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);

        /* fec blocks */
        /* mfec swizzle 100G */
        mfec_field_SWIZZLE100G_RAM_LOWPWR_set(NULL,lineotn_handle,1);

        /* swizzle7 40G - disable/decoder/encoder */
        /* put rams to low power                  */
        /* in low power                           */
        for(slice=0;slice<3;slice++)
        {
            swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,1);
            swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,1);
            swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);
        }

    }
    else
    {
        sysotn_field_BYPASS_GFEC_RX_set(NULL,lineotn_handle,1);
        sysotn_field_BYPASS_GFEC_TX_set(NULL,lineotn_handle,1);
    }

    /* RS FEC (gfec 709) */
    /* all stream decoders disabled, low power               */
    /* ram is in deep sleep      */
    if(lineotn_handle->var.lineotn_mode)
    {
        gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0x0fff);
        gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
        gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
    }
    else
    {
        sysotn_gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0x0fff);
        sysotn_gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
        sysotn_gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
    }

    if(lineotn_handle->var.lineotn_mode)
    {
        /* I4 FEC (40G) */
        /* all stream decoders disabled, low power               */
        /* ram is in low power                                   */
        for(slice=0;slice<3;slice++)
        {
            i4_fec_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,1);
            i4_fec_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);
            i4_fec_field_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,0xf);
        }

        /* I7 FEC (40G) */
        /* all stream decoders disabled, low power               */
        /* ram is in low power                                   */
        for(slice=0;slice<3;slice++)
        {
            i7_fec_field_FEC_SW_LOWPWR_set(NULL,lineotn_handle,slice,1);
            i7_fec_field_FEC_SW_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);
        }



    }

    PMC_RETURN();

} /* fec_blk_init */

/*******************************************************************************
* fec_gfec_lowpwr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets GFEC lowpwr.
*
* INPUTS:
*   lineotn_handle          - pointer to Lineotn handle instance.
*   low_power               - boolean, TRUE: set to low power;
*                                      FALSE: take out of low power
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

PUBLIC void fec_gfec_lowpwr_set( lineotn_handle_t  *lineotn_handle,
                                 BOOL8              low_power)

{
    UINT32 low_power_int = TRUE == low_power ? 0x1 : 0x0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if(lineotn_handle->var.lineotn_mode)
    {
        gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle, low_power_int);
    }
    else
    {
        sysotn_gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle, low_power_int);
    }

    PMC_RETURN();
}/* fec_gfec_lowpwr_set */


/*******************************************************************************
* fec_all_swizzle_fec_lowpwr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets all swizzle fec lowpwr.
*
* INPUTS:
*   lineotn_handle          - pointer to Lineotn handle instance.
*   low_power               - boolean,TRUE: set to low power
*                                    ,FALSE: take out of low power
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
PUBLIC void fec_all_swizzle_fec_lowpwr_set( lineotn_handle_t  *lineotn_handle,
                                            BOOL8              low_power)

{
    UINT32 low_power_int = TRUE == low_power ? 0x1 : 0x0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    mfec_field_SWIZZLE100G_RAM_LOWPWR_set(NULL,lineotn_handle,low_power_int);
    swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,0,low_power_int);
    swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,1,low_power_int);
    swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,2,low_power_int);
    
    PMC_RETURN();
}/* fec_all_swizzle_fec_lowpwr_set */


/*******************************************************************************
* fec_pwr_mode_config_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   sets the fec power mode into one of 2 predefined power modes per requested
*   fec module or the fec block in general.
*
* INPUTS:
*   lineotn_handle          - pointer to Lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
*   low_power               - boolean,TRUE: set to low power
*                                    ,FALSE: take out of low power
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
PUBLIC PMC_ERROR fec_pwr_mode_config_set( lineotn_handle_t  *lineotn_handle,
                                          UINT8             dsi_stream,
                                          lineotn_fec_t     fec_type,
                                          BOOL              low_power)

{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       i;
    UINT8       slice = 0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    
    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        slice = FEC_SLICE(dsi_stream);
        if(low_power)
        {
            i4_fec_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,1);
            i4_fec_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);
        }
        else
        {
            i4_fec_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,0);
            i4_fec_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,0);
        }

        fec_shared_ram_pwr_set(lineotn_handle,dsi_stream,fec_type,low_power);
        break;

    case LINE_OTN_FEC_I7:
        slice = FEC_SLICE(dsi_stream);
        if(low_power)
        {
            i7_fec_field_FEC_SW_LOWPWR_set(NULL,lineotn_handle,slice,1);
            i7_fec_field_FEC_SW_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);

        }
        else
        {
            i7_fec_field_FEC_SW_LOWPWR_set(NULL,lineotn_handle,slice,0);
            i7_fec_field_FEC_SW_RAM_LOWPWR_set(NULL,lineotn_handle,slice,0);
        }

        fec_shared_ram_pwr_set(lineotn_handle,dsi_stream,fec_type,low_power);
        break;

    case LINE_OTN_FEC_G709:
        if(low_power)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,1);
                gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
                gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
            }
            else
            {
                sysotn_gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,1);
                sysotn_gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
                sysotn_gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
            }
        }
        else
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,0);
                gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,0);
                gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,0);
            }
            else
            {
                sysotn_gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,0);
                sysotn_gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,0);
                sysotn_gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,0);

            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        if(low_power)
        {
            mfec_field_SWIZZLE100G_RAM_LOWPWR_set(NULL,lineotn_handle,1);
            for(i=0;i<3;i++) 
            {
                swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,i,1);
                swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,i,1);
            }
        }
        else
        {
            mfec_field_SWIZZLE100G_RAM_LOWPWR_set(NULL,lineotn_handle,0);
            for(i=0;i<3;i++) 
            {
                swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,i,0);
                swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,i,0);
            }
        }
        fec_shared_ram_pwr_set(lineotn_handle,dsi_stream,fec_type,low_power);

        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        slice = FEC_SLICE(dsi_stream);
        if(low_power)
        {
            swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,1);
            swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);

        }
        else
        {
            swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice,0);
            swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice,0);

        }

        fec_shared_ram_pwr_set(lineotn_handle,dsi_stream,fec_type,low_power);
        break;



    case LINE_OTN_FEC_NONE:
        /* we just need to start/stop mfec glue power in case we are in bypass only */
        /* this will be done in the segment bellow                                  */    
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    if( PMC_SUCCESS == rc )
    {
        if(low_power)
        {
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
            for(i=0; i < lineotn_handle->cfg.max_dsi_streams;i++)
                if( LINE_OTN_PWR_OPERATIONAL == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr )
                    break;
            
            if(lineotn_handle->cfg.max_dsi_streams == i && lineotn_handle->var.lineotn_mode) 
            {
                /* no operational FECs found - put MFEC GLUE logic in low power mode 
                   and mark all streams as having un-provisioned FECs (i.e. even no bypass) */
                mfec_field_MFEC_GLUE_LOWPWR_set(NULL,lineotn_handle,1);
                for(i=0; i < lineotn_handle->cfg.max_dsi_streams;i++)
                {
                    lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LAST_LINE_OTN_FEC;
                }
            }
        }
        else  
        {
            /* Determine whether there are any FECs already provisioned; if not, take MFEC_GLUE out of low power mode */
            for(i=0; i < lineotn_handle->cfg.max_dsi_streams;i++)
                if( LINE_OTN_PWR_OPERATIONAL == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr )
                    break;
            if(lineotn_handle->cfg.max_dsi_streams == i && lineotn_handle->var.lineotn_mode)            
            {
                mfec_field_MFEC_GLUE_LOWPWR_set(NULL,lineotn_handle,0);
            }
            
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;

        }
    }
    PMC_RETURN(rc);
} /* fec_pwr_mode_config_set */


/*******************************************************************************
* fec_blk_prov_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision a dsi stream per requested fec configuration. This function
*   will call functions to allocate and set all fec HW modules for a dsi
*   stream per requested configuration.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       LINEOTN_ERR_OTHER_FEC_ALREADY_PROV - Internally generated error code
*       LINEOTN_ERR_STRONG_FEC_MIX_NOT_SUPPORTED
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is allocating resources
*      for all lanes that belong to this slice.
* 
*******************************************************************************/

PUBLIC PMC_ERROR fec_blk_prov_stream     ( lineotn_handle_t   *lineotn_handle,
                                           UINT8              dsi_stream,
                                           lineotn_fec_t      fec_type )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8  dsi_end = dsi_stream;
    UINT8 itr;
    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC ,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);    

    /* if this fec is already set - no need to do anything */
    if( fec_type == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type )
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    if (fec_type != LINE_OTN_FEC_NONE)
    {
        rc = fec_validate_prov(lineotn_handle,dsi_stream,fec_type);
    }

    if (PMC_SUCCESS == rc )
    {
        switch ( fec_type )
        {
        case LINE_OTN_FEC_NONE:
            rc = fec_mfec_cfg_set(lineotn_handle,dsi_stream,fec_type);
            if (PMC_SUCCESS == rc)
            {
                rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, TRUE);
            }
            break;
    
        case LINE_OTN_FEC_I4:
            rc = fec_i4_cfg_set(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_I7:
            rc = fec_i7_cfg_set(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_G709:
            rc = fec_gfec_cfg_set(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_SWIZZLE_100G:
            rc = fec_swizzle100_cfg_set(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_SWIZZLE_40G:
            rc = fec_swizzle40_cfg_set(lineotn_handle,dsi_stream);
            break;



        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

        }
    }

    if ( PMC_SUCCESS == rc)
    {
        switch(lineotn_stream_rate_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
        {
        case LINE_OTN_RATE_40G:
            dsi_end = dsi_stream + LINEOTN_MAX_LANES_PER_OPSM23_SLICE;
            break;
        case LINE_OTN_RATE_100G:
            dsi_end = lineotn_handle->cfg.max_dsi_streams;
            break;            
        default:
            dsi_end = dsi_stream + 1;
            break;
        }

        if (fec_type != LINE_OTN_FEC_NONE)
        {
            /* set the scrambling mode */
            rc = fec_set_scrambling_mode(lineotn_handle,dsi_stream,lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm);
        }

        for (itr = dsi_stream; itr < dsi_end && itr < lineotn_handle->cfg.max_dsi_streams; itr++)
        {
            lineotn_handle->var.lineotn_dsi_dist[itr].fec_dec_passthru = FALSE;
            if(LINE_OTN_FEC_NONE == fec_type)
            {
   
                /* disable scramble after encoder and before decoder */
                lineotn_handle->var.lineotn_dsi_dist[itr].scramble_in_opsm = FALSE;
                fec_set_scrambling_mode(lineotn_handle,
                                        dsi_stream,
                                        FALSE);

            }
        }        
    }
    
    PMC_RETURN(rc);
}/* fec_blk_prov_stream */


/*******************************************************************************
* fec_blk_deprov_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   De-provision a dsi stream per requested fec configuration. This function
*   will call functions to de-allocate and reset all fec HW modules for a dsi
*   stream per requested fec configuration.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
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
PUBLIC PMC_ERROR fec_blk_deprov_stream     ( lineotn_handle_t   *lineotn_handle,
                                             UINT8              dsi_stream )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_NONE;
    UINT32          dsi_end;
    UINT32          itr;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    fec_type = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type;

    /* if this fec is already removed - no need to do anything */
    if( LINE_OTN_FEC_NONE == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type )
    {
        if(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].fec_bypassed )
        {
            /* if bypass is enabled; mfec glue should be on */
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,FALSE);/* high power  mode */
            
            /* set the encoder and decoder to 100G / (40-10G) mode */
            /* need to be set even if no fec is configured when we */
            /* are in bypass mode                                  */
            if(PMC_SUCCESS == rc ) 
            {
                rc = fec_mfec_enc_set(lineotn_handle,dsi_stream,fec_type);
                rc |= fec_mfec_dec_set(lineotn_handle,dsi_stream,fec_type);
            }

        }
        else
        {
            /* if we have a by pass mfec glue should be off */
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */
        }
        PMC_RETURN(rc);

    }

    /* check for de-provisioning validity */
    rc = fec_validate_deprov(lineotn_handle,dsi_stream,fec_type);

    if (PMC_SUCCESS == rc && fec_type != LAST_LINE_OTN_FEC)
    {
        switch ( fec_type )
        {
        case LINE_OTN_FEC_I4:
            rc = fec_i4_cfg_reset(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_I7:
            rc = fec_i7_cfg_reset(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_G709:
            rc = fec_gfec_cfg_reset(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_SWIZZLE_100G:
            rc = fec_swizzle100_cfg_reset(lineotn_handle,dsi_stream);
            break;

        case LINE_OTN_FEC_SWIZZLE_40G:
            rc = fec_swizzle40_cfg_reset(lineotn_handle,dsi_stream);
            break;



        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

        }
    }

    if ( PMC_SUCCESS == rc)
    {
        switch(lineotn_stream_rate_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
        {
        case LINE_OTN_RATE_40G:
            dsi_end = dsi_stream + LINEOTN_MAX_LANES_PER_OPSM23_SLICE;
            break;
        case LINE_OTN_RATE_100G:
            dsi_end = lineotn_handle->cfg.max_dsi_streams;
            break;            
        default:
            dsi_end = dsi_stream + 1;
            break;
        }

        for (itr = dsi_stream; itr < dsi_end && itr < lineotn_handle->cfg.max_dsi_streams; itr += 1)
        {        
            lineotn_handle->var.lineotn_dsi_dist[itr].scramble_in_opsm = FALSE;
        }        
    }
      
    PMC_RETURN(rc);
}/* fec_blk_deprov_stream */


/*******************************************************************************
* fec_dec_passthru_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set the decoder state for a selected fec per dsi stream.
*   Setting the decoder state to disabled will cause traffic to go through the 
*   decoder maintaining the current fec latency, however, no error correction or statistics
*   collection will be performed.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_40G
*
*   dec_passthru           - boolean, TRUE  :fec decoder is disabled
*                                   , FALSE :fec decoder is enabled
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this slice. \n
*   2. LINE_OTN_FEC_SWIZZLE_100G is not supported for this type of operation.\n
*   3. I7 FEC 10G does not support disabling the decoder per 10G channel
*
*******************************************************************************/
PUBLIC  PMC_ERROR fec_dec_passthru_set(lineotn_handle_t     *lineotn_handle,
                                       UINT8                dsi_stream,
                                       lineotn_fec_t        fec_type,
                                       BOOL                 dec_passthru )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate = (lineotn_frame_rate_t)0;
    UINT32 dec_bit = 0;
    UINT8  i=0;
 
    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == fec_type,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type,dsi_stream);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(dec_passthru)
                i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,3,3,1); /* disable error correction for the 40G lanes - set bit 3 */
            else
                i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,3,3,0); /* enable error correction for the 40G lanes - set bit 3 */

            for(i=0;i<4;i++)
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;
    
        }
        else /* 10G fec  */
        {
            dec_bit = (dsi_stream%4);
            if(dec_passthru)
                i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,dec_bit,dec_bit,1);  /* disable error correction for the 10G lane */
            else
                i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,dec_bit,dec_bit,0);  /* enable error correction for the 10G lane */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].fec_dec_passthru=dec_passthru;
        }
        break;

    case LINE_OTN_FEC_I7:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            /* disable/enable error correction for the 40G lanes - set all bits even though only bit 3 is used */
            if(dec_passthru)
                i7_fec_field_DECODER_DISABLE_set(NULL,lineotn_handle,slice, 0xF); 
            else
                i7_fec_field_DECODER_DISABLE_set(NULL,lineotn_handle,slice, 0x0); 

            for(i=0;i<4;i++)
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;
        }
        else /* 10G */
        {
            dec_bit = (dsi_stream%4);
            if(dec_passthru)
            {
                /*  I7FEC does not support disabling the FEC Decoder on a per channel basis */
                rc = LINEOTN_ERR_INVALID_PARAMETERS;        
            }           
            else
            {
                i7_fec_field_range_DECODER_DISABLE_set(NULL,lineotn_handle,slice,dec_bit,dec_bit,0);  /* enable error correction for the 10G lane */
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream].fec_dec_passthru=dec_passthru;
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
           dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(dec_passthru)
                swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,1);  /* disable error correction for the 10G lane */
            else
                swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,0); /* enable error correction for the 10G lane */

            for(i=0;i<4;i++)
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        break;



    case LINE_OTN_FEC_G709:
        if (LINE_OTN_RATE_OTU4 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate) /* 100G */
        {
            if(dec_passthru)
            {
                /*rc = LINEOTN_ERR_INVALID_PARAMETERS; *//* g.798 - MI_FecEn OTU4 - always on */
                if(lineotn_handle->var.lineotn_mode)
                {
                    gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0xfff);
                }
                else
                {
                    sysotn_gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0xfff);
                }
            }
            else
            {
                /* enable decoder for all LINE_OTN_MAX_DSI_STREAMS streams */
                if(lineotn_handle->var.lineotn_mode)
                {
                    gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0);
                }
                else
                {
                    sysotn_gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0);
                }
            }

            if(PMC_SUCCESS==rc)
                for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
                    lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;

        }
        else if(LINE_OTN_RATE_OTU3 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||   /* 40G  */
                LINE_OTN_RATE_OTU3E2 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||
                LINE_OTN_RATE_OTU3E1 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
            /* G709 FEC 10G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* read modify write */
            UINT32 enable_dec_bm = 0x0fff; /* all LINE_OTN_MAX_DSI_STREAMS dsi streams are disabled */

            UINT32 dec_passthru_reg_setting = gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle);
            
            if(dec_passthru)
            {
                enable_dec_bm = 0; /* all LINE_OTN_MAX_DSI_STREAMS dsi streams are enabled */
                /* disable decoder for all 4 dsi streams on this slice */
                /* turn on the bits for this slice */
                enable_dec_bm |= (UINT32)((0x0000000f<<dsi_stream));
                dec_passthru_reg_setting |= enable_dec_bm;
            }
            else
            {    
                /* enable decoder for all 4 dsi streams on this slice */
                /* turn off the bits for this slice */
                enable_dec_bm &= (UINT32)(~(0x0000000f<<dsi_stream));
                dec_passthru_reg_setting &= enable_dec_bm;
            }
                           
            gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,dec_passthru_reg_setting);

            for(i=0;i<4;i++)
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;

        }
        else  /* 10G */
        {
            /* G709 FEC 10G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* read modify write */
            UINT32 enable_dec_bm = 0x0fff; /* all LINE_OTN_MAX_DSI_STREAMS dsi streams are disabled */
            UINT32 dec_passthru_reg_setting = gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle);
            
            if(dec_passthru)
            {
                enable_dec_bm = 0; /* all LINE_OTN_MAX_DSI_STREAMS dsi streams are enabled */
                /* disable decoder for all 4 dsi streams on this slice */
                /* turn on the bits for this slice */
                enable_dec_bm |= (UINT32)((0x00000001<<dsi_stream));
                dec_passthru_reg_setting |= enable_dec_bm;
            }
            else
            {    
                /* enable decoder for all 4 dsi streams on this slice */
                /* turn off the bits for this slice */
                enable_dec_bm &= (UINT32)(~(0x00000001<<dsi_stream));
                dec_passthru_reg_setting &= enable_dec_bm;
            }

            gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,dec_passthru_reg_setting);

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].fec_dec_passthru=dec_passthru;

        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G: 
        if(dsi_rate == LINE_OTN_RATE_OTU4)             
        {
            if(dec_passthru)
            {
                for(i=0; i<3; i++)
                {
                    swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,i,1);
                }
            }  
            else
            {
                for(i=0; i<3; i++)
                {
                    swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,i,0);
                }
            }

            if(PMC_SUCCESS==rc)
                for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
                    lineotn_handle->var.lineotn_dsi_dist[dsi_stream+i].fec_dec_passthru=dec_passthru;
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        break;
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }
    
    PMC_RETURN(rc);
}/* fec_dec_passthru_set */


/*******************************************************************************
* fec_mfec_bypass_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set an already fec provisioned dsi stream into/out of
*   fec bypass mode.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*   bypass_enc_dec          - boolean,TRUE: set bypass mode
*                                    ,FALSE: take out of bypass mode
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
*      for all lanes that belong to this slice.
* 
*******************************************************************************/
PUBLIC PMC_ERROR fec_mfec_bypass_set(lineotn_handle_t   *lineotn_handle,
                                     UINT8              dsi_stream,
                                     BOOL               bypass_enc_dec)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    UINT8 i = 0;
    lineotn_frame_rate_t dsi_rate;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    /* PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0); */

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            if(bypass_enc_dec)
            {
                mfec_field_ENCODER_BYPASS_set(NULL,lineotn_handle,0x00000fff);
                mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,0x00000fff);
            }
            else
            {
                mfec_field_ENCODER_BYPASS_set(NULL,lineotn_handle,0);
                mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,0);
            }
        }
        else
        {
            if(bypass_enc_dec)
            {
                sysotn_field_BYPASS_GFEC_RX_set(NULL, lineotn_handle, 1);
                sysotn_field_BYPASS_GFEC_TX_set(NULL, lineotn_handle, 1);
            }
            else
            {
                sysotn_field_BYPASS_GFEC_RX_set(NULL, lineotn_handle, 0);
                sysotn_field_BYPASS_GFEC_TX_set(NULL, lineotn_handle, 0);
            }
        }

        /* update the stream as bypass mode all lineotn_handle->cfg.max_dsi_streams */
        for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++) 
        {
            lineotn_handle->var.lineotn_dsi_dist[i].fec_bypassed = bypass_enc_dec;
            /* Only change the fec_type if the stream is not in low power as this function
            is also called when a FEC is deprovisioned */
            if (bypass_enc_dec &&
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr == LINE_OTN_PWR_OPERATIONAL) 
            {
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
            }
        }
    }
    else if(LINE_OTN_RATE_OTU3 == dsi_rate ||   
            LINE_OTN_RATE_OTU3E2 == dsi_rate ||
            LINE_OTN_RATE_OTU3E1 == dsi_rate 
        )

    {
        UINT32 dec_enc_value = 0;

        if(bypass_enc_dec)
        {
            dec_enc_value = mfec_field_ENCODER_BYPASS_get(NULL,lineotn_handle);
            dec_enc_value |= (0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_ENCODER_BYPASS_set(NULL,lineotn_handle,dec_enc_value);

            dec_enc_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
            dec_enc_value |= (0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,dec_enc_value);
        }
        else
        {
            dec_enc_value = mfec_field_ENCODER_BYPASS_get(NULL,lineotn_handle);
            dec_enc_value &= ~(0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_ENCODER_BYPASS_set(NULL,lineotn_handle,dec_enc_value);

            dec_enc_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
            dec_enc_value &= ~(0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,dec_enc_value);

        }

        /* update the stream as bypass mode all 3 in a slice */
        for(i=(4*slice);i<((4*slice)+LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i++)
        {
            lineotn_handle->var.lineotn_dsi_dist[i].fec_bypassed = bypass_enc_dec;
            /* Only change the fec_type if the stream is not in low power as this function
            is also called when a FEC is deprovisioned */
            if (bypass_enc_dec &&
                lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr == LINE_OTN_PWR_OPERATIONAL) 
            {
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
            }
        }

    }
    else    /* 10G always 0-11 */
    {
        mfec_field_range_ENCODER_BYPASS_set(NULL,lineotn_handle,dsi_stream,dsi_stream,bypass_enc_dec);
        mfec_field_range_DECODER_BYPASS_set(NULL,lineotn_handle,dsi_stream,dsi_stream,bypass_enc_dec);
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].fec_bypassed = bypass_enc_dec;
        /*  Only change the fec_type if the stream is not in low power as this function
        is also called when a FEC is deprovisioned */
        if (bypass_enc_dec &&
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr == LINE_OTN_PWR_OPERATIONAL) 
        {       
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = LINE_OTN_FEC_NONE;
        }
    }

    PMC_RETURN(rc);

} /* fec_mfec_bypass_set */

/*******************************************************************************
* fec_mfec_dec_bypass_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set an already fec provisioned dsi stream decoder into/out 
*   of fec bypass mode.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*   bypass_dec              - boolean,TRUE: set bypass mode \n
*                                    ,FALSE: take out of bypass mode
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
* 
*******************************************************************************/
PUBLIC PMC_ERROR fec_mfec_dec_bypass_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         BOOL               bypass_dec)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {

        if(bypass_dec)
        {
            mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,0x00000fff);
        }
        else
        {
           mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,0);
        }
    }
    else if(LINE_OTN_RATE_OTU3 == dsi_rate ||   
            LINE_OTN_RATE_OTU3E2 == dsi_rate ||
            LINE_OTN_RATE_OTU3E1 == dsi_rate 
        )

    {
        UINT32 dec_value = 0;

        if(bypass_dec)
        {
            dec_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
            dec_value |= (0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,dec_value);
        }
        else
        {
            dec_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
            dec_value &= ~(0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
            mfec_field_DECODER_BYPASS_set(NULL,lineotn_handle,dec_value);
        }

    }
    else    /* 10G always 0-11 */
    {
        mfec_field_range_DECODER_BYPASS_set(NULL,lineotn_handle,dsi_stream,dsi_stream,bypass_dec);
    }

    PMC_RETURN(rc);

} /* fec_mfec_dec_bypass_set */

/*******************************************************************************
* fec_mfec_dec_bypass_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a TRUE boolean if fec decoder mode is in bypass mode 
*   per requested dsi stream (0..11).
*
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*
* OUTPUTS:
*   *bypass_dec            - pointer to fec decoder mode mode  \n
*                            TRUE: if in  bypass mode \n
*                            FALSE: if not in bypass mode
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
* 
*******************************************************************************/
PUBLIC PMC_ERROR fec_mfec_dec_bypass_get(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         BOOL               *bypass_dec)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate;
    UINT32 dec_value = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        dec_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
        if(dec_value == 0x00000fff)
        {
            *bypass_dec = TRUE;  
        }
        else if (dec_value == 0)
        {
            *bypass_dec = FALSE;
        }
        else
        {
            rc = LINEOTN_ERR_INCORRECT_DECODER_BYPASS_CFG_DETECTED;   
        }
    }
    else if(LINE_OTN_RATE_OTU3 == dsi_rate ||   
            LINE_OTN_RATE_OTU3E2 == dsi_rate ||
            LINE_OTN_RATE_OTU3E1 == dsi_rate 
        )

    {
        dec_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
        dec_value &= (0xf<<(slice*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE)); 
        if(dec_value == 0xf)
        {
            *bypass_dec = TRUE;  
        }
        else if (dec_value == 0)
        {
            *bypass_dec = FALSE;
        }
        else
        {
            rc = LINEOTN_ERR_INCORRECT_DECODER_BYPASS_CFG_DETECTED;  
        }
    }
    else    /* 10G always 0-11 */
    {
        dec_value=mfec_field_range_DECODER_BYPASS_get(NULL,lineotn_handle,dsi_stream,dsi_stream);
/*        
        dec_value = mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle);
        dec_value &= (0x1<<(dsi_stream)); 
*/        
        if(dec_value == 0x1)
        {
            *bypass_dec = TRUE;  
        }
        else if (dec_value == 0)
        {
            *bypass_dec = FALSE;
        }
        else
        {
            rc = LINEOTN_ERR_INCORRECT_DECODER_BYPASS_CFG_DETECTED;    
        }
    }

    PMC_RETURN(rc);

} /* fec_mfec_dec_bypass_get */

/*******************************************************************************
* fec_stat_intf_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  This function initializes 4-wire OTN FEC statistics interface. This function 
*  shall be called before enabling collection of FEC statistics on a specific 
*  stream.  fec_stat_set can be used to enable statistic 
*  collection and fec_stat_intf_uninit is provided to disable the
*  interface.
*
* INPUTS:
*   *h                     - pointer to LINEOTN handle instance
*   *fec_stat_intf_cfg_ptr - pointer to FEC stat interface configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fec_stat_intf_init(lineotn_handle_t      *h,
                                    digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check state */
    if (TRUE == h->var.fec_stat_cfg.enable)
    {
        rc = LINEOTN_ERR_FEC_STAT_ALREADY_INIT;
    }

    /* validate parameters */    
    if (fec_stat_intf_cfg_ptr->collection_period == 0 ||
        fec_stat_intf_cfg_ptr->collection_period > LINEOTN_FEC_STAT_COLLECTION_PER_MAX_MS)
    {
        rc = LINEOTN_ERR_FEC_STAT_BAD_COLLECTION_PERIOD;
    }

    if (PMC_SUCCESS == rc)
    {
        /* configure fec stat block */
        rc = fec_stat_cfg(h, 
                          fec_stat_intf_cfg_ptr,
                          TRUE);
    }
    
    if (PMC_SUCCESS == rc)
    {
        /* set state */
        h->var.fec_stat_cfg.enable = TRUE;
    }

    PMC_RETURN(rc);
} /*fec_stat_intf_init*/


/*******************************************************************************
* fec_stat_intf_uninit
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   The function disables the 4-wire OTN FEC statistics interface.
*
* INPUTS:
*   *h          - pointer to LINEOTN handle instance
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fec_stat_intf_uninit(lineotn_handle_t      *h)
{
    UINT32 entry_itr;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* check state */
    if (FALSE == h->var.fec_stat_cfg.enable)
    {
        rc = LINEOTN_ERR_FEC_STAT_NOT_INIT;
    }
    
    /* check if that the FEC STAT logic is empty*/    
    for (entry_itr = 0; 
         entry_itr < LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES && rc == PMC_SUCCESS;
         entry_itr++)             
    {
        if (h->var.fec_stat_cfg.fsl_table[entry_itr] != LINEOTN_FEC_STAT_NULL_ENTRY)
        {
            rc = LINEOTN_ERR_FEC_STAT_TABLE_NOT_EMPTY;
        }
    }
   
    if (PMC_SUCCESS == rc)
    {
        /* configure fec stat block */
        rc = fec_stat_cfg(h, 
                          NULL,
                          FALSE);
    }
    
    if (PMC_SUCCESS == rc)
    {
        /* set state */
        h->var.fec_stat_cfg.enable = FALSE;
    }

    PMC_RETURN(rc);
} /* fec_stat_intf_uninit*/


/*******************************************************************************
* fec_stat_enabled
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   The function returns TRUE if the fec stat block is enabled
*
* INPUTS:
*   *h          - pointer to LINEOTN handle instance
*
* OUTPUTS:
*  None
*
* RETURNS:
*   BOOL8           - TRUE when it is enabled, otherwise FALSE
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 fec_stat_enabled(lineotn_handle_t *h)
{
    PMC_ENTRY();
    PMC_RETURN(h->var.fec_stat_cfg.enable);
} /* fec_stat_initialized */


/*******************************************************************************
* fec_stat_validate_stream
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates a given dsi stream satisfies the condition to be 
*   used to setup entries in FEC statistic block. \n
*   1. DSI stream shall be provisioned\n
*   2. FEC shall not be RSFEC (G.709) or disabled\n
*   3. Interface shall not be SFI5.1 or SFIS
*
* INPUTS:
*   *h              - pointer to LINEOTN handle instance
*   dsi_stream      - unsigned, dsi stream number to configure:\n
*                       ->for 10G configurations valid values are: (0..11) \n
*                       ->for OTL3.4 40G configurations valid values are: (0,4,8)\n
*                       ->for 40G SFIS configurations valid values are: (0,5) \n
*                       ->for 40G SFI5.1 configurations valid values are: (0,17) \n
*                       ->for 100G configurations valid values are: (0) \n
*                    - boolean, TRUE: enable statistic collection;
*                             , FALSE: disable enable statistic collection 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS -   when API execution is successful otherwise a 
*                   descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fec_stat_validate_stream(lineotn_handle_t *h,
                                          UINT32 dsi_stream)
{
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    
    if (FALSE == h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        rc = LINEOTN_ERR_FEC_STAT_SET_STREAM_NOT_PROV;
    } else if (!(h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I4 ||
                 h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I7 ||
                 h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_SWIZZLE_100G ||
                 h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_SWIZZLE_40G))
    {      
        rc = LINEOTN_ERR_FEC_STAT_SET_INVALID_FEC;
    } 
    else if (TRUE == h->var.lineotn_dsi_dist[dsi_stream].fec_bypassed)
    {
        rc = LINEOTN_ERR_FEC_STAT_SET_FEC_BYPASS;
    } 
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc ==  MLD_SFI_51 ||
               h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc ==  MLD_SFIS)
    {
        rc = LINEOTN_ERR_FEC_STAT_SET_INVALID_INTF;
    }
    
    if (PMC_SUCCESS == rc)
    {
        PMC_ASSERT(dsi_stream < h->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    }

    PMC_RETURN(rc);
} /* fec_stat_validate_stream */


/*******************************************************************************
* fec_stat_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables a statistic collection on the 4-wire OTN FEC
*   statistics interface for a specified stream.  In this function, we assume
*   that the stream is correct for the FEC Stat block
*
* INPUTS:
*   *h              - pointer to LINEOTN handle instance
*   dsi_stream      - unsigned, dsi stream number to configure:\n
*                            ->for 10G configurations valid values are: (0..11) \n
*                            ->for OTL3.4 40G configurations valid values are: (0,4,8) \n
*                            ->for 40G SFIS configurations valid values are: (0,5) \n
*                            ->for 40G SFI5.1 configurations valid values are: (0,17) \n 
*                            ->for 100G configurations valid values are: (0) 
*  logical_lane_id   - an array of logical lane identifier associated to the dsi stream.\n
*                             for 10G  : item 0 is valid\n
*                             for 40G  : item 0 to 3 are valids \n
*                             for 100G : item 0 to 19 are valids 
*   enable           - boolean, TRUE: enable statistic collection
*                             , FALSE: disable enable statistic collection \n
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - when API execution is successful otherwise a 
*                     descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fec_stat_set(lineotn_handle_t *h,
                              UINT32 dsi_stream, 
                              UINT32 logical_lane_id[LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE],
                              BOOL8 enable)
{
    fecstat_buffer_t b[1];
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 opsm23_inst_idx = 0;
    UINT8 opsm23_stream_idx = 0;
    UINT32 entry_itr = 0;
    UINT32 llid_itr = 0;
    UINT32 llid = 0;
    UINT32 num_llid = 0;
    UINT32 num_fec_source = 0;
    UINT32 phy_lane = 0;
    UINT8  fec_source_addr = 0;   
    BOOL8  found_entry;
    UINT8  llsa_latch_source = 0;
    UINT8  slice = FEC_SLICE(dsi_stream);

    PMC_ENTRY();
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    
    fecstat_buffer_init(b,h);

    /* check state */
    if  ( TRUE == enable && 
          TRUE == h->var.lineotn_dsi_dist[dsi_stream].fec_stat_enabled)
    {        
        rc = LINEOTN_ERR_FEC_STAT_SET_ALREADY_SET;
    } 
    else if (FALSE == enable && 
             FALSE == h->var.lineotn_dsi_dist[dsi_stream].fec_stat_enabled)
    {
        rc = LINEOTN_ERR_FEC_STAT_SET_NOT_SET;
    }

    /* precompute fec_source addr */
    /* 0, 4, 8: Address for I.4 FEC #0, #1, and #2. */
    /* 1, 5, 9: Address for I.7 FEC #0, #1, and #2. */
    /* 2, 6, A: Address for PMC Swizzle FEC #0, #1, and #2. */
    /* 3, 7, B: Reserved. */
    if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I4 )
    {
        fec_source_addr = 0;
    } 
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I7)
    {
        fec_source_addr = 1;
    } 
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_SWIZZLE_40G)
    {                         
        fec_source_addr = 2;
    } 
    else
    {
        fec_source_addr = 3;
    }

    num_fec_source = h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_SWIZZLE_100G?
        LINEOTN_FEC_STAT_3_FEC_SOURCE:
        LINEOTN_FEC_STAT_1_FEC_SOURCE;

    /* configure the FEC STAT block */
    /* determine number of logical lane */
    if(LINE_OTN_RATE_OTU4==h->var.lineotn_dsi_dist[dsi_stream].dsi_rate
        ) 
    {
        num_llid =  LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE;
    } 
    else
    {
        opsm23_inst_idx = dsi_stream/4; /* set the opsm idx to configure (0,1,2) */
        fec_source_addr += opsm23_inst_idx*  LINEOTN_MAX_LANES_PER_OPSM23_SLICE;
        if (h->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
            h->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 )
        {
            num_llid = LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE;            
        } 
        else 
        {
            num_llid = 1;
            opsm23_stream_idx = dsi_stream%4;
        }
    }
    
    if (PMC_SUCCESS == rc)
    {      
        llid_itr = 0;
        for (entry_itr = 0; entry_itr < LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES && llid_itr < num_llid;entry_itr++)             
        {
            found_entry = FALSE;
            if (TRUE == enable)
            {
                if (h->var.fec_stat_cfg.fsl_table[entry_itr] == LINEOTN_FEC_STAT_NULL_ENTRY)
                {
                    h->var.fec_stat_cfg.fsl_table[entry_itr] = dsi_stream;                    
                    llid = 4 * opsm23_inst_idx + opsm23_stream_idx + llid_itr;
                    phy_lane = logical_lane_id[llid_itr];
                    
                    fec_stat_logic_table_cfg(h,
                                             entry_itr,
                                             num_fec_source,
                                             fec_source_addr,
                                             llid,
                                             phy_lane);   
                    llid_itr +=1;
                    found_entry = TRUE;
                }
            } 
            else 
            {
                /* clear our entry */
                if (h->var.fec_stat_cfg.fsl_table[entry_itr] == dsi_stream)
                {
                    h->var.fec_stat_cfg.fsl_table[entry_itr] = LINEOTN_FEC_STAT_NULL_ENTRY;
                    llid_itr +=1;
                    found_entry = TRUE;
                }
            }
            
            if (found_entry == TRUE)
            {
                /* update fec_row enable*/
                fecstat_lfield_range_FEC_ROW_EN_set(b,h,entry_itr, entry_itr,
                                                    h->var.fec_stat_cfg.fsl_table[entry_itr] == LINEOTN_FEC_STAT_NULL_ENTRY?FALSE:TRUE);
            }
        }
        /* check if you gonna exceed the number of entries in the table */
        if (llid_itr !=  num_llid)
        {
            rc = LINEOTN_ERR_FEC_STAT_SET_FSL_TBL_ERROR;
        } 
        else 
        {        
            /* flush row enable */
            fecstat_buffer_flush(b);
        }
    }

    /* Select the LLSA PMon LCLK to come from the FEC Stat interface rather than the LLSA LCLK */
    if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_SWIZZLE_40G)
    {
        llsa_latch_source = (TRUE == enable) ? 0 : 1;    
        swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set(NULL,h,slice,llsa_latch_source);
    } 
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_SWIZZLE_100G)
    {
        llsa_latch_source = (TRUE == enable) ? 0 : 1;
        for (slice = 0; slice <= 2; slice++)
        {
            swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set(NULL,h,slice,llsa_latch_source);
        }
    }
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I4)
    {
        llsa_latch_source = (TRUE == enable) ? 1 : 0;    
        i4_fec_field_LLSA_LATCH_SOURCE_set(NULL,h,slice,llsa_latch_source); 
    }
    else if (h->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type ==  LINE_OTN_FEC_I7)
    {
        llsa_latch_source = (TRUE == enable) ? 1 : 0;    
        i7_fec_field_LLSA_LATCH_SOURCE_set(NULL,h,slice,llsa_latch_source); 
    }

    
    if (PMC_SUCCESS == rc)
    {
        /* update state */
        h->var.lineotn_dsi_dist[dsi_stream].fec_stat_enabled = enable;
    }
    
    PMC_RETURN(rc);
}


/*******************************************************************************
* fec_shared_ram_pwr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set a shared ram module into/out of low power mode
*   shared RAM is used only with the fecs listed bellow.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             etc.
*
*   low_power               - boolean,TRUE : set to low power
*                                    ,FALSE : take out of low power
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
*      for all lanes that belong to this slice.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_shared_ram_pwr_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         lineotn_fec_t      fec_type,
                                         BOOL               low_power)
{
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate =  LAST_LINE_OTN_FRAME_RATE;
    UINT8 i = 0;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(LINE_OTN_FEC_NONE < fec_type && 
               LAST_LINE_OTN_FEC > fec_type &&
               LINE_OTN_FEC_G709 != fec_type, LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }

    if (LINE_OTN_RATE_OTU4 == dsi_rate)  /* 100G - all slices are affected */
    {
        for (i = 0;i<3; i++) /* go over slices 0,1,2 */
        {
            if(low_power)  /* shut down all the shared rams */
            {
                shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,i,0xf);
                shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,i,0x3);
                shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,i,0x7);
                shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,i,0xf);
                shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,i,0x7);
                shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,i,0xf);
                shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,i,0xf);
                shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,i,0xf);
                shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,i,0xf);
            }
            else
            {
                switch(fec_type)
                {
                case LINE_OTN_FEC_SWIZZLE_100G:
                    shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,i,0x0);
                    break;
                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }
        }

    }    
    else if((dsi_rate >= LINE_OTN_RATE_OTU3E2 && dsi_rate <= LINE_OTN_RATE_OTU3)   ) /*40G FEC - configure 1 slice only*/            
    {
        if(low_power)  /* shut down all the shared rams */
        {
            shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x3);
            shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
            shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
            shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
        }
        else
        {
            switch(fec_type)
            {
            case LINE_OTN_FEC_I4:
                shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x3);
                shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
                shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x4);
                shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0xc);
                shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                break;

            case LINE_OTN_FEC_I7:
            case LINE_OTN_FEC_SWIZZLE_40G:
                shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                break;


            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
    }
    else /* 10G fecs - we configure a full slice */
    {
        if(low_power)  /* shut down all the shared rams */
        {
            /* make sure no 10G are operational on a slice before we pull the plug */
            for(i=(slice*LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i< ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i++)
                if( LINE_OTN_PWR_OPERATIONAL == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr )
                    break;

            if(i==((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE)) /* no more operational dsi streams - turn off shared ram for this slice */
            {
                shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x3);
                shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
                shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
                shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
            }
        }
        else   /* we set the power to high per slice on the first dsi stream configured */
        {      /* This way we configure only once */ 
            switch(fec_type)
            {
            case LINE_OTN_FEC_I4:
                if(0x4 != shared_ram_field_REGION_D2_LOWPWR_get(NULL,lineotn_handle,slice))
                {
                    shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                    shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x3);
                    shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x7);
                    shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x4);
                    shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0xc);
                    shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0xf);
                    shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0xf);   
                }
                break;

            case LINE_OTN_FEC_I7:
                if(0x0 != shared_ram_field_REGION_A_LOWPWR_get(NULL,lineotn_handle,slice))
                {
                    shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                    shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle,slice,0x0);
                }
                break;
            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
    }
    
    PMC_RETURN(rc);
}/* fec_shared_ram_pwr_set */


/*******************************************************************************
* fec_shared_ram_select_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set a shared ram module to the requested fec.
*   Shared RAM is used only with the fecs listed below.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             LINE_OTN_FEC_SWIZZLE_100G
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
PRIVATE PMC_ERROR fec_shared_ram_select_set(lineotn_handle_t   *lineotn_handle,
                                            UINT8              dsi_stream,
                                            lineotn_fec_t      fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(LINE_OTN_FEC_NONE < fec_type && 
               LAST_LINE_OTN_FEC > fec_type &&
               LINE_OTN_FEC_G709 != fec_type, LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);

    slice = FEC_SLICE(dsi_stream);

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,slice,0x2);
        break;

    case LINE_OTN_FEC_I7:
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,slice,0xC);
        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,slice,0x8);
        break;



    case LINE_OTN_FEC_SWIZZLE_100G:
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,0,0x8);
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,1,0x8);
        shared_ram_field_FEC_SELECT_set(NULL,lineotn_handle,2,0x8);
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* fec_shared_ram_select_set */


/*******************************************************************************
* fec_mfec_enc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set the mfec glue encoder to the selected fec per dsi stream.
*   This function will also set the 40G/100G mfec glue encoder selector.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
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
*      for all lanes that belong to this slice.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_mfec_enc_set(lineotn_handle_t *lineotn_handle,
                                   UINT8            dsi_stream,
                                   lineotn_fec_t    fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate = (lineotn_frame_rate_t)0 ;
    UINT32 current_fec = 0;
    UINT32 lanes_set = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    /* set 40G/100G mode */
    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            mfec_field_ENCODER_40G_100G_SEL_set(NULL,lineotn_handle,0x1); /* 100G */
        }

        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }
    else
    {
        mfec_field_ENCODER_40G_100G_SEL_set(NULL,lineotn_handle,0x0); /* 40G/10G */
    }

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x2);
            else if (1==slice)
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x2);
            else /* slice == 2 */
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x2);
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else if (1==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
        }
        break;

    case LINE_OTN_FEC_I7:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x4);
            else if (1==slice)
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x4);
            else /* slice == 2 */
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x4);
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else if (1==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 FEC to the currently set fec */

                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
        }

        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
           dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x10);
            else if (1==slice)
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x10);
            else /* slice == 2 */
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x10);
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }

        break;


    case LINE_OTN_FEC_G709:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0x2);
            }
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G RSFEC */
        {
            if(0==slice)
            {
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
            else if (1==slice)
            {
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
            else /* slice == 2 */
            {
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
        }
        else     /*10G RSFEC  - extra configuration needed if RS fec is processing with other fecs */
        {
            if(0==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modolo 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else if (1==slice)
            {
                current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modolo 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modolo 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        if(LINE_OTN_RATE_OTU4 != dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else
        {
            mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0x4);
        }
        break;

    case LINE_OTN_FEC_NONE:
        /* do nothing - mfec_field_ENCODER_40G_100G_SEL_set is the only configuration required */
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* fec_mfec_enc_set */



/*******************************************************************************
* fec_mfec_dec_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set the mfec glue decoder to the selected fec per dsi stream.
*   This function will also set the 40G/100G mfec glue decoder selector.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G
*                             etc.
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
*      for all lanes that belong to this slice.
* 
*******************************************************************************/

PRIVATE PMC_ERROR fec_mfec_dec_set(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream,
                                   lineotn_fec_t      fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate = (lineotn_frame_rate_t)0;
    UINT32 current_fec = 0;
    UINT32 lanes_set = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    /* set 40G/100G mode */
    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            mfec_field_DECODER_40G_100G_SEL_set(NULL,lineotn_handle,0x1); /* 100G */
        }
        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }
    else
    {
        mfec_field_DECODER_40G_100G_SEL_set(NULL,lineotn_handle,0x0); /* 40G/10G */
    }

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x2);
            else if (1==slice)
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x2);
            else /* slice == 2 */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x2);
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 to the currently set fec */
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else if (1==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 to the currently set fec */
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000002; /* add I4 to the currently set fec */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
        }
        break;

    case LINE_OTN_FEC_I7:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x4);
            else if (1==slice)
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x4);
            else /* slice == 2 */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x4);
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 to the currently set fec */
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else if (1==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 to the currently set fec */
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000004; /* add I7 to the currently set fec */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
            }
        }

        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
           dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x10);
            else if (1==slice)
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x10);
            else /* slice == 2 */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x10);
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        break;



    case LINE_OTN_FEC_G709:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0x2);
            }
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G RSFEC */
        {
            if(0==slice)
            {
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
            else if (1==slice)
            {
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
            else /* slice == 2 */
            {
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0x8);
                mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0xf); 
            }
        }
        else     /*10G RSFEC  - extra configuration needed if RS fec is processing with other fecs */
        {
            if(0==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 mod 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else if (1==slice)
            {
                current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else /* slice == 2 */
            {
                current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                current_fec |= 0x00000008; /* add RSFEC to the currently set fec */

                /* read modify write - RSFEC can be selected with any other 40G fec */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to add lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set |= (1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        if(LINE_OTN_RATE_OTU4 != dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else
        {
            mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0x4);
        }
        break;

    case LINE_OTN_FEC_NONE:
        /* do nothing - mfec_field_DECODER_40G_100G_SEL_set is the only configuration required */
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* fec_mfec_dec_set */


/*******************************************************************************
* fec_mfec_enc_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will reset the mfec glue encoder per the selected fec per dsi stream.
*   This function will also reset the 40G/100G mfec glue encoder selector.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
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

PRIVATE PMC_ERROR fec_mfec_enc_reset(lineotn_handle_t *lineotn_handle,
                                     UINT8            dsi_stream,
                                     lineotn_fec_t    fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate = (lineotn_frame_rate_t)0 ;
    UINT32 current_fec = 0;
    UINT32 lanes_set = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else /* with 10G fec we need a read modify write operation to make sure RS fec is not already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else /* slice == 2 */
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
        }
        break;

    case LINE_OTN_FEC_I7:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else /* with 10G fec we need a read modify write operation to make sure RS fec is not already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else /* slice == 2 */
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
        }

        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
           dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
            else if (1==slice)
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
            else /* slice == 2 */
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
        }
        else 
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }

        break;


    case LINE_OTN_FEC_G709:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
            }
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G RSFEC */
        {
            if(0==slice)
            {
                mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
            else if (1==slice)
            {
                mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
            else /* slice == 2 */
            {
                mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
        }
        else     /*10G RSFEC  - extra configuration needed if RS fec is processing with other fecs */
        {
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else /* slice == 2 */
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        if(LINE_OTN_RATE_OTU4 != dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else
        {
            mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
        }
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* fec_mfec_enc_reset */



/*******************************************************************************
* fec_mfec_dec_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will reset the mfec glue decoder per the selected fec per dsi stream.
*   This function will also reset the 40G/100G mfec glue decoder selector.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
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
*      for all lanes that belong to this slice.
* 
*******************************************************************************/

PRIVATE PMC_ERROR fec_mfec_dec_reset(lineotn_handle_t   *lineotn_handle,
                                     UINT8              dsi_stream,
                                     lineotn_fec_t      fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 slice = 0;
    lineotn_frame_rate_t dsi_rate = (lineotn_frame_rate_t)0;
    UINT32 current_fec = 0;
    UINT32 lanes_set = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(LINE_OTN_RATE_OTU4 == dsi_rate
        )
    {
        dsi_rate = LINE_OTN_RATE_OTU4; /* this is done to keep the code clean of ifdefs */
    }

    switch ( fec_type )
    {
    case LINE_OTN_FEC_I4:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else /* slice == 2 */
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000002); /* remove I4 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
        }
        break;

    case LINE_OTN_FEC_I7:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else /* with 10G fec we need a read modify write operation to make not to overrun RS fec 
                if it is already set */
        {
            /* read modify write - RSFEC can be selected with any other 40G fec */
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
            else /* slice == 2 */
            {

                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {                    
                    current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000004); /* remove I7 FEC to the currently set fec */
                    mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
            }
        }

        break;

    case LINE_OTN_FEC_SWIZZLE_40G:
        if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
           dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G FEC */            
        {
            if(0==slice)
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
            else if (1==slice)
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
            else /* slice == 2 */
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        break;



    case LINE_OTN_FEC_G709:
        if(LINE_OTN_RATE_OTU4 == dsi_rate)
        {
            if(lineotn_handle->var.lineotn_mode)
            {
                mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
            }
        }
        else if(dsi_rate >= LINE_OTN_RATE_OTU3E2 && 
                dsi_rate <= LINE_OTN_RATE_OTU3 ) /*40G RSFEC */
        {
            if(0==slice)
            {
                mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
            else if (1==slice)
            {
                mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
            else /* slice == 2 */
            {
                mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,0);
                mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,0); 
            }
        }
        else     /* 10G RSFEC  - extra configuration needed if RS fec is processing with other fecs */
        {
            if(0==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else if (1==slice)
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {                    current_fec = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }

                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
            else /* slice == 2 */
            {
                if( FALSE==fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type) )
                {
                    current_fec = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                    current_fec &= ~(0x00000008); /* remove RSFEC FEC to the currently set fec */

                    /* read modify write - RSFEC can be selected with any other 40G fec */
                    mfec_field_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle,current_fec);
                }
                /* configure the RSFEC processed lanes */
                /* dsi stream 0..11 modulo 4 to map to 4 bit range */
                /* read modify write in order to remove lanes */
                lanes_set = mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle); 
                lanes_set &= ~(1<<(dsi_stream%4));
                mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle,lanes_set); 
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        if(LINE_OTN_RATE_OTU4 != dsi_rate)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
        else
        {
            mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
        }
        break;

    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;

    }

    PMC_RETURN(rc);
}/* fec_mfec_dec_reset */


/*******************************************************************************
* fec_mfec_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will set the mfec glue configuration for a selected fec
*   per dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_NONE,
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this slice.
* 
*******************************************************************************/

PUBLIC PMC_ERROR fec_mfec_cfg_set(lineotn_handle_t   *lineotn_handle,
                                  UINT8              dsi_stream,
                                  lineotn_fec_t      fec_type)
{
    PMC_ERROR       rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type < LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);

    /* set power mode to operational */
    rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,FALSE);/* operational  mode */

    /* shared RAM  - only for fecs that use it*/
    if( PMC_SUCCESS == rc  && 
        (LINE_OTN_FEC_NONE < fec_type && 
         LAST_LINE_OTN_FEC > fec_type &&
         LINE_OTN_FEC_G709 != fec_type))
    {
        rc = fec_shared_ram_select_set(lineotn_handle,dsi_stream,fec_type);
    }

    /* mfec glue enc/dec */
    if( PMC_SUCCESS == rc )
    {
        rc = fec_mfec_enc_set(lineotn_handle,dsi_stream,fec_type);
    }

    if( PMC_SUCCESS == rc )
    {
        rc = fec_mfec_dec_set(lineotn_handle,dsi_stream,fec_type);
    }

    PMC_RETURN(rc);
} /* fec_mfec_cfg_set */


/*******************************************************************************
* fec_i4_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and provisions a I4 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_i4_cfg_set(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_I4;
    UINT8           slice = FEC_SLICE(dsi_stream);
    UINT8           decoder_op_mode = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    /* make sure this section of the code is done only once for 100G/40G configurations */    
    if( FALSE == slice_provisioned ||
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LAST_LINE_OTN_FEC || 
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_NONE )
    {
        rc = fec_mfec_cfg_set(lineotn_handle,dsi_stream,fec_type);
        i4_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,0); /* take out of reset (after configuring MFEC)*/
    }

    if (PMC_SUCCESS == rc)
    {
        rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, FALSE);
    }   

    /* I4 block */
    if( PMC_SUCCESS == rc )
    {
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = fec_type;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

        if(LINE_OTN_RATE_OTU3 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||   /* 40G  */
           LINE_OTN_RATE_OTU3E2 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||
           LINE_OTN_RATE_OTU3E1 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
            /* make sure dsi_stream is the right boundary for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            i4_fec_field_DECODER_STATS_ENABLE_set(NULL,lineotn_handle,slice,1);/* enable lane based statistics */
            i4_fec_field_ENCODER_DATA_TYPE_set(NULL,lineotn_handle,slice,1);
            i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,3,3,0); /*enable error correction for the 40G lanes - set bit 3 */
            i4_fec_field_LLSA_LATCH_SOURCE_set(NULL,lineotn_handle,slice,0);
            switch (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].deskew_type) 
            {
            case LINE_OTN_OIF_SFIS_DESKEW:
                decoder_op_mode = 1;
                break;
            default:
                decoder_op_mode = 3;
                break;
            }
            /* Set the mode to either OTL based (lane based) or not */
            i4_fec_field_DECODER_OP_MODE_set(NULL,lineotn_handle,slice,decoder_op_mode); 
           
            /* mark rest of the 3 dsi streams as provisioned for 40G fec */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */
        }
        else  /* 4x10G */
        {
            UINT8 lane = (dsi_stream%4);

            if( FALSE == slice_provisioned ) /* make sure this section of the code is done only once per slice */
            {
                i4_fec_field_DECODER_OP_MODE_set(NULL,lineotn_handle,slice,0);
                i4_fec_field_ENCODER_DATA_TYPE_set(NULL,lineotn_handle,slice,0);
                i4_fec_field_DECODER_STATS_ENABLE_set(NULL,lineotn_handle,slice,0);/* disable lane based statistics */
            }
            i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,lane,lane,0); /*enable error correction for the 10G lane */
        }
    }

    PMC_RETURN(rc);

}/* fec_i4_cfg_set */


/*******************************************************************************
* fec_i4_cfg_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a I4 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_i4_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    UINT8           slice = 0;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_I4;
    UINT8           i = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    /* I4 block */
    if( TRUE == slice_provisioned )
    {
        if(LINE_OTN_RATE_OTU3 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||   /* 40G  */
           LINE_OTN_RATE_OTU3E2 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||
           LINE_OTN_RATE_OTU3E1 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
            /* make sure dsi_stream is the right boundary for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            i4_fec_field_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice, 0xF); /*disable error correction for the 40G lanes - set bit 3 */

            for(i = dsi_stream;i < (dsi_stream + LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i++)
            {

                /* mark the 4 dsi streams as not provisioned  */
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
            }
            rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I4);

            if( PMC_SUCCESS == rc )
            {
                rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I4);
            }

            
        }
        else  /* 4x10G */
        {
            UINT8 lane = (dsi_stream%4);
            i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle,slice,lane,lane,1); /*disable error correction for the 10G lane */

            /* mark the current 10G dsi stream as not provisioned  */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = LINE_OTN_FEC_NONE;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;

            rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I4);

            if( PMC_SUCCESS == rc )
            {
                rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I4);
            }
        }
    }

    if(  PMC_SUCCESS == rc  && /* make sure this section of the code is done only once */
         0xf == i4_fec_field_DECODER_LN_DISABLE_get(NULL,lineotn_handle,slice)) /* this will make sure we are calling this code only after all 10G streams are deprovisioned */
    {
        /* set to bypass mode */
        rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, TRUE);

        if( PMC_SUCCESS == rc )
        {
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */
        }
        i4_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1); /* reset */
    }

    PMC_RETURN(rc);

}/* fec_i4_cfg_reset */


/*******************************************************************************
* fec_swizzle40_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and provisions a swizzle7 40G fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G configurations (0,4,8)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_swizzle40_cfg_set(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream)
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_SWIZZLE_40G;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 || 
               dsi_stream == 4 ||
               dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    if( FALSE == slice_provisioned ) /* make sure this section of the code is done only once */
    {        
        UINT8 slice = FEC_SLICE(dsi_stream);
        rc = fec_mfec_cfg_set( lineotn_handle,dsi_stream,fec_type );/* operational  mode */
        swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,0); /* take out of reset */
        if (PMC_SUCCESS == rc)
        {
            rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, FALSE);
        }   

        /* swizzle7 40G block */
        if( PMC_SUCCESS == rc )
        {
            mfec_field_SWIZZLE_SEL_100G_40G_set(NULL,lineotn_handle,0);/* mfec set to swizzle 40G */
            swizzle7_40g_field_DEC_DISABLE_RESYNC_set(NULL,lineotn_handle,slice,1); 
            swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set(NULL,lineotn_handle,slice,1);

            swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,0); /* enable decoder */

            /* Configure FEC for optimal performance */
            swizzle7_40g_field_REPROCESS_CONTROL_set(NULL, lineotn_handle,slice,0x1f);
            swizzle7_40g_field_CONF_T5_0_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T5_1_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T5_2_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T5_3_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T5_4_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T5_5_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_0_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_1_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_2_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_3_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_4_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_5_set(NULL, lineotn_handle,slice,0x7f);
            swizzle7_40g_field_CONF_T6_6_set(NULL, lineotn_handle,slice,0x7f);

            /* mark the 4 dsi streams as provisioned for 40G fec */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm = FALSE;/* scramble before encoder and after decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].scramble_in_opsm = FALSE;/* scramble before encoder and after decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].scramble_in_opsm = FALSE;/* scramble before encoder and after decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].scramble_in_opsm = FALSE;/* scramble before encoder and after decoder */
        }
    }

    PMC_RETURN(rc);

}/* fec_swizzle40_cfg_set */


/*******************************************************************************
* fec_swizzle40_cfg_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a swizzle 7 40G fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 40G configurations (0,4,8)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_swizzle40_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                          UINT8              dsi_stream)
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    UINT8           slice = 0;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_SWIZZLE_40G;
    UINT8           i = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 || 
               dsi_stream == 4 ||
               dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    /* swizzle7 40G block */
    if( TRUE == slice_provisioned )
    {
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,1); /* disable decoder */

        for(i=dsi_stream;i<dsi_stream+LINEOTN_MAX_LANES_PER_OPSM23_SLICE;i++)
        {
            /* mark the 4 dsi streams as not provisioned  */
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
        }
        rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_SWIZZLE_40G);

        if( PMC_SUCCESS == rc )
            rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_SWIZZLE_40G);
       
        if( PMC_SUCCESS == rc )
            rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, TRUE);

        if( PMC_SUCCESS == rc )
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */

        swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1); /*set to reset */
    }

    PMC_RETURN(rc);

}/* fec_swizzle40_cfg_reset */


/*******************************************************************************
* fec_i7_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and provisions a I7 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_i7_cfg_set(lineotn_handle_t   *lineotn_handle,
                                 UINT8              dsi_stream )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_I7;
    UINT8           slice = FEC_SLICE(dsi_stream);
    lineotn_deskew_type_t deskew_type;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);
    
    /* make sure this section of the code is done only once for 10G/40G configurations */    
    if( FALSE == slice_provisioned ||
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LAST_LINE_OTN_FEC || 
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_NONE )
    {
        rc = fec_mfec_cfg_set(lineotn_handle,dsi_stream,fec_type);

        /* I7 fec needs to be taken out of reset after out of taking low power */
        i7_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,0);  /* take out of reset */
        i7_fec_field_DECODER_DISABLE_set(NULL,lineotn_handle,slice, 0x0); 
    }
    if (PMC_SUCCESS == rc)
    {
        rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, FALSE);
    }   

    /* I7 block */
    if( PMC_SUCCESS == rc )
    {
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = fec_type;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm = FALSE;/* scramble after encoder and before decoder */

        if(LINE_OTN_RATE_OTU3 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||   /* 40G  */
           LINE_OTN_RATE_OTU3E2 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||
           LINE_OTN_RATE_OTU3E1 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
            /* make sure dsi_stream is the right boundary for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            i7_fec_field_DECODE_TYPE_set(NULL,lineotn_handle,slice,1);/* 40G,if statistics collected set statistics to 1x40G */
            i7_fec_field_ENCODE_TYPE_set(NULL,lineotn_handle,slice,1);
            i7_fec_field_LLSA_LATCH_SOURCE_set(NULL,lineotn_handle,slice,0); 

            deskew_type = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].deskew_type;
            if ((deskew_type == LINE_OTN_ENHANCED_SFIS_DESKEW) ||
                (deskew_type == LINE_OTN_NO_DESKEW))
            {
                /* Enable lane based statistics when the OTL layer is used */
                i7_fec_field_STATS_ENABLE_set(NULL,lineotn_handle,slice,1);    
            }      
            
            /* mark rest of the 3 dsi streams as provisioned for 40G fec */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].scramble_in_opsm = FALSE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].scramble_in_opsm = FALSE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].scramble_in_opsm = FALSE;/* scramble after encoder and before decoder */

        }
        else  /* 4x10G */
        {
            UINT8 lane = (dsi_stream%4);

            if( FALSE == slice_provisioned ) /* make sure this section of the code is done only once per slice */
            {
                i7_fec_field_DECODE_TYPE_set(NULL,lineotn_handle,slice,0);
                i7_fec_field_ENCODE_TYPE_set(NULL,lineotn_handle,slice,0);
                i7_fec_field_STATS_ENABLE_set(NULL,lineotn_handle,slice,0); 
                i7_fec_field_LLSA_LATCH_SOURCE_set(NULL,lineotn_handle,slice,0);  
            }

            /* Disable fec_fail input coming from the OTN_MLD */
            opsm23_field_RX_FAIL_LANE_LOS_set(NULL,lineotn_handle,slice, lane, 0);
            opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set(NULL,lineotn_handle,slice, lane, 0);
            opsm23_field_RX_FAIL_LANE_OTUK_AIS_set(NULL,lineotn_handle,slice, lane, 0);
            opsm23_field_RX_FAIL_LANE_OTL_AIS_set(NULL,lineotn_handle,slice, lane, 0);
            otn_mld_field_DLOM_TO_FFAIL_MAPPING_set(NULL,lineotn_handle,dsi_stream,0);
       
        }
    }

    PMC_RETURN(rc);

}/* fec_i7_cfg_set */


/*******************************************************************************
* fec_i7_cfg_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a I7 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice.\n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_i7_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    UINT8           slice = 0;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_I7;
    UINT8           i = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    /* I7 block */
    if( TRUE == slice_provisioned )
    {
        if(LINE_OTN_RATE_OTU3 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||   /* 40G  */
           LINE_OTN_RATE_OTU3E2 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate ||
           LINE_OTN_RATE_OTU3E1 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
            /* make sure dsi_stream is the right boundary for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            /* Disable the I7 FEC Decoder for the entire slice */
            i7_fec_field_DECODER_DISABLE_set(NULL,lineotn_handle,slice, 0xF); 
            i7_fec_field_STATS_ENABLE_set(NULL,lineotn_handle,slice, 0x0); 

            for(i=dsi_stream;i<dsi_stream+LINEOTN_MAX_LANES_PER_OPSM23_SLICE;i++)
            {                
                /* mark the 4 dsi streams as not provisioned  */
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
            }
            rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I7);

            if( PMC_SUCCESS == rc )
                rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I7);
           
        }
        else  /* 4x10G */
        {
            /* mark the current 10G dsi stream as not provisioned  */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = LINE_OTN_FEC_NONE;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
 
            rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I7);

            if( PMC_SUCCESS == rc )
                rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_I7);
        }
    }

    if(  PMC_SUCCESS == rc  && 
         FALSE == fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type))
    {
        /* set to bypass mode */
        rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, TRUE);

        if( PMC_SUCCESS == rc )
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */

        i7_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1);  /* reset */
    }

    PMC_RETURN(rc);

}/* fec_i7_cfg_reset */






/*******************************************************************************
* fec_gfec_enc_dec_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function finds the gfec encoder/decoder configuration based on
*   already provisioned dsi streams.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       unsigned - gfec encoder/decoder configuration
*
* NOTES:
*   1. in 40G/100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE UINT8 fec_gfec_enc_dec_type_get(lineotn_handle_t   *lineotn_handle)
{
    UINT8     enc_dec_type = 0; /* encoder/decoder type 12x10G */

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* are we at 100G configuration? - check dsi 0 only */
    if(4 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[0].dsi_rate))
    {
        enc_dec_type = 0x08; /* encoder/decoder type 100G */
    }
    else /* count 40G streams and their spread over the 12 dsi lanes */
    {
        if(3 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[0].dsi_rate))
            enc_dec_type |= (0x1<<0);

        if(3 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[4].dsi_rate))
            enc_dec_type |= (0x1<<1);

        if(3 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[8].dsi_rate))
            enc_dec_type |= (0x1<<2);
    }

    PMC_RETURN(enc_dec_type);
}/* fec_gfec_enc_dec_type_get */

/*******************************************************************************
* fec_gfec_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and provisions a gfec fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11);
*                             for 40G configurations (0,4,8);
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice. \n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_gfec_cfg_set(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream)
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            gfec_provisioned = FALSE;
    UINT8           i = 0;
    UINT8           enc_dec_type = 0;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_G709;
    BOOL            slice_provisioned = FALSE;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the gfec has already been provisioned before  */
    /* find if there are any streams provisioned on this fec */
    for( i=0; i < lineotn_handle->cfg.max_dsi_streams ; i++ )
        if( LINE_OTN_FEC_G709 == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type)
            gfec_provisioned = TRUE;

    if( FALSE == gfec_provisioned ) /* make sure this section of the code is done only once */
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            /* gfec one time config */
            gfec_field_FEC_SW_RST_set(NULL,lineotn_handle,0);  /* take out of reset */
            gfec_field_REG_DISABLE_UNCORR_set(NULL,lineotn_handle,0); /* uncorrected words detection enabled */
            gfec_field_REG_DISABLE_SCRAM_set(NULL,lineotn_handle,0); /* scrambler enabled */
        }
        else
        {
            /* gfec one time config */
            sysotn_gfec_field_FEC_SW_RST_set(NULL,lineotn_handle,0);  /* take out of reset */
            sysotn_gfec_field_REG_DISABLE_UNCORR_set(NULL,lineotn_handle,0); /* uncorrected words detection enabled */
            sysotn_gfec_field_REG_DISABLE_SCRAM_set(NULL,lineotn_handle,0); /* scrambler enabled */
        }
    }

    /* check if the slice has already been provisioned before  */
    /* make sure this section of the code is done only once for 100G/40G configurations */    
    if( FALSE == slice_provisioned ||
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LAST_LINE_OTN_FEC || 
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type == LINE_OTN_FEC_NONE )
    {
        rc = fec_mfec_cfg_set( lineotn_handle,dsi_stream,fec_type );/* operational  mode */
    }
    if (PMC_SUCCESS == rc)
    {
    rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, FALSE);
    }   

    /* gfec block */
    if( PMC_SUCCESS == rc )
    {
        /* get the encoder/decoder type based on already provisioned dsi streams */
        enc_dec_type = fec_gfec_enc_dec_type_get(lineotn_handle);

        if(lineotn_handle->var.lineotn_mode)
        {
            gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,enc_dec_type);
            gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,enc_dec_type);
        }
        else
        {
            sysotn_gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,enc_dec_type);
            sysotn_gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,enc_dec_type);
        }
  
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = fec_type;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
        lineotn_handle->var.lineotn_dsi_dist[dsi_stream].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

        if (4 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
        {
            UINT8 i = 0;

            
            /* enable decoder for all 12 streams */
            if(lineotn_handle->var.lineotn_mode)
            {
                gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0);
            }
            else
            {
                sysotn_gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,0);
            }
          

            /* mark rest of the 12 dsi streams as provisioned for 100G fec */
            for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
            {
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = fec_type;
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
                lineotn_handle->var.lineotn_dsi_dist[i].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

            }

        }
        else if (3 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
        {
            /* G709 FEC 40G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* read modify write */
            UINT32 enable_dec_bm = 0x0fff; /* all 12 dsi streams are disabled */
            UINT32 dec_disable_reg_setting = gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle);

            /* make sure dsi_stream is the right boundaries for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            /* turn off the bits for this slice */
            enable_dec_bm &= (UINT32)(~(0x0000000f<<dsi_stream));

            dec_disable_reg_setting &= enable_dec_bm;

            gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,dec_disable_reg_setting);
            
            /* mark rest of the 3 dsi streams as provisioned for 40G fec */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+1].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+2].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream+3].scramble_in_opsm = TRUE;/* scramble after encoder and before decoder */

        }
        else  /* 10G */
        {
            /* G709 FEC 10G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* read modify write */
            UINT32 enable_dec_bm = 0x0fff; /* all 12 dsi streams are disabled */
            UINT32 dec_disable_reg_setting = gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle);

            /* turn off the bits for this stream  */
            enable_dec_bm &= (UINT32)(~(1<<dsi_stream));
            dec_disable_reg_setting &= enable_dec_bm;

            /* enable decoder for all 4 dsi streams on this slice */
            gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle,dec_disable_reg_setting);
        }

    }

    PMC_RETURN(rc);

}/* fec_gfec_cfg_set */


/*******************************************************************************
* fec_gfec_cfg_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a gfec fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice.\n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_gfec_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                     UINT8              dsi_stream)
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            gfec_provisioned = FALSE;
    UINT8           i = 0;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_G709;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the gfec has already been provisioned before  */
    /* find if there are any streams provisioned on this fec */
    for( i=0; i < lineotn_handle->cfg.max_dsi_streams ; i++ )
        if( LINE_OTN_FEC_G709 == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type)
        {
            gfec_provisioned = TRUE;
            break;
        }

    /* gfec block */
    if( TRUE == gfec_provisioned )
    {
        if (4 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
        {
            /* 100G no longer valid --> mark all 12 dsi streams as free 12x10G */
            if(lineotn_handle->var.lineotn_mode)
            {
                gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,0);
                gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,0);
            }
            else
            {
                sysotn_gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,0);
                sysotn_gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,0);
            }

            /* mark rest of the 12 dsi streams as provisioned for 100G fec */
            for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
            {
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
            }

            rc = fec_mfec_enc_reset(lineotn_handle,0,LINE_OTN_FEC_G709);

            if( PMC_SUCCESS == rc )
                rc = fec_mfec_dec_reset(lineotn_handle,0,LINE_OTN_FEC_G709);

        }
        else if (3 == lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)) 
        {
            /* G709 FEC 40G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* read modify write */
            /* remove 40G config from the enc/dec type */
            UINT32 enc_dec_reg_val = gfec_field_REG_TX_ENCODE_TYPE_get(NULL,lineotn_handle);
            UINT32 enc_dec_rmv_bm = ~(1<<(dsi_stream/4));

            /* make sure dsi_stream is the right boundaries for 40G cases */
            PMC_ASSERT(dsi_stream == 0 || 
                       dsi_stream == 4 ||
                       dsi_stream == 8,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

            enc_dec_reg_val &= enc_dec_rmv_bm;

            /* set new config */
            gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,enc_dec_reg_val);
            gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,enc_dec_reg_val);

            /* mark the 4 dsi streams as not provisioned  */
            for(i=dsi_stream;i<dsi_stream+LINEOTN_MAX_LANES_PER_OPSM23_SLICE;i++)
            {
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
                lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
 
                rc = fec_mfec_enc_reset(lineotn_handle,i,LINE_OTN_FEC_G709);
             
                if( PMC_SUCCESS == rc )
                    rc = fec_mfec_dec_reset(lineotn_handle,i,LINE_OTN_FEC_G709);
            }

        }
        else  /* 4x10G */
        {
            /* G709 FEC 10G mode is only valid for LINEOTN */
            PMC_ASSERT(lineotn_handle->var.lineotn_mode,LINEOTN_ERR_INVALID_PARAMETERS,0,0);

            /* no need to touch gfec_field_REG_TX_ENCODE_TYPE_set/gfec_field_REG_RX_DECODE_TYPE_set */
            /* with 10G dsi streams                                                                 */

            /* mark the current 10G dsi stream as not provisioned  */
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = LINE_OTN_FEC_NONE;
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
 
            rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_G709);

            if( PMC_SUCCESS == rc )
                rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_G709);

        }
    }

    /* find if there are any streams provisioned on this fec */
    gfec_provisioned = FALSE;
    for( i=0; i < lineotn_handle->cfg.max_dsi_streams ; i++ )
        if( LINE_OTN_FEC_G709 == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type)
        {
            gfec_provisioned = TRUE;
            break;
        }

    /* make sure this section of the code is done only once */
    /* when there are no streams provisioned as gfec        */
    if(  PMC_SUCCESS == rc  && FALSE == gfec_provisioned )
    {
        /* set to bypass mode */
        rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, TRUE);

        if( PMC_SUCCESS == rc )
            rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */

        if(lineotn_handle->var.lineotn_mode)
        {
            gfec_field_FEC_SW_RST_set(NULL,lineotn_handle,1);  /* set to reset */
        }
        else
        {
            sysotn_gfec_field_FEC_SW_RST_set(NULL,lineotn_handle,1);  /* set to reset */
        }

    }

    PMC_RETURN(rc);

}/* fec_gfec_cfg_reset */


/*******************************************************************************
* fec_swizzle100_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and provisions a swizzle 100G fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. in 100G configurations base dsi stream is de-allocating resources
*      for all lanes that belong to this slice.\n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_swizzle100_cfg_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL      slice_provisioned = FALSE;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_SWIZZLE_100G;
    UINT8 i = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0 ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    if( FALSE == slice_provisioned ) /* make sure this section of the code is done only once */
    {
        rc = fec_mfec_cfg_set( lineotn_handle,dsi_stream,fec_type );/* operational  mode */

        mfec_field_SWIZZLE_SEL_100G_40G_set(NULL,lineotn_handle,7);/* mfec set to swizzle 100G */
        for(i=0;i<3;i++) {
            swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set(NULL,lineotn_handle,i,1);
            /* take out of reset (required for all 3 instances)*/
            swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,i,0); 
            /* enable decoder for all participating 40G slices */
            swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,i,0);
            /* Configure FEC for optimal performance */
            swizzle7_40g_field_REPROCESS_CONTROL_set(NULL, lineotn_handle,i,0x1f);
            swizzle7_40g_field_CONF_T5_0_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T5_1_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T5_2_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T5_3_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T5_4_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T5_5_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_0_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_1_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_2_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_3_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_4_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_5_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_CONF_T6_6_set(NULL, lineotn_handle,i,0x7f);
            swizzle7_40g_field_set( NULL, lineotn_handle, i, MEM_TYPE_CONFIG,
                                    0x00020030, 0x00000001, 0xfffffffe, 0, 1);
    }
         
    }
    if (PMC_SUCCESS == rc)
    {
    rc = fec_mfec_bypass_set(lineotn_handle, dsi_stream, FALSE);
    }   

    /* swizzle 100G block */
    if( PMC_SUCCESS == rc )
    {
        /* mark rest of the 12 dsi streams as provisioned for 100G fec */
        for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
        {
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = fec_type;
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_OPERATIONAL;
            lineotn_handle->var.lineotn_dsi_dist[i].scramble_in_opsm = FALSE;/* scramble before encoder and after decoder */
        }

    }

    PMC_RETURN(rc);

}/* fec_swizzle100_cfg_set */

/*******************************************************************************
* fec_swizzle100_cfg_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a swizzle 100G fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
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
*      for all lanes that belong to this slice.\n
*   2. dsi stream must be provisioned before invoking this function.
* 
*******************************************************************************/
PRIVATE PMC_ERROR fec_swizzle100_cfg_reset(lineotn_handle_t   *lineotn_handle,
                                           UINT8              dsi_stream)
{
    PMC_ERROR       rc = PMC_SUCCESS;
    BOOL            slice_provisioned = FALSE;
    lineotn_fec_t   fec_type = LINE_OTN_FEC_SWIZZLE_100G;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream == 0,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(LINE_OTN_FEC_SWIZZLE_100G == fec_type,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);

    /* check if the slice has already been provisioned before  */
    slice_provisioned = fec_is_slice_prov(lineotn_handle,dsi_stream,fec_type);

    /* swizzle 100G block */
    if( TRUE == slice_provisioned )
    {
        UINT8 i = 0;

        /* mark rest of the 12 dsi streams as not provisioned for 100G fec */
        for(i=0;i<lineotn_handle->cfg.max_dsi_streams;i++)
        {
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type = LINE_OTN_FEC_NONE;
            lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
        }

    }

    rc = fec_pwr_mode_config_set( lineotn_handle,dsi_stream,fec_type,TRUE);/* low power  mode */
    
    if( PMC_SUCCESS == rc )
        rc = fec_mfec_enc_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_SWIZZLE_100G);

    if( PMC_SUCCESS == rc )
        rc = fec_mfec_dec_reset(lineotn_handle,dsi_stream,LINE_OTN_FEC_SWIZZLE_100G);

    if( PMC_SUCCESS == rc )
    {
        /* disable decoder for all participating 40G slices */
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,0,1);
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,1,1);
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,2,1);

        /* Put the SWIZZLE FEC back in S/W Reset */
        swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,0,1);
        swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,1,1);
        swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,2,1);
    }

    PMC_RETURN(rc);

}/* fec_swizzle100_cfg_reset */

/*******************************************************************************
* fec_validate_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates deprovisioning parameters and makes sure that
*   the proper resources are being deprovisioned.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
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

PRIVATE PMC_ERROR fec_validate_deprov(lineotn_handle_t   *lineotn_handle,
                                      UINT8              dsi_stream,
                                      lineotn_fec_t      fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    /* PMC_ASSERT(fec_type< LAST_LINE_OTN_FEC ,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0); */
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    switch (lineotn_frame_rate_otuk_num_get(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate))
    {
        case 4:
            /* check that dsi stream is 0 */
            if(dsi_stream != 0 )
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

        case 3:
            /* check that dsi stream is 0,4 or 8 */
            if(dsi_stream != 0 && dsi_stream != 4 && dsi_stream != 8)
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

        case 2:
            /* nothing to check here */
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

    }

    PMC_RETURN(rc);

}/* fec_validate_deprov */


/*******************************************************************************
* fec_validate_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates provisioning parameters and makes sure that
*   the proper resources are being provisioned.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
* OUTPUTS:
*   None.
*
* RETURNS:
*       0 - PMC_SUCCESS or internally generated error code is returned: \n 
*       LINEOTN_ERR_INVALID_PARAMETERS;
*       LINEOTN_ERR_OTHER_FEC_ALREADY_PROV;
*       LINEOTN_ERR_STRONG_FEC_MIX_NOT_SUPPORTED

*
* NOTES:
*
*******************************************************************************/

PRIVATE PMC_ERROR fec_validate_prov(lineotn_handle_t   *lineotn_handle,
                                    UINT8              dsi_stream,
                                    lineotn_fec_t      fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 i = 0;
    UINT8 slice = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type< LAST_LINE_OTN_FEC ,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check that the dsi stream is not currently provisioned as different fec  */
    if( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type != LAST_LINE_OTN_FEC)       
        rc = LINEOTN_ERR_OTHER_FEC_ALREADY_PROV; 
    

    /* check that the dsi stream rate provisioning works with the selected fec */
    if (PMC_SUCCESS == rc )
    {
        switch (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate)
        {
        case LINE_OTN_RATE_OTU4:
            if(fec_type != LINE_OTN_FEC_G709 &&
               fec_type != LINE_OTN_FEC_SWIZZLE_100G
                )
                rc = LINEOTN_ERR_INVALID_PARAMETERS;

            /* check that dsi stream is 0 */
            if(dsi_stream != 0 )
                rc = LINEOTN_ERR_INVALID_PARAMETERS;

            break;

        case LINE_OTN_RATE_OTU3E2:
        case LINE_OTN_RATE_OTU3E1:
        case LINE_OTN_RATE_OTU3:
            /* check that dsi stream is 0,4 or 8 */
            if(dsi_stream != 0 && dsi_stream != 4 && dsi_stream != 8)
                rc = LINEOTN_ERR_INVALID_PARAMETERS;

            break;

        case LINE_OTN_RATE_OTU2F:
        case LINE_OTN_RATE_OTU2E:
        case LINE_OTN_RATE_OTU2:
        case LINE_OTN_RATE_OTU1E:
        case LINE_OTN_RATE_OTU1F:
            if(fec_type != LINE_OTN_FEC_G709 &&
               fec_type != LINE_OTN_FEC_I4 &&
               fec_type != LINE_OTN_FEC_I7)
                rc = LINEOTN_ERR_INVALID_PARAMETERS;

            if(PMC_SUCCESS == rc)
            {
                /* HW supports mixing 10G RSFEC with only 1 other strong FEC; */
                /* thus, check to see whether the requested config is valid. */
                
                slice = FEC_SLICE(dsi_stream);
               
                /* Iterate over all 10G streams in the 40G slice */               
                for(i = (slice * LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i < ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE) ; i++ ) 
                {
                    /* compare configured fec with newly requested fec */
                    /* if they are not the same type at least one of them should be RSFEC */                                        
                    if( i != dsi_stream &&
                        fec_type != LINE_OTN_FEC_G709 && 
                        lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type != LAST_LINE_OTN_FEC && 
                        lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type != LINE_OTN_FEC_NONE) 
                    {
                        if (fec_type != lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type &&
                           lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type != LINE_OTN_FEC_G709)
                        {
                            rc = LINEOTN_ERR_STRONG_FEC_MIX_NOT_SUPPORTED;
                            break;
                        }                                               
                    }
                }                    
            }
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }
    }

    PMC_RETURN(rc);

}/* fec_validate_prov */


/*******************************************************************************
* fec_is_slice_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks fec slice provisioning status per requested fec and dsi
*   stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
*   fec_type                - enum, requested fec module:
*                             LINE_OTN_FEC_I4,
*                             LINE_OTN_FEC_I7,
*                             LINE_OTN_FEC_G709,
*                             LINE_OTN_FEC_SWIZZLE_100G,
*                             LINE_OTN_FEC_SWIZZLE_40G,
*                             etc.
* OUTPUTS:
*   None.
*
* RETURNS:
*       boolean - TRUE : slice is provisioned; 
*                 FALSE : slice is not provisioned
*
* NOTES:
*
*******************************************************************************/

PRIVATE BOOL fec_is_slice_prov(lineotn_handle_t   *lineotn_handle,
                               UINT8              dsi_stream,
                               lineotn_fec_t      fec_type)
{
    BOOL  rc = FALSE;
    UINT8 slice = 0,i;
    UINT8 count_10G = 0 ;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(fec_type< LAST_LINE_OTN_FEC ,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);

    if (TRUE == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {        
        /* find if there are more than 1 10G dsi streams provisioned on this slice */
        slice = FEC_SLICE(dsi_stream); /* 0,1,2 */
        
        for(i = (slice*LINEOTN_MAX_LANES_PER_OPSM23_SLICE);i < ((slice + 1)*LINEOTN_MAX_LANES_PER_OPSM23_SLICE) ; i++ ) 
            if( fec_type == lineotn_handle->var.lineotn_dsi_dist[i].dsi_fec_type)
                count_10G++;
        
        if(0 != count_10G) 
            rc = TRUE;  /* slice has already been provisioned before */
    }

    PMC_RETURN(rc);
}/* fec_is_slice_prov */


/*******************************************************************************
* fec_set_scrambling_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the requested scrambling mode to be either in the opsm
*   or at the otu_frm depending on the fec type.
*
* INPUTS:
*   lineotn_handle    - pointer to lineotn handle instance.
*
*   dsi_stream        - unsigned, dsi stream to configure:
*                       for 10G configurations (0..11),
*                       for 40G configurations (0,4,8),
*                       for 100G configurations (0)
*
*   scramble_in_opsm  - boolean,TRUE:scrambling/descrambling is done at the opsm;
*                               FALSE:scrambling/descrambling is not done at the opsm
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       0 - PMC_SUCCESS or internally generated error code
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR fec_set_scrambling_mode(lineotn_handle_t *lineotn_handle,
                                          UINT8            dsi_stream,
                                          BOOL             scramble_in_opsm)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 opsm23_idx=0;
    otu_frm_stream_rate_t otu_rate = RATE_10G;
    UINT8 otu_dsi_stream = 0;
    BOOL  scramble_in_otu = (scramble_in_opsm?FALSE:TRUE); /* OTU/opsm scrambling are opposite in values */

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    if (LINE_OTN_RATE_OTU4 == lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate
    ) /* 100G */
    {
        /* configure opsm4 */
        mld_opsm4_scramble_set(lineotn_handle,scramble_in_opsm);
        otu_rate = RATE_100G;
    }
    else
    {
        opsm23_idx = FEC_SLICE(dsi_stream); /* 0,1,2 */

        if( lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate <= LINE_OTN_RATE_OTU3 &&
            lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate >= LINE_OTN_RATE_OTU3E2)  /* 40G */
        {
            otu_rate = RATE_40G;
            otu_dsi_stream = opsm23_idx; /* 0,1,2 */
        }
        else /* 10G */
        {
            otu_rate = RATE_10G;
            otu_dsi_stream = dsi_stream; /* 0-11 */
        }

        mld_opsm23_scramble_set(lineotn_handle,opsm23_idx,dsi_stream,scramble_in_opsm);
    }

    rc = otu_frm_stream_scrambling_set(lineotn_handle->otu_frm_handle,otu_dsi_stream,otu_rate,scramble_in_otu);
    
    PMC_RETURN(rc);

}/* fec_set_scrambling_mode */


/*******************************************************************************
* fec_stat_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  This function configures the FEC STAT block.
*
* INPUTS:
*   *lineotn_handle        - pointer to LINEOTN handle instance
*   *fec_stat_intf_cfg_ptr - pointer to FEC stat interface configuration
*   enable                 - enables or disables the fec state block
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR fec_stat_cfg(lineotn_handle_t *lineotn_handle,                               
                               digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr,
                               BOOL8            enable)
{
    fecstat_buffer_t b[1];
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    
    fecstat_buffer_init(b,lineotn_handle);

    if (TRUE == enable) 
    {
        PMC_ASSERT(fec_stat_intf_cfg_ptr!=NULL,LINEOTN_ERR_INVALID_PARAMETERS,0,0);
        fecstat_field_FEC_COLLCT_CNT_set(b,lineotn_handle,
                                         fec_stat_intf_cfg_ptr->collection_period*LINEOTN_FEC_STAT_COLLECTION_MS_200MHZ_TICK );        
        fecstat_field_FWI_INV_GL_CLK_set(b,lineotn_handle,fec_stat_intf_cfg_ptr->inv_polarity);
    }
    fecstat_field_FECSTAT_EN_set(b,lineotn_handle,enable);

    fecstat_buffer_flush(b);

    PMC_RETURN(rc);
} /*fec_stat_cfg*/


/*******************************************************************************
* fec_stat_logic_table_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  This function configures the FEC STAT logic table entry
*
* INPUTS:
*   *h              - pointer to LINEOTN handle instance
*   entry_id        - index to the entry
*   num_fec_source  - Number of FEC source. Usually 1, 3 for swizzle FEC 100G
*   fec_source_addr - FEC source addr, valid when number_fec_source is 1
*   llid            - logical lane id
*   phy_id          - physical lane if
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
* 
* NOTES:
*
*******************************************************************************/
PRIVATE void fec_stat_logic_table_cfg(lineotn_handle_t *h,
                                      UINT8 entry_id,
                                      UINT8 num_fec_source,
                                      UINT8 fec_source_addr,
                                      UINT8 llid,
                                      UINT8 phy_id)
{
    fecstat_buffer_t b[1];

    PMC_ENTRY();
    
    fecstat_buffer_init(b,h);

    fecstat_field_STAT_SOURCE_EN_set(b,h,entry_id,num_fec_source);
    if (LINEOTN_FEC_STAT_3_FEC_SOURCE == num_fec_source)
    {
        fecstat_field_STAT_SOURCE0_ADDR_set(b,h,entry_id,0x2);
        fecstat_field_STAT_SOURCE1_ADDR_set(b,h,entry_id,0x6);
        fecstat_field_STAT_SOURCE2_ADDR_set(b,h,entry_id,0xA);
    } 
    else
    {
        fecstat_field_STAT_SOURCE0_ADDR_set(b,h,entry_id,fec_source_addr);
    }    
    fecstat_field_LLID_set(b,h,entry_id,llid);
    fecstat_field_PHY_LN_set(b,h,entry_id,phy_id);

    fecstat_buffer_flush(b);

    PMC_RETURN();
} /* fec_stat_logic_table_cfg */


/*******************************************************************************
* fec_stat_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  This function perfoms register clean-up
*
* INPUTS:
*   *h              - pointer to LINEOTN handle instance
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void fec_stat_reg_cleanup(lineotn_handle_t        *h)
{
    UINT32 entry_itr;
    fecstat_buffer_t b[1];

    PMC_ENTRY();
    
    fecstat_buffer_init(b,h);
    
    for (entry_itr = 0; entry_itr < LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES; entry_itr++)             
    {
        if (LINEOTN_FEC_STAT_NULL_ENTRY == h->var.fec_stat_cfg.fsl_table[entry_itr])
        {
            /* update fec_row enable*/
            fecstat_lfield_range_FEC_ROW_EN_set(b,h,entry_itr, entry_itr,
                                                FALSE);
        }
    }
    
    fecstat_buffer_flush(b);

    PMC_RETURN();
} /* fec_stat_reg_cleanup */


/*******************************************************************************
* fec_pwr_mode_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Shutdown all FECs associated to a given dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to Lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - when operation is successful otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void fec_pwr_mode_cleanup(lineotn_handle_t  *lineotn_handle,
                                 UINT8              dsi_stream)
    
{
    UINT8       slice = 0;
    BOOL8       cleanup_slice[(UINT32)LAST_LINE_OTN_FEC];
    UINT8       itr;
    UINT32      jtr;
    BOOL8       one_stream_act = FALSE;
    UINT32      start_fec_type = ((UINT32)LINE_OTN_FEC_NONE + 1);
    UINT32      end_fec_type = ((UINT32)LAST_LINE_OTN_FEC);

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    
    slice = FEC_SLICE(dsi_stream);

    for (itr = 0; itr <(UINT32)LAST_LINE_OTN_FEC; itr++)
    {
        if (TRUE == lineotn_handle->var.lineotn_mode)
        {
            cleanup_slice[itr] = TRUE;
        }
        else
        {
            if (itr == (UINT32)LINE_OTN_FEC_G709)
            {
                cleanup_slice[itr] = TRUE;
            }
            else
            {
                cleanup_slice[itr] = FALSE;
            }
        }
    }

    /* ONLY G709 can be configured in SYSOTN ss */
    if (FALSE == lineotn_handle->var.lineotn_mode)
    {
        start_fec_type = ((UINT32)LINE_OTN_FEC_G709);
        end_fec_type = ((UINT32)LINE_OTN_FEC_G709 + 1);
    }

    for (jtr = start_fec_type; jtr < end_fec_type; jtr++)
    {
        PMC_LOG_TRACE("jtr = %d\n", jtr);
        for (itr = slice * LINEOTN_MAX_LANES_PER_OPSM23_SLICE; itr < ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE); itr++)
        {
            PMC_LOG_TRACE("jtr = %d itr = %d lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type = %d\n", jtr, itr, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type);
            if (dsi_stream != itr &&
                (((lineotn_fec_t)jtr) == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type))
            {
                PMC_LOG_TRACE("fec %d cannot e cleanup\n", jtr);
                cleanup_slice[jtr] = FALSE;
                one_stream_act = TRUE;
            }
        }
    }

    if (FALSE == one_stream_act)
    {
        /* fec_shared_ram_pwr_set(lineotn_handle,dsi_stream,fec_type, 1); */
        shared_ram_field_REGION_A_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
        shared_ram_field_REGION_B_LOWPWR_set(NULL,lineotn_handle, slice,0x3);
        shared_ram_field_REGION_C_LOWPWR_set(NULL,lineotn_handle, slice,0x7);
        shared_ram_field_REGION_D1_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
        shared_ram_field_REGION_D2_LOWPWR_set(NULL,lineotn_handle, slice,0x7);
        shared_ram_field_REGION_E_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
        shared_ram_field_REGION_F1_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
        shared_ram_field_REGION_F2_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
        shared_ram_field_REGION_F3_LOWPWR_set(NULL,lineotn_handle, slice,0xf);
    }
    
    if (TRUE == cleanup_slice[(UINT32)LINE_OTN_FEC_I4])
    {
        PMC_LOG_TRACE("I4 FEC low power mode set stream to cleanup = %d\n", dsi_stream);
        i4_fec_field_FEC_LOWPWR_set(NULL,lineotn_handle,slice, 1);
        i4_fec_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle,slice, 1);
    }
    
    if (TRUE == cleanup_slice[(UINT32)LINE_OTN_FEC_I7])
    { 
        i7_fec_field_FEC_SW_LOWPWR_set(NULL,lineotn_handle,slice,1);
        i7_fec_field_FEC_SW_RAM_LOWPWR_set(NULL,lineotn_handle,slice,1);
    }

    /* shut it down only if all fec are disabled */
    /* lineotn_handle->var.mld_curr_num_otu_configured bit 31-30 was re-used for 
       sysotn interrupt type, mask 0x3FFFFFFF to ignore bit 31-30 */
    if (0 == (lineotn_handle->var.mld_curr_num_otu_configured & SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE))
    {
        if(lineotn_handle->var.lineotn_mode)
        {
            gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,1);
            gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
            gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
        }
        else
        {
            sysotn_gfec_field_REG_SHUT_DOWN_set(NULL,lineotn_handle,1);
            sysotn_gfec_field_REG_DEEP_SLEEP_set(NULL,lineotn_handle,1);
            sysotn_gfec_field_GFEC_LOWPWR_set(NULL,lineotn_handle,1);
        }
    }

    if (TRUE == cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_100G])
    {
        if (0 == dsi_stream)
        {
            mfec_field_SWIZZLE100G_RAM_LOWPWR_set(NULL,lineotn_handle,1);
        }
    }

    if (TRUE == cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_40G])
    {
        swizzle7_40g_field_FEC_LOWPWR_set(NULL,lineotn_handle, slice, 1);
        swizzle7_40g_field_FEC_RAM_LOWPWR_set(NULL,lineotn_handle, slice, 1);
    }



    /* put in low power MFEC if no OTU stream are provisioned */
    /* lineotn_handle->var.mld_curr_num_otu_configured bit 31-30 was re-used for 
       sysotn interrupt type, mask 0x3FFFFFFF to ignore bit 31-30 */
    if (0 == (lineotn_handle->var.mld_curr_num_otu_configured & SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE))
    {
        if(TRUE == lineotn_handle->var.lineotn_mode)
        {
            mfec_field_MFEC_GLUE_LOWPWR_set(NULL,lineotn_handle,1);
        }
    }

    PMC_RETURN();
} /* fec_pwr_mode_cleanup */


/*******************************************************************************
* fec_dec_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns status of fec decoder. 
*
* INPUTS:
*   lineotn_handle            - pointer to LINEOTN handle instance
*   dsi_stream                - DSI stream number
*   fec_mode                  - FEC mode
*
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE - when fec associated to given fec mode is enables, otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 fec_dec_status_get(lineotn_handle_t  *lineotn_handle,
                                UINT32             dsi_stream,
                                lineotn_fec_t      fec_mode)
{
    BOOL8 rc = FALSE;
    UINT8 slice = 0;
    UINT32 decoder_40g_fec_sel;
    UINT32 rsfec_10g_sel;

    PMC_ENTRY();
    slice = FEC_SLICE(dsi_stream);

    if (0==slice) 
    {
        decoder_40g_fec_sel = mfec_field_DECODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle);
        rsfec_10g_sel = mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle);
    }
    else if (1==slice)
    {
        decoder_40g_fec_sel = mfec_field_DECODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
        rsfec_10g_sel = mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle);
    }
    else
    {
        decoder_40g_fec_sel = mfec_field_DECODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
        rsfec_10g_sel = mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle);
    }

    /* regarding fec_mode, check disable register value to determine fec status: ON or OFF */
    switch(fec_mode)
    {
    case LINE_OTN_FEC_I4:
        PMC_LOG_TRACE("in dec_status dsi_stream = %d disable mask = %x, %d\n", dsi_stream, 
                      i4_fec_field_DECODER_LN_DISABLE_get(NULL,lineotn_handle, dsi_stream / 4), 
                      i4_fec_field_range_DECODER_LN_DISABLE_get(NULL,lineotn_handle, dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 
                          dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE));

        if (0 == i4_fec_field_range_DECODER_LN_DISABLE_get(NULL,lineotn_handle, dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 
                                                           dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
        {
            rc = TRUE;
        }
        break;

    case LINE_OTN_FEC_I7:
        if (0 == i7_fec_field_FEC_SW_LOWPWR_get(NULL,lineotn_handle,slice) &&
            ((decoder_40g_fec_sel & 0x4) > 0) && ((rsfec_10g_sel & (1 << (dsi_stream % 4))) == 0))
        {
            rc = TRUE;
        }
        break;

    case LINE_OTN_FEC_G709:
        if(lineotn_handle->var.lineotn_mode)
        {                                        
            if (0 == ((0x1 << (dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE)) & gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
            {
                rc = TRUE;
            }
        }            
        else
        {
            if (0 == ((0x1 << (dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE)) & sysotn_gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
            {
                rc = TRUE;
            }
        }
        break;

    case LINE_OTN_FEC_SWIZZLE_100G:
        rc = TRUE;
        break;
        

    case LINE_OTN_FEC_SWIZZLE_40G: 
        if (0 == swizzle7_40g_field_DEC_DISABLE_get(NULL,lineotn_handle, dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
        {
            rc = TRUE;
        }    
        break;

    default:
        break;
    }

    PMC_RETURN(rc);
}/*fec_dec_status_get*/


/*******************************************************************************
* fec_stat_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve fec stat status from register space.
*
* INPUTS:
*   lineotn_handle            - pointer to LINEOTN handle instance
*
* OUTPUTS:
*   *fec_stat_cfg_ptr         - fec stat status of each DSI stream
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void fec_stat_cfg_get(lineotn_handle_t          *lineotn_handle,
                             lineotn_fec_stat_cfg_t    *fec_stat_cfg_ptr)
{
    UINT8       entry_itr;

    PMC_ENTRY();
    
    for (entry_itr = 0; entry_itr < LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES;entry_itr++)             
    {
        if ( 0 != fecstat_lfield_range_FEC_ROW_EN_get(NULL, lineotn_handle, entry_itr, entry_itr))
        {
            fec_stat_cfg_ptr->fsl_table[entry_itr] = 1;
            fec_stat_cfg_ptr->enable = TRUE;
        }
        else
        {            
            fec_stat_cfg_ptr->fsl_table[entry_itr] = LINEOTN_FEC_STAT_NULL_ENTRY;
        }
    }

    PMC_RETURN();
} /*fec_stat_cfg_get*/


/*******************************************************************************
* fec_config_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve fec configuration from the register space.
*
* INPUTS:
*   lineotn_handle            - pointer to LINEOTN handle instance
*   is_lineotn_ss             - when TRUE, LINEOTN SS is to be cleaned up, otherwise 
*                               it is a SYSOTN SS.
*
* OUTPUTS:
*   fec_type                  - fec type associated to each DSI stream.
*   fec_pwr                   - fec power mode associated to each DSI stream.
*   fec_bypassed              - fec bypass mode associated to each DSI stream.
*   fec_stat_enabled          - fec stat status associated to each DSI stream.
*   fec_dec_passthru          - fec decoder passthru status associated to each DSI stream.
*   fec_type_fuzzy            - Fuzzy state associated to fec type.
*
* RETURNS:
*   PMC_SUCCESS - when operation is successful otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fec_config_get(lineotn_handle_t       *lineotn_handle,
                                BOOL8                   is_lineotn_ss, 
                                lineotn_fec_t          *fec_type,
                                lineotn_power_mode_t   *fec_pwr,
                                BOOL8                  *fec_bypassed,
                                BOOL8                  *fec_stat_enabled,
                                BOOL8                  *fec_dec_passthru,
                                BOOL8                  *fec_type_fuzzy)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       itr;
    UINT8       jtr;
    UINT32      stream_end;
    BOOL8       stream_dec_passthru[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8 lowpwr;
    BOOL8 is_100g;
    BOOL8 is_40g;
    UINT32 fec_slice;
    UINT32 mfec_enc_val;
    UINT32 mfec_enc_type;
    BOOL8  is_gfec_10g;
    BOOL8  slice_fec_en;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    
    for (itr = 0; itr < lineotn_handle->cfg.max_dsi_streams; itr++)
    {
        fec_type_fuzzy[itr] = FALSE;
        fec_pwr[itr] = LINE_OTN_PWR_LOWPWR;
        fec_type[itr] = LAST_LINE_OTN_FEC;
        fec_stat_enabled[itr] = FALSE;
        fec_dec_passthru[itr] = TRUE; 
        fec_bypassed[itr] = FALSE;
    }

    for (itr = 0; itr < lineotn_handle->cfg.max_dsi_streams && PMC_SUCCESS == rc; itr++)
    {
        lowpwr = TRUE;        
        is_100g = FALSE;
        is_40g = FALSE;
        fec_slice = FEC_SLICE(itr);
        stream_end = itr;
        is_gfec_10g = FALSE;
        slice_fec_en = FALSE;
        mfec_enc_type = 0;
        mfec_enc_val = 0;

        PMC_LOG_TRACE("stream = %d ENCODER_40G_SLICE0_FEC_SEL = %x ENCODER_40G_SLICE0_RSFEC_10G_SEL = %x %x %x %x %x\n",
                      itr,
                      mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle),
                      mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle),
                      mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle),
                      mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle),
                      mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle),
                      mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle));

        if (TRUE == lineotn_handle->var.lineotn_mode &&
            0 == mfec_field_ENCODER_40G_100G_SEL_get(NULL,lineotn_handle) &&
            0 == mfec_field_DECODER_40G_100G_SEL_get(NULL,lineotn_handle))
        {
            switch(fec_slice)
            {
            case 0:                
                if (0 != mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle))
                {
                    slice_fec_en = TRUE;
                }
                if (0 != (0x8 & mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle)))
                {
                    is_gfec_10g = TRUE;
                    mfec_enc_val = 0x1 & (mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get(NULL,lineotn_handle) >> (itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE));
                }
                mfec_enc_type = mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL, lineotn_handle);
                break;
            case 1:
                if (0 != mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle))
                {
                    slice_fec_en = TRUE;
                }
                if (0 != (0x8 & mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle)))
                {
                    is_gfec_10g = TRUE;
                    mfec_enc_val = 0x1 & (mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get(NULL,lineotn_handle) >> (itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE));
                }
                mfec_enc_type = mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle);
                break;
            case 2:
                if (0 != mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle))
                {
                    slice_fec_en = TRUE;
                }
                if (0 != (0x8 & mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle)))
                {
                    is_gfec_10g = TRUE;
                    mfec_enc_val = 0x1 & (mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get(NULL,lineotn_handle) >> (itr % 4));
                }
                mfec_enc_type = mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle);
                break;
            default:
                mfec_enc_val = 0;
                break;
            }
        }

        for (jtr = 0; jtr < lineotn_handle->cfg.max_dsi_streams; jtr++)
        {
            stream_dec_passthru[jtr] = TRUE; 
        }

        PMC_LOG_TRACE("fec_slice = %d %d\n", fec_slice, itr);

        /*elaborate decision taking, use DECODER_OP_MODE, ENCODER_OP_MODE*/
        /*potential 10G port */
        PMC_LOG_TRACE("itr = %d %d %d %d %d mfec_enc_val = %d\n", itr, i4_fec_field_FEC_LOWPWR_get(NULL, lineotn_handle, fec_slice), 
                      i7_fec_field_FEC_SW_LOWPWR_get(NULL, lineotn_handle, fec_slice),
                      swizzle7_40g_field_FEC_LOWPWR_get(NULL, lineotn_handle, fec_slice),
                      gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle),
                      mfec_enc_val);
        
        if (TRUE == is_lineotn_ss)
        {
            if (LAST_LINE_OTN_FEC == fec_type[itr] &&
                0x0 == i4_fec_field_FEC_LOWPWR_get(NULL,lineotn_handle, fec_slice) &&
                (0 != (mfec_enc_type & 0x2)) &&
                (0 == mfec_enc_val) &&
                (0x0 == i4_fec_field_DECODER_OP_MODE_get(NULL,lineotn_handle, fec_slice) ||
                 0x0 == i4_fec_field_ENCODER_DATA_TYPE_get(NULL,lineotn_handle, fec_slice)))
            {
                PMC_LOG_TRACE("LINE_OTN_FEC_I4 -> itr = %d %x\n", itr, i4_fec_field_DECODER_LN_DISABLE_get(NULL,lineotn_handle, fec_slice));
                fec_type[itr] = LINE_OTN_FEC_I4;
                fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                lowpwr = 0;
                fec_type_fuzzy[itr] = TRUE;
            
                if (0 == i4_fec_field_range_DECODER_LN_DISABLE_get(NULL,lineotn_handle, fec_slice, itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
                {
                    fec_dec_passthru[itr] = FALSE;
                    PMC_LOG_TRACE("LINE_OTN_FEC_I4 - 2-> itr = %d  %d %x\n", itr, i4_fec_field_DECODER_LN_DISABLE_get(NULL,lineotn_handle, fec_slice), fec_dec_passthru[itr]);
                }
            }
        
            if (LAST_LINE_OTN_FEC == fec_type[itr] &&
                (0 != (mfec_enc_type & 0x4)) &&
                (0 == mfec_enc_val) &&
                0x0 == i7_fec_field_FEC_SW_LOWPWR_get(NULL,lineotn_handle, fec_slice) &&
                0x0 == i7_fec_field_DECODE_TYPE_get(NULL,lineotn_handle, fec_slice) &&
                0x0 == i7_fec_field_ENCODE_TYPE_get(NULL,lineotn_handle, fec_slice ))
            {
                PMC_LOG_TRACE("LINE_OTN_FEC_I7 -> itr = %d %x\n", itr, i4_fec_field_DECODER_LN_DISABLE_get(NULL,lineotn_handle, fec_slice));
                fec_type[itr] = LINE_OTN_FEC_I7;
                fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                lowpwr = 0;
                fec_type_fuzzy[itr] = TRUE;
                if (0 == i7_fec_field_range_DECODER_DISABLE_get(NULL,lineotn_handle, fec_slice, itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, itr % LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
                {
                    fec_dec_passthru[itr] = FALSE;
                }
            }
        

            if (LAST_LINE_OTN_FEC == fec_type[itr] &&
                0 == gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle) && 
                is_gfec_10g == TRUE &&
                0 != mfec_enc_val)
            {
                PMC_LOG_TRACE("LINE_OTN_FEC_G709 found\n");
                fec_type[itr] = LINE_OTN_FEC_G709; 
                fec_type_fuzzy[itr] = TRUE;   
                lowpwr = 0;        
                PMC_LOG_TRACE("is_lineotn_ss = %d\n", is_lineotn_ss);
                if(TRUE == is_lineotn_ss)
                {                                        
                    if (0 == ((0x1 << (itr)) & gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
                    {
                        fec_dec_passthru[itr] = FALSE;
                    }
                }            
                else
                {
                    if (0 != ((0x1 << (itr)) & sysotn_gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
                    {
                        fec_dec_passthru[itr] = FALSE;
                    }
                }
            }

            if (LAST_LINE_OTN_FEC == fec_type[itr] &&
                0 == (itr % 4) &&
                0 == gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle) && 
                is_gfec_10g == FALSE &&
                mfec_enc_type == 0x8)
            {
                PMC_LOG_TRACE("LINE_OTN_FEC_G709 found\n");
                fec_type[itr] = LINE_OTN_FEC_G709; 
                fec_type_fuzzy[itr] = TRUE;   
                lowpwr = 0;        
                PMC_LOG_TRACE("is_lineotn_ss = %d\n", is_lineotn_ss);
                if(TRUE == is_lineotn_ss)
                {                                        
                    if (0 == ((0xf << (itr)) & gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
                    {
                        fec_dec_passthru[itr] = FALSE;
                    }
                }            
                else
                {
                    if (0 != ((0xf << (itr)) & sysotn_gfec_field_REG_DISABLE_DEC_get(NULL,lineotn_handle)))
                    {
                        fec_dec_passthru[itr] = FALSE;
                    }
                }
            }

        } /*TRUE == is_lineotn_ss*/

        if (0 == itr && LAST_LINE_OTN_FEC == fec_type[itr])
        {    
            if(TRUE == is_lineotn_ss)
            {    
                if (0 != mfec_field_ENCODER_40G_100G_SEL_get(NULL,lineotn_handle) && 
                    0 != mfec_field_DECODER_40G_100G_SEL_get(NULL,lineotn_handle))
                {
                    if (0 == gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle))
                    {
                        fec_type[itr] = LINE_OTN_FEC_G709;
                        fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                        lowpwr = 0;
                        is_100g = TRUE;
                    }
                }
                if (0 == mfec_field_SWIZZLE100G_RAM_LOWPWR_get(NULL,lineotn_handle)  &&
                    0 != mfec_field_SWIZZLE_SEL_100G_40G_get(NULL, lineotn_handle) &&
                    0 == swizzle7_40g_field_FEC_LOWPWR_get(NULL,lineotn_handle, 0) &&
                    0 == swizzle7_40g_field_FEC_LOWPWR_get(NULL,lineotn_handle, 1) &&
                    0 == swizzle7_40g_field_FEC_LOWPWR_get(NULL,lineotn_handle, 2))
                {
                    fec_type[itr] = LINE_OTN_FEC_SWIZZLE_100G;
                    fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                    fec_dec_passthru[itr] = FALSE;
                    lowpwr = 0;
                    is_100g = TRUE;
                }
               
            }
            else
            {
                if (0 == sysotn_gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle))
                {
                    fec_type[itr] = LINE_OTN_FEC_G709;
                    fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                    fec_dec_passthru[itr] = FALSE;
                    lowpwr = 0;
                    is_100g = TRUE;
                }
            }
        }

            
        PMC_LOG_TRACE("itr = %d, fec_type = %d\n", itr, fec_type[itr]);
        if (TRUE == is_lineotn_ss &&
            0 == (itr % 4) && 
            LAST_LINE_OTN_FEC == fec_type[itr])
        {
            PMC_LOG_TRACE("itr = %d %d %d %d %d\n", itr, i4_fec_field_FEC_LOWPWR_get(NULL, lineotn_handle, fec_slice),
                          i7_fec_field_FEC_SW_LOWPWR_get(NULL, lineotn_handle, fec_slice),
                          swizzle7_40g_field_FEC_LOWPWR_get(NULL, lineotn_handle, fec_slice),
                          gfec_field_GFEC_LOWPWR_get(NULL, lineotn_handle));
                
            /* case LINE_OTN_FEC_I4: */
            if (0 == i4_fec_field_FEC_LOWPWR_get(NULL, lineotn_handle, fec_slice) &&
                ((0 != i4_fec_field_DECODER_OP_MODE_get(NULL, lineotn_handle, fec_slice) ||  
                  0 != i4_fec_field_ENCODER_DATA_TYPE_get(NULL,lineotn_handle, fec_slice))))
            {
                fec_type[itr] = LINE_OTN_FEC_I4;
                fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                lowpwr = 0;
                is_40g = TRUE;
                PMC_LOG_TRACE("LINE_OTN_FEC_I4 -> itr = %d\n", itr);
                /* potential 40G port */
                if (0 == i4_fec_field_range_DECODER_LN_DISABLE_get(NULL,lineotn_handle, fec_slice, 3, 3))
                {
                    for (jtr = itr; jtr < (itr + LINEOTN_MAX_LANES_PER_OPSM23_SLICE); jtr++)
                    {
                        stream_dec_passthru[jtr] = FALSE;
                    }
                }
            }

            /* case LINE_OTN_FEC_I7: */
            if (0 == i7_fec_field_FEC_SW_LOWPWR_get(NULL, lineotn_handle, fec_slice) &&
                (0 != i7_fec_field_DECODE_TYPE_get(NULL, lineotn_handle, fec_slice) ||
                 0 != i7_fec_field_ENCODE_TYPE_get(NULL, lineotn_handle, fec_slice)))
            {
                PMC_LOG_TRACE("LINE_OTN_FEC_I7 -> itr = %d\n", itr);
                fec_type[itr] = LINE_OTN_FEC_I7;
                fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                lowpwr = 0;
                is_40g = TRUE;                
                if (0 == i7_fec_field_range_DECODER_DISABLE_get(NULL,lineotn_handle, fec_slice, 3, 3))
                {
                    for (jtr = itr; jtr < (itr + LINEOTN_MAX_LANES_PER_OPSM23_SLICE); jtr++)
                    {
                        stream_dec_passthru[jtr] = FALSE;
                    }
                }
            }

            if (LAST_LINE_OTN_FEC == fec_type[itr] &&
                0 != mfec_field_SWIZZLE100G_RAM_LOWPWR_get(NULL,lineotn_handle) &&
                0 == swizzle7_40g_field_FEC_LOWPWR_get(NULL,lineotn_handle, fec_slice))
            {

                PMC_LOG_TRACE("stream = %d swizzle40g = %d %x\n", jtr, swizzle7_40g_field_FEC_LOWPWR_get(NULL,lineotn_handle, fec_slice), swizzle7_40g_field_DEC_DISABLE_get(NULL,lineotn_handle, fec_slice));
                fec_type[itr] = LINE_OTN_FEC_SWIZZLE_40G;
                fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                lowpwr = 0;
                is_40g = TRUE;

                if (0 == swizzle7_40g_field_DEC_DISABLE_get(NULL,lineotn_handle, fec_slice))
                {
                    for (jtr = itr; jtr < (itr + LINEOTN_MAX_LANES_PER_OPSM23_SLICE); jtr++)
                    {
                        stream_dec_passthru[jtr] = FALSE;
                    }
                }
            }

            
        }
        
        /* retrieve fec bypass status*/
        if (is_lineotn_ss)
        {
            if (0 != ((1 << itr) & mfec_field_ENCODER_BYPASS_get(NULL,lineotn_handle)) &&
                0 != (((1 << itr) & mfec_field_DECODER_BYPASS_get(NULL,lineotn_handle))))
            {
                fec_bypassed[itr] = TRUE;
            }                
        }        
        else
        {
            if (0 != sysotn_field_BYPASS_GFEC_RX_get(NULL, lineotn_handle) &&
                0 != sysotn_field_BYPASS_GFEC_TX_get(NULL, lineotn_handle))                
            {
                fec_bypassed[itr] = TRUE;
            }
        }
        /* special procedure to determine 40G and 100G stream power mode and decoder passthru status */
        if (TRUE == is_40g || TRUE == is_100g)
        {
            for (jtr = itr; jtr < (itr + (TRUE == is_40g ? LINEOTN_MAX_LANES_PER_OPSM23_SLICE : LINE_OTN_MAX_DSI_STREAMS)); jtr++)
            {
                if (TRUE == is_100g)
                {
                    fec_dec_passthru[jtr] = FALSE;
                    fec_pwr[jtr] = fec_pwr[0];
                }
                else
                {            
                    fec_type_fuzzy[jtr] = fec_type_fuzzy[itr];
                    fec_dec_passthru[jtr] = stream_dec_passthru[itr];
                }
            }
        }
    
        /* retrieve fec stat status */
        if (0 != otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, itr) ||
            (1 == otn_mld_field_MUX13_SYS_STREAM_get(NULL,lineotn_handle) &&
             0x2 == otn_mld_field_MUX7_LINE_STREAM_get(NULL,lineotn_handle)) ||
            0x2 == otn_mld_field_MUX11_LINE_STREAM_get(NULL,lineotn_handle))
        {
            if (0 == mfec_field_MFEC_GLUE_LOWPWR_get(NULL,lineotn_handle))
            {          
                if (TRUE == slice_fec_en)
                {
                    fec_pwr[itr] = LINE_OTN_PWR_OPERATIONAL;
                }

                PMC_LOG_TRACE("step 0-0 itr = %d %d %d \n", itr, LINE_OTN_PWR_OPERATIONAL, fec_type[itr]);
                /* to elaborate */
                if (0 != fecstat_field_FECSTAT_EN_get(NULL, lineotn_handle))
                {
                    fec_stat_enabled[itr] = TRUE;
                }
            }  
        }              
    }

    PMC_RETURN(rc);
} /* fec_config_get */


/*******************************************************************************
*  lineotn_fec_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable LINEOTN FEC specified dsi streams.
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
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_fec_reg_cleanup(lineotn_handle_t    *lineotn_handle,
                                    UINT32               num_dsi_streams, 
                                    UINT32              *dsi_streams)
{
    UINT32 itr;

    PMC_ENTRY();

    for (itr = 0; itr < num_dsi_streams; itr++)
    {
        PMC_LOG_TRACE("in %s clean up stream %d\n", __FUNCTION__, dsi_streams[itr]);

        if (TRUE == lineotn_handle->var.lineotn_mode)
        {
            fec_i4_cfg_cleanup(lineotn_handle, dsi_streams[itr]);
            
            fec_i7_cfg_cleanup(lineotn_handle, dsi_streams[itr]);
            
            if (0 == dsi_streams[itr])
            {
                PMC_LOG_TRACE("about to call fec_swizzle100_cfg_cleanup\n ");
                fec_swizzle100_cfg_cleanup(lineotn_handle, dsi_streams[itr]);
            }
            if (0 == (dsi_streams[itr] % LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
            {
                PMC_LOG_TRACE("about to call fec_swizzle40_cfg_cleanup\n ");
                fec_swizzle40_cfg_cleanup(lineotn_handle, dsi_streams[itr]);            
            }
            
            
            fec_mfec_cleanup(lineotn_handle, dsi_streams[itr]); 
        }
            
        PMC_LOG_TRACE("about to call gfec_fec_gfec_cfg_cleanup\n ");
        fec_gfec_cfg_cleanup(lineotn_handle, dsi_streams[itr]);

        /* cleanup fec power mode */
        fec_pwr_mode_cleanup(lineotn_handle, dsi_streams[itr]);/* low power  mode */
    }

    /* cleanup fec stat register space */
    if(TRUE == lineotn_handle->var.lineotn_mode)
    {
        fec_stat_reg_cleanup(lineotn_handle);
        
        /* cleanup mfec selector */
        fec_mfec_sel_cleanup(lineotn_handle); 
    }

    PMC_RETURN();
}/* lineotn_fec_reg_cleanup */


/*******************************************************************************
* fec_mfec_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reset MFEC registers associated to a given stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_mfec_cleanup(lineotn_handle_t   *lineotn_handle,
                              UINT8               dsi_stream)
{
    UINT8       slice;

    PMC_ENTRY();

    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);
    
    /* read modify write - RSFEC can be selected with any other 40G fec */
    switch (slice)
    {
    case 0:
        mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % 4, dsi_stream % 4, 0);
        mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % 4, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 0);
        break;
    case 1:
        mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 0);
        mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 0);
        break;
    case 2:
        mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 0);
        mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set(NULL,lineotn_handle, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, dsi_stream % LINEOTN_MAX_LANES_PER_OPSM23_SLICE, 0);
        break;
    default:
        break;
    }

    /* encoder and decoder bypass mode */
    mfec_field_range_ENCODER_BYPASS_set(NULL,lineotn_handle,dsi_stream,dsi_stream, 1);
    mfec_field_range_DECODER_BYPASS_set(NULL,lineotn_handle,dsi_stream,dsi_stream, 1);

    /* mark the current 10G dsi stream as not provisioned  */
    lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_type = LAST_LINE_OTN_FEC;
    lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;

    PMC_RETURN();

}/* fec_mfec_cleanup */


/*******************************************************************************
* fec_i4_cfg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a I4 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_i4_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                UINT8              dsi_stream )
{
    UINT8          slice = 0;
    UINT8          itr;    

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12 ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);

    /* I4 block */
    UINT8 lane = (dsi_stream % 4);
    i4_fec_field_range_DECODER_LN_DISABLE_set(NULL,lineotn_handle, slice, lane, lane, 1); /*disable error correction for the 10G lane */
    
    i4_fec_field_DECODER_OP_MODE_set(NULL,lineotn_handle, slice, 0); /*disable error correction for the 10G lane */
    
    i4_fec_field_ENCODER_DATA_TYPE_set(NULL,lineotn_handle, slice, 0);

    for (itr = (slice * LINEOTN_MAX_LANES_PER_OPSM23_SLICE); itr < ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE); itr++)
    {
        if (TRUE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_provisioned &&
            LINE_OTN_FEC_I4 == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type)
        {
            break;
        }
    }

    if (itr == ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
    {
        i4_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1);  /* reset */
    }

    PMC_RETURN();
}/* fec_i4_cfg_cleanup */


/*******************************************************************************
* fec_swizzle40_cfg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up a swizzle 40G fec register associated to a given 
*   stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_swizzle40_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                       UINT8               dsi_stream)
{
    UINT8           slice = 0;

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    slice = FEC_SLICE(dsi_stream);

    /* swizzle7 40G block */
    swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,slice,1); /* disable decoder */
    swizzle7_40g_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1); /*set to reset */

    PMC_RETURN();
}/* fec_swizzle40_cfg_cleanup */


/*******************************************************************************
* fec_i7_cfg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a I7 fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             for 10G configurations (0..11),
*                             for 40G configurations (0,4,8),
*                             for 100G configurations (0)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_i7_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                UINT8              dsi_stream )
{
    UINT8           slice = 0;
    UINT8           itr;    

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12 ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    slice = FEC_SLICE(dsi_stream);

    
    for (itr = (slice * LINEOTN_MAX_LANES_PER_OPSM23_SLICE); itr < ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE); itr++)
    {
        if (TRUE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_provisioned &&
            LINE_OTN_FEC_I7 == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type)
        {
            break;
        }
    }

    if (itr == ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE))
    {
        i7_fec_field_FEC_SW_RST_set(NULL,lineotn_handle,slice,1);  /* reset */
    }
    
    PMC_RETURN();
}/* fec_i7_cfg_cleanup */






/*******************************************************************************
* fec_gfec_cfg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a gfec fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_gfec_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                  UINT8              dsi_stream)
{

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < lineotn_handle->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);   

    /* read modify write */
    /* remove 40G config from the enc/dec type */
    UINT32 enc_dec_reg_val = gfec_field_REG_TX_ENCODE_TYPE_get(NULL,lineotn_handle);
    UINT32 enc_dec_rmv_bm = ~(1 << (dsi_stream / LINEOTN_MAX_LANES_PER_OPSM23_SLICE));
    

    if(TRUE == lineotn_handle->var.lineotn_mode)
    {
        gfec_field_range_REG_DISABLE_DEC_set(NULL,lineotn_handle, dsi_stream, dsi_stream, 1);
        /* it cannot be a 100G stream anymore */
        gfec_field_range_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle, 3, 3, 0);
        gfec_field_range_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle, 3, 3, 0);
        
        /* make sure dsi_stream is the right boundaries for 40G cases */
        if (dsi_stream == 0 || 
            dsi_stream == 4 ||
            dsi_stream == 8)
        {        
            enc_dec_reg_val &= enc_dec_rmv_bm;
            
            /* set new config */
            gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle,enc_dec_reg_val);
            gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle,enc_dec_reg_val);
            
            /* update depending on number of stream enable in slice */
            /* lineotn_handle->var.mld_curr_num_otu_configured bit 31-30 was 
               re-used for sysotn interrupt type, mask 0x3FFFFFFF to ignore bit 31-30 */
            if (0 == (lineotn_handle->var.mld_curr_num_otu_configured & SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE))
            {
                if(TRUE == lineotn_handle->var.lineotn_mode)
                {
                    gfec_field_FEC_SW_RST_set(NULL,lineotn_handle,1);  /* set to reset */
                }
            }
        }
    }
    else
    {
        if (0 == dsi_stream)
        {
            sysotn_gfec_field_REG_DISABLE_DEC_set(NULL,lineotn_handle, 0x800);
            sysotn_gfec_field_REG_TX_ENCODE_TYPE_set(NULL,lineotn_handle, 0);
            sysotn_gfec_field_REG_RX_DECODE_TYPE_set(NULL,lineotn_handle, 0);

            sysotn_field_BYPASS_GFEC_RX_set(NULL, lineotn_handle, 1);
            sysotn_field_BYPASS_GFEC_TX_set(NULL, lineotn_handle, 1);
            sysotn_gfec_field_FEC_SW_RST_set(NULL,lineotn_handle, 1);  /* set to reset */            
        }
    }

    PMC_RETURN();
}/* fec_gfec_cfg_cleanup */


/*******************************************************************************
* fec_swizzle100_cfg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully validates and de-provisions a swizzle 100G fec per requested
*   already provisioned dsi stream.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*   dsi_stream              - dsi stream to cleanup.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_swizzle100_cfg_cleanup(lineotn_handle_t   *lineotn_handle,
                                        UINT8               dsi_stream)
{
    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);


    /* lineotn_handle->var.mld_curr_num_otu_configured bit 31-30 was re-used for 
       sysotn interrupt type, mask 0x3FFFFFFF to ignore bit 31-30 */
    if (0 == dsi_stream &&
        0 == (lineotn_handle->var.mld_curr_num_otu_configured & SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE))
    {
        /* disable decoder for all participating 40G slices */
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,0,1);
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,1,1);
        swizzle7_40g_field_DEC_DISABLE_set(NULL,lineotn_handle,2,1);
    }
    /* Disable Frame Slip Monitor if FEC type is Swizzle 100G */
    (void)lineotn_swizzle_fec_frame_slip_monitor_cfg(lineotn_handle, FALSE); 
    
    PMC_RETURN();
}/* fec_swizzle100_cfg_cleanup */

/*******************************************************************************
* fec_mfec_sel_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will reset the mfec FEC selector depending on provisioned streams.
*
* INPUTS:
*   lineotn_handle          - pointer to lineotn handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*  None.
*
* NOTES:
*******************************************************************************/
PRIVATE void fec_mfec_sel_cleanup(lineotn_handle_t *lineotn_handle)
{
    UINT8       slice;
    BOOL8       cleanup_slice[(UINT32)LAST_LINE_OTN_FEC];
    UINT8       itr;
    UINT32      jtr;
    BOOL8       is_100g_prov = FALSE;
    UINT32      start_fec_type = ((UINT32)LINE_OTN_FEC_NONE + 1);
    UINT32      end_fec_type = ((UINT32)LAST_LINE_OTN_FEC);
    UINT32      max_slices = LINEOTN_MAX_NUM_40G_SLICES;        
    

    PMC_ENTRY();
    PMC_ASSERT(lineotn_handle!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* ONLY G709 can be configured in SYSOTN ss */
    if (FALSE == lineotn_handle->var.lineotn_mode)
    {
        start_fec_type = ((UINT32)LINE_OTN_FEC_G709);
        end_fec_type = ((UINT32)LINE_OTN_FEC_G709 + 1);
        max_slices = 1;
    }

    /* for each slice, check FEC used by any provisioned stream */
    /* if one fec is not used then disable fec selector associated to FEC type */
    for (slice = 0; slice < max_slices; slice += 1)
    {
        /* check if there is at least */
        /* collect fec usage status */
        for (jtr = start_fec_type; jtr < end_fec_type; jtr++)
        {
            cleanup_slice[jtr] = TRUE;

            PMC_LOG_TRACE("jtr = %d\n", jtr);
            for (itr = slice * LINEOTN_MAX_LANES_PER_OPSM23_SLICE; itr < ((slice + 1) * LINEOTN_MAX_LANES_PER_OPSM23_SLICE) && 
                     TRUE == cleanup_slice[jtr]; itr++)
            {
                PMC_LOG_TRACE("jtr = %d itr = %d lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type = %d\n", jtr, itr, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type);
                if ((((lineotn_fec_t)jtr) == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type) &&
                    (TRUE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_provisioned))
                {
                    PMC_LOG_TRACE("fec %d cannot e cleanup stream = %d, slice = %d\n", jtr, itr, slice);
                    cleanup_slice[jtr] = FALSE;
                    if (LINE_OTN_RATE_100G ==  lineotn_stream_rate_get(lineotn_handle->var.lineotn_dsi_dist[itr].dsi_rate))
                    {
                        is_100g_prov = TRUE;
                    }
                }
            }
        }
        
        /* disable FEC selector for non-used FEC */
        switch (slice)
        {
        case 0:
            PMC_LOG_TRACE("clean slice 0\n");
            for (jtr = start_fec_type; jtr < end_fec_type; jtr++)
            {             
                if (TRUE == cleanup_slice[jtr])
                {
                    switch((lineotn_fec_t)jtr)
                    {
                    case LINE_OTN_FEC_I4:
                        mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                        mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                        PMC_LOG_TRACE("clean up I4 ENCODER_40G_SLICE0_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                        break;
                    case LINE_OTN_FEC_I7:
                        PMC_LOG_TRACE("clean up I7 ENCODER_40G_SLICE0_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                        mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                        mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                        break;
                    case LINE_OTN_FEC_G709:
                        PMC_LOG_TRACE("clean up G709 ENCODER_40G_SLICE0_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                        mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                        mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                        
                        break;
                    case LINE_OTN_FEC_SWIZZLE_100G:
                    case LINE_OTN_FEC_SWIZZLE_40G:
                        if (cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_100G] ==
                            cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_40G])
                        {
                            PMC_LOG_TRACE("clean up SWIZZLE ENCODER_40G_SLICE0_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                            mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                        }
                        break;
                    default :
                        break;
                    }
                    PMC_LOG_TRACE("after cleanup ENCODER_40G_SLICE0_FEC_SEL = %x\n", mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get(NULL,lineotn_handle));
                }
            }            

            break;
        case 1:
            PMC_LOG_TRACE("clean slice 1\n");
            if (0 != mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle))
            {    
                for (jtr = start_fec_type; jtr < end_fec_type; jtr++)                
                { 
                    if (TRUE == cleanup_slice[jtr])
                    {
                        switch((lineotn_fec_t)jtr)
                        {
                        case LINE_OTN_FEC_I4:
                            mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                            mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                            PMC_LOG_TRACE("clean up I4 ENCODER_40G_SLICE1_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            break;
                        case LINE_OTN_FEC_I7:
                            PMC_LOG_TRACE("clean up I7 ENCODER_40G_SLICE1_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                            mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                            break;
                        case LINE_OTN_FEC_G709:
                            PMC_LOG_TRACE("clean up G709 ENCODER_40G_SLICE1_FEC_SEL %x\n",mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                            mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                            break;
                        case LINE_OTN_FEC_SWIZZLE_100G:
                        case LINE_OTN_FEC_SWIZZLE_40G:
                            if (cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_100G] ==
                                cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_40G])
                            {
                                PMC_LOG_TRACE("clean up SWIZZLE ENCODER_40G_SLICE1_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                                mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                                mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                            }
                            break;
                        default :
                            break;
                        }
                    }
                } 
            }
            break;
        case 2:
            PMC_LOG_TRACE("clean slice 2\n");
            if (0 != mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle))
            {
                for (jtr = start_fec_type; jtr < end_fec_type; jtr++)                
                { 
                    if (TRUE == cleanup_slice[jtr])
                    {
                        switch((lineotn_fec_t)jtr)
                        {
                        case LINE_OTN_FEC_I4:
                            mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                            mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 1, 1, 0);
                            PMC_LOG_TRACE("clean up I4 ENCODER_40G_SLICE2_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            break;
                        case LINE_OTN_FEC_I7:
                            PMC_LOG_TRACE("clean up I7 ENCODER_40G_SLICE2_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                            mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 2, 2, 0);
                            break;
                        case LINE_OTN_FEC_G709:
                            PMC_LOG_TRACE("clean up G709 ENCODER_40G_SLICE2_FEC_SEL%x\n", mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                            mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                            mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 3, 3, 0);
                            break;
                        case LINE_OTN_FEC_SWIZZLE_100G:
                        case LINE_OTN_FEC_SWIZZLE_40G:
                            if (cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_100G] ==
                                cleanup_slice[(UINT32)LINE_OTN_FEC_SWIZZLE_40G])
                            {
                                PMC_LOG_TRACE("clean up SWIZZLE ENCODER_40G_SLICE2_FEC_SEL %x\n", mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle, 1,1));
                                mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                                mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set(NULL,lineotn_handle, 4, 4, 0);
                            }
                            break;
                        default :
                            break;
                        }
                        PMC_LOG_TRACE("after cleanup ENCODER_40G_SLICE2_FEC_SEL = %x\n", mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get(NULL,lineotn_handle));
                    }
                } 
            }
            break;
        default:
            break;
        }
    } /* end for slice*/

    if (TRUE != is_100g_prov)
    {
        mfec_field_ENCODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
        mfec_field_DECODER_100G_FEC_SEL_set(NULL,lineotn_handle,0);
    }

    PMC_RETURN();
}/* fec_mfec_sel_cleanup */


