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
*     This file contains C functions for operating PGMRCLK features.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi.h"
#include "digi_loc.h"
#include "digi_pgmrclk_io.h"
#include "util_global.h"

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

/*
** Forward References
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_pgmrclk_div_internal_cfg(digi_handle_t *digi_handle,
                                                digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                                BOOL8 enable);

PRIVATE PMC_ERROR digi_pgmrclk_div_disable(digi_handle_t *digi_handle,
                                           digi_pgmrclk_div_id_t div_id);

PRIVATE PMC_ERROR digi_pgmrclk_div_params_compute(digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                             digi_pgmrclk_div_params_t *div_params);

PRIVATE PMC_ERROR digi_pgmrclk_clk_source_check(digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr);

PRIVATE INT32 digi_pgmrclk_sel_value_get(digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr);

PRIVATE PMC_ERROR digi_pgmrclk_ctxt_audit(digi_handle_t         *digi_handle,
                                          digi_pgmrclk_ctxt_t    *ctxt_clean_ptr);

PRIVATE PMC_ERROR digi_pgmrclk_ctxt_cleanup(digi_handle_t          *digi_handle,
                                            digi_pgmrclk_ctxt_t     *ctxt_clean_ptr);
PRIVATE PMC_ERROR digi_pgmrclk_fpi_internal_cfg(digi_handle_t *digi_handle,
                                                digi_pgmrclk_fpi_cfg_t *pgmrclk_fpi_cfg_ptr);

#endif /*DOXYGEN_PUBLIC */
/*
** Public Functions
*/

/*
**  Exported Interface Functions - Start
*/

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures and enable or disable one clock divider.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   pgmrclk_cfg_ptr          - DIGI PGMRCLK config for one clock divider
*   enable               - when TRUE clock divider is enable
*                           when FALSE it is disabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - when clock source and clock index are correct
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*   DIGI_ERR_INVALID_ARG   - when clock divider id, or, clock source, or
*                              clock source index are invali
*
* NOTES:
*  If enable is TRUE, the function enables all PGMRCLK dividers when there are not,
*  If enable is FALSE, the functon disabled all PGMRCLK dividers when all
*   divider output clocks are disabled.
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_div_cfg(digi_handle_t *digi_handle,
                                      digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                      BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pgmrclk_div_internal_cfg(digi_handle,pgmrclk_cfg_ptr,enable);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pgmrclk_div_cfg */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PGMRCLK configuration of one clock divider.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   div_id               - DIGI PGMR clock dividers identifier. Refer to
*                          enumeration digi_pgmrclk_div_id_t.
*
* OUTPUTS:
*   pgmrclk_cfg_ptr      - digi_pgmrclk_div_cfg_t for clock divider
*   enable               - Divider enable (TRUE) or disable (FALSE)
*
* RETURNS:
*   PMC_SUCCESS            
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_div_cfg_get(digi_handle_t          *digi_handle,
                                          digi_pgmrclk_div_id_t  div_id,
                                          digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                          BOOL8                  *enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(div_id < LAST_DIGI_PGMRCLK_DIV, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(pgmrclk_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(enable != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    PMC_MEMCPY(pgmrclk_cfg_ptr, &(digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[div_id]), sizeof(digi_pgmrclk_div_cfg_t));
    *enable = digi_handle->var.pgmrclk_ctxt.valid[div_id];

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pgmrclk_div_cfg_get */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_demapper_source_ctxt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the context required to call digi_pgmrclk_div_cfg()
*   for a demapped client.\n\n
*
*   The demapped client must be using the ENET_LINE ETRANS path in order for
*   the returned data to be valid.  If incorrect MAPOTN channel handle mode,
*   an error code is returned.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   *chnl_handle         - pointer to MAPOTN channel handle instance
*
* OUTPUTS:
*   clk_source_ptr       - digi_pgmrclk_input_source_t for provided channel handle
*   clk_source_idx_ptr   - digi_pgmrclk_input_source_t index associated with
*                          channel handle
*
* RETURNS:
*   PMC_SUCCESS            - on success otherwise meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_demapper_source_ctxt_get(digi_handle_t *digi_handle,
                                                       util_global_switch_data_t *chnl_handle,
                                                       digi_pgmrclk_input_source_t *clk_source_ptr,
                                                       UINT32 *clk_source_idx_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Confirm MAPOTN Channel handle type */
    result = digi_mapper_chnl_handle_validate(digi_handle,
                                              chnl_handle);

    /* Retrieve the enet handle & link */
    if(PMC_SUCCESS == result)
    {
        result = digi_enet_validate_parms(digi_handle, chnl_handle,
                                          &enet_handle, &link);
    }

    if(PMC_SUCCESS == result)
    {
        *clk_source_ptr = DIGI_PGMRCLK_DEMAP_ENET;
        *clk_source_idx_ptr = link;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pgmrclk_demapper_source_ctxt_get */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_fpi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the FPI Generator.
*   This function must be called after digi_device_init().
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   pgmrclk_fpi_cfg_ptr  - DIGI FPI config
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_fpi_cfg(digi_handle_t *digi_handle,
                                      digi_pgmrclk_fpi_cfg_t *pgmrclk_fpi_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pgmrclk_fpi_internal_cfg(digi_handle,pgmrclk_fpi_cfg_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pgmrclk_fpi_cfg */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_output_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enable or disable one clock divider output to the PGMRCLK
*   device pin.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   div_id               - DIGI PGMR clock dividers identifier. Refer to
*                          enumeration digi_pgmrclk_div_id_t.
*   enable               - when TRUE it is enabled
*                          when FALSE it is disabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - when clock source and clock index are correct
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*   DIGI_ERR_INVALID_ARG   - when clock divider id is invalid
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_div_output_cfg(digi_handle_t *digi_handle,
                                             digi_pgmrclk_div_id_t div_id,
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 div_en_val = 0;
    UINT32 mask = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(div_id < LAST_DIGI_PGMRCLK_DIV, DIGI_ERR_INVALID_ARG, 0, 0);

    div_en_val = digi_pgmrclk_field_FDIV_OUT_ENABLE_get(NULL, digi_handle);
    mask = (UINT32)(1 << (UINT32)div_id);

    if(TRUE == enable) {
        if(div_en_val & mask) {
            /* Already enabled, return SUCCESS */
            PMC_ATOMIC_RETURN(digi_handle,result);
        }
        div_en_val = (div_en_val | mask) & 0xf;
    } else {
        if(div_en_val & mask) {
            div_en_val = (div_en_val & (~mask)) & 0xf;
        } else {
            /* Already disabled, return SUCCESS */
            PMC_ATOMIC_RETURN(digi_handle,result);
        }
    }

    if(PMC_SUCCESS == result) {
        digi_pgmrclk_field_FDIV_OUT_ENABLE_set(NULL, digi_handle, div_en_val);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pgmrclk_div_output_cfg */


/*
 *
 * private functions
 *
 */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_internal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures and enable or disable one clock divider.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   pgmrclk_cfg_ptr          - DIGI PGMRCLK config for one clock divider
*   enable               - when TRUE clock divider is enable
*                           when FALSE it is disabled
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - when clock source and clock index are correct
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*   DIGI_ERR_INVALID_ARG   - when clock divider id, or, clock source, or
*                              clock source index are invali
*
* NOTES:
*  If enable is TRUE, the function enables all PGMRCLK dividers when there are not,
*  If enable is FALSE, the functon disabled all PGMRCLK dividers when all
*   divider output clocks are disabled.
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_div_internal_cfg(digi_handle_t *digi_handle,
                                               digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                               BOOL8 enable)
{
    UINT32 div_en_val = 0;
    UINT32 div_out_en_val = 0;
    UINT32 div_reset_val = 0;
    INT32 clk_sel_val;
    digi_pgmrclk_div_params_t div_params;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pgmrclk_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Enable or disable clock divider */
    if (enable == TRUE)
    {
        /* Check that the input source clock exists */
        result = digi_pgmrclk_clk_source_check(pgmrclk_cfg_ptr);

        if (PMC_SUCCESS == result)
        {
            result = digi_pgmrclk_div_disable(digi_handle, pgmrclk_cfg_ptr->div_id);
        }

        /* Get clock select value to write in register*/
        if (PMC_SUCCESS == result )
        {
            clk_sel_val = digi_pgmrclk_sel_value_get(pgmrclk_cfg_ptr);
            if (clk_sel_val < 0)
            {
                result = DIGI_ERR_INVALID_ARG;
            }
        }
        if (PMC_SUCCESS == result )
        {

            /* Compute clock divider parameters (n, d, and integer) */
            result = digi_pgmrclk_div_params_compute(pgmrclk_cfg_ptr, &div_params);

            if (PMC_SUCCESS == result )
            {
                /* Configure device */
                /* Set divider selected source, integer, numerator, and denominator */
                switch (pgmrclk_cfg_ptr->div_id)
                {
                case DIGI_PGMRCLK_DIV_0:
                    digi_pgmrclk_field_FDIV_CLK_MUX_SEL_0_set(NULL, digi_handle, clk_sel_val);
                    digi_pgmrclk_field_FDIV_INTEGER_0_set(NULL, digi_handle, div_params.div_integer);
                    digi_pgmrclk_field_FDIV_NUMERATOR_0_set(NULL, digi_handle, div_params.div_n);
                    digi_pgmrclk_field_FDIV_DENOMINATOR_0_set(NULL, digi_handle, div_params.div_d);
                    break;
                case DIGI_PGMRCLK_DIV_1:
                    digi_pgmrclk_field_FDIV_CLK_MUX_SEL_1_set(NULL, digi_handle, clk_sel_val);
                    digi_pgmrclk_field_FDIV_INTEGER_1_set(NULL, digi_handle, div_params.div_integer);
                    digi_pgmrclk_field_FDIV_NUMERATOR_1_set(NULL, digi_handle, div_params.div_n);
                    digi_pgmrclk_field_FDIV_DENOMINATOR_1_set(NULL, digi_handle, div_params.div_d);
                    break;
                case DIGI_PGMRCLK_DIV_2:
                    digi_pgmrclk_field_FDIV_CLK_MUX_SEL_2_set(NULL, digi_handle, clk_sel_val);
                    digi_pgmrclk_field_FDIV_INTEGER_2_set(NULL, digi_handle, div_params.div_integer);
                    digi_pgmrclk_field_FDIV_NUMERATOR_2_set(NULL, digi_handle, div_params.div_n);
                    digi_pgmrclk_field_FDIV_DENOMINATOR_2_set(NULL, digi_handle, div_params.div_d);
                    break;
                case DIGI_PGMRCLK_DIV_3:
                    digi_pgmrclk_field_FDIV_CLK_MUX_SEL_3_set(NULL, digi_handle, clk_sel_val);
                    digi_pgmrclk_field_FDIV_INTEGER_3_set(NULL, digi_handle, div_params.div_integer);
                    digi_pgmrclk_field_FDIV_NUMERATOR_3_set(NULL, digi_handle, div_params.div_n);
                    digi_pgmrclk_field_FDIV_DENOMINATOR_3_set(NULL, digi_handle, div_params.div_d);
                    break;
                default:
                    /* divider unknown */
                    result = DIGI_ERR_INVALID_ARG;
                    break;
                }

                if (PMC_SUCCESS == result)
                {
                    /* Enable fractional divider */
                    div_en_val = digi_pgmrclk_field_FDIV_ENABLE_get(NULL, digi_handle);
                    div_en_val |= (1 << (UINT32)pgmrclk_cfg_ptr->div_id) & 0xf;
                    digi_pgmrclk_field_FDIV_ENABLE_set(NULL, digi_handle, div_en_val);

                    /* Take fractional divider out of reset */
                    div_reset_val =  digi_pgmrclk_field_FDIV_SW_RST_get(NULL, digi_handle);
                    div_reset_val &= ~(1 << (UINT32)pgmrclk_cfg_ptr->div_id) & 0xf;
                    digi_pgmrclk_field_FDIV_SW_RST_set(NULL, digi_handle, div_reset_val);

                    /* Enable output of fractional divider */
                    div_out_en_val = digi_pgmrclk_field_FDIV_OUT_ENABLE_get(NULL, digi_handle);
                    div_out_en_val |= (1 << (UINT32)pgmrclk_cfg_ptr->div_id) & 0xf;
                    digi_pgmrclk_field_FDIV_OUT_ENABLE_set(NULL, digi_handle, div_out_en_val);

                    PMC_MEMCPY(&(digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[pgmrclk_cfg_ptr->div_id]), pgmrclk_cfg_ptr, sizeof(digi_pgmrclk_div_cfg_t));
                    digi_handle->var.pgmrclk_ctxt.valid[pgmrclk_cfg_ptr->div_id] = TRUE;
                }
            }
        }
    }
    else
    {
        result = digi_pgmrclk_div_disable(digi_handle, pgmrclk_cfg_ptr->div_id);

        if (PMC_SUCCESS == result)
        {
            /* Update context */
            digi_handle->var.pgmrclk_ctxt.valid[pgmrclk_cfg_ptr->div_id] = FALSE;
        }
    }

    PMC_RETURN(result);
}/* digi_pgmrclk_div_internal_cfg */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function disables a specific PGMR clock divider.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   div_id               - PGMR clock divider ID
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - when clock source and clock index are correct
*
* NOTES:
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_div_disable(digi_handle_t *digi_handle,
                                           digi_pgmrclk_div_id_t div_id)
{
    UINT32 div_en_val = 0;
    UINT32 div_out_en_val = 0;
    UINT32 div_reset_val = 0;


    PMC_ENTRY();

    /* Disable output of fractional divider */
    div_out_en_val = digi_pgmrclk_field_FDIV_OUT_ENABLE_get(NULL, digi_handle);
    div_out_en_val &= ~(1 << (UINT32)div_id) & 0xf;
    digi_pgmrclk_field_FDIV_OUT_ENABLE_set(NULL, digi_handle, div_out_en_val);

    /* Place fractional divider in reset */
    div_reset_val =  digi_pgmrclk_field_FDIV_SW_RST_get(NULL, digi_handle);
    div_reset_val |= (1 << (UINT32)div_id) & 0xf;
    digi_pgmrclk_field_FDIV_SW_RST_set(NULL, digi_handle, div_reset_val);

    /* Disable fractional divider */
    div_en_val = digi_pgmrclk_field_FDIV_ENABLE_get(NULL, digi_handle);
    div_en_val &= ~(1 << (UINT32)div_id) & 0xf;
    digi_pgmrclk_field_FDIV_ENABLE_set(NULL, digi_handle, div_en_val);

    PMC_RETURN(PMC_SUCCESS);
}
/* digi_pgmrclk_div_disable */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_div_params_compute
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Computes PGMRCLK divider parameters: integer, n, and d according to
*    input clock frequency and desired output clock frequency.
*
* INPUTS:
*   pgmrclk_cfg_ptr      - pointer to cfg structure to contain computed
*                          values
*
* OUTPUTS:
*  *div_params           - divider computed parameters: integer, n, d
*
* RETURNS:
*   PMC_SUCCESS - always returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_div_params_compute(digi_pgmrclk_div_cfg_t    *pgmrclk_cfg_ptr,
                                                  digi_pgmrclk_div_params_t *div_params)
{
    PMC_ERROR rc = PMC_SUCCESS;
    DOUBLE div_fract;

    PMC_ENTRY();
    PMC_ASSERT(pgmrclk_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(div_params != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pgmrclk_cfg_ptr->output_clk_freq != 0.0, DIGI_ERR_PGMRCLK_DIV_BY_ZERO, 0, 0);

    /* Compute the integer value*/
    div_params->div_integer  = (UINT32) (pgmrclk_cfg_ptr->input_clk_freq / pgmrclk_cfg_ptr->output_clk_freq);

    /* Compute the fractional value */
    div_fract =  ((DOUBLE)pgmrclk_cfg_ptr->input_clk_freq / pgmrclk_cfg_ptr->output_clk_freq) - div_params->div_integer;

    /* Fractional Divider Restrictions :
    **  -Numerator must be less that the denominator
    ** Also ensure div_integer <= 0xFFFFFFFF
    */
    if(div_fract > 1) {
        rc = DIGI_ERR_PGMRCLK_RATE_NOT_SUPPORTED;
        PMC_RETURN(rc);
    }

    /* When there is a fractional value convert the fractional value in a decimal to get the divider numerator and denominator */
    if (div_fract != 0) {
        util_global_fract_to_dec_conv_max_num(div_fract, 0x7FFFFFFF, &div_params->div_n, &div_params->div_d);
    }
    else
    {
        div_params->div_n = 0;
        div_params->div_d = 1;
    }

    /* check that the integer part is alway greater than 0 */
    if (div_params->div_integer == 0)
    {
        rc = DIGI_ERR_PGMRCLK_RATE_NOT_SUPPORTED;
    } else
    {
        /* Fractional Divider Restrictions :
        ** a) Fractional division of 1.X is not supported (e.g. 1.75, 1.26, 1.33, etc.), integer division of 1 is supported (passthrough mode).
        ** b) Fractional division of 4.X is not supported (e.g. 4.15, 4.39, 4.21, etc.), integer division of 4 is supported.
        */
        if(((div_params->div_integer == 1) || (div_params->div_integer == 4))
            && (div_params->div_n != 0)) {
            rc = DIGI_ERR_PGMRCLK_RATE_NOT_SUPPORTED;
        }

        /* FDIV_INTEGER_1 configures the integer portion of the Fractional Divider. Register value must be set to be between 0x1 and 0xFFFFFFFF for normal operation. */
        /* In normal mode of operation, the clock selected by FDIV_CLK_MUX_SEL_1 is divided by: */
        /* [(FDIV_INTEGER_1 + 1) + (FDIV_NUMERATOR_1 / FDIV_DENOMINATOR_1)] */
        /* For pass through mode of operation (divide by 1), set FDIV_INTEGER_1 = 0x0, FDIV_NUMERATOR_1 = 0x0 , and FDIV_DENOMINATOR_1 = 0x1. */
        /* Every time this register is reconfigured the pmgrclk output must be first disabled used the corresponding FDIV_OUTPUT_EN bit. */

        div_params->div_integer -= 1;
    }

    PMC_RETURN(rc);
}/* digi_pgmrclk_div_params_compute */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_clk_source_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks that clock source and clock index index value.
*
* INPUTS:
*   pgmrclk_cfg_ptr          - DIGI PGMRCLK config for one clock divider
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - when clock source and clock index are correct
*   DIGI_ERR_INVALID_ARG   - when clkock source is unknown or clock index
*                            exceed clock source maximum number of clocks
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_clk_source_check(digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr)
{

    PMC_ENTRY();

    PMC_ASSERT(pgmrclk_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Check that clock source and clock source index are valid */
    switch (pgmrclk_cfg_ptr->clk_source)
    {
    case (DIGI_PGMRCLK_SFI_1_DSC_TX ):
    case (DIGI_PGMRCLK_SFI_1_DSC_RX ):
    case (DIGI_PGMRCLK_SFI_2_DSC_TX ):
    case (DIGI_PGMRCLK_SFI_2_DSC_RX ):
        break;
    case (DIGI_PGMRCLK_SFI_1_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SFI_1_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SFI_1_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SFI_1_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SFI_2_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SFI_2_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SFI_2_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SFI_2_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_LINE_IF_TX ) :
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_LINE_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_LINE_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_LINE_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SYS_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SYS_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SYS_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SYS_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_MGMT_IF_TX ):
    case (DIGI_PGMRCLK_MGMT_IF_RX ):
        break;
    case (DIGI_PGMRCLK_PCIE_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_PCIE_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_PCIE_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_PCIE_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SR_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SR_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SR_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_SR_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_LR_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_LR_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_LR_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_LR_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_SYS1_DCSU_PCLK ):
    case (DIGI_PGMRCLK_SYS1_DCSU_REF300_CLK ):
    case (DIGI_PGMRCLK_SYS1_DCSU_REF600_CLK ):
    case (DIGI_PGMRCLK_SYS2_DCSU_REF500_CLK ):
    case (DIGI_PGMRCLK_ENET1_DCSU_REF653_CLK ):
    case (DIGI_PGMRCLK_ENET2_DCSU_REF515_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF349_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF311_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF155_CLK ):
    case (DIGI_PGMRCLK_OTN2_DCSU_REF355_CLK ):
    case (DIGI_PGMRCLK_PTP_CLK ):
        break;
    case (DIGI_PGMRCLK_RCP_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_RCP_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_RCP_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_RCP_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_OTN_OH_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_OTN_OH_IF_RX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_OTN_OH_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_OTN_OH_IF_TX_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_DEMAP_ENET):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_DEMAP_ENE_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_INTERNAL_CLK ):
        if (pgmrclk_cfg_ptr->clk_source_idx >= DIGI_PGMRCLK_INTERNAL_CLK_NUM_CLKS)
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;
    case (DIGI_PGMRCLK_DEV_IF_REFCLK ):
    case (DIGI_PGMRCLK_ENET3_DCSU_REF518_CLK ):
    case (DIGI_PGMRCLK_ENET3_DCSU_REF388_CLK ):
    case (DIGI_PGMRCLK_IRIG_IF_IRIG_CLKI ):
    case (DIGI_PGMRCLK_IRIG_IF_IRIG_CLKO ):
    case (DIGI_PGMRCLK_SPI_IF_SPI_SCLK ):
    case (DIGI_PGMRCLK_SYS2_DCSU_DIV10_REF50_CLK ):
        break;
    default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);

    }

    PMC_RETURN(PMC_SUCCESS);
}/* digi_pgmrclk_clk_source_check */


/*******************************************************************************
* FUNCTION: digi_pgmrclk_clk_source_value_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the select value corresponding to clock source
*    and clock index.
*
* INPUTS:
*   pgmrclk_cfg_ptr          - DIGI PGMRCLK config for one clock divider
*
* OUTPUTS:
*   None.
*
* RETURNS:
*  INT32 >= 0  - Clock source select value to write in congiguration register.
*  INT32 == -1 - invalid select value is returned when clock source is incorrect.
*
* NOTES:
*
*******************************************************************************/
PRIVATE INT32 digi_pgmrclk_sel_value_get(digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr)
{
    BOOL8 sel_val_found = TRUE;
    BOOL8 use_index = FALSE;
    INT32 result = -1;

    PMC_ENTRY();

    PMC_ASSERT(pgmrclk_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Verify that clock source index should be used to compute select value */
    switch (pgmrclk_cfg_ptr->clk_source)
    {
    case (DIGI_PGMRCLK_SFI_1_DSC_TX ):
    case (DIGI_PGMRCLK_SFI_1_DSC_RX ):
    case (DIGI_PGMRCLK_SFI_2_DSC_TX ):
    case (DIGI_PGMRCLK_SFI_2_DSC_RX ):
        break;

    case (DIGI_PGMRCLK_SFI_1_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SFI_1_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SFI_1_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SFI_1_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SFI_2_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SFI_2_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SFI_2_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SFI_2_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_LINE_IF_TX ) :
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_LINE_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_LINE_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_LINE_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SYS_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SYS_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SYS_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SYS_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_MGMT_IF_TX ):
    case (DIGI_PGMRCLK_MGMT_IF_RX ):
        break;
    case (DIGI_PGMRCLK_PCIE_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_PCIE_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_PCIE_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_PCIE_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SR_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SR_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SR_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_SR_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_LR_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_LR_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_LR_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_LR_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_SYS1_DCSU_PCLK ):
    case (DIGI_PGMRCLK_SYS1_DCSU_REF300_CLK ):
    case (DIGI_PGMRCLK_SYS1_DCSU_REF600_CLK ):
    case (DIGI_PGMRCLK_SYS2_DCSU_REF500_CLK ):
    case (DIGI_PGMRCLK_ENET1_DCSU_REF653_CLK ):
    case (DIGI_PGMRCLK_ENET2_DCSU_REF515_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF349_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF311_CLK ):
    case (DIGI_PGMRCLK_OTN1_DCSU_REF155_CLK ):
    case (DIGI_PGMRCLK_OTN2_DCSU_REF355_CLK ):
    case (DIGI_PGMRCLK_PTP_CLK ):
        break;

    case (DIGI_PGMRCLK_RCP_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_RCP_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_RCP_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_RCP_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_OTN_OH_IF_RX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_OTN_OH_IF_RX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_OTN_OH_IF_TX ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_OTN_OH_IF_TX_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_DEMAP_ENET):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_DEMAP_ENE_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_INTERNAL_CLK ):
        if (pgmrclk_cfg_ptr->clk_source_idx < DIGI_PGMRCLK_INTERNAL_CLK_NUM_CLKS)
        {
            use_index = TRUE;
        } else
        {
            sel_val_found = FALSE;
        }
        break;
    case (DIGI_PGMRCLK_DEV_IF_REFCLK ):
    case (DIGI_PGMRCLK_ENET3_DCSU_REF518_CLK ):
    case (DIGI_PGMRCLK_ENET3_DCSU_REF388_CLK ):
    case (DIGI_PGMRCLK_IRIG_IF_IRIG_CLKI ):
    case (DIGI_PGMRCLK_IRIG_IF_IRIG_CLKO ):
    case (DIGI_PGMRCLK_SPI_IF_SPI_SCLK ):
    case (DIGI_PGMRCLK_SYS2_DCSU_DIV10_REF50_CLK ):
        break;
    default:
        sel_val_found = FALSE;
    }

    /* Compute select value */
    if (sel_val_found == TRUE)
    {
        result = (INT32) pgmrclk_cfg_ptr->clk_source;
        if (use_index == TRUE)
        {
            result += (INT32) pgmrclk_cfg_ptr->clk_source_idx;
        }
    }

    PMC_RETURN(result);
}/* digi_pgmrclk_sel_value_get */

/*******************************************************************************
* digi_pgmrclk_div_all_disable
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This disables all the pgmrclk_divs by calling each through a loop.
*   Note, should a failure occur, this is only logged and it progresses to the
*   next pgmrclk.
*
* INPUTS:
*  *digi_handle   - Pointer to the digi handle instance.
*
* OUTPUTS:
*  None
*
* RETURNS:
*  None
*
* NOTES:
*  None
*******************************************************************************/
PUBLIC void digi_pgmrclk_div_all_disable(digi_handle_t *digi_handle)
{
    digi_pgmrclk_div_id_t div_id;

    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    for (div_id = DIGI_PGMRCLK_DIV_0; div_id < LAST_DIGI_PGMRCLK_DIV; div_id = (digi_pgmrclk_div_id_t)((UINT32)div_id + 1))
    {
        if (digi_handle->var.pgmrclk_ctxt.valid[div_id] == TRUE) {

            rc = digi_pgmrclk_div_internal_cfg(digi_handle, &(digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[div_id]), FALSE);

            if (rc != PMC_SUCCESS) {

                /* Log failure, but continue to try and unconfigure the next pgmclk */
                PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_PGMRCLK_COULD_NOT_UNCONFIG, div_id, 0);
            }
        }
    }

    PMC_RETURN();

} /* digi_pgmrclk_div_all_disable */

/*******************************************************************************
* digi_pgmrclk_restart_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup PGMRCLK context at digi top layer and in block level.
*
* INPUTS:
*  *digi_handle            - Pointer to the digi handle instance.
*  *restart_init_cfg_ptr   - pointer to restart init config
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned.
*
* NOTES:
*  None
*******************************************************************************/
PUBLIC PMC_ERROR digi_pgmrclk_restart_init(digi_handle_t                  *digi_handle,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_pgmrclk_ctxt_t *ctxt_clean_ptr;

    PMC_ENTRY();

    ctxt_clean_ptr = (digi_pgmrclk_ctxt_t *) PMC_CALLOC(sizeof(digi_pgmrclk_ctxt_t));

    /*
       Recover context from register space
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only enabled CLOCK configruation are recovered
       - anyway a partial CLOCK config is an error in warm restart and will be cleanup in crash-restarted
       - incoherency in register context means that the CLOCK shall be disabled
    */

    /*
     * Audit register context and memory context
     *   - Create a new context that represents the intersection between memory and register space (ctxt_clean_ptr)
     */
    rc = digi_pgmrclk_ctxt_audit(digi_handle,
                                 ctxt_clean_ptr);
    PMC_LOG_TRACE("pgmrclk_ctxt_audit rc = %s\n", pmc_log_string_get(rc));

    if (DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /*
         * Cleanup
         *   - At the end, pgmrclk_ctxt is the new context
         *    - We keep enabled CLOCK defined in ctxt_clean_ptr
         *    - We clean (deactivate/deprov) all CLOCKs that are not enabled
         *       - they may have be partially enabled
         */
        rc = digi_pgmrclk_ctxt_cleanup(digi_handle,
                                       ctxt_clean_ptr);
    }

    PMC_FREE(&ctxt_clean_ptr);

    PMC_RETURN(rc);

} /* digi_pgmrclk_restart_init */


/*******************************************************************************
* digi_pgmrclk_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits DIGI_PGMRCLK handle context variable against register space content.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI_PGMRCLK handle instance.
*   *ctxt_clean_ptr        - pointer to DIGI_PGMRCLK context.
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
PRIVATE PMC_ERROR digi_pgmrclk_ctxt_audit(digi_handle_t         *digi_handle,
                                          digi_pgmrclk_ctxt_t   *ctxt_clean_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR loop_rc = PMC_SUCCESS;
    UINT32 div_id = DIGI_PGMRCLK_DIV_0;
    BOOL8 div_en, div_en_out;
    UINT32 div_en_out_word, div_en_word;
    INT32 clk_sel_val_exp;
    INT32 clk_sel_val_act;
    digi_pgmrclk_div_params_t div_params_exp;
    digi_pgmrclk_div_params_t div_params_act;

    PMC_ENTRY();

    div_en_out_word = digi_pgmrclk_field_FDIV_OUT_ENABLE_get(NULL, digi_handle);
    div_en_word = digi_pgmrclk_field_FDIV_ENABLE_get(NULL, digi_handle);

    for (div_id = 0; div_id < (UINT32)LAST_DIGI_PGMRCLK_DIV; div_id += 1)
    {
        loop_rc = PMC_SUCCESS;

        div_en_out = (0 == ((div_en_out_word >> div_id) & 0x1)) ? FALSE : TRUE;
        div_en = (0 == ((div_en_word >> div_id) & 0x1)) ? FALSE : TRUE;

        if (digi_handle->var.pgmrclk_ctxt.valid[div_id] != div_en_out)
        {
            loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
        }

        if(PMC_SUCCESS == loop_rc)
        {
            if (TRUE == div_en_out)
            {
                /* if output is enabled, the fdiv must be enabled */
                if (FALSE == div_en)
                {
                    loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
                }

                if(PMC_SUCCESS == loop_rc)
                {
                    clk_sel_val_exp = digi_pgmrclk_sel_value_get(&digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[div_id]);
                    if (clk_sel_val_exp < 0)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_INCOHERENT;
                    }
                }

                if(PMC_SUCCESS == loop_rc)
                {
                    /* Compute clock divider parameters (n, d, and integer) */
                    loop_rc = digi_pgmrclk_div_params_compute(&digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[div_id], &div_params_exp);

                    if (PMC_SUCCESS != loop_rc)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_INCOHERENT;
                    }
                }

                if(PMC_SUCCESS == loop_rc)
                {
                    /* retrieve clock parameters from register space */
                    switch ((digi_pgmrclk_div_id_t)div_id)
                    {
                    case DIGI_PGMRCLK_DIV_0:
                        clk_sel_val_act = digi_pgmrclk_field_FDIV_CLK_MUX_SEL_0_get(NULL, digi_handle);
                        div_params_act.div_integer = digi_pgmrclk_field_FDIV_INTEGER_0_get(NULL, digi_handle);
                        div_params_act.div_n = digi_pgmrclk_field_FDIV_NUMERATOR_0_get(NULL, digi_handle);
                        div_params_act.div_d = digi_pgmrclk_field_FDIV_DENOMINATOR_0_get(NULL, digi_handle);
                        break;
                    case DIGI_PGMRCLK_DIV_1:
                        clk_sel_val_act = digi_pgmrclk_field_FDIV_CLK_MUX_SEL_1_get(NULL, digi_handle);
                        div_params_act.div_integer = digi_pgmrclk_field_FDIV_INTEGER_1_get(NULL, digi_handle);
                        div_params_act.div_n = digi_pgmrclk_field_FDIV_NUMERATOR_1_get(NULL, digi_handle);
                        div_params_act.div_d = digi_pgmrclk_field_FDIV_DENOMINATOR_1_get(NULL, digi_handle);
                        break;
                    case DIGI_PGMRCLK_DIV_2:
                        clk_sel_val_act = digi_pgmrclk_field_FDIV_CLK_MUX_SEL_2_get(NULL, digi_handle);
                        div_params_act.div_integer = digi_pgmrclk_field_FDIV_INTEGER_2_get(NULL, digi_handle);
                        div_params_act.div_n = digi_pgmrclk_field_FDIV_NUMERATOR_2_get(NULL, digi_handle);
                        div_params_act.div_d = digi_pgmrclk_field_FDIV_DENOMINATOR_2_get(NULL, digi_handle);
                        break;
                    case DIGI_PGMRCLK_DIV_3:
                        clk_sel_val_act = digi_pgmrclk_field_FDIV_CLK_MUX_SEL_3_get(NULL, digi_handle);
                        div_params_act.div_integer = digi_pgmrclk_field_FDIV_INTEGER_3_get(NULL, digi_handle);
                        div_params_act.div_n = digi_pgmrclk_field_FDIV_NUMERATOR_3_get(NULL, digi_handle);
                        div_params_act.div_d = digi_pgmrclk_field_FDIV_DENOMINATOR_3_get(NULL, digi_handle);
                        break;
                    default:
                        break;
                    }

                    if (clk_sel_val_exp != clk_sel_val_act)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("digi_pgmrclk audit -> clk_sel_val incorrect for clk_id %d\n", div_id);
                    }
                    if (div_params_exp.div_integer != div_params_act.div_integer)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("digi_pgmrclk audit -> div_interger incorrect for clk_id %d\n", div_id);
                    }
                    if (div_params_exp.div_n != div_params_act.div_n)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("digi_pgmrclk audit -> div_n incorrect for clk_id %d\n", div_id);
                    }
                    if (div_params_exp.div_d != div_params_act.div_d)
                    {
                        loop_rc = DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("digi_pgmrclk audit -> div_d incorrect for clk_id %d\n", div_id);
                    }
                }
            }
        }

        if (PMC_SUCCESS == rc)
        {
            rc = loop_rc;
        }
        else
        {
            /* save valid configuration in clean context */
            if (TRUE == div_en)
            {
                ctxt_clean_ptr->valid[div_id] = TRUE;
                PMC_MEMCPY((void *)&ctxt_clean_ptr->pgmrclk_cfg[div_id],
                           (void *)&digi_handle->var.pgmrclk_ctxt.pgmrclk_cfg[div_id],
                           sizeof(digi_pgmrclk_div_cfg_t));
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_pgmrclk_ctxt_audit */

/*******************************************************************************
* digi_pgmrclk_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up DIGI_PGMRCLK handle context.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI_PGMRCLK handle instance.
*   *ctxt_clean_ptr        - pointer to DIGI_PGMRCLK clean context.
*
* OUTPUTS:
*   None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_ctxt_cleanup(digi_handle_t          *digi_handle,
                                            digi_pgmrclk_ctxt_t    *ctxt_clean_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 div_id;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* disable uncertain clocks to leave register space in clean state */
    for (div_id = 0; div_id < (UINT32)LAST_DIGI_PGMRCLK_DIV && PMC_SUCCESS == rc; div_id += 1)
    {
        if (FALSE == ctxt_clean_ptr->valid[div_id])
        {
            ctxt_clean_ptr->pgmrclk_cfg[div_id].div_id = (digi_pgmrclk_div_id_t)div_id;
            rc = digi_pgmrclk_div_internal_cfg(digi_handle,
                                               &ctxt_clean_ptr->pgmrclk_cfg[div_id],
                                               FALSE) ;
        }
    }

    /* copy clean context in digi_handle context */
    PMC_MEMCPY((void*)&digi_handle->var.pgmrclk_ctxt, (void *)ctxt_clean_ptr, sizeof(digi_pgmrclk_ctxt_t));

    PMC_RETURN(rc);
} /* digi_pgmrclk_ctxt_cleanup */

/*******************************************************************************
* FUNCTION: digi_pgmrclk_fpi_internal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the FPI Generator.
*
* INPUTS:
*   digi_handle          - pointer to DIGI handle instance
*   pgmrclk_fpi_cfg_ptr  - DIGI FPI config
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_NULL_HANDLE   - when function's arguments are NULL
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pgmrclk_fpi_internal_cfg(digi_handle_t *digi_handle,
                                                digi_pgmrclk_fpi_cfg_t *pgmrclk_fpi_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 fpi_sync_i = 0;
    UINT32 misc_fpi_8 = 0;
    UINT32 retry_count = 0;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pgmrclk_fpi_cfg_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    if( pgmrclk_fpi_cfg_ptr->mask_window_8 > 0xF ||  pgmrclk_fpi_cfg_ptr->misc_fpi_8_bit_0 > 1) {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    switch(pgmrclk_fpi_cfg_ptr->cfg_action) {
    case DIGI_PGMRCLK_FPI_CFG_ACTION_DISABLE_RESYNC:
        /* Disable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x1);

        /* Disable windowing */
        digi_pgmrclk_field_CNTR_ENABLE_8_set(NULL, digi_handle, 0x1);

        /* Clear FPI_SYNC_I */
        digi_pgmrclk_field_FPI_SYNC_I_set_to_clear(NULL, digi_handle, 1);

        /* Enable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x0);

        /* Check the FPI_SYNC_I status. */
        do {
            fpi_sync_i = digi_pgmrclk_field_FPI_SYNC_I_get(NULL, digi_handle);
            if(fpi_sync_i == 0)
            {
                PMC_Q_USLEEP(digi_handle,125000);
                retry_count ++;
            }
        } while ((fpi_sync_i == 0) && (retry_count < 3));

        if(retry_count == 3)
        {   
            /* If no synchronization event, return error. */
            PMC_RETURN(DIGI_ERR_PGMRCLK_FPI_NOT_SYNCHRONIZED);
        }

        /* Disable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x1);

        break;
    case DIGI_PGMRCLK_FPI_CFG_ACTION_ENABLE_RESYNC:
        /* Disable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x1);

        /* Enable windowing */
        digi_pgmrclk_field_CNTR_ENABLE_8_set(NULL, digi_handle, 0x0);

        /* Enable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x0);

        break;
    case DIGI_PGMRCLK_FPI_CFG_ACTION_RESIZE_MASK_WINDOW:
        /* Disable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x1);

        /* Disable windowing */
        digi_pgmrclk_field_CNTR_ENABLE_8_set(NULL, digi_handle, 0x1);

        /* Disable FPI */
        digi_pgmrclk_field_FPI_OUT_EN_set(NULL, digi_handle, 0x0);

        /* Configure MISC_FPI_8 bit 0 field */
        misc_fpi_8 = digi_pgmrclk_field_MISC_FPI_8_get(NULL, digi_handle);
        if(pgmrclk_fpi_cfg_ptr->misc_fpi_8_bit_0 == 0) {
            misc_fpi_8 &= 0x6;
        } else {
            misc_fpi_8 |= 0x1;
        }
        digi_pgmrclk_field_MISC_FPI_8_set(NULL, digi_handle, misc_fpi_8);

        /* Configure MASK_WINDOW_8 field */
        digi_pgmrclk_field_MASK_WINDOW_8_set(NULL, digi_handle, pgmrclk_fpi_cfg_ptr->mask_window_8);

        /* Enable FPI */
        digi_pgmrclk_field_FPI_OUT_EN_set(NULL, digi_handle, 0x1);

        /* Enable windowing */
        digi_pgmrclk_field_CNTR_ENABLE_8_set(NULL, digi_handle, 0x0);

        /* Enable synchronization */
        digi_pgmrclk_field_SYNC_DISABLE_8_set(NULL, digi_handle, 0x0);

        break;
    default:
        result = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_RETURN(result);
} /* digi_pgmrclk_fpi_internal_cfg */


#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** End of file
*/

