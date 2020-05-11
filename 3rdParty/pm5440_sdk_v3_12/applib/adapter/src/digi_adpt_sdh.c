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
*     The file describes the SONET/SDH Run-Time functionality of CBRC subsystem.
*
*   NOTES:
*     All SONET/SDH configuration operates on an STS-192 or STS-768 payload.
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
* adpt_sdh_tti_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the TTI mode of SONET/SDH in either transmitted or received
*   direction, which includes 16-byte mode and 64-byte mode.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   tti_mode           - tti mode: 1-byte, 16-byte, 64-byte.
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
*   Please note this API will insert a E.164 CRC in byte 16 for the
*   DIGI_SDH_TTI_LEN_16_BYTE mode. And it will automatically insert CR/LF in 
*   bytes 63 and 64 for the DIGI_SDH_TTI_LEN_64_BYTE mode.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_tti_mode_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_sdh_tti_len_t tti_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rttp_algo_t algo = SDH_PMG_RTTP_ALGO_2;
    UINT32 client_mode = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, tti_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
    if ((DIGI_CBR_CLIENT_40G_STS768 == client_mode)
         || (DIGI_CBR_CLIENT_10G_STS192 == client_mode)
         || (DIGI_CBR_CLIENT_10G_STS192_PRBS == client_mode))
    {
        algo = SDH_PMG_RTTP_ALGO_1;
    }

    /* only SDH_PMG_RTTP_ALGO_3 support 1-byte mode */
    if (DIGI_SDH_TTI_LEN_1_BYTE == tti_mode)
    {
        algo = SDH_PMG_RTTP_ALGO_3;
    }
    /* disable TTI monitor */
    else if (DIGI_SDH_TTI_DISABLED == tti_mode) 
    {
       tti_mode = DIGI_SDH_TTI_LEN_1_BYTE;
       algo = SDH_PMG_RTTP_ALGO_DISABLED;
    }

    rc |= digi_cbr_sdh_rttp_len_cfg(digi_handle,
                                    cbr_handle,
                                    (cbrc_port_dir_t)dir,
                                    (sdh_pmg_rttp_length_t)tti_mode,
                                    algo);

    PMC_RETURN(rc);
} /* adpt_sdh_tti_mode_set */


/*******************************************************************************
* adpt_sdh_tti_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the TTI mode of SONET/SDH in either transmitted or received
*   direction, which includes 16-byte mode and 64-byte mode.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *tti_mode          - tti mode: 16-byte, 64-byte.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_tti_mode_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_sdh_tti_len_t *tti_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_mode);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_cbr_sdh_rttp_len_get(digi_handle,
                                    cbr_handle,
                                    (cbrc_port_dir_t)dir,
                                    (sdh_pmg_rttp_length_t*)tti_mode);

    PMC_RETURN(rc);
} /* adpt_sdh_tti_mode_get */


/*******************************************************************************
* adpt_sdh_rx_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted TTI bytes of SONET/SDH in either transmitted or 
*   received direction. The length of TTI buffer should be 64 bytes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *tti_data          - accepted tti bytes information.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_rx_tti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir,
                                 UINT8 *tti_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    sdh_pmg_rttp_trace_t tti_trace;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    digi_sdh_tti_len_t tti_mode;
    UINT32 client_mode = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_data);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get TTI mode */
    rc = adpt_sdh_tti_mode_get(dev_id, chnl_id, dir, &tti_mode);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    PMC_MEMSET((void*)&tti_trace, 0, sizeof(sdh_pmg_rttp_trace_t));
    if ((DIGI_SDH_TTI_LEN_1_BYTE == tti_mode)
         || (DIGI_CBR_CLIENT_40G_STS768 == client_mode)
         || (DIGI_CBR_CLIENT_10G_STS192 == client_mode)
         || (DIGI_CBR_CLIENT_10G_STS192_PRBS == client_mode))
    {
        rc = digi_cbr_sdh_rttp_cap_trace_get(digi_handle,
                                             cbr_handle,
                                             (cbrc_port_dir_t)dir,
                                             &tti_trace);
    }
    else
    {
        rc = digi_cbr_sdh_rttp_accept_trace_get(digi_handle,
                                                cbr_handle,
                                                (cbrc_port_dir_t)dir,
                                                &tti_trace);
    }
    /* TTI data conversion */
    PMC_MEMCPY(tti_data, &tti_trace.trace_msg[0], 64 * sizeof(UINT8));

    PMC_RETURN(rc);
} /* adpt_sdh_rx_tti_get */


/*******************************************************************************
* adpt_sdh_exp_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected TTI bytes of SONET/SDH in either transmitted or 
*   received direction. The length of TTI buffer should be 64 bytes.
*
*   Please note that this API should be called after adpt_sdh_tti_mode_set()
*   API has been executed.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   tti_data           - expected tti bytes information.
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
PUBLIC PMC_ERROR adpt_sdh_exp_tti_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT8 tti_data[64])
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rttp_length_t tti_len = LAST_SDH_PMG_RTTP_LENGTH;
    sdh_pmg_rttp_trace_t tti_trace;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id,
               chnl_id, dir, tti_data);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* parameter conversion */
    rc = digi_cbr_sdh_rttp_len_get(digi_handle,
                                   cbr_handle,
                                   (cbrc_port_dir_t)dir,
                                   &tti_len);
    PMC_MEMCPY(&tti_trace.trace_msg, &tti_data[0], 64 * sizeof(UINT8));
    tti_trace.crlf_ptr = NULL;
    tti_trace.e164_crc_ptr = NULL;
    tti_trace.size = tti_len;

    rc = digi_cbr_sdh_rttp_expect_trace_set(digi_handle,
                                            cbr_handle,
                                            (cbrc_port_dir_t)dir,
                                            &tti_trace);

    PMC_RETURN(rc);
} /* adpt_sdh_exp_tti_set */


/*******************************************************************************
* adpt_sdh_exp_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected TTI bytes of SONET/SDH in either transmitted or 
*   received direction. The length of TTI buffer should be 64 bytes.
*
*   This API is used for debug or data recovery.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *tti_data          - expected tti bytes information.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_exp_tti_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT8 *tti_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rttp_trace_t tti_trace;
     PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tti_data);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&tti_trace, 0, sizeof(sdh_pmg_rttp_trace_t));
    rc = digi_cbr_sdh_rttp_expect_trace_get(digi_handle,
                                            cbr_handle,
                                            (cbrc_port_dir_t)dir,
                                            &tti_trace);
    /* TTI data conversion */
    PMC_MEMCPY(tti_data, &tti_trace.trace_msg[0], 64 * sizeof(UINT8));

    PMC_RETURN(rc);
} /* adpt_sdh_exp_tti_get */


/*******************************************************************************
* adpt_sdh_bn_type_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the byte (B1 or B2) that the SDH SBER block will use to detect
*   signal degrade and signal fail. The SBER in the SDH_PMG can only monitor
*   either the B1 or B2 byte at one time. Changing this configuration will
*   affect all channels.
*
* INPUTS:
*   dev_id             - device identity.
*   bn_type            - B1/B2 selection: DIGI_SDH_B1, DIGI_SDH_B2.
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
PUBLIC PMC_ERROR adpt_sdh_bn_type_set(UINT32 dev_id, digi_sdh_pm_t bn_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    sdh_pmg_sber_byte_t sber_byte = SDH_PMG_SBER_BYTE_B1;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, bn_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch(bn_type)
    {
        case DIGI_SDH_B1:
            sber_byte = SDH_PMG_SBER_BYTE_B1;
            break;
            
        case DIGI_SDH_B2:
            sber_byte = SDH_PMG_SBER_BYTE_B2;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        
    }
    
    rc = digi_cbr_sdh_sber_byte_cfg(adpt_handle->digi_handle, sber_byte);

    PMC_RETURN(rc);
} /* adpt_sdh_bn_type_set */


/*******************************************************************************
* adpt_sdh_bn_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the byte (B1 or B2) that the SDH SBER block will use to detect
*   signal degrade and signal fail. The SBER in the SDH_PMG can only monitor
*   either the B1 or B2 byte at one time. 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *bn_type           - B1/B2 selection: DIGI_SDH_B1, DIGI_SDH_B2.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_bn_type_get(UINT32 dev_id, digi_sdh_pm_t *bn_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    sdh_pmg_sber_byte_t sber_byte = SDH_PMG_SBER_BYTE_B1;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(bn_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_sber_byte_get(adpt_handle->digi_handle, &sber_byte);

    switch(sber_byte)
    {
        case SDH_PMG_SBER_BYTE_B1:
            *bn_type = DIGI_SDH_B1;
            break;
            
        case SDH_PMG_SBER_BYTE_B2:
            *bn_type = DIGI_SDH_B2;
            break;
            
        default:
            PMC_RETURN(PMC_ERR_FAIL);
    }

    PMC_RETURN(rc);
} /* adpt_sdh_bn_type_get */


/*******************************************************************************
* adpt_sdh_bn_thr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the B1/B2 EXC/DEG threshold of SONET/SDH in either transmitted 
*   or received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   exc_thr            - dEXC(Excessive error) threshold.
*                        B1 STS-768: DIGI_SDH_1E_05 to DIGI_SDH_1E_12 
*                        B1 STS-192: DIGI_SDH_1E_05 to DIGI_SDH_1E_11     
*                        B2 STS-768: DIGI_SDH_1E_04 to DIGI_SDH_1E_11 
*                        B2 STS-192: DIGI_SDH_1E_04 to DIGI_SDH_1E_11 
*   deg_thr            - dDEG(Degraded Signal Defect) threshold.
*                        B1 STS-768: DIGI_SDH_1E_05 to DIGI_SDH_1E_12 
*                        B1 STS-192: DIGI_SDH_1E_05 to DIGI_SDH_1E_11     
*                        B2 STS-768: DIGI_SDH_1E_04 to DIGI_SDH_1E_11 
*                        B2 STS-192: DIGI_SDH_1E_04 to DIGI_SDH_1E_11 
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
PUBLIC PMC_ERROR adpt_sdh_bn_thr_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_thr_t exc_thr,
                                  digi_sdh_thr_t deg_thr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    digi_sdh_thr_t org_exc_thr = DIGI_SDH_DISABLED;
    digi_sdh_thr_t org_deg_thr = DIGI_SDH_DISABLED;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, exc_thr, deg_thr);

    if ((exc_thr >= LAST_DIGI_SDH_THR) || (deg_thr >= LAST_DIGI_SDH_THR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_sdh_bn_thr_get(dev_id, chnl_id, dir, &org_exc_thr, &org_deg_thr);
    ADPT_RETURN_VERIFY(rc);

    /* EXC threshold */
    if (org_exc_thr != exc_thr)
    {
        rc = digi_cbr_sdh_sber_sf_enable(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         (sdh_pmg_sber_ber_t)exc_thr);
        ADPT_RETURN_VERIFY(rc);
    }

    /* DEG threshold */
    if (org_deg_thr != deg_thr)
    {
        rc = digi_cbr_sdh_sber_sd_enable(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         (sdh_pmg_sber_ber_t)deg_thr);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_sdh_bn_thr_set */


/*******************************************************************************
* adpt_sdh_bn_thr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the B1/B2 EXC/DEG threshold of SONET/SDH in either transmitted 
*   or received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *exc_thr           - dEXC(Excessive error) threshold.
*   *deg_thr           - dDEG(Degraded Signal Defect) threshold.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_bn_thr_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_thr_t *exc_thr,
                                  digi_sdh_thr_t *deg_thr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(exc_thr);
    ADPT_PARA_VERIFY(deg_thr);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    /* EXC threshold */
    rc = digi_cbr_sdh_sber_sf_ber_get(digi_handle,
                                      cbr_handle,
                                      (cbrc_port_dir_t)dir,
                                      (sdh_pmg_sber_ber_t*)exc_thr);
    ADPT_RETURN_VERIFY(rc);

    /* DEG threshold */
    rc = digi_cbr_sdh_sber_sd_ber_get(digi_handle,
                                      cbr_handle,
                                      (cbrc_port_dir_t)dir,
                                      (sdh_pmg_sber_ber_t*)deg_thr);

    PMC_RETURN(rc);
} /* adpt_sdh_bn_thr_get */


/*******************************************************************************
* adpt_sdh_bip_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the B1/B2 accumulation errors mode: BIP errors or block errors.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   rs_bip_mode        - RS layer BIP mode: BIP-8 errors or BIP-8 block errors. 
*   ms_bip_mode        - MS layer BIP mode: BIP-8 errors or BIP-24 block errors. 
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
PUBLIC PMC_ERROR adpt_sdh_bip_mode_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_bip_mode_t rs_bip_mode,
                                     digi_bip_mode_t ms_bip_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rrmp_alarm_cfg_t alarm_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, rs_bip_mode, ms_bip_mode);

    if ((rs_bip_mode >= LAST_DIGI_SDH_BIP) || (ms_bip_mode >= LAST_DIGI_SDH_BIP))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);


    PMC_MEMSET((void*)&alarm_cfg, 0, sizeof(sdh_pmg_rrmp_alarm_cfg_t));
    alarm_cfg.mask = SDH_PMG_RRMP_ALARM_CFG_MASK_LBIP_ACCUM 
                     | SDH_PMG_RRMP_ALARM_CFG_MASK_SBIP_ACCUM;
    alarm_cfg.LBIP_accum = ms_bip_mode;
    alarm_cfg.SBIP_accum = rs_bip_mode;

    rc = digi_cbr_sdh_rrmp_alarm_cfg(digi_handle,
                                     cbr_handle,
                                     (cbrc_port_dir_t)dir,
                                     &alarm_cfg);

    PMC_RETURN(rc);
} /* adpt_sdh_bip_mode_set */


/*******************************************************************************
* adpt_sdh_bip_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the B1/B2 accumulation errors mode: BIP errors or block errors.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *rs_bip_mode       - RS layer BIP mode: BIP-8 errors or BIP-8 block errors. 
*   *ms_bip_mode       - MS layer BIP mode: BIP-8 errors or BIP-24 block errors. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_bip_mode_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_bip_mode_t *rs_bip_mode,
                                     digi_bip_mode_t *ms_bip_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rrmp_alarm_cfg_t alarm_cfg;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(rs_bip_mode);
    ADPT_PARA_VERIFY(ms_bip_mode);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&alarm_cfg, 0, sizeof(sdh_pmg_rrmp_alarm_cfg_t));
    rc = digi_cbr_sdh_rrmp_alarm_cfg_get(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         &alarm_cfg);

    *ms_bip_mode = alarm_cfg.LBIP_accum ? DIGI_SDH_BLOCK_ERR : DIGI_SDH_BIP_ERR;
    *rs_bip_mode = alarm_cfg.SBIP_accum ? DIGI_SDH_BLOCK_ERR : DIGI_SDH_BIP_ERR;

    PMC_RETURN(rc);
} /* adpt_sdh_bip_mode_get */


/*******************************************************************************
* adpt_sdh_rx_oh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted overheads of SONET/SDH in either transmitted or 
*   received direction. 
*
*   J0  (Received J0 byte, 8 bits)
*   SSM (Sync Status Message, 8 bits)
*   K1  (APS K1,  8 bits)
*   K2  (APS K2, 8 bits)
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   type               - SDH overhead type.
*
* OUTPUTS:
*   *oh_byte           - the accepted overhead. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_rx_oh_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  sdh_pmg_rrmp_byte_type_t type,
                                  UINT8 *oh_byte)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_byte);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_rrmp_byte_get(digi_handle,
                                    cbr_handle,
                                    (cbrc_port_dir_t)dir,
                                    type,
                                    oh_byte);

    PMC_RETURN(rc);
} /* adpt_sdh_rx_oh_get */


/*******************************************************************************
* adpt_sdh_prbs_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the PRBS function which mapped into SONET/SDH.
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   prbs_invert        - PRBS parameter: invert, non-invert.
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_prbs_en_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL prbs_invert,
                                    BOOL prbs_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_prbs_cfg_t prbs_cfg;
    BOOL org_invert = FALSE;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, prbs_invert, prbs_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_sdh_prbs_en_get(dev_id, chnl_id, &org_invert, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if ((((!org_invert && prbs_invert) || (org_invert && !prbs_invert)) && prbs_enable)
         || (!org_enable && prbs_enable) || (org_enable && !prbs_enable))
    {
        /******************************************************* 
         * note: The digi_cbr_sdh_prbs_gen_set() API should be  
         *       called prior to digi_cbr_sdh_prbs_mon_set().
         ******************************************************/

        if (org_enable && prbs_enable)
        {
            /* egress direction */
            prbs_cfg.prbs_type   = SDH_PMG_PRBS_TYPE_TX_GENERATOR;
            prbs_cfg.inv_cfg     = prbs_invert;
            prbs_cfg.enable_prbs = FALSE;
            rc = digi_cbr_sdh_prbs_gen_set(digi_handle, cbr_handle, &prbs_cfg);
            ADPT_RETURN_VERIFY(rc);

            /* ingress direction */
            prbs_cfg.prbs_type   = SDH_PMG_PRBS_TYPE_RX_MONITOR;
            rc = digi_cbr_sdh_prbs_mon_set(digi_handle, cbr_handle, &prbs_cfg);
            ADPT_RETURN_VERIFY(rc);
        }

        /* egress direction */
        prbs_cfg.prbs_type   = SDH_PMG_PRBS_TYPE_TX_GENERATOR;
        prbs_cfg.inv_cfg     = prbs_invert;
        prbs_cfg.enable_prbs = prbs_enable;
        rc = digi_cbr_sdh_prbs_gen_set(digi_handle, cbr_handle, &prbs_cfg);
        ADPT_RETURN_VERIFY(rc);

        /* ingress direction */
        prbs_cfg.prbs_type   = SDH_PMG_PRBS_TYPE_RX_MONITOR;
        rc = digi_cbr_sdh_prbs_mon_set(digi_handle, cbr_handle, &prbs_cfg);
        ADPT_RETURN_VERIFY(rc);
    }

    if (prbs_enable)
    {
        /* forces PRBS monitor to resample pattern */
        rc = digi_cbr_sdh_prbs_mon_resync(digi_handle, cbr_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_sdh_prbs_en_set */


/*******************************************************************************
* adpt_sdh_prbs_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of PRBS function which mapped into 
*   SONET/SDH.
*
*   Please note that PRBS is generated in transmitted direction and monitored
*   in received direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *prbs_invert       - PRBS parameter: invert, non-invert.
*   *prbs_enable       - TRUE: enable prbs function.
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
PUBLIC PMC_ERROR adpt_sdh_prbs_en_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *prbs_invert,
                                    BOOL *prbs_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_prbs_cfg_t prbs_cfg;
     PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_invert);
    ADPT_PARA_VERIFY(prbs_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_prbs_gen_get(digi_handle, cbr_handle, &prbs_cfg);

    *prbs_invert = prbs_cfg.inv_cfg;
    *prbs_enable = prbs_cfg.enable_prbs;

    PMC_RETURN(rc);
} /* adpt_sdh_prbs_en_get */


/*******************************************************************************
* adpt_sdh_prbs_err_inst
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Insert an error into the PRBS stream. The MSB of the next byte will be
*    inverted, inducing a single bit error.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
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
PUBLIC PMC_ERROR adpt_sdh_prbs_err_inst(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, chnl_id);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_prbs_error_ins(digi_handle, cbr_handle);

    PMC_RETURN(rc);
} /* adpt_sdh_prbs_err_inst */


/*******************************************************************************
* adpt_sdh_prbs_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS synchronization status and error counts in the Rx direction.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *prbs_syn          - TRUE: PRBS synchronization.
*                        FALSE: loss of PRBS synchronization.
*   *err_cnt           - error count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_prbs_status_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *prbs_syn,
                                       UINT32 *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
     util_patt_status_t status;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_syn);
    ADPT_PARA_VERIFY(err_cnt);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_prbs_mon_sync_status_get(digi_handle, cbr_handle, &status, err_cnt);
    if (PMC_SUCCESS == rc)
    {
        *prbs_syn = (UTIL_PATT_STATUS_LOCKED == status) ? TRUE : FALSE;
    }

    PMC_RETURN(rc);
} /* adpt_sdh_prbs_status_get */


/*******************************************************************************
* adpt_sdh_gais_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures if PN11 is inserted into the datapath when an LOS or LOF occurs.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   alm_type           - defect type: dLOF, dLOS.
*   enable             - TRUE: enable G-AIS(PN11) insertion
*                        FALSE: disable G-AIS(PN11) insertion
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
PUBLIC PMC_ERROR adpt_sdh_gais_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    sdh_pmg_pn11_insert_t alm_type,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, alm_type, enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_pn11_ins_set(digi_handle,
                                   cbr_handle,
                                   (cbrc_port_dir_t)dir,
                                   alm_type,
                                   enable);

    PMC_RETURN(rc);
} /* adpt_sdh_gais_inst_set */


/*******************************************************************************
* adpt_sdh_gais_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 is inserted into the datapath when an LOS or LOF occurs.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   alm_type           - defect type: dLOF, dLOS.
*
* OUTPUTS:
*   *enable            - TRUE: enable G-AIS(PN11) insertion
*                        FALSE: disable G-AIS(PN11) insertion
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_gais_inst_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    sdh_pmg_pn11_insert_t alm_type,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_pn11_ins_set_get(digi_handle,
                                       cbr_handle,
                                       (cbrc_port_dir_t)dir,
                                       alm_type,
                                       enable);

    PMC_RETURN(rc);
} /* adpt_sdh_gais_inst_get */


/*******************************************************************************
* adpt_sdh_oof_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces an Out Of Frame into the next framing boundary.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
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
PUBLIC PMC_ERROR adpt_sdh_oof_force(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, dir);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_sdh_oof_force(digi_handle, cbr_handle, (cbrc_port_dir_t)dir);

    PMC_RETURN(rc);
} /* adpt_sdh_oof_force */


/*******************************************************************************
 * adpt_sdh_m0_en_set
 * ______________________________________________________________________________
 *
 * DESCRIPTION:
 *   Enables or disables the M0 byte to indicate that the receive M0 byte is valid  
 *   and should be considered (STS-192/STM-64 only). When the M0 byte is enabled, 
 *   it is taken into consideration when extracting the LREI from the incoming stream. 
 *   When the M0 byte is disabled, it is not considered for the LREI extraction.
 *
 * INPUTS:
 *   dev_id             - device identity.
 *   chnl_id            - channel identity.
 *   dir                - traffic direction: Tx, Rx.
 *   m0_enable          - TRUE: enable M0 byte. 
 *                        FALSE: disable M0 byte.
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
PUBLIC PMC_ERROR adpt_sdh_m0_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL m0_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rrmp_alarm_cfg_t rrmp_alm_cfg;
    sdh_pmg_trmp_alarm_cfg_t trmp_alm_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, dir, m0_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&rrmp_alm_cfg, 0, sizeof(sdh_pmg_rrmp_alarm_cfg_t));
    rrmp_alm_cfg.mask  = SDH_PMG_RRMP_ALARM_CFG_MASK_M0EN;
    rrmp_alm_cfg.M0_en = m0_enable;

    rc = digi_cbr_sdh_rrmp_alarm_cfg(digi_handle,
                                     cbr_handle,
                                     (cbrc_port_dir_t)dir,
                                     &rrmp_alm_cfg);
    ADPT_RETURN_VERIFY(rc);

    /* enable m0 byte in PRBS MG */
    if (DIGI_PORT_DIR_TX == dir)
    {
        PMC_MEMSET((void*)&trmp_alm_cfg, 0, sizeof(sdh_pmg_trmp_alarm_cfg_t));
        trmp_alm_cfg.mask = SDH_PMG_TRMP_ALARM_CFG_MASK_M0EN;
        trmp_alm_cfg.M0_en = m0_enable;

        rc = digi_cbr_sdh_trmp_alarm_cfg(digi_handle,
                                         cbr_handle,
                                         &trmp_alm_cfg);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_sdh_m0_en_set */


/*******************************************************************************
 * adpt_sdh_m0_en_get
 * ______________________________________________________________________________
 *
 * DESCRIPTION:
 *   Retrieves the M0 byte status to indicate that the receive M0 byte is valid  
 *   and should be considered (STS-192/STM-64 only). When the M0 byte is enabled, 
 *   it is taken into consideration when extracting the LREI from the incoming stream. 
 *   When the M0 byte is disabled, it is not considered for the LREI extraction.
 *
 * INPUTS:
 *   dev_id             - device identity.
 *   chnl_id            - channel identity.
 *   dir                - traffic direction: Tx, Rx.
 *
 * OUTPUTS:
 *   *m0_enable         - TRUE: enable M0 byte. 
 *                        FALSE: disable M0 byte.
 *
 * RETURNS:
 *   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
 *                        PMC_ERR_FAIL: get invalid SW status.
 *                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
 *
 * NOTES:
 *
 *******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_m0_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL *m0_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    sdh_pmg_rrmp_alarm_cfg_t rrmp_alm_cfg;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(m0_enable);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&rrmp_alm_cfg, 0, sizeof(sdh_pmg_rrmp_alarm_cfg_t));

    rc = digi_cbr_sdh_rrmp_alarm_cfg_get(digi_handle,
                                         cbr_handle,
                                         (cbrc_port_dir_t)dir,
                                         &rrmp_alm_cfg);
    *m0_enable = rrmp_alm_cfg.M0_en;

    PMC_RETURN(rc);
} /* adpt_sdh_m0_en_get */


/*
** End of file
*/
