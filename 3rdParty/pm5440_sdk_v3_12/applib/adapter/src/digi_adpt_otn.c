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
*     This file describes the OTN overhead configuration Run-Time function.
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
* adpt_otn_fec_type_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the FEC type both in source and sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fec_type           - FEC type selection.
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
PUBLIC PMC_ERROR adpt_otn_fec_type_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_server_fec_t fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, fec_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_fec_type_set(digi_handle, otu_handle, fec_type);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_pmon_fw_fec_cfg(digi_handle);

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].fec_type = fec_type;
    }
    
    PMC_RETURN(rc);
} /* adpt_otn_fec_type_set */


/*******************************************************************************
* adpt_otn_fec_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FEC type both in source and sink position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *fec_type          - FEC type selection.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_type_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_server_fec_t *fec_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(fec_type);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_fec_type_get(digi_handle, otu_handle, fec_type);

    PMC_RETURN(rc);
} /* adpt_otn_fec_type_get */


/*******************************************************************************
* adpt_otn_fec_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables FEC error correction fuction in sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fec_enable         - TRUE: enable FEC error correction.
*                        FALSE: disable FEC error correction.
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
*   This API is invalid as to LINE_OTN_FEC_SWIZZLE_100G FEC type.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  BOOL   fec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, fec_enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_fec_dec_passthru_set(digi_handle,
                                       otu_handle,
                                       adpt_handle->traffic_info[index].fec_type,
                                       !fec_enable);

    PMC_RETURN(rc);
} /* adpt_otn_fec_en_set */


/*******************************************************************************
* adpt_otn_fec_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FEC error correction fuction status in sink position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *fec_enable        - TRUE: enable FEC error correction.
*                        FALSE: disable FEC error correction.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  BOOL   *fec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    BOOL fec_pass = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(fec_enable);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_fec_dec_passthru_get(digi_handle, otu_handle, &fec_pass);

    /* parameter conversion */
    *fec_enable = !fec_pass;
    
    PMC_RETURN(rc);
} /* adpt_otn_fec_en_get */


/*******************************************************************************
* adpt_otn_fec_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables FEC error insertion in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   err_rate           - requested error rate percentage. Range: 0 ~ 10000.
*                        resolution: 0.01%
*   err_mode           - BERGEN error insertion mode.
*   err_enable         - TRUE: enable FEC error insertion.
*                        FALSE: disable FEC error insertion.
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
*   1. This feature is invalid in SYSOTN.
*   2. It is not supported on SFI5.1 interface.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_err_inst_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 err_rate,
                                      digi_otn_err_mode_t err_mode,
                                      BOOL   err_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    UINT32 lane_mask = 0;
    UINT8 lane_index = 0;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 i = 0;
    digi_bergen_lane_cfg_t lane_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, err_rate, err_mode, err_enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX) || (traffic_loc >= DIGI_SIDE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
        {
            if (DIGI_SERDES_PIN_NOT_USED != adpt_handle->dev_info->line_port_lane_tx[serdes_port][lane_index])
            {
                lane_mask |= 1 << i;
                i++;
            }
        }
    }
    else
    {
        for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
        {
            if (DIGI_SERDES_PIN_NOT_USED != adpt_handle->dev_info->sys_port_lane_tx[serdes_port][lane_index])
            {
                lane_mask |= 1 << i;
                i++;
            }
        }
    }

    lane_cfg.error_rate     = (FLOAT)err_rate / 10000;
    lane_cfg.ringosc_enable = TRUE;
    lane_cfg.bergen_mode    = (lineotn_bergen_err_mode)err_mode;

    rc = digi_lineotn_bergen_set(digi_handle,
                                 otu_handle,
                                 lane_mask,
                                 &lane_cfg,
                                 err_enable);

    PMC_RETURN(rc);
} /* adpt_otn_fec_err_inst_set */


/*******************************************************************************
* adpt_otn_fec_err_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of FEC error insertion in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *err_rate          - requested error rate percentage. Range: 0 ~ 10000.
*                        resolution: 0.01%
*   *err_mode          - BERGEN error insertion mode.
*   *err_enable        - TRUE: enable FEC error insertion.
*                        FALSE: disable FEC error insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. This feature is invalid in SYSOTN.
*   2. It is not supported on SFI5.1 interface.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_err_inst_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *err_rate,
                                      digi_otn_err_mode_t *err_mode,
                                      BOOL   *err_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_bergen_lane_cfg_t lane_cfg;
    BOOL8 error_enable = FALSE;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(err_rate);
    ADPT_PARA_VERIFY(err_mode);
    ADPT_PARA_VERIFY(err_enable);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&lane_cfg, 0, sizeof(lane_cfg));
    rc = digi_lineotn_bergen_get(digi_handle,
                                 otu_handle,
                                 0,
                                 &lane_cfg,
                                 &error_enable);

    *err_rate = (UINT32)(10000 * lane_cfg.error_rate + 0.5);
    *err_mode = (digi_otn_err_mode_t)lane_cfg.bergen_mode;
    *err_enable = error_enable ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_otn_fec_err_inst_get */


/*******************************************************************************
* adpt_otn_tx_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted TTI field of an ODU/OTU framer channel in source
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   tti_data           - TTI buffer with 64 bytes at max.
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
PUBLIC PMC_ERROR adpt_otn_tx_tti_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_seg_t seg_id,
                                digi_otn_loc_t location,
                                UINT8 tti_data[64])
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%p].\r\n", dev_id, 
               chnl_id, seg_id, location, tti_data);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_sm_so_tti_set(digi_handle,
                                            otu_handle,
                                            LINE_OTN_INS_TYPE_USER_VALUE,
                                            &tti_data[0]);
            break;
            
        default:
            rc = digi_otn_odu_tx_tti_set(digi_handle, 
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         ODU_STRUCT_TTI_SAPI_DAPI_OP_SPEC_INS,
                                         (UINT8*)&tti_data[0],
                                         (UINT8*)&tti_data[16],
                                         (UINT8*)&tti_data[32]);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_tti_set */


/*******************************************************************************
* adpt_otn_tx_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted TTI field of an ODU/OTU framer channel in source
*   position.
*
*   This API is used for debug or data recovery.
*   
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tti_data          - TTI buffer with 64 bytes at max.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_tti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_seg_t seg_id,
                                 digi_otn_loc_t location,
                                 UINT8 *tti_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_data);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_sm_so_tti_get(digi_handle, otu_handle, tti_data);
            break;
            
        default:
            rc = digi_otn_odu_tx_tti_get(digi_handle, 
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         tti_data,
                                         (UINT8*)(tti_data + 16),
                                         (UINT8*)(tti_data + 32));
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_tti_get */


/*******************************************************************************
* adpt_otn_rx_tti_lock
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Locks Rx accepted TTI bytes. This function only needs to be called once for 
*   a digi chip.
*   
*   Typical sequences:
*     adpt_otn_rx_tti_lock(..., TRUE);
*     A series of adpt_otn_rx_tti_get().
*     adpt_otn_rx_tti_lock(..., FALSE);
*
* INPUTS:
*   dev_id             - device identity.
*   tti_lock           - TRUE: lock accepted TTI bytes.
*                        FALSE: unlock accepted TTI bytes.
*
* OUTPUTS:
*   none.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_tti_lock(UINT32 dev_id, BOOL tti_lock)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_accepted_tti_lock(adpt_handle->digi_handle, tti_lock);

    PMC_RETURN(rc);
} /* adpt_otn_rx_tti_lock */


/*******************************************************************************
* adpt_otn_rx_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted TTI field of an ODU/OTU framer channel in sink 
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tti_data          - TTI buffer with 64 bytes at max.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_tti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_seg_t seg_id,
                                 digi_otn_loc_t location,
                                 UINT8 *tti_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_data);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_sm_accepted_tti_get(digi_handle, otu_handle, tti_data);
            break;
            
        default:
            rc = digi_otn_odu_rx_accepted_tti_get(digi_handle, 
                                                  odu_handle,
                                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                  tti_data,
                                                  (UINT8*)(tti_data + 16),
                                                  (UINT8*)(tti_data + 32));
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_rx_tti_get */


/*******************************************************************************
* adpt_otn_exp_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected TTI field of an ODU/OTU framer channel in sink
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   tti_data           - TTI buffer with 32 bytes at max.
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
*   Please note that only the first 32 TTI bytes are valid.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_exp_tti_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 tti_data[64])
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%p].\r\n", dev_id, 
               chnl_id, seg_id, location, tti_data);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_sm_exp_tti_set(digi_handle, otu_handle, &tti_data[0]);
            break;
            
        default:
            rc = digi_otn_odu_rx_expected_tti_set(digi_handle, 
                                                  odu_handle,
                                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                  (UINT8*)&tti_data[0],
                                                  (UINT8*)&tti_data[16]);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_tti_set */


/*******************************************************************************
* adpt_otn_exp_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected TTI field of an ODU/OTU framer channel in sink 
*   position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tti_data          - TTI buffer with 64 bytes at max.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_exp_tti_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 *tti_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_data);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_sm_exp_tti_get(digi_handle, otu_handle, tti_data);
            break;
            
        default:
            rc = digi_otn_odu_rx_expected_tti_get(digi_handle, 
                                                  odu_handle,
                                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                  tti_data,
                                                  (UINT8*)(tti_data + 16));
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_tti_get */


/*******************************************************************************
* adpt_otn_tim_det_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures dTIM detection mode of an ODU/OTU framer channel in sink
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   tim_mode           - dTIM detection mode.
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
PUBLIC PMC_ERROR adpt_otn_tim_det_mode_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        digi_otn_loc_t location,
                                        digi_otn_tim_mode_t tim_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, tim_mode);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_exp_tti_tim_det_mode_set(digi_handle, 
                                                   otu_handle, 
                                                   (lineotn_sk_tim_det_mode_t)tim_mode);
            break;
            
        default:
            rc = digi_otn_odu_tim_det_mon_set(digi_handle, 
                                              odu_handle, 
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id, 
                                              (UINT8)tim_mode);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tim_det_mode_set */


/*******************************************************************************
* adpt_otn_tim_det_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves dTIM detection mode of an ODU/OTU framer channel in sink
*   position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tim_mode          - dTIM detection mode.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tim_det_mode_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        digi_otn_loc_t location,
                                        digi_otn_tim_mode_t *tim_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 tim_det_mo = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tim_mode);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_exp_tti_tim_det_mode_get(digi_handle,
                                                   otu_handle,
                                                   (lineotn_sk_tim_det_mode_t*)tim_mode);
            break;
            
        default:
            rc = digi_otn_odu_tim_det_mon_get(digi_handle, 
                                              odu_handle, 
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id, 
                                              (UINT8*)&tim_det_mo);
            *tim_mode = (digi_otn_tim_mode_t)tim_det_mo;                                   
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tim_det_mode_get */


/*******************************************************************************
* adpt_otn_tim_act_dis_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures dTIM consequent action in sink position, which is MI_TIMActDis 
*   in G.798.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   tim_act_dis        - dTIM consequent action disable status.
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
PUBLIC PMC_ERROR adpt_otn_tim_act_dis_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL tim_act_dis)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, tim_act_dis);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_consq_action_cfg(digi_handle,
                                               otu_handle,
                                               LINE_OTN_A_AIS,
                                               LINE_OTN_D_TIM,
                                               (BOOL8)!tim_act_dis);
            break;
            
        default:
            rc = digi_otn_odu_tim_act_dis_set(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              (BOOL8)tim_act_dis);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tim_act_dis_set */


/*******************************************************************************
* adpt_otn_tim_act_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures dTIM consequent action in sink position, which is MI_TIMActDis 
*   in G.798.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tim_act_dis       - dTIM consequent action disable status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tim_act_dis_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL *tim_act_dis)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT32 defect_bm = 0;
    BOOL8 tim_act_disable = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tim_act_dis);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_consq_action_cfg_get(digi_handle,
                                                   otu_handle,
                                                   LINE_OTN_A_AIS,
                                                   LINE_OTN_D_TIM,
                                                   &defect_bm);
            if (LINE_OTN_D_TIM == defect_bm)
            {
                *tim_act_dis = FALSE;
            }
            else
            {
                *tim_act_dis = TRUE;
            }
            break;
            
        default:
            rc = digi_otn_odu_tim_act_dis_get(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              &tim_act_disable);
            *tim_act_dis = tim_act_disable;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tim_act_dis_get */


/*******************************************************************************
* adpt_otn_tx_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted payload type in source position, which is PSI[0] 
*   byte.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pt_value           - payload type value.
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
PUBLIC PMC_ERROR adpt_otn_tx_pt_set(UINT32 dev_id, UINT32 chnl_id, UINT8 pt_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, pt_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            rc = digi_otn_opu_tx_pt_set(digi_handle, odu_handle, pt_value);
        }
        else
        {
            rc = PMC_ERR_FAIL;
        }
    }
    /* stage 4 process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_opu_tx_pt_set(digi_handle, map_handle, pt_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_pt_set */


/*******************************************************************************
* adpt_otn_tx_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted payload type in source position, which is PSI[0] 
*   byte.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pt_value          - payload type value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_pt_get(UINT32 dev_id, UINT32 chnl_id, UINT8 *pt_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pt_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            rc = digi_otn_opu_tx_pt_get(digi_handle, odu_handle, pt_value);
        }
        else
        {
            *pt_value = 0;
            rc = PMC_ERR_FAIL;
        }
    }
    /* stage 4 process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_opu_tx_pt_get(digi_handle, map_handle, pt_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_pt_get */


/*******************************************************************************
* adpt_otn_rx_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted payload type in sink position, which is PSI[0] 
*   byte.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pt_value          - payload type value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_pt_get(UINT32 dev_id, UINT32 chnl_id, UINT8 *pt_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pt_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            rc = digi_otn_opu_rx_accepted_pt_get(digi_handle, odu_handle, pt_value);
        }
        else
        {
            *pt_value = 0;
            rc = PMC_ERR_FAIL;
        }
    }
    /* stage 4 process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_opu_rx_accpt_pt_get(digi_handle, map_handle, pt_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_rx_pt_get */


/*******************************************************************************
* adpt_otn_exp_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected payload type in sink position, which is PSI[0] 
*   byte.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pt_value           - payload type value.
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
PUBLIC PMC_ERROR adpt_otn_exp_pt_set(UINT32 dev_id, UINT32 chnl_id, UINT8 pt_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, pt_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            rc = digi_otn_opu_rx_expected_pt_set(digi_handle, odu_handle, pt_value);
        }
        else
        {
            rc = PMC_ERR_FAIL;
        }
    }
    /* stage 4 process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_opu_rx_pt_set(digi_handle, map_handle, pt_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_pt_set */


/*******************************************************************************
* adpt_otn_exp_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected payload type in sink position, which is PSI[0] 
*   byte.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pt_value          - payload type value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_exp_pt_get(UINT32 dev_id, UINT32 chnl_id, UINT8 *pt_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pt_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            rc = digi_otn_opu_rx_expected_pt_get(digi_handle, odu_handle, pt_value);
        }
        else
        {
            *pt_value = 0;
            rc = PMC_ERR_FAIL;
        }
    }
    /* stage 4 process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_opu_rx_exp_pt_get(digi_handle, map_handle, pt_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_pt_get */


/*******************************************************************************
* adpt_otn_tx_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted multiplex structure identifier overhead in source 
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   trib_slot          - tributary slot of a HO/MO channel whose MSI value
*                        to be updated
*                          Valid range:
*                          ODU1 with 1G25 TS: 0 -- 1
*                          ODU1 with 2G5 TS : 0
*                          ODU2 with 1G25 TS: 0 -- 7
*                          ODU2 with 2G5 TS: 0 -- 3
*                          ODU3 with 1G25 TS : 0 -- 31
*                          ODU3 with 2G5 TS : 0 -- 15
*                          ODU4 with 1G25 TS: 0 -- 79
*                          ...
*   *msi_value         - MSI value.
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
*   Please note that only those active Tx tributory slots can be configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_msi_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  UINT8  trib_slot,
                                  UINT8  msi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               trib_slot, msi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = digi_otn_opu_tx_msi_set(digi_handle, odu_handle, trib_slot, msi_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_msi_set */


/*******************************************************************************
* adpt_otn_tx_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted multiplex structure identifier overhead in source 
*   position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *msi_value         - MSI value: PSI[2] ~ PSI[81].
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_msi_get(UINT32 dev_id, UINT32 chnl_id, UINT8 *msi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(msi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = digi_otn_opu_tx_msi_get(digi_handle, odu_handle, msi_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_msi_get */


/*******************************************************************************
* adpt_otn_rx_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the accepted multiplex structure identifier overhead in sink 
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *msi_value         - MSI value: PSI[2] ~ PSI[81].
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_msi_get(UINT32 dev_id, UINT32 chnl_id, UINT8 *msi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(msi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = digi_otn_opu_rx_accepted_msi_get(digi_handle, odu_handle, msi_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_rx_msi_get */


/*******************************************************************************
* adpt_otn_exp_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected multiplex structure identifier overhead in sink 
*   position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   trib_slot          - tributary slot of a HO/MO channel whose MSI value
*                        to be updated
*                          Valid range:
*                          ODU1 with 1G25 TS: 0 -- 1
*                          ODU1 with 2G5 TS : 0
*                          ODU2 with 1G25 TS: 0 -- 7
*                          ODU2 with 2G5 TS: 0 -- 3
*                          ODU3 with 1G25 TS : 0 -- 31
*                          ODU3 with 2G5 TS : 0 -- 15
*                          ODU4 with 1G25 TS: 0 -- 79
*                          ...
*   msi_value          - MSI value.
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
*   Please note that only those active Rx tributory slots can be configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_exp_msi_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8  trib_slot,
                                   UINT8  msi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               trib_slot, msi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = digi_otn_opu_rx_expected_msi_set(digi_handle, odu_handle, trib_slot, msi_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_msi_set */


/*******************************************************************************
* adpt_otn_exp_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected multiplex structure identifier overhead in sink 
*   position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *msi_value         - MSI value: PSI[2] ~ PSI[81].
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_exp_msi_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *msi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(msi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (NULL != adpt_handle->traffic_info[index].odu_handle))
    {
        rc = digi_otn_opu_rx_expected_msi_get(digi_handle, odu_handle, msi_value);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_exp_msi_get */


/*******************************************************************************
* adpt_otn_tx_ftfl_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted FTFL(Fault Type & Fault Location) overhead in 
*   source position, which is 20 bytes length altogether. Please note that DIGI 
*   device can support only 10 forward FTFL fields and 10 backward FTFL fields.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   ftfl_value         - FTFL value.
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
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_loc_t location,
                                 UINT8 ftfl_value[20])
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, chnl_id,  
               location, ftfl_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* FTFL configuration */
    rc = digi_otn_odu_tx_ftfl_set(digi_handle, 
                                  odu_handle,
                                  ftfl_value[0],
                                  &ftfl_value[1],
                                  ftfl_value[10],
                                  &ftfl_value[11]);

    PMC_RETURN(rc);
} /* adpt_otn_tx_ftfl_set */


/*******************************************************************************
* adpt_otn_tx_ftfl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted FTFL(Fault Type & Fault Location) overhead in 
*   source position, which is 20 bytes length altogether. Please note that DIGI 
*   device can support only 10 forward FTFL fields and 10 backward FTFL fields.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *ftfl_value        - FTFL value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_loc_t location,
                                 UINT8 *ftfl_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 ftfl_src = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ftfl_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_ftfl_get(digi_handle, 
                                  odu_handle,
                                  &ftfl_src,
                                  ftfl_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_ftfl_get */


/*******************************************************************************
* adpt_otn_tx_ftfl_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source selection of the transmitted FTFL(Fault Type & 
*   Fault Location) overhead insertion in source position, which is 20 bytes 
*   length altogether. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   oh_inst            - TRUE: enable FTFL insertion.
*                        FALSE: disable FTFL insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_src_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location,
                                    BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               location, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* Force FTFL value with both backward and forward field */
    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    rc = digi_otn_odu_ftfl_src_set(digi_handle, odu_handle, ins_src, 0);

    PMC_RETURN(rc);
} /* adpt_otn_tx_ftfl_src_set */


/*******************************************************************************
* adpt_otn_tx_ftfl_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted FTFL(Fault Type & 
*   Fault Location) overhead insertion in source position, which is 20 bytes 
*   length altogether. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable FTFL insertion.
*                        FALSE: disable FTFL insertion and passthrough it.
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_src_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location,
                                    BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 ftfl_src = 0;
    UINT8 ftfl_val[20];
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_ftfl_get(digi_handle, odu_handle, &ftfl_src, &ftfl_val[0]);
    *oh_inst = ftfl_src ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_otn_tx_ftfl_src_get */


/*******************************************************************************
* adpt_otn_rx_ftfl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted FTFL(Fault Type & Fault Location) overhead in 
*   sink position, which is 20 bytes length altogether. Please note that DIGI 
*   device can support only 10 forward FTFL fields and 10 backward FTFL fields.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *ftfl_value        - FTFL value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_ftfl_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_loc_t location,
                                 UINT8 *ftfl_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ftfl_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_accepted_ftfl_get(digi_handle, odu_handle, ftfl_value);

    PMC_RETURN(rc);
} /* adpt_otn_rx_ftfl_get */


/*******************************************************************************
* adpt_otn_tx_gcc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted GCC(General Communication Channel) field overhead 
*   in source position, which includes GCC0, GCC1 and GCC2. Please note that GCC0 
*   is only available at OTUk layer. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   gcc_type           - GCC type: GCC0, GCC1 and GCC2.
*   gcc_value          - GCC value.
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
PUBLIC PMC_ERROR adpt_otn_tx_gcc_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gcc_type_t gcc_type,
                                  UINT16 gcc_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT16 gcc_val = 0;
    UINT8 gcc_src = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, gcc_type, gcc_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (gcc_type)
    {
        case DIGI_OTN_GCC_0:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_GCC0,
                                         LINE_OTN_INS_TYPE_USER_VALUE,
                                         gcc_value);
            break;
            
        case DIGI_OTN_GCC_1:
            /* get GCC1 source/value */
            rc = digi_otn_odu_tx_gcc1_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            ADPT_RETURN_VERIFY(rc);

            ins_src = gcc_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            /* set GCC1 value */
            rc = digi_otn_odu_tx_gcc1_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              gcc_value);
            break;
            
        case DIGI_OTN_GCC_2:
            /* get GCC2 source/value */
            rc = digi_otn_odu_tx_gcc2_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            ADPT_RETURN_VERIFY(rc);

            ins_src = gcc_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            /* set GCC2 value */
            rc = digi_otn_odu_tx_gcc2_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              gcc_value);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_gcc_set */


/*******************************************************************************
* adpt_otn_tx_gcc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted GCC(General Communication Channel) field overhead 
*   in source position, which includes GCC0, GCC1 and GCC2. Please note that GCC0 
*   is only available at OTUk layer. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   gcc_type           - GCC type: GCC0, GCC1 and GCC2.
*
* OUTPUTS:
*   *gcc_value         - GCC value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_gcc_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gcc_type_t gcc_type,
                                  UINT16 *gcc_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 gcc_src = 0;
    lineotn_oh_ins_type_t ctrl_ins_type;
    UINT32 oh_value = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(gcc_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (gcc_type)
    {
        case DIGI_OTN_GCC_0:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_GCC0,
                                         &ctrl_ins_type,
                                         &oh_value);
            *gcc_value = (UINT16)oh_value;
            break;
            
        case DIGI_OTN_GCC_1:
            rc = digi_otn_odu_tx_gcc1_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          gcc_value);
            break;
            
        case DIGI_OTN_GCC_2:
            rc = digi_otn_odu_tx_gcc2_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          gcc_value);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_gcc_get */


/*******************************************************************************
* adpt_otn_tx_gcc_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source selection of the transmitted GCC(General Communication
*   Channel) field overhead insertion in source position, which includes GCC0,
*   GCC1 and GCC2. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   gcc_type           - GCC type: GCC0, GCC1 and GCC2.
*   oh_inst            - TRUE: enable GCC insertion.
*                        FALSE: disable GCC insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_gcc_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_gcc_type_t gcc_type,
                                     BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT16 gcc_val = 0;
    UINT8 gcc_src = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, gcc_type, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    switch (gcc_type)
    {
        case DIGI_OTN_GCC_0:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_GCC0,
                                         oh_inst ? LINE_OTN_INS_TYPE_USER_VALUE : LINE_OTN_INS_TYPE_UNMODIFIED,
                                         0);
            break;

        case DIGI_OTN_GCC_1:
            /* get GCC1 source/value */
            rc = digi_otn_odu_tx_gcc1_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            ADPT_RETURN_VERIFY(rc);

            /* set GCC1 source */
            rc = digi_otn_odu_tx_gcc1_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              gcc_val);
            break;
            
        case DIGI_OTN_GCC_2:
            /* get GCC2 source/value */
            rc = digi_otn_odu_tx_gcc2_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            ADPT_RETURN_VERIFY(rc);

            /* set GCC2 source */
            rc = digi_otn_odu_tx_gcc2_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              gcc_val);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_gcc_src_set */


/*******************************************************************************
* adpt_otn_tx_gcc_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted GCC(General 
*   Communication Channel) field overhead insertion in source position, which 
*   includes GCC0, GCC1 and GCC2. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   gcc_type           - GCC type: GCC0, GCC1 and GCC2.
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable GCC insertion.
*                        FALSE: disable GCC insertion and passthrough it.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_gcc_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_gcc_type_t gcc_type,
                                     BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT16 gcc_val = 0;
    UINT8 gcc_src = 0;
    lineotn_oh_ins_type_t ctrl_ins_type;
    UINT32 oh_value = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (gcc_type)
    {
        case DIGI_OTN_GCC_0:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_GCC0,
                                         &ctrl_ins_type,
                                         &oh_value);
            *oh_inst = (LINE_OTN_INS_TYPE_UNMODIFIED == ctrl_ins_type) ? FALSE : TRUE;
            break;
            
        case DIGI_OTN_GCC_1:
            /* get GCC1 source/value */
            rc = digi_otn_odu_tx_gcc1_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            *oh_inst = gcc_src;
            break;

        case DIGI_OTN_GCC_2:
            /* get GCC2 source/value */
            rc = digi_otn_odu_tx_gcc2_get(digi_handle, 
                                          odu_handle,
                                          &gcc_src,
                                          &gcc_val);
            *oh_inst = gcc_src;
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_gcc_src_get */


/*******************************************************************************
* adpt_otn_rx_gcc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted GCC(General Communication Channel) field overhead 
*   in sink position, which includes GCC0, GCC1 and GCC2. Please note that GCC0 
*   is only available at OTUk layer. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   gcc_type           - GCC type: GCC0, GCC1 and GCC2.
*
* OUTPUTS:
*   *gcc_value         - GCC value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_gcc_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gcc_type_t gcc_type,
                                  UINT16 *gcc_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT32 gcc0_value;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(gcc_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (gcc_type)
    {
        case DIGI_OTN_GCC_0:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_extract_get(digi_handle, 
                                             otu_handle,
                                             LINE_OTN_EXT_SK_GCC0,
                                             &gcc0_value);
            *gcc_value = gcc0_value & 0xFFFF;
            break;
            
        case DIGI_OTN_GCC_1:
            rc = digi_otn_odu_rx_accepted_gcc1_get(digi_handle, 
                                                   odu_handle,
                                                   gcc_value);
            break;
            
        case DIGI_OTN_GCC_2:
            rc = digi_otn_odu_rx_accepted_gcc2_get(digi_handle, 
                                                   odu_handle,
                                                   gcc_value);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_rx_gcc_get */


/*******************************************************************************
* adpt_otn_tx_exp_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted EXP(Experimental) field overhead in source 
*   position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   exp_value          - EXP value.
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
PUBLIC PMC_ERROR adpt_otn_tx_exp_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT16 exp_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 exp_src = 0;
    UINT16 exp_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, exp_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get EXP source/value */
    rc = digi_otn_odu_tx_exp_get(digi_handle, 
                                 odu_handle,
                                 &exp_src,
                                 &exp_val);
    ADPT_RETURN_VERIFY(rc);

    ins_src = exp_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    /* set EXP value */
    rc = digi_otn_odu_tx_exp_src_set(digi_handle, 
                                     odu_handle,
                                     ins_src,
                                     exp_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_exp_set */


/*******************************************************************************
* adpt_otn_tx_exp_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted EXP(Experimental) field overhead in source 
*   position. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *exp_value         - EXP value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_exp_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT16 *exp_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 exp_src = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(exp_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_exp_get(digi_handle, 
                                 odu_handle,
                                 &exp_src,
                                 exp_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_exp_get */


/*******************************************************************************
* adpt_otn_tx_exp_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source selection of transmitted EXP(Experimental) field 
*   overhead insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   oh_inst            - TRUE: enable EXP insertion.
*                        FALSE: disable EXP insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_exp_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 exp_src = 0;
    UINT16 exp_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get EXP source/value */
    rc = digi_otn_odu_tx_exp_get(digi_handle, 
                                 odu_handle,
                                 &exp_src,
                                 &exp_val);
    ADPT_RETURN_VERIFY(rc);

    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    /* set EXP source */
    rc = digi_otn_odu_tx_exp_src_set(digi_handle,
                                     odu_handle,
                                     ins_src,
                                     exp_val);

    PMC_RETURN(rc);
} /* adpt_otn_tx_exp_src_set */


/*******************************************************************************
* adpt_otn_tx_exp_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted EXP(Experimental) 
*   field overhead insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable EXP insertion.
*                        FALSE: disable EXP insertion and passthrough it.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_exp_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 exp_src = 0;
    UINT16 exp_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get EXP source/value */
    rc = digi_otn_odu_tx_exp_get(digi_handle, odu_handle, &exp_src, &exp_val);
    *oh_inst = exp_src;

    PMC_RETURN(rc);
} /* adpt_otn_tx_exp_src_get */


/*******************************************************************************
* adpt_otn_rx_exp_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted EXP(Experimental) field overhead in sink position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *exp_value         - EXP value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_exp_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT16 *exp_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(exp_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_exp_get(digi_handle, odu_handle, exp_value);

    PMC_RETURN(rc);
} /* adpt_otn_rx_exp_get */


/*******************************************************************************
* adpt_otn_tx_tcm_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted TCM ACT(TCM Activation) field overhead in source 
*   position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   tcm_act_value      - TCM ACT value.
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
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT8 tcm_act_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 tcm_act_src = 0;
    UINT8 tcm_act_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, tcm_act_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get TCM_ACT source/value */
    rc = digi_otn_odu_tx_tcm_act_get(digi_handle, 
                                     odu_handle,
                                     &tcm_act_src,
                                     &tcm_act_val);
    ADPT_RETURN_VERIFY(rc);

    ins_src = tcm_act_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    /* set TCM_ACT source */
    rc = digi_otn_odu_tx_tcm_act_src_set(digi_handle, 
                                         odu_handle,
                                         ins_src,
                                         tcm_act_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_tcm_act_set */


/*******************************************************************************
* adpt_otn_tx_tcm_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted TCM ACT(TCM Activation) field overhead in source 
*   position. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *tcm_act_value     - TCM ACT value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT8 *tcm_act_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 tcm_act_src = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tcm_act_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_tcm_act_get(digi_handle, 
                                     odu_handle,
                                     &tcm_act_src,
                                     tcm_act_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_tcm_act_get */


/*******************************************************************************
* adpt_otn_tx_tcm_act_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the transmitted TCM ACT(TCM Activation) field overhead 
*   insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   oh_inst            - TRUE: enable TCM ACT insertion.
*                        FALSE: disable TCM ACT insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_src_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 tcm_act_src = 0;
    UINT8 tcm_act_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get TCM_ACT source/value */
    rc = digi_otn_odu_tx_tcm_act_get(digi_handle, 
                                     odu_handle,
                                     &tcm_act_src,
                                     &tcm_act_val);
    ADPT_RETURN_VERIFY(rc);

    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    /* set TCM_ACT source */
    rc = digi_otn_odu_tx_tcm_act_src_set(digi_handle, 
                                         odu_handle,
                                         ins_src,
                                         tcm_act_val);

    PMC_RETURN(rc);
} /* adpt_otn_tx_tcm_act_src_set */


/*******************************************************************************
* adpt_otn_tx_tcm_act_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted TCM ACT(TCM Activation)
*   field overhead insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable TCM ACT insertion.
*                        FALSE: disable TCM ACT insertion and passthrough it.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_src_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 tcm_act_src = 0;
    UINT8 tcm_act_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get TCM_ACT source/value */
    rc = digi_otn_odu_tx_tcm_act_get(digi_handle, 
                                     odu_handle,
                                     &tcm_act_src,
                                     &tcm_act_val);
    *oh_inst = tcm_act_src;

    PMC_RETURN(rc);
} /* adpt_otn_tx_tcm_act_src_get */


/*******************************************************************************
* adpt_otn_rx_tcm_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted TCM ACT(TCM Activation) field overhead in sink 
*   position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *tcm_act_value     - TCM ACT value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_tcm_act_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT8 *tcm_act_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tcm_act_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_accepted_tcm_act_get(digi_handle, 
                                              odu_handle,
                                              tcm_act_value);

    PMC_RETURN(rc);
} /* adpt_otn_rx_tcm_act_get */


/*******************************************************************************
* adpt_otn_tx_res_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted RES(Reserved for future international standardization) 
*   field overhead in source position. RES field overhead includes RES(1,10)BIT7~8,
*   RES(1,13), RES(1,14), RES(2,1), RES(2,2), RES(4,9), RES(4,10), RES(4,11),
*   RES(4,12), RES(4,13) and RES(4,14). Please note that RES(1,10), RES(1,13), 
*   RES(1,14) overheads are available at OTUk layer. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   res_type           - RES overhead type.
*   res_value          - RES value.
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
PUBLIC PMC_ERROR adpt_otn_tx_res_set(UINT32     dev_id,
                                  UINT32        chnl_id,
                                  digi_res_oh_t res_type,
                                  UINT8         res_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 res_src = 0;
    UINT8 res_val = 0;
    UINT16 res1_val = 0;
    UINT8 res2_val[6];
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, res_type, res_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (res_type)
    {
        case DIGI_RES_1_10:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES7,
                                         LINE_OTN_INS_TYPE_USER_VALUE,
                                         (res_value & 0x2) >> 1);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES8,
                                         LINE_OTN_INS_TYPE_USER_VALUE,
                                         res_value & 0x1);
            ADPT_RETURN_VERIFY(rc);
            break;
            
        case DIGI_RES_1_13:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES13,
                                         LINE_OTN_INS_TYPE_USER_VALUE,
                                         res_value);
            break;
            
        case DIGI_RES_1_14:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES14,
                                         LINE_OTN_INS_TYPE_USER_VALUE,
                                         res_value);
            break;
            
        case DIGI_RES_2_1:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            ADPT_RETURN_VERIFY(rc);

            res1_val = (res1_val & 0xFF) | (res_value << 8);
            ins_src = res_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            rc = digi_otn_odu_tx_res1_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              res1_val);
            break;
            
        case DIGI_RES_2_2:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            ADPT_RETURN_VERIFY(rc);

            res1_val = (res1_val & 0xFF00) | res_value;
            ins_src = res_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            rc = digi_otn_odu_tx_res1_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              res1_val);
            break;
            
        case DIGI_RES_2_3:
            rc = digi_otn_odu_pm_tcm_res_get(digi_handle, 
                                             odu_handle,
                                             &res_src,
                                             &res_val);
            ADPT_RETURN_VERIFY(rc);

            ins_src = res_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            rc = digi_otn_odu_pm_tcm_res_src_set(digi_handle, 
                                                 odu_handle,
                                                 ins_src,
                                                 res_value & 0x1);
            break;
            
        case DIGI_RES_4_9:
        case DIGI_RES_4_10:
        case DIGI_RES_4_11:
        case DIGI_RES_4_12:
        case DIGI_RES_4_13:
        case DIGI_RES_4_14:
            rc = digi_otn_odu_tx_res2_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res2_val[0]);
            ADPT_RETURN_VERIFY(rc);

            res2_val[res_type - DIGI_RES_4_9] = res_value;
            ins_src = res_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
            rc = digi_otn_odu_tx_res2_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              &res2_val[0]);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_res_set */


/*******************************************************************************
* adpt_otn_tx_res_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted RES(Reserved for future international standardization) 
*   field overhead in source position. RES field overhead includes RES(1,10)BIT7~8,
*   RES(1,13), RES(1,14), RES(2,1), RES(2,2), RES(4,9), RES(4,10), RES(4,11),
*   RES(4,12), RES(4,13) and RES(4,14). Please note that RES(1,10), RES(1,13), 
*   RES(1,14) overheads are available at OTUk layer.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   res_type           - RES overhead type.
*
* OUTPUTS:
*   *res_value         - RES value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_res_get(UINT32     dev_id,
                                  UINT32        chnl_id,
                                  digi_res_oh_t res_type,
                                  UINT8         *res_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 res_src = 0;
    UINT32 res_val = 0;
    UINT16 res1_val = 0;
    UINT8 res2_val[6];
    lineotn_oh_ins_type_t ctrl_ins_type;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(res_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (res_type)
    {
        case DIGI_RES_1_10:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES7,
                                         &ctrl_ins_type,
                                         &res_val);
            ADPT_RETURN_VERIFY(rc);

            *res_value = (res_val & 0x1) << 1;
            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES8,
                                         &ctrl_ins_type,
                                         &res_val);
            *res_value |= res_val & 0x1;
            break;

        case DIGI_RES_1_13:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES13,
                                         &ctrl_ins_type,
                                         &res_val);
            *res_value = res_val;
            break;

        case DIGI_RES_1_14:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES14,
                                         &ctrl_ins_type,
                                         &res_val);
            *res_value = res_val;
            break;
            
        case DIGI_RES_2_1:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            *res_value = res1_val >> 8;
            break;
            
        case DIGI_RES_2_2:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            *res_value = res1_val & 0xFF;
            break;
            
        case DIGI_RES_2_3:
            rc = digi_otn_odu_pm_tcm_res_get(digi_handle, 
                                             odu_handle,
                                             &res_src,
                                             res_value);
            break;
            
        case DIGI_RES_4_9:
        case DIGI_RES_4_10:
        case DIGI_RES_4_11:
        case DIGI_RES_4_12:
        case DIGI_RES_4_13:
        case DIGI_RES_4_14:
            rc = digi_otn_odu_tx_res2_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res2_val[0]);
            *res_value = res2_val[res_type - DIGI_RES_4_9];
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_res_get */


/*******************************************************************************
* adpt_otn_tx_res_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source selection of transmitted RES(Reserved for future 
*   international standardization) field overhead insertion in source position. 
*   RES field overhead includes RES(1,10)BIT7~8, RES(1,13), RES(1,14), RES(2,1),
*   RES(2,2), RES(4,9), RES(4,10), RES(4,11), RES(4,12), RES(4,13) and RES(4,14). 
*
*   Please note that RES(1,10), RES(1,13), RES(1,14) overheads are available 
*   at OTUk layer.
*
*   Please note that the following overheads are in one group.
*   RES1 group: DIGI_RES_2_1, DIGI_RES_2_2
*   RES2 group: DIGI_RES_4_9/10/11/12/13/14
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   res_type           - RES overhead type.
*   oh_inst            - TRUE: enable RES insertion.
*                        FALSE: disable RES insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_res_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_res_oh_t res_type,
                                     BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 res_src = 0;
    UINT8 res_val = 0;
    UINT16 res1_val = 0;
    UINT8 res2_val[6];
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, res_type, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    switch (res_type)
    {
        case DIGI_RES_1_10:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES7,
                                         oh_inst ? LINE_OTN_INS_TYPE_USER_VALUE : LINE_OTN_INS_TYPE_UNMODIFIED,
                                         0);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES8,
                                         oh_inst ? LINE_OTN_INS_TYPE_USER_VALUE : LINE_OTN_INS_TYPE_UNMODIFIED,
                                         0);
            break;

        case DIGI_RES_1_13:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES13,
                                         oh_inst ? LINE_OTN_INS_TYPE_USER_VALUE : LINE_OTN_INS_TYPE_UNMODIFIED,
                                         0);
            break;

        case DIGI_RES_1_14:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES14,
                                         oh_inst ? LINE_OTN_INS_TYPE_USER_VALUE : LINE_OTN_INS_TYPE_UNMODIFIED,
                                         0);
            break;

        case DIGI_RES_2_1:
        case DIGI_RES_2_2:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_tx_res1_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              res1_val);
            break;
            
        case DIGI_RES_2_3:
            rc = digi_otn_odu_pm_tcm_res_get(digi_handle, 
                                             odu_handle,
                                             &res_src,
                                             &res_val);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_pm_tcm_res_src_set(digi_handle, 
                                                 odu_handle,
                                                 ins_src,
                                                 res_val);
            break;
            
        case DIGI_RES_4_9:
        case DIGI_RES_4_10:
        case DIGI_RES_4_11:
        case DIGI_RES_4_12:
        case DIGI_RES_4_13:
        case DIGI_RES_4_14:
            rc = digi_otn_odu_tx_res2_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res2_val[0]);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_tx_res2_src_set(digi_handle, 
                                              odu_handle,
                                              ins_src,
                                              res2_val);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_res_src_set */


/*******************************************************************************
* adpt_otn_res_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted RES(Reserved for 
*   future international standardization) field overhead insertion in source 
*   position. RES field overhead includes RES(1,10)BIT7~8, RES(1,13), RES(1,14),
*   RES(2,1), RES(2,2), RES(4,9), RES(4,10), RES(4,11), RES(4,12), RES(4,13) 
*   and RES(4,14). 
*
*   Please note that RES(1,10), RES(1,13), RES(1,14) overheads are available 
*   at OTUk layer.
*
*   Please note that the following overheads are in one group.
*   RES1 group: DIGI_RES_2_1, DIGI_RES_2_2
*   RES2 group: DIGI_RES_4_9/10/11/12/13/14
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   res_type           - RES overhead type.
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable RES insertion.
*                        FALSE: disable RES insertion and passthrough it.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_res_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_res_oh_t res_type,
                                     BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 res_src = 0;
    UINT8 res_val = 0;
    UINT16 res1_val = 0;
    UINT8 res2_val[6];
    lineotn_oh_ins_type_t ctrl_ins_type;
    UINT32 oh_value = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (res_type)
    {
        case DIGI_RES_1_10:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES7,
                                         &ctrl_ins_type,
                                         &oh_value);
            *oh_inst = (LINE_OTN_INS_TYPE_UNMODIFIED == ctrl_ins_type) ? FALSE : TRUE;
            break;

        case DIGI_RES_1_13:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES13,
                                         &ctrl_ins_type,
                                         &oh_value);
            *oh_inst = (LINE_OTN_INS_TYPE_UNMODIFIED == ctrl_ins_type) ? FALSE : TRUE;
            break;

        case DIGI_RES_1_14:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         LINE_OTN_SO_RES14,
                                         &ctrl_ins_type,
                                         &oh_value);
            *oh_inst = (LINE_OTN_INS_TYPE_UNMODIFIED == ctrl_ins_type) ? FALSE : TRUE;
            break;
            
        case DIGI_RES_2_1:
        case DIGI_RES_2_2:
            rc = digi_otn_odu_tx_res1_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res1_val);
            *oh_inst = res_src;
            break;
            
        case DIGI_RES_2_3:
            rc = digi_otn_odu_pm_tcm_res_get(digi_handle, 
                                             odu_handle,
                                             &res_src,
                                             &res_val);
            *oh_inst = res_src;
            break;
            
        case DIGI_RES_4_9:
        case DIGI_RES_4_10:
        case DIGI_RES_4_11:
        case DIGI_RES_4_12:
        case DIGI_RES_4_13:
        case DIGI_RES_4_14:
            rc = digi_otn_odu_tx_res2_get(digi_handle, 
                                          odu_handle,
                                          &res_src,
                                          &res2_val[0]);
            *oh_inst = res_src;
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_res_src_get */


/*******************************************************************************
* adpt_otn_rx_res_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted RES(Reserved for future international standardization) 
*   field overhead in source position. RES field overhead includes RES(1,10)BIT7~8,
*   RES(1,13), RES(1,14), RES(2,1), RES(2,2), RES(4,9), RES(4,10), RES(4,11),
*   RES(4,12), RES(4,13) and RES(4,14). Please note that RES(1,10), RES(1,13), 
*   RES(1,14) overheads are available at OTUk layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   res_type           - RES overhead type.
*
* OUTPUTS:
*   *res_value         - RES value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_res_get(UINT32     dev_id,
                                  UINT32        chnl_id,
                                  digi_res_oh_t res_type,
                                  UINT8         *res_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT16 res1_val = 0;
    UINT8 res2_val[6];
    UINT32 otu_res = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(res_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (res_type)
    {
        case DIGI_RES_1_10:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_extract_get(digi_handle, 
                                             otu_handle,
                                             LINE_OTN_EXT_SK_SM_RES,
                                             &otu_res);
            *res_value = otu_res & 0x3;
            break;
        
        case DIGI_RES_1_13:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_extract_get(digi_handle, 
                                             otu_handle,
                                             LINE_OTN_EXT_SK_RES,
                                             &otu_res);
            *res_value = (otu_res & 0xFF00) >> 8;
            break;
            
        case DIGI_RES_1_14:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_oh_extract_get(digi_handle, 
                                             otu_handle,
                                             LINE_OTN_EXT_SK_RES,
                                             &otu_res);
            *res_value = otu_res & 0xFF;
            break;
            
        case DIGI_RES_2_1:
            rc = digi_otn_odu_rx_res1_get(digi_handle, odu_handle, &res1_val);
            *res_value = res1_val >> 8;
            break;
            
        case DIGI_RES_2_2:
            rc = digi_otn_odu_rx_res1_get(digi_handle, odu_handle, &res1_val);
            *res_value = res1_val & 0xFF;
            break;
            
        case DIGI_RES_2_3:
            rc = digi_otn_odu_rx_pm_tcm_get(digi_handle, odu_handle, &res2_val[0]);
            *res_value = res2_val[0] & 0x1;
            break;

        case DIGI_RES_4_9:
        case DIGI_RES_4_10:
        case DIGI_RES_4_11:
        case DIGI_RES_4_12:
        case DIGI_RES_4_13:
        case DIGI_RES_4_14:
            rc = digi_otn_odu_rx_res2_get(digi_handle, odu_handle, &res2_val[0]);
            *res_value = res2_val[res_type - DIGI_RES_4_9];
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_rx_res_get */


/*******************************************************************************
* adpt_otn_tx_aps_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted APS PCC field overhead in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   aps_value          - APS PCC value.
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
PUBLIC PMC_ERROR adpt_otn_tx_aps_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 aps_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT8 aps_src = 0;
    UINT8 mon_level = (DIGI_OTN_SEG_SM == seg_id) ? 7 : seg_id;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][0x%x].\r\n", dev_id, 
               chnl_id, seg_id, aps_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get APS/PCC source */
    rc = digi_otn_odu_tx_aps_pcc_src_get(digi_handle, 
                                         odu_handle,
                                         mon_level,
                                         &aps_src);
    ADPT_RETURN_VERIFY(rc);

    ins_src = aps_src ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    /* set APS/PCC source/value */
    rc = digi_otn_odu_aps_pcc_src_set(digi_handle, 
                                      odu_handle,
                                      mon_level,
                                      ins_src,
                                      aps_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_aps_set */


/*******************************************************************************
* adpt_otn_tx_aps_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted APS PCC field overhead in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*
* OUTPUTS:
*   *aps_value         - APS PCC value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_aps_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 *aps_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 mon_level = (DIGI_OTN_SEG_SM == seg_id) ? 7 : seg_id;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(aps_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_aps_pcc_get(digi_handle, 
                                     odu_handle,
                                     mon_level,
                                     aps_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_aps_get */


/*******************************************************************************
* adpt_otn_tx_aps_src_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source selection of transmitted APS PCC field overhead 
*   insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   oh_inst            - TRUE: enable APS PCC insertion.
*                        FALSE: disable APS PCC insertion and passthrough it.
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
PUBLIC PMC_ERROR adpt_otn_tx_aps_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     BOOL oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_odu_oh_ins_src_t ins_src = DIGI_OTN_ODU_OH_INS_INCOMING;
    UINT32 aps_val = 0;
    UINT8 mon_level = (DIGI_OTN_SEG_SM == seg_id) ? 7 : seg_id;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get APS/PCC source */
    rc = digi_otn_odu_tx_aps_pcc_get(digi_handle, 
                                     odu_handle,
                                     mon_level,
                                     &aps_val);
    ADPT_RETURN_VERIFY(rc);

    /* set APS/PCC source/value */
    ins_src = oh_inst ? DIGI_OTN_ODU_OH_INS_REGISTER_FORCE : DIGI_OTN_ODU_OH_INS_INCOMING;
    rc = digi_otn_odu_aps_pcc_src_set(digi_handle, 
                                      odu_handle,
                                      mon_level,
                                      ins_src,
                                      aps_val);

    PMC_RETURN(rc);
} /* adpt_otn_tx_aps_src_set */


/*******************************************************************************
* adpt_otn_tx_aps_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the source status of the transmitted APS PCC field overhead 
*   insertion in source position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*
* OUTPUTS:
*   *oh_inst           - TRUE: enable APS PCC insertion.
*                        FALSE: disable APS PCC insertion and passthrough it.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_aps_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     BOOL *oh_inst)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 aps_src = 0;
    UINT8 mon_level = (DIGI_OTN_SEG_SM == seg_id) ? 7 : seg_id;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_inst);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get APS/PCC source */
    rc = digi_otn_odu_tx_aps_pcc_src_get(digi_handle, 
                                         odu_handle,
                                         mon_level,
                                         &aps_src);
    *oh_inst = aps_src;

    PMC_RETURN(rc);
} /* adpt_otn_tx_aps_src_get */


/*******************************************************************************
* adpt_otn_rx_aps_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted APS PCC field overhead in sink position. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*
* OUTPUTS:
*   *aps_value         - APS PCC value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_aps_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 *aps_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 mon_level = (DIGI_OTN_SEG_SM == seg_id) ? 7 : seg_id;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(aps_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_aps_pcc_get(digi_handle, 
                                     odu_handle,
                                     mon_level,
                                     aps_value);

    PMC_RETURN(rc);
} /* adpt_otn_rx_aps_get */


/*******************************************************************************
* adpt_otn_tcm_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the TCM(Tandem Connection Monitoring) mode, which includes 
*   TANSPARENT, MONITOR and OPERATIONAL. 
*   
*   Please note that the TCM resource is limited with 256 numbers at most in one 
*   stage which mode is MONITOR or OPERATIONAL. When the resource is over than 
*   the maximum number, adpt_otn_tcm_mode_set() interface will return with error 
*   information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for PM, TCM1 - TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*   tcm_mode           - TCM mode: TRANSPARENT, MONITOR and OPERATIONAL.
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
*   TCM description.
*             Line side(stage 1/2/3A/N)     System side(Stage 3B)
*   -->            Sink Pos                     Source Pos      -->  ODUk SW 
*   <--            Source Pos                   Sink Pos        <--  ODUk SW
*
*   1. As to DIGI_OTN_SEG_PM, DIGI_TCM_NON_INTRUSIVE_MONITOR should be used 
*      instead of DIGI_TCM_MONITOR mode for none mux/demux TP.
*   2. As to DIGI_OTN_SEG_PM_NIM, the TCM mode is only available for 
*      DIGI_TCM_TRANSPARENT/DIGI_TCM_NON_INTRUSIVE_MONITOR at the sink position. 
*      It will be connected to DIGI_OTN_SEG_DCI from the SM segment. 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_mode_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_otn_pos_t position,
                                     digi_tcm_mode_t tcm_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT16 pool_id = 0;
    digi_otn_seg_t p_seg_id = DIGI_OTN_SEG_DCI;
    UINT8 tcm_enable = DIGI_TCM_ENABLE;
    digi_tcm_mode_t org_tcm_mode = DIGI_TCM_TRANSPARENT;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", 
               dev_id, chnl_id, seg_id, location, position, tcm_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id))
        || ((DIGI_OTN_SEG_PM_NIM == seg_id) && (DIGI_OTN_SINK_POS != position))
        || (location >= LAST_DIGI_OTN_SIDE)
        || (position >= LAST_DIGI_OTN_POS))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* sink condition */
    if ((DIGI_OTN_SINK_POS == position) 
         && (DIGI_TCM_OPERATIONAL != tcm_mode)
         && (DIGI_TCM_TRANSPARENT != tcm_mode)
         && (DIGI_TCM_MONITOR != tcm_mode)
         && (DIGI_TCM_NON_INTRUSIVE_MONITOR != tcm_mode)
         && (DIGI_TCM_OPERATIONAL_OHNZ != tcm_mode)
         && (DIGI_TCM_ERASE != tcm_mode))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    } 

    /* source condition */
    if ((DIGI_OTN_SOURCE_POS == position) 
         && (DIGI_TCM_OPERATIONAL != tcm_mode)
         && (DIGI_TCM_TRANSPARENT != tcm_mode))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    } 

    /* get parent segement ID */
    rc |= adpt_parent_segid_get(dev_id, chnl_id, seg_id, location, position, &p_seg_id);
    ADPT_RETURN_VERIFY(rc);

    /* TCM mode configuration */
    switch (position)
    {
        case DIGI_OTN_SINK_POS:
            /* get TCM mode from device */
            result = digi_otn_odu_pm_tcmi_mode_get(digi_handle, 
                                                   odu_handle,
                                                   1,
                                                   (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                   (odu_struct_pm_tcm_mode_t*)&org_tcm_mode);
            if (PMC_SUCCESS != result)
            {
                org_tcm_mode = DIGI_TCM_TRANSPARENT;
            }

            if (org_tcm_mode != tcm_mode)
            {
                /***********************************************************
                 * process the following configuration switch:
                 * 1. OPERATIONAL -> MONITOR
                 * 2. MONITOR -> OPERATIONAL
                 ***********************************************************/
                if ((DIGI_TCM_TRANSPARENT != tcm_mode) && (DIGI_TCM_TRANSPARENT != org_tcm_mode))
                {
                    /* TCM mode set */                                    
                    rc = digi_otn_odu_rx_tcmi_mode_update(digi_handle, 
                                                          odu_handle,
                                                          (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                          (odu_struct_pm_tcm_mode_t)tcm_mode);
                    ADPT_RETURN_VERIFY(rc);

                    /* pool ID */
                    pool_id = DIGI_TCM_ID_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+position][seg_id]);
                }
                else
                {
                    /* TCM mode set */                                    
                    rc = digi_otn_odu_rx_pm_tcmi_set(digi_handle, 
                                                     odu_handle,
                                                     (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                     0,
                                                     0,
                                                     0,
                                                     (odu_struct_odu_frm_pm_tcm_id_t)p_seg_id,
                                                     (odu_struct_pm_tcm_mode_t)tcm_mode,
                                                     &pool_id);
                    ADPT_RETURN_VERIFY(rc);
                }

                /* add a workaround to disable LTCAct by default */
                if ((DIGI_TCM_OPERATIONAL == tcm_mode) || (DIGI_TCM_OPERATIONAL_OHNZ == tcm_mode))
                {
                    rc = digi_otn_odu_tcmi_ltc_to_ais_tsf_set(digi_handle, 
                                                              odu_handle,
                                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                              TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }

                /* delay 76 ms */
                if ((DIGI_TCM_TRANSPARENT != tcm_mode) 
                    && (DIGI_TCM_NON_INTRUSIVE_MONITOR != tcm_mode)
                    && (DIGI_TCM_ERASE != tcm_mode))
                {
                    PMC_OS_USLEEP(76000);
                }
                rc = digi_otn_odu_rx_pm_tcmi_activate(digi_handle, odu_handle);
            }
            break;

        case DIGI_OTN_SOURCE_POS:
            /* get TCM mode from device */
            result = digi_otn_odu_pm_tcmi_mode_get(digi_handle, 
                                                   odu_handle,
                                                   0,
                                                   (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                   (odu_struct_pm_tcm_mode_t*)&org_tcm_mode);
            if (PMC_SUCCESS != result)
            {
                org_tcm_mode = DIGI_TCM_TRANSPARENT;
            }

            if (org_tcm_mode != tcm_mode)
            {
                /* TCM mode set */                                    
                rc = digi_otn_odu_tx_pm_tcmi_set(digi_handle, 
                                                 odu_handle,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                 0,
                                                 0,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)p_seg_id,
                                                 (odu_struct_pm_tcm_mode_t)tcm_mode,
                                                 &pool_id);
            }
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    /* record */
    if ((PMC_SUCCESS == rc) && (org_tcm_mode != tcm_mode))
    {
        if (DIGI_TCM_TRANSPARENT == tcm_mode)              
        {
            tcm_enable = 0;
        }

        if (DIGI_OTN_SEG_PM != seg_id)
        {
            /* take the PM position for DIGI_OTN_SEG_PM_NIM */
            tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;

            adpt_handle->traffic_info[index].tcm_pool_id[location*2+position][tcm_index] = DIGI_TCM_SET(tcm_enable, tcm_mode, pool_id);
        }
    }

    PMC_RETURN(rc);
} /* adpt_otn_tcm_mode_set */


/*******************************************************************************
* adpt_otn_tcm_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the TCM(Tandem Connection Monitoring) mode, which includes 
*   TANSPARENT, MONITOR and OPERATIONAL. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for TCM1 - TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*
* OUTPUTS:
*   *tcm_mode          - TCM mode: TRANSPARENT, MONITOR and OPERATIONAL.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_mode_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_otn_pos_t position,
                                     digi_tcm_mode_t *tcm_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    odu_struct_pm_tcm_mode_t org_tcm_mode;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tcm_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id))
        || (location >= LAST_DIGI_OTN_SIDE)
        || (position >= LAST_DIGI_OTN_POS))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get TCM mode */
    switch (position)
    {
        case DIGI_OTN_SINK_POS:
            rc = digi_otn_odu_pm_tcmi_mode_get(digi_handle, 
                                               odu_handle,
                                               1,
                                               (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                               &org_tcm_mode);
            break;

        case DIGI_OTN_SOURCE_POS:
            rc = digi_otn_odu_pm_tcmi_mode_get(digi_handle, 
                                               odu_handle,
                                               0,
                                               (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                               &org_tcm_mode);
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    *tcm_mode = (digi_tcm_mode_t)org_tcm_mode;

    PMC_RETURN(rc);
} /* adpt_otn_tcm_mode_get */


/*******************************************************************************
* adpt_otn_tcm_pri_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the TCM(Tandem Connection Monitoring) process priority. 
*   
*                             TCM default priority
*   Sink: SM -> TCM6 -> TCM5 -> TCM4 -> TCM3 -> TCM2 -> TCM1 -> PM
*   Source: PM -> TCM1 -> TCM2 -> TCM3 -> TCM4 -> TCM5 -> TCM6 -> SM
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*   tcm_pri            - TCM priority.
*                        tcm_pri[0] -- PM priority
*                        tcm_pri[1] -- TCM1 priority
*                        tcm_pri[2] -- TCM2 priority
*                        tcm_pri[3] -- TCM3 priority
*                        tcm_pri[4] -- TCM4 priority
*                        tcm_pri[5] -- TCM5 priority
*                        tcm_pri[6] -- TCM6 priority
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
*   TCM description.
*             Line side(stage 1/2/3A/4/N)       System side(Stage 3B)
*   -->            Sink Pos                     Source Pos      -->  ODUk SW 
*   <--            Source Pos                   Sink Pos        <--  ODUk SW
*
*   Please note:
*   1. The priority of any TCMi shouldn't be the same. Otherwise, it will return 
*      failure.
*   2. This API should be configured on condition that the TCM mode of all layers
*      is TRANSPARENT at this source/sink position.
*   3. The PM pririoty can't be configured that it is always the last one at the
*      sink position and the first one at the source position. 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_pri_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_tcm_pri_t tcm_pri[7])
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 tcm_index = 0;
    UINT32 tcm_index1 = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%p].\r\n", 
               dev_id, chnl_id, location, position, tcm_pri);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if ((location >= LAST_DIGI_OTN_SIDE) || (position >= LAST_DIGI_OTN_POS))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* TCM priority check */
    for (tcm_index = 0; tcm_index < 7; tcm_index++)
    {
        for (tcm_index1 = (tcm_index + 1); tcm_index1 < 7; tcm_index1++)
        {
            if ((tcm_pri[tcm_index] == tcm_pri[tcm_index1])
                || (tcm_pri[tcm_index] >= LAST_DIGI_TCM_PRI))
            {
                PMC_RETURN(PMC_ERR_FAIL);
            }
        }
    }

    for (tcm_index = 0; tcm_index < 7; tcm_index++)
    {
        adpt_handle->traffic_info[index].tcm_pri[location*2+position][tcm_index] = (UINT8)tcm_pri[tcm_index];
    }

    PMC_RETURN(rc);
} /* adpt_otn_tcm_pri_set */


/*******************************************************************************
* adpt_otn_tcm_pri_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the TCM(Tandem Connection Monitoring) process priority. 
*   
*                             TCM default priority
*   Sink: SM -> TCM6 -> TCM5 -> TCM4 -> TCM3 -> TCM2 -> TCM1 -> PM
*   Source: PM -> TCM1 -> TCM2 -> TCM3 -> TCM4 -> TCM5 -> TCM6 -> SM
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for TCM1 - TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*
* OUTPUTS:
*   *tcm_pri           - TCM priority.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   TCM description.
*             Line side(stage 1/2/3A/N)     System side(Stage 3B)
*   -->            Sink Pos                     Source Pos      -->  ODUk SW 
*   <--            Source Pos                   Sink Pos        <--  ODUk SW
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_pri_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_tcm_pri_t *tcm_pri)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tcm_pri);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if ((location >= LAST_DIGI_OTN_SIDE) 
         || (position >= LAST_DIGI_OTN_POS)
         || (seg_id > DIGI_OTN_SEG_TCM6))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    *tcm_pri = (digi_tcm_pri_t)adpt_handle->traffic_info[index].tcm_pri[location*2+position][seg_id];

    PMC_RETURN(rc);
} /* adpt_otn_tcm_pri_get */


/*******************************************************************************
* adpt_otn_tcm_parent_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API reconnects the specified TCM layer with DIGI_TCM_NON_INTRUSIVE_MONITOR
*   to a new parent layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for TCM1 - TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   p_seg_id           - parent segment identity. Only be available for TCM1 - 
*                        TCM6, DCI.
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
*   1. The TCM mode of seg_id must be the DIGI_TCM_NON_INTRUSIVE_MONITOR mode.
*   2. If the p_seg_id isn't DIGI_OTN_SEG_DCI, the TCM mode of p_seg_id 
*      shouldn't be the DIGI_TCM_NON_INTRUSIVE_MONITOR/DIGI_TCM_TRANSPARENT mode.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_parent_set(UINT32 dev_id, 
                                      UINT32 chnl_id, 
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      digi_otn_seg_t p_seg_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT16 tcm_pool_id = 0;
    UINT16 p_tcm_pool_id = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", 
               dev_id, chnl_id, seg_id, location, p_seg_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if ((seg_id > DIGI_OTN_SEG_TCM6) ||
        (seg_id < DIGI_OTN_SEG_TCM1) ||
        (p_seg_id > DIGI_OTN_SEG_DCI) ||
        (p_seg_id < DIGI_OTN_SEG_TCM1) ||
        (p_seg_id == DIGI_OTN_SEG_PM_NIM) ||
        (location >= LAST_DIGI_OTN_SIDE))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    tcm_pool_id = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][seg_id];
    if ((DIGI_TCM_NON_INTRUSIVE_MONITOR != DIGI_TCM_MODE_GET(tcm_pool_id)) ||
        (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(tcm_pool_id)))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_OTN_SEG_DCI != p_seg_id)
    {
        p_tcm_pool_id = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][p_seg_id];
        if ((DIGI_TCM_NON_INTRUSIVE_MONITOR == DIGI_TCM_MODE_GET(p_tcm_pool_id)) ||
            (DIGI_TCM_TRANSPARENT == DIGI_TCM_MODE_GET(p_tcm_pool_id)) ||
            (DIGI_TCM_ENABLE != DIGI_TCM_EN_GET(p_tcm_pool_id)))
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    rc = digi_otn_odu_rx_pm_tcmi_nim_parent_update(digi_handle, odu_handle, 
                                                   (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                   (odu_struct_odu_frm_pm_tcm_id_t)p_seg_id);
    ADPT_RETURN_VERIFY(rc);

    /* activate this TCM priority change */
    rc = digi_otn_odu_rx_pm_tcmi_activate(digi_handle, odu_handle);

    PMC_RETURN(rc);
} /* adpt_otn_pm_tcm_parent_set */


/*******************************************************************************
* adpt_otn_ltc_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the dLTC consequent action status in sink position. If ltc_act to 
*   TRUE, aAIS will be followed in the downstream when dLTC is raised. Otherwise, 
*   aAIS will be not done.  
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   ltc_act            - TRUE:  enable aAIS for dLTC.
*                        FALSE: disable aAIS for dLTC.
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
PUBLIC PMC_ERROR adpt_otn_ltc_act_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL ltc_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, ltc_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tcmi_ltc_to_ais_tsf_set(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              !ltc_act);
    
    PMC_RETURN(rc);
} /* adpt_otn_ltc_act_set */


/*******************************************************************************
* adpt_otn_ltc_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the dLTC consequent action status in sink position. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *ltc_act           - TRUE:  enable aAIS for dLTC.
*                        FALSE: disable aAIS for dLTC.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ltc_act_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL *ltc_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    BOOL8 ltc_dis = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ltc_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tcmi_ltc_to_ais_tsf_get(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              &ltc_dis);
    /* parameter conversion */
    *ltc_act = !ltc_dis;

    PMC_RETURN(rc);
} /* adpt_otn_ltc_act_get */


/*******************************************************************************
* adpt_otn_ais_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the aAIS consequent action status in sink position. If ais_act is 
*   TRUE, AIS signal will be followed in the downstream when aAIS is raised. 
*   Otherwise, there is no effect on downstream.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   ais_act            - TRUE:  enable aAIS consequent action.
*                        FALSE: disable aAIS consequent action.
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
*   Please note that only TCM1 ~ TCM6 are available.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ais_act_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL ais_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, ais_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_ais_dis_set(digi_handle, 
                                          odu_handle,
                                          (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                          !ais_act);
    
    PMC_RETURN(rc);
} /* adpt_otn_ais_act_set */


/*******************************************************************************
* adpt_otn_ais_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the aAIS consequent action status in sink position. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *ais_act           - TRUE:  enable aAIS for dAIS.
*                        FALSE: disable aAIS for dAIS.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ais_act_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL *ais_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    BOOL8 ais_dis = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ais_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_ais_dis_get(digi_handle, 
                                          odu_handle,
                                          (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                          &ais_dis);
    /* parameter conversion */
    *ais_act = !ais_dis;

    PMC_RETURN(rc);
} /* adpt_otn_ais_act_get */


/*******************************************************************************
* adpt_otn_tsd_tsf_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   configures the aTSD and aTSF consequent actions at the sink position if they   
*   are generated as a result of TSD or TSF defect detection.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   tsd_act            - TRUE:  enable aTSD consequent action.
*                        FALSE: disable aTSD consequent action.
*   tsf_act            - TRUE:  enable aTSF consequent action.
*                        FALSE: disable aTSF consequent action.
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
PUBLIC PMC_ERROR adpt_otn_tsd_tsf_act_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL tsd_act,
                                     BOOL tsf_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, tsd_act, tsf_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_tsd_tsf_dis_set(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              2, 
                                              (UINT8)(!tsd_act),
                                              (UINT8)(!tsf_act));

    PMC_RETURN(rc);
} /* adpt_otn_tsd_tsf_act_set */


/*******************************************************************************
* adpt_otn_tsd_tsf_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the aTSD and aTSF consequent actions at the sink position if they   
*   are generated as a result of TSD or TSF defect detection.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *tsd_act           - TRUE:  enable aTSD consequent action.
*                        FALSE: disable aTSD consequent action.
*   *tsf_act           - TRUE:  enable aTSF consequent action.
*                        FALSE: disable aTSF consequent action.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tsd_tsf_act_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL *tsd_act,
                                     BOOL *tsf_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 tsd_dis = 0;
    UINT8 tsf_dis = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tsd_act);
    ADPT_PARA_VERIFY(tsf_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_tsd_tsf_dis_get(digi_handle, 
                                              odu_handle,
                                              (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                              &tsd_dis,
                                              &tsf_dis);
    *tsd_act = (0 == tsd_dis) ? TRUE : FALSE;
    *tsf_act = (0 == tsf_dis) ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_otn_tsd_tsf_act_get */


/*******************************************************************************
* adpt_otn_dm_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the DM(Delay Measurement) mode: disable, initiator, loopback. 
*   When a TCMi is OPERATIONAL or ODUkP overhead insertion is enabled, the 
*   corresponding bit (DMti or DMp) in the PM&TCM byte is inserted based on 
*   whether the delay measurement state machine is provisioned as a source.
*   
*   Please note that initiator/loopback mode should be under OPERATIONAL state 
*   of TCMi or overhead insertion state of ODUkP.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity for PM and TCMi only.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   dm_mode            - DM mode: initiator, loopback.
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
PUBLIC PMC_ERROR adpt_otn_dm_mode_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_dm_mode_t dm_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id, location, dm_mode);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (dm_mode)
    {
        case DIGI_DM_INITIATOR:
            /* enable DM */
            rc = digi_otn_odu_delay_measure_init(digi_handle, 
                                                 odu_handle,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                 DIGI_OTN_DM_SOURCE,
                                                 0);
            break;
            
        case DIGI_DM_LOOPBACK:
            /* DMp/DMti value is passed transparently from sink to source */
            rc = digi_otn_odu_delay_measure_init(digi_handle, 
                                                 odu_handle,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                 DIGI_OTN_DM_FEEDTHROUGH,
                                                 0);
            break;
            
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_dm_mode_set */


/*******************************************************************************
* adpt_otn_dm_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the DM(Delay Measurement) mode: disable, initiator, loopback. 
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity for PM and TCMi only.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *dm_mode           - DM mode: initiator, loopback.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_dm_mode_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_dm_mode_t *dm_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_dm_mode_t otn_dm_mode = DIGI_OTN_DM_SOURCE;
    UINT32 dm_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(dm_mode);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_delay_measure_mode_get(digi_handle, 
                                             odu_handle, 
                                             (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                             &otn_dm_mode,
                                             &dm_val);
    ADPT_RETURN_VERIFY(rc);

    switch (otn_dm_mode)
    {
        case DIGI_OTN_DM_SOURCE:
            *dm_mode = DIGI_DM_INITIATOR;
            break;

        case DIGI_OTN_DM_FEEDTHROUGH:
            *dm_mode = DIGI_DM_LOOPBACK;
            break;

        default:
            rc = PMC_ERR_FAIL;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_dm_mode_get */


/*******************************************************************************
* adpt_otn_deg_para_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the dDEG detection parameter in sink position, which is based on 
*   burst mode distribution.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   deg_th             - error block count threshold. The range is 
*                        0 < DEGTHR <= number of blocks.
*   deg_m              - dDEG monitoring interval. The range is from 2 to 10.
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
PUBLIC PMC_ERROR adpt_otn_deg_para_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_loc_t location,
                                    UINT32 deg_th,
                                    UINT32 deg_m)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, deg_th, deg_m);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            /* set threshold */
            rc = digi_otn_otu_deg_th_set(digi_handle,
                                         otu_handle,
                                         deg_th);
            ADPT_RETURN_VERIFY(rc);

            /* set dDEG monitoring interval */
            rc = digi_otn_otu_deg_m_set(digi_handle,
                                        otu_handle,
                                        deg_m);
            ADPT_RETURN_VERIFY(rc);
            break;

        default:
            /* set threshold */
            rc = digi_otn_odu_deg_th_set(digi_handle, 
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         deg_th);
            ADPT_RETURN_VERIFY(rc);

            /* set dDEG monitoring interval */
            rc = digi_otn_odu_deg_m_set(digi_handle, 
                                        odu_handle,
                                        (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                        (UINT8)deg_m);
            ADPT_RETURN_VERIFY(rc);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_deg_para_set */


/*******************************************************************************
* adpt_otn_deg_para_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the dDEG detection parameter in sink position, which is based on 
*   burst mode distribution.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *deg_th            - error block count threshold. The range is 
*                        0 < DEGTHR <= number of blocks.
*   *deg_m             - dDEG monitoring interval. The range is from 2s to 10s.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_deg_para_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_loc_t location,
                                    UINT32 *deg_th,
                                    UINT32 *deg_m)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 deg_m_value = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(deg_th);
    ADPT_PARA_VERIFY(deg_m);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            /* get threshold */
            rc = digi_otn_otu_deg_th_get(digi_handle,
                                         otu_handle,
                                         deg_th);
            ADPT_RETURN_VERIFY(rc);

            /* get dDEG monitoring interval */
            rc = digi_otn_otu_deg_m_get(digi_handle,
                                        otu_handle,
                                        &deg_m_value);
            *deg_m = deg_m_value;
            break;

        default:
            /* get threshold */
            rc = digi_otn_odu_deg_th_get(digi_handle,
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         deg_th);
            ADPT_RETURN_VERIFY(rc);

            /* get dDEG monitoring interval */
            rc = digi_otn_odu_deg_m_get(digi_handle,
                                        odu_handle,
                                        (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                        &deg_m_value);
            *deg_m = deg_m_value;
            ADPT_RETURN_VERIFY(rc);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_deg_para_get */


/*******************************************************************************
* adpt_otn_deg_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the dDEG detection mode: hardware calculation or firmware 
*   calculation.
*
* INPUTS:
*   dev_id             - device identity.
*   odu_seg            - selects the ODU segment to monitor.
*                        DIGI_OTN_DDEG_MON_LINE   -- COREOTN STG1/2/N/3A framers
*                        DIGI_OTN_DDEG_MON_CLIENT -- COREOTN STG4 framer               
*   deg_mode           - dDEG detection mode.
*                        DIGI_OTN_HARDWARE_DDEG: hardware dDEG mode.
*                        DIGI_OTN_ENHANCED_DDEG: enhanced dEDEG mode.
*                        DIGI_OTN_ENH_BURSTY_DDEG: enhanced dEDEG and FW burst dDEG mode.
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
*   1. The odu_seg parameter is invalid under HW dDEG mode.
*   2. Once DI_RCP is enabled through adpt_di_rcp_en_set(), only HW dDEG mode
*      can be selected.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_deg_mode_set(UINT32 dev_id,
                                     digi_otn_ddeg_mon_seg_t odu_seg,
                                     digi_otn_ddeg_mode_t deg_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_ddeg_mon_seg_t org_odu_seg = DIGI_OTN_DDEG_MON_LINE;
    BOOL hw_deg_en = FALSE;
    BOOL deg_en = FALSE;
    UINT32 ddeg_mode = DIGI_OTN_HARDWARE_DDEG;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, odu_seg, deg_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get HW DEG status */
    rc = digi_otn_odu_ddeg_mode_get(adpt_handle->digi_handle,
                                    DIGI_OTN_HARDWARE_DDEG,
                                    &hw_deg_en,
                                    &org_odu_seg);
    ADPT_RETURN_VERIFY(rc);

    /* disable hardware dDEG monitor control */
    if (hw_deg_en && (DIGI_OTN_HARDWARE_DDEG != deg_mode))
    {
        rc = digi_otn_otu_hw_ddeg_mon_enable(adpt_handle->digi_handle, FALSE);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_otn_odu_hw_ddeg_mon_enable(adpt_handle->digi_handle, FALSE);
        ADPT_RETURN_VERIFY(rc);
    }

    /* mode clear */
    for (ddeg_mode = DIGI_OTN_HARDWARE_DDEG; ddeg_mode <= DIGI_OTN_ENH_BURSTY_DDEG; ddeg_mode++)
    {
        if (ddeg_mode == deg_mode)
        {
            continue;
        }

        rc = digi_otn_odu_ddeg_mode_get(adpt_handle->digi_handle, 
                                        (digi_otn_ddeg_mode_t)ddeg_mode, 
                                        &deg_en, &org_odu_seg);
        ADPT_RETURN_VERIFY(rc);

        if (deg_en)
        {
            rc = digi_otn_odu_ddeg_mode_set(adpt_handle->digi_handle, 
                                            (digi_otn_ddeg_mode_t)ddeg_mode, 
                                            org_odu_seg, FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    /* COREOTN */
    rc = digi_otn_odu_ddeg_mode_set(adpt_handle->digi_handle, deg_mode, odu_seg, TRUE);
    ADPT_RETURN_VERIFY(rc);

    /* LINEOTN */
    rc = digi_otn_otu_ddeg_mode_set(adpt_handle->digi_handle, deg_mode);
    ADPT_RETURN_VERIFY(rc);

    /* hardware dDEG monitor control */
    if (DIGI_OTN_HARDWARE_DDEG == deg_mode)
    {
        rc = digi_otn_otu_hw_ddeg_mon_enable(adpt_handle->digi_handle, TRUE);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_otn_odu_hw_ddeg_mon_enable(adpt_handle->digi_handle, TRUE);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_otn_deg_mode_set */


/*******************************************************************************
* adpt_otn_deg_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the dDEG detection mode: hardware calculation or firmware 
*   calculaiton.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *odu_seg           - selects the ODU segment to monitor.
*                        DIGI_OTN_DDEG_MON_LINE   -- COREOTN STG1/2/N/3A framers
*                        DIGI_OTN_DDEG_MON_CLIENT -- COREOTN STG4 framer               
*   *deg_mode          - dDEG detection mode: hardware or firmware.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_deg_mode_get(UINT32 dev_id,
                                     digi_otn_ddeg_mon_seg_t *odu_seg,
                                     digi_otn_ddeg_mode_t *deg_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 ddeg_mode = DIGI_OTN_HARDWARE_DDEG;
    BOOL deg_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(odu_seg);
    ADPT_PARA_VERIFY(deg_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    *deg_mode = DIGI_OTN_HARDWARE_DDEG;
    for (ddeg_mode = DIGI_OTN_HARDWARE_DDEG; ddeg_mode <= DIGI_OTN_ENH_BURSTY_DDEG; ddeg_mode++)
    {
        rc = digi_otn_odu_ddeg_mode_get(adpt_handle->digi_handle,
                                        (digi_otn_ddeg_mode_t)ddeg_mode,
                                        &deg_en,
                                        odu_seg);
        ADPT_RETURN_VERIFY(rc);

        if (deg_en)
        {
            *deg_mode = (digi_otn_ddeg_mode_t)ddeg_mode;
            break;
        }
    }

    PMC_RETURN(rc);
} /* adpt_otn_deg_mode_get */


/*******************************************************************************
* adpt_otn_enhanced_deg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the dDEG detection interval in sink position, which is based on 
*   burst mode distribution. This API, dedicated for SNC protection, can shorten 
*   dDEG detection time. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   deg_th             - error block count threshold. The range is 
*                        0 < DEGTHR <= number of blocks.
*   seg_id             - segment identity.
*   interval           - time interval: 10ms or 100ms.
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
PUBLIC PMC_ERROR adpt_otn_enhanced_deg_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        UINT32 deg_th,
                                        digi_deg_interval_t interval)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               seg_id, deg_th, interval);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            /* set threshold */
            rc = digi_otn_otu_enhanced_ddeg_th_set(digi_handle,
                                                   otu_handle,
                                                   deg_th,
                                                   deg_th - 1);
            ADPT_RETURN_VERIFY(rc);

            switch (interval)
            {
                case DIGI_DEG_10MS:
                    rc = digi_otn_otu_enhanced_ddeg_interval_set(digi_handle,
                                                                 otu_handle,
                                                                 0);
                    break;

                case DIGI_DEG_100MS:
                    rc = digi_otn_otu_enhanced_ddeg_interval_set(digi_handle,
                                                                 otu_handle,
                                                                 1);
                    break;

                case DIGI_DEG_1000MS:
                    rc = digi_otn_otu_enhanced_ddeg_interval_set(digi_handle,
                                                                 otu_handle,
                                                                 2);
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;

        default:
            /* set threshold */
            rc = digi_otn_odu_enhanced_ddeg_th_set(digi_handle,
                                                   odu_handle,
                                                   (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                   deg_th,
                                                   deg_th - 1);
            ADPT_RETURN_VERIFY(rc);

            switch (interval)
            {
                case DIGI_DEG_10MS:
                    rc = digi_otn_odu_enhanced_ddeg_interval_set(digi_handle,
                                                                 odu_handle,
                                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                                 0);
                    break;

                case DIGI_DEG_100MS:
                    rc = digi_otn_odu_enhanced_ddeg_interval_set(digi_handle,
                                                                 odu_handle,
                                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                                 1);
                    break;

                case DIGI_DEG_1000MS:
                    rc = digi_otn_odu_enhanced_ddeg_interval_set(digi_handle,
                                                                 odu_handle,
                                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                                 2);
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_enhanced_deg_set */


/*******************************************************************************
* adpt_otn_enhanced_deg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the dDEG detection interval in sink position, which is based on 
*   burst mode distribution. This API, dedicated for SNC protection, can shorten 
*   dDEG detection time. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*
* OUTPUTS:
*   *deg_th            - error block count threshold. The range is 
*                        0 < DEGTHR <= number of blocks.
*   *interval          - time interval: 10ms or 100ms.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_enhanced_deg_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        UINT32 *deg_th,
                                        digi_deg_interval_t *interval)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 mon_interval = 0;
    UINT32 clear_deg_th = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(deg_th);
    ADPT_PARA_VERIFY(interval);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_enhanced_ddeg_th_get(digi_handle,
                                                   otu_handle,
                                                   deg_th,
                                                   &clear_deg_th);
            ADPT_RETURN_VERIFY(rc);
                                                   
            rc = digi_otn_otu_enhanced_ddeg_interval_get(digi_handle,
                                                         otu_handle,
                                                         &mon_interval);
            break;

        default:
            rc = digi_otn_odu_enhanced_ddeg_th_get(digi_handle,
                                                   odu_handle,
                                                   (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                   deg_th,
                                                   &clear_deg_th);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_enhanced_ddeg_interval_get(digi_handle,
                                                         odu_handle,
                                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                         &mon_interval);
            break;
    }

    switch (mon_interval)
    {
        case 0:
            *interval = DIGI_DEG_10MS;
            break;

        case 1:
            *interval = DIGI_DEG_100MS;
            break;

        case 2:
            *interval = DIGI_DEG_1000MS;
            break;

        default:
            *interval = LAST_DIGI_DEG;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_enhanced_deg_get */


/*******************************************************************************
* adpt_otn_deg_mon_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables dDEG reporting via firmware. 
*
*   Prior to calling this API, the following APIs must have been called to 
*   properly configure dDEG monitoring variables and set the monitoring mode. 
*   adpt_otn_deg_mode_set()
*   adpt_otn_enhanced_deg_set()
*   adpt_otn_deg_para_set()
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   mon_en             - TRUE: enable dDEG monitoring via firmware.
*                        FALSE: disable dDEG monitoring via firmware.
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
PUBLIC PMC_ERROR adpt_otn_deg_mon_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    BOOL mon_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_ddeg_mon_seg_t odu_seg = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mode_t deg_mode = DIGI_OTN_HARDWARE_DDEG;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               seg_id, mon_en);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_OTN_HARDWARE_DDEG != deg_mode)
    {
        switch (seg_id)
        {
            /* only available in OTU layer */
            case DIGI_OTN_SEG_SM:
                /* handle process */
                rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_otn_otu_ddeg_mon_enable(digi_handle, otu_handle, mon_en);
                break;

            default:
                rc = digi_otn_odu_ddeg_mon_enable(digi_handle, 
                                                  odu_handle, 
                                                  deg_mode,
                                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id, 
                                                  mon_en);
                break;
        }
    }

    PMC_RETURN(rc);
} /* adpt_otn_deg_mon_set */


/*******************************************************************************
* adpt_otn_mt_sig_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the maintenance signal insertion in either sink or source position, 
*   which is based on TP level.
*
*   Please note that this API operation, matched with G.709, will affect the 
*   payload information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*   mt_type            - maintenance signal type: Normal, AIS, OCI, LCK.
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
*   1. As the sink position of Stage 3B, the maintenace signal insertion is only
*   availble after the ODUk SW has been created.
*   2. Both DIGI_OTN_SEG_SM and DIGI_OTN_SEG_PM at the source position can't be 
*   used at the same time. 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_mt_sig_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_mt_type_t mt_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, position, mt_type);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        case DIGI_OTN_SEG_SM:
            /* get adapter container handle */
            rc = adpt_handle_get(dev_id, &adpt_handle);
            ADPT_RETURN_VERIFY(rc);

            /* get the index in traffic container */
            rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
            ADPT_RETURN_VERIFY(rc);

            if ((NULL != adpt_handle->traffic_info[index].otu_handle) 
                && (DIGI_OTN_LINE_SIDE == location))
            {
                /* handle process */
                rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_otn_otu_force_maintenance_sig_cfg(digi_handle,
                                                            otu_handle,
                                                            (lineotn_maint_sig_type_t)mt_type,
                                                            (lineotn_dir_t)position);
            }
            else
            {
                rc = digi_otn_odu_maint_sig_set(digi_handle,
                                                odu_handle,
                                                (UINT8)position,
                                                (coreotn_maint_sig_type_t)mt_type);
            }
            break;

        case DIGI_OTN_SEG_TCM1:
        case DIGI_OTN_SEG_TCM2:
        case DIGI_OTN_SEG_TCM3:
        case DIGI_OTN_SEG_TCM4:
        case DIGI_OTN_SEG_TCM5:
        case DIGI_OTN_SEG_TCM6:
            rc = digi_otn_odu_tcmi_maint_sig_set(digi_handle,
                                                 odu_handle,
                                                 (UINT8)position,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                 (coreotn_maint_sig_type_t)mt_type);
            break;

        case DIGI_OTN_SEG_PM:
            if (DIGI_OTN_SOURCE_POS == position)
            {
                rc = digi_otn_odu_odukp_pck_maint_sig_set(digi_handle,
                                                          odu_handle,
                                                          (coreotn_maint_sig_type_t)mt_type);
            }
            else
            {
                rc = PMC_ERR_INVALID_PARAMETERS;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_mt_sig_set */


/*******************************************************************************
* adpt_otn_mt_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the maintenance signal insertion in either sink or source position, 
*   which is based on TP level.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*
* OUTPUTS:
*   *mt_type           - maintenance signal type: Normal, AIS, OCI, LCK.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_mt_sig_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_mt_type_t *mt_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    lineotn_maint_sig_type_t otu_mt_sig = LINE_OTN_NO_MAINT_SIG;
    coreotn_maint_sig_type_t mt_sig_type = CORE_OTN_NO_MAINT_SIG;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(mt_type);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        case DIGI_OTN_SEG_SM:
            /* get adapter container handle */
            rc = adpt_handle_get(dev_id, &adpt_handle);
            ADPT_RETURN_VERIFY(rc);

            /* get the index in traffic container */
            rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
            ADPT_RETURN_VERIFY(rc);

            if ((NULL != adpt_handle->traffic_info[index].otu_handle) 
                && (DIGI_OTN_LINE_SIDE == location))
            {
                /* handle process */
                rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_otn_maintenance_sig_state_get(digi_handle,
                                                        otu_handle,
                                                        (lineotn_dir_t)position,
                                                        &otu_mt_sig);
                *mt_type = (digi_mt_type_t)otu_mt_sig;
            }
            else
            {
                rc = digi_otn_odu_maint_sig_get(digi_handle,
                                                odu_handle,
                                                (UINT8)position,
                                                &mt_sig_type);
                *mt_type = (digi_mt_type_t)mt_sig_type;

                /* workaround to check the maintenance signal at the PM layer */
                rc = digi_otn_odu_odukp_pck_maint_sig_get(digi_handle,
                                                          odu_handle,
                                                          &mt_sig_type);
                if (CORE_OTN_NO_MAINT_SIG != mt_sig_type)
                {
                    *mt_type = DIGI_MT_NORMAL;
                }
            }
            break;
            
        case DIGI_OTN_SEG_TCM1:
        case DIGI_OTN_SEG_TCM2:
        case DIGI_OTN_SEG_TCM3:
        case DIGI_OTN_SEG_TCM4:
        case DIGI_OTN_SEG_TCM5:
        case DIGI_OTN_SEG_TCM6:
            rc = digi_otn_odu_tcmi_maint_sig_get(digi_handle, 
                                                 odu_handle,
                                                 (UINT8)position,
                                                 (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                 &mt_sig_type);
            *mt_type = (digi_mt_type_t)mt_sig_type;
            break;

        case DIGI_OTN_SEG_PM:
            if (DIGI_OTN_SOURCE_POS == position)
            {
                rc = digi_otn_odu_odukp_pck_maint_sig_get(digi_handle,
                                                          odu_handle,
                                                          &mt_sig_type);
                *mt_type = (digi_mt_type_t)mt_sig_type;
            }
            else
            {
                rc = PMC_ERR_INVALID_PARAMETERS;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_mt_sig_get */


/*******************************************************************************
* adpt_otn_bw_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the source of BDI/BEI/BIAE bits in the TX framer of a specified
*   channel. Options are:
*    - Local Rx Framer
*    - RI_RCP interface
*    - Force values at Tx Framer
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   inst_src           - insertion source.
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
PUBLIC PMC_ERROR adpt_otn_bw_inst_set(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location,
                                   digi_source_type_t inst_src)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, location, inst_src);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_bdi_bei_biae_insert_cfg(digi_handle,
                                              odu_handle,
                                              (UINT8)inst_src);

    PMC_RETURN(rc);
} /* adpt_otn_bw_inst_set */


/*******************************************************************************
* adpt_otn_tx_bdi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the BDI signal force insertion in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   bdi_insert         - TRUE: enable BDI insertion.
*                        FALSE: diable BDI insertion.
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
PUBLIC PMC_ERROR adpt_otn_tx_bdi_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL bdi_insert)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, bdi_insert);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_bkwd_oh_ins_cfg(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BDI,
                                              bdi_insert ? LINE_OTN_BKWD_INS_TYPE_USER_VALUE : LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE,
                                              bdi_insert);
            break;

        default:
            rc = digi_otn_odu_bdi_set(digi_handle, 
                                      odu_handle,
                                      (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                      bdi_insert);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_bdi_set */


/*******************************************************************************
* adpt_otn_tx_bdi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the BDI signal force insertion in source position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *bdi_insert        - TRUE: enable BDI insertion.
*                        FALSE: diable BDI insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_bdi_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL *bdi_insert)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 bdi_inst = 0;
    lineotn_bkwd_ins_type_t bkwd_ins_type = LINE_OTN_BKWD_INS_TYPE_UNMODIFIED;
    UINT32 bkwd_value = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(bdi_insert);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_bkwd_oh_ins_get(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BDI,
                                              &bkwd_ins_type,
                                              &bkwd_value);
            *bdi_insert = (LINE_OTN_BKWD_INS_TYPE_USER_VALUE == bkwd_ins_type) ? TRUE : FALSE;
            break;

        default:
            rc = digi_otn_odu_bdi_get(digi_handle,
                                      odu_handle,
                                      (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                      &bdi_inst);
            *bdi_insert = bdi_inst;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_bdi_get */


/*******************************************************************************
* adpt_otn_tx_bei_biae_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the BEI/BIAE signal force insertion in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   bei_biae           - BEI/BIAE value.
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
PUBLIC PMC_ERROR adpt_otn_tx_bei_biae_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      UINT8 bei_biae)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, bei_biae);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_bkwd_oh_ins_cfg(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BEI_BIAE,
                                              (0 != bei_biae) ? LINE_OTN_BKWD_INS_TYPE_USER_VALUE : LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE,
                                              bei_biae);
            break;

        default:
            rc = digi_otn_odu_bei_biae_set(digi_handle, 
                                           odu_handle,
                                           (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                           bei_biae);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_bei_biae_set */


/*******************************************************************************
* adpt_otn_tx_bei_biae_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the BEI/BIAE signal status of force insertion in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *bei_biae          - BEI/BIAE value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_bei_biae_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      UINT8 *bei_biae)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    lineotn_bkwd_ins_type_t bkwd_ins_type = LINE_OTN_BKWD_INS_TYPE_UNMODIFIED;
    UINT32 bkwd_value = 0;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_bkwd_oh_ins_get(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BEI_BIAE,
                                              &bkwd_ins_type,
                                              &bkwd_value);
            *bei_biae = bkwd_value;
            break;

        default:
            rc = digi_otn_odu_bei_biae_get(digi_handle, 
                                           odu_handle,
                                           (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                           bei_biae);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_tx_bei_biae_get */


/*******************************************************************************
* adpt_otn_tx_stat_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the tansmitted 3-bit STAT(Status) overhead in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   stat_value         - STAT value. The range is from 0 to 7.
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
PUBLIC PMC_ERROR adpt_otn_tx_stat_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 stat_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, stat_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_stat_set(digi_handle, 
                                  odu_handle,
                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                  stat_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_stat_set */


/*******************************************************************************
* adpt_otn_tx_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the tansmitted 3-bit STAT(Status) overhead in source position.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *stat_value        - STAT value. The range is from 0 to 7.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_stat_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 *stat_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(stat_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_stat_get(digi_handle, 
                                  odu_handle,
                                  (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                  stat_value);

    PMC_RETURN(rc);
} /* adpt_otn_tx_stat_get */


/*******************************************************************************
* adpt_otn_rx_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted 3-bit STAT(Status) overhead in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *stat_value        - STAT value. The range is from 0 to 7.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_rx_stat_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 *stat_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(stat_value);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_rx_accepted_stat_get(digi_handle,
                                           odu_handle,
                                           (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                           stat_value);

    PMC_RETURN(rc);
} /* adpt_otn_rx_stat_get */


/*******************************************************************************
* adpt_otn_tx_iae_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces an IAE event at all operational TCMs for this channel (inserts 010
*   into STAT field for 16 multiframes) 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
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
PUBLIC PMC_ERROR adpt_otn_tx_iae_force(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, location);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_tx_all_tcm_iae_force(digi_handle, odu_handle);

    PMC_RETURN(rc);
} /* adpt_otn_tx_iae_force */


/*******************************************************************************
* adpt_otn_conseq_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables OTN aBDI, aTSF, aAIS consequent action globally.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity.
*   defect_act         - TRUE: enable aBDI, aTSF, aAIS consequent action.
*                        FALSE: disable aBDI, aTSF, aAIS consequent action.
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
*   1. Please note DIGI_LTC_ACT contributes to only aBDI consequential action 
*   declaration and DIGI_AIS_ACT contributes to aBDI and aTSF consequential 
*   action declaration.
*   2. As to DIGI_MSIM_TO_SSF, it is a global setting that applies to all ODU 
*   channels. When dMSIM to SSF consequential action is enabled for one channel, 
*   this applies to all the other ODU channels in the same stage.
*   3. As to DIGI_LOFLOM_ACT, it isn't supported on Stage 3B and 4.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_conseq_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_defect_act_t defect_id,
                                      BOOL defect_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, defect_id, defect_act);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (defect_id)
    {
        case DIGI_LTC_ACT:
        case DIGI_AIS_ACT:
        case DIGI_OCI_ACT:
        case DIGI_LCK_ACT:
            rc = digi_otn_odu_defect_actions_ctrl_set(digi_handle,
                                                      odu_handle,
                                                      (1 << defect_id),
                                                      (defect_act ? (0 << defect_id) : (1 << defect_id)));
            break;

        case DIGI_LOFLOM_ACT:
            rc = digi_otn_odu_chnl_lof_lom_act_dis_set(digi_handle,
                                                       odu_handle,
                                                       !defect_act);
            break;

        case DIGI_PLM_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                rc = digi_otn_odu_dplm_to_ssf_set(digi_handle,
                                                  odu_handle,
                                                  (BOOL8)defect_act);
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* handle process */
                rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_mapper_dplm_to_ssf_set(digi_handle,
                                                 map_handle,
                                                 (BOOL8)defect_act);
            }
            break;

        case DIGI_LOOMFI_TO_SSF:
            rc = digi_otn_odu_dloomfi_to_ssf_set(digi_handle,
                                                 odu_handle,
                                                 (BOOL8)defect_act);
            break;

        case DIGI_MSIM_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                rc = digi_otn_odu_dmsim_to_ssf_set(digi_handle,
                                                   odu_handle,
                                                   (BOOL8)defect_act);
            }
            break;

        case DIGI_CSF_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* handle process */
                rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_mapper_dcsf_to_ssf_set(digi_handle,
                                                 map_handle,
                                                 (BOOL8)defect_act);
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }
    
    PMC_RETURN(rc);
} /* adpt_otn_conseq_act_set */


/*******************************************************************************
* adpt_otn_conseq_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OTN aBDI, aTSF, aAIS consequent action enable/disable status globally.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity.
*
* OUTPUTS:
*   *defect_act        - TRUE: enable aBDI, aTSF, aAIS consequent action.
*                        FALSE: disable aBDI, aTSF, aAIS consequent action.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_conseq_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_defect_act_t defect_id,
                                      BOOL *defect_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT8 defect_act_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_act);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (defect_id)
    {
        case DIGI_LTC_ACT:
        case DIGI_AIS_ACT:
        case DIGI_OCI_ACT:
        case DIGI_LCK_ACT:
            rc = digi_otn_odu_defect_actions_ctrl_get(digi_handle,
                                                      odu_handle,
                                                      &defect_act_val);
            *defect_act = (defect_act_val & (1 << defect_id)) ? FALSE : TRUE;
            break;

        case DIGI_LOFLOM_ACT:
            rc = digi_otn_odu_chnl_lof_lom_act_dis_get(digi_handle,
                                                       odu_handle,
                                                       (BOOL8*)&defect_act_val);
            *defect_act = defect_act_val ? FALSE : TRUE;
            break;

        case DIGI_PLM_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                rc = digi_otn_odu_dplm_to_ssf_get(digi_handle,
                                                  odu_handle,
                                                  (BOOL8*)&defect_act_val);
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* handle process */
                rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_mapper_dplm_to_ssf_get(digi_handle,
                                                 map_handle,
                                                 (BOOL8*)&defect_act_val);
            }
            *defect_act = defect_act_val ? TRUE : FALSE;
            break;

        case DIGI_LOOMFI_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                rc = digi_otn_odu_dloomfi_to_ssf_get(digi_handle,
                                                     odu_handle,
                                                     (BOOL8*)&defect_act_val);
            }
            *defect_act = defect_act_val ? TRUE : FALSE;
            break;

        case DIGI_MSIM_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                rc = digi_otn_odu_dmsim_to_ssf_get(digi_handle,
                                                   odu_handle,
                                                   (BOOL8*)&defect_act_val);
            }
            *defect_act = defect_act_val ? TRUE : FALSE;
            break;

        case DIGI_CSF_TO_SSF:
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* handle process */
                rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
                ADPT_RETURN_VERIFY(rc);

                rc = digi_mapper_dcsf_to_ssf_get(digi_handle,
                                                 map_handle,
                                                 (BOOL8*)&defect_act_val);
            }
            *defect_act = defect_act_val ? TRUE : FALSE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_conseq_act_get */


/*******************************************************************************
* adpt_otn_tcm_bw_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables TCM aBDI, aBEI, and aBIAE backward consequent action.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for PM and TCM1 ~ TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   bw_act             - TRUE: enable aBDI, aBEI, aBIAE consequent action.
*                        FALSE: disable aBDI, aBEI, aBIAE consequent action.
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
PUBLIC PMC_ERROR adpt_otn_tcm_bw_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL bw_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, seg_id, location, bw_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_bw_act_set(digi_handle,
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         (UINT32)!bw_act);
    
    PMC_RETURN(rc);
} /* adpt_otn_tcm_bw_act_set */


/*******************************************************************************
* adpt_otn_tcm_bw_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of TCM aBDI, aBEI, and aBIAE backward 
*   consequent action.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for PM and TCM1 ~ TCM6.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *bw_act            - TRUE: enable aBDI, aBEI, aBIAE consequent action.
*                        FALSE: disable aBDI, aBEI, aBIAE consequent action.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tcm_bw_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL *bw_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 bw_act_dis = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(bw_act);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_pm_tcmi_bw_act_get(digi_handle,
                                         odu_handle,
                                         (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                         &bw_act_dis);
    *bw_act = (1 == bw_act_dis) ? FALSE : TRUE;

    PMC_RETURN(rc);
} /* adpt_otn_tcm_bw_act_get */


/*******************************************************************************
* adpt_otn_tx_otu_oh_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables BIP8/BEI/BIAE/IAE/BDI/MFAS overheads insertion in OTU 
*   layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   oh_field           - overhead field.
*   oh_value           - overhead value.
*   oh_type            - overhead insertion type.
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
*   As to FAS type, the oh_value is unavailable. 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_otu_oh_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_oh_field_t oh_field,
                                     UINT32 oh_value,
                                     digi_oh_inst_type_t oh_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, oh_field, oh_value, oh_type);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (oh_field)
    {
        case DIGI_OTU_OH_BIP8:
            switch (oh_type)
            {
                case DIGI_OH_INST_DISABLE:
                    rc = digi_otn_otu_bip8_oh_ins_cfg(digi_handle,
                                                      otu_handle,
                                                      LINE_OTN_BIP8_INS_DISABLED,
                                                      oh_value);
                    break;

                case DIGI_OH_INST_ENABLE:
                    rc = digi_otn_otu_bip8_oh_ins_cfg(digi_handle,
                                                      otu_handle,
                                                      LINE_OTN_BIP8_INS_TYPE_ENABLED,
                                                      oh_value);
                   break;

                case DIGI_OH_INST_USER_VALUE:
                    rc = digi_otn_otu_bip8_oh_ins_cfg(digi_handle,
                                                      otu_handle,
                                                      LINE_OTN_BIP8_INS_TYPE_USER_VALUE,
                                                      oh_value);
                    break;

                default:
                   rc = PMC_ERR_INVALID_PARAMETERS;
                   break;
            }
            break;

        case DIGI_OTU_OH_BDI:
            rc = digi_otn_otu_bkwd_oh_ins_cfg(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BDI,
                                              (lineotn_bkwd_ins_type_t)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_BEI_BIAE:
            rc = digi_otn_otu_bkwd_oh_ins_cfg(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BEI_BIAE,
                                              (lineotn_bkwd_ins_type_t)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_IAE:
            rc = digi_otn_otu_iae_oh_ins_cfg(digi_handle,
                                             otu_handle,
                                             (lineotn_iae_ins_type_t)oh_type,
                                             oh_value);
            break;
            
        case DIGI_OTU_OH_MFAS:
            rc = digi_otn_otu_mfas_oh_ins_cfg(digi_handle,
                                              otu_handle,
                                              (lineotn_mfas_ins_type_t)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_FAS_F6:
        case DIGI_OTU_OH_FAS_28:
            switch (oh_type)
            {
                case DIGI_OH_INST_DISABLE:
                    rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                                 otu_handle,
                                                 (DIGI_OTU_OH_FAS_F6 == oh_field) ? LINE_OTN_SO_FAS_F6 : LINE_OTN_SO_FAS_28,
                                                 LINE_OTN_INS_TYPE_UNMODIFIED,
                                                 oh_value);
                    break;

                case DIGI_OH_INST_ENABLE:
                case DIGI_OH_INST_USER_VALUE:
                    rc = digi_otn_otu_oh_ins_cfg(digi_handle, 
                                                 otu_handle,
                                                 (DIGI_OTU_OH_FAS_F6 == oh_field) ? LINE_OTN_SO_FAS_F6 : LINE_OTN_SO_FAS_28,
                                                 LINE_OTN_INS_TYPE_USER_VALUE,
                                                 oh_value);
                    break;

                default:
                   rc = PMC_ERR_INVALID_PARAMETERS;
                   break;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }
    
    PMC_RETURN(rc);
} /* adpt_otn_tx_otu_oh_set */


/*******************************************************************************
* adpt_otn_tx_otu_oh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of BIP8/BEI/BIAE/IAE/BDI/MFAS overheads 
*   insertion in OTU layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   oh_field           - overhead field.
*
* OUTPUTS:
*   *oh_value          - overhead value.
*   *oh_type           - overhead insertion type.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_tx_otu_oh_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_oh_field_t oh_field,
                                     UINT32 *oh_value,
                                     digi_oh_inst_type_t *oh_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    lineotn_bip8_ins_type_t bip8_ins_type = LINE_OTN_BIP8_INS_DISABLED;
    lineotn_oh_ins_type_t ctrl_ins_type = LINE_OTN_INS_TYPE_UNMODIFIED;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_value);
    ADPT_PARA_VERIFY(oh_type);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (oh_field)
    {
        case DIGI_OTU_OH_BIP8:
            rc = digi_otn_otu_bip8_oh_ins_get(digi_handle,
                                              otu_handle,
                                              &bip8_ins_type,
                                              oh_value);
            switch (bip8_ins_type)
            {
                case LINE_OTN_BIP8_INS_DISABLED:
                    *oh_type = DIGI_OH_INST_DISABLE;
                    break;

                case LINE_OTN_BIP8_INS_TYPE_ENABLED:
                    *oh_type = DIGI_OH_INST_ENABLE;
                    break;

                case LINE_OTN_BIP8_INS_TYPE_USER_VALUE:
                case LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE:
                case LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE:
                    *oh_type = DIGI_OH_INST_USER_VALUE;
                    break;

                default:
                   rc = PMC_ERR_FAIL;
                   break;
            }
            break;

        case DIGI_OTU_OH_BDI:
            rc = digi_otn_otu_bkwd_oh_ins_get(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BDI,
                                              (lineotn_bkwd_ins_type_t*)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_BEI_BIAE:
            rc = digi_otn_otu_bkwd_oh_ins_get(digi_handle,
                                              otu_handle,
                                              LINE_OTN_SO_BEI_BIAE,
                                              (lineotn_bkwd_ins_type_t*)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_IAE:
            rc = digi_otn_otu_iae_oh_ins_get(digi_handle,
                                             otu_handle,
                                             (lineotn_iae_ins_type_t*)oh_type,
                                             oh_value);
            break;
            
        case DIGI_OTU_OH_MFAS:
            rc = digi_otn_otu_mfas_oh_ins_get(digi_handle,
                                              otu_handle,
                                              (lineotn_mfas_ins_type_t*)oh_type,
                                              oh_value);
            break;

        case DIGI_OTU_OH_FAS_F6:
        case DIGI_OTU_OH_FAS_28:
            rc = digi_otn_otu_oh_ins_get(digi_handle, 
                                         otu_handle,
                                         (DIGI_OTU_OH_FAS_F6 == oh_field) ? LINE_OTN_SO_FAS_F6 : LINE_OTN_SO_FAS_28,
                                         &ctrl_ins_type,
                                         oh_value);
            switch (ctrl_ins_type)
            {
                case LINE_OTN_INS_TYPE_UNMODIFIED:
                    *oh_type = DIGI_OH_INST_DISABLE;
                    break;

                case LINE_OTN_INS_TYPE_USER_VALUE:
                    *oh_type = DIGI_OH_INST_USER_VALUE;
                    break;

                default:
                   rc = PMC_ERR_FAIL;
                   break;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }
    
    PMC_RETURN(rc);
} /* adpt_otn_tx_otu_oh_get */


/*******************************************************************************
* adpt_otu_conseq_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the consequent action at the OTU layer. In order to 
*   enable/disable a consequential action at least one relevant defect bit/s 
*   should appear in the defect bit mask variable.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   act_type           - consequent action type at the OTU layer.
*   defect_msk         - relevant defect bit mask based on the following bits:
*                        LINE_OTN_D_LOS_P
*                        LINE_OTN_D_LOL_P
*                        LINE_OTN_D_LOFLAN
*                        LINE_OTN_D_AIS
*                        LINE_OTN_D_LOF
*                        LINE_OTN_D_LOM
*                        LINE_OTN_D_TIM
*                        LINE_OTN_D_IAE
*                        LINE_OTN_D_DEG
*                        LINE_OTN_N_BIPV
*                        LINE_OTN_IAE
*                        LINE_OTN_D_BDI_FROM_SSF  
*                        LINE_OTN_D_TSF_FROM_SSF
*                        LINE_OTN_D_SSF_FROM_TSF
*                        LINE_OTN_D_AIS_FROM_TSF
*                        LINE_OTN_D_SSD_FROM_TSD
*                        Or a pre-defined activation bit masks which set all relevant bits: 
*                        LINE_OTN_A_TSF_ACT 
*                        LINE_OTN_A_SSF_ACT 
*                        LINE_OTN_A_BDI_ACT 
*                        LINE_OTN_A_BEI_ACT 
*                        LINE_OTN_A_BIAE_ACT
*                        LINE_OTN_A_TSD_ACT 
*                        LINE_OTN_A_SSD_ACT 
*                        LINE_OTN_A_IAE_ACT 
*                        LINE_OTN_A_AIS_ACT 
*                        LINE_OTN_A_FFAIL_ACT 
*   defect_act         - TRUE: enable the consequent action at the OTU layer.
*                        FALSE: disable the consequent action at the OTU layer.
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
*   Please note the LINE_OTN_A_AIS consequent action for LINE_OTN_D_LOM can 
*   be used by the adpt_otn_tim_act_dis_set() as well.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_conseq_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      lineotn_action_type_t act_type,
                                      UINT32 defect_msk,
                                      BOOL defect_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, act_type, defect_msk, defect_act);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_otu_consq_action_cfg(digi_handle,
                                       otu_handle,
                                       act_type,
                                       defect_msk,
                                       defect_act);

    PMC_RETURN(rc);
} /* adpt_otu_conseq_act_set */


/*******************************************************************************
* adpt_otu_conseq_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves enable/disable status of the consequent action at the OTU layer. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   act_type           - consequent action type at the OTU layer.
*   defect_msk         - relevant defect bit mask based on the following bits:
*                        LINE_OTN_D_LOS_P
*                        LINE_OTN_D_LOL_P
*                        LINE_OTN_D_LOFLAN
*                        LINE_OTN_D_AIS
*                        LINE_OTN_D_LOF
*                        LINE_OTN_D_LOM
*                        LINE_OTN_D_TIM
*                        LINE_OTN_D_IAE
*                        LINE_OTN_D_DEG
*                        LINE_OTN_N_BIPV
*                        LINE_OTN_IAE
*                        LINE_OTN_D_BDI_FROM_SSF  
*                        LINE_OTN_D_TSF_FROM_SSF
*                        LINE_OTN_D_SSF_FROM_TSF
*                        LINE_OTN_D_AIS_FROM_TSF
*                        LINE_OTN_D_SSD_FROM_TSD
*                        Or a pre-defined activation bit masks which set all relevant bits: 
*                        LINE_OTN_A_TSF_ACT 
*                        LINE_OTN_A_SSF_ACT 
*                        LINE_OTN_A_BDI_ACT 
*                        LINE_OTN_A_BEI_ACT 
*                        LINE_OTN_A_BIAE_ACT
*                        LINE_OTN_A_TSD_ACT 
*                        LINE_OTN_A_SSD_ACT 
*                        LINE_OTN_A_IAE_ACT 
*                        LINE_OTN_A_AIS_ACT 
*                        LINE_OTN_A_FFAIL_ACT 
*
* OUTPUTS:
*   *defect_act        - defect bit mask for the consequent action at the OTU layer.
*                        LINE_OTN_D_LOS_P
*                        LINE_OTN_D_LOL_P
*                        LINE_OTN_D_LOFLANE
*                        LINE_OTN_D_AIS
*                        LINE_OTN_D_LOF
*                        LINE_OTN_D_LOM
*                        LINE_OTN_D_TIM
*                        LINE_OTN_D_IAE
*                        LINE_OTN_D_DEG
*                        LINE_OTN_N_BIPV
*                        LINE_OTN_IAE
*                        LINE_OTN_D_BDI_FROM_SSF
*                        LINE_OTN_D_TSF_FROM_SSF
*                        LINE_OTN_D_SSF_FROM_TSF
*                        LINE_OTN_D_AIS_FROM_TSF
*                        LINE_OTN_D_SSD_FROM_TSD
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otu_conseq_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      lineotn_action_type_t act_type,
                                      UINT32 defect_msk,
                                      UINT32 *defect_act)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_act);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_otu_consq_action_cfg_get(digi_handle,
                                           otu_handle,
                                           act_type,
                                           defect_msk,
                                           defect_act);

    PMC_RETURN(rc);
} /* adpt_otu_conseq_act_get */


/*******************************************************************************
* adpt_otn_prbs_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the PRBS function mapped into ODUk.
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   prbs_chid          - PRBS channel ID that carries PRBS pattern
*                        Valid range: 0 - 11 
*   prbs_mode          - PRBS mode.
*   so_prbs_en         - TRUE: enable prbs function at the source position. (generation) 
*                        FALSE: disable PRBS function at the source position.
*   sk_prbs_en         - TRUE: enable prbs function at the sink position. (monitor)
*                        FALSE: disable PRBS function at the sink position.
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
*   1. Stage 3A won't support the PRBS feature.
*   2. The OTN PRBS procedure is as follows.
*      A. Enable OTN PRBS: adpt_otn_prbs_en_set()
*      B. Configure the threshold: adpt_otn_prbs_thresh_set()
*      C. PMON trigger: adpt_pmon_trigger()
*      D. Retrieve dLSS/PRBS count: adpt_otn_prbs_dlss_get()/adpt_otn_prbs_pm_get()
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 prbs_chid,
                                   digi_otn_prbs_pattern_t prbs_mode,
                                   BOOL so_prbs_en,
                                   BOOL sk_prbs_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, prbs_chid, prbs_mode, so_prbs_en, sk_prbs_en);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* configure PRBS pattern */
    rc = digi_otn_prbs_pattern_cfg(digi_handle,
                                   odu_handle,
                                   prbs_chid,
                                   prbs_mode);
    ADPT_RETURN_VERIFY(rc);

    /* PRBS initialization at the sink poistion */
    rc = digi_otn_prbs_chnl_init(digi_handle,
                                 odu_handle,
                                 prbs_chid,
                                 0,
                                 sk_prbs_en);
    ADPT_RETURN_VERIFY(rc);

    /* PRBS initialization at the source poistion */
    rc = digi_otn_prbs_chnl_init(digi_handle,
                                 odu_handle,
                                 prbs_chid,
                                 1,
                                 so_prbs_en);
    ADPT_RETURN_VERIFY(rc);

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].prbs_chid = prbs_chid;
        adpt_handle->traffic_info[index].prbs_enable = (UINT8)(sk_prbs_en | (so_prbs_en << 4));
    }
    
    PMC_RETURN(rc);
} /* adpt_otn_prbs_en_set */


/*******************************************************************************
* adpt_otn_prbs_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of PRBS function mapped into ODUk.
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *prbs_chid         - PRBS channel ID that carries PRBS pattern
*                        Valid range: 0 - 11 
*   *prbs_mode         - PRBS mode.
*   *so_prbs_en        - TRUE: enable prbs function at the source position. (generation) 
*                        FALSE: disable PRBS function at the source position.
*   *sk_prbs_en        - TRUE: enable prbs function at the sink position. (monitor)
*                        FALSE: disable PRBS function at the sink position.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 *prbs_chid,
                                   digi_otn_prbs_pattern_t *prbs_mode,
                                   BOOL *so_prbs_en,
                                   BOOL *sk_prbs_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_chid);
    ADPT_PARA_VERIFY(prbs_mode);
    ADPT_PARA_VERIFY(so_prbs_en);
    ADPT_PARA_VERIFY(sk_prbs_en);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get PRBS channel ID */
    result = digi_otn_odu_prbs_chid_get(digi_handle,
                                        odu_handle,
                                        prbs_chid);

    if (PMC_SUCCESS == result)
    {
        /* get PRBS pattern */
        rc = digi_otn_chnl_prbs_pattern_get(digi_handle,
                                            odu_handle,
                                            *prbs_chid,
                                            prbs_mode);
        *so_prbs_en = adpt_handle->traffic_info[index].prbs_enable >> 4;
        *sk_prbs_en = adpt_handle->traffic_info[index].prbs_enable & 0xF;
    }
    else
    {
        *so_prbs_en = FALSE;
        *sk_prbs_en = FALSE;
        *prbs_mode  = DIGI_OTN_INVERTED_PRBS_11;
    }

    PMC_RETURN(rc);
} /* adpt_otn_prbs_en_get */


/*******************************************************************************
* adpt_otn_prbs_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the PRBS bit error insertion. When enable bit error 
*   insertion, a single bit error is inserted once into the PRBS pattern for 
*   the channel.
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   err_insert         - TRUE: enable prbs bit error insertion.
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_err_inst_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL err_insert)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, err_insert);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* check if PRBS is enabled or not */
    if (FALSE == (BOOL)(adpt_handle->traffic_info[index].prbs_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_otn_prbs_chnl_bit_err_inject(digi_handle,
                                           odu_handle,
                                           adpt_handle->traffic_info[index].prbs_chid,
                                           (UINT32)err_insert);

    PMC_RETURN(rc);
} /* adpt_otn_prbs_err_inst_set */


/*******************************************************************************
* adpt_otn_prbs_err_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS bit error insertion enable/disable status. 
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *err_insert        - TRUE: enable prbs bit error insertion.
*                        FALSE: disable PRBS function.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_err_inst_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *err_insert)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(err_insert);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_prbs_err_inst_get */


/*******************************************************************************
* adpt_otn_prbs_thresh_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the defect threshold count for the given PRBS channel. 
*   If the bit error rate goes over the programmed threshold in a given LCLK 
*   one second interval, the PRBS monitor will enter out of lock state. Changing 
*   this configuration will affect all channels.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   threshold          - the defect threshold count.
*                        Valid range: 0 ~ 0x7FFFFFFFF.
*   reseed_thresh      - number of bit errors detected before PRBS monitor reseed
*                        reference value: 0xF0
*   reseed_interval    - number of cycles between re-seed attempts
*                        reference value: 0x180
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
PUBLIC PMC_ERROR adpt_otn_prbs_thresh_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT64 threshold,
                                      UINT16 reseed_thresh,
                                      UINT16 reseed_interval)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT32 low_thresh = threshold & 0xFFFFFFFF;
    UINT32 high_thresh = threshold >> 32;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, low_thresh, high_thresh, reseed_thresh, reseed_interval);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_prbs_ber_thres_cfg(digi_handle,
                                     odu_handle,
                                     low_thresh,
                                     high_thresh,
                                     reseed_thresh,
                                     reseed_interval);    

    PMC_RETURN(rc);
} /* adpt_otn_prbs_thresh_set */


/*******************************************************************************
* adpt_otn_prbs_thresh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the defect threshold count for the given PRBS channel. 
*   If the bit error rate goes over the programmed threshold in a given LCLK 
*   one second interval, the PRBS monitor will enter out of lock state.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *threshold         - the defect threshold count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_thresh_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT64 *threshold)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(threshold);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    *threshold = 0;

    PMC_RETURN(rc);
} /* adpt_otn_prbs_thresh_get */


/*******************************************************************************
* adpt_otn_prbs_dlss_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the dLSS(Loss of PRBS lock) defect from PRBS channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *dlss_state        - TRUE: dLSS is raised.
*                        FALSE: dLSS is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_dlss_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL   *dlss_state)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 dlss = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(dlss_state);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (adpt_handle->traffic_info[index].prbs_enable)
    {
        rc = digi_otn_prbs_dlss_status_get(digi_handle,
                                           odu_handle,
                                           adpt_handle->traffic_info[index].prbs_chid,
                                           &dlss);
        *dlss_state = dlss;
    }
    else
    {
        *dlss_state = FALSE;
    }

    PMC_RETURN(rc);
} /* adpt_otn_prbs_dlss_get */


/*******************************************************************************
* adpt_otn_null_sig_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the NULL signal function mapped into ODUk. ODUkP -> NULL.
*
*   Please note that NULL signal is generated in transmitted direction and 
*   monitored in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   null_enable        - TRUE: enable NULL signal function.
*                        FALSE: disable NULL signal function.
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
*   Please note the following limitations.
*   1. The cross connection in ODUk SW must be disabled before calling this API.
*   2. The ODUk in Stage3A can't allocate NULL signal.
*   3. The ODUk with daughter ODUj can't allocate NULL signal.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_null_sig_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL null_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 ho_index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_msi_cfg_mode_t msi_mode = DIGI_OTN_MSI_CFG_AUTO;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, null_enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if ((DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        && (DIGI_ODU_NULL != adpt_handle->traffic_info[index].payload_type))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    else if (((DIGI_ODU_NULL == adpt_handle->traffic_info[index].payload_type) && null_enable)
              || ((DIGI_ODU_UNCHANNELIZED == adpt_handle->traffic_info[index].payload_type) && !null_enable))
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    /* Stage 3A has no this feature. */
    if ((DIGI_ODU_STRUCT_LEVEL_LO_ODU == adpt_handle->traffic_info[index].odu_stage)
         || ((DIGI_ODU_STRUCT_LEVEL_MO_ODU == adpt_handle->traffic_info[index].odu_stage)
        && (DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* HO ODUk payload type process */
    if (NULL != adpt_handle->traffic_info[index].otu_handle)
    {
        /* HO ODUk deprovision*/
        rc = digi_otn_server_ho_deprov(adpt_handle->digi_handle, 
                                       &(adpt_handle->traffic_info[index].odu_handle),
                                       &(adpt_handle->traffic_info[index].otu_handle));
        ADPT_RETURN_VERIFY(rc);

        if (null_enable)
        {
            /* HO ODUk channel provision */
            rc = digi_otn_server_ho_prov(adpt_handle->digi_handle,
                                         adpt_handle->traffic_info[index].otu_handle,
                                         DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                         DIGI_ODU_NULL,
                                         &(adpt_handle->traffic_info[index].odu_handle));
            /* payload type */
            adpt_handle->traffic_info[index].payload_type = DIGI_ODU_NULL;
        }
        else
        {
            /* HO ODUk channel provision */
            rc = digi_otn_server_ho_prov(adpt_handle->digi_handle,
                                         adpt_handle->traffic_info[index].otu_handle,
                                         DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                         DIGI_ODU_UNCHANNELIZED,
                                         &(adpt_handle->traffic_info[index].odu_handle));
            /* payload type */
            adpt_handle->traffic_info[index].payload_type = DIGI_ODU_UNCHANNELIZED;
        }
        /* workaround: default TCM pool ID */
        PMC_MEMSET((void*)&(adpt_handle->traffic_info[index].tcm_pool_id), 0, 
                   sizeof(adpt_handle->traffic_info[index].tcm_pool_id));
    }
    /* MO ODUk payload type process */
    else if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        rc = adpt_chnl_index_get(adpt_handle, 
                                 adpt_handle->traffic_info[index].ho_chnl_id, 
                                 &ho_index);
        ADPT_RETURN_VERIFY(rc);

        /* workaround on MSI configuration mode */
        rc |= digi_otn_msi_mode_get(adpt_handle->digi_handle, &msi_mode);
        rc |= digi_otn_msi_mode_set(adpt_handle->digi_handle, DIGI_OTN_MSI_CFG_MANUAL);
        ADPT_RETURN_VERIFY(rc);

        /* MO ODUk deprovision */
        rc = digi_otn_server_lo_deprov_asym(adpt_handle->digi_handle,
                                            &(adpt_handle->traffic_info[index].odu_handle),
                                            &(adpt_handle->traffic_info[ho_index].odu_handle),
                                            adpt_otn_dir_cvt(adpt_handle->traffic_info[index].otn_dir));
        ADPT_RETURN_VERIFY(rc);

        if (null_enable)
        {
            /* MO ODUk deprovision */
            rc = digi_otn_server_lo_prov_asym(adpt_handle->digi_handle,
                                              adpt_handle->traffic_info[ho_index].odu_handle,
                                              DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                              adpt_handle->traffic_info[index].oduk_type,
                                              adpt_handle->traffic_info[index].oduflex_rate,
                                              adpt_handle->traffic_info[index].mapping_type,
                                              &(adpt_handle->traffic_info[index].tx_ts_mask[0]),
                                              &(adpt_handle->traffic_info[index].rx_ts_mask[0]),
                                              DIGI_ODU_NULL,
                                              &(adpt_handle->traffic_info[index].odu_handle));
            /* payload type */
            adpt_handle->traffic_info[index].payload_type = DIGI_ODU_NULL;
        }
        else
        {
            /* MO ODUk deprovision */
            rc = digi_otn_server_lo_prov_asym(adpt_handle->digi_handle,
                                              adpt_handle->traffic_info[ho_index].odu_handle,
                                              DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                              adpt_handle->traffic_info[index].oduk_type,
                                              adpt_handle->traffic_info[index].oduflex_rate,
                                              adpt_handle->traffic_info[index].mapping_type,
                                              &(adpt_handle->traffic_info[index].tx_ts_mask[0]),
                                              &(adpt_handle->traffic_info[index].rx_ts_mask[0]),
                                              DIGI_ODU_UNCHANNELIZED,
                                              &(adpt_handle->traffic_info[index].odu_handle));
            /* payload type */
            adpt_handle->traffic_info[index].payload_type = DIGI_ODU_UNCHANNELIZED;
        }
        ADPT_RETURN_VERIFY(rc);

        /* workaround on MSI configuration mode */
        rc = digi_otn_msi_mode_set(adpt_handle->digi_handle, msi_mode);
        ADPT_RETURN_VERIFY(rc);

        /* workaround: default TCM pool ID */
        PMC_MEMSET((void*)&(adpt_handle->traffic_info[index].tcm_pool_id), 0, 
                   sizeof(adpt_handle->traffic_info[index].tcm_pool_id));
    }
    /* MAPOTN payload type process */
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        rc = digi_mapper_null_frame_cfg(adpt_handle->digi_handle,
                                        adpt_handle->traffic_info[index].map_handle,
                                        null_enable);
        /* payload type */
        adpt_handle->traffic_info[index].payload_type = null_enable ? DIGI_ODU_NULL : DIGI_ODU_UNCHANNELIZED;
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_null_sig_set */


/*******************************************************************************
* adpt_otn_null_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the NULL signal function enable/disable status which is mapped 
*   into ODUk. ODUkP -> NULL.
*
*   Please note that NULL signal is generated in transmitted direction and 
*   monitored in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *null_enable       - TRUE: enable NULL signal function.
*                        FALSE: disable NULL signal function.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_null_sig_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL *null_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    BOOL8 null_sig = FALSE;
    UINT32 traffic_loc = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(null_enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* Stage 3A has no this feature. */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    if ((DIGI_ODU_STRUCT_LEVEL_LO_ODU == adpt_handle->traffic_info[index].odu_stage)
         || ((DIGI_ODU_STRUCT_LEVEL_MO_ODU == adpt_handle->traffic_info[index].odu_stage)
         && (DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type))
         || ((DIGI_ODU_STRUCT_LEVEL_HO_ODU == adpt_handle->traffic_info[index].odu_stage)
         && (DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
         && (DIGI_LINE_SIDE == traffic_loc)))
    {
        *null_enable = FALSE;
        PMC_RETURN(rc);
    }

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        /* handle process */
        rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                                 &digi_handle, &odu_handle);
        ADPT_RETURN_VERIFY(rc);
    
        rc = digi_otn_opu_null_payload_status_get(digi_handle,
                                                  odu_handle,
                                                  &null_sig);
        *null_enable = null_sig ? TRUE : FALSE;
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_null_frame_get(digi_handle,
                                        map_handle,
                                        null_enable);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_otn_null_sig_get */


/*******************************************************************************
* adpt_otn_opu_csf_force_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables OPU-CSF force insertion.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: insert OPU-CSF forcely.
*                        FALSE: remove OPU-CSF forcely.
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
*   This API is available for those datapaths through MAPOTN.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_opu_csf_force_inst_set(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (enable)
    {
        rc = digi_cmf_trigger_opu_csf_cfg(digi_handle,
                                          map_handle,
                                          DIGI_SW_START_FORCING_ACTION);
    }
    else
    {
        /* stop OPU-CSF insertion */
        rc = digi_cmf_trigger_opu_csf_cfg(digi_handle,
                                          map_handle,
                                          DIGI_SW_STOP_FORCING_ACTION);
        ADPT_RETURN_VERIFY(rc);

        /* release control for auto OPU-CSF */
        rc = digi_cmf_trigger_opu_csf_cfg(digi_handle,
                                          map_handle,
                                          DIGI_SW_RELEASE_ACTION_CONTROL);
    }

    PMC_RETURN(rc);
} /* adpt_otn_opu_csf_force_inst_set */


/*******************************************************************************
* adpt_otn_opu_csf_force_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of OPU-CSF force insertion.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: insert OPU-CSF forcely.
*                        FALSE: remove OPU-CSF forcely.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is available for those datapaths through MAPOTN.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_opu_csf_force_inst_get(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_opu_csf_force_inst_get */


/*******************************************************************************
* adpt_otn_cm_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Cm value and Cm out of limit interrupt from the OTN channel. It's used to 
*   identify the received ODUk rate if it is out of range. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *cm_out_limit_i    - Cm out of limit interrupt. 
*   *cm_value          - Cm value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. The adpt_defect_trigger() should be called prior to this API.
*   2. This API can be only available on Stage 2, 3A and 4.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_cm_status_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL *cm_out_limit_i,
                                     UINT32 *cm_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    coreotn_int_framer_chnl_t *frm_int_table = NULL;
    rgmp_int_chnl_t *rgmp_int_table = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(cm_out_limit_i);
    ADPT_PARA_VERIFY(cm_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        /* handle process */
        rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                                 &digi_handle, &odu_handle);
        ADPT_RETURN_VERIFY(rc);

        /* get odu channel ID and odu stage number */
        rc = adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
        ADPT_RETURN_VERIFY(rc);

        switch (odu_stage)
        {
            case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                break;

            case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }

        *cm_out_limit_i = frm_int_table->odtu_dmx.cm_out_limit_i ? TRUE : FALSE;
        frm_int_table->odtu_dmx.cm_out_limit_i = FALSE;

        rc = digi_cm_status_get(digi_handle, odu_handle, cm_value);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* handle process */
        rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
        ADPT_RETURN_VERIFY(rc);

        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        rgmp_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].status.rgmp);

        *cm_out_limit_i = rgmp_int_table->cm_out_limit_i ? TRUE : FALSE;
        rgmp_int_table->cm_out_limit_i = FALSE;

        rc = digi_cm_status_get(digi_handle, map_handle, cm_value);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(rc);
} /* adpt_otn_cm_status_get */


/*******************************************************************************
* adpt_odu_ts_type_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to transition the tributary slot type of an ODUk. This 
*   API will also automatically provision the PM layer in the event that there
*   is a transition from UNCHANNELIZED->CHANNELIZED payload.
*
*   Supports the following transitions:
*   DIGI_ODU_UNCHANNELIZED --> DIGI_ODU_TS_1G25
*   DIGI_ODU_UNCHANNELIZED --> DIGI_ODU_TS_2G5
*   DIGI_ODU_UNCHANNELIZED --> DIGI_ODU_NULL
*   DIGI_ODU_TS_1G25 --> DIGI_ODU_UNCHANNELIZED
*   DIGI_ODU_TS_1G25 --> DIGI_ODU_NULL
*   DIGI_ODU_TS_1G25 --> DIGI_ODU_TS_2G5
*   DIGI_ODU_TS_2G5 --> DIGI_ODU_UNCHANNELIZED
*   DIGI_ODU_TS_2G5 --> DIGI_ODU_NULL
*   DIGI_ODU_TS_2G5 --> DIGI_ODU_TS_1G25
*   DIGI_ODU_NULL --> DIGI_ODU_UNCHANNELIZED
*   DIGI_ODU_NULL --> DIGI_ODU_TS_1G25
*   DIGI_ODU_NULL --> DIGI_ODU_TS_2G5
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ts_type            - the tributary slot type.
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
PUBLIC PMC_ERROR adpt_odu_ts_type_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_odu_line_payload_t ts_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_otn_msi_cfg_mode_t msi_mode = DIGI_OTN_MSI_CFG_AUTO;
    digi_otn_odu_chnl_prov_mode_t prov_mode = DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, ts_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (ts_type != adpt_handle->traffic_info[index].payload_type)
    {
        rc = adpt_prov_mode_get(dev_id, &prov_mode);
        ADPT_RETURN_VERIFY(rc);

        /* workaround in the bulk provision mode */
        /* switch to the non-bulk provision mode before the digi_otn_payload_type_transition() */
        if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
        {
            rc = adpt_prov_mode_set(dev_id, DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT);
            ADPT_RETURN_VERIFY(rc);
        }

        /* HO ODUk payload type process */
        if (NULL != adpt_handle->traffic_info[index].otu_handle)
        {
            rc = digi_otn_payload_type_transition(digi_handle, odu_handle, ts_type);
            ADPT_RETURN_VERIFY(rc);
        }
        /* MO ODUk payload type process */
        else
        {
            /* workaround on MSI configuration mode */
            rc |= digi_otn_msi_mode_get(adpt_handle->digi_handle, &msi_mode);
            rc |= digi_otn_msi_mode_set(adpt_handle->digi_handle, DIGI_OTN_MSI_CFG_MANUAL);
            ADPT_RETURN_VERIFY(rc);

            /* payload type transition */
            rc = digi_otn_payload_type_transition(digi_handle, odu_handle, ts_type);
            ADPT_RETURN_VERIFY(rc);

            /* workaround on MSI configuration mode */
            rc |= digi_otn_msi_mode_set(adpt_handle->digi_handle, msi_mode);
            ADPT_RETURN_VERIFY(rc);
        }

        /* reset TCM pool ID */
        rc = adpt_tcm_pool_reset(dev_id, chnl_id);
        ADPT_RETURN_VERIFY(rc);

        /* payload type */
        adpt_handle->traffic_info[index].payload_type = ts_type;

        /* workaround in the bulk provision mode */
        /* switch to the bulk provision mode after the digi_otn_payload_type_transition() */
        if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
        {
            rc = adpt_prov_mode_set(dev_id, prov_mode);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_odu_ts_type_set */


/*******************************************************************************
* adpt_odu_ts_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API retrieves the tributary slot type of an ODUk.  
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ts_type            - the tributary slot type.
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
*   Please note that once the NULL signal is enabled through the 
*   adpt_otn_null_sig_set(), the adpt_odu_ts_type_get() will retrieve the 
*   DIGI_ODU_NULL type accordingly.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_ts_type_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_odu_line_payload_t *ts_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ts_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    *ts_type = adpt_handle->traffic_info[index].payload_type;

    PMC_RETURN(rc);
} /* adpt_odu_ts_type_get */


/*******************************************************************************
* adpt_otn_pm_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the OTUk/ODUk performance monitoring information at the sink 
*   position. This API retrieves the OTN PMON from registers directly. It 
*   requires to disable the OTN mask in the manual_trig_msk of the digi_dev_info_t
*   initialization table. In this way, FW won't retrieve this PMON to the shared
*   memory through DMA so that adpt_otn_pm_get() isn't available at this moment.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *otn_cnt           - OTN PMON count. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   Please note this API can't coexist with the adpt_otn_pm_get(). 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_pm_reg_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_cnt_t *otn_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    digi_pmon_lineotn_otu_frm_counters_t sm_ebc;
    digi_pmon_coreotn_odukp_counters_t pm_ebc;
    digi_pmon_coreotn_odukt_counters_t tcm_ebc;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(otn_cnt);

    switch (seg_id)
    {
        /* only available in OTU layer */
        case DIGI_OTN_SEG_SM:
            /* handle process */
            rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_otu_ebc_get(digi_handle, otu_handle, &sm_ebc);
            ADPT_RETURN_VERIFY(rc);

            otn_cnt->bip   = sm_ebc.otu_frm_sk_sm_p_n_ebc;
            otn_cnt->nds   = sm_ebc.otu_frm_sk_sm_p_n_ds;
            otn_cnt->piae  = sm_ebc.otu_frm_sk_sm_p_iae;
            otn_cnt->bei   = sm_ebc.otu_frm_sk_sm_p_f_ebc;
            otn_cnt->fds   = sm_ebc.otu_frm_sk_sm_p_f_ds;
            otn_cnt->pbiae = sm_ebc.otu_frm_sk_sm_p_biae;
            break;

        default:
            /* handle process */
            rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_ebc_get(digi_handle, 
                                      odu_handle,
                                      (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                      &pm_ebc,
                                      &tcm_ebc);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_OTN_SEG_PM == seg_id)
            {
                otn_cnt->bip   = pm_ebc.odukp_sk_pn_ebc_pm;
                otn_cnt->nds   = pm_ebc.odukp_sk_pn_ds_pm;
                otn_cnt->piae  = 0;
                otn_cnt->bei   = pm_ebc.odukp_sk_pf_ebc_pm;
                otn_cnt->fds   = pm_ebc.odukp_sk_pf_ds_pm;
                otn_cnt->pbiae = 0;
            }
            else
            {
                otn_cnt->bip   = tcm_ebc.odukt_sk_pn_ebc;
                otn_cnt->nds   = tcm_ebc.odukt_sk_pn_ds;
                otn_cnt->piae  = tcm_ebc.odukt_sk_piae;
                otn_cnt->bei   = tcm_ebc.odukt_sk_pf_ebc;
                otn_cnt->fds   = tcm_ebc.odukt_sk_pf_ds;
                otn_cnt->pbiae = tcm_ebc.odukt_sk_pbiae;
            }
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_pm_reg_get */




/*
** End of file
*/
