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
*   DESCRIPTION: 
*     The file describes the misc Run-Time functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_adpt.h"

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
** Local Functions
*/

/*
** Public Functions
*/
/*******************************************************************************
* adpt_pgmrclk_div_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures and enables/disables one clock divider.
*
* INPUTS:
*   dev_id             - device identity.
*   div_id             - divider ID between 0 and 3.
*   clk_source         - divider clock source.
*   clk_source_idx     - divider clock source index.
*   input_clk_freq     - source clock frequency.
*   output_clk_freq    - output clock desired frequency.
*   enable             - TRUE: enable clock divider.
*                        FALSE: disable clock divider.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   For example to select source clock SFI5.1 TX[2] of clock divider 3:
*   div_id = 3, clk_source = DIGI_PGMRCLK_SFI_1_TX and clk_source_idx = 2
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pgmrclk_div_cfg(UINT32 dev_id,
                                   digi_pgmrclk_div_id_t div_id,
                                   digi_pgmrclk_input_source_t clk_source,
                                   UINT32 clk_source_idx,
                                   DOUBLE input_clk_freq,
                                   DOUBLE output_clk_freq,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pgmrclk_div_cfg_t pgmrclk_cfg_ptr;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%lf][%lf][%d].\r\n", dev_id, div_id, 
               clk_source, clk_source_idx, input_clk_freq, output_clk_freq, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    pgmrclk_cfg_ptr.div_id          = div_id;
    pgmrclk_cfg_ptr.clk_source      = clk_source;
    pgmrclk_cfg_ptr.clk_source_idx  = clk_source_idx;
    pgmrclk_cfg_ptr.input_clk_freq  = input_clk_freq;
    pgmrclk_cfg_ptr.output_clk_freq = output_clk_freq;
    rc = digi_pgmrclk_div_cfg(adpt_handle->digi_handle,
                              &pgmrclk_cfg_ptr,
                              (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_pgmrclk_div_cfg */


/*******************************************************************************
* adpt_pgmrclk_div_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves PGMRCLK configuration of one clock divider.
*
* INPUTS:
*   dev_id             - device identity.
*   div_id             - divider ID between 0 and 3.
*
* OUTPUTS:
*   *clk_source        - divider clock source.
*   *clk_source_idx    - divider clock source index.
*   *input_clk_freq    - source clock frequency.
*   *output_clk_freq   - output clock desired frequency.
*   *enable            - divider enable (TRUE) or disable (FALSE).
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pgmrclk_div_cfg_get(UINT32 dev_id,
                                      digi_pgmrclk_div_id_t div_id,
                                      digi_pgmrclk_input_source_t *clk_source,
                                      UINT32 *clk_source_idx,
                                      DOUBLE *input_clk_freq,
                                      DOUBLE *output_clk_freq,
                                      BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pgmrclk_div_cfg_t pgmrclk_cfg_ptr;
    BOOL8 div_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(clk_source);
    ADPT_PARA_VERIFY(clk_source_idx);
    ADPT_PARA_VERIFY(input_clk_freq);
    ADPT_PARA_VERIFY(output_clk_freq);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_pgmrclk_div_cfg_get(adpt_handle->digi_handle,
                                  div_id,
                                  &pgmrclk_cfg_ptr,
                                  &div_en);

    *clk_source      = pgmrclk_cfg_ptr.clk_source;
    *clk_source_idx  = pgmrclk_cfg_ptr.clk_source_idx;
    *input_clk_freq  = pgmrclk_cfg_ptr.input_clk_freq;
    *output_clk_freq = pgmrclk_cfg_ptr.output_clk_freq;
    *enable          = div_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_pgmrclk_div_cfg_get */


/*******************************************************************************
* adpt_pgmrclk_output_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables one clock divider output to the PGMRCLK device pin.
*
* INPUTS:
*   dev_id             - device identity.
*   div_id             - divider ID between 0 and 3.
*   enable             - TRUE: enable clock divider output to PGMECLK device pin.
*                        FALSE: disable clock divider output to PGMECLK device pin.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pgmrclk_output_cfg(UINT32 dev_id,
                                      digi_pgmrclk_div_id_t div_id,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, div_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_pgmrclk_div_output_cfg(adpt_handle->digi_handle, div_id, (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_pgmrclk_output_cfg */


/*******************************************************************************
* adpt_m1_rate_monitor_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the DIGI_M1 rate monitor to enable Ethernet line timing mode.
*
*   Ethernet Line Timing is where a transmit data stream is timed with the clock from
*   any active receive data stream in the system or the PTP clock. An extra
*   rate monitoring block (PMM) is used to  measure the selected receive data
*   stream or PTP clock. The output rate modulation control signal (ckctl) is
*   then used by the ACB of any/all of the LINE and SYSTEM SERDES to control
*   the transmit rate.
*
*   This function is to configure communal resources required to enable
*   line timing on an ethernet channels. digi_enet_tx_timing_mode_set be used
*   to enable line timing mode. The rate monitor cannot be disabled when there
*   are active ethernet channels that use it.
*
* INPUTS:
*   dev_id             - device identity.
*   ref_clk_src        - the PMM reference clock source.
*   ref_clk_freq       - the clock frequency of the specicied clock source.
*                        This value is specified in Hz and shall be within the
*                        125E6 to 155.52E6 range.
*   enable             - TRUE: enable rate monitor.
*                        FALSE: disbale rate monitor.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_m1_rate_monitor_cfg_set(UINT32 dev_id,
                                          digi_m1_pmm_lite_clk_ref_t ref_clk_src,
                                          DOUBLE ref_clk_freq,
                                          BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%lf][%d].\r\n", dev_id, 
               ref_clk_src, ref_clk_freq, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_m1_rate_monitor_cfg_set(adpt_handle->digi_handle,
                                      ref_clk_src,
                                      ref_clk_freq,
                                      (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_m1_rate_monitor_cfg_set */


/*******************************************************************************
* adpt_m1_rate_monitor_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the configuration of the DIGI_M1 rate monitor.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *ref_clk_src       - the PMM reference clock source.
*   *ref_clk_freq      - the clock frequency of the specicied clock source.
*                        This value is specified in Hz and shall be within the
*                        125E6 to 152.52E6 range.
*   *enable            - TRUE: enable rate monitor.
*                        FALSE: disbale rate monitor.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_m1_rate_monitor_cfg_get(UINT32 dev_id,
                                          digi_m1_pmm_lite_clk_ref_t *ref_clk_src,
                                          DOUBLE *ref_clk_freq,
                                          BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 clk_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ref_clk_src);
    ADPT_PARA_VERIFY(ref_clk_freq);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_m1_rate_monitor_cfg_get(adpt_handle->digi_handle,
                                      ref_clk_src,
                                      ref_clk_freq,
                                      &clk_en);

    *enable = clk_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_m1_rate_monitor_cfg_get */


/*
** End of file
*/
