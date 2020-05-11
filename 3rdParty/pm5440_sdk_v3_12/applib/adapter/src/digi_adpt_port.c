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
*     This file describes the peripheral port configuration Run-Time function.
*     Include the following:
*       -- OHP (OverHead add/drop Port)
*       -- DI RCP(Defect Information Ring Control Port)
*       -- RI RCP(Remote Information ODU Ring Control Port)
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
* adpt_otn_ohp_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   There are three ODU Overhead processors (OHPs). Each OHP contains two 
*   ports (port A and port B) which can connect to two different framers.
*   The first OHP (ODU_OHP_1) supports 12 channels and has a fixed 
*   connection to the stage 1 (HO) framer (PORT A) and the Stage N framer (PORT B). 
*   The second and third OHPs can be configured to connect to framer 2 (MO), 
*   framer 3A (LO), framer 3B (LO) or framer 4.\n\n
*
*   This function initializes a single OHP. The function should be called once
*   for each OHP that is needed. The configuration of each OHP is specified 
*   in a odu_ohp_cfg_t structure which has the following fields:\n
*       ohp                 - Specifies which OHP (ODU_OHP_1, 
*                             ODU_OHP_2 or ODU_OHP_3) 
*       port_a_framer       - Port A framer connection 
*       port_a_src_addr     - Port A Source user ID (32 bits). 
*       port_a_dest_addr    - Port A Destination user id (32 bits) 
*       port_b_framer       - Port B framer connection
*       port_b_src_addr     - Port B Source user ID (32 bits). 
*       port_b_dest_addr    - Port B Destination user id (32 bits) 
*  
*   If a port is unused, the value should be set to ODU_OHP_PORT_UNUSED. The OHP1 
*   port_a_framer value must either be ODU_OHP_PORT_FRAMER_1 or ODU_OHP_PORT_UNUSED.
*   The OHP1 port_b_framer value must either be ODU_OHP_PORT_FRAMER_N or 
*   ODU_OHP_UNUSED.\n\n
*
*   The ethernet source and destination addresses will consist of:\n
*       -32 bit source user ID
*       -32 bit dest user ID
*       -8 bit framer ID
*       -8 bit channel id.
*
* INPUTS:
*   dev_id             - device identity.
*   *ohp_cfg           - ODU Overhead Processors configuration.
*   enable             - TRUE: enable OHP configuration.
*                        FALSE: disable OHP configuration.
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
*   Please note that GCC2 bytes from STG N (ODU4) via the second framer port of 
*   the OHP1 only work when the src_addr and dest_addr are set to 0.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ohp_en_set(UINT32 dev_id, 
                                   odu_ohp_cfg_t *ohp_cfg,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_cfg_t org_ohp_cfg;
    BOOL org_enable = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ohp_cfg);

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d][%d][%d].\r\n", dev_id, 
               ohp_cfg->ohp, ohp_cfg->port_a_framer, ohp_cfg->port_a_src_addr,
               ohp_cfg->port_a_dest_addr, ohp_cfg->port_b_framer,
               ohp_cfg->port_b_src_addr, ohp_cfg->port_b_dest_addr, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ohp_init_get(adpt_handle->digi_handle,
                               ohp_cfg->ohp,
                               &org_ohp_cfg,
                               &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        if (enable)
        {
            rc = digi_otn_ohp_init(adpt_handle->digi_handle, ohp_cfg);
        }
        else
        {
            rc = digi_otn_ohp_uninit(adpt_handle->digi_handle, ohp_cfg->ohp);
        }
    }
    else if (enable)
    {
        if (0 != PMC_MEMCMP(&org_ohp_cfg, ohp_cfg, sizeof(odu_ohp_cfg_t)))
        {
            rc = digi_otn_ohp_uninit(adpt_handle->digi_handle, ohp_cfg->ohp);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_ohp_init(adpt_handle->digi_handle, ohp_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_otn_ohp_en_set */


/*******************************************************************************
* adpt_otn_ohp_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the OHP configuration for a given OHP channel.
*
* INPUTS:
*   dev_id             - device identity.
*   ohp_inst           - enumeration of the three OHPs.
*
* OUTPUTS:
*   *ohp_cfg           - ODU Overhead Processors configuration.
*   *enable            - TRUE: enable OHP configuration.
*                        FALSE: disable OHP configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ohp_en_get(UINT32 dev_id, 
                                   odu_ohp_inst_t ohp_inst,
                                   odu_ohp_cfg_t *ohp_cfg,
                                   BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ohp_cfg);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ohp_init_get(adpt_handle->digi_handle, ohp_inst, ohp_cfg, enable);

    PMC_RETURN(rc);
} /* adpt_otn_ohp_en_get */


/*******************************************************************************
* adpt_otn_ohp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the overhead extraction/insertion 
*   for a particular channel.
*
*   The ODUk overhead data consists of 4 rows of 16 bytes each for a 
*   total of 64 bytes (16 32-bit words).  The OHP can extract all 
*   overhead bytes except the BIP-8 from PM/TCMs and JC1, JC2 and JC3. 
*   OTUk overhead (Row 1, bytes 7-14) are only extracted by stage 1 
*   framers. The OHP can insert all bytes except the BIP-8 bytes 
*   from SM, PM and TCMs. OTUk overhead (Row 1, bytes 1-14) can only 
*   be inserted by stage 1 framers.
*
*
*   A bit mask is used to indicate which words should be inserted or 
*   extracted. Each bit in the mask indicates whether the corresponding 
*   overhead word (32 bits) should be inserted/extracted. The OHP 
*   supports extracting or inserting a maximum of 10 words. There is a
*   separate mask for insertion (insert_mask) and extraction (extract_mask).
*             ODUk Word         ODUk fields
*             ---------     ---------------------
*                 0         {FAS, FAS, FAS, FAS}
*                 1         {FAS, FAS, MFAS, 0x00}
*                 2:        {SM_TTI, SM_BW_INFO, GCC0_0, GCC0_1}
*                 3:        {RES, RES, JC4, 0x00}
*                 4:        {RES, RES, PM&TCM, TCM_ACT}
*                 5:        {0x00, 0x00, 0x00, 0x00}
*                 6:        {TCM6_TTI, TCM6_BW_INFO, TCM5_TTI, TCM5_BW_INFO}
*                 7:        {TCM4_TTI, TCM4_BW_INFO, FTFL, JC5}
*                 8:        {0x00, 0x00, 0x00, 0x00}
*                 9:        {TCM3_TTI, TCM3_BW_INFO, TCM2_TTI, TCM2_BW_INFO}
*                10:        {TCM1_TTI, TCM1_BW_INFO, PM_TTI, PM_BW_INFO}
*                11:        {EXP, EXP, JC6, 0x00}
*                12:        {GCC1_0, GCC1_1, GCC2_0, GCC2_1}
*                13:        {APS, APS, APS, APS, APS}
*                14:        {RES, RES, RES, RES}
*                15:        {RES, RES, PSI, OMFI}
* 
*   The extraction and insertion ethernet packets include destination 
*   and source addresses. 
*   The destination address consists of:
*       -Destination user id (32 bit) 
*       -Framer ID (8 bit) The OHP input port.
*       -Channel ID (8 bit)
*   The source address consists of:
*       -source user id (32 bit)
*       -framer ID (8 bit) The OHP input port.
*       -Channel ID (8 bit)
*  
*   Framer ID is as follows: 
*
*   Framer ID   Framer 
*   ---------  -------
*       0      Framer 1
*       1      Framer 2
*       2      Framer 3A
*       3      Framer 3B
*       4      Framer 4
*       5      Framer N
*
*   The destination user id and source user id are configured using the 
*   adpt_otn_ohp_en_set function.
*
*   The external data source identifies the individual overhead bytes or 
*   masked overhead bytes (ie. *_BW_INFO notated bytes) with TX_INS_ENn 
*   fields of the Insertion Port Ethernet Packet.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   dir                - traffic direction: Tx, Rx, Rx_Tx.
*                        DIGI_OHP_DIR_RX: Insert overheads into OHP.
*                        DIGI_OHP_DIR_TX: Extract overheads from OHP.
*                        DIGI_OHP_DIR_RX_TX: Extract and insert overheads from OHP.
*   oh_mask            - overhead mask.
*                        Tx: Extraction mask (16 bits). Each bit indicates 
*                        whether the corresponding overhead word should be 
*                        extracted. Exactly 10 bits must be set to 1.
*                        (corresponding to the extraction of 10 words).
*
*                        Rx: Insertion mask (16 bits). Each bit indicates 
*                        whether the corresponding overhead word should be 
*                        inserted. A maximum of 10 bits can be set to 1.
*                        (corresponding to the insertion of 10 words).
*
*   enable             - TRUE: enable the overhead insertion/extraction.
*                        FALSE: disable the overhead insertion/extraction.
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
PUBLIC PMC_ERROR adpt_otn_ohp_cfg(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_loc_t location,
                                digi_ohp_dir_t dir,
                                UINT16 oh_mask,
                                BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT16 org_rx_mask = 0;
    UINT16 org_tx_mask = 0;
    BOOL org_rx_en = FALSE;
    BOOL org_tx_en = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, location, dir, oh_mask, enable);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (dir)
    {
        case DIGI_OHP_DIR_RX:
            rc = adpt_otn_ohp_get(dev_id, chnl_id, location, 
                                  dir, &org_rx_mask, &org_rx_en);
            ADPT_RETURN_VERIFY(rc);

            if (org_rx_en != enable)
            {
                rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                odu_handle,
                                                oh_mask,
                                                enable);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (enable)
            {
                if (org_rx_mask != oh_mask)
                {
                    rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                    odu_handle,
                                                    org_rx_mask,
                                                    FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                    odu_handle,
                                                    oh_mask,
                                                    enable);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
            break;

        case DIGI_OHP_DIR_TX:
            rc = adpt_otn_ohp_get(dev_id, chnl_id, location, 
                                  dir, &org_tx_mask, &org_tx_en);
            ADPT_RETURN_VERIFY(rc);

            if (org_tx_en != enable)
            {
                rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                 odu_handle,
                                                 oh_mask,
                                                 enable);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (enable)
            {
                if (org_tx_mask != oh_mask)
                {
                    rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                     odu_handle,
                                                     org_tx_mask,
                                                     FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                     odu_handle,
                                                     oh_mask,
                                                     enable);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
            break;

        case DIGI_OHP_DIR_RX_TX:
            rc = adpt_otn_ohp_get(dev_id, chnl_id, location, 
                                  DIGI_OHP_DIR_RX, &org_rx_mask, &org_rx_en);
            ADPT_RETURN_VERIFY(rc);

            rc = adpt_otn_ohp_get(dev_id, chnl_id, location, 
                                  DIGI_OHP_DIR_TX, &org_tx_mask, &org_tx_en);
            ADPT_RETURN_VERIFY(rc);

            if ((org_rx_en != enable) && (org_tx_en != enable))
            {
                rc = digi_otn_ohp_ch_cfg(digi_handle,
                                         odu_handle,
                                         oh_mask,
                                         oh_mask,
                                         enable);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                if (enable)
                {
                    if ((org_rx_mask != oh_mask) && org_rx_en)
                    {
                        rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                        odu_handle,
                                                        org_rx_mask,
                                                        FALSE);
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                        odu_handle,
                                                        oh_mask,
                                                        enable);
                        ADPT_RETURN_VERIFY(rc);
                    }

                    if ((org_tx_mask != oh_mask) && org_tx_en)
                    {
                        rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                         odu_handle,
                                                         org_tx_mask,
                                                         FALSE);
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                         odu_handle,
                                                         oh_mask,
                                                         enable);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
                else
                {
                    if (org_rx_en)
                    {
                        rc = digi_otn_ohp_ch_insert_cfg(digi_handle,
                                                        odu_handle,
                                                        org_rx_mask,
                                                        enable);
                        ADPT_RETURN_VERIFY(rc);
                    }

                    if (org_tx_en)
                    {
                        rc = digi_otn_ohp_ch_extract_cfg(digi_handle,
                                                         odu_handle,
                                                         org_tx_mask,
                                                         enable);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_ohp_cfg */


/*******************************************************************************
* adpt_otn_ohp_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the enable/disable status of the overhead extraction 
*   /insertion for a given channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   dir                - traffic direction: Tx, Rx, Rx_Tx.
*                        DIGI_OHP_DIR_RX: Insert overheads into OHP.
*                        DIGI_OHP_DIR_TX: Extract overheads from OHP.
*                        DIGI_OHP_DIR_RX_TX: Extract and insert overheads from OHP.
*
* OUTPUTS:
*   *oh_mask           - overhead mask.
*   *enable            - TRUE: enable the overhead insertion/extraction.
*                        FALSE: disable the overhead insertion/extraction.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   Please note the oh_mask comes from the Tx direction when the dir is used 
*   as DIGI_OHP_DIR_RX_TX. 
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ohp_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_loc_t location,
                                digi_ohp_dir_t dir,
                                UINT16 *oh_mask,
                                BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(oh_mask);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (dir)
    {
        case DIGI_OHP_DIR_RX:
            rc = digi_otn_ohp_ch_insert_cfg_get(digi_handle,
                                                odu_handle,
                                                oh_mask,
                                                enable);
            ADPT_RETURN_VERIFY(rc);
            break;

        case DIGI_OHP_DIR_TX:
            rc = digi_otn_ohp_ch_extract_cfg_get(digi_handle,
                                                 odu_handle,
                                                 oh_mask,
                                                 enable);
            ADPT_RETURN_VERIFY(rc);
            break;

        case DIGI_OHP_DIR_RX_TX:
            rc = digi_otn_ohp_ch_cfg_get(digi_handle,
                                         odu_handle,
                                         oh_mask,
                                         oh_mask,
                                         enable);
            ADPT_RETURN_VERIFY(rc);
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_ohp_get */


/*******************************************************************************
* adpt_otn_ohp_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function resizes overhead extraction/insertion for G.HAO. The datapath 
*   should be resized before calling this function when ramping down and OHP 
*   should be resized first when ramping up.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   resize             - number of time slots to resize: +ve increase, -ve decrease.
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
PUBLIC PMC_ERROR adpt_otn_ohp_resize(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_loc_t location,
                                  INT32 resize)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               location, resize);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ohp_ch_resize(digi_handle, odu_handle, resize);

    PMC_RETURN(rc);
} /* adpt_otn_ohp_resize */


/*******************************************************************************
* adpt_otn_ohp_chnl_num_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves internal channel number for OHP port usage.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *chnl_num          - channel number in one stage: 0 ~ 95.
*   *framer_num        - stage number.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_ohp_chnl_num_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_loc_t location,
                                        UINT32 *chnl_num,
                                        odu_ohp_port_framer_t *framer_num)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(chnl_num);
    ADPT_PARA_VERIFY(framer_num);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ohp_channel_framer_num_get(digi_handle, 
                                             odu_handle, 
                                             chnl_num,
                                             framer_num);

    PMC_RETURN(rc);
} /* adpt_otn_ohp_chnl_num_get */




/*******************************************************************************
* adpt_di_rcp_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs Defects Information Ring Control Port bring up of the
*   DIGI device. That includes the initialization of the:
*    - SGMII Ethernet subsystem
*    - 1GB serdes (D8)
*    - The Defects Information ODU Ring Control Port 
*
* INPUTS:
*   dev_id             - device identity.
*   *di_rcp_cfg        - DI_RCP overhead configuration.
*   enable             - TRUE: enable DI_RCP configuration.
*                        FALSE: disable DI_RCP configuration.
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
*   The feature can be only enabled under hardware dDEG mode through 
*   adpt_otn_deg_mode_set() API.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_di_rcp_en_set(UINT32 dev_id, 
                                 odu_di_rcp_cfg_t *di_rcp_cfg,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_di_rcp_cfg_t org_cfg;
    BOOL org_enable = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p][%d].\r\n", dev_id, di_rcp_cfg, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(di_rcp_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_di_rcp_en_get(dev_id, &org_cfg, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        if (enable)
        {
            rc = digi_di_rcp_init(adpt_handle->digi_handle, di_rcp_cfg);
        }
        else
        {
            rc = digi_di_rcp_uninit(adpt_handle->digi_handle);
        }
    }
    else if (enable)
    {
        if (0 != PMC_MEMCMP(&org_cfg, di_rcp_cfg, sizeof(odu_di_rcp_cfg_t)))
        {
            rc = digi_di_rcp_uninit(adpt_handle->digi_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_di_rcp_init(adpt_handle->digi_handle, di_rcp_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    /* record */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->di_rcp_en = enable;
    }

    PMC_RETURN(rc);
} /* adpt_di_rcp_en_set */


/*******************************************************************************
* adpt_di_rcp_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the enable/disable status of DI_RCP(Defects Information 
*   Ring Control Port). 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *di_rcp_cfg        - DI_RCP overhead configuration.
*   *enable            - TRUE: enable DI_RCP configuration.
*                        FALSE: disable DI_RCP configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API can be only available under hardware dDEG mode through 
*   adpt_otn_deg_mode_set() API.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_di_rcp_en_get(UINT32 dev_id,
                                 odu_di_rcp_cfg_t *di_rcp_cfg,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(di_rcp_cfg);
    ADPT_PARA_VERIFY(enable);
    
    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_di_rcp_init_get(adpt_handle->digi_handle, di_rcp_cfg, enable);

    PMC_RETURN(rc);
} /* adpt_di_rcp_en_get */


/*******************************************************************************
* adpt_di_rcp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Defect Information (DI) Ring Control Port (RCP) PDU transmission.
*
*   Function digi_di_rcp_sdh_counters_cfg should be called to configure SONET/SDH
*   B1, B2 and M0_M1 counts.
*
*   Bit mask:
*   DIGI_DI_RCP_CHNL_LINE_OTU_HO_ODUK                > Line Side OTU + Channel based HO ODUk                     
*   DIGI_DI_RCP_CHNL_SYS_OTU_HO_ODUK                 > System Side OTU + Channel based HO ODUk                   
*   DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STG1     > Channel based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)     
*   DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STGN     > Channel based HO ODUk Rx Framer Stage N (NEBC & FEBC)     
*   DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STG1  > Pool based HO ODUk Rx Framer Stage 1                      
*   DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STGN  > Pool based HO ODUk Rx Framer Stage N                      
*   DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STG1     > Pool based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)        
*   DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STGN     > Pool based HO ODUk Rx Framer Stage N (NEBC & FEBC)        
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG2     > Channel based ODUk Rx Framer Stage 2                      
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3A    > Channel based ODUk Rx Framer Stage 3a                     
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3B    > Channel based ODUk Rx Framer Stage 3b                     
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG4     > Channel based ODUk Rx Framer Stage 4                      
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG2        > Channel based ODUk Rx Framer Stage 2 (NEBC & FEBC)        
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3A       > Channel based ODUk Rx Framer Stage 3a (NEBC & FEBC)       
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3B       > Channel based ODUk Rx Framer Stage 3b (NEBC & FEBC)       
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG4        > Channel based ODUk Rx Framer Stage 4 (NEBC & FEBC)        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG2     > Pool based ODUk Rx Framer Stage 2                         
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3A    > Pool based ODUk Rx Framer Stage 3a                        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3B    > Pool based ODUk Rx Framer Stage 3b                        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG4     > Pool based ODUk Rx Framer Stage 4                         
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG2        > Pool based ODUk Rx Framer Stage 2 (NEBC & FEBC)           
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3A       > Pool based ODUk Rx Framer Stage 3a (NEBC & FEBC)          
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3B       > Pool based ODUk Rx Framer Stage 3b (NEBC & FEBC)          
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG4        > Pool based ODUk Rx Framer Stage 4 (NEBC & FEBC)           
*   DIGI_DI_RCP_GFPF                                 > GFP-F                                                     
*   DIGI_DI_RCP_CLIENT                               > Clients (ENET, SDH, FC1200, FC800, PRBS)                  
*   DIGI_DI_RCP_SDH                                  > SDH B1 & B2 & M0/M1                                       
*   DIGI_DI_RCP_PRBS_NTSE                            > PRBS nTSE                                                 
*   DIGI_DI_RCP_ALL                                  > All PMON and all defects
*
* INPUTS:
*   dev_id             - device identity.
*   di_rcp_pdu         - the DI RCP PDUs to enable or disable. Use 
*                        digi_di_rcp_pdu_t bit masks to define which PDUs 
*                        you want to enable by or-ing  masks. 
*   enable             - TRUE: enable transfer.
*                        FALSE: disable transfer.
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
PUBLIC PMC_ERROR adpt_di_rcp_cfg(UINT32 dev_id, UINT32 di_rcp_pdu, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, di_rcp_pdu, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_di_rcp_cfg(adpt_handle->digi_handle, di_rcp_pdu, enable);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_di_rcp_cfg */


/*******************************************************************************
* adpt_di_rcp_sdh_counts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Ring Control Port (RCP) SONET/SDH (B1, B2) counts in PDU transmission.
*
* INPUTS:
*   dev_id             - device identity.
*   cnt_sel            - SDH counts select mask for channels 0 to 11. 
*                        when channel bit is 0 -> ingress count is selected
*                        when channel bit is 1 -> egress count is selected
*                        sdh_cnt_sel is ignored when PDU transmission is disabled 
*                        (enable == FALSE).
*   sdh_cnt            - TRUE: enable SDH/SONET B1/B2 counts to be reported and disable
*                        SDH counts to RCP port.
*                        FALSE: disable SDH/SONET B1/B2 counts to be reported and enable
*                        SDH counts to RCP port.
*   enable             - TRUE: enable transfer.
*                        FALSE: disable transfer.
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
*   Once the sdh_cnt is enabled, both ingress and egress counts will be affected.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_di_rcp_sdh_counts_cfg(UINT32 dev_id, 
                                        UINT16 cnt_sel, 
                                        BOOL   sdh_cnt, 
                                        BOOL   enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, cnt_sel, sdh_cnt, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_di_rcp_sdh_counts_cfg(adpt_handle->digi_handle, 
                                    (BOOL8)enable, 
                                    cnt_sel,
                                    (BOOL8)sdh_cnt);

    PMC_RETURN(rc);
} /* adpt_di_rcp_sdh_counts_cfg */


/*******************************************************************************
* adpt_di_rcp_otn_counts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Ring Control Port (RCP) OTN (NEBC & FEBC) counts in PDU transmission.
*
* INPUTS:
*   dev_id             - device identity.
*   enable             - TRUE: disable transfer. Stop OTN counts being sent to RCP 
*                        port. Counts are available through PMON.
*                        FALSE: enable transfer.
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
PUBLIC PMC_ERROR adpt_di_rcp_otn_counts_cfg(UINT32 dev_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_di_rcp_otn_counts_cfg(adpt_handle->digi_handle, enable); 

    PMC_RETURN(rc);
} /* adpt_di_rcp_otn_counts_cfg */


/*******************************************************************************
* adpt_di_rcp_prbs_cycle_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Updates the PRBS collection mode settings. When DI_RCP is configured, it
*   automatically causes the PRBS monitor to accumulate error counts over 5ms. 
*   This API can be called to accumulate pN_TSE over a 1s period. 
*
* INPUTS:
*   dev_id             - device identity.
*   prbs_cycle         - 1: PRBS monitor to accumulate error counts over 5ms.
*                        0: PRBS monitor to accumulate pN_TSE counts over 1s.
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
PUBLIC PMC_ERROR adpt_di_rcp_prbs_cycle_set(UINT32 dev_id, UINT32 prbs_cycle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_pmon_prbs_collection_mode_cfg(adpt_handle->digi_handle, prbs_cycle);

    PMC_RETURN(rc);
} /* adpt_di_rcp_prbs_cycle_set */


/*******************************************************************************
* adpt_tcm_pool_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves TCM pool ID for DI_RCP usage. The TCM ID resource is limited within 
*   256 numbers at most in one stage which mode is MONITOR or OPERATIONAL. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only be available for TCM1 - TCM6 and
*                        PM NIM.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   position           - sink/source position.
*
* OUTPUTS:
*   *tcm_pool_id       - TCM pool ID. If TCM mode isn't OPERATIONAL or MONITOR,
*                        default value 0xFFFF will be returned.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_tcm_pool_id_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_pos_t position,
                                   UINT32 *tcm_pool_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT8 tcm_enable = DIGI_TCM_ENABLE;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(tcm_pool_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* parameter check */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id)) 
        || (DIGI_OTN_SEG_PM == seg_id)
        || (location >= LAST_DIGI_OTN_SIDE)
        || (position >= LAST_DIGI_OTN_POS))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
    tcm_enable = DIGI_TCM_EN_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+position][tcm_index]);

    if (DIGI_TCM_ENABLE == tcm_enable)
    {
        *tcm_pool_id = DIGI_TCM_ID_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+position][tcm_index]);
    }
    else
    {
        *tcm_pool_id = 0xFFFF;
    }

    PMC_RETURN(rc);
} /* adpt_tcm_pool_id_get */


/*******************************************************************************
* adpt_ri_rcp_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs Remote Information Ring Control Port bring up of the
*   DIGI device. That includes the initialization of the:
*    - SGMII Ethernet subsystem
*    - 1GB serdes (D8)
*    - The Remote Information ODU Ring Control Port 
*
*   This functions shall be called before enabling any insertion or extraction
*   from this port.
*
* INPUTS:
*   dev_id             - device identity.
*   eth_mac_oui        - Organizationally Unique  Identifier is inserted in the 3
*                        first bytes  of the source MAC.
*   eth_mac_id         - DIGI identifier is inserted in the fourth byte of the
*                        source MAC.
*   eth_etype          - The Ethertype field of the Ethernet frame. The value must
*                        be higher than 0x0800 and not conflict with any of the
*                        standardized Ethertype values. 
*   enable             - TRUE: enable OHP configuration.
*                        FALSE: disable OHP configuration.
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
PUBLIC PMC_ERROR adpt_ri_rcp_en_set(UINT32 dev_id, 
                                 UINT32 eth_mac_oui,
                                 UINT8 eth_mac_id,
                                 UINT32 eth_etype,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 org_mac_oui = 0;
    UINT8 org_mac_id = 0;
    UINT32 org_etype = 0;
    BOOL org_enable = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_odu_ri_rcp_cfg_t ri_rcp_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               eth_mac_oui, eth_mac_id, eth_etype, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_ri_rcp_en_get(dev_id, &org_mac_oui, &org_mac_id, &org_etype, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    ri_rcp_cfg.eth_mac_oui     = eth_mac_oui;
    ri_rcp_cfg.eth_etype       = eth_etype;
    ri_rcp_cfg.eth_mac_digi_id = eth_mac_id;

    if (org_enable != enable)
    {
        if (enable)
        {
            rc = digi_ri_rcp_init(adpt_handle->digi_handle, &ri_rcp_cfg);
        }
        else
        {
            rc = digi_ri_rcp_uninit(adpt_handle->digi_handle);
        }
    }
    else if (enable)
    {
        if ((org_mac_oui != eth_mac_oui) 
            || (org_mac_id != eth_mac_id)
            || (org_etype != eth_etype))
        {
            rc = digi_ri_rcp_uninit(adpt_handle->digi_handle);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_ri_rcp_init(adpt_handle->digi_handle, &ri_rcp_cfg);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    /* record */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->ri_rcp_en = enable;
    }

    PMC_RETURN(rc);
} /* adpt_ri_rcp_en_set */


/*******************************************************************************
* adpt_ri_rcp_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the enable/disable status of RI_RCP(Remote Information 
*   Ring Control Port). 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *eth_mac_oui       - Organizationally Unique  Identifier is inserted in the 3
*                        first bytes  of the source MAC.
*   *eth_mac_id        - DIGI identifier is inserted in the fourth byte of the
*                        source MAC.
*   *eth_etype         - The Ethertype field of the Ethernet frame. The value must
*                        be higher than 0x0800 and not conflict with any of the
*                        standardized Ethertype values. 
*   *enable            - TRUE: enable RI_RCP configuration.
*                        FALSE: disable RI_RCP configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API can be only available under hardware dDEG mode through 
*   adpt_otn_deg_mode_set() API.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ri_rcp_en_get(UINT32 dev_id, 
                                 UINT32 *eth_mac_oui,
                                 UINT8 *eth_mac_id,
                                 UINT32 *eth_etype,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_odu_ri_rcp_cfg_t ri_rcp_cfg;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);
    ADPT_PARA_VERIFY(eth_mac_oui);
    ADPT_PARA_VERIFY(eth_mac_id);
    ADPT_PARA_VERIFY(eth_etype);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_ri_rcp_init_get(adpt_handle->digi_handle, &ri_rcp_cfg, enable);
    *eth_mac_oui = ri_rcp_cfg.eth_mac_oui;
    *eth_etype   = ri_rcp_cfg.eth_etype;
    *eth_mac_id  = ri_rcp_cfg.eth_mac_digi_id;

    PMC_RETURN(rc);
} /* adpt_ri_rcp_en_get */


/*******************************************************************************
* adpt_ri_rcp_extract_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures RI_RCP extraction for an ODU framer channel. A RI_RCP PDU is 
*   assembled  with the following information for the specified channel:
*    - The BDI/BEI/BIAE bits from the TCMi and PM layers are extracted. 
*    - The four APS/PCC bytes from a single connection monitoring level are 
*      extracted (one of ODUk Path, ODUk TCM1-6, or ODUk server layer trail 
*      where the server layer trail is an OTUk or HO ODUk). 
*    - The Server Signal Fail (SSF) over the 8 ODUk frame window where the 
*      APS/PCC bytes are captured. 
*
*   Assembled RI_RCP DPU are sent to a the destination specified by a 
*   digi_otn_odu_ri_rcp_dst_cfg_t structure.  digi_otn_odu_ri_rcp_dest_get 
*   function can be used to retrieve a digi_otn_odu_ri_rcp_dst_cfg_t structure 
*   from a provisioned ODU framer channel on this DIGI or on a remote DIGI.
*   The APS/PCC connection monitoring level can updated dynamically. 
*   digi_otn_odu_ri_rcp_aps_pcc_cfg should be used to update this parameter.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   *ri_rcp_dst_cfg    - pointer to ODU RI_RCP destination configuration
*   enable             - TRUE: enable RI_RCP configuration.
*                        FALSE: disable RI_RCP configuration.
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
PUBLIC PMC_ERROR adpt_ri_rcp_extract_cfg(UINT32 dev_id, 
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 aps_pcc = 0;
    digi_otn_seg_t org_seg_id = DIGI_OTN_SEG_PM;
    digi_otn_odu_ri_rcp_dst_cfg_t org_cfg;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p][%d].\r\n", dev_id, 
               chnl_id, seg_id, ri_rcp_dst_cfg, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ri_rcp_dst_cfg);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_ri_rcp_extract_get(dev_id, chnl_id, &org_seg_id, &org_cfg, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    aps_pcc = (DIGI_OTN_SEG_SM == seg_id) ? 7 : (UINT8)seg_id;

    if (org_enable != enable)
    {
        rc = digi_otn_odu_ri_rcp_extract_cfg(digi_handle,
                                             odu_handle,
                                             ri_rcp_dst_cfg,
                                             aps_pcc,
                                             (BOOL8)enable);
    }
    else if (enable)
    {
        if (0 != PMC_MEMCMP(&org_cfg, ri_rcp_dst_cfg, sizeof(org_cfg)))
        {
            rc = digi_otn_odu_ri_rcp_extract_cfg(digi_handle,
                                                 odu_handle,
                                                 &org_cfg,
                                                 aps_pcc,
                                                 FALSE);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_otn_odu_ri_rcp_extract_cfg(digi_handle,
                                                 odu_handle,
                                                 ri_rcp_dst_cfg,
                                                 aps_pcc,
                                                 (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (seg_id != org_seg_id)
        {
            rc = adpt_ri_rcp_mon_level_cfg(dev_id, chnl_id, seg_id);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_ri_rcp_extract_cfg */


/*******************************************************************************
* adpt_ri_rcp_extract_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the RI_RCP extraction for a given ODU framer channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *seg_id            - segment identity.
*   *ri_rcp_dst_cfg    - pointer to ODU RI_RCP destination configuration
*   *enable            - TRUE: enable RI_RCP configuration.
*                        FALSE: disable RI_RCP configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ri_rcp_extract_get(UINT32 dev_id, 
                                    UINT32 chnl_id,
                                    digi_otn_seg_t *seg_id,
                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    BOOL8 ext_enable = FALSE;
    UINT8 aps_pcc = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(seg_id);
    ADPT_PARA_VERIFY(ri_rcp_dst_cfg);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_ri_rcp_extract_cfg_get(digi_handle,
                                             odu_handle,
                                             ri_rcp_dst_cfg,
                                             &aps_pcc,
                                             &ext_enable);
    *enable = ext_enable ? TRUE : FALSE;
    *seg_id = (7 == aps_pcc) ? DIGI_OTN_SEG_SM : (digi_otn_seg_t)aps_pcc;

    PMC_RETURN(rc);
} /* adpt_ri_rcp_extract_get */


/*******************************************************************************
* adpt_ri_rcp_mon_level_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the APS/PCC connection monitoring for an ODU framer channel. This 
*   function shall be called on a channel where the DI RCP extraction has been 
*   already enabled using adpt_otn_ri_rcp_extract_cfg. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
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
PUBLIC PMC_ERROR adpt_ri_rcp_mon_level_cfg(UINT32 dev_id, 
                                       UINT32 chnl_id,
                                       digi_otn_seg_t seg_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT8 aps_pcc = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, 
               chnl_id, seg_id);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    aps_pcc = (DIGI_OTN_SEG_SM == seg_id) ? 7 : (UINT8)seg_id;

    rc = digi_otn_odu_ri_rcp_aps_pcc_mon_level_cfg(digi_handle, odu_handle, aps_pcc);

    PMC_RETURN(rc);
} /* adpt_ri_rcp_mon_level_cfg */


/*******************************************************************************
* adpt_ri_rcp_dest_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides the RI_RCP destination configuration of a specified 
*   ODU framer channel
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *ri_rcp_dst_cfg    - pointer to ODU RI RCP destination configuration
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ri_rcp_dest_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_ri_rcp_dest_get(digi_handle, odu_handle, ri_rcp_dst_cfg);

    PMC_RETURN(rc);
} /* adpt_ri_rcp_dest_get */


/*******************************************************************************
* adpt_ri_rcp_aps_inst_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the APS/PCC bytes insertion for an ODU framer channel from the
*   RI RCP port. The insertion monitoring level is taken from the RI_RCP PDU
*   received on the port. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   inst_src           - TRUE: enable APS PCC insertion in RI_RCP.
*                        FALSE: disable APS PCC insertion in RI_RCP.
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
PUBLIC PMC_ERROR adpt_ri_rcp_aps_inst_cfg(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     BOOL inst_src)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    BOOL org_src = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, inst_src);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_ri_rcp_aps_inst_get(dev_id, chnl_id, &org_src); 
    ADPT_RETURN_VERIFY(rc);

    if (org_src != inst_src)
    {
        rc = digi_otn_odu_ri_rcp_aps_insert_cfg(digi_handle, odu_handle, (BOOL8)inst_src);
    }

    PMC_RETURN(rc);
} /* adpt_ri_rcp_aps_inst_cfg */


/*******************************************************************************
* adpt_ri_rcp_aps_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of the APS/PCC bytes insertion for an ODU framer channel 
*   from the RI RCP port. The insertion monitoring level is taken from the RI_RCP 
*   PDU received on the port. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *inst_src          - TRUE: enable APS PCC insertion in RI_RCP.
*                        FALSE: disable APS PCC insertion in RI_RCP.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ri_rcp_aps_inst_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     BOOL *inst_src)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    BOOL8 source = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(inst_src);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_ri_rcp_aps_insert_cfg_get(digi_handle, odu_handle, &source);
    *inst_src = source ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_ri_rcp_aps_inst_get */


/*******************************************************************************
* adpt_fege_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function brings up/down ENET FEGE block and its associated serdes.
*
* INPUTS:
*   dev_id             - device identity.
*   enable             - TRUE: enable FEGE MGMT configuration.
*                        FALSE: disable FEGE MGMT configuration.
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
PUBLIC PMC_ERROR adpt_fege_en_set(UINT32 dev_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_fege_en_get(dev_id, &org_enable); 
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        if (enable)
        {
            rc = digi_enet_mgmt_fege_init(adpt_handle->digi_handle);
        }
        else
        {
            rc = digi_enet_mgmt_fege_uninit(adpt_handle->digi_handle);
        }
    }

    PMC_RETURN(rc);
} /* adpt_fege_en_set */


/*******************************************************************************
* adpt_fege_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the enable/disable status of the ENET FEGE block.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable FEGE MGMT configuration.
*                        FALSE: disable FEGE MGMT configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fege_en_get(UINT32 dev_id, BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 fege_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_mgmt_fege_init_get(adpt_handle->digi_handle, &fege_en);
    *enable = fege_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_fege_en_get */


/*******************************************************************************
* adpt_fege_rx_fc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures DIGI MGMT_GE RX flow control. When flow control is enabled, MGMT_GE 
*   stops frames transmission when a PAUSE frame is received and it resumes the
*   transmission when pause quanta counter is expired.
*
* INPUTS:
*   dev_id             - device identity.
*   drop_pause_frm     - TRUE: RX pause frames are terminated in MGMT_FEGE
*                        FALSE: RX pause frames are forwarded.  
*   enable             - TRUE: Flow control is activated.
*                        FALSE: Flow control is deactivated.
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
PUBLIC PMC_ERROR adpt_fege_rx_fc_set(UINT32 dev_id, 
                                 BOOL drop_pause_frm,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, drop_pause_frm, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    digi_mgmt_fege_rx_flow_control_set(adpt_handle->digi_handle,
                                       (BOOL8)enable,
                                       (BOOL8)drop_pause_frm);

    PMC_RETURN(rc);
} /* adpt_fege_rx_fc_set */


/*******************************************************************************
* adpt_fege_rx_fc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DIGI MGMT_FEGE RX flow control status.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *drop_pause_frm    - TRUE: RX pause frames are terminated in MGMT_FEGE
*                        FALSE: RX pause frames are forwarded.  
*   *enable            - TRUE: Flow control is activated.
*                        FALSE: Flow control is deactivated.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fege_rx_fc_get(UINT32 dev_id, 
                                 BOOL *drop_pause_frm,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 fc_enable = FALSE;
    BOOL8 fc_drop_pause_frm = FALSE;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    digi_mgmt_fege_rx_flow_control_get(adpt_handle->digi_handle,
                                       &fc_enable,
                                       &fc_drop_pause_frm);
    *enable = fc_enable ? TRUE : FALSE;
    *drop_pause_frm = fc_drop_pause_frm ? TRUE : FALSE;
    
    PMC_RETURN(rc);
} /* adpt_fege_rx_fc_get */


/*******************************************************************************
* adpt_fege_frm_size_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures MGMT_GE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   dev_id             - device identity.
*   txMaxFrameSize     - TX frames maximum size: 0 ~ 0xFFFB
*   txMinFrameSize     - TX frames minimum size: 0 ~ 0x3F
*   rxMaxFrameSize     - RX frames maximum size: 0 ~ 0xFFFB
*   rxMinFrameSize     - RX frames minimum size: 0 ~ 0x3F
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
PUBLIC PMC_ERROR adpt_fege_frm_size_set(UINT32 dev_id, 
                                    UINT16 txMaxFrameSize,
                                    UINT16 txMinFrameSize,
                                    UINT16 rxMaxFrameSize,
                                    UINT16 rxMinFrameSize)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, txMaxFrameSize, 
               txMinFrameSize, rxMaxFrameSize, rxMinFrameSize);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mgmt_fege_frame_sizes_set(adpt_handle->digi_handle,
                                        txMaxFrameSize,
                                        txMinFrameSize,
                                        rxMaxFrameSize,
                                        rxMinFrameSize);

    PMC_RETURN(rc);
} /* adpt_fege_frm_size_set */


/*******************************************************************************
* adpt_fege_frm_size_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves MGMT_FEGE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *txMaxFrameSize    - TX frames maximum size
*   *txMinFrameSize    - TX frames minimum size
*   *rxMaxFrameSize    - RX frames maximum size
*   *rxMinFrameSize    - RX frames minimum size
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fege_frm_size_get(UINT32 dev_id, 
                                    UINT16 *txMaxFrameSize,
                                    UINT16 *txMinFrameSize,
                                    UINT16 *rxMaxFrameSize,
                                    UINT16 *rxMinFrameSize)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    digi_mgmt_fege_frame_sizes_get(adpt_handle->digi_handle,
                                   txMaxFrameSize,
                                   txMinFrameSize,
                                   rxMaxFrameSize,
                                   rxMinFrameSize);

    PMC_RETURN(rc);
} /* adpt_fege_frm_size_get */


/*
** End of file
*/
