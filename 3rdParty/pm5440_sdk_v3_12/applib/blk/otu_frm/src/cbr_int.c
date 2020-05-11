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
#include "otu_frm_io_inline.h"
#include "otu_frm.h"

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
** Forward declarations
*/


/*
** Public Functions
*/


/*******************************************************************************
* cbr_int_channel_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function fully configures an interleaver channel. The channel configuration
*   consists of 2 major steps:
*   1) Multi-lane configuration per data rate (10G,40G,100G); 
*   2) Assigned multi-lanes channels configuration by specifying channel descriptors.\n\n
*
*   The interleaver has 12 lanes of 10 Gbps all together. These lanes can be used as
*   separate 10G channels or aggregated as 40G or 100G lanes based on a predefined lane
*   provisioning into multi-lanes.
*
*   Multi-lane provisioning is as follows: \n
*   1) For 100G multi-lane - all channels 0..11 are allocated\n
*   2) For 40G multi-lanes:\n
*           - 10G lanes 0..3  map to 40G lane 0\n
*           - 10G lanes 4..7  map to 40G lane 1\n
*           - 10G lanes 8..11 map to 40G lane 2\n
*   3) For separate 10G lanes any choice of lanes is OK.\n\n
*
*   Since the lanes can be allocated only once per multi-lane the following mutual
*   exclusion rules apply when configuring different multi-lanes: \n
*   1) When configuring 100G multi-lane no other 10G or 40G can be configured \n
*   2) When configuring 40G Multi-lane, 100G multi-lane should be disabled \n
*   3) When configuring 10G separate lanes, 100G multi-lane should be disabled and
*      if the lane belongs to an already configured 40G multi-lane, this 40G multi-lane
*      should be disabled.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   lane_rate           - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
*   channel_dscptr      - pointer to user channel descriptor structure:
*
*   provision_ch        - otu_frm_bit_value_t, SET_BIT : provision channel;
*                                              RESET_BIT : deprovision channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise CBR_DINT_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_int_channel_cfg_set( otu_frm_handle_t*otu_frm_handle,
                                          otu_frm_stream_rate_t lane_rate,
                                          int_dint_channel_cfg_dsc_t*channel_dscptr,
                                          otu_frm_bit_value_t provision_ch )

{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(channel_dscptr!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(channel_dscptr->channel_lane < MAX_10G_LANE_NUM,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,0);

    switch (lane_rate)
    {

    case RATE_10G:
        /* set the multi-lane configuration for the requested lane */
        rc = cbr_int_multilane_cfg_set(otu_frm_handle,RATE_10G,channel_dscptr->channel_lane,provision_ch);

        /* set the channel descriptor for the requested lane */
        if( PMC_SUCCESS == rc )
        {
            rc = cbr_int_channel_dsc_set(otu_frm_handle,channel_dscptr,RATE_10G);
            PMC_ASSERT(PMC_SUCCESS == rc ,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        }
        break;

    case RATE_40G:
        /* set the multi-lane configuration for the requested lane */
        rc = cbr_int_multilane_cfg_set(otu_frm_handle,RATE_40G,channel_dscptr->channel_lane,provision_ch);

        if( PMC_SUCCESS == rc )
        {
            /* Calculate start and end 10G lanes assigned to the requested 40G lane */
            /* 10G lanes 0..3  map to 40G lane 0                                    */
            /* 10G lanes 4..7  map to 40G lane 1                                    */
            /* 10G lanes 8..11 map to 40G lane 2                                    */
            rc = cbr_int_channel_dsc_set(otu_frm_handle, channel_dscptr,RATE_40G);
            PMC_ASSERT(PMC_SUCCESS == rc ,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        }
        break;

    case RATE_100G:
        /* set the multi-lane configuration for the requested lane */
        rc = cbr_int_multilane_cfg_set(otu_frm_handle,RATE_100G,channel_dscptr->channel_lane,provision_ch);

        if( PMC_SUCCESS == rc )
        {
            /* when setting a 100G channel we use all 12 10G lanes                  */
            /* 10G lanes 0..3  map to 40G lane 0                                    */
            /* 10G lanes 4..7  map to 40G lane 1                                    */
            /* 10G lanes 8..11 map to 40G lane 2                                    */
            rc = cbr_int_channel_dsc_set(otu_frm_handle, channel_dscptr,RATE_100G);
            PMC_ASSERT(rc==PMC_SUCCESS,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        }
        break;

    default:
        rc = CBR_INT_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* cbr_int_channel_cfg_set */


/*******************************************************************************
* cbr_int_multi-lane_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures an interleaver Multi-lane.
*   See cbr_int_channel_cfg_set for a more detailed description.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   lane_rate           - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
*   lane_group_select   - lane group selection:
*                               For 10G channels valid values are 0..11;
*                               For 40G channels valid values are 0..2;
*                               For 100G channels valid values is 0
*
*   enable_lane         - enum,requested lane configuration value: 
*                               SET_BIT     : 1, enable multi-lane egress ports;
*                               RESET_BIT   : 0, disable multi-lane egress ports
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise CBR_DINT_ERR_INVALID_PARAMETER or CBR_INT_ERR_CFG_COLLISION is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_int_multilane_cfg_set( otu_frm_handle_t*otu_frm_handle,
                                            otu_frm_stream_rate_t lane_rate,
                                            UINT8 lane_group_select,
                                            otu_frm_bit_value_t enable_lane )

{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     mapped_40G_lane = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);

    /* 1 Lane for 100G, 3 lanes for 40G, 12 lanes for 10G,zero based */
    PMC_ASSERT(lane_group_select < MAX_10G_LANE_NUM,CBR_INT_ERR_INVALID_PARAMETERS,lane_group_select,0);

    switch (lane_rate)
    {
        /* 10G -> no multi-lane configuration register        */
        /* just update configuration storage                */
    case RATE_10G:
        /* Find the relevant 40G lane                   */
        /* 10G lanes 0..3  map to 40G lane 0            */
        /* 10G lanes 4..7  map to 40G lane 1            */
        /* 10G lanes 8..11 map to 40G lane 2            */
        mapped_40G_lane = lane_group_select / (MAX_10G_LANE_NUM/MAX_40G_LANE_NUM);

        if( SET_BIT == enable_lane )
        {
            /* check that 100G lane or the relevant 40G lanes or the requested 10G lane are not already configured */
            if((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G != 0) ||
               ((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G & (1<<mapped_40G_lane)) > 0) ||  /* 40G lane */
               ((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G & (1<<lane_group_select)) > 0 ) ) /* 10G lanes */
            {
                rc = CBR_INT_ERR_CFG_COLLISION;  /* error - disable other configuration before setting new one */
            }
            else
            {
                /* update the multi-lane configuration storage */
                SET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G, (1<<lane_group_select) );
            }
        }
        else
        {
            /* check that no 100G lane and no relevant 40G lanes are set and the requested 10G lane is configured */
            if( (0 == otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G ) &&
                (0 == (otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G & (1<<mapped_40G_lane)) ) &&  /* 40G lane */
                ((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G & (1<<lane_group_select)) > 0 ) ) /* 10G lanes */
            {
                /* update the multi-lane configuration storage */
                RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G, (1<<lane_group_select) );
            }
            else
            {
                rc = CBR_INT_ERR_INVALID_PARAMETERS;

            }
        }
        break;

    case RATE_40G:
        if( lane_group_select < MAX_40G_LANE_NUM )
        {
            if( SET_BIT == enable_lane )
            {
                /* check that 100G lane or the requested 40G or separate 10G lanes are not already configured */
                if((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G != 0) ||
                   ((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G & (1<<lane_group_select)) > 0) ||  /* 40G lane */
                   ((otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G & (MULTILANE_10G_40G_GRP0<<(lane_group_select*4)))> 0 ) ) /* 10G lanes */
                {
                    rc = CBR_INT_ERR_CFG_COLLISION;  /* error - disable other configuration before setting new one */
                }
                else
                {
                    SET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G, (MULTILANE_40G_GRP0<<lane_group_select));
                    SET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G ,(MULTILANE_10G_40G_GRP0<<(lane_group_select* 4)) );
                }
            }
            else   /* disable 40G lane */
            {
                /* check to see 40G lane exists before disabling it */
                if( (otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G & (1<<lane_group_select)) > 0 )
                {
                    RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G, (MULTILANE_40G_GRP0<<lane_group_select));
                    RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G ,(MULTILANE_10G_40G_GRP0<<(lane_group_select* 4)) );
                }
                else
                {
                    rc = CBR_INT_ERR_INVALID_PARAMETERS; /*error - can't disable non-existent configuration */
                }
            }
        }
        else
        {
            rc = CBR_INT_ERR_INVALID_PARAMETERS;
        }
        break;

    case RATE_100G:
        if( SET_BIT == enable_lane )
        {
            /* check to see if other configuration is already set */
            if(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G != 0 ||
               otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G  != 0 ||
               otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G  != 0 )
            {
                rc = CBR_INT_ERR_CFG_COLLISION;  /* error - disable other configuration before setting new one */
            }
            else
            {
                /* update the multi-lane configuration storage */
                SET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G, MULTILANE_100G);
                SET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G , MULTILANE_10G_100G);
            }
        }
        else
        {   /* check to see 100G exists before disabling it */
            if(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G != 0 )
            {
                /* update the multi-lane configuration storage */
                RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G, MULTILANE_100G);
                RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G , MULTILANE_10G_100G);
            }
            else
            {
                rc = CBR_INT_ERR_INVALID_PARAMETERS; /*error - can't disable non-existent configuration */
            }
        }
        break;

    default:
        rc = CBR_INT_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* cbr_int_multi-lane_cfg_set */


/*******************************************************************************
* cbr_int_channel_dsc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures an interleaver 10G lane channel descriptor.
*   See cbr_int_channel_cfg_set for a more detailed description.
* 
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*
*   channel_dscptr          - pointer to user channel descriptor structure
*
*   lane_rate               - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise CBR_DINT_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_int_channel_dsc_set( otu_frm_handle_t*otu_frm_handle,
                                          int_dint_channel_cfg_dsc_t*channel_dscptr,
                                          otu_frm_stream_rate_t lane_rate )

{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     ch_id_cfg_lane = 0;
    UINT8     ch_dsc_cfg_lane = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(channel_dscptr!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);

    switch(lane_rate)
    {
    case RATE_10G:
        ch_id_cfg_lane = channel_dscptr->channel_lane;
        ch_dsc_cfg_lane = channel_dscptr->channel_lane;
        otu_frm_field_OTU_FRM_10G_INTLV_EN_set(NULL,otu_frm_handle,channel_dscptr->channel_lane,channel_dscptr->channel_dsc.ch_enable);
        break;

    case RATE_40G:
        ch_id_cfg_lane = (channel_dscptr->channel_lane* 4 + 3);
        ch_dsc_cfg_lane = (channel_dscptr->channel_lane* 4);
        otu_frm_field_OTU_FRM_40G_INTLV_EN_set(NULL,otu_frm_handle,channel_dscptr->channel_lane,channel_dscptr->channel_dsc.ch_enable);
        break;

    case RATE_100G:
        ch_id_cfg_lane = 11;
        ch_dsc_cfg_lane = 0;
        otu_frm_field_OTU_FRM_100G_INTLV_EN_set(NULL,otu_frm_handle,channel_dscptr->channel_dsc.ch_enable);
        break;

    default:
        rc = CBR_INT_ERR_INVALID_PARAMETERS;
        break;
    }


    if( PMC_SUCCESS == rc )
    {
        /* Set the channel descriptor values to the requested channel lane */
        otu_frm_field_OTU_FRM_CH_ID_set(NULL,otu_frm_handle,ch_id_cfg_lane,channel_dscptr->channel_dsc.ch_id);

        channel_dscptr->channel_dsc.ch_id_lane = ch_id_cfg_lane;

        /* copy channel parameters to the handle configuration */
        PMC_MEMCPY(&otu_frm_handle->var.cbr_int_channel_id_cfg[ch_dsc_cfg_lane],
                   channel_dscptr,
                   sizeof(int_dint_channel_cfg_dsc_t));
    }

    PMC_RETURN(rc);
}/* cbr_int_channel_dsc_set */


/*******************************************************************************
* cbr_int_channel_reg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans interleaver 10G, 40G and 100G lanes registers
*   associated to a given stream.
*
* INPUTS:
*   otu_frm_handle          - pointer to OTU_FRM handle instance.
*   dsi_stream              - dsi stream identifier
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
PUBLIC void cbr_int_channel_reg_cleanup( otu_frm_handle_t          *otu_frm_handle,
                                         UINT32                      dsi_stream)

{
    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);

    /* disable 10G deinterleaver */
    otu_frm_field_OTU_FRM_10G_INTLV_EN_set(NULL,otu_frm_handle, dsi_stream, 0);

    RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G, (1<<dsi_stream));
    
    /* potential 100G channel: disable 100G deinterleaver */
    if (0 == dsi_stream)
    {
        otu_frm_field_OTU_FRM_100G_INTLV_EN_set(NULL, otu_frm_handle, 0);

        RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G, 0);
    }

    /* potential 40G channel: disable 40G deinterleaver */
    if (0 == (dsi_stream % 4))
    {
        otu_frm_field_OTU_FRM_40G_INTLV_EN_set(NULL,otu_frm_handle, dsi_stream / 4, 0);
        RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G, (MULTILANE_40G_GRP0 << (dsi_stream / 4)));
    }

    otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_stream].channel_lane = FREE_LANE;
    otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_stream].channel_dsc.ch_id_lane = FREE_LANE;

    PMC_RETURN();
}/* cbr_int_channel_reg_cleanup */


/*******************************************************************************
* cbr_int_lanes_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the relevant lanes per dsi_stream and rate.\n
*
*   The interleaver has 12 lanes of 10 Gbps all together. These lanes can be used as
*   separate 10G channels or aggregated as 40G or 100G lanes based on a predefined lane
*   provisioning into multi-lanes.\n
*
*   Multi-lane provisioning is as follows: \n
*   1. For 100G multi-lane - all channels 0..11 are allocated\n
*   2. For 40G multi-lanes:\n
*           10G lanes 0..3  map to 40G lane 0\n
*           10G lanes 4..7  map to 40G lane 1\n
*           10G lanes 8..11 map to 40G lane 2\n
*   3. For separate 10G lanes any choice of lanes is OK.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*   lane_rate           - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
*   channel_dscptr      - pointer to user channel descriptor structure
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise CBR_DINT_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_int_lanes_cfg_set( otu_frm_handle_t*otu_frm_handle,
                                        otu_frm_stream_rate_t lane_rate,
                                        int_dint_channel_cfg_dsc_t*channel_dscptr )

{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(channel_dscptr!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(channel_dscptr->channel_lane < MAX_10G_LANE_NUM,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,0);

    switch (lane_rate)
    {

    case RATE_10G:
        rc = cbr_int_channel_dsc_set(otu_frm_handle,channel_dscptr,RATE_10G);
        PMC_ASSERT(PMC_SUCCESS == rc ,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        break;

    case RATE_40G:
        /* Calculate start and end 10G lanes assigned to the requested 40G lane */
        /* 10G lanes 0..3  map to 40G lane 0                                    */
        /* 10G lanes 4..7  map to 40G lane 1                                    */
        /* 10G lanes 8..11 map to 40G lane 2                                    */
        rc = cbr_int_channel_dsc_set(otu_frm_handle, channel_dscptr,RATE_40G);
        PMC_ASSERT(PMC_SUCCESS == rc ,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        break;

    case RATE_100G:
        /* when setting a 100G channel we use all 12 10G lanes                  */
        /* 10G lanes 0..3  map to 40G lane 0                                    */
        /* 10G lanes 4..7  map to 40G lane 1                                    */
        /* 10G lanes 8..11 map to 40G lane 2                                    */
        rc = cbr_int_channel_dsc_set(otu_frm_handle, channel_dscptr,RATE_100G);
        PMC_ASSERT(rc==PMC_SUCCESS,CBR_INT_ERR_INVALID_PARAMETERS,channel_dscptr->channel_lane,rc);
        break;

    default:
        rc = CBR_INT_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* cbr_int_lanes_cfg_set */


/*******************************************************************************
* cbr_int_free_lanes
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions set all the lanes allocated to a dsi stream to FREE_LANE
*   status.
*
* INPUTS:
*   otu_frm_handle      - pointer to OTU_FRM handle instance.
*
*
*   dsi_stream          - unsigned, requested dsi stream to free lanes for:
*                               For 10G channels valid values are 0..11; 
*                               For 40G channels valid values are 0..2;
*                               For 100G channels valid values is 0
*
*   lane_rate           - enum, requested data rate:
*                             RATE_10G,
*                             RATE_40G,
*                             RATE_100G
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise CBR_DINT_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_int_free_lanes( otu_frm_handle_t*otu_frm_handle,
                                     UINT8 dsi_stream,
                                     otu_frm_stream_rate_t lane_rate )

{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     start_lane = 0;
    UINT8     end_lane   = 0;

    PMC_ENTRY();

    PMC_ASSERT(otu_frm_handle!=NULL,CBR_INT_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < MAX_10G_LANE_NUM,CBR_INT_ERR_INVALID_PARAMETERS,dsi_stream,0);

    switch (lane_rate)
    {

    case RATE_10G:
        otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_stream].channel_lane = FREE_LANE;
        otu_frm_handle->var.cbr_int_channel_id_cfg[dsi_stream].channel_dsc.ch_id_lane = FREE_LANE;
        RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G, (1<<dsi_stream));
        break;


    case RATE_40G:
        if(dsi_stream < MAX_40G_LANE_NUM )
        {
            /* Calculate start and end 10G lanes assigned to the requested 40G lane */
            /* 10G lanes 0..3  map to 40G lane 0                                    */
            /* 10G lanes 4..7  map to 40G lane 1                                    */
            /* 10G lanes 8..11 map to 40G lane 2                                    */
            start_lane = (dsi_stream* 4);
            end_lane   = (start_lane + 4);

            for(    ; start_lane < end_lane ; start_lane++ )
            {
                otu_frm_handle->var.cbr_int_channel_id_cfg[start_lane].channel_lane = FREE_LANE;
                otu_frm_handle->var.cbr_int_channel_id_cfg[start_lane].channel_dsc.ch_id_lane = FREE_LANE;
            }

            RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_40G, (MULTILANE_40G_GRP0<<dsi_stream) );
            RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G ,(MULTILANE_10G_40G_GRP0<<(dsi_stream* 4)));
        }
        else
        {
            rc = CBR_INT_ERR_INVALID_PARAMETERS;
        }

        break;


    case RATE_100G:
        /* when setting a 100G channel we use all 12 10G lanes                  */
        start_lane = 0;
        end_lane   = MAX_10G_LANE_NUM;

        for(    ; start_lane < end_lane ; start_lane++ )
        {
            otu_frm_handle->var.cbr_int_channel_id_cfg[start_lane].channel_lane = FREE_LANE;
            otu_frm_handle->var.cbr_int_channel_id_cfg[start_lane].channel_dsc.ch_id_lane = FREE_LANE;
        }

        RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_100G, MULTILANE_100G);
        RESET_MULTILANE(otu_frm_handle->var.cbr_int_multilane_cfg.multilane_10G , MULTILANE_10G_100G);
        break;

    default:
        rc = CBR_INT_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
} /* cbr_int_free_lanes */


/*
** Private Functions
*/

/*
** End of file
*/
