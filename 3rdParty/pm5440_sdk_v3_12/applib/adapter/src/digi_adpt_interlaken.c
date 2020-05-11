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
*     The file describes the interlaken Run-Time functionality of SIFD subsystem.
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
* adpt_ilkn_info_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures interlaken channel related parameter information. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   oduk_type          - ODUk type.
*   oduflex_rate       - The oduflex_rate with bps unit is only available for 
*                        ODUflex(CBR). It should be more than 1000bps.
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
PUBLIC PMC_ERROR adpt_ilkn_info_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_oduk_t oduk_type, 
                                DOUBLE oduflex_rate)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        /* get the index in traffic container */
        rc = adpt_rst_chnl_index_get(adpt_handle, chnl_id, &index);
        ADPT_RETURN_VERIFY(rc);

        /* chnl_id verification */
        if (DIGI_TYPE_INLK != DIGI_TRAFFIC_TYPE_GET(adpt_handle->restart_info[index]->traffic_mode))
        {
            PMC_RETURN(PMC_ERR_FAIL);
        }

        adpt_handle->restart_info[index]->oduk_type    = oduk_type;
        adpt_handle->restart_info[index]->oduflex_rate = oduflex_rate;

        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
        if (PMC_SUCCESS != rc)
        {
            PMC_RETURN(PMC_SUCCESS);
        }
    }

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);

    adpt_handle->traffic_info[index].oduk_type    = oduk_type;
    adpt_handle->traffic_info[index].oduflex_rate = oduflex_rate;

    PMC_RETURN(rc);
} /* adpt_ilkn_info_set */


/*******************************************************************************
* adpt_ilkn_cfg_rx_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls the IL_RX_SOFT reset field. This field controls the
*   receive activation state of an ILKN instance.
*
* INPUTS:
*   dev_id             - device identity.
*   sifd_id            - SIFD identity. Only ILKN_SIFD_1 and ILKN_SIFD_2 may
*                        be available.
*   enable             - TRUE: Receive ILKN out of reset.
*                        FALSE: Receive ILKN in reset.
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
PUBLIC PMC_ERROR adpt_ilkn_cfg_rx_reset(UINT32 dev_id, ilkn_sifd_type_t sifd_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_ilkn_link_t *ilkn_link = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, sifd_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (sifd_id)
    {
        case ILKN_SIFD_1:
            ilkn_link = adpt_handle->ilkn_1_ctxt;
            break;

        case ILKN_SIFD_2:
            ilkn_link = adpt_handle->ilkn_2_ctxt;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_PARA_VERIFY(ilkn_link);

    rc = digi_ilkn_chnl_cfg_rx_reset(adpt_handle->digi_handle, ilkn_link, enable);

    PMC_RETURN(rc);
} /* adpt_ilkn_cfg_rx_reset */




/*******************************************************************************
* adpt_ilkn_fifo_thresh_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API configures the ODUKSW or CPB FIFO threshold settings for XON to XOFF
*   and XOFF to XON flow control status indications to upstream device.
*
*   The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*   Zone thresholds are specified in units of '48 byte increments'.
*
*   This API can be used to support developers in setting XON and XOFF Zone
*   thresholds.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   xon_thresh         - number of '48 byte increments' in a FIFO
*                        that will trigger XOFF to XON event
*                        in ILKN flow control.
*   xoff_thresh        - number of '48 byte increments' in a FIFO
*                        that will trigger XON to XOFF event
*                        in ILKN flow control.
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
PUBLIC PMC_ERROR adpt_ilkn_fifo_thresh_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 xon_thresh,
                                     UINT32 xoff_thresh)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_ilkn_chnl_t *ilkn_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_ilkn_handle_get(dev_id, chnl_id, &digi_handle, &ilkn_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_sifd_ilkn_fifo_thresh_set(digi_handle, 
                                        ilkn_handle, 
                                        xon_thresh, 
                                        xoff_thresh);

    PMC_RETURN(rc);
} /* adpt_ilkn_fifo_thresh_set */


/*******************************************************************************
* adpt_ilkn_fifo_thresh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API outputs the ODUKSW or CPB FIFO threshold settings for XON to XOFF
*   and XOFF to XON flow control status indications to upstream device.
*
*   The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*   Zone thresholds are specified in units of '48 byte increments'.
*
*   This API can be used to support developers in setting XON and XOFF Zone
*   thresholds.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *xon_thresh        - number of '48 byte increments' in a FIFO
*                        that will trigger XOFF to XON event
*                        in ILKN flow control.
*   *xoff_thresh       - number of '48 byte increments' in a FIFO
*                        that will trigger XON to XOFF event
*                        in ILKN flow control.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_fifo_thresh_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 *xon_thresh,
                                     UINT32 *xoff_thresh)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_ilkn_chnl_t *ilkn_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(xon_thresh);
    ADPT_PARA_VERIFY(xoff_thresh);

    /* handle process */
    rc = adpt_ilkn_handle_get(dev_id, chnl_id, &digi_handle, &ilkn_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_sifd_ilkn_fifo_thresh_get(digi_handle, 
                                        ilkn_handle, 
                                        xon_thresh, 
                                        xoff_thresh);

    PMC_RETURN(rc);
} /* adpt_ilkn_fifo_thresh_get */


/*******************************************************************************
* adpt_ilkn_fifo_size_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API outputs the ODUKSW or CPB FIFO size for a backplane to DIGI ILKN
*   channel.
*
*   The data storage in the ODUKSW and CPB FIFO has a granularity of 48 bytes.
*   Zone thresholds are specified in units of '48 byte increments'.
*
*   This API can be used to support developers in setting XON and XOFF Zone
*   thresholds.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *num_fifo_blk      - number of blocks associated with ILKN channel handle.
*   *num_fifo_incr     - number of 48 byte increments contained within the FIFO.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_fifo_size_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *num_fifo_blk,
                                    UINT32 *num_fifo_incr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_ilkn_chnl_t *ilkn_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(num_fifo_blk);
    ADPT_PARA_VERIFY(num_fifo_incr);

    /* handle process */
    rc = adpt_ilkn_handle_get(dev_id, chnl_id, &digi_handle, &ilkn_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_sifd_ilkn_fifo_size_get(digi_handle, 
                                      ilkn_handle, 
                                      num_fifo_blk, 
                                      num_fifo_incr);

    PMC_RETURN(rc);
} /* adpt_ilkn_fifo_size_get */


/*******************************************************************************
* adpt_ilkn_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the PIF Interlaken SERDES PRBS Generator and Monitor.
*
* INPUTS:
*   dev_id             - device identity.
*   sifd_id            - SIFD identity. Only ILKN_SIFD_1 and ILKN_SIFD_2 may
*                        be available.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   prbs_mode          - prbs pattern mode.
*   inv_cfg            - pattern monitor inverter configuration ON(TRUE) or 
*                        OFF(FALSE).
*   enable             - TRUE: enable ILKN PRBS.
*                        FALSE: disable ILKN PRBS.
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
PUBLIC PMC_ERROR adpt_ilkn_prbs_set(UINT32 dev_id,
                                 ilkn_sifd_type_t sifd_id,
                                 digi_serdes_dir_t dir,
                                 digi_serdes_prbs_t prbs_mode,
                                 BOOL inv_cfg,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_ilkn_link_t *ilkn_link = NULL;
    util_serdes_patt_cfg_t prbs_cfg;
    util_serdes_patt_cfg_t org_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               sifd_id, dir, prbs_mode, inv_cfg, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (sifd_id)
    {
        case ILKN_SIFD_1:
            ilkn_link = adpt_handle->ilkn_1_ctxt;
            break;

        case ILKN_SIFD_2:
            ilkn_link = adpt_handle->ilkn_2_ctxt;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_PARA_VERIFY(ilkn_link);

    /* reset */
    PMC_MEMSET((void*)&prbs_cfg, 0, sizeof(util_serdes_patt_cfg_t));
    prbs_cfg.enable  = (BOOL8)enable;
    prbs_cfg.inv_cfg = (BOOL8)inv_cfg;
    prbs_cfg.mode    = (util_patt_mode_t)prbs_mode;

    /* configuration on generation */
    if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_sifd_ilkn_prbs_gen_cfg_get(adpt_handle->digi_handle, ilkn_link, &org_cfg);
        ADPT_RETURN_VERIFY(rc);

        if (org_cfg.enable != prbs_cfg.enable)
        {
            rc = digi_sifd_ilkn_prbs_gen_cfg(adpt_handle->digi_handle, ilkn_link, &prbs_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (prbs_cfg.enable)
        {
            if ((org_cfg.inv_cfg != prbs_cfg.inv_cfg) || (org_cfg.mode != prbs_cfg.mode))
            {
                org_cfg.enable = FALSE;
                rc = digi_sifd_ilkn_prbs_gen_cfg(adpt_handle->digi_handle, ilkn_link, &org_cfg);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_sifd_ilkn_prbs_gen_cfg(adpt_handle->digi_handle, ilkn_link, &prbs_cfg);
                ADPT_RETURN_VERIFY(rc);
            }
        }
    }

    /* configuration on monitor */
    if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_sifd_ilkn_prbs_mon_cfg_get(adpt_handle->digi_handle, ilkn_link, &org_cfg);
        ADPT_RETURN_VERIFY(rc);

        if (org_cfg.enable != prbs_cfg.enable)
        {
            rc = digi_sifd_ilkn_prbs_mon_cfg(adpt_handle->digi_handle, ilkn_link, &prbs_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (prbs_cfg.enable)
        {
            if ((org_cfg.inv_cfg != prbs_cfg.inv_cfg) || (org_cfg.mode != prbs_cfg.mode))
            {
                org_cfg.enable = FALSE;
                rc = digi_sifd_ilkn_prbs_mon_cfg(adpt_handle->digi_handle, ilkn_link, &org_cfg);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_sifd_ilkn_prbs_mon_cfg(adpt_handle->digi_handle, ilkn_link, &prbs_cfg);
                ADPT_RETURN_VERIFY(rc);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_ilkn_prbs_set */


/*******************************************************************************
* adpt_ilkn_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the prbs_monitor synchroniation state.
*
* INPUTS:
*   dev_id             - device identity.
*   sifd_id            - SIFD identity. Only ILKN_SIFD_1 and ILKN_SIFD_2 may
*                        be available.
*
* OUTPUTS:
*   *status            - the Word boundary lock status for the lanes bit[0~24].
*                        bit[x] == 1: word boundary locked.
*                        bit[x] == 0: No lock detected.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_rx_state_get(UINT32 dev_id,
                                   ilkn_sifd_type_t sifd_id,
                                   UINT32 *status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_ilkn_link_t *ilkn_link = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (sifd_id)
    {
        case ILKN_SIFD_1:
            ilkn_link = adpt_handle->ilkn_1_ctxt;
            break;

        case ILKN_SIFD_2:
            ilkn_link = adpt_handle->ilkn_2_ctxt;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_PARA_VERIFY(ilkn_link);

    rc = digi_sifd_ilkn_prbs_status_get(adpt_handle->digi_handle, ilkn_link, status);

    PMC_RETURN(rc);
} /* adpt_ilkn_rx_state_get */


/*******************************************************************************
* adpt_ilkn_2b_header_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function can be used to dynamically update TX/RX 2B header on a specified
*    channel. This function can only be used under the following condition:
*    - ILKN link shall be configured in 2B header insertion without 4B header 
*       - digi_sifd_ilkn_cfg_t::oduksw_2b_header_mode == FALSE when configuring ILKN link
*    - specified channel shall be mapped to the ODUKSW 
*       - digi_sifd_ilkn_cfg_t::oduksw_intf_en == TRUE when configuring ILKN link
*    - at least 1 bit should remain static (0 or 1) from the previous and new 
*      2B header value
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   header             - the 2B header for ODUKSW channels.
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
PUBLIC PMC_ERROR adpt_ilkn_2b_header_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT16 header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_ilkn_chnl_t *ilkn_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, header);

    /* handle process */
    rc = adpt_ilkn_handle_get(dev_id, chnl_id, &digi_handle, &ilkn_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configuration in the Tx direction */
    if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_ilkn_oduk_tx_2b_header_set(digi_handle, ilkn_handle, header);
        ADPT_RETURN_VERIFY(rc);
    }

    /* configuration in the Rx direction */
    if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_ilkn_oduk_rx_2b_header_set(digi_handle, ilkn_handle, header);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_ilkn_2b_header_set */


/*******************************************************************************
* adpt_ilkn_2b_header_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function can be used to retrieve TX/RX 2B header on a specified
*    channel. This function can only be used under the following condition:
*    - ILKN link shall be configured in 2B header insertion without 4B header 
*       - digi_sifd_ilkn_cfg_t::oduksw_2b_header_mode == FALSE when configuring ILKN link
*    - specified channel shall be mapped to the ODUKSW 
*       - digi_sifd_ilkn_cfg_t::oduksw_intf_en == TRUE when configuring ILKN link
*    - at least 1 bit should remain static (0 or 1) from the previous and new 
*      2B header value
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *header            - the 2B header for ODUKSW channels.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_2b_header_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT16 *header)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_ilkn_chnl_t *ilkn_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(header);

    /* handle process */
    rc = adpt_ilkn_handle_get(dev_id, chnl_id, &digi_handle, &ilkn_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_SERDES_DIR_TX == dir)
    {
        rc = digi_ilkn_oduk_tx_2b_header_get(digi_handle, ilkn_handle, header);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (DIGI_SERDES_DIR_RX == dir)
    {
        rc = digi_ilkn_oduk_rx_2b_header_get(digi_handle, ilkn_handle, header);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        rc = PMC_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
} /* adpt_ilkn_2b_header_get */

/*******************************************************************************
* adpt_ilkn_copi_lowpwr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures COPI in low power state.
*
* INPUTS:
*   dev_id             - device identity.
*   sifd_id            - SIFD identity. Only ILKN_SIFD_1 and ILKN_SIFD_2 may
*                        be available.
*   lowpwr_flag        - TRUE: in low power state.
*                        FALSE: not in low power state.
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
PUBLIC PMC_ERROR adpt_ilkn_copi_lowpwr_cfg(UINT32             dev_id, 
                                           ilkn_sifd_type_t   sifd_id,
                                           BOOL               lowpwr_flag)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_ilkn_link_t *ilkn_link = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (sifd_id)
    {
        case ILKN_SIFD_1:
            ilkn_link = adpt_handle->ilkn_1_ctxt;
            break;

        case ILKN_SIFD_2:
            ilkn_link = adpt_handle->ilkn_2_ctxt;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_PARA_VERIFY(ilkn_link);

    rc = digi_sifd_ilkn_copi_lowpwr_cfg(adpt_handle->digi_handle, ilkn_link, lowpwr_flag);

    PMC_RETURN(rc);
} /* adpt_ilkn_copi_lowpwr_cfg */

/*
** End of file
*/
