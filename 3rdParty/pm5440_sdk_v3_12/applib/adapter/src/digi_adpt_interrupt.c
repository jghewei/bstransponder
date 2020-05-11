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
*     The file describes the interrupt functionality.
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
* adpt_dev_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the propagation of one or more interrupts to the 
*   device pin or PCIE interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   int_type           - interrupt aggregator type. For device pin interrupt, 
*                        sellect CPUIF_INT_DEVICE.
*   int_msk            - determines which interrupts to enable. See 
*                        digi_int_irq_mask_t for an enumeration of the interrupt 
*                        bit values.
*                           DIGI_INT_IRQ_MASK_PMON          
*                           DIGI_INT_IRQ_MASK_TOP_SYSOTN    
*                           DIGI_INT_IRQ_MASK_MCPB          
*                           DIGI_INT_IRQ_MASK_DCPB          
*                           DIGI_INT_IRQ_MASK_SIFD          
*                           DIGI_INT_IRQ_MASK_CBRC          
*                           DIGI_INT_IRQ_MASK_ENET_SYS      
*                           DIGI_INT_IRQ_MASK_ENET_LINE     
*                           DIGI_INT_IRQ_MASK_MAPOTN        
*                           DIGI_INT_IRQ_MASK_LINEOTN       
*                           DIGI_INT_IRQ_MASK_COREOTN
*   enable             - TRUE: enable interrupt based on int_msk.
*                        FALSE: disable interrupt based on int_msk.
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
PUBLIC PMC_ERROR adpt_dev_int_en_set(UINT32 dev_id,
                                  cpuif_int_t int_type,
                                  UINT32 int_msk,
                                  BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (enable)
    {
        digi_int_device_aggregator_enable(adpt_handle->digi_handle,
                                          int_type,
                                          int_msk);
    }
    else
    {
        digi_int_device_aggregator_disable(adpt_handle->digi_handle,
                                           int_type,
                                           int_msk);
    }

    PMC_RETURN(rc);
} /* adpt_dev_int_en_set */


/*******************************************************************************
* adpt_dev_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of one or more interrupts to the 
*   device pin or PCIE interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   int_type           - interrupt aggregator type. For device pin interrupt, 
*                        sellect CPUIF_INT_DEVICE.
*
* OUTPUTS:
*   *int_en_msk        - retrieves which interrupts to enable. See 
*                        digi_int_irq_mask_t for an enumeration of the interrupt 
*                        bit values.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dev_int_en_get(UINT32 dev_id,
                                  cpuif_int_t int_type,
                                  UINT32 *int_en_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_en_msk);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    *int_en_msk = digi_int_device_aggregator_get(adpt_handle->digi_handle, int_type);

    PMC_RETURN(rc);
} /* adpt_dev_int_en_get */


/*******************************************************************************
* adpt_enet_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the Ethernet interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   *int_table         - Ethernet defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_enet_int_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 client_mode = 0;
    enet_int_chnl_t enet_int_table;
    UINT32 lane_id = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 enet_chnl = 0;
    UINT32 src_index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if ((traffic_loc >= DIGI_SIDE_MAX) || (dir >= LAST_DIGI_PORT_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    /* reset */
    PMC_MEMSET((void*)&enet_int_table, 0, sizeof(enet_int_table));
    /* enable interrupt */
    enet_int_table.emac.phy_los_i           = int_table->phy_los;
    enet_int_table.emac.rx_loc_fault_i      = int_table->rx_loc_fault;
    enet_int_table.emac.rx_rem_fault_i      = int_table->rx_rem_fault;
    enet_int_table.emac.hi_ber_i            = int_table->hi_ber;
    enet_int_table.emac.block_lock_i        = int_table->block_lock;
    enet_int_table.emac.align_status_i      = int_table->align_status;
    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_MAC:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            for (lane_id = 0; lane_id < EXCO4M_N_LANES; lane_id++)
            {
                enet_int_table.etrans.exco4m.rx_loam_i[lane_id] = int_table->loam;
            }
            break;

        case DIGI_ENET_SERDES_40GE_MAC:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            for (lane_id = 0; lane_id < EXCO3M_N_LANES; lane_id++)
            {
                enet_int_table.etrans.exco3m.rx_loam_i[lane_id] = int_table->loam;
            }
            break;

        default:
            break;
    }

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            switch (adpt_handle->traffic_info[index].mapping_mode)
            {
                case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
                case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
                case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
                case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
                case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
                case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
                    rc = digi_mapper_enet_etrans_int_chnl_enable(adpt_handle->digi_handle,
                                                                 adpt_handle->traffic_info[index].map_handle,
                                                                 &enet_int_table);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL != adpt_handle->traffic_info[index].enet_handle)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                rc = digi_enet_line_int_chnl_enable(adpt_handle->digi_handle, 
                                                    adpt_handle->traffic_info[index].enet_handle,
                                                    &enet_int_table);
            }
            else
            {
                rc = digi_enet_sys_int_chnl_enable(adpt_handle->digi_handle, 
                                                   adpt_handle->traffic_info[index].enet_handle,
                                                   &enet_int_table);
            }
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                rc = digi_mapper_enet_pmon_int_chnl_enable(adpt_handle->digi_handle,
                                                           adpt_handle->traffic_info[index].map_handle,
                                                           &enet_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    rc = digi_sifd_enet_pmon_int_chnl_enable(adpt_handle->digi_handle,
                                                             adpt_handle->traffic_info[index].ilkn_handle,
                                                             &enet_int_table);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
        }
    }

    /* disable interrupt */
    enet_int_table.emac.phy_los_i           = !(int_table->phy_los);
    enet_int_table.emac.rx_loc_fault_i      = !(int_table->rx_loc_fault);
    enet_int_table.emac.rx_rem_fault_i      = !(int_table->rx_rem_fault);
    enet_int_table.emac.hi_ber_i            = !(int_table->hi_ber);
    enet_int_table.emac.block_lock_i        = !(int_table->block_lock);
    enet_int_table.emac.align_status_i      = !(int_table->align_status);
    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_MAC:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            for (lane_id = 0; lane_id < EXCO4M_N_LANES; lane_id++)
            {
                enet_int_table.etrans.exco4m.rx_loam_i[lane_id] = !(int_table->loam);
            }
            break;

        case DIGI_ENET_SERDES_40GE_MAC:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            for (lane_id = 0; lane_id < EXCO3M_N_LANES; lane_id++)
            {
                enet_int_table.etrans.exco3m.rx_loam_i[lane_id] = !(int_table->loam);
            }
            break;

        default:
            break;
    }

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            switch (adpt_handle->traffic_info[index].mapping_mode)
            {
                case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
                case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
                case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
                case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
                case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
                case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
                    rc = digi_mapper_enet_etrans_int_chnl_disable(adpt_handle->digi_handle,
                                                                  adpt_handle->traffic_info[index].map_handle,
                                                                  &enet_int_table);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL != adpt_handle->traffic_info[index].enet_handle)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                rc = digi_enet_line_int_chnl_disable(adpt_handle->digi_handle, 
                                                     adpt_handle->traffic_info[index].enet_handle,
                                                     &enet_int_table);
            }
            else
            {
                rc = digi_enet_sys_int_chnl_disable(adpt_handle->digi_handle, 
                                                    adpt_handle->traffic_info[index].enet_handle,
                                                    &enet_int_table);
            }
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                rc = digi_mapper_enet_pmon_int_chnl_disable(adpt_handle->digi_handle,
                                                            adpt_handle->traffic_info[index].map_handle,
                                                            &enet_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    rc = digi_sifd_enet_pmon_int_chnl_disable(adpt_handle->digi_handle,
                                                              adpt_handle->traffic_info[index].ilkn_handle,
                                                              &enet_int_table);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_enet_int_en_set */


/*******************************************************************************
* adpt_enet_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of Ethernet interrupt based on channel 
*   level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *int_table         - Ethernet defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_int_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_enet_int_t enet_int_table;
    UINT32 client_mode = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 src_index = 0;
    digi_mapper_int_t *mapper_int_table= NULL;
    enet_int_chnl_t enet_int;
    digi_sifd_int_t sifd_int;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    /* reset */
    PMC_MEMSET(int_table, 0, sizeof(digi_enet_int_en_t));
    PMC_MEMSET((void*)&enet_int, 0, sizeof(enet_int_chnl_t));

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            switch (adpt_handle->traffic_info[index].mapping_mode)
            {
                case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
                case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
                case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
                case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
                case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
                case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
                    /* allocate memory */
                    mapper_int_table = (digi_mapper_int_t*)PMC_CALLOC(sizeof(digi_mapper_int_t));
                    ADPT_PARA_VERIFY(mapper_int_table);

                    /* get interrupt information */
                    rc = digi_mapper_int_enabled_get(adpt_handle->digi_handle, mapper_int_table);

                    /* memory copy */
                    PMC_MEMCPY((void*)&enet_int, &mapper_int_table->mapper_chnl[gfp_chnl].enet_etrans, sizeof(enet_int_chnl_t));
                    
                    /* memory free */
                    PMC_FREE(&mapper_int_table);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL != adpt_handle->traffic_info[index].enet_handle)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                rc = digi_enet_line_int_enabled_get(adpt_handle->digi_handle, &enet_int_table);
            }
            else
            {
                rc = digi_enet_sys_int_enabled_get(adpt_handle->digi_handle, &enet_int_table);
            }
            ADPT_RETURN_VERIFY(rc);

            /* memory copy */
            PMC_MEMCPY((void*)&enet_int, &enet_int_table.enet_chnl[enet_chnl].enet, sizeof(enet_int_chnl_t));
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                /* allocate memory */
                mapper_int_table = (digi_mapper_int_t*)PMC_CALLOC(sizeof(digi_mapper_int_t));
                ADPT_PARA_VERIFY(mapper_int_table);

                /* get interrupt information */
                rc = digi_mapper_int_enabled_get(adpt_handle->digi_handle, mapper_int_table);

                /* memory copy */
                PMC_MEMCPY((void*)&enet_int, &mapper_int_table->mapper_chnl[gfp_chnl].enet_pmon, sizeof(enet_int_chnl_t));

                /* memory free */
                PMC_FREE(&mapper_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    rc = digi_sifd_int_enabled_get(adpt_handle->digi_handle, &sifd_int);
                    ADPT_RETURN_VERIFY(rc);

                    /* memory copy */
                    PMC_MEMCPY((void*)&enet_int, &sifd_int.enet_pmon_int_chnl[enet_chnl].enet_pmon, sizeof(enet_int_chnl_t));
                }
            }
        }
    }

    int_table->phy_los      = enet_int.emac.phy_los_i;
    int_table->rx_loc_fault = enet_int.emac.rx_loc_fault_i;
    int_table->rx_rem_fault = enet_int.emac.rx_rem_fault_i;
    int_table->hi_ber       = enet_int.emac.hi_ber_i;     
    int_table->block_lock   = enet_int.emac.block_lock_i;  
    int_table->align_status = enet_int.emac.align_status_i;
    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_MAC:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            int_table->loam = enet_int.etrans.exco4m.rx_loam_i[0];
            break;

        case DIGI_ENET_SERDES_40GE_MAC:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            int_table->loam = enet_int.etrans.exco3m.rx_loam_i[0];
            break;

        default:
            int_table->loam = FALSE;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_int_en_get */


/*******************************************************************************
* adpt_otn_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the OTN interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. 
*                        Available for PM(including SM) and TCMi
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not available for stage 4.
*   *int_table         - OTN defect interrupt data structure.
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
*   1. Due to the limitation of the DIGI device, the PM and TCM interrupts of 
*      the RFRM block are excluded from this API. Please refer to the  
*      adpt_otn_seg_int_en_set() and adpt_otn_defect_int_en_set() APIs.
*   2. The dLOOMFI and dOTU-AIS interrupt are only valid in ODU4(OTU4).
*   3. The dCSF interrupt is only valid on Stage 4.
*   
*   Support the following interrupts.
*   SM:   OTUk     - dLOM OOM_IM aAIS dBIAE aBDI dIAE dTIM dDEG dEDEG dBDI aTSF 
*                    aTSD dLOF OOF dLOL
*         ODUj/[i] - None
*   TCMi: dEDEG
*   PM:   dMSIM dPLM dEDEG dLOOMFI dCSF
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    coreotn_int_framer_chnl_t odu_int_table;
    odu_rfrm_int_pid_chnl_t tcm_int_table;
    lineotn_int_chnl_t otu_int_table;
    UINT32 tcm_index = 0; 
    UINT16 tcm_pool_id = 0;
    UINT32 traffic_loc = 0;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    mapotn_int_chnl_t map_int_table;
    coreotn_int_t coreotn_int_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);
    if (((DIGI_OTN_SEG_SM != seg_id) && (DIGI_OTN_SEG_PM_NIM != seg_id) 
        && (seg_id > DIGI_OTN_SEG_TCM6)) || (location >= LAST_DIGI_OTN_SIDE))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* reset */
    PMC_MEMSET((void*)&odu_int_table, 0, sizeof(odu_int_table));
    PMC_MEMSET((void*)&otu_int_table, 0, sizeof(otu_int_table));
    PMC_MEMSET((void*)&tcm_int_table, 0, sizeof(tcm_int_table));
    PMC_MEMSET((void*)&map_int_table, 0, sizeof(map_int_table));
    PMC_MEMSET((void*)&coreotn_int_table, 0, sizeof(coreotn_int_table));

    if (DIGI_OTN_SEG_SM == seg_id)
    {
        if (NULL != adpt_handle->traffic_info[index].otu_handle)
        {
            /* enable interrupt */
            otu_int_table.otu_frm.otu_frm_d_lom_i    = int_table->dlom;
            otu_int_table.otu_frm.otu_frm_oom_i      = int_table->oom_im;
            otu_int_table.otu_frm.otu_sk_a_ais_i     = int_table->aais; 
            otu_int_table.otu_frm.otu_sk_a_bdi_i     = int_table->abdi; 
            otu_int_table.otu_frm.otu_sk_d_iae_i     = int_table->diae; 
            otu_int_table.otu_frm.otu_sk_d_tim_i     = int_table->dtim; 
            otu_int_table.otu_frm.otu_sk_d_deg_i     = int_table->ddeg; 
            otu_int_table.otu_frm.otu_sk_d_deg_enh_i = int_table->dedeg; 
            otu_int_table.otu_frm.otu_sk_d_bdi_i     = int_table->dbdi; 
            otu_int_table.otu_frm.otu_sk_d_biae_i    = int_table->dbiae;
            otu_int_table.otu_frm.otu_sk_a_tsf_i     = int_table->atsf;
            otu_int_table.otu_frm.otu_sk_a_tsd_i     = int_table->atsd;
            if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
            {
                otu_int_table.opsm4_rx_otu_lof_int_i  = int_table->dlof;
                otu_int_table.opsm4_rx_otu_oof_int_i  = int_table->oof;
                otu_int_table.opsm4_rx_lol_int_i      = int_table->dlol;
            }
            else
            {
                otu_int_table.opsm23_rx_otu_lof_int_i = int_table->dlof;
                otu_int_table.opsm23_rx_otu_oof_int_i = int_table->oof;
                otu_int_table.opsm23_rx_lol_int_i     = int_table->dlol;
            }

            if (DIGI_LINE_SIDE == traffic_loc)
            {
                rc = digi_otn_server_int_chnl_enable(adpt_handle->digi_handle,
                                                     adpt_handle->traffic_info[index].otu_handle,
                                                     &otu_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                rc = digi_sysotn_server_int_chnl_enable(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].otu_handle,
                                                        &otu_int_table);
                ADPT_RETURN_VERIFY(rc);
            }

            /* disable interrupt */
            otu_int_table.otu_frm.otu_frm_d_lom_i    = !(int_table->dlom);
            otu_int_table.otu_frm.otu_frm_oom_i      = !(int_table->oom_im);
            otu_int_table.otu_frm.otu_sk_a_ais_i     = !(int_table->aais); 
            otu_int_table.otu_frm.otu_sk_a_bdi_i     = !(int_table->abdi); 
            otu_int_table.otu_frm.otu_sk_d_iae_i     = !(int_table->diae); 
            otu_int_table.otu_frm.otu_sk_d_tim_i     = !(int_table->dtim); 
            otu_int_table.otu_frm.otu_sk_d_deg_i     = !(int_table->ddeg); 
            otu_int_table.otu_frm.otu_sk_d_deg_enh_i = !(int_table->dedeg); 
            otu_int_table.otu_frm.otu_sk_d_bdi_i     = !(int_table->dbdi); 
            otu_int_table.otu_frm.otu_sk_d_biae_i    = !(int_table->dbiae);
            otu_int_table.otu_frm.otu_sk_a_tsf_i     = !(int_table->atsf);
            otu_int_table.otu_frm.otu_sk_a_tsd_i     = !(int_table->atsd);
            if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
            {
                otu_int_table.opsm4_rx_otu_lof_int_i  = !(int_table->dlof);
                otu_int_table.opsm4_rx_otu_oof_int_i  = !(int_table->oof);
                otu_int_table.opsm4_rx_lol_int_i      = !(int_table->dlol);
            }
            else
            {
                otu_int_table.opsm23_rx_otu_lof_int_i = !(int_table->dlof);
                otu_int_table.opsm23_rx_otu_oof_int_i = !(int_table->oof);
                otu_int_table.opsm23_rx_lol_int_i     = !(int_table->dlol);
            }

            if (DIGI_LINE_SIDE == traffic_loc)
            {
                rc = digi_otn_server_int_chnl_disable(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[index].otu_handle,
                                                      &otu_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                rc = digi_sysotn_server_int_chnl_disable(adpt_handle->digi_handle,
                                                         adpt_handle->traffic_info[index].otu_handle,
                                                         &otu_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
        }
        else
        {
            /* enable interrupt */
            odu_int_table.rfrm.oof_i         = int_table->oof;
            odu_int_table.rfrm.dloflom_i     = int_table->dloflom;

            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                if (DIGI_OTN_LINE_SIDE == location)
                {
                    rc = digi_otn_odu_int_framer_chnl_enable(adpt_handle->digi_handle, 
                                                             adpt_handle->traffic_info[index].odu_handle,
                                                             &odu_int_table);
                }
                else
                {
                    rc = digi_otn_odu_int_stg3b_chnl_enable(adpt_handle->digi_handle, 
                                                            adpt_handle->traffic_info[index].odu_handle,
                                                            &odu_int_table);
                }
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                rc = digi_mapper_int_fo2_chnl_enable(adpt_handle->digi_handle,
                                                     adpt_handle->traffic_info[index].map_handle,
                                                     &odu_int_table);    
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }
            ADPT_RETURN_VERIFY(rc);

            /* disable interrupt */
            odu_int_table.rfrm.oof_i         = !(int_table->oof);
            odu_int_table.rfrm.dloflom_i     = !(int_table->dloflom);

            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                if (DIGI_OTN_LINE_SIDE == location)
                {
                    rc = digi_otn_odu_int_framer_chnl_disable(adpt_handle->digi_handle, 
                                                              adpt_handle->traffic_info[index].odu_handle,
                                                              &odu_int_table);
                }
                else
                {
                    rc = digi_otn_odu_int_stg3b_chnl_disable(adpt_handle->digi_handle, 
                                                             adpt_handle->traffic_info[index].odu_handle,
                                                             &odu_int_table);
                }
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                rc = digi_mapper_int_fo2_chnl_disable(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[index].map_handle,
                                                      &odu_int_table);    
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }
        }
    }    
    else if (DIGI_OTN_SEG_PM == seg_id)
    {
        /* enable interrupt */
        odu_int_table.rfrm.ac_aps_i      = int_table->ac_aps;
        odu_int_table.rfrm.ddeg_enh_pm_i = int_table->dedeg;
        odu_int_table.odtu_dmx.dmsim_i   = int_table->dmsim;
        odu_int_table.odtu_dmx.dplm_i    = int_table->dplm;

        if (NULL != adpt_handle->traffic_info[index].odu_handle)
        {
            if (DIGI_OTN_LINE_SIDE == location)
            {
                rc = digi_otn_odu_int_framer_chnl_enable(adpt_handle->digi_handle, 
                                                         adpt_handle->traffic_info[index].odu_handle,
                                                         &odu_int_table);
            }
            else
            {
                rc = digi_otn_odu_int_stg3b_chnl_enable(adpt_handle->digi_handle, 
                                                        adpt_handle->traffic_info[index].odu_handle,
                                                        &odu_int_table);
            }

            if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
            {
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    coreotn_int_table.fmf1_dmx.dloomfi_i = int_table->dloomfi;
                }
                else
                {
                    coreotn_int_table.fmf2_dmx.dloomfi_i = int_table->dloomfi;
                }

                rc = digi_otn_int_coreotn_enable(adpt_handle->digi_handle,
                                                 &coreotn_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
        }
        else if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            rc = digi_mapper_int_fo2_chnl_enable(adpt_handle->digi_handle,
                                                 adpt_handle->traffic_info[index].map_handle,
                                                 &odu_int_table);    

            ADPT_RETURN_VERIFY(rc);

            map_int_table.rgmp_valid  = TRUE;
            map_int_table.opu_csf_i   = int_table->dcsf;
            map_int_table.rgmp.csf_i  = int_table->dcsf;
            map_int_table.dplm_i      = int_table->dplm;
            map_int_table.rgmp.dplm_i = int_table->dplm;
            rc = digi_mapper_int_chnl_enable(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].map_handle,
                                             &map_int_table);
        }
        else
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
        ADPT_RETURN_VERIFY(rc);

        /* disable interrupt */
        odu_int_table.rfrm.ac_aps_i      = !(int_table->ac_aps);
        odu_int_table.rfrm.ddeg_enh_pm_i = !(int_table->dedeg);
        odu_int_table.odtu_dmx.dmsim_i   = !(int_table->dmsim);
        odu_int_table.odtu_dmx.dplm_i    = !(int_table->dplm);

        if (NULL != adpt_handle->traffic_info[index].odu_handle)
        {
            if (DIGI_OTN_LINE_SIDE == location)
            {
                rc = digi_otn_odu_int_framer_chnl_disable(adpt_handle->digi_handle, 
                                                          adpt_handle->traffic_info[index].odu_handle,
                                                          &odu_int_table);
            }
            else
            {
                rc = digi_otn_odu_int_stg3b_chnl_disable(adpt_handle->digi_handle, 
                                                         adpt_handle->traffic_info[index].odu_handle,
                                                         &odu_int_table);
            }

            if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
            {
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    coreotn_int_table.fmf1_dmx.dloomfi_i = !(int_table->dloomfi);
                }
                else
                {
                    coreotn_int_table.fmf2_dmx.dloomfi_i = !(int_table->dloomfi);
                }

                rc = digi_otn_int_coreotn_disable(adpt_handle->digi_handle,
                                                  &coreotn_int_table);
                ADPT_RETURN_VERIFY(rc);
            }
        }
        else if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            rc = digi_mapper_int_fo2_chnl_disable(adpt_handle->digi_handle,
                                                  adpt_handle->traffic_info[index].map_handle,
                                                  &odu_int_table);    
            ADPT_RETURN_VERIFY(rc);

            map_int_table.rgmp_valid  = TRUE;
            map_int_table.opu_csf_i   = !(int_table->dcsf);
            map_int_table.rgmp.csf_i  = !(int_table->dcsf);
            map_int_table.dplm_i      = !(int_table->dplm);
            map_int_table.rgmp.dplm_i = !(int_table->dplm);
            rc = digi_mapper_int_chnl_disable(adpt_handle->digi_handle,
                                              adpt_handle->traffic_info[index].map_handle,
                                              &map_int_table);
        }
        else
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        /* check TCM pool ID */
        tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
        tcm_pool_id = adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index];
        if (DIGI_TCM_EN_GET(tcm_pool_id) != DIGI_TCM_ENABLE)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        /* handle process */
        rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
        ADPT_RETURN_VERIFY(rc);

        /* enable interrupt */
        tcm_int_table.ddeg_enh_i = int_table->dedeg;
        rc = digi_otn_odu_int_pid_chnl_enable(digi_handle,
                                              odu_handle,
                                              (digi_otn_int_tcm_num_t)(seg_id - 1),
                                              &tcm_int_table);
        ADPT_RETURN_VERIFY(rc);

        /* disable interrupt */
        tcm_int_table.ddeg_enh_i = !(int_table->dedeg);
        rc = digi_otn_odu_int_pid_chnl_disable(digi_handle,
                                               odu_handle,
                                               (digi_otn_int_tcm_num_t)(seg_id - 1),
                                               &tcm_int_table);
    }

    PMC_RETURN(rc);
} /* adpt_otn_int_en_set */


/*******************************************************************************
* adpt_otn_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of the OTN interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. 
*                        Available for PM(including SM) and TCMi
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not available for stage 4.
*
* OUTPUTS:
*   *int_table         - OTN defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 otu_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_int_t *otn_int_table = NULL;
    digi_mapper_int_t *mapper_int_table= NULL;
    coreotn_int_framer_chnl_t *odu_int_table = NULL;
    odu_rfrm_int_pid_chnl_t *tcm_int_table = NULL;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    UINT32 traffic_loc = 0;
    lineotn_int_chnl_t *otu_int_table = NULL;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);
    if ((DIGI_OTN_SEG_SM != seg_id) 
        && (DIGI_OTN_SEG_PM_NIM != seg_id) 
        && (seg_id > DIGI_OTN_SEG_TCM6))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* allocate memory */
    otn_int_table = (digi_otn_int_t*)PMC_CALLOC(sizeof(digi_otn_int_t));
    mapper_int_table = (digi_mapper_int_t*)PMC_CALLOC(sizeof(digi_mapper_int_t));
    ADPT_PARA_VERIFY(otn_int_table);
    ADPT_PARA_VERIFY(mapper_int_table);

    /* reset */
    PMC_MEMSET(int_table, 0, sizeof(digi_otn_int_en_t));

    rc = digi_otn_int_enabled_get(adpt_handle->digi_handle, otn_int_table);
    if (PMC_SUCCESS != rc)
    {
        PMC_FREE(&otn_int_table);
        PMC_FREE(&mapper_int_table);
        PMC_RETURN(rc);
    }

    if ((DIGI_OTN_SEG_SM == seg_id) 
        && (NULL != adpt_handle->traffic_info[index].otu_handle))
    {
        /* get OTU channel */
        rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
        if (PMC_SUCCESS != rc)
        {
            PMC_FREE(&otn_int_table);
            PMC_FREE(&mapper_int_table);
            PMC_RETURN(rc);
        }

        if (DIGI_LINE_SIDE == traffic_loc)
        {
            otu_int_table = &(otn_int_table->otn_server_chnl[otu_chnl]);
        }
        else
        {
            otu_int_table = &(otn_int_table->sysotn_server_chnl[otu_chnl]);
        }

        int_table->dlom   = otu_int_table->otu_frm.otu_frm_d_lom_i;
        int_table->oom_im = otu_int_table->otu_frm.otu_frm_oom_i;
        int_table->aais   = otu_int_table->otu_frm.otu_sk_a_ais_i;
        int_table->abdi   = otu_int_table->otu_frm.otu_sk_a_bdi_i;
        int_table->diae   = otu_int_table->otu_frm.otu_sk_d_iae_i;
        int_table->dtim   = otu_int_table->otu_frm.otu_sk_d_tim_i;
        int_table->ddeg   = otu_int_table->otu_frm.otu_sk_d_deg_i;
        int_table->dedeg  = otu_int_table->otu_frm.otu_sk_d_deg_enh_i;
        int_table->dbdi   = otu_int_table->otu_frm.otu_sk_d_bdi_i;
        int_table->dbiae  = otu_int_table->otu_frm.otu_sk_d_biae_i;
        int_table->atsf   = otu_int_table->otu_frm.otu_sk_a_tsf_i;
        int_table->atsd   = otu_int_table->otu_frm.otu_sk_a_tsd_i;
        if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
        {
            int_table->dlof    = otu_int_table->opsm4_rx_otu_lof_int_i;
            int_table->oof     = otu_int_table->opsm4_rx_otu_oof_int_i;
            int_table->dlol    = otu_int_table->opsm4_rx_lol_int_i;
        }
        else
        {
            int_table->dlof = otu_int_table->opsm23_rx_otu_lof_int_i;
            int_table->oof  = otu_int_table->opsm23_rx_otu_oof_int_i;
            int_table->dlol = otu_int_table->opsm23_rx_lol_int_i;
        }
    }
    else
    {
        if (NULL != adpt_handle->traffic_info[index].odu_handle)
        {
            /* get odu channel ID and odu stage number */
            rc |= adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
            if (PMC_SUCCESS != rc)
            {
                PMC_FREE(&otn_int_table);
                PMC_FREE(&mapper_int_table);
                PMC_RETURN(rc);
            }

            tcm_index = (DIGI_OTN_SEG_PM == seg_id) ? 0 : (seg_id - 1);
            switch (odu_stage)
            {
                case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                    if ((DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
                         && (DIGI_LINE_SIDE == traffic_loc))
                    {
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            odu_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                            tcm_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[tcm_index]);
                        }
                        else
                        {
                            odu_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                            tcm_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[tcm_index]);
                        }
                    }
                    else
                    {
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            odu_int_table = &(otn_int_table->otn_odu_ho_chnl[odu_chnl].status.framer_chnl);
                            tcm_int_table = &(otn_int_table->otn_odu_ho_chnl[odu_chnl].status.pid.rx_tcm[tcm_index]);
                        }
                        else
                        {
                            odu_int_table = &(otn_int_table->otn_odu_ho_chnl[odu_chnl].stg3b.framer_chnl);
                            tcm_int_table = &(otn_int_table->otn_odu_ho_chnl[odu_chnl].stg3b.pid.rx_tcm[tcm_index]);
                        }
                    }
                    break;
                    
                case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                    if (DIGI_OTN_LINE_SIDE == location)
                    {
                        odu_int_table = &(otn_int_table->otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                        tcm_int_table = &(otn_int_table->otn_odu_mo_chnl[odu_chnl].status.pid.rx_tcm[tcm_index]);
                    }
                    else
                    {
                        odu_int_table = &(otn_int_table->otn_odu_mo_chnl[odu_chnl].stg3b.framer_chnl);
                        tcm_int_table = &(otn_int_table->otn_odu_mo_chnl[odu_chnl].stg3b.pid.rx_tcm[tcm_index]);
                    }
                    break;

                case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                    if (DIGI_OTN_LINE_SIDE == location)
                    {
                        odu_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                        tcm_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[tcm_index]);
                    }
                    else
                    {
                        odu_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                        tcm_int_table = &(otn_int_table->otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[tcm_index]);
                    }
                    break;

                default:
                    PMC_FREE(&otn_int_table);
                    PMC_FREE(&mapper_int_table);
                    PMC_RETURN(PMC_ERR_FAIL);
                    break;
            }
        }
        else if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            /* get interrupt information */
            rc |= digi_mapper_int_enabled_get(adpt_handle->digi_handle, mapper_int_table);
            /* get GFP channel */
            rc |= adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
            if (PMC_SUCCESS != rc)
            {
                PMC_FREE(&otn_int_table);
                PMC_FREE(&mapper_int_table);
                PMC_RETURN(rc);
            }

            odu_int_table = &(mapper_int_table->mapper_chnl[gfp_chnl].fo2.status.framer_chnl);
            tcm_int_table = &(mapper_int_table->mapper_chnl[gfp_chnl].fo2.status.pid.rx_tcm[tcm_index]);
        }
        else
        {
            PMC_FREE(&otn_int_table);
            PMC_FREE(&mapper_int_table);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        if (DIGI_OTN_SEG_SM == seg_id)
        {
            int_table->oof     = odu_int_table->rfrm.oof_i;
            int_table->dloflom = odu_int_table->rfrm.dloflom_i;
        }
        else if (DIGI_OTN_SEG_PM == seg_id)
        {
            int_table->ac_aps  = odu_int_table->rfrm.ac_aps_i;
            int_table->dlck    = odu_int_table->rfrm.dlck_pm_i;
            int_table->doci    = odu_int_table->rfrm.doci_pm_i;
            int_table->dais    = odu_int_table->rfrm.dais_pm_i;
            int_table->dtim    = odu_int_table->rfrm.dtim_pm_i;
            int_table->ddeg    = odu_int_table->rfrm.ddeg_pm_i;
            int_table->dedeg   = odu_int_table->rfrm.ddeg_enh_pm_i;
            int_table->dbdi    = odu_int_table->rfrm.dbdi_pm_i;
            int_table->atsf    = odu_int_table->rfrm.atsf_pm_i;
            int_table->atsd    = odu_int_table->rfrm.atsd_pm_i;
            int_table->abdi    = odu_int_table->rfrm.abdi_pm_i;
            int_table->dmsim   = odu_int_table->odtu_dmx.dmsim_i;
            int_table->dplm    = odu_int_table->odtu_dmx.dplm_i;
            if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
            {
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    int_table->dloomfi = otn_int_table->coreotn.fmf1_dmx.dloomfi_i;
                }
                else
                {
                    int_table->dloomfi = otn_int_table->coreotn.fmf2_dmx.dloomfi_i;
                }
            }
            else
            {
                int_table->dloomfi = FALSE;
            }

            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                if (mapper_int_table->mapper_chnl[gfp_chnl].status.rgmp_valid)
                {
                    int_table->dcsf = mapper_int_table->mapper_chnl[gfp_chnl].status.rgmp.csf_i;
                    int_table->dplm = mapper_int_table->mapper_chnl[gfp_chnl].status.rgmp.dplm_i;
                }
                else
                {
                    int_table->dcsf = mapper_int_table->mapper_chnl[gfp_chnl].status.opu_csf_i;
                    int_table->dplm = mapper_int_table->mapper_chnl[gfp_chnl].status.dplm_i;
                }
            }
        }
        else
        {
            int_table->dlck  = tcm_int_table->dlck_i;
            int_table->doci  = tcm_int_table->doci_i;
            int_table->dais  = tcm_int_table->dais_i;
            int_table->dtim  = tcm_int_table->dtim_i;
            int_table->ddeg  = tcm_int_table->ddeg_i;
            int_table->dedeg = tcm_int_table->ddeg_enh_i;
            int_table->dbdi  = tcm_int_table->dbdi_i;
            int_table->dbiae = tcm_int_table->dbiae_i;
            int_table->dltc  = tcm_int_table->dltc_i;
            int_table->diae  = tcm_int_table->diae_i;
            int_table->atsf  = tcm_int_table->atsf_i;
            int_table->atsd  = tcm_int_table->atsd_i;
            int_table->aais  = tcm_int_table->aais_i;
            int_table->abdi  = tcm_int_table->abdi_i;
        }
    }

    /* free memory */
    PMC_FREE(&otn_int_table);
    PMC_FREE(&mapper_int_table);

    PMC_RETURN(rc);
} /* adpt_otn_int_en_get */


/*******************************************************************************
* adpt_otn_seg_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the OTN interrupt based on the segment.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only available for PM and TCMi.
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not available for stage 4.
*   enable             - TRUE: enable this segment interrupt.
*                        FALSE: disable this segment interrupt.
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
*   The adpt_otn_seg_int_en_set() API covers the interrupts of the segment level
*   for a given framer, while the adpt_otn_defect_int_en_set() API covers the 
*   OTN interupts of the stage level. Only both of them are enabled, its interrupt
*   can be available.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_seg_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT32 tcm_pool_id = 0;
    odu_struct_odu_level_t odu_level = ODU_STRUCT_LEVEL_HO_ODU;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* check parameter validation */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id))
        || (location >= LAST_DIGI_OTN_SIDE))
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
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_OTN_SEG_PM == seg_id)
    {
        if (enable)
        {
            rc = digi_otn_odu_int_pm_chnl_enable(digi_handle, odu_handle);
        }
        else
        {
            rc = digi_otn_odu_int_pm_chnl_disable(digi_handle, odu_handle);
        }
        ADPT_RETURN_VERIFY(rc);
    }    
    else 
    {
        tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
        if (!(DIGI_TCM_EN_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index])))
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        rc = adpt_tcm_pool_id_get(dev_id, chnl_id, seg_id, location, 
                                  DIGI_OTN_SINK_POS, &tcm_pool_id);
        ADPT_RETURN_VERIFY(rc);

        odu_level = digi_otn_odu_level_get(digi_handle, odu_handle);
        if (enable)
        {
            rc = digi_otn_odu_int_pid_enable(digi_handle,
                                             odu_level,
                                             tcm_pool_id);
        }
        else
        {
            rc = digi_otn_odu_int_pid_disable(digi_handle, 
                                              odu_level,
                                              tcm_pool_id);
        }
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_otn_seg_int_en_set */


/*******************************************************************************
* adpt_otn_seg_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of the OTN interrupts based on the segment.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. Only available for PM and TCMi.
*   location           - line side(stage 1/2/3A) or system side (stage 3B).
*                        Not available for stage 4.
*   enable             - TRUE: enable this segment interrupt.
*                        FALSE: disable this segment interrupt.
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
PUBLIC PMC_ERROR adpt_otn_seg_int_en_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    UINT32 tcm_pool_id = 0;
    odu_struct_odu_level_t odu_level = ODU_STRUCT_LEVEL_HO_ODU;
    util_global_otn_pm_status_t otn_int;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* check parameter validation */
    if (((seg_id > DIGI_OTN_SEG_TCM6) && (DIGI_OTN_SEG_PM_NIM != seg_id))
        || (location >= LAST_DIGI_OTN_SIDE))
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
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&otn_int, 0, sizeof(util_global_otn_pm_status_t));

    rc = digi_otn_pm_status_get(adpt_handle->digi_handle, odu_handle, odu_level, &otn_int);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_OTN_SEG_PM == seg_id)
    {
        if (NULL != adpt_handle->traffic_info[index].odu_handle)
        {
            /* get odu channel ID and odu stage number */
            rc |= adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            /* get GFP channel */
            rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        *enable = otn_int.int_chnl_en[odu_chnl] ? TRUE : FALSE;
    }
    else
    {
        tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
        if (!(DIGI_TCM_EN_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index])))
        {
            *enable = FALSE;
        }
        else
        {
            rc = adpt_tcm_pool_id_get(dev_id, chnl_id, seg_id, location, 
                                      DIGI_OTN_SINK_POS, &tcm_pool_id);
            ADPT_RETURN_VERIFY(rc);

            *enable = otn_int.int_pid_en[tcm_pool_id];
        }
    }

    PMC_RETURN(rc);
} /* adpt_otn_seg_int_en_get */


/*******************************************************************************
* adpt_otn_defect_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the OTN interrupt based on ODU level.
*
* INPUTS:
*   dev_id             - device identity.
*   odu_level          - indicates whether to enable interrupts on HO/MO/LO/3B/4.
*   int_mask           - OTN interrupt mask. The 'bit 1' is to enable its interrupt
*                        and the 'bit 0' is to disable its interrupt. The OTN
*                        interrupt mask lists as follows.
*                        DIGI_OTN_INT_MASK_DLCK
*                        DIGI_OTN_INT_MASK_DOCI    
*                        DIGI_OTN_INT_MASK_DAIS    
*                        DIGI_OTN_INT_MASK_DTIM    
*                        DIGI_OTN_INT_MASK_DDEG    
*                        DIGI_OTN_INT_MASK_DBDI    
*                        DIGI_OTN_INT_MASK_DLTC    
*                        DIGI_OTN_INT_MASK_DIAE    
*                        DIGI_OTN_INT_MASK_DBIAE   
*                        DIGI_OTN_INT_MASK_DLOFLOM 
*                        DIGI_OTN_INT_MASK_OOF     
*                        DIGI_OTN_INT_MASK_ATSF    
*                        DIGI_OTN_INT_MASK_ATSD    
*                        DIGI_OTN_INT_MASK_AAIS    
*                        DIGI_OTN_INT_MASK_ABDI    
*                        DIGI_OTN_INT_MASK_ALL (cover all OTN interrupt masks)     
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
*   The odu_level can be retrieved through the adpt_otn_odu_level_get() API based
*   on the given channel.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_defect_int_en_set(UINT32 dev_id, 
                                       odu_struct_odu_level_t odu_level,
                                       UINT32 int_mask)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* dLCK interrupt */
    if (DIGI_OTN_INT_MASK_DLCK & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLCK);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLCK);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dOCI interrupt */
    if (DIGI_OTN_INT_MASK_DOCI & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DOCI);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DOCI);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dAIS interrupt */
    if (DIGI_OTN_INT_MASK_DAIS & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DAIS);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DAIS);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dTIM interrupt */
    if (DIGI_OTN_INT_MASK_DTIM & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DTIM);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DTIM);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dDEG interrupt */
    if (DIGI_OTN_INT_MASK_DDEG & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DDEG);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DDEG);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dBDI interrupt */
    if (DIGI_OTN_INT_MASK_DBDI & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DBDI);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DBDI);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dLTC interrupt */
    if (DIGI_OTN_INT_MASK_DLTC & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLTC);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLTC);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dIAE interrupt */
    if (DIGI_OTN_INT_MASK_DIAE & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DIAE);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DIAE);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dBIAE interrupt */
    if (DIGI_OTN_INT_MASK_DBIAE & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DBIAE);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DBIAE);
    }
    ADPT_RETURN_VERIFY(rc);

    /* dLOFLOM interrupt */
    if (DIGI_OTN_INT_MASK_DLOFLOM & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLOFLOM);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_DLOFLOM);
    }
    ADPT_RETURN_VERIFY(rc);

    /* OOF interrupt */
    if (DIGI_OTN_INT_MASK_OOF & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_OOF);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_OOF);
    }
    ADPT_RETURN_VERIFY(rc);

    /* aTSF interrupt */
    if (DIGI_OTN_INT_MASK_ATSF & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ATSF);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ATSF);
    }
    ADPT_RETURN_VERIFY(rc);

    /* aTSD interrupt */
    if (DIGI_OTN_INT_MASK_ATSD & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ATSD);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ATSD);
    }
    ADPT_RETURN_VERIFY(rc);

    /* aAIS interrupt */
    if (DIGI_OTN_INT_MASK_AAIS & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_AAIS);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_AAIS);
    }
    ADPT_RETURN_VERIFY(rc);

    /* aBDI interrupt */
    if (DIGI_OTN_INT_MASK_ABDI & int_mask)
    {
        rc = digi_otn_odu_int_defect_enable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ABDI);
    }
    else
    {
        rc = digi_otn_odu_int_defect_disable(adpt_handle->digi_handle,
                                            odu_level,
                                            ODU_RFRM_INT_DEFECT_COMB_ABDI);
    }
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_defect_int_en_set */


/*******************************************************************************
* adpt_otn_defect_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of the OTN interrupts based on ODU level.
*
* INPUTS:
*   dev_id             - device identity.
*   odu_level          - indicates whether to enable interrupts on HO/MO/LO/3B/4.
*
* OUTPUTS:
*   *int_mask          - OTN interrupt mask. The 'bit 1' is to enable its interrupt
*                        and the 'bit 0' is to disable its interrupt. The OTN
*                        interrupt mask lists as follows.
*                        DIGI_OTN_INT_MASK_DLCK
*                        DIGI_OTN_INT_MASK_DOCI    
*                        DIGI_OTN_INT_MASK_DAIS    
*                        DIGI_OTN_INT_MASK_DTIM    
*                        DIGI_OTN_INT_MASK_DDEG    
*                        DIGI_OTN_INT_MASK_DBDI    
*                        DIGI_OTN_INT_MASK_DLTC    
*                        DIGI_OTN_INT_MASK_DIAE    
*                        DIGI_OTN_INT_MASK_DBIAE   
*                        DIGI_OTN_INT_MASK_DLOFLOM 
*                        DIGI_OTN_INT_MASK_OOF     
*                        DIGI_OTN_INT_MASK_ATSF    
*                        DIGI_OTN_INT_MASK_ATSD    
*                        DIGI_OTN_INT_MASK_AAIS    
*                        DIGI_OTN_INT_MASK_ABDI    
*                        DIGI_OTN_INT_MASK_ALL (cover all OTN interrupt masks)     
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The odu_level can be retrieved through the adpt_otn_odu_level_get() API based
*   on the given channel.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_defect_int_en_get(UINT32 dev_id, 
                                       odu_struct_odu_level_t odu_level,
                                       UINT32 *int_mask)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_otn_pm_status_t otn_int;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_mask);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&otn_int, 0, sizeof(util_global_otn_pm_status_t));

    rc = digi_otn_pm_status_get(adpt_handle->digi_handle, NULL, odu_level, &otn_int);
    ADPT_RETURN_VERIFY(rc);

    *int_mask = 0;
    for (index = 0; index < UTIL_GLOBAL_NUM_ODU_RFRM_DEFECTS; index++)
    {
        *int_mask |= otn_int.int_comb_en[index] ? (1 << index) : 0;
    }

    PMC_RETURN(rc);
} /* adpt_otn_defect_int_en_get */


/*******************************************************************************
* adpt_mld_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the OTN interrupt in MLD layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. OTU4: 20, OTU3: 4.
*   *int_table         - MLD defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_mld_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  UINT32 lane_id,
                                  digi_mld_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    lineotn_int_chnl_t otu_int_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    /* reset */
    PMC_MEMSET((void*)&otu_int_table, 0, sizeof(otu_int_table));

    /* enable interrupt */
    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
    {
        otu_int_table.opsm4_rx_lane_los_int_i[lane_id]         = int_table->dlos;
        otu_int_table.opsm4_rx_lane_oof_int_i[lane_id]         = int_table->doof;
        otu_int_table.opsm4_rx_lane_otuk_ais_int_i[lane_id]    = int_table->dotuais;
        otu_int_table.opsm4_rx_lane_lof_loflane_int_i[lane_id] = int_table->dloflane;
        otu_int_table.opsm4_rx_lane_oor_int_i[lane_id]         = int_table->door;
        otu_int_table.opsm4_rx_lane_lor_int_i[lane_id]         = int_table->dlor;
        otu_int_table.opsm4_rx_lane_oom_int_i[lane_id]         = int_table->doom;
    }
    else
    {
        if (lane_id < 4)
        {
            otu_int_table.opsm23_rx_lane_los_int_i[lane_id]         = int_table->dlos;
            otu_int_table.opsm23_rx_lane_oof_int_i[lane_id]         = int_table->doof;
            otu_int_table.opsm23_rx_lane_otuk_ais_int_i[lane_id]    = int_table->dotuais;
            otu_int_table.opsm23_rx_lane_lof_loflane_int_i[lane_id] = int_table->dloflane;
            otu_int_table.opsm23_rx_lane_oor_int_i[lane_id]         = int_table->door;
            otu_int_table.opsm23_rx_lane_lor_int_i[lane_id]         = int_table->dlor;
        }
    }

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].otu_handle);

    rc = digi_otn_server_int_chnl_enable(adpt_handle->digi_handle,
                                         adpt_handle->traffic_info[index].otu_handle,
                                         &otu_int_table);
    ADPT_RETURN_VERIFY(rc);

    /* disable interrupt */
    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
    {
        otu_int_table.opsm4_rx_lane_los_int_i[lane_id]         = !(int_table->dlos);
        otu_int_table.opsm4_rx_lane_oof_int_i[lane_id]         = !(int_table->doof);
        otu_int_table.opsm4_rx_lane_otuk_ais_int_i[lane_id]    = !(int_table->dotuais);
        otu_int_table.opsm4_rx_lane_lof_loflane_int_i[lane_id] = !(int_table->dloflane);
        otu_int_table.opsm4_rx_lane_oor_int_i[lane_id]         = !(int_table->door);
        otu_int_table.opsm4_rx_lane_lor_int_i[lane_id]         = !(int_table->dlor);
        otu_int_table.opsm4_rx_lane_oom_int_i[lane_id]         = !(int_table->doom);
    }
    else
    {
        if (lane_id < 4)
        {
            otu_int_table.opsm23_rx_lane_los_int_i[lane_id]         = !(int_table->dlos);
            otu_int_table.opsm23_rx_lane_oof_int_i[lane_id]         = !(int_table->doof);
            otu_int_table.opsm23_rx_lane_otuk_ais_int_i[lane_id]    = !(int_table->dotuais);
            otu_int_table.opsm23_rx_lane_lof_loflane_int_i[lane_id] = !(int_table->dloflane);
            otu_int_table.opsm23_rx_lane_oor_int_i[lane_id]         = !(int_table->door);
            otu_int_table.opsm23_rx_lane_lor_int_i[lane_id]         = !(int_table->dlor);
        }
    }

    rc = digi_otn_server_int_chnl_disable(adpt_handle->digi_handle,
                                          adpt_handle->traffic_info[index].otu_handle,
                                          &otu_int_table);
    

    PMC_RETURN(rc);
} /* adpt_mld_int_en_set */


/*******************************************************************************
* adpt_mld_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of the OTN interrupt in MLD layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. OTU4: 20, OTU3: 4.
*
* OUTPUTS:
*   *int_table         - MLD defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mld_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  UINT32 lane_id,
                                  digi_mld_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 otu_chnl = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_int_t *otn_int_table = NULL;
    UINT32 traffic_loc = 0;
    lineotn_int_chnl_t *otu_int_table = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (NULL == adpt_handle->traffic_info[index].otu_handle)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    /* allocate memory */
    otn_int_table = (digi_otn_int_t*)PMC_CALLOC(sizeof(digi_otn_int_t));
    ADPT_PARA_VERIFY(otn_int_table);

    /* reset */
    PMC_MEMSET(int_table, 0, sizeof(digi_mld_int_en_t));

    /* get interrupt information */
    rc |= digi_otn_int_enabled_get(adpt_handle->digi_handle, otn_int_table);
    /* get OTU channel */
    rc |= adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
    if (PMC_SUCCESS != rc)
    {
        PMC_FREE(&otn_int_table);
        PMC_RETURN(rc);
    }

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        otu_int_table = &(otn_int_table->otn_server_chnl[otu_chnl]);
    }
    else
    {
        otu_int_table = &(otn_int_table->sysotn_server_chnl[otu_chnl]);
    }

    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
    {
        int_table->dlos     = otu_int_table->opsm4_rx_lane_los_int_i[lane_id];
        int_table->doof     = otu_int_table->opsm4_rx_lane_oof_int_i[lane_id];
        int_table->dotuais  = otu_int_table->opsm4_rx_lane_otuk_ais_int_i[lane_id];
        int_table->dloflane = otu_int_table->opsm4_rx_lane_lof_loflane_int_i[lane_id];
        int_table->door     = otu_int_table->opsm4_rx_lane_oor_int_i[lane_id];
        int_table->dlor     = otu_int_table->opsm4_rx_lane_lor_int_i[lane_id];
        int_table->doom     = otu_int_table->opsm4_rx_lane_oom_int_i[lane_id];
    }
    else
    {
        if (lane_id < 4)
        {
            int_table->dlos     = otu_int_table->opsm23_rx_lane_los_int_i[lane_id];
            int_table->doof     = otu_int_table->opsm23_rx_lane_oof_int_i[lane_id];
            int_table->dotuais  = otu_int_table->opsm23_rx_lane_otuk_ais_int_i[lane_id];
            int_table->dloflane = otu_int_table->opsm23_rx_lane_lof_loflane_int_i[lane_id];
            int_table->door     = otu_int_table->opsm23_rx_lane_oor_int_i[lane_id];
            int_table->dlor     = otu_int_table->opsm23_rx_lane_lor_int_i[lane_id];
            int_table->doom     = FALSE;
        }
    }

    /* free memory */
    PMC_FREE(&otn_int_table);

    PMC_RETURN(rc);
} /* adpt_otn_int_en_get */


/*******************************************************************************
* adpt_gfp_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the GFP-F/GFP-T interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - GFP-F/GFP-T defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_gfp_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_gfp_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    mapotn_int_chnl_t map_int_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&map_int_table, 0, sizeof(map_int_table));

    /* enable interrupt */
    map_int_table.lofd_i          = int_table->dlfd;
    map_int_table.exi_i           = int_table->dexm;
    map_int_table.pti_upi_msmch_i = int_table->dupm;
    map_int_table.csf_i           = int_table->dcsf;
    map_int_table.idle_i          = int_table->didle;

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].map_handle);

    rc = digi_mapper_int_chnl_enable(adpt_handle->digi_handle,
                                     adpt_handle->traffic_info[index].map_handle,
                                     &map_int_table);
    ADPT_RETURN_VERIFY(rc);

    /* enable interrupt */
    map_int_table.lofd_i          = !(int_table->dlfd);
    map_int_table.exi_i           = !(int_table->dexm);
    map_int_table.pti_upi_msmch_i = !(int_table->dupm);
    map_int_table.csf_i           = !(int_table->dcsf);
    map_int_table.idle_i          = !(int_table->didle);

    rc = digi_mapper_int_chnl_disable(adpt_handle->digi_handle,
                                      adpt_handle->traffic_info[index].map_handle,
                                      &map_int_table);

    PMC_RETURN(rc);
} /* adpt_gfp_int_en_set */


/*******************************************************************************
* adpt_gfp_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of GFP-F/GFP-T interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *int_table         - GFP-F/GFP-T defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_gfp_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_mapper_int_t *mapper_int_table= NULL;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    mapotn_int_chnl_t *gfp_int = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* allocate memory */
    mapper_int_table = (digi_mapper_int_t*)PMC_CALLOC(sizeof(digi_mapper_int_t));
    ADPT_PARA_VERIFY(mapper_int_table);

    /* reset */
    PMC_MEMSET(int_table, 0, sizeof(digi_gfp_int_en_t));

    /* get interrupt information */
    rc |= digi_mapper_int_enabled_get(adpt_handle->digi_handle, mapper_int_table);
    /* get GFP channel */
    rc |= adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
    if (PMC_SUCCESS != rc)
    {
        PMC_FREE(&mapper_int_table);
        PMC_RETURN(rc);
    }

    gfp_int = &(mapper_int_table->mapper_chnl[gfp_chnl].status);

    int_table->dlfd  = gfp_int->lofd_i;
    int_table->dexm  = gfp_int->exi_i;
    int_table->dupm  = gfp_int->pti_upi_msmch_i;
    int_table->dcsf  = gfp_int->csf_i;
    int_table->didle = gfp_int->idle_i;

    /* free memory */
    PMC_FREE(&mapper_int_table);

    PMC_RETURN(rc);
} /* adpt_gfp_int_en_get */


/*******************************************************************************
* adpt_sdh_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the SDH/SONET interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   *int_table         - SDH/SONET defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_sdh_int_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    cbrc_int_chnl_t cbr_int_table;
    sdh_pmg_int_chnl_t *sdh_pmg = NULL;
    cbr_fsgm_int_chnl_t *cbr_fsgm = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        sdh_pmg  = &(cbr_int_table.sdh_pmg_rx);
        cbr_fsgm = &(cbr_int_table.cbr_fsgm_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        sdh_pmg  = &(cbr_int_table.sdh_pmg_tx);
        cbr_fsgm = &(cbr_int_table.cbr_fsgm_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));
    /* enable interrupt */
    sdh_pmg->los_i       = int_table->dlos;
    cbr_fsgm->pn11_det_i = int_table->dgais;
    sdh_pmg->lof_i       = int_table->dlof;
    sdh_pmg->oof_i       = int_table->doof;
    sdh_pmg->tim_i       = int_table->dtim;
    sdh_pmg->tiu_i       = int_table->dtiu;
    sdh_pmg->lais_i      = int_table->dms_ais;
    sdh_pmg->lrdi_i      = int_table->dms_rdi;

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

    rc = digi_cbr_int_chnl_enable(adpt_handle->digi_handle, 
                                  adpt_handle->traffic_info[index].cbr_handle,
                                  &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    /* disable interrupt */
    sdh_pmg->los_i       = !(int_table->dlos);
    cbr_fsgm->pn11_det_i = !(int_table->dgais);
    sdh_pmg->lof_i       = !(int_table->dlof);
    sdh_pmg->oof_i       = !(int_table->doof);
    sdh_pmg->tim_i       = !(int_table->dtim);
    sdh_pmg->tiu_i       = !(int_table->dtiu);
    sdh_pmg->lais_i      = !(int_table->dms_ais);
    sdh_pmg->lrdi_i      = !(int_table->dms_rdi);

    rc = digi_cbr_int_chnl_disable(adpt_handle->digi_handle, 
                                   adpt_handle->traffic_info[index].cbr_handle,
                                   &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_sdh_int_en_set */


/*******************************************************************************
* adpt_sdh_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of the SDH/SONET interrupt based on 
*   channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *int_table         - SDH/SONET defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_int_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    sdh_pmg_int_chnl_t *sdh_pmg = NULL;
    cbr_fsgm_int_chnl_t *cbr_fsgm = NULL;
    digi_cbr_int_t cbr_int_table;
    UINT32 cbr_chnl = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc = adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        sdh_pmg  = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_rx);
        cbr_fsgm = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        sdh_pmg  = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_tx);
        cbr_fsgm = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));

    rc = digi_cbr_int_enabled_get(adpt_handle->digi_handle, &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    int_table->dlos    = sdh_pmg->los_i;
    int_table->dgais   = cbr_fsgm->pn11_det_i;
    int_table->dlof    = sdh_pmg->lof_i;
    int_table->doof    = sdh_pmg->oof_i;
    int_table->dtim    = sdh_pmg->tim_i;
    int_table->dtiu    = sdh_pmg->tiu_i;
    int_table->dms_ais = sdh_pmg->lais_i;
    int_table->dms_rdi = sdh_pmg->lrdi_i;

    PMC_RETURN(rc);
} /* adpt_sdh_int_en_get */


/*******************************************************************************
* adpt_fc1200_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the FC1200 interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - FC1200 defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_fc1200_int_en_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    cbrc_int_chnl_t cbr_int_table;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));
    /* enable interrupt */
    cbr_int_table.fc1200_pcs_rx_los_i       = int_table->dlos;
    cbr_int_table.fc1200_pcs_rx_loss_sync_i = int_table->dloss_sync;
    cbr_int_table.fc1200_pcs_rx_high_ber_i  = int_table->dhigh_ber;
    cbr_int_table.fc1200_pcs_rx_lf_i        = int_table->dlf;
    cbr_int_table.fc1200_pcs_rx_rf_i        = int_table->drf;

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

    rc = digi_cbr_int_chnl_enable(adpt_handle->digi_handle, 
                                  adpt_handle->traffic_info[index].cbr_handle,
                                  &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    /* disable interrupt */
    cbr_int_table.fc1200_pcs_rx_los_i       = !(int_table->dlos);
    cbr_int_table.fc1200_pcs_rx_loss_sync_i = !(int_table->dloss_sync);
    cbr_int_table.fc1200_pcs_rx_high_ber_i  = !(int_table->dhigh_ber);
    cbr_int_table.fc1200_pcs_rx_lf_i        = !(int_table->dlf);
    cbr_int_table.fc1200_pcs_rx_rf_i        = !(int_table->drf);

    rc = digi_cbr_int_chnl_disable(adpt_handle->digi_handle, 
                                   adpt_handle->traffic_info[index].cbr_handle,
                                   &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_fc1200_int_en_set */


/*******************************************************************************
* adpt_fc1200_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC1200 interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *int_table         - FC1200 defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc1200_int_en_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_cbr_int_t cbr_int_table;
    UINT32 cbr_chnl = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc = adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));

    rc = digi_cbr_int_enabled_get(adpt_handle->digi_handle, &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    int_table->dlos       = cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc1200_pcs_rx_los_i;
    int_table->dloss_sync = cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc1200_pcs_rx_loss_sync_i;
    int_table->dhigh_ber  = cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc1200_pcs_rx_high_ber_i;
    int_table->dlf        = cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc1200_pcs_rx_lf_i;
    int_table->drf        = cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc1200_pcs_rx_rf_i;

    PMC_RETURN(rc);
} /* adpt_fc1200_int_en_get */


/*******************************************************************************
* adpt_fc800_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the FC800 interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   *int_table         - FC800 defect interrupt data structure.
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
PUBLIC PMC_ERROR adpt_fc800_int_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    cbrc_int_chnl_t cbr_int_table;
    fc800_pmg_int_chnl_t *fc800_pmg = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));

    if (DIGI_PORT_DIR_RX == dir)
    {
        fc800_pmg = &(cbr_int_table.fc800_pmg_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        fc800_pmg = &(cbr_int_table.fc800_pmg_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* enable interrupt */
    fc800_pmg->rx_disp_invert_i = int_table->ddisp_inv;
    fc800_pmg->rx_xdet_i        = int_table->dxdet;
    fc800_pmg->rx_dlolb_i       = int_table->dlolb;
    fc800_pmg->rx_asd_i         = int_table->dasd;
    fc800_pmg->rx_sync_i        = int_table->dsync;
    fc800_pmg->rx_lcv_i         = int_table->dlcv;

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

    rc = digi_cbr_int_chnl_enable(adpt_handle->digi_handle, 
                                  adpt_handle->traffic_info[index].cbr_handle,
                                  &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    /* disable interrupt */
    fc800_pmg->rx_disp_invert_i = !(int_table->ddisp_inv);
    fc800_pmg->rx_xdet_i        = !(int_table->dxdet);
    fc800_pmg->rx_dlolb_i       = !(int_table->dlolb);
    fc800_pmg->rx_asd_i         = !(int_table->dasd);
    fc800_pmg->rx_sync_i        = !(int_table->dsync);
    fc800_pmg->rx_lcv_i         = !(int_table->dlcv);

    rc = digi_cbr_int_chnl_disable(adpt_handle->digi_handle, 
                                   adpt_handle->traffic_info[index].cbr_handle,
                                   &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_fc800_int_en_set */


/*******************************************************************************
* adpt_fc800_int_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC800 interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *int_table         - FC800 defect interrupt data structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc800_int_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_int_en_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_cbr_int_t cbr_int_table;
    UINT32 cbr_chnl = 0;
    fc800_pmg_int_chnl_t *fc800_pmg = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc = adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        fc800_pmg = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        fc800_pmg = &(cbr_int_table.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* reset */
    PMC_MEMSET((void*)&cbr_int_table, 0, sizeof(cbr_int_table));

    rc = digi_cbr_int_enabled_get(adpt_handle->digi_handle, &cbr_int_table);
    ADPT_RETURN_VERIFY(rc);

    int_table->ddisp_inv = fc800_pmg->rx_disp_invert_i;
    int_table->dxdet     = fc800_pmg->rx_xdet_i;
    int_table->dlolb     = fc800_pmg->rx_dlolb_i;
    int_table->dasd      = fc800_pmg->rx_asd_i;
    int_table->dsync     = fc800_pmg->rx_sync_i;
    int_table->dlcv      = fc800_pmg->rx_lcv_i;

    PMC_RETURN(rc);
} /* adpt_fc800_int_en_get */




/*******************************************************************************
* adpt_int_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check interrupt information. If it exists, the defect status and interrupt
*   will be collected.
*
* INPUTS:
*   dev_id             - device identity.
*   blk_msk            - block mask.
*                        DIGI_BLK_ENET: ENET(line) and ENET(sys)
*                        DIGI_BLK_OTN:  LINEOTN and COREOTN
*                        DIGI_BLK_MAP:  MAPOTN
*                        DIGI_BLK_SIFD: SIFD
*                        DIGI_BLK_CBR:  CBR
*                        DIGI_BLK_ALL:  all blocks
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
PUBLIC PMC_ERROR adpt_int_check(UINT32 dev_id, digi_blk_type_t blk_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_BLK_ALL == blk_msk)
    {
        rc = digi_int_enabled_check(adpt_handle->digi_handle);

        /* return directly */
        PMC_RETURN(rc);
    }

    if (blk_msk & DIGI_BLK_ENET)
    {
        rc = digi_enet_line_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);

        rc = digi_enet_sys_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    if (blk_msk & DIGI_BLK_OTN)
    {
        rc = digi_otn_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    if (blk_msk & DIGI_BLK_MAP)
    {
        rc = digi_mapper_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    if (blk_msk & DIGI_BLK_SIFD)
    {
        rc = digi_sifd_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    if (blk_msk & DIGI_BLK_CBR)
    {
        rc = digi_cbr_int_enabled_check(adpt_handle->digi_handle);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_int_check */


/*******************************************************************************
* adpt_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear interrupt information. In order to shorten the interrupt process time,
*   this API should be called after finishing interrupt collection.
*
* INPUTS:
*   dev_id             - device identity.
*   blk_msk            - block mask to clear interrupts.
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
PUBLIC PMC_ERROR adpt_int_clear(UINT32 dev_id, digi_blk_type_t blk_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (blk_msk & DIGI_BLK_OTN)
    {
        rc = digi_otn_int_clear(adpt_handle->digi_handle,
                                &(adpt_handle->pmon_backup->otn_defect_bk));
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_int_clear */


/*******************************************************************************
* adpt_int_cb_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles all PMON interrupts in the callback. The following blocks will be 
*   involved.
*   + OTN
*   + CBR
*   + CPB
*   + ENET
*   + GFP 
*   + SIFD
*
* INPUTS:
*   *digi_handle       - pointer to digi handle instance
*   *token             - token, shared memory buffer, provided by user at 
*                        callback registration
*   *channel_handle    - channel handle pointer
*   int_type           - interrupt type
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
PUBLIC PMC_ERROR adpt_int_cb_handle(digi_handle_t *digi_handle,
                                 void *token,
                                 void *channel_handle,
                                 digi_int_type_t int_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_int_t int_table;
    digi_otn_int_filter_t int_filter;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_query(digi_handle, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (int_type)
    {
        case DIGI_INT_TYPE_PMON:
            /* retrieve interrupt table for this int type */
            rc = digi_pmon_int_retrieve(digi_handle, &int_table);
            ADPT_RETURN_VERIFY(rc);

            /* check for active ENET/SIFD/CBRC/MAPOTN PMON counters */
            if (TRUE == int_table.misc_i) 
            {
                /* obtain shared memory address for PMON counters */
                rc |= digi_enet_line_pmon_retrieve(digi_handle,
                                                   &(adpt_handle->pmon_backup->line_enet_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_enet_pmon_merge(&(adpt_handle->pmon_backup->line_enet_pmon), 
                                           &(adpt_handle->pmon_data->line_enet_pmon));

                /* obtain shared memory address for PMON counters */
                rc |= digi_enet_sys_pmon_retrieve(digi_handle, 
                                                  &(adpt_handle->pmon_backup->sys_enet_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_enet_pmon_merge(&(adpt_handle->pmon_backup->sys_enet_pmon), 
                                           &(adpt_handle->pmon_data->sys_enet_pmon));

                /* GFP PMON */
                rc |= digi_mapper_pmon_retrieve(digi_handle, 
                                                &(adpt_handle->pmon_backup->mapper_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_mapper_pmon_merge(&(adpt_handle->pmon_backup->mapper_pmon), 
                                             &(adpt_handle->pmon_data->mapper_pmon));

                /* obtain shared memory address for PMON counters */
                rc |= digi_sifd_pmon_retrieve(digi_handle, 
                                              &(adpt_handle->pmon_backup->sifd_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_sifd_pmon_merge(&(adpt_handle->pmon_backup->sifd_pmon), 
                                           &(adpt_handle->pmon_data->sifd_pmon));

                /* obtain shared memory address for PMON counters */
                rc |= digi_cbr_pmon_retrieve(digi_handle, 
                                             &(adpt_handle->pmon_backup->cbr_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_cbr_pmon_merge(&(adpt_handle->pmon_backup->cbr_pmon), 
                                          &(adpt_handle->pmon_data->cbr_pmon));
            }

            /* check for active OTN PMON counters */
            if (TRUE == int_table.otn_i)
            {
                /* obtain shared memory address for PMON counters */
                rc |= digi_otn_pmon_retrieve(digi_handle, 
                                             &(adpt_handle->pmon_backup->otn_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_otn_pmon_merge(&(adpt_handle->pmon_backup->otn_pmon), 
                                          &(adpt_handle->pmon_data->otn_pmon));

                /* stage 4 */
                rc |= digi_mapper_pmon_fo2_retrieve(digi_handle, 
                                                    &(adpt_handle->pmon_backup->mapper_otn_pmon));
                /* merge PMON counters into Adapter */
                rc |= adpt_mapper_otn_pmon_merge(&(adpt_handle->pmon_backup->mapper_otn_pmon), 
                                                 &(adpt_handle->pmon_data->mapper_otn_pmon));
            }

            /* clear processed interrupt table */
            rc = digi_pmon_int_clear(digi_handle, &int_table);
            break;

        case DIGI_INT_TYPE_ENET_LINE:
            /* retrieve interrupt table for this int type */
            rc |= digi_enet_line_int_retrieve(digi_handle, 
                                              &(adpt_handle->pmon_backup->line_enet_defect));
            /* merge defect and interrupt into Adapter */
            rc |= adpt_enet_int_merge(&(adpt_handle->pmon_backup->line_enet_defect), 
                                      &(adpt_handle->pmon_data->line_enet_defect));
            ADPT_RETURN_VERIFY(rc);

            /* clear processed interrupt table */
            rc = digi_enet_line_int_clear(digi_handle, 
                                          &(adpt_handle->pmon_backup->line_enet_defect));
            ADPT_RETURN_VERIFY(rc);

            /* interrupt process: workaround to collect defects again in order to avoid  
               missing defect changes between xxx_retrieve() and xxx_clear() */
            if (NULL == token)
            {
                /* retrieve interrupt table for this int type */
                rc |= digi_enet_line_int_retrieve(digi_handle, 
                                                  &(adpt_handle->pmon_backup->line_enet_defect));
                /* merge defect and interrupt into Adapter */
                rc |= adpt_enet_int_merge(&(adpt_handle->pmon_backup->line_enet_defect), 
                                          &(adpt_handle->pmon_data->line_enet_defect));
                ADPT_RETURN_VERIFY(rc);
            }
            break;

        case DIGI_INT_TYPE_ENET_SYS:
            /* retrieve interrupt table for this int type */
            rc |= digi_enet_sys_int_retrieve(digi_handle, 
                                             &(adpt_handle->pmon_backup->sys_enet_defect));
            /* merge defect and interrupt into Adapter */
            rc |= adpt_enet_int_merge(&(adpt_handle->pmon_backup->sys_enet_defect), 
                                      &(adpt_handle->pmon_data->sys_enet_defect));
            ADPT_RETURN_VERIFY(rc);

            /* clear processed interrupt table */
            rc = digi_enet_sys_int_clear(digi_handle, 
                                         &(adpt_handle->pmon_backup->sys_enet_defect));
            ADPT_RETURN_VERIFY(rc);

            /* interrupt process: workaround to collect defects again in order to avoid  
               missing defect changes between xxx_retrieve() and xxx_clear() */
            if (NULL == token)
            {
                /* retrieve interrupt table for this int type */
                rc |= digi_enet_sys_int_retrieve(digi_handle, 
                                                 &(adpt_handle->pmon_backup->sys_enet_defect));
                /* merge defect and interrupt into Adapter */
                rc |= adpt_enet_int_merge(&(adpt_handle->pmon_backup->sys_enet_defect), 
                                          &(adpt_handle->pmon_data->sys_enet_defect));
                ADPT_RETURN_VERIFY(rc);
            }
            break;

        case DIGI_INT_TYPE_OTN:
            /* defect polling */
            if (NULL != token)
            {
                /* retrieve OTN interrupt table for this int type */
                rc |= digi_otn_ddeg_int_start(digi_handle, 0);
                rc |= digi_otn_odu_int_start(digi_handle, COREOTN_LATCH_DEFECT_ODUKP);

                if (*(BOOL*)token)
                {
                    PMC_MEMSET((void*)&int_filter, 0, sizeof(int_filter));
                    int_filter.blocks = DIGI_OTN_INT_FILTER_BLOCKS_OTN_MLD    |
                                        DIGI_OTN_INT_FILTER_BLOCKS_OTN_SERVER |
                                        DIGI_OTN_INT_FILTER_BLOCKS_OTN_ODU    |
                                        DIGI_OTN_INT_FILTER_BLOCKS_OTN_STG3B  |
                                        DIGI_OTN_INT_FILTER_BLOCKS_ODTU_DMX;
                    rc |= digi_otn_int_filt_retrieve(digi_handle, 
                                                     &int_filter,
                                                     &(adpt_handle->pmon_backup->otn_defect));
                }
                else
                {
                    rc |= digi_otn_int_retrieve(digi_handle, 
                                                &(adpt_handle->pmon_backup->otn_defect));
                }

                rc |= digi_otn_odu_int_end(digi_handle, COREOTN_LATCH_DEFECT_ODUKP);
                rc |= digi_otn_ddeg_int_end(digi_handle, 0);
                ADPT_RETURN_VERIFY(rc);

                /* merge defect and interrupt into Adapter */
                rc |= adpt_otn_int_merge(&(adpt_handle->pmon_backup->otn_defect), 
                                         &(adpt_handle->pmon_data->otn_defect));
                ADPT_RETURN_VERIFY(rc);

                /* clear processed interrupt table */
                if (*(BOOL*)token)
                {
                    rc |= digi_otn_int_clear(digi_handle, 
                                             &(adpt_handle->pmon_backup->otn_defect));
                }
            }
            /* interrupt process */
            else
            {
                PMC_MEMSET((void*)&int_filter, 0, sizeof(int_filter));
                int_filter.blocks = DIGI_OTN_INT_FILTER_BLOCKS_OTN_MLD    |
                                    DIGI_OTN_INT_FILTER_BLOCKS_OTN_SERVER |
                                    DIGI_OTN_INT_FILTER_BLOCKS_OTN_ODU    |
                                    DIGI_OTN_INT_FILTER_BLOCKS_ODTU_DMX;

                /* retrieve OTN interrupt table for this int type */
                rc |= digi_otn_ddeg_int_start(digi_handle, 0);
                rc |= digi_otn_odu_int_start(digi_handle, COREOTN_LATCH_DEFECT_ODUKP);
                rc |= digi_otn_int_filt_retrieve(digi_handle, 
                                                 &int_filter,
                                                 &(adpt_handle->pmon_backup->otn_defect_bk));
                rc |= digi_otn_odu_int_end(digi_handle, COREOTN_LATCH_DEFECT_ODUKP);
                rc |= digi_otn_ddeg_int_end(digi_handle, 0);
                ADPT_RETURN_VERIFY(rc);

                /* merge defect and interrupt into Adapter */
                rc |= adpt_otn_int_merge(&(adpt_handle->pmon_backup->otn_defect_bk), 
                                         &(adpt_handle->pmon_data->otn_defect));
            }
            break;

        case DIGI_INT_TYPE_OTN_MAPPER:
            /* retrieve mapper interrupt table for this int type */
            rc |= digi_otn_ddeg_int_start(digi_handle, 0);
            rc |= digi_otn_odu_int_start(digi_handle, COREOTN_LATCH_DEFECT_STG4);
            rc |= digi_mapper_int_retrieve(digi_handle, 
                                           &(adpt_handle->pmon_backup->mapper_defect));
            rc |= digi_otn_odu_int_end(digi_handle, COREOTN_LATCH_DEFECT_STG4);
            rc |= digi_otn_ddeg_int_end(digi_handle, 0);
            ADPT_RETURN_VERIFY(rc);

            /* merge defect and interrupt into Adapter */
            rc |= adpt_mapper_int_merge(&(adpt_handle->pmon_backup->mapper_defect), 
                                        &(adpt_handle->pmon_data->mapper_defect));
            ADPT_RETURN_VERIFY(rc);

            /* clear processed interrupt table */
            rc |= digi_mapper_int_clear(digi_handle, 
                                        &(adpt_handle->pmon_backup->mapper_defect));
            break;

        case DIGI_INT_TYPE_SIFD:
            /* retrieve interrupt table for this int type */
            rc |= digi_sifd_int_retrieve(digi_handle, 
                                         &(adpt_handle->pmon_backup->sifd_defect));
            /* merge defect and interrupt into Adapter */
            rc |= adpt_sifd_int_merge(&(adpt_handle->pmon_backup->sifd_defect), 
                                      &(adpt_handle->pmon_data->sifd_defect));
            ADPT_RETURN_VERIFY(rc);

            /* clear processed interrupt table */
            rc = digi_sifd_int_clear(digi_handle, 
                                     &(adpt_handle->pmon_backup->sifd_defect));
            break;

        case DIGI_INT_TYPE_CBR:
            /* retrieve interrupt table for this int type */
            rc |= digi_cbr_int_retrieve(digi_handle, 
                                        &(adpt_handle->pmon_backup->cbr_defect));
            /* merge defect and interrupt into Adapter */
            rc |= adpt_cbr_int_merge(&(adpt_handle->pmon_backup->cbr_defect), 
                                     &(adpt_handle->pmon_data->cbr_defect));
            ADPT_RETURN_VERIFY(rc);

            /* clear processed interrupt table */
            rc = digi_cbr_int_clear(digi_handle, 
                                    &(adpt_handle->pmon_backup->cbr_defect));
            ADPT_RETURN_VERIFY(rc);

            /* interrupt process: workaround to collect defects again in order to avoid  
               missing defect changes between xxx_retrieve() and xxx_clear() */
            if (NULL == token)
            {
                /* retrieve interrupt table for this int type */
                rc |= digi_cbr_int_retrieve(digi_handle, 
                                            &(adpt_handle->pmon_backup->cbr_defect));
                /* merge defect and interrupt into Adapter */
                rc |= adpt_cbr_int_merge(&(adpt_handle->pmon_backup->cbr_defect), 
                                         &(adpt_handle->pmon_data->cbr_defect));
                ADPT_RETURN_VERIFY(rc);
            }
            break;

        default:
            rc = PMC_ERR_FAIL;
            break;
    } 

    PMC_RETURN(rc);
}/* adpt_int_cb_handle */


/*
** End of file
*/
