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
*     The file describes the FC800/FC1200/GDPS/CPRI Run-Time functionality of 
*     CBRC subsystem.
*
*   NOTES:
*     digi_fc_xxx():  FC1200 Run-Time configuration.
*     digi_cbr_xxx(): FC800/GDPS/CPRI Run-Time configuration.
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
* adpt_fc_prbs_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the PRBS 31 function both in generator and monitor.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   prbs_enable        - TRUE: enable prbs function.
*                        FALSE: disable PRBS function.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_prbs_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL prbs_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, prbs_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_prbs31_enable(digi_handle,
                                        cbr_handle,
                                        (cbrc_port_dir_t)dir,
                                        prbs_enable);

    PMC_RETURN(rc);
} /* adpt_fc_prbs_en_set */


/*******************************************************************************
* adpt_fc_prbs_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of PRBS 31 function both in generator 
*   and monitor.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *prbs_enable       - TRUE: enable prbs function.
*                        FALSE: disable PRBS function.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_prbs_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL *prbs_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, 
               chnl_id, dir, prbs_enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_prbs31_enable_get(digi_handle,
                                           cbr_handle,
                                           (cbrc_port_dir_t)dir,
                                           prbs_enable);

    PMC_RETURN(rc);
} /* adpt_fc_prbs_en_get */


/*******************************************************************************
* adpt_fc_rx_descramble_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the descramble function in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable descramble function in Rx.
*                        FALSE: disable descramble function in Rx.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_descramble_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_rx_descrambler_enable(digi_handle,
                                                cbr_handle,
                                                enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_descramble_set */


/*******************************************************************************
* adpt_fc_rx_descramble_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of descramble function in received 
*   direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable descramble function in Rx.
*                        FALSE: disable descramble function in Rx.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_descramble_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_rx_descrambler_enable_get(digi_handle,
                                                   cbr_handle,
                                                   enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_descramble_get */


/*******************************************************************************
* adpt_fc_rx_ecb_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the Error Control Block bits insertion in received 
*   direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   low_ecb            - lower 32 bits of Error Control Block bits.
*   high_ecb           - higher 32 bits of Error Control Block bits.
*   ecb_enable         - TRUE: enable Error Control Block bits insertion.
*                        FALSE: disable Error Control Block bits insertion.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_ecb_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 low_ecb,
                                 UINT32 high_ecb,
                                 BOOL   ecb_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_ecb_t fc_ecb;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, low_ecb, high_ecb, ecb_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure ECB bits */
    fc_ecb.ecb[0] = low_ecb;
    fc_ecb.ecb[1] = high_ecb;
    rc |= digi_cbr_fc1200_rx_ecb_set(digi_handle,
                                     cbr_handle,
                                     &fc_ecb);
    /* enable ECB insertion */
    rc |= digi_cbr_fc1200_rx_ecb_force(digi_handle,
                                       cbr_handle,
                                       ecb_enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_ecb_set */


/*******************************************************************************
* adpt_fc_rx_ecb_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of Error Control Block bits insertion 
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *low_ecb           - lower 32 bits of Error Control Block bits.
*   *high_ecb          - higher 32 bits of Error Control Block bits.
*   *ecb_enable        - TRUE: enable Error Control Block bits insertion.
*                        FALSE: disable Error Control Block bits insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_ecb_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *low_ecb,
                                 UINT32 *high_ecb,
                                 BOOL   *ecb_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_ecb_t fc_ecb;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p][%p][%p].\r\n", dev_id, 
               chnl_id, low_ecb, high_ecb, ecb_enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(low_ecb);
    ADPT_PARA_VERIFY(high_ecb);
    ADPT_PARA_VERIFY(ecb_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get ECB bits configuration */
    rc |= digi_cbr_fc1200_rx_ecb_get(digi_handle,
                                     cbr_handle,
                                     &fc_ecb);
    *low_ecb  = fc_ecb.ecb[0];
    *high_ecb = fc_ecb.ecb[1];
    /* get the enable/disable status of ECB insertion */
    rc |= digi_cbr_fc1200_rx_ecb_force_get(digi_handle,
                                           cbr_handle,
                                           ecb_enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_ecb_get */


/*******************************************************************************
* adpt_fc_tx_corrupt_blk_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables converting corrupt blocks to error blocks in the FC1200 
*   TX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   conv_enable        - TRUE: enable conversion from corrupt blocks to error blocks.
*                        FALSE: disable conversion from corrupt blocks to error blocks.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_tx_corrupt_blk_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL conv_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, conv_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_tx_corrupt_blk_conv_enable(digi_handle,
                                                     cbr_handle,
                                                     conv_enable);

    PMC_RETURN(rc);
} /* adpt_fc_tx_corrupt_blk_set */


/*******************************************************************************
* adpt_fc_tx_corrupt_blk_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of corrupt blocks to error blocks 
*   conversion in the FC1200 TX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *conv_enable       - TRUE: enable conversion from corrupt blocks to error blocks.
*                        FALSE: disable conversion from corrupt blocks to error blocks.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_tx_corrupt_blk_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *conv_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, conv_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_tx_corrupt_blk_conv_enable_get(digi_handle,
                                                        cbr_handle,
                                                        conv_enable);

    PMC_RETURN(rc);
} /* adpt_fc_tx_corrupt_blk_get */


/*******************************************************************************
* adpt_fc_rx_invalid_idle_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls how a 66b block with block type 0x1E, but an
*   unrecognized payload should be handled when received.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TURE: The invalid idle block is restored to a valid
*                              Idle/Idle block.
*                        FALSE: The invalid idle block is treated as an
*                               errored block. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_invalid_idle_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_rx_restore_invalid_idle_enable(digi_handle,
                                                         cbr_handle,
                                                         enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_invalid_idle_set */


/*******************************************************************************
* adpt_fc_rx_invalid_idle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls how a 66b block with block type 0x1E, but an
*   unrecognized payload should be handled when received.
*   Retrieves the status of invalid idle block to valid idle block or errored 
*   block.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TURE: The invalid idle block is restored to a valid
*                              Idle/Idle block.
*                        FALSE: The invalid idle block is treated as an
*                               errored block. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_invalid_idle_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_rx_restore_invalid_idle_enable_get(digi_handle,
                                                            cbr_handle,
                                                            enable);

    PMC_RETURN(rc);
} /* adpt_fc_rx_invalid_idle_get */


/*******************************************************************************
* adpt_fc_rx_lfos_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS insertion during error conditions in the FC1200 RX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lfos_mask          - LFOS mask.
*   enable             - TURE: defect causes LFOS to be asserted.
*                        FALSE: defect has no effect on LFOS. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_lfos_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_rx_lfos_mask_t lfos_mask,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_lfos_cfg_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               lfos_mask, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    lfos_cfg.mask = lfos_mask;
    switch (lfos_mask)
    {
        case DIGI_FC_RX_LFOS_MASK_LINK_FAIL:
            lfos_cfg.link_fail_enable = enable;
            break;
            
        case DIGI_FC_RX_LFOS_MASK_SYNC_LOST:
            lfos_cfg.sync_lost_enable = enable;
            break;
            
        case DIGI_FC_RX_LFOS_MASK_HIGH_BER:
            lfos_cfg.high_ber_enable = enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    rc |= digi_cbr_fc1200_rx_lfos_cfg(digi_handle,
                                      cbr_handle,
                                      &lfos_cfg);

    PMC_RETURN(rc);
} /* adpt_fc_rx_lfos_set */


/*******************************************************************************
* adpt_fc_rx_lfos_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves LFOS insertion during error conditions in the FC1200 RX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lfos_mask          - LFOS mask.
*
* OUTPUTS:
*   *enable            - TURE: defect causes LFOS to be asserted.
*                        FALSE: defect has no effect on LFOS. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_rx_lfos_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_rx_lfos_mask_t lfos_mask,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_lfos_cfg_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, chnl_id,
               lfos_mask, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_rx_lfos_cfg_get(digi_handle,
                                          cbr_handle,
                                          &lfos_cfg);
    switch (lfos_mask)
    {
        case DIGI_FC_RX_LFOS_MASK_LINK_FAIL:
            *enable = lfos_cfg.link_fail_enable;
            break;
            
        case DIGI_FC_RX_LFOS_MASK_SYNC_LOST:
            *enable = lfos_cfg.sync_lost_enable;
            break;
            
        case DIGI_FC_RX_LFOS_MASK_HIGH_BER:
            *enable = lfos_cfg.high_ber_enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc_rx_lfos_get */


/*******************************************************************************
* adpt_fc_tx_lfos_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS(Local Fault Ordered Set) insertion during error conditions 
*   in the FC1200 TX stream.
*   
*   The Loss-of-Signal LF enables or disables Local Fault Ordered Set insertion
*   upon detection of system-side signal fail.
*   The External LF enables or disables Local Fault Ordered Set insertion when
*   an external control signal is asserted.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lfos_mask          - LFOS mask.
*   enable             - TURE: defect causes LFOS to be asserted.
*                        FALSE: defect has no effect on LFOS. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_tx_lfos_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_tx_lfos_mask_t lfos_mask,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_tx_lfos_cfg_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               lfos_mask, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    lfos_cfg.mask = lfos_mask;
    switch (lfos_mask)
    {
        case DIGI_FC_TX_LFOS_MASK_LOS_LF:
            lfos_cfg.los_enable = enable;
            break;
            
        case DIGI_FC_TX_LFOS_MASK_EXT_LF:
            lfos_cfg.ext_enable = enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    rc |= digi_cbr_fc1200_tx_lfos_cfg(digi_handle,
                                      cbr_handle,
                                      &lfos_cfg);

    PMC_RETURN(rc);
} /* adpt_fc_tx_lfos_set */


/*******************************************************************************
* adpt_fc_tx_lfos_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrives the enable/disable status of LFOS(Local Fault Ordered Set) insertion 
*   during error conditions in the FC1200 TX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lfos_mask          - LFOS mask.
*   enable             - TURE: defect causes LFOS to be asserted.
*                        FALSE: defect has no effect on LFOS. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_tx_lfos_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_tx_lfos_mask_t lfos_mask,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_tx_lfos_cfg_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, chnl_id,
               lfos_mask, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_tx_lfos_cfg_get(digi_handle,
                                          cbr_handle,
                                          &lfos_cfg);
    switch (lfos_mask)
    {
        case DIGI_FC_TX_LFOS_MASK_LOS_LF:
            *enable = lfos_cfg.los_enable;
            break;
            
        case DIGI_FC_TX_LFOS_MASK_EXT_LF:
            *enable = lfos_cfg.ext_enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc_tx_lfos_get */


/*******************************************************************************
* adpt_fc_lfos_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables LFOS(Local Fault Ordered Set) force insertion.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   low_lfos           - lower 32 bits of LFOS bits.
*   high_lfos          - higher 32 bits of LFOS bits.
*   enable             - TURE: LFOS force insertion
*                        FALSE: remove LFOS force insertion. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_lfos_inst_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 low_lfos,
                                  UINT32 high_lfos,
                                  BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_lfos_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", 
               dev_id, chnl_id, dir, low_lfos, high_lfos, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure LFOS */
    lfos_cfg.lfos[0] = low_lfos;
    lfos_cfg.lfos[1] = high_lfos;
    rc |= digi_cbr_fc1200_lfos_set(digi_handle,
                                   cbr_handle,
                                   (cbrc_port_dir_t)dir,
                                   &lfos_cfg);
    /* enable LFOS insetion */                               
    rc |= digi_cbr_fc1200_lfos_force(digi_handle,
                                     cbr_handle,
                                     (cbrc_port_dir_t)dir,
                                     enable);

    PMC_RETURN(rc);
} /* adpt_fc_lfos_inst_set */


/*******************************************************************************
* adpt_fc_lfos_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the enable/disable status and LFOS bytes of LFOS force insertion.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *low_lfos          - lower 32 bits of LFOS bits.
*   *high_lfos         - higher 32 bits of LFOS bits.
*   *enable            - TURE: LFOS force insertion
*                        FALSE: remove LFOS force insertion. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_lfos_inst_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 *low_lfos,
                                  UINT32 *high_lfos,
                                  BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_lfos_t lfos_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p][%p][%p].\r\n", 
               dev_id, chnl_id, dir, low_lfos, high_lfos, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(low_lfos);
    ADPT_PARA_VERIFY(high_lfos);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get LFOS configuration */
    rc |= digi_cbr_fc1200_lfos_get(digi_handle,
                                   cbr_handle,
                                   (cbrc_port_dir_t)dir,
                                   &lfos_cfg);
    *low_lfos  = lfos_cfg.lfos[0];
    *high_lfos = lfos_cfg.lfos[1];
    /* get the enable/disable status of LFOS insetion */                               
    rc |= digi_cbr_fc1200_lfos_force_get(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         enable);
    PMC_RETURN(rc);
} /* adpt_fc_lfos_inst_get */


/*******************************************************************************
* adpt_fc_dlolb_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables FC1200 PMON DLOLB insertion options in the FC1200 RX 
*   stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   dlolb_mask         - DLOLB mask.
*   enable             - TURE: enable defect to DLOLB insertion.
*                        FALSE: disable defect to DLOLB insertion. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_set(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_port_dir_t dir,
                               digi_dlolb_mask_t dlolb_mask,
                               BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    fc1200_pmon_dlolb_cfg_t dlolb_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, dlolb_mask, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    dlolb_cfg.mask = dlolb_mask;
    switch (dlolb_mask)
    {
        case DIGI_FC_DLOLB_MASK_LOS:
            dlolb_cfg.los_enable = enable;
            break;
            
        case DIGI_FC_DLOLB_MASK_LOSS_SYNC:
            dlolb_cfg.loss_sync_enable = enable;
            break;
            
        case DIGI_FC_DLOLB_MASK_HIGH_BER:
            dlolb_cfg.high_ber_enable = enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    rc |= digi_cbr_fc1200_pmon_dlolb_cfg(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         &dlolb_cfg);

    PMC_RETURN(rc);
} /* adpt_fc_dlolb_set */


/*******************************************************************************
* adpt_fc_dlolb_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of FC1200 PMON DLOLB insertion options 
*   in the FC1200 RX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   dlolb_mask         - DLOLB mask.
*
* OUTPUTS:
*   *enable            - TURE: enable defect to DLOLB insertion.
*                        FALSE: disable defect to DLOLB insertion. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_port_dir_t dir,
                               digi_dlolb_mask_t dlolb_mask,
                               BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    fc1200_pmon_dlolb_cfg_t dlolb_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%p].\r\n", dev_id, 
               chnl_id, dir, dlolb_mask, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_pmon_dlolb_cfg_get(digi_handle,
                                             cbr_handle,
                                             (cbrc_port_dir_t)dir,
                                             &dlolb_cfg);

    switch (dlolb_mask)
    {
        case DIGI_FC_DLOLB_MASK_LOS:
            *enable = dlolb_cfg.los_enable;
            break;
            
        case DIGI_FC_DLOLB_MASK_LOSS_SYNC:
            *enable = dlolb_cfg.loss_sync_enable;
            break;
            
        case DIGI_FC_DLOLB_MASK_HIGH_BER:
            *enable = dlolb_cfg.high_ber_enable;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    
    PMC_RETURN(rc);
} /* adpt_fc_dlolb_get */


/*******************************************************************************
* adpt_fc_dlolb_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables FC1200 DLOLB force insertion in the FC1200 RX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TURE: enable DLOLB force insertion.
*                        FALSE: disable DLOLB force insertion. 
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_inst_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_pmon_dlolb_force(digi_handle,
                                           cbr_handle,
                                           (cbrc_port_dir_t)dir,
                                           enable);

    PMC_RETURN(rc);
} /* adpt_fc_dlolb_inst_set */


/*******************************************************************************
* adpt_fc_dlolb_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of  FC1200 DLOLB force insertion in the 
*   FC1200 RX stream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *enable            - TURE: enable DLOLB force insertion.
*                        FALSE: disable DLOLB force insertion. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_inst_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_pmon_dlolb_force_get(digi_handle,
                                              cbr_handle,
                                              (cbrc_port_dir_t)dir,
                                              enable);

    PMC_RETURN(rc);
} /* adpt_fc_dlolb_inst_get */


/*******************************************************************************
* adpt_fc_invalid_blk_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how blocks with invalid block type encodings are handled when
*   encountered during processing.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   mode               - configures behaviour when an invalid block is received.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_invalid_blk_mode_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         digi_invalid_blk_mode_t mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, mode);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fc1200_pmon_invalid_block_mode_set(digi_handle,
                                                      cbr_handle,
                                                      (cbrc_port_dir_t)dir,
                                                      (fc1200_pmon_invalid_block_mode_t)mode);

    PMC_RETURN(rc);
} /* adpt_fc_invalid_blk_mode_set */


/*******************************************************************************
* adpt_fc_invalid_blk_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves how blocks with invalid block type encodings are handled when
*   encountered during processing.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *mode              - configures behaviour when an invalid block is received.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_invalid_blk_mode_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         digi_invalid_blk_mode_t *mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", 
               dev_id, chnl_id, dir, mode);

    /* check parameter validation */
    ADPT_PARA_VERIFY(mode);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fc1200_pmon_invalid_block_mode_get(digi_handle,
                                                     cbr_handle,
                                                     (cbrc_port_dir_t)dir,
                                                     (fc1200_pmon_invalid_block_mode_t*)mode);

    PMC_RETURN(rc);
} /* adpt_fc_invalid_blk_mode_get */


/*******************************************************************************
* adpt_fc_dlolb_gen_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures what defects contribute to dLOLB generation.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   lcv                - generate dLOLB with line code violation defect.
*   xdet               - generate dLOLB with transition detect defect.
*   asd                - generate dLOLB with analog loss of signal defect.
*   density            - generate dLOLB with comma density defect.
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
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC800.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_gen_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL lcv,
                                   BOOL xdet,
                                   BOOL asd,
                                   BOOL density)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    fc800_pmg_dlolb_gen_t dlolb_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id,
               dir, lcv, xdet, asd, density);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&dlolb_cfg, 0, sizeof(dlolb_cfg));
    dlolb_cfg.mask = FC800_PMG_DLOLB_GEN_MASK_LCV 
                     | FC800_PMG_DLOLB_GEN_MASK_DENSITY
                     | FC800_PMG_DLOLB_GEN_MASK_XDET
                     | FC800_PMG_DLOLB_GEN_MASK_ASD;
    dlolb_cfg.lcv     = lcv;
    dlolb_cfg.xdet    = xdet;
    dlolb_cfg.asd     = asd;
    dlolb_cfg.density = density;

    rc |= digi_cbr_fc800_dlolb_gen_set(digi_handle,
                                       cbr_handle,
                                       (cbrc_port_dir_t)dir,
                                       &dlolb_cfg);

    PMC_RETURN(rc);
} /* adpt_fc_dlolb_gen_set */


/*******************************************************************************
* adpt_fc_dlolb_gen_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves what defects contribute to dLOLB generation.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *lcv               - generate dLOLB with line code violation defect.
*   *xdet              - generate dLOLB with transition detect defect.
*   *asd               - generate dLOLB with analog loss of signal defect.
*   *density           - generate dLOLB with comma density defect.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This feature is available only for DIGI_CBR_CLIENT_10G_FC800.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_dlolb_gen_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL *lcv,
                                   BOOL *xdet,
                                   BOOL *asd,
                                   BOOL *density)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    fc800_pmg_dlolb_gen_t dlolb_cfg;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(lcv);
    ADPT_PARA_VERIFY(xdet);
    ADPT_PARA_VERIFY(asd);
    ADPT_PARA_VERIFY(density);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&dlolb_cfg, 0, sizeof(dlolb_cfg));
    rc |= digi_cbr_fc800_dlolb_gen_get(digi_handle,
                                       cbr_handle,
                                       (cbrc_port_dir_t)dir,
                                       &dlolb_cfg);

    *lcv     = dlolb_cfg.lcv;
    *xdet    = dlolb_cfg.xdet;
    *asd     = dlolb_cfg.asd;
    *density = dlolb_cfg.density;

    PMC_RETURN(rc);
} /* adpt_fc_dlolb_gen_get */


/*******************************************************************************
* adpt_cbr_pn11_mon_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables PN11 monitoring, which is valid for 10G and 40G modes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TURE: enable PN11 monitoring.
*                        FALSE: disable PN11 monitoring. 
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
PUBLIC PMC_ERROR adpt_cbr_pn11_mon_set(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_port_dir_t dir,
                                          BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_fsgm_pn11_mon_enable(digi_handle,
                                        cbr_handle,
                                        (cbrc_port_dir_t)dir,
                                        enable);

    PMC_RETURN(rc);
} /* adpt_cbr_pn11_mon_set */


/*******************************************************************************
* adpt_cbr_pn11_mon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of PN11 monitoring, which is valid for 
*   10G and 40G modes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *enable            - TURE: enable PN11 monitoring.
*                        FALSE: disable PN11 monitoring. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_pn11_mon_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, 
               chnl_id, dir, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_fsgm_pn11_mon_enable_get(digi_handle,
                                           cbr_handle,
                                           (cbrc_port_dir_t)dir,
                                           enable);
    PMC_RETURN(rc);
} /* adpt_cbr_pn11_mon_get */


/*******************************************************************************
* adpt_cbr_pattern_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the fault pattern to be inserted during LOS or faults.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pattern            - insertion signal pattern.
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
PUBLIC PMC_ERROR adpt_cbr_pattern_src_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      digi_cbr_pattern_t pattern)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, pattern);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure fault pattern */
    rc |= digi_cbr_fsgm_pattern_cfg(digi_handle,
                                    cbr_handle,
                                    (cbrc_port_dir_t)dir,
                                    (cbr_fsgm_pattern_t)pattern);

    PMC_RETURN(rc);
} /* adpt_cbr_pattern_src_set */


/*******************************************************************************
* adpt_cbr_pattern_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the fault pattern to be inserted during LOS or faults.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *pattern           - insertion signal pattern.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_pattern_src_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      digi_cbr_pattern_t *pattern)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id,
               chnl_id, dir, pattern);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get fault pattern */
    rc = digi_cbr_fsgm_pattern_cfg_get(digi_handle,
                                       (cbrc_port_dir_t)dir,
                                       cbr_handle,
                                       (cbr_fsgm_pattern_t*)pattern);

    PMC_RETURN(rc);
} /* adpt_cbr_pattern_src_get */


/*******************************************************************************
* adpt_cbr_user_pattern_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the user fault pattern value to be inserted.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pattern            - user pattern.
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
PUBLIC PMC_ERROR adpt_cbr_user_pattern_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      cbr_fsgm_user_fault_pattern_t pattern)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, pattern);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure user fault pattern */
    rc |= digi_cbr_fsgm_user_fault_pattern_set(digi_handle,
                                               cbr_handle,
                                               (cbrc_port_dir_t)dir,
                                               pattern);

    PMC_RETURN(rc);
} /* adpt_cbr_user_pattern_set */


/*******************************************************************************
* adpt_cbr_user_pattern_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the user fault pattern value to be inserted.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *pattern           - user pattern.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_user_pattern_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       cbr_fsgm_user_fault_pattern_t *pattern)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id,
               chnl_id, dir, pattern);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure user fault pattern */
    rc |= digi_cbr_fsgm_user_fault_pattern_get(digi_handle,
                                               cbr_handle,
                                               (cbrc_port_dir_t)dir,
                                               pattern);

    PMC_RETURN(rc);
} /* adpt_cbr_user_pattern_get */


/*******************************************************************************
* adpt_cbr_pattern_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables fault pattern into output forcely.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TURE: enable fault pattern into output.
*                        FALSE: disable fault pattern into output. 
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
PUBLIC PMC_ERROR adpt_cbr_pattern_inst_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* enable fault insetion */                               
    rc |= digi_cbr_fsgm_fault_force(digi_handle,
                                    cbr_handle,
                                    (cbrc_port_dir_t)dir,
                                    enable);

    PMC_RETURN(rc);
} /* adpt_cbr_pattern_inst_set */


/*******************************************************************************
* adpt_cbr_pattern_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the insertion status of fault pattern into output.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *enable            - TURE: enable fault pattern into output.
*                        FALSE: disable fault pattern into output. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_pattern_inst_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, 
               chnl_id, dir, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the status of the fault insetion */                               
    rc |= digi_cbr_fsgm_fault_force_get(digi_handle,
                                        cbr_handle,
                                        (cbrc_port_dir_t)dir,
                                        enable);

    PMC_RETURN(rc);
} /* adpt_cbr_pattern_inst_get */


/*******************************************************************************
* adpt_cbr_cmf_fault_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function forces the CBRC client to insert nominal rate fault replacement 
*   signal on the egress direction. This function will supersede any CBRC 
*   consequential actions that FW may be executing. This function will continue 
*   to have control on CBRC fault pattern insertion until releasing control is 
*   specified.
*
*   Once contorl is released, normal operation will continue for the trunk to 
*   client CBRC consequential actions that are set (if any). 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   action             - action to insert normial rate fault replacement. 
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
PUBLIC PMC_ERROR adpt_cbr_cmf_fault_cfg(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_cmf_sw_control_action_t action)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, action);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_trigger_cbr_fault_cfg(digi_handle, cbr_handle, action);

    PMC_RETURN(rc);
} /* adpt_cbr_cmf_fault_cfg */


/*
** End of file
*/
