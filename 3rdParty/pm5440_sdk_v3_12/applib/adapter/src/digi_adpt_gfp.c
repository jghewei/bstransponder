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
*     The file describes the GFP Run-Time functionality of CBRC subsystem.
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
* adpt_gfp_tx_cmf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   configures the insertion of transmitted CMF(Client Management Frame)/MCF
*   (Management Communications Frame) frame based on user input and sends a one 
*   shot CMF on the given channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   payload_len_type   - flag to determine whether to include payload: 0~1.
*   upi_value          - UPI value to be send in the CMF: 0~255.
*   fcs_en             - TRUE: 4-byte payload FCS should be generated if client 
*                        payload field is used.
*                        FALSE: 4-byte payload FCS shouldn't be generated.
*   *cmf_data          - payload information.
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
PUBLIC PMC_ERROR adpt_gfp_tx_cmf_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT32 payload_len_type,
                                  UINT32 upi_value,
                                  BOOL   fcs_en,
                                  digi_cmf_one_shot_cmf_t *cmf_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    BOOL8 inst_result = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%p].\r\n", dev_id, 
               chnl_id, payload_len_type, upi_value, fcs_en, cmf_data);

    /* check parameter validation */
    ADPT_PARA_VERIFY(cmf_data);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_insert_one_shot_gfp_cmf_cfg(digi_handle,
                                              map_handle,
                                              payload_len_type,
                                              upi_value,
                                              (UINT32)fcs_en,
                                              &inst_result,
                                              cmf_data);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_cmf_set */


/*******************************************************************************
* adpt_gfp_rx_cmf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Extracts the CMF(Client Management Frame) from either the CPU FIFO or from 
*   the memory where the last stored CMF was saved after being extracted after 
*   an interrupt. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   cmf_mode           - the mode to extract CMF. 
*
* OUTPUTS:
*   *cmf_data          - CMF data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_cmf_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gfp_cmf_mode_t cmf_mode,
                                  digi_cmf_extract_cmf_t *cmf_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(cmf_data);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_extract_gfp_cmf_get(digi_handle,
                                      map_handle,
                                      cmf_mode,
                                      cmf_data);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_cmf_get */


/*******************************************************************************
* adpt_gfp_tx_fcs_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted FCS field mode of a GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fcs_mode           - FCS mode.
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
*   1. For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*   DIGI_MAPOTN_ODU2EP_FC_1200_GFP
*
*   2. This API can only be called before ODUk SW/CPB is created.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_fcs_mode_t fcs_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, fcs_mode);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_tx_fcs_mode_set(digi_handle,
                                         map_handle,
                                         (mapotn_fcs_mode_t)fcs_mode);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_fcs_mode_set */


/*******************************************************************************
* adpt_gfp_tx_fcs_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted FCS field mode of a GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *fcs_mode          - FCS mode.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_fcs_mode_t *fcs_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(fcs_mode);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_tx_fcs_mode_get(digi_handle,
                                         map_handle,
                                         (mapotn_fcs_mode_t*)fcs_mode);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_fcs_mode_get */


/*******************************************************************************
* adpt_gfp_tx_fcs_corrupt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables Tx frame FCS corruption feature. When enabled, the 
*   device inverses GFP-F frame FCS when an errored data frame is encountered.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: GFP frame payload FCS is corrupted.
*                        FALSE: corruption is disabled.
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
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_corrupt_set(UINT32 dev_id, UINT32 chnl_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_tx_fcs_corrupt_set(digi_handle,
                                            map_handle,
                                            enable);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_fcs_corrupt_set */


/*******************************************************************************
* adpt_gfp_tx_fcs_corrupt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of Tx frame FCS corruption feature. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: GFP frame payload FCS is corrupted.
*                        FALSE: corruption is disabled.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_corrupt_get(UINT32 dev_id, UINT32 chnl_id, BOOL *enable)
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

    *enable = digi_mapper_gfp_tx_fcs_corrupt_get(digi_handle, map_handle);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_fcs_corrupt_get */


/*******************************************************************************
* adpt_gfp_rx_fcs_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   configures the received FCS field mode of a GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fcs_mode           - Rx FCS mode.
*   preamble_size      - length of preamble that needs to be processed in 
*                        payload\n
*                        0 -- Three bytes of preamble and one byte of SFD\n
*                        1 -- Seven bytes of preamble and one byte of SFD\n
*                        2 -- Eleven bytes of preamble and one byte of SFD\n
*                        3 -- Fifteen bytes of preamble and one byte of SFD\n
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
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*   DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET
*   DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET
*   DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_fcs_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_rx_fcs_mode_t fcs_mode,
                                       UINT32 preamble_size)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, fcs_mode, preamble_size);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_fcs_chk_mode_set(digi_handle,
                                             map_handle,
                                             0,
                                             (UINT32)fcs_mode,
                                             preamble_size);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_fcs_mode_set */


/*******************************************************************************
* adpt_gfp_rx_fcs_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the received FCS field mode of a GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   fcs_mode           - Rx FCS mode.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_fcs_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_rx_fcs_mode_t *fcs_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(fcs_mode);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_fcs_chk_mode_get(digi_handle,
                                             map_handle,
                                             (UINT32*)fcs_mode);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_fcs_mode_get */


/*******************************************************************************
* adpt_gfp_tx_ehec_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the transmitted extension header of the GFP frame, which
*   is 64 bytes length at most.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ehec_len           - EHEC length.
*   ehec_buffer        - EHEC buffer.
*   ehec_enable        - TRUE: enable EHEC insertion.
*                        FALSE: disable EHEC insertion.
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
*   1. For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*   DIGI_MAPOTN_ODU2EP_FC_1200_GFP
*
*   2. This API can only be called before ODUk SW/CPB is created.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_ehec_en_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 ehec_len,
                                      UINT32 ehec_buffer[16],
                                      BOOL ehec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, ehec_len, ehec_enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_tx_ext_header_set(digi_handle,
                                           map_handle,
                                           ehec_enable,
                                           ehec_len,
                                           &ehec_buffer[0]);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_ehec_en_set */


/*******************************************************************************
* adpt_gfp_tx_ehec_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted extension header enable/disable status of the 
*   GFP frame, which is 64 bytes length at most.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *ehec_len          - EHEC length.
*   *ehec_enable       - TRUE: enable EHEC insertion.
*                        FALSE: disable EHEC insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_ehec_en_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *ehec_len,
                                      BOOL   *ehec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ehec_len);
    ADPT_PARA_VERIFY(ehec_enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_tx_ext_header_get(digi_handle,
                                           map_handle,
                                           (UINT32*)ehec_enable,
                                           ehec_len);

    PMC_RETURN(rc);
} /* adpt_gfp_tx_ehec_en_get */


/*******************************************************************************
* adpt_gfp_rx_ehec_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the received extension header check of the GFP frame, 
*   which is 64 bytes length at most.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ehec_err_cor       - defines whether or not single-bit error correction 
*                        is performed on the GFP Extension Header
*                        0 -- no single bit error correction is done
*                        1 -- perform single-bit error correction
*   exi_len            - length of the extension header if the incoming GFP 
*                        frame's extracted EXI value matchs with EXI_VALUE1
*   ehec_enable        - TRUE: enable EHEC check.
*                        FALSE: disable EHEC check.
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
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_ehec_en_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 ehec_err_cor,
                                      UINT32 exi_len,
                                      BOOL   ehec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 ehec_en = 0;
    UINT32 ehec_err = 0;
    UINT32 exi_length = 0;
    UINT32 exi_val  = 0;
    UINT32 exi_length1 = 0;
    UINT32 exi_val1 = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               ehec_err_cor, exi_len, ehec_enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_get(digi_handle,
                                           map_handle,
                                           &ehec_en,
                                           &ehec_err,
                                           &exi_val,
                                           &exi_length,
                                           &exi_val1,
                                           &exi_length1);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_set(digi_handle,
                                           map_handle,
                                           ehec_enable,
                                           ehec_err_cor,
                                           exi_val,
                                           exi_len,
                                           exi_val1,
                                           exi_len);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_ehec_en_set */


/*******************************************************************************
* adpt_gfp_rx_ehec_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the received extension header enable/disable status of the GFP 
*   frame, which is 64 bytes length at most.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *ehec_err_cor      - defines whether or not single-bit error correction 
*                        is performed on the GFP Extension Header
*                        0 -- no single bit error correction is done
*                        1 -- perform single-bit error correction
*   *exi_len           - length of the extension header if the incoming GFP 
*                        frame's extracted EXI value matchs with EXI_VALUE1
*   *ehec_enable       - TRUE: enable EHEC check.
*                        FALSE: disable EHEC check.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_ehec_en_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *ehec_err_cor,
                                      UINT32 *exi_len,
                                      BOOL   *ehec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 exi_val = 0;
    UINT32 exi_val1 = 0;
    UINT32 exi_len1 = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ehec_err_cor);
    ADPT_PARA_VERIFY(exi_len);
    ADPT_PARA_VERIFY(ehec_enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_get(digi_handle,
                                           map_handle,
                                           (UINT32*)ehec_enable,
                                           ehec_err_cor,
                                           &exi_val,
                                           exi_len,
                                           &exi_val1,
                                           &exi_len1);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_ehec_en_get */


/*******************************************************************************
* adpt_gfp_tx_exi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted EXI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   exi_value          - EXI value. Valid: bit0 ~ bit3.
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
PUBLIC PMC_ERROR adpt_gfp_tx_exi_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8  exi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, exi_value);

    /* check parameter validation */
    if (0 != (exi_value & 0xF0))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        PMC_MEMSET((void*)&gfp_cfg, 0, sizeof(cbrc_fc1200_rx_gfp_t));
        gfp_cfg.mask = CBRC_FC1200_RX_GFP_MASK_EXI;
        gfp_cfg.exi  = exi_value;
        rc = digi_cbr_fc1200_rx_gfp_set(digi_handle, cbr_handle, &gfp_cfg);
    }
    else
    {
        rc = digi_mapper_gfp_tx_exi_set(digi_handle, map_handle, exi_value);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_exi_set */


/*******************************************************************************
* adpt_gfp_tx_exi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted EXI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *exi_value         - EXI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_exi_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8  *exi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    UINT32 exi_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(exi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_cbr_fc1200_rx_gfp_get(digi_handle, cbr_handle, &gfp_cfg);
        *exi_value = gfp_cfg.exi;
    }
    else
    {
        rc = digi_mapper_gfp_tx_exi_get(digi_handle, map_handle, &exi_val);
        *exi_value = (UINT8)exi_val;
    }


    PMC_RETURN(rc);
} /* adpt_gfp_tx_exi_get */


/*******************************************************************************
* adpt_gfp_exp_exi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected EXI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   exi_value1         - EXI value #1 with which the extracted EXI value from 
*                        received GFP frame is compared to determine the 
*                        length of Extension header.
*   exi_value2         - EXI value #2 with which the extracted EXI value from 
*                        received GFP frame is compared to determine the 
*                        length of Extension header.
*
*                        If received EXI value isn't equal to 0, 1, exi_value1
*                        or exi_value2, dEXI will be raised.
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
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAPOTN_ODU4P_100_GE_GFP 
*   DIGI_MAPOTN_ODU3P_40_GE_GFP
*   DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP
*   DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_exi_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 exi_value1,
                                   UINT8 exi_value2)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 ehec_en = 0;
    UINT32 ehec_err = 0;
    UINT32 exi_length = 0;
    UINT32 exi_val  = 0;
    UINT32 exi_length1 = 0;
    UINT32 exi_val1 = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               exi_value1, exi_value2);

    /* check parameter validation */
    if ((0 != (exi_value1 & 0xF0)) || (0 != (exi_value2 & 0xF0)))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_get(digi_handle,
                                           map_handle,
                                           &ehec_en,
                                           &ehec_err,
                                           &exi_val,
                                           &exi_length,
                                           &exi_val1,
                                           &exi_length1);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_set(digi_handle,
                                           map_handle,
                                           ehec_en,
                                           ehec_err,
                                           (UINT32)exi_value1,
                                           exi_length,
                                           (UINT32)exi_value2,
                                           exi_length1);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_gfp_exp_exi_set */


/*******************************************************************************
* adpt_gfp_exp_exi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected EXI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *exi_value1        - EXI value #1 with which the extracted EXI value from 
*                        received GFP frame is compared to determine the 
*                        length of Extension header.
*   *exi_value2        - EXI value #2 with which the extracted EXI value from 
*                        received GFP frame is compared to determine the 
*                        length of Extension header.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_exi_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *exi_value1,
                                   UINT8 *exi_value2)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 ehec_en = 0;
    UINT32 ehec_err = 0;
    UINT32 exi_length = 0;
    UINT32 exi_val  = 0;
    UINT32 exi_length1 = 0;
    UINT32 exi_val1 = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(exi_value1);
    ADPT_PARA_VERIFY(exi_value2);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_rx_ext_header_get(digi_handle,
                                           map_handle,
                                           &ehec_en,
                                           &ehec_err,
                                           &exi_val,
                                           &exi_length,
                                           &exi_val1,
                                           &exi_length1);

    *exi_value1 = (UINT8)exi_val;
    *exi_value2 = (UINT8)exi_val1;

    PMC_RETURN(rc);
} /* adpt_gfp_exp_exi_get */


/*******************************************************************************
* adpt_gfp_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted UPI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   upi_value          - UPI value.
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
PUBLIC PMC_ERROR adpt_gfp_tx_upi_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8  upi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, upi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        PMC_MEMSET((void*)&gfp_cfg, 0, sizeof(cbrc_fc1200_rx_gfp_t));
        gfp_cfg.mask = CBRC_FC1200_RX_GFP_MASK_UPI;
        gfp_cfg.upi  = upi_value;
        rc = digi_cbr_fc1200_rx_gfp_set(digi_handle, cbr_handle, &gfp_cfg);
    }
    else
    {
        rc = digi_mapper_gfp_tx_upi_set(digi_handle, map_handle, (UINT32)upi_value);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_upi_set */


/*******************************************************************************
* adpt_gfp_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted UPI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *upi_value         - UPI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_upi_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8  *upi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    UINT32 upi_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(upi_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_cbr_fc1200_rx_gfp_get(digi_handle, cbr_handle, &gfp_cfg);
        *upi_value = gfp_cfg.upi;
    }
    else
    {
        rc = digi_mapper_gfp_tx_upi_get(digi_handle, map_handle, &upi_val);
        *upi_value = (UINT8)upi_val;  
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_upi_get */


/*******************************************************************************
* adpt_gfp_exp_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected UPI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   upi_value          - UPI value.
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
PUBLIC PMC_ERROR adpt_gfp_exp_upi_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 upi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT8 org_upi_val = 0;
    UINT8 pti_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, upi_value);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get original PTI and UPI */
    rc = adpt_gfp_exp_upi_get(dev_id, chnl_id, &org_upi_val);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_gfp_exp_pti_get(dev_id, chnl_id, &pti_val);
    ADPT_RETURN_VERIFY(rc);

    if (org_upi_val != upi_value)
    {
        /* filter out CMF frames to CPU FIFO */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          0,
                                          0x1,
                                          MAPOTN_FLD_EQ,
                                          0x4,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_CPU);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_cpu_fifo_enable(digi_handle,
                                         map_handle);
        ADPT_RETURN_VERIFY(rc);

        /* send client frames with a particular UPI/PTI value to the egress */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          1,
                                          0x3,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)upi_value,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_EGRESS);
        ADPT_RETURN_VERIFY(rc);

        /* drop remaining client frames and trigger a dUPM */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          2,
                                          0x3,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)upi_value,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_DROP);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_exp_upi_set */


/*******************************************************************************
* adpt_gfp_exp_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected UPI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *upi_value         - UPI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_upi_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *upi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 field_mask = 0;
    mapotn_rgfpf_comparison_op_t pti_comp = MAPOTN_FLD_EQ; 
    UINT32 pti_val = 0;
    mapotn_rgfpf_comparison_op_t upi_comp = MAPOTN_FLD_EQ; 
    UINT32 upi_val = 0;
    mapotn_rgfpf_comparison_op_t exi_comp = MAPOTN_FLD_EQ; 
    UINT32 exi_val = 0;
    mapotn_rgfpf_classify_actions_t action = MAPOTN_ACTION_DROP;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(upi_value);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_classify_get(digi_handle,
                                      map_handle,
                                      1,
                                      &field_mask,
                                      &pti_comp,
                                      &pti_val,
                                      &upi_comp,
                                      &upi_val,
                                      &exi_comp,
                                      &exi_val,
                                      &action);
    *upi_value = (UINT8)upi_val;

    PMC_RETURN(rc);
} /* adpt_gfp_exp_upi_get */


/*******************************************************************************
* adpt_gfp_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the received UPI information of the CMF(Client Management Frame) 
*   frame. If there is no valid CMF received in the Rx direction, the upi_value
*   will return 0.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *upi_value         - UPI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_upi_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT8 *upi_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cmf_extract_cmf_t cmf_data;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(upi_value);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&(cmf_data), 0, sizeof(cmf_data));
    rc = adpt_gfp_rx_cmf_get(dev_id, chnl_id, DIGI_GFP_CMF_INT, &cmf_data);

    if (cmf_data.retrieved_cmf_valid)
    {
        *upi_value = cmf_data.cmf_typeheader[1];
    }
    else
    {
        *upi_value = 0; 
    }

    PMC_RETURN(rc);
} /* adpt_gfp_rx_upi_get */


/*******************************************************************************
* adpt_gfp_legacy_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures a GSUP43 7.3 datapath to use legacy UPI values. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   data_frm_upi       - data frame UPI value.
*   ordered_set_upi    - ordered set UPI value.
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
*   Only the following values can be selected.
*   Case #1: data_frm_upi -- 0x13; ordered_set_upi -- 0x14
*   Case #2: data_frm_upi -- 0xFD; ordered_set_upi -- 0xFE
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_legacy_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT8  data_frm_upi,
                                       UINT8  ordered_set_upi)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               data_frm_upi, ordered_set_upi);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_gsup43_7_3_legacy_mode_cfg(digi_handle,
                                         map_handle,
                                         data_frm_upi,
                                         ordered_set_upi);

    PMC_RETURN(rc);
} /* adpt_gfp_legacy_mode_set */


/*******************************************************************************
* adpt_gfp_legacy_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves legacy UPI values for a GSUP43 7.3 datapath. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *data_frm_upi      - data frame UPI value.
*   *ordered_set_upi   - ordered set UPI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_legacy_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT8  *data_frm_upi,
                                       UINT8  *ordered_set_upi)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL8 enable = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(data_frm_upi);
    ADPT_PARA_VERIFY(ordered_set_upi);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->traffic_info[index].mapping_mode)
    {
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP:
        case DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET:
            /* handle process */
            rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enet_tx_upi_get(digi_handle,
                                      chnl_handle,
                                      data_frm_upi,
                                      ordered_set_upi,
                                      &enable);
            ADPT_RETURN_VERIFY(rc);
            break;

        case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
            rc = digi_gsup43_7_3_legacy_mode_get(digi_handle,
                                                 map_handle,
                                                 data_frm_upi,
                                                 ordered_set_upi,
                                                 &enable);
            ADPT_RETURN_VERIFY(rc);
            break;

        default:
            rc = PMC_ERR_FAIL;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_legacy_mode_get */


/*******************************************************************************
* adpt_gfp_exp_upi_ext_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected UPI information of the GFP frame which is mainly 
*   used for a GSUP43 7.3 datapath.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   upi_value1         - the first UPI value.
*   upi_value2         - the second UPI value.
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
*   Please note this API, as well as adpt_gfp_exp_upi_ext_get(), can't be used
*   with the adpt_gfp_exp_upi_set() / adpt_gfp_exp_upi_get() at the same time 
*   in a given channel.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_upi_ext_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT8 upi_value1, 
                                      UINT8 upi_value2)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT8 org_upi_val1 = 0;
    UINT8 org_upi_val2 = 0;
    UINT8 pti_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               upi_value1, upi_value2);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_gfp_exp_pti_get(dev_id, chnl_id, &pti_val);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_gfp_exp_upi_ext_get(dev_id, chnl_id, &org_upi_val1, &org_upi_val2);
    ADPT_RETURN_VERIFY(rc);

    if (((org_upi_val1 != upi_value1) && (org_upi_val1 != upi_value2))
        || ((org_upi_val2 != upi_value1) && (org_upi_val2 != upi_value2)))
    {
        /* filter out CMF frames to CPU FIFO */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          0,
                                          0x1,
                                          MAPOTN_FLD_EQ,
                                          0x4,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_CPU);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_cpu_fifo_enable(digi_handle,
                                         map_handle);
        ADPT_RETURN_VERIFY(rc);

        /* send client frames with a particular UPI/PTI value to the egress */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          1,
                                          0x3,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)upi_value1,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_EGRESS);
        ADPT_RETURN_VERIFY(rc);

        /* send client frames with a particular UPI/PTI value to the egress */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          3,
                                          0x3,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)upi_value2,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_EGRESS);
        ADPT_RETURN_VERIFY(rc);

        /* drop remaining client frames and trigger a dUPM */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          2,
                                          0x3,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)upi_value1,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_DROP);
        ADPT_RETURN_VERIFY(rc);

        /* drop remaining client frames and trigger a dUPM */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          4,
                                          0x3,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)pti_val,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)upi_value2,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_DROP);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_exp_upi_ext_set */


/*******************************************************************************
* adpt_gfp_exp_upi_ext_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected UPI information of the GFP frame which is mainly 
*   used for a GSUP43 7.3 datapath.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *upi_value1        - the first UPI value.
*   *upi_value2        - the second UPI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_upi_ext_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT8 *upi_value1,
                                      UINT8 *upi_value2)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 field_mask = 0;
    mapotn_rgfpf_comparison_op_t pti_comp = MAPOTN_FLD_EQ; 
    UINT32 pti_val = 0;
    mapotn_rgfpf_comparison_op_t upi_comp = MAPOTN_FLD_EQ; 
    UINT32 upi_val = 0;
    mapotn_rgfpf_comparison_op_t exi_comp = MAPOTN_FLD_EQ; 
    UINT32 exi_val = 0;
    mapotn_rgfpf_classify_actions_t action = MAPOTN_ACTION_DROP;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(upi_value1);
    ADPT_PARA_VERIFY(upi_value2);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_classify_get(digi_handle,
                                      map_handle,
                                      1,
                                      &field_mask,
                                      &pti_comp,
                                      &pti_val,
                                      &upi_comp,
                                      &upi_val,
                                      &exi_comp,
                                      &exi_val,
                                      &action);
    ADPT_RETURN_VERIFY(rc);
    *upi_value1 = (UINT8)upi_val;

    rc = digi_mapper_gfp_classify_get(digi_handle,
                                      map_handle,
                                      3,
                                      &field_mask,
                                      &pti_comp,
                                      &pti_val,
                                      &upi_comp,
                                      &upi_val,
                                      &exi_comp,
                                      &exi_val,
                                      &action);
    ADPT_RETURN_VERIFY(rc);
    *upi_value2 = (UINT8)upi_val;

    PMC_RETURN(rc);
} /* adpt_gfp_exp_upi_ext_get */


/*******************************************************************************
* adpt_gfp_non_std_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API will pass the FW to handle the required non standard UPI values 
*   needed in the CMF frames. The FW will then execute all actions with the 
*   requested UPI values in the CMF frames. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   los_upi            - alternate LOS UPI value for FW to use.
*   loss_sync_upi      - alternate LOSS SYNC UPI value for FW to use.
*   dci_upi            - alternate DCI UPI value for FW to use.
*   fdi_upi            - alternate FDI UPI value for FW to use.
*   rdi_upi            - alternate RDI UPI value for FW to use.
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
PUBLIC PMC_ERROR adpt_gfp_non_std_upi_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT32 los_upi,
                                       UINT32 loss_sync_upi,
                                       UINT32 dci_upi,
                                       UINT32 fdi_upi,
                                       UINT32 rdi_upi)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cmf_upi_values_t upi_values;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d].\r\n", dev_id,  
               chnl_id, los_upi, loss_sync_upi, dci_upi, fdi_upi, rdi_upi);

    upi_values.los_upi       = los_upi;
    upi_values.loss_sync_upi = loss_sync_upi;
    upi_values.dci_upi       = dci_upi;
    upi_values.fdi_upi       = fdi_upi;
    upi_values.rdi_upi       = rdi_upi;

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_non_std_upi_cfg(digi_handle, map_handle, upi_values);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_gfp_non_std_upi_set */


/*******************************************************************************
* adpt_gfp_tx_pti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the transmitted PTI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pti_value          - PTI value.
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
PUBLIC PMC_ERROR adpt_gfp_tx_pti_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 pti_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, pti_value);

    /* check parameter validation */
    if (0 != (pti_value & 0xF8))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        PMC_MEMSET((void*)&gfp_cfg, 0, sizeof(cbrc_fc1200_rx_gfp_t));
        gfp_cfg.mask = CBRC_FC1200_RX_GFP_MASK_PTI;
        gfp_cfg.pti  = pti_value;
        rc = digi_cbr_fc1200_rx_gfp_set(digi_handle, cbr_handle, &gfp_cfg);
    }
    else
    {
        rc = digi_mapper_gfp_tx_pti_set(digi_handle, map_handle, (UINT32)pti_value);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_pti_set */


/*******************************************************************************
* adpt_gfp_tx_pti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the transmitted PTI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pti_value         - PTI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_pti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8  *pti_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    cbrc_fc1200_rx_gfp_t gfp_cfg;
    UINT32 pti_val = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pti_value);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_MAPOTN_ODU2EP_FC_1200_GFP == adpt_handle->traffic_info[index].mapping_mode)
    {
        /* handle process */
        rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_cbr_fc1200_rx_gfp_get(digi_handle, cbr_handle, &gfp_cfg);
        *pti_value = gfp_cfg.pti;
    }
    else
    {
        rc = digi_mapper_gfp_tx_pti_get(digi_handle, map_handle, &pti_val);
        *pti_value = (UINT8)pti_val;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_pti_get */


/*******************************************************************************
* adpt_gfp_exp_pti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the expected PTI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pti_value          - PTI value.
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
PUBLIC PMC_ERROR adpt_gfp_exp_pti_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 pti_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT8 org_pti_val = 0;
    UINT8 upi_val = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, pti_value);

    /* check parameter validation */
    if (0 != (pti_value & 0xF8))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get original PTI and UPI */
    rc = adpt_gfp_exp_upi_get(dev_id, chnl_id, &upi_val);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_gfp_exp_pti_get(dev_id, chnl_id, &org_pti_val);
    ADPT_RETURN_VERIFY(rc);

    if (org_pti_val != pti_value)
    {
        /* filter out CMF frames to CPU FIFO */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          0,
                                          0x1,
                                          MAPOTN_FLD_EQ,
                                          0x4,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_CPU);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_mapper_cpu_fifo_enable(digi_handle,
                                         map_handle); 
        ADPT_RETURN_VERIFY(rc);

        /* send client frames with a particular UPI/PTI value to the egress */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          1,
                                          0x3,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)pti_value,
                                          MAPOTN_FLD_EQ,
                                          (UINT32)upi_val,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_EGRESS);
        ADPT_RETURN_VERIFY(rc);

        /* drop remaining client frames and trigger a dUPM */
        rc = digi_mapper_gfp_classify_cfg(digi_handle,
                                          map_handle,
                                          2,
                                          0x3,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)pti_value,
                                          MAPOTN_FLD_NQ,
                                          (UINT32)upi_val,
                                          MAPOTN_FLD_EQ,
                                          0,
                                          MAPOTN_ACTION_DROP);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_exp_pti_set */


/*******************************************************************************
* adpt_gfp_exp_pti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the expected PTI information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pti_value         - PTI value.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_exp_pti_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *pti_value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    UINT32 field_mask = 0;
    mapotn_rgfpf_comparison_op_t pti_comp = MAPOTN_FLD_EQ; 
    UINT32 pti_val = 0;
    mapotn_rgfpf_comparison_op_t upi_comp = MAPOTN_FLD_EQ; 
    UINT32 upi_val = 0;
    mapotn_rgfpf_comparison_op_t exi_comp = MAPOTN_FLD_EQ; 
    UINT32 exi_val = 0;
    mapotn_rgfpf_classify_actions_t action = MAPOTN_ACTION_DROP;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pti_value);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_classify_get(digi_handle,
                                      map_handle,
                                      1,
                                      &field_mask,
                                      &pti_comp,
                                      &pti_val,
                                      &upi_comp,
                                      &upi_val,
                                      &exi_comp,
                                      &exi_val,
                                      &action);
    *pti_value = (UINT8)pti_val;

    PMC_RETURN(rc);
} /* adpt_gfp_exp_pti_get */


/*******************************************************************************
* adpt_gfp_frm_len_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures minimum and maximum length of payload information 
*   field in a GFP frame. Frames with client payload information field 
*   (payload area excluding header and optional pFCS) out of the range 
*   of lengths are unconditionally discarded.
*
*   Frame minimum and maximum length check is applicable only for frames 
*   with PTI = "000".
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   min_frm_len        - minimum frame length.
*   max_frm_len        - maximum frame length.
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
PUBLIC PMC_ERROR adpt_gfp_frm_len_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT32 min_frm_len,
                                  UINT32 max_frm_len)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               min_frm_len, max_frm_len);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_gfp_frm_len_cfg(digi_handle,
                                     map_handle,
                                     max_frm_len,
                                     min_frm_len);

    PMC_RETURN(rc);
} /* adpt_gfp_frm_len_set */


/*******************************************************************************
* adpt_gfp_frm_len_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the minimum and maximum length of payload information 
*   field in a GFP frame. Frames with client payload information field 
*   (payload area excluding header and optional pFCS) out of the range 
*   of lengths are unconditionally discarded.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *min_frm_len       - minimum frame length.
*   *max_frm_len       - maximum frame length.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_frm_len_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   UINT32 *min_frm_len,
                                   UINT32 *max_frm_len)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(min_frm_len);
    ADPT_PARA_VERIFY(max_frm_len);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    *min_frm_len = 0;
    *max_frm_len = 0;

    PMC_RETURN(rc);
} /* adpt_gfp_frm_len_get */


/*******************************************************************************
* adpt_gfp_tx_conseq_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables OPU-CSF / GFP defect auto insertion in Tx direction. The
*   following defects are supported.
*
*   GFPdCSF-LOS(Loss Of client Signal)
*   GFPdCSF-LOCS(Loss Of Character Synchronization)
*   GFPdDCI
*   GFPdFDI
*   GFPdRDI
*   OPUdCSF
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_msk            - defect mask to trigger the consequent action. The 
*                        defect mask lists as follows.
*                        DIGI_ACT_ENET_PHY_LOS
*                        DIGI_ACT_ENET_BLK_LCK    
*                        DIGI_ACT_ENET_HI_BER     
*                        DIGI_ACT_ENET_RX_LF
*                        DIGI_ACT_ENET_RX_RF      
*                        DIGI_ACT_CBRC_LOS_SYNC   
*                        DIGI_ACT_CBRC_LOS_SIG    
*                        DIGI_ACT_FC1200_LOS_SYNC 
*                        DIGI_ACT_FC1200_LOS_SIG  
*                        DIGI_ACT_SDH_LOS_SIG     
*                        DIGI_ACT_SDH_LOS_FRM  
*                        DIGI_ACT_EPMM_LOS
*                        DIGI_ACT_EPMM_LF
*                        DIGI_ACT_EPMM_RF
*                        DIGI_ACT_SW_FORCE
*   alm_act1           - the first defect action for insertion.
*   alm_act2           - the second defect action for insertion.
*   enable             - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
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
*   1. This API is available for those datapaths through MAPOTN.
*   2. This API can only be called after a full datapath provisioning and 
*      activation. After reactivating the datapath, the consequential actions 
*      may once again be re-configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_conseq_act_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        UINT32 alm_msk,
                                        digi_gfp_tx_act_t alm_act1,
                                        digi_gfp_tx_act_t alm_act2,
                                        BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_client2trunk_tables_cmf_t csf_table;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               alm_msk, alm_act1, alm_act2, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_client2trunk_tables_cmf_t));

    rc = digi_cmf_clienttotrunk_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_clienttotrunk_port_action_get(digi_handle,
                                                map_handle,
                                                &csf_table.port_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* workaround: reset unecessary consequent action */
    rc = adpt_tx_conseq_act_reset(dev_id, chnl_id, &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    csf_table.port_action_cmf.enable_monitoring = 1;
    /* ENET PHY LOS */
    if (DIGI_ACT_ENET_PHY_LOS & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_emac_phy_los_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_emac_phy_los_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_emac_phy_los_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_emac_phy_los_mon.priority_idx   = enable ? 3 : 0;
    }
    /* ENET BLK LCK */
    if (DIGI_ACT_ENET_BLK_LCK & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_emac_block_lock_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_emac_block_lock_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_emac_block_lock_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_emac_block_lock_mon.priority_idx   = enable ? 5 : 0;
    }
    /* ENET HIGH BER */
    if (DIGI_ACT_ENET_HI_BER & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.priority_idx   = enable ? 6 : 0;
    }
    /* ENET RX LF */
    if (DIGI_ACT_ENET_RX_LF & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.priority_idx   = enable ? 7 : 0;
    }
    /* ENET RX RF */
    if (DIGI_ACT_ENET_RX_RF & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.priority_idx   = enable ? 9 : 0;
    }
    /* CBRC LOSS SYNC */
    if (DIGI_ACT_CBRC_LOS_SYNC & alm_msk)
    {
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.priority_idx   = enable ? 2 : 0;
    }
    /* CBRC LOS */
    if (DIGI_ACT_CBRC_LOS_SIG & alm_msk)
    {
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.priority_idx   = enable ? 3 : 0;
    }
    /* FC1200 LOSS SYNC */
    if (DIGI_ACT_FC1200_LOS_SYNC & alm_msk)
    {
        csf_table.conseq_action_cmf.fc1200_loss_sync_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.fc1200_loss_sync_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.fc1200_loss_sync_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.fc1200_loss_sync_mon.priority_idx   = enable ? 2 : 0;
    }
    /* FC1200 LOS */
    if (DIGI_ACT_FC1200_LOS_SIG & alm_msk)
    {
        csf_table.conseq_action_cmf.fc1200_loss_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.fc1200_loss_signal_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.fc1200_loss_signal_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.fc1200_loss_signal_mon.priority_idx   = enable ? 3 : 0;
    }
    /* SDH/SONET LOS SIGNAL */
    if (DIGI_ACT_SDH_LOS_SIG & alm_msk)
    {
        csf_table.conseq_action_cmf.sonet_loss_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.sonet_loss_signal_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.sonet_loss_signal_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.sonet_loss_signal_mon.priority_idx   = enable ? 2 : 0;
    }
    /* SDH/SONET LOF */
    if (DIGI_ACT_SDH_LOS_FRM & alm_msk)
    {
        csf_table.conseq_action_cmf.sonet_loss_frame_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.sonet_loss_frame_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.sonet_loss_frame_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.sonet_loss_frame_mon.priority_idx   = enable ? 3 : 0;
    }
    /* EPMM LOS */
    if (DIGI_ACT_EPMM_LOS & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.priority_idx   = enable ? 4 : 0;
    }
    /* EPMM LF */
    if (DIGI_ACT_EPMM_LF & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.priority_idx   = enable ? 8 : 0;
    }
    /* EPMM RF */
    if (DIGI_ACT_EPMM_RF & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.priority_idx   = enable ? 10 : 0;
    }
    /* SW force/manual trigger */
    if (DIGI_ACT_SW_FORCE & alm_msk)
    {
        csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.priority_idx   = enable ? 1 : 0;
    }
    /* ENET 40/100G LOAM */
    if (DIGI_ACT_ENET_LOAM & alm_msk)
    {
        csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.action1        = (digi_client2trunk_actions_t)alm_act1;
        csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.action2        = (digi_client2trunk_actions_t)alm_act2;
        csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.priority_idx   = enable ? 2 : 0;
    }

    rc = digi_cmf_clienttotrunk_conseq_action_cfg(digi_handle, map_handle, &csf_table);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_clienttotrunk_tables_update(digi_handle, map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* SW force/manual trigger */
    if (DIGI_ACT_SW_FORCE & alm_msk)
    {
        rc = digi_cmf_clienttotrunk_sw_manual_trigger(digi_handle, map_handle, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_conseq_act_set */


/*******************************************************************************
* adpt_gfp_tx_conseq_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable / disable status of OPU-CSF / GFP defect auto 
*   insertion in Tx direction. The following defects are supported.
*
*   GFPdCSF-LOS(Loss Of client Signal)
*   GFPdCSF-LOCS(Loss Of Character Synchronization)
*   GFPdDCI
*   GFPdFDI
*   GFPdRDI
*   OPUdCSF
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_item           - defect item to trigger the consequent action.
*
* OUTPUTS:
*   *alm_act1          - the first defect action for insertion.
*   *alm_act2          - the second defect action for insertion.
*   *enable            - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is available for those datapaths through MAPOTN. Only one mask can
*   be used for each calling.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_tx_conseq_act_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_alm_act_mask_t alm_item,
                                        digi_gfp_tx_act_t *alm_act1,
                                        digi_gfp_tx_act_t *alm_act2,
                                        BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_client2trunk_tables_cmf_t csf_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(alm_act1);
    ADPT_PARA_VERIFY(alm_act2);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_client2trunk_tables_cmf_t));
    rc = digi_cmf_clienttotrunk_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* ENET PHY LOS */
    if (DIGI_ACT_ENET_PHY_LOS & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.enet_emac_phy_los_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_phy_los_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_phy_los_mon.action2;
        PMC_RETURN(rc);
    }
    /* ENET BLK LCK */
    if (DIGI_ACT_ENET_BLK_LCK & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_emac_block_lock_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_block_lock_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_block_lock_mon.action2;
        PMC_RETURN(rc);
    }
    /* ENET HIGH BER */
    if (DIGI_ACT_ENET_HI_BER & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_hi_ber_mon.action2;
        PMC_RETURN(rc);
    }
    /* ENET RX LF */
    if (DIGI_ACT_ENET_RX_LF & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_rx_loc_fault_mon.action2;
        PMC_RETURN(rc);
    }
    /* ENET RX RF */
    if (DIGI_ACT_ENET_RX_RF & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_emac_rx_rem_fault_mon.action2;
        PMC_RETURN(rc);
    }
    /* CBRC LOSS SYNC */
    if (DIGI_ACT_CBRC_LOS_SYNC & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.cbrc_8b10b_loss_sync_mon.action2;
        PMC_RETURN(rc);
    }
    /* CBRC LOS */
    if (DIGI_ACT_CBRC_LOS_SIG & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.cbrc_8b10b_loss_signal_mon.action2;
        PMC_RETURN(rc);
    }
    /* FC1200 LOSS SYNC */
    if (DIGI_ACT_FC1200_LOS_SYNC & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.fc1200_loss_sync_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.fc1200_loss_sync_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.fc1200_loss_sync_mon.action2;
        PMC_RETURN(rc);
    }
    /* FC1200 LOS */
    if (DIGI_ACT_FC1200_LOS_SIG & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.fc1200_loss_signal_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.fc1200_loss_signal_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.fc1200_loss_signal_mon.action2;
        PMC_RETURN(rc);
    }
    /* SDH/SONET LOSS SIGNAL */
    if (DIGI_ACT_SDH_LOS_SIG & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.sonet_loss_signal_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sonet_loss_signal_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sonet_loss_signal_mon.action2;
        PMC_RETURN(rc);
    }
    /* SDH/SONET LOSS FRM */
    if (DIGI_ACT_SDH_LOS_FRM & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.sonet_loss_frame_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sonet_loss_frame_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sonet_loss_frame_mon.action2;
        PMC_RETURN(rc);
    }
    /* EPMM LOS */
    if (DIGI_ACT_EPMM_LOS & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_loss_signal_mon.action2;
        PMC_RETURN(rc);
    }
    /* EPMM LF */
    if (DIGI_ACT_EPMM_LF & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_local_fault_mon.action2;
        PMC_RETURN(rc);
    }
    /* EPMM RF */
    if (DIGI_ACT_EPMM_RF & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_epmm_remote_fault_mon.action2;
        PMC_RETURN(rc);
    }
    /* SW force/manual trigger */
    if (DIGI_ACT_SW_FORCE & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.sw_force_manual_trigger_mon.action2;
        PMC_RETURN(rc);
    }
    /* ENET 40/100G LOAM */
    if (DIGI_ACT_ENET_LOAM & alm_item)
    {
        *enable   = csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.monitor_defect;
        *alm_act1 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.action1;
        *alm_act2 = (digi_gfp_tx_act_t)csf_table.conseq_action_cmf.enet_mac_40g_100g_loam_mon.action2;
        PMC_RETURN(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_tx_conseq_act_get */


/*******************************************************************************
* adpt_gfp_rx_conseq_act_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables client traffic defect auto insertion in Rx direction. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_msk            - defect mask to trigger client traffic defect auto 
*                        insertion. The defect mask lists as follows.
*                        DIGI_ACT_SSF_SF
*                        DIGI_ACT_PLM
*                        DIGI_ACT_OPU_CSF
*                        DIGI_ACT_GFP_LFD
*                        DIGI_ACT_GFP_UPM
*                        DIGI_ACT_GFP_CSF_LOS
*                        DIGI_ACT_GFP_CSF_LOCS
*                        DIGI_ACT_GFP_FDI
*                        DIGI_ACT_GFP_RDI
*                        DIGI_ACT_SDH_SFIS_LOF
*                        DIGI_ACT_GFP_EXM
*   alm_act            - defect action for insertion.
*   enable             - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
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
*   1. This API is available for those datapaths through MAPOTN.
*   2. This API can only be called after a full datapath provisioning and 
*      activation. After reactivating the datapath, the consequential actions 
*      may once again be re-configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        UINT32 alm_msk,
                                        digi_gfp_rx_act_t alm_act,
                                        BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_trunk2client_tables_cmf_t csf_table;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               alm_msk, alm_act, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_trunk2client_tables_cmf_t));

    rc = digi_cmf_trunktoclient_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);
    rc = digi_cmf_trunktoclient_port_mapping_get(digi_handle,
                                                 map_handle,
                                                 &csf_table.port_mapping_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* workaround: reset unecessary consequent action */
    rc = adpt_rx_conseq_act_reset(dev_id, chnl_id, &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* SSF_SF */
    if (DIGI_ACT_SSF_SF & alm_msk)
    {
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.priority_idx   = enable ? 1 : 0;
    }
    /* dPLM */
    if (DIGI_ACT_PLM & alm_msk)
    {
        csf_table.conseq_action_cmf.dplm_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.dplm_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.dplm_mon.priority_idx   = enable ? 2 : 0;
    }
    /* OPUdCSF */
    if (DIGI_ACT_OPU_CSF & alm_msk)
    {
        csf_table.conseq_action_cmf.opu_csf_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.opu_csf_mon.action1        = NO_T2C_ACTION;
        csf_table.conseq_action_cmf.opu_csf_mon.action2        = (digi_trunk2client_actions_t)alm_act;
        /* 100 ms holdoff */ 
        csf_table.conseq_action_cmf.opu_csf_mon.second_action_holdoff_time = 100;
        csf_table.conseq_action_cmf.opu_csf_mon.priority_idx   = enable ? 3 : 0;
    }
    /* GFPdLFD */
    if (DIGI_ACT_GFP_LFD & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_lofd_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_lofd_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_lofd_mon.priority_idx   = enable ? 4 : 0;
    }
    /* GFPdUPM */
    if (DIGI_ACT_GFP_UPM & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.priority_idx   = enable ? 5 : 0;
    }
    /* GFPdCSF_LOS */
    if (DIGI_ACT_GFP_CSF_LOS & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.priority_idx   = enable ? 6 : 0;
    }
    /* GFPdCSF_LOCS */
    if (DIGI_ACT_GFP_CSF_LOCS & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.priority_idx   = enable ? 7 : 0;
    }
    /* GFPdEXM */
    if (DIGI_ACT_GFP_EXM & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_dexm_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_dexm_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_dexm_mon.priority_idx   = enable ? 8 : 0;
    }
    /* GFPdFDI */
    if (DIGI_ACT_GFP_FDI & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.priority_idx   = enable ? 9 : 0;
    }
    /* GFPdRDI */
    if (DIGI_ACT_GFP_RDI & alm_msk)
    {
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.priority_idx   = enable ? 10 : 0;
    }
    /* SDHdLOF */
    if (DIGI_ACT_SDH_SFIS_LOF & alm_msk)
    {
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.monitor_defect = enable;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action1        = (digi_trunk2client_actions_t)alm_act;
        csf_table.conseq_action_cmf.sonet_sfis_lof_mon.priority_idx   = enable ? 11 : 0;
    }

    rc = digi_cmf_trunktoclient_conseq_action_cfg(digi_handle,
                                                  map_handle,
                                                  &csf_table);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_trunktoclient_tables_update(digi_handle,
                                              map_handle);

    PMC_RETURN(rc);
} /* adpt_gfp_rx_conseq_act_set */


/*******************************************************************************
* adpt_gfp_rx_conseq_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves enable / disable status of client traffic defect auto insertion 
*   in Rx direction. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   alm_item           - defect item to trigger client traffic defect auto 
*                        insertion.
*
* OUTPUTS:
*   *alm_act           - defect action for insertion.
*   *enable            - TRUE: enable consequent action auto insertion.
*                        FALSE: disable consequent action auto insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is available for those datapaths through MAPOTN. Only one mask can
*   be used for each calling.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_rx_alm_act_mask_t alm_item,
                                        digi_gfp_rx_act_t *alm_act,
                                        BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    digi_trunk2client_tables_cmf_t csf_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(alm_act);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&csf_table, 0, sizeof(digi_trunk2client_tables_cmf_t));
    rc = digi_cmf_trunktoclient_conseq_action_get(digi_handle,
                                                  map_handle,
                                                  &csf_table.conseq_action_cmf);
    ADPT_RETURN_VERIFY(rc);

    /* SSF_SF */
    if (DIGI_ACT_SSF_SF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.ssf_sf_rcvd_mon.action1;
        PMC_RETURN(rc);
    }
    /* dPLM */
    if (DIGI_ACT_PLM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.dplm_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.dplm_mon.action1;
        PMC_RETURN(rc);
    }
    /* OPUdCSF */
    if (DIGI_ACT_OPU_CSF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.opu_csf_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.opu_csf_mon.action2;
        PMC_RETURN(rc);
    }
    /* GFPdLFD */
    if (DIGI_ACT_GFP_LFD & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_lofd_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_lofd_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdUPM */
    if (DIGI_ACT_GFP_UPM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_pti_upi_mismatch_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdCSF_LOS */
    if (DIGI_ACT_GFP_CSF_LOS & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdCSF_LOCS */
    if (DIGI_ACT_GFP_CSF_LOCS & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdEXM */
    if (DIGI_ACT_GFP_EXM & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_dexm_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_dexm_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdFDI */
    if (DIGI_ACT_GFP_FDI & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_fdi_mon.action1;
        PMC_RETURN(rc);
    }
    /* GFPdRDI */
    if (DIGI_ACT_GFP_RDI & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.gfp_cmf_rdi_mon.action1;
        PMC_RETURN(rc);
    }
    /* SDHdLOF */
    if (DIGI_ACT_SDH_SFIS_LOF & alm_item)
    {
        *enable  = csf_table.conseq_action_cmf.sonet_sfis_lof_mon.monitor_defect;
        *alm_act = (digi_gfp_rx_act_t)csf_table.conseq_action_cmf.sonet_sfis_lof_mon.action1;
        PMC_RETURN(rc);
    }

    PMC_RETURN(rc);
} /* adpt_gfp_rx_conseq_act_get */


/*******************************************************************************
* adpt_gfp_os_rate_inc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API configures a transparent G.sup43 7.3 datapath to generate a 
*   higher rate of ordered sets. This API is only applicable to a datapath 
*   that contains a MAPOTN <-> CPB <-> ENET (EPMM) segment.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable higher rate of ordered sets.
*                        FALSE: disable higher rate of ordered setes.
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
PUBLIC PMC_ERROR adpt_gfp_os_rate_inc_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      BOOL   enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_gsup43_7_3_increase_orderred_set_rate(digi_handle,
                                                    map_handle,
                                                    (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_gfp_os_rate_inc_set */


/*******************************************************************************
* adpt_gfp_os_rate_inc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the higher rate status of ordered sets for a transparent G.sup43 
*   7.3 datapath. This API is only applicable to a datapath that contains a 
*   MAPOTN <-> CPB <-> ENET (EPMM) segment.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable higher rate of ordered sets.
*                        FALSE: disable higher rate of ordered setes.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_os_rate_inc_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      BOOL   *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    BOOL8 os_enable = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_gsup43_7_3_increase_orderred_set_rate_get(digi_handle,
                                                        map_handle,
                                                        &os_enable);
    *enable = os_enable ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_gfp_os_rate_inc_get */


/*
** End of file
*/
