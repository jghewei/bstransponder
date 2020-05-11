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
*     The file provides API conversion function for internal usage. Those APIs
*     with pointer parameter will be included here.
*
*   NOTES:
*     These APIs are used internally.
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_adpt_pv_cvt.h"
#ifdef PMC_COMEXPRESS 
#include <pthread.h>
#endif

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
UINT8 adpt_buffer[ADPT_BUFFER_SIZE];

/*
** Local Functions
*/

/*
** Public Functions
*/


/*******************************************************************************
* adpt_cvt_odu_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure ODUk multiplexing/demultiplexing. The
*   maximum datapath for ODUk is two stages altogeter. before calling this
*   function, adpt_traffic_mode_cfg interface should be called beforehand.
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   oduk_type          - ODUk type.
*   mapping_type       - mapping type: AMP or GMP.
*   payload_type       - payload type: 1G25 or 2G5.
*   ts_mask0           - TS position: TS0 ~ TS31.
*   ts_mask1           - TS position: TS32 ~ TS63.
*   ts_mask2           - TS position: TS64 ~ TS79.
*   tp_enable          - TRUE: enable TP.
*                        FALSE: disable TP.
*   oduflex_rate       - For ODUFLEX(CBR), specify the client bit rate in bps. 
*                        For all the other LO ODU types, set to 0.  
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
PUBLIC PMC_ERROR adpt_cvt_odu_mux_cfg(UINT32 dev_id,
                                    UINT32 ho_chnl_id,
                                    UINT32 lo_chnl_id,
                                    digi_oduk_t oduk_type,
                                    digi_otn_mapping_type_t mapping_type,
                                    digi_odu_line_payload_t payload_type,
                                    UINT32 ts_mask0,
                                    UINT32 ts_mask1,
                                    UINT32 ts_mask2,
                                    BOOL tp_enable,
                                    DOUBLE oduflex_rate)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_odu_mux_t odu_mux;
    PMC_ENTRY();

    odu_mux.oduk_type     = oduk_type;
    odu_mux.mapping_type  = mapping_type;
    odu_mux.payload_type  = payload_type;
    odu_mux.otn_dir       = DIGI_OTN_DIR_RX_TX;
    odu_mux.tx_ts_mask[0] = ts_mask0;
    odu_mux.tx_ts_mask[1] = ts_mask1;
    odu_mux.tx_ts_mask[2] = ts_mask2;
    odu_mux.rx_ts_mask[0] = ts_mask0;
    odu_mux.rx_ts_mask[1] = ts_mask1;
    odu_mux.rx_ts_mask[2] = ts_mask2;
    odu_mux.tp_enable     = tp_enable;
    odu_mux.oduflex_rate  = oduflex_rate;
    odu_mux.self_ts_type  = (digi_odu_line_payload_t)adpt_buffer[0];

    rc = adpt_odu_mux_cfg(dev_id, ho_chnl_id, lo_chnl_id, odu_mux);

    PMC_RETURN(rc);
}/* adpt_cvt_odu_mux_cfg */


/*******************************************************************************
* adpt_cvt_odu_mux_asym_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure ODUk multiplexing/demultiplexing. The
*   maximum datapath for ODUk is two stages altogeter. before calling this
*   function, adpt_traffic_mode_cfg interface should be called beforehand.
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   oduk_type          - ODUk type.
*   mapping_type       - mapping type: AMP or GMP.
*   payload_type       - payload type: 1G25 or 2G5.
*   otn_dir            - OTN Mux/Demux direction: Rx, Tx or both.
*   tx_ts_mask0        - Tx TS position: TS0 ~ TS31.
*   tx_ts_mask1        - Tx TS position: TS32 ~ TS63.
*   tx_ts_mask2        - Tx TS position: TS64 ~ TS79.
*   rx_ts_mask0        - Rx TS position: TS0 ~ TS31.
*   rx_ts_mask1        - Rx TS position: TS32 ~ TS63.
*   rx_ts_mask2        - Rx TS position: TS64 ~ TS79.
*   tp_enable          - TRUE: enable TP.
*                        FALSE: disable TP.
*   oduflex_rate       - For ODUFLEX(CBR), specify the client bit rate in bps. 
*                        For all the other LO ODU types, set to 0.  
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
PUBLIC PMC_ERROR adpt_cvt_odu_mux_asym_cfg(UINT32 dev_id,
                                         UINT32 ho_chnl_id,
                                         UINT32 lo_chnl_id,
                                         digi_oduk_t oduk_type,
                                         digi_otn_mapping_type_t mapping_type,
                                         digi_odu_line_payload_t payload_type,
                                         digi_otn_dir_t otn_dir,
                                         UINT32 tx_ts_mask0,
                                         UINT32 tx_ts_mask1,
                                         UINT32 tx_ts_mask2,
                                         UINT32 rx_ts_mask0,
                                         UINT32 rx_ts_mask1,
                                         UINT32 rx_ts_mask2,
                                         BOOL tp_enable,
                                         DOUBLE oduflex_rate)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_odu_mux_t odu_mux;
    PMC_ENTRY();

    odu_mux.oduk_type     = oduk_type;
    odu_mux.mapping_type  = mapping_type;
    odu_mux.payload_type  = payload_type;
    odu_mux.otn_dir       = otn_dir;
    odu_mux.tx_ts_mask[0] = tx_ts_mask0;
    odu_mux.tx_ts_mask[1] = tx_ts_mask1;
    odu_mux.tx_ts_mask[2] = tx_ts_mask2;
    odu_mux.rx_ts_mask[0] = rx_ts_mask0;
    odu_mux.rx_ts_mask[1] = rx_ts_mask1;
    odu_mux.rx_ts_mask[2] = rx_ts_mask2;
    odu_mux.tp_enable     = tp_enable;
    odu_mux.oduflex_rate  = oduflex_rate;
    odu_mux.self_ts_type  = (digi_odu_line_payload_t)adpt_buffer[0];

    rc = adpt_odu_mux_cfg(dev_id, ho_chnl_id, lo_chnl_id, odu_mux);

    PMC_RETURN(rc);
}/* adpt_cvt_odu_mux_asym_cfg */


/*******************************************************************************
* adpt_cvt_enet_test_pkt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the Ethernet test packet generation of the provisioed channel.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   packet_size        - length in bytes.
*   interval           - 12 cycles is minimum transmission opportunity at 10G.
*   dest_addr_hi       - high-order 16 bits.
*   dest_addr_lo       - low-order 32 bits.
*   src_addr_hi        - high-order 16 bits.
*   src_addr_lo        - low-order 32 bits.
*   ether_type         - 16 bits for etherType.
*   outer_vlan         - outer VLAN (0 indicates no VLAN).
*   inner_vlan         - inner VLAN (0 indicates no VLAN).
*   payload            - payload repeating data pattern.
*   enable             - TRUE: enable test packet.
*                        FALSE: disable test packet.
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
PUBLIC PMC_ERROR adpt_cvt_enet_test_pkt_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT16 packet_size,
                                       UINT16 interval,
                                       UINT16 dest_addr_hi,
                                       UINT32 dest_addr_lo,
                                       UINT16 src_addr_hi,
                                       UINT32 src_addr_lo,
                                       UINT16 ether_type,
                                       UINT32 outer_vlan,
                                       UINT32 inner_vlan,
                                       UINT8 payload,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_test_packet_t test_pkt;
    PMC_ENTRY();

    test_pkt.packet_size = packet_size;
    test_pkt.interval = interval;
    test_pkt.dest_addr_hi = dest_addr_hi;
    test_pkt.dest_addr_lo = dest_addr_lo;
    test_pkt.src_addr_hi = src_addr_hi;
    test_pkt.src_addr_lo = src_addr_lo;
    test_pkt.ether_type = ether_type;
    test_pkt.outer_vlan = outer_vlan;
    test_pkt.inner_vlan = inner_vlan;
    test_pkt.payload = payload;

    rc = adpt_enet_test_pkt_set(dev_id, chnl_id, &test_pkt, enable);

    PMC_RETURN(rc);
}/* adpt_cvt_enet_test_pkt_set */


/*******************************************************************************
* adpt_buffer_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets the DIGI Adapter buffer.
*
* INPUTS:
*   None.
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
PUBLIC PMC_ERROR adpt_buffer_reset()
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_MEMSET((void*)&adpt_buffer[0], 0, sizeof(adpt_buffer));

    PMC_RETURN(rc);
}/* adpt_buffer_reset */


/*******************************************************************************
* adpt_buffer_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the DIGI Adapter buffer data.
*
* INPUTS:
*   pos                - buffer position.
*   value              - buffer value.
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
PUBLIC PMC_ERROR adpt_buffer_set(UINT32 pos, UINT32 value)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    if ((4 * pos) >= (ADPT_BUFFER_SIZE - 4))
    {
        return PMC_ERR_INVALID_PARAMETERS;
    }

    adpt_buffer[4*pos]   = value & 0xFF;
    adpt_buffer[4*pos+1] = (value >> 8) & 0xFF;
    adpt_buffer[4*pos+2] = (value >> 16) & 0xFF;
    adpt_buffer[4*pos+3] = (value >> 24) & 0xFF;

    PMC_RETURN(rc);
}/* adpt_buffer_set */


/*******************************************************************************
* adpt_cvt_gfp_tx_cmf_set
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
*   payload            - payload information.
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
PUBLIC PMC_ERROR adpt_cvt_gfp_tx_cmf_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 payload_len_type,
                                     UINT32 upi_value,
                                     BOOL   fcs_en,
                                     UINT64 payload)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cmf_one_shot_cmf_t cfm_data;
    PMC_ENTRY();

    cfm_data.cmf_payload[0] = payload & 0xFF;
    cfm_data.cmf_payload[1] = (payload >> 8) & 0xFF;
    cfm_data.cmf_payload[2] = (payload >> 16) & 0xFF;
    cfm_data.cmf_payload[3] = (payload >> 24) & 0xFF;
    cfm_data.cmf_payload[4] = (payload >> 32) & 0xFF;
    cfm_data.cmf_payload[5] = (payload >> 40) & 0xFF;
    cfm_data.cmf_payload[6] = (payload >> 48) & 0xFF;
    cfm_data.cmf_payload[7] = (payload >> 56) & 0xFF;

    rc = adpt_gfp_tx_cmf_set(dev_id,
                             chnl_id,
                             payload_len_type,
                             upi_value,
                             fcs_en,
                             &cfm_data);

    PMC_RETURN(rc);
}/* adpt_cvt_gfp_tx_cmf_set */


/*******************************************************************************
* adpt_cvt_gfp_tx_ehec_en_set
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
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_gfp_tx_ehec_en_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT32 ehec_len,
                                         BOOL ehec_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 ehec_buffer[16];
    UINT32 i = 0;
    PMC_ENTRY();

    for (i = 0; i < 16; i++)
    {
        ehec_buffer[i] = adpt_buffer[4*i] + (adpt_buffer[4*i+1] << 8)
                         + (adpt_buffer[4*i+2] << 16) + (adpt_buffer[4*i+3] << 24);
    }
    rc = adpt_gfp_tx_ehec_en_set(dev_id, 
                                 chnl_id, 
                                 ehec_len, 
                                 &ehec_buffer[0],
                                 ehec_enable);

    PMC_RETURN(rc);
}/* adpt_cvt_gfp_tx_ehec_en_set */


/*******************************************************************************
* adpt_cvt_otn_tx_tti_set
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
PUBLIC PMC_ERROR adpt_cvt_otn_tx_tti_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_otn_tx_tti_set(dev_id, chnl_id, seg_id, location, &adpt_buffer[0]);

    PMC_RETURN(rc);
}/* adpt_cvt_otn_tx_tti_set */


/*******************************************************************************
* adpt_cvt_otn_exp_tti_set
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
PUBLIC PMC_ERROR adpt_cvt_otn_exp_tti_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_otn_exp_tti_set(dev_id, chnl_id, seg_id, location, &adpt_buffer[0]);

    PMC_RETURN(rc);
}/* adpt_cvt_otn_exp_tti_set */


/*******************************************************************************
* adpt_cvt_otn_tx_ftfl_set
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
PUBLIC PMC_ERROR adpt_cvt_otn_tx_ftfl_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_otn_tx_ftfl_set(dev_id, chnl_id, location, &adpt_buffer[0]);

    PMC_RETURN(rc);
}/* adpt_cvt_otn_tx_ftfl_set */


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
*             Line side(Stage 1/2/3A)       System side(Stage 3B)
*   -->            Sink Pos                     Source Pos      -->  ODUk SW 
*   <--            Source Pos                   Sink Pos        <--  ODUk SW
*
*   Please note that the priority of any TCMi shouldn't be the same. Otherwise,
*   it will return failure.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_otn_tcm_pri_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_loc_t location,
                                      digi_otn_pos_t position)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_tcm_pri_t tcm_pri[7];
    PMC_ENTRY();

    tcm_pri[0] = (digi_tcm_pri_t)adpt_buffer[0];
    tcm_pri[1] = (digi_tcm_pri_t)adpt_buffer[1];
    tcm_pri[2] = (digi_tcm_pri_t)adpt_buffer[2];
    tcm_pri[3] = (digi_tcm_pri_t)adpt_buffer[3];
    tcm_pri[4] = (digi_tcm_pri_t)adpt_buffer[4];
    tcm_pri[5] = (digi_tcm_pri_t)adpt_buffer[5];
    tcm_pri[6] = (digi_tcm_pri_t)adpt_buffer[6];

    rc = adpt_otn_tcm_pri_set(dev_id, chnl_id, location, position, &tcm_pri[0]);

    PMC_RETURN(rc);
} /* adpt_cvt_otn_tcm_pri_set */


/*******************************************************************************
* adpt_cvt_sdh_exp_tti_set
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
*   direction          - traffic direction: Tx, Rx.
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
PUBLIC PMC_ERROR adpt_cvt_sdh_exp_tti_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t direction)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_sdh_exp_tti_set(dev_id, chnl_id, direction, &adpt_buffer[0]);

    PMC_RETURN(rc);
}/* adpt_cvt_sdh_exp_tti_set */


/*******************************************************************************
* adpt_cvt_pmon_int_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the PMON interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   otn_i              - TRUE: enable OTN PMON interrupt.
*                        FALSE: disable OTN PMON interrupt.
*   misc_i             - TRUE: enable other PMON interrupt.
*                        FALSE: disable other PMON interrupt.
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
PUBLIC PMC_ERROR adpt_cvt_pmon_int_set(UINT32 dev_id, BOOL otn_i, BOOL misc_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_pmon_int_t int_table;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, otn_i, misc_i);

    int_table.otn_i  = otn_i;
    int_table.misc_i = misc_i;

    rc = adpt_pmon_int_set(dev_id, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_pmon_int_set */


/*******************************************************************************
* adpt_cvt_serdes_c8_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for C8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane to configure (0-7).
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_serdes_c8_values_set(UINT32 dev_id, UINT32 lane_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    serdes_c8_adapt_objects_t c8_values;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, lane_id);

    c8_values.dp_ffe_a      = adpt_buffer[0] + (adpt_buffer[1] << 8)
                              + (adpt_buffer[2] << 16) + (adpt_buffer[3] << 24);
    c8_values.target_amp    = adpt_buffer[4] + (adpt_buffer[5] << 8)
                              + (adpt_buffer[6] << 16) + (adpt_buffer[7] << 24);
    c8_values.adc_map_scale = adpt_buffer[8] + (adpt_buffer[9] << 8)
                              + (adpt_buffer[10] << 16) + (adpt_buffer[11] << 24);

    rc = adpt_serdes_c8_values_set(dev_id, lane_id, &c8_values);

    PMC_RETURN(rc);
}/* adpt_cvt_serdes_c8_values_set */


/*******************************************************************************
* adpt_cvt_serdes_t8_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for T8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - Lane to configure (0-39).
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_serdes_t8_values_set(UINT32 dev_id, UINT32 lane_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    serdes_t8_adapt_objects_t t8_values;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, lane_id);

    t8_values.dp_ffe_a   = adpt_buffer[0] + (adpt_buffer[1] << 8)
                           + (adpt_buffer[2] << 16) + (adpt_buffer[3] << 24);
    t8_values.dp_ffe_b   = adpt_buffer[4] + (adpt_buffer[5] << 8)
                           + (adpt_buffer[6] << 16) + (adpt_buffer[7] << 24);
    t8_values.dp_ffe_c   = adpt_buffer[8] + (adpt_buffer[9] << 8)
                           + (adpt_buffer[10] << 16) + (adpt_buffer[11] << 24);
    t8_values.tr_ffe_a   = adpt_buffer[12] + (adpt_buffer[13] << 8)
                           + (adpt_buffer[14] << 16) + (adpt_buffer[15] << 24);
    t8_values.tr_ffe_b   = adpt_buffer[16] + (adpt_buffer[17] << 8)
                           + (adpt_buffer[18] << 16) + (adpt_buffer[19] << 24);
    t8_values.dfe_coef0  = adpt_buffer[20] + (adpt_buffer[21] << 8)
                           + (adpt_buffer[22] << 16) + (adpt_buffer[23] << 24);
    t8_values.dfe_coef1  = adpt_buffer[24] + (adpt_buffer[25] << 8)
                           + (adpt_buffer[26] << 16) + (adpt_buffer[27] << 24);
    t8_values.dfe_coef2  = adpt_buffer[28] + (adpt_buffer[29] << 8)
                           + (adpt_buffer[30] << 16) + (adpt_buffer[31] << 24);
    t8_values.dfe_coef3  = adpt_buffer[32] + (adpt_buffer[33] << 8)
                           + (adpt_buffer[34] << 16) + (adpt_buffer[35] << 24);
    t8_values.dfe_coef4  = adpt_buffer[36] + (adpt_buffer[37] << 8)
                           + (adpt_buffer[38] << 16) + (adpt_buffer[39] << 24);
    t8_values.dfe_coef5  = adpt_buffer[40] + (adpt_buffer[41] << 8)
                           + (adpt_buffer[42] << 16) + (adpt_buffer[43] << 24);
    t8_values.pga_gain   = adpt_buffer[44] + (adpt_buffer[45] << 8)
                           + (adpt_buffer[46] << 16) + (adpt_buffer[47] << 24);
    t8_values.pga_offset = adpt_buffer[48] + (adpt_buffer[49] << 8)
                           + (adpt_buffer[50] << 16) + (adpt_buffer[51] << 24);
    t8_values.target_amp = adpt_buffer[52] + (adpt_buffer[53] << 8)
                           + (adpt_buffer[54] << 16) + (adpt_buffer[55] << 24);
    t8_values.adc_map_scale = adpt_buffer[56] + (adpt_buffer[57] << 8)
                              + (adpt_buffer[58] << 16) + (adpt_buffer[59] << 24);

    rc = adpt_serdes_t8_values_set(dev_id, lane_id, &t8_values);

    PMC_RETURN(rc);
}/* adpt_cvt_serdes_t8_values_set */


/*******************************************************************************
* adpt_cvt_serdes_s16_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for S16 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   if_type            - S16 serdes interface type. 
*   lane_id            - Lane to configure (0:11 for Line S16,
*                        0:15 for System S16).
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_serdes_s16_values_set(UINT32 dev_id,  
                                            hostmsg_s16_intf_type_t if_type,
                                            UINT32 lane_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    serdes_s16_adapt_objects_t s16_values;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, if_type, lane_id);

    s16_values.dp_ffe_a   = adpt_buffer[0] + (adpt_buffer[1] << 8)
                            + (adpt_buffer[2] << 16) + (adpt_buffer[3] << 24);
    s16_values.dp_ffe_b   = adpt_buffer[4] + (adpt_buffer[5] << 8)
                            + (adpt_buffer[6] << 16) + (adpt_buffer[7] << 24);
    s16_values.dp_ffe_c   = adpt_buffer[8] + (adpt_buffer[9] << 8)
                            + (adpt_buffer[10] << 16) + (adpt_buffer[11] << 24);
    s16_values.tr_ffe_a   = adpt_buffer[12] + (adpt_buffer[13] << 8)
                            + (adpt_buffer[14] << 16) + (adpt_buffer[15] << 24);
    s16_values.tr_ffe_b   = adpt_buffer[16] + (adpt_buffer[17] << 8)
                            + (adpt_buffer[18] << 16) + (adpt_buffer[19] << 24);
    s16_values.dfe_coef0  = adpt_buffer[20] + (adpt_buffer[21] << 8)
                            + (adpt_buffer[22] << 16) + (adpt_buffer[23] << 24);
    s16_values.dfe_coef1  = adpt_buffer[24] + (adpt_buffer[25] << 8)
                            + (adpt_buffer[26] << 16) + (adpt_buffer[27] << 24);
    s16_values.dfe_coef2  = adpt_buffer[28] + (adpt_buffer[29] << 8)
                            + (adpt_buffer[30] << 16) + (adpt_buffer[31] << 24);
    s16_values.pga_gain   = adpt_buffer[32] + (adpt_buffer[33] << 8)
                            + (adpt_buffer[34] << 16) + (adpt_buffer[35] << 24);
    s16_values.pga_offset = adpt_buffer[36] + (adpt_buffer[37] << 8)
                            + (adpt_buffer[38] << 16) + (adpt_buffer[39] << 24);
    s16_values.target_amp = adpt_buffer[40] + (adpt_buffer[41] << 8)
                            + (adpt_buffer[42] << 16) + (adpt_buffer[43] << 24);
    s16_values.adc_map_scale = adpt_buffer[44] + (adpt_buffer[45] << 8)
                            + (adpt_buffer[46] << 16) + (adpt_buffer[47] << 24);

    rc = adpt_serdes_s16_values_set(dev_id,
                                    if_type,
                                    lane_id,
                                    &s16_values);

    PMC_RETURN(rc);
}/* adpt_cvt_serdes_s16_values_set */


/*******************************************************************************
* adpt_cvt_serdes_cfg_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually set serdes configuration.
*
* INPUTS:
*   dev_id             - device identity.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_serdes_cfg_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_serdes_cfg_t *serdes_cfg = NULL;
    PMC_ENTRY();

    serdes_cfg = (digi_serdes_cfg_t*)PMC_CALLOC(sizeof(digi_serdes_cfg_t));

    /* convert file */
    rc = adpt_serdes_cfg_file_parse(dev_id, (char*)"serdes.csv", serdes_cfg);

    /* set serdes configuration */
    if (PMC_SUCCESS == rc)
    {
        rc = adpt_serdes_cfg_set(dev_id, serdes_cfg);
    }

    PMC_FREE(&serdes_cfg);

    PMC_RETURN(rc);
}/* adpt_cvt_serdes_cfg_set */


/*******************************************************************************
* adpt_cvt_serdes_cfg_file_save
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Save serdes adapatation parameters in a CSV file in which each line defines:
*   Serdes Interface,Rate,Lane,dp_ffe_a,dp_ffe_b,dp_ffe_c,tr_ffe_a,tr_ffe_b,
*   dfe_coef0,dfe_coef1,dfe_coef2,dfe_coef3,dfe_coef4,dfe_coef5,pga_gain,
*   pga_offset,target_amp,adc_map_scale
*
*   CSV file first contains the colums header.
*
* INPUTS:
*   dev_id             - device identity.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_serdes_cfg_file_save(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = adpt_serdes_cfg_file_save(dev_id, "serdes.csv");

    PMC_RETURN(rc);
}/* adpt_cvt_serdes_cfg_file_save */


/*******************************************************************************
* adpt_cvt_enet_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the Ethernet interrupt based on channel level.
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
PUBLIC PMC_ERROR adpt_cvt_enet_int_en_set(UINT32 dev_id, UINT32 chnl_id, digi_port_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_int_en_t int_table;
    PMC_ENTRY();

    int_table.phy_los           = adpt_buffer[0];
    int_table.rx_loc_fault      = adpt_buffer[1];
    int_table.rx_rem_fault      = adpt_buffer[2];
    int_table.hi_ber            = adpt_buffer[3];
    int_table.block_lock        = adpt_buffer[4];
    int_table.align_status      = adpt_buffer[5];
    int_table.loam              = adpt_buffer[6];

    rc = adpt_enet_int_en_set(dev_id, chnl_id, dir, &int_table);
    
    PMC_RETURN(rc);
} /* adpt_cvt_enet_int_en_set */


/*******************************************************************************
* adpt_cvt_otn_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the OTN interrupt based on channel level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity. 
*                        Available for PM(including SM) and TCMi
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
PUBLIC PMC_ERROR adpt_cvt_otn_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_otn_int_en_t int_table;
    PMC_ENTRY();

    int_table.dlof    = adpt_buffer[0];
    int_table.dlom    = adpt_buffer[1];
    int_table.dlck    = adpt_buffer[2];
    int_table.doci    = adpt_buffer[3];
    int_table.dais    = adpt_buffer[4];
    int_table.dtim    = adpt_buffer[5];
    int_table.ddeg    = adpt_buffer[6];
    int_table.dedeg   = adpt_buffer[7];    
    int_table.dbdi    = adpt_buffer[8];
    int_table.oof     = adpt_buffer[9];
    int_table.dloflom = adpt_buffer[10];
    int_table.dbiae   = adpt_buffer[11];
    int_table.dltc    = adpt_buffer[12];
    int_table.diae    = adpt_buffer[13];
    int_table.atsf    = adpt_buffer[14];
    int_table.atsd    = adpt_buffer[15];
    int_table.abdi    = adpt_buffer[16];
    int_table.aais    = adpt_buffer[17];
    int_table.dplm    = adpt_buffer[18];
    int_table.dloomfi = adpt_buffer[19];
    int_table.dmsim   = adpt_buffer[20];
    int_table.dcsf    = adpt_buffer[21];
    int_table.oom_im  = adpt_buffer[22];
    int_table.ac_aps  = adpt_buffer[23];

    rc = adpt_otn_int_en_set(dev_id, chnl_id, seg_id, location, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_otn_int_en_set */


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
PUBLIC PMC_ERROR adpt_cvt_mld_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     UINT32 lane_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_mld_int_en_t int_table;
    PMC_ENTRY();

    int_table.dlos     = adpt_buffer[0];
    int_table.doof     = adpt_buffer[1];
    int_table.dotuais  = adpt_buffer[2];
    int_table.dloflane = adpt_buffer[3];
    int_table.door     = adpt_buffer[4];
    int_table.dlor     = adpt_buffer[5];
    int_table.doom     = adpt_buffer[6];

    rc = adpt_mld_int_en_set(dev_id, chnl_id, lane_id, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_mld_int_en_set */


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
PUBLIC PMC_ERROR adpt_cvt_gfp_int_en_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_gfp_int_en_t int_table;
    PMC_ENTRY();

    int_table.dlfd  = adpt_buffer[0];
    int_table.dexm  = adpt_buffer[1];
    int_table.dupm  = adpt_buffer[2];
    int_table.dcsf  = adpt_buffer[3];
    int_table.didle = adpt_buffer[4];

    rc = adpt_gfp_int_en_set(dev_id, chnl_id, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_gfp_int_en_set */


/*******************************************************************************
* adpt_cvt_sdh_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the SDH/SONET interrupt based on channel level.
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
PUBLIC PMC_ERROR adpt_cvt_sdh_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_sdh_int_en_t int_table;
    PMC_ENTRY();

    int_table.dlos    = adpt_buffer[0];
    int_table.dgais   = adpt_buffer[1];
    int_table.dlof    = adpt_buffer[2];
    int_table.doof    = adpt_buffer[3];
    int_table.dtim    = adpt_buffer[4];
    int_table.dtiu    = adpt_buffer[5];
    int_table.dms_ais = adpt_buffer[6];
    int_table.dms_rdi = adpt_buffer[7];    

    rc = adpt_sdh_int_en_set(dev_id, chnl_id, dir, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_sdh_int_en_set */


/*******************************************************************************
* adpt_cvt_fc1200_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the FC1200 interrupt based on channel level.
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
PUBLIC PMC_ERROR adpt_cvt_fc1200_int_en_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_fc1200_int_en_t int_table;
    PMC_ENTRY();

    int_table.drf        = adpt_buffer[0];
    int_table.dlf        = adpt_buffer[1];
    int_table.dhigh_ber  = adpt_buffer[2];
    int_table.dloss_sync = adpt_buffer[3];
    int_table.dlos       = adpt_buffer[4];

    rc = adpt_fc1200_int_en_set(dev_id, chnl_id, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_fc1200_int_en_set */


/*******************************************************************************
* adpt_cvt_fc800_int_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the FC800 interrupt based on channel level.
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
PUBLIC PMC_ERROR adpt_cvt_fc800_int_en_set(UINT32 dev_id, 
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_fc800_int_en_t int_table;
    PMC_ENTRY();

    int_table.ddisp_inv = adpt_buffer[0];
    int_table.dxdet     = adpt_buffer[1];
    int_table.dlolb     = adpt_buffer[2];
    int_table.dasd      = adpt_buffer[3];
    int_table.dsync     = adpt_buffer[4];
    int_table.dlcv      = adpt_buffer[5];

    rc = adpt_fc800_int_en_set(dev_id, chnl_id, dir, &int_table);

    PMC_RETURN(rc);
} /* adpt_cvt_fc800_int_en_set */


/*******************************************************************************
* adpt_cvt_otn_ohp_en_set
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
* INPUTS:
*   dev_id             - device identity.
*   ohp                - OHP (ODU_OHP_1, ODU_OHP_2,ODU_OHP_3)
*   port_a_framer      - OHP port A framer (see digi_ohp_port_framer_t)
*   port_a_src_addr    - OHP Port A src address
*   port_a_dest_addr   - OHP Port A dest address
*   port_b_framer      - OHP port B framer (see digi_ohp_port_framer_t)
*   port_b_src_addr    - OHP Port B src address
*   port_b_dest_addr   - OHP Port b dest address
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
PUBLIC PMC_ERROR adpt_cvt_otn_ohp_en_set(UINT32 dev_id, 
                                      odu_ohp_inst_t ohp,
                                      odu_ohp_port_framer_t port_a_framer,
                                      UINT32 port_a_src_addr,
                                      UINT32 port_a_dest_addr,
                                      odu_ohp_port_framer_t port_b_framer,
                                      UINT32 port_b_src_addr,
                                      UINT32 port_b_dest_addr,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_ohp_cfg_t ohp_cfg;
    PMC_ENTRY();

    PMC_MEMSET((void*)&ohp_cfg, 0, sizeof(ohp_cfg));
    ohp_cfg.ohp              = ohp;
    ohp_cfg.port_a_framer    = port_a_framer;
    ohp_cfg.port_a_src_addr  = port_a_src_addr;
    ohp_cfg.port_a_dest_addr = port_a_dest_addr;
    ohp_cfg.port_b_framer    = port_b_framer;
    ohp_cfg.port_b_src_addr  = port_b_src_addr;
    ohp_cfg.port_b_dest_addr = port_b_dest_addr;


    rc = adpt_otn_ohp_en_set(dev_id, &ohp_cfg, enable);
    
    PMC_RETURN(rc);
} /* adpt_cvt_otn_ohp_en_set */


/*******************************************************************************
* adpt_cvt_di_rcp_en_set
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
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_di_rcp_en_set(UINT32 dev_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_di_rcp_cfg_t di_rcp_cfg;
    PMC_ENTRY();

    di_rcp_cfg.src_mac[0] = adpt_buffer[0];
    di_rcp_cfg.src_mac[1] = adpt_buffer[1];
    di_rcp_cfg.src_mac[2] = adpt_buffer[2];
    di_rcp_cfg.src_mac[3] = adpt_buffer[3];
    di_rcp_cfg.src_mac[4] = adpt_buffer[4];
    di_rcp_cfg.src_mac[5] = adpt_buffer[5];
    di_rcp_cfg.dest_mac[0] = adpt_buffer[6];
    di_rcp_cfg.dest_mac[1] = adpt_buffer[7];
    di_rcp_cfg.dest_mac[2] = adpt_buffer[8];
    di_rcp_cfg.dest_mac[3] = adpt_buffer[9];
    di_rcp_cfg.dest_mac[4] = adpt_buffer[10];
    di_rcp_cfg.dest_mac[5] = adpt_buffer[11];
    di_rcp_cfg.eth_etype = (UINT16)(adpt_buffer[13] << 8) | (UINT16)adpt_buffer[12];
    di_rcp_cfg.discard_empty_pdu = adpt_buffer[14];

    rc = adpt_di_rcp_en_set(dev_id, &di_rcp_cfg, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_di_rcp_en_set */


/*******************************************************************************
* adpt_cvt_otn_ri_rcp_extract_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures DI RCP extraction for an ODU framer channel. A RI_RCP PDU is 
*   assembled  with the following information for the specified channel:
*    - The BDI/BEI/BIAE bits from the TCMs and PM layers are extracted. 
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
*   dst_digi           - destination DIGI Identifier. User can specify unique 
*                        number as per number  of DIGI's on board.
*   dst_frm            - destination framer stage on destination DIGI.
*   dst_channel        - destination channel on destination DIGI.
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
PUBLIC PMC_ERROR adpt_cvt_otn_ri_rcp_extract_cfg(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_otn_seg_t seg_id,
                                           UINT8 dst_digi,
                                           UINT8 dst_frm,
                                           UINT8 dst_channel,
                                           BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_otn_odu_ri_rcp_dst_cfg_t ri_rcp_dst_cfg;
    PMC_ENTRY();

    ri_rcp_dst_cfg.dst_digi = dst_digi;
    ri_rcp_dst_cfg.dst_frm = dst_frm;
    ri_rcp_dst_cfg.dst_channel = dst_channel;
 
    rc = adpt_ri_rcp_extract_cfg(dev_id,
                                 chnl_id,
                                 seg_id,
                                 &ri_rcp_dst_cfg,
                                 enable);

    PMC_RETURN(rc);
} /* adpt_cvt_otn_ri_rcp_extract_cfg */




/*******************************************************************************
* adpt_cvt_ptp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the PTP mode of operation as Boundary
*   Clock or Transparent Clock, and related PTP parameters. The parameters
*   configured by this message correlate to the members of the defaultDS data
*   set described in IEEE 1588v2 standard.
*
* INPUTS:
*   dev_id             - device identity.
*   enet_line          - TRUE: PTP configuration in the line ENET.
*                        FALSE: PTP configuration in the system ENET.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_cfg(UINT32 dev_id, BOOL enet_line)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_config_t ptp_cfg;
    UINT32 index = 0;
    PMC_ENTRY();

    PMC_MEMSET((void*)&ptp_cfg, 0, sizeof(ptp_config_t));
    ptp_cfg.dev_type      = adpt_buffer[0];
    ptp_cfg.twoStepFlag   = adpt_buffer[1];
    ptp_cfg.clockIdentity = (UINT64)adpt_buffer[2] | ((UINT64)adpt_buffer[3] << 8) 
                            | ((UINT64)adpt_buffer[4] << 16) | ((UINT64)adpt_buffer[5] << 24)
                            | ((UINT64)adpt_buffer[6] << 32) | ((UINT64)adpt_buffer[7] << 40)
                            | ((UINT64)adpt_buffer[8] << 48) | ((UINT64)adpt_buffer[9] << 56);
    ptp_cfg.domainNumber      = adpt_buffer[10];
    ptp_cfg.transportProtocolBitmask = adpt_buffer[11];
    ptp_cfg.multicastEnable   = adpt_buffer[12];
    ptp_cfg.optMode           = adpt_buffer[13];
    ptp_cfg.vlanTagPresent    = adpt_buffer[14];
    ptp_cfg.cfcModeMask       = adpt_buffer[15] | (adpt_buffer[16] << 8);
    ptp_cfg.frq_synt_rate     = adpt_buffer[17];
    ptp_cfg.slave_mode        = adpt_buffer[18];
    for (index = 0; index < DIGI_ENET_LINE_CHNL_MAX; index++)
    {
        ptp_cfg.mstProtocol[index] = adpt_buffer[19+index];
    }

    rc = adpt_ptp_cfg(dev_id, &ptp_cfg, enet_line);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_cfg */


/*******************************************************************************
* adpt_cvt_ptp_ipv6_para_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP over UDP over ipv6 parameters.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_ipv6_para_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_ipv6_param_t ptp_ipv6;
    PMC_ENTRY();

    ptp_ipv6.trafficClass     = adpt_buffer[0];
    ptp_ipv6.flowLabel[0]     = adpt_buffer[1];
    ptp_ipv6.flowLabel[1]     = adpt_buffer[2];
    ptp_ipv6.flowLabel[2]     = adpt_buffer[3];
    ptp_ipv6.hopLimit         = adpt_buffer[4];
    ptp_ipv6.daScopeValue     = adpt_buffer[5];
    ptp_ipv6.sourceAddress[0] = adpt_buffer[6] | (adpt_buffer[7] << 8) 
                                | (adpt_buffer[8] << 16) | (adpt_buffer[9] << 24);
    ptp_ipv6.sourceAddress[1] = adpt_buffer[10] | (adpt_buffer[11] << 8) 
                                | (adpt_buffer[12] << 16) | (adpt_buffer[13] << 24);
    ptp_ipv6.sourceAddress[2] = adpt_buffer[14] | (adpt_buffer[15] << 8) 
                                | (adpt_buffer[16] << 16) | (adpt_buffer[17] << 24);
    ptp_ipv6.sourceAddress[3] = adpt_buffer[18] | (adpt_buffer[19] << 8) 
                                | (adpt_buffer[20] << 16) | (adpt_buffer[21] << 24);
    ptp_ipv6.mst_destAddress[0] = adpt_buffer[22] | (adpt_buffer[23] << 8) 
                                  | (adpt_buffer[24] << 16) | (adpt_buffer[25] << 24);
    ptp_ipv6.mst_destAddress[1] = adpt_buffer[26] | (adpt_buffer[27] << 8) 
                                  | (adpt_buffer[28] << 16) | (adpt_buffer[29] << 24);
    ptp_ipv6.mst_destAddress[2] = adpt_buffer[30] | (adpt_buffer[31] << 8) 
                                  | (adpt_buffer[32] << 16) | (adpt_buffer[33] << 24);
    ptp_ipv6.mst_destAddress[3] = adpt_buffer[34] | (adpt_buffer[35] << 8) 
                                  | (adpt_buffer[36] << 16) | (adpt_buffer[37] << 24);

    rc = adpt_ptp_ipv6_para_set(dev_id, &ptp_ipv6);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_ipv6_para_set */


/*******************************************************************************
* adpt_cvt_ptp_port_asymmetry_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure port's asymmetry and latency delays.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_port_asymmetry_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_port_asymmetry_t ptp_port;
    PMC_ENTRY();

    ptp_port.delayAsymmetry = adpt_buffer[0] | (adpt_buffer[1] << 8) 
                              | (adpt_buffer[2] << 16) | (adpt_buffer[3] << 24);
    ptp_port.cfc_ingress_latency_delay = adpt_buffer[4] | (adpt_buffer[5] << 8);
    ptp_port.cfc_egress_latency_delay  = adpt_buffer[6] | (adpt_buffer[7] << 8);
    ptp_port.tod_ingress_latency_delay = adpt_buffer[8] | (adpt_buffer[9] << 8);
    ptp_port.tod_egress_latency_delay  = adpt_buffer[10] | (adpt_buffer[11] << 8);

    rc = adpt_ptp_port_asymmetry_set(dev_id, chnl_id, &ptp_port);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_port_asymmetry_set */


/*******************************************************************************
* adpt_cvt_ptp_default_pro_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP Boundary Clock mode default properties.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_default_pro_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_default_properties_t default_pro;
    PMC_ENTRY();

    default_pro.clockClass              = adpt_buffer[0];
    default_pro.clockAccuracy           = adpt_buffer[1];
    default_pro.offsetScaledLogVariance = adpt_buffer[2] | (adpt_buffer[3] << 8) ;
    default_pro.priority1               = adpt_buffer[4];
    default_pro.priority2               = adpt_buffer[5];
    default_pro.slaveOnly               = adpt_buffer[6];

    rc = adpt_ptp_default_pro_set(dev_id, &default_pro);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_default_pro_set */


/*******************************************************************************
* adpt_cvt_ptp_time_pro_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP Boundary Clock mode time properties.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_time_pro_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_set_time_param_t time_pro;
    PMC_ENTRY();

    time_pro.currentUtcOffset      = adpt_buffer[0] | (adpt_buffer[1] << 8);
    time_pro.leap61                = adpt_buffer[2] & 0x01;
    time_pro.leap59                = (adpt_buffer[2] & 0x02) >> 1;
    time_pro.currentUtcOffsetValid = (adpt_buffer[2] & 0x04) >> 2;
    time_pro.ptpTimescale          = (adpt_buffer[2] & 0x08) >> 3;
    time_pro.timeTraceable         = (adpt_buffer[2] & 0x10) >> 4;
    time_pro.frequencyTraceable    = (adpt_buffer[2] & 0x20) >> 5;
    time_pro.timeSource            = adpt_buffer[3] ;

    rc = adpt_ptp_time_pro_set(dev_id, &time_pro);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_time_pro_set */


/*******************************************************************************
* adpt_cvt_ptp_acceptable_master_table_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure PTP Boundary Clock mode acceptable 
*   master clocks.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_acceptable_master_table_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_acceptable_master_clocks_t clock_para;
    ptp_acceptable_master_clocks_data_t clock_data;
    PMC_ENTRY();

    clock_para.enableTable   = adpt_buffer[0];
    clock_para.clearTable    = adpt_buffer[1];
    clock_para.numEntries    = adpt_buffer[2];
    clock_data.clockIdentity = (UINT64)adpt_buffer[3] | ((UINT64)adpt_buffer[4] << 8) 
                               | ((UINT64)adpt_buffer[5] << 16) | ((UINT64)adpt_buffer[6] << 24)
                               | ((UINT64)adpt_buffer[7] << 32) | ((UINT64)adpt_buffer[8] << 40)
                               | ((UINT64)adpt_buffer[9] << 48) | ((UINT64)adpt_buffer[10] << 56);
    clock_data.portNumber    = (UINT16)(adpt_buffer[11] | (adpt_buffer[12] << 8));

    rc = adpt_ptp_acceptable_master_table_set(dev_id, &clock_para, &clock_data);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_acceptable_master_table_set */


/*******************************************************************************
* adpt_cvt_ptp_start_initialize  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP state machine to the INITIALIZE state.
*
* INPUTS:
*   dev_id             - device identity.
*   out_vlan_tag       - outer VLAN tag.
*   in_vlan_tag        - inner VLAN tag.
*   ptpChnlBitmask     - PTP ENET channel bitmask
*                        10G rate: bits 0 to 11
*                        40G rate: bits 0, 4, 8
*                        100G rate: bit 0
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
PUBLIC PMC_ERROR adpt_cvt_ptp_start_initialize(UINT32 dev_id, 
                                       UINT16 out_vlan_tag,
                                       UINT16 in_vlan_tag,
                                       UINT16 ptpChnlBitmask)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 chnl_id[DIGI_ENET_CHNL_MAX];
    ptp_start_init_param_t start_init;
    PMC_ENTRY();

    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        chnl_id[index] = adpt_buffer[4*index] | (adpt_buffer[4*index+1] << 8)
                         | (adpt_buffer[4*index+2] << 16) | (adpt_buffer[4*index+3] << 24);
    }

    start_init.innerVlanTag   = in_vlan_tag;
    start_init.outerVlanTag   = out_vlan_tag;
    start_init.ptpChnlBitmask = ptpChnlBitmask;
    rc = adpt_ptp_start_initialize(dev_id, chnl_id, &start_init);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_start_initialize */


/*******************************************************************************
* adpt_cvt_ptp_parent_data_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set Boundary Clock mode parent data properties.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_parent_data_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_parent_data_t parent_data;
    UINT32 index = 0;
    PMC_ENTRY();

    parent_data.stepsRemoved = adpt_buffer[0] | (adpt_buffer[1] << 8); 
    parent_data.parentClockIdentity = (UINT64)adpt_buffer[2] | ((UINT64)adpt_buffer[3] << 8) 
                                      | ((UINT64)adpt_buffer[4] << 16) | ((UINT64)adpt_buffer[5] << 24)
                                      | ((UINT64)adpt_buffer[6] << 32) | ((UINT64)adpt_buffer[7] << 40)
                                      | ((UINT64)adpt_buffer[8] << 48) | ((UINT64)adpt_buffer[9] << 56); 
    parent_data.parentPortNumber         = adpt_buffer[10] | (adpt_buffer[11] << 8); 
    parent_data.grandmasterPriority1     = adpt_buffer[12]; 
    parent_data.grandmasterPriority2     = adpt_buffer[13]; 
    parent_data.grandmasterClockClass    = adpt_buffer[14]; 
    parent_data.grandmasterClockAccuracy = adpt_buffer[15]; 
    parent_data.grandmasterOffsetScaledLogVariance = adpt_buffer[16] | (adpt_buffer[17] << 8); 
    parent_data.grandmasterIdentity = (UINT64)adpt_buffer[18] | ((UINT64)adpt_buffer[19] << 8) 
                                      | ((UINT64)adpt_buffer[20] << 16) | ((UINT64)adpt_buffer[21] << 24)
                                      | ((UINT64)adpt_buffer[22] << 32) | ((UINT64)adpt_buffer[23] << 40)
                                      | ((UINT64)adpt_buffer[24] << 48) | ((UINT64)adpt_buffer[25] << 56);  
    for (index = 0; index < ENET_MAC_ADDR_SIZE; index++)
    {
        parent_data.unicastDA[index] = adpt_buffer[26+index]; 
    }

    rc = adpt_ptp_parent_data_set(dev_id, &parent_data);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_parent_data_set */


/*******************************************************************************
* adpt_cvt_ptp_port_data_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set Boundary Clock mode port data properties.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_port_data_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_port_data_t port_data;
    PMC_ENTRY();

    port_data.logMinDelayReqInterval  = adpt_buffer[0];
    port_data.logAnnounceInterval     = adpt_buffer[1];
    port_data.announceReceiptTimeout  = adpt_buffer[2];
    port_data.logSyncInterval         = adpt_buffer[3];
    port_data.delayMechanism          = adpt_buffer[4];
    port_data.logMinPdelayReqInterval = adpt_buffer[5];
    port_data.qualificationTimeoutN   = adpt_buffer[6];
    port_data.syncReceiptTimeout      = adpt_buffer[7];
    port_data.delayRespReceiptTimeout = adpt_buffer[8];

    rc = adpt_ptp_port_data_set(dev_id, &port_data);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_port_data_set */


/*******************************************************************************
* adpt_cvt_ptp_port_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set states of all Boundary Clock mode ports.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_port_state_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    port_state_t port_state;
    PMC_ENTRY();

    port_state.portState                  = adpt_buffer[0];
    port_state.ptsfStatus                 = adpt_buffer[1];
    port_state.announceReceiptTimeoutFlag = adpt_buffer[2];
    rc = adpt_ptp_port_state_set(dev_id, chnl_id, &port_state);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_port_state_set */


/*******************************************************************************
* adpt_cvt_ptp_unicast_master_para_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a function to specify the parameters for the unicast message 
*   negotiation.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_unicast_master_para_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_master_unicast_param_t unicast_para;
    PMC_ENTRY();

    unicast_para.logInterMessagePeriod_announce   = adpt_buffer[0];
    unicast_para.logInterMessagePeriod_sync       = adpt_buffer[1];
    unicast_para.logInterMessagePeriod_delay_resp = adpt_buffer[2];
    unicast_para.durationField_announce   = adpt_buffer[3] | (adpt_buffer[4] << 8) 
                                            | (adpt_buffer[5] << 16) | (adpt_buffer[6] << 24);
    unicast_para.durationField_sync       = adpt_buffer[7] | (adpt_buffer[8] << 8) 
                                            | (adpt_buffer[9] << 16) | (adpt_buffer[10] << 24);
    unicast_para.durationField_delay_resp = adpt_buffer[11] | (adpt_buffer[12] << 8) 
                                            | (adpt_buffer[13] << 16) | (adpt_buffer[14] << 24);

    rc = adpt_ptp_unicast_master_para_set(dev_id, &unicast_para);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_unicast_master_para_set */


/*******************************************************************************
* adpt_cvt_ptp_unicast_request_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to start the transmission of the 
*   REQUEST_UNICAST_TRANSMISSION TLV for a port in the LISTENING, PASSIVE, 
*   UNCALIBRATED or SLAVE state.
*   A port will resend the REQUEST_UNICAST_TRANSMISSION TLV two more times at 
*   one second interval if a GRANT_UNICST_TRANSMISSION TLV is not received.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_unicast_request_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_unicast_request_t request_para;
    PMC_ENTRY();

    request_para.messageType                      = adpt_buffer[0];
    request_para.logInterMessagePeriod_announce   = adpt_buffer[1];
    request_para.logInterMessagePeriod_sync       = adpt_buffer[2];
    request_para.logInterMessagePeriod_delay_resp = adpt_buffer[3];
    request_para.durationField_announce   = adpt_buffer[4] | (adpt_buffer[5] << 8) 
                                            | (adpt_buffer[6] << 16) | (adpt_buffer[7] << 24);
    request_para.durationField_sync       = adpt_buffer[8] | (adpt_buffer[9] << 8) 
                                            | (adpt_buffer[10] << 16) | (adpt_buffer[11] << 24);
    request_para.durationField_delay_resp = adpt_buffer[12] | (adpt_buffer[13] << 8) 
                                            | (adpt_buffer[14] << 16) | (adpt_buffer[15] << 24);
    request_para.continuous     = adpt_buffer[16];
    request_para.individual_tlv = adpt_buffer[17];

    rc = adpt_ptp_unicast_request_set(dev_id, chnl_id, &request_para);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_unicast_request_set */


/*******************************************************************************
* adpt_ptp_pass_thru_general_param_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set the general PTP pass-thru parameters.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_pass_thru_general_param_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_pass_thru_general_param_t ptp_para;
    UINT32 index = 0;
    PMC_ENTRY();

    ptp_para.vlanTci = adpt_buffer[0] | (adpt_buffer[1] << 8);
    for (index = 0; index < 6; index++)
    {
        ptp_para.externalHostMacAddr[index] = adpt_buffer[2+index];
    }
    for (index = 0; index < 6; index++)
    {
        ptp_para.mipsMacAddr[index] = adpt_buffer[8+index];
    }
    ptp_para.vlanPresent = adpt_buffer[14] | (adpt_buffer[15] << 8);

    rc = adpt_ptp_pass_thru_general_param_set(dev_id, &ptp_para);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_pass_thru_general_param_set */


/*******************************************************************************
* adpt_ptp_pass_thru_port_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set the port PTP pass-thru parameters.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_pass_thru_port_param_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_pass_thru_port_param_t ptp_para;
    PMC_ENTRY();

    ptp_para.msgInterface = (ptp_pass_thru_msg_interface_enum)adpt_buffer[0];
    ptp_para.peerMeanPathDelay = (INT64)((UINT64)adpt_buffer[1] | ((UINT64)adpt_buffer[2] << 8) 
                                 | ((UINT64)adpt_buffer[3] << 16) | ((UINT64)adpt_buffer[4] << 24)
                                 | ((UINT64)adpt_buffer[5] << 32) | ((UINT64)adpt_buffer[6] << 40)
                                 | ((UINT64)adpt_buffer[7] << 48) | ((UINT64)adpt_buffer[8] << 56));  
    PMC_MEMCPY(&(ptp_para.sync_ingress), &adpt_buffer[9], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.delay_req_ingress), &adpt_buffer[10], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.pdelay_req_ingress), &adpt_buffer[11], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.pdelay_resp_ingress), &adpt_buffer[12], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.sync_egress), &adpt_buffer[13], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.delay_req_egress), &adpt_buffer[14], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.pdelay_req_egress), &adpt_buffer[15], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.pdelay_resp_egress), &adpt_buffer[16], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.follow_up_ingress), &adpt_buffer[17], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.delay_resp_ingress), &adpt_buffer[18], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.pdelay_resp_follow_up_ingress), &adpt_buffer[19], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.announce_ingress), &adpt_buffer[20], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.signaling_ingress), &adpt_buffer[21], sizeof(UINT8));
    PMC_MEMCPY(&(ptp_para.management_ingress), &adpt_buffer[22], sizeof(UINT8));

    rc = adpt_ptp_pass_thru_port_param_set(dev_id, chnl_id, &ptp_para);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_pass_thru_port_param_set */


/*******************************************************************************
* adpt_cvt_ptp_tc_param_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the SyncE mode and SSM quality level.
*
* INPUTS:
*   dev_id             - device identity.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_tc_param_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    ptp_tc_param_t ptp_tc_para;
    UINT32 index = 0;
    UINT32 chnl_id[DIGI_ENET_CHNL_MAX];
    PMC_ENTRY();

    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        chnl_id[index] = adpt_buffer[4*index] | (adpt_buffer[4*index+1] << 8)
                         | (adpt_buffer[4*index+2] << 16) | (adpt_buffer[4*index+3] << 24);
    }

    ptp_tc_para.masterClockIdentity = ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX] | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+1] << 8) 
                                      | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+2] << 16) | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+3] << 24)
                                      | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+4] << 32) | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+5] << 40)
                                      | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+6] << 48) | ((UINT64)adpt_buffer[4*DIGI_ENET_CHNL_MAX+7] << 56));  
    ptp_tc_para.masterPortNumber = adpt_buffer[4*DIGI_ENET_CHNL_MAX+8] | (adpt_buffer[4*DIGI_ENET_CHNL_MAX+9] << 8);
    ptp_tc_para.enableSynt = adpt_buffer[4*DIGI_ENET_CHNL_MAX+10];
    ptp_tc_para.ipSaEgressCtrlMask = adpt_buffer[4*DIGI_ENET_CHNL_MAX+11] | (adpt_buffer[4*DIGI_ENET_CHNL_MAX+12] << 8);
    ptp_tc_para.ipDaIngressCtrlMask = adpt_buffer[4*DIGI_ENET_CHNL_MAX+13] | (adpt_buffer[4*DIGI_ENET_CHNL_MAX+14] << 8);
    ptp_tc_para.ptpChnlBitmask = adpt_buffer[4*DIGI_ENET_CHNL_MAX+15] | (adpt_buffer[4*DIGI_ENET_CHNL_MAX+16] << 8);

    rc = adpt_ptp_tc_param_set(dev_id, chnl_id, &ptp_tc_para);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_tc_param_set */


/*******************************************************************************
* adpt_cvt_ptp_tc_port_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure parameters for each port of a Transparent 
*   Clock.
*
* INPUTS:
*   dev_id             - device identity.
*   req_interval       - Log base 2 of the Pdelay_Req message transmission interval.
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
PUBLIC PMC_ERROR adpt_cvt_ptp_tc_port_set(UINT32 dev_id, UINT8 req_interval)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 chnl_id[DIGI_ENET_CHNL_MAX];
    PMC_ENTRY();

    for (index = 0; index < DIGI_ENET_CHNL_MAX; index++)
    {
        chnl_id[index] = adpt_buffer[4*index] | (adpt_buffer[4*index+1] << 8)
                         | (adpt_buffer[4*index+2] << 16) | (adpt_buffer[4*index+3] << 24);
    }

    rc = adpt_ptp_tc_port_set(dev_id, chnl_id, req_interval);

    PMC_RETURN(rc);
} /* adpt_cvt_ptp_tc_port_set */


/*******************************************************************************
* adpt_cvt_synce_ssm_cfg  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the SyncE mode and SSM quality level.
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
PUBLIC PMC_ERROR adpt_cvt_synce_ssm_cfg(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_synce_ch_cfg_t synce_ctrl;
    PMC_ENTRY();

    synce_ctrl.event_flag = adpt_buffer[0];
    synce_ctrl.mode       = adpt_buffer[1];
    synce_ctrl.ql_ssm     = adpt_buffer[2];
    synce_ctrl.mac_sa[0]  = adpt_buffer[3];
    synce_ctrl.mac_sa[1]  = adpt_buffer[4];
    synce_ctrl.mac_sa[2]  = adpt_buffer[5];
    synce_ctrl.mac_sa[3]  = adpt_buffer[6];
    synce_ctrl.mac_sa[4]  = adpt_buffer[7];
    synce_ctrl.mac_sa[5]  = adpt_buffer[8];

    rc = adpt_synce_ssm_cfg(dev_id, chnl_id, &synce_ctrl);

    PMC_RETURN(rc);
} /* adpt_cvt_synce_ssm_cfg */


/*******************************************************************************
* adpt_cvt_synce_tx_ssm_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to send an event ESMC PDU.
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
PUBLIC PMC_ERROR adpt_cvt_synce_tx_ssm_set(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_synce_ch_cfg_t synce_ctrl;
    PMC_ENTRY();

    synce_ctrl.event_flag = adpt_buffer[0];
    synce_ctrl.mode       = adpt_buffer[1];
    synce_ctrl.ql_ssm     = adpt_buffer[2];
    synce_ctrl.mac_sa[0]  = adpt_buffer[3];
    synce_ctrl.mac_sa[1]  = adpt_buffer[4];
    synce_ctrl.mac_sa[2]  = adpt_buffer[5];
    synce_ctrl.mac_sa[3]  = adpt_buffer[6];
    synce_ctrl.mac_sa[4]  = adpt_buffer[7];
    synce_ctrl.mac_sa[5]  = adpt_buffer[8];

    rc = adpt_synce_tx_ssm_set(dev_id, chnl_id, &synce_ctrl);

    PMC_RETURN(rc);
} /* adpt_cvt_synce_tx_ssm_set */


/*******************************************************************************
* adpt_cvt_ri_rcp_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable/disable to extract APS and insert the APS 
*   into RI_RCP.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   eth_mac_oui        - Organizationally Unique Identifier is inserted in the 3
*                        first bytes of the source MAC.
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
*   This is an example to configure RI_RCP fucntionality.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_ri_rcp_set(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_seg_t seg_id, 
                                 UINT32 eth_mac_oui,
                                 UINT8  eth_mac_id,
                                 UINT32 eth_etype,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_otn_odu_ri_rcp_dst_cfg_t ri_rcp_dst_cfg;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, 
                             &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);
    
    if (enable)
    {
        rc = adpt_ri_rcp_en_set(dev_id, eth_mac_oui, eth_mac_id, eth_etype, enable);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_dest_get(dev_id, chnl_id, &ri_rcp_dst_cfg);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_extract_cfg(dev_id, chnl_id, seg_id, &ri_rcp_dst_cfg, enable);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_mon_level_cfg(dev_id, chnl_id, seg_id);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_aps_inst_cfg(dev_id, chnl_id, TRUE);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_otn_bw_inst_set(dev_id, chnl_id, DIGI_OTN_LINE_SIDE, DIGI_SRC_RI_RCP);
    } 
    else /* disable RI_RCP */
    {
        rc = adpt_ri_rcp_dest_get(dev_id, chnl_id, &ri_rcp_dst_cfg);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_aps_inst_cfg(dev_id, chnl_id, FALSE);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_extract_cfg(dev_id, chnl_id, seg_id, &ri_rcp_dst_cfg, FALSE);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_ri_rcp_en_set(dev_id, eth_mac_oui, eth_mac_id, eth_etype, FALSE);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
}/* adpt_cvt_ri_rcp_set */


/*******************************************************************************
* adpt_cvt_di_rcp_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable/disable to extract defects from DI_RCP.
*
* INPUTS:
*   dev_id             - device identity.
*   src_mac            - source MAC address.
*   dest_mac           - destination MAC address.
*   eth_etype          - The Ethertype field of the Ethernet frame. The value must
*                        be higher than 0x0800 and not conflict with any of the
*                        standardized Ethertype values. 
*   pdu_mask           - the DI RCP PDUs to enable or disable. Use 
*                        digi_di_rcp_pdu_t bit masks to define which PDUs 
*                        you want to enable by or-ing  masks. 
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
*   This is an example to configure DI_RCP fucntionality.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_di_rcp_set(UINT32 dev_id, 
                                 UINT32 src_mac, 
                                 UINT32 dest_mac,
                                 UINT32 eth_etype,
                                 UINT32 pdu_mask,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    odu_di_rcp_cfg_t di_rcp_dst_cfg;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    di_rcp_dst_cfg.dest_mac[0] =  dest_mac & 0xff;
    di_rcp_dst_cfg.dest_mac[1] = (dest_mac >> 8) & 0xff;
    di_rcp_dst_cfg.dest_mac[2] = (dest_mac >> 16) & 0xff;
    di_rcp_dst_cfg.dest_mac[3] = (dest_mac >> 24) & 0xff;
    di_rcp_dst_cfg.dest_mac[4] = 0;
    di_rcp_dst_cfg.dest_mac[5] = 0;

    di_rcp_dst_cfg.src_mac[0] = src_mac & 0xff;
    di_rcp_dst_cfg.src_mac[1] = (src_mac >> 8) & 0xff;
    di_rcp_dst_cfg.src_mac[2] = (src_mac >> 16) & 0xff;
    di_rcp_dst_cfg.src_mac[3] = (src_mac >> 24) & 0xff;
    di_rcp_dst_cfg.src_mac[4] = 0;
    di_rcp_dst_cfg.src_mac[5] = 0;

    di_rcp_dst_cfg.discard_empty_pdu = TRUE;
    di_rcp_dst_cfg.eth_etype = eth_etype;
    
    if (enable) 
    {
        rc = adpt_di_rcp_en_set(dev_id, &di_rcp_dst_cfg, enable);
        ADPT_RETURN_VERIFY(rc);

        rc =  adpt_di_rcp_cfg(dev_id, pdu_mask, enable);
        ADPT_RETURN_VERIFY(rc);
    } 
    else /* disable DI_RCP */
    {
        rc =  adpt_di_rcp_cfg(dev_id, pdu_mask, enable);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_di_rcp_en_set(dev_id, &di_rcp_dst_cfg, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
}/* adpt_cvt_di_rcp_set */


/*******************************************************************************
* adpt_cvt_otn_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted OTN defects in sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_otn_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_seg_t seg_id,
                                digi_otn_loc_t location,
                                digi_otn_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_otn_defect_get(dev_id, chnl_id, seg_id, location, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_otn_defect_get */


/*******************************************************************************
* adpt_cvt_otn_mld_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted MLD layer defects status in sink position. The lane 
*   number is 20 for OTU4, and 4 for OTU3.  
*
*   Please note that this API is available under OTL interface. The following 
*   defect ID list is available.
*    DIGI_OTN_LOS,
*    DIGI_OTN_OOF,
*    DIGI_OTN_OTUAIS,
*    DIGI_OTN_LOFLANE,
*    DIGI_OTN_OOR,
*    DIGI_OTN_LOR,
*    DIGI_OTN_OOM,
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. OTU4: 20, OTU3: 4.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: MLD defect is raised.
*                        FALSE: MLD defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_otn_mld_get(UINT32 dev_id,
                              UINT32 chnl_id,
                              UINT32 lane_id,
                              digi_otn_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_otn_mld_get(dev_id, chnl_id, lane_id, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_otn_mld_get */


/*******************************************************************************
* adpt_cvt_enet_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet defect information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_enet_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_enet_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_enet_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_enet_defect_get */


/*******************************************************************************
* adpt_cvt_sdh_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted SONET/SDH defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_sdh_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_sdh_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_sdh_defect_get(dev_id, chnl_id, dir, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_sdh_defect_get */


/*******************************************************************************
* adpt_cvt_gfp_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted defects status of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_gfp_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_gfp_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_gfp_defect_get(dev_id, chnl_id, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_gfp_defect_get */


/*******************************************************************************
* adpt_cvt_fc1200_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted FC1200 defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_fc1200_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_fc1200_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_fc1200_defect_get(dev_id, chnl_id, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_fc1200_defect_get */


/*******************************************************************************
* adpt_cvt_fc800_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted FC800 defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL               - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL adpt_cvt_fc800_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_fc800_defect_t defect_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL defect_status = FALSE;

    rc = adpt_fc800_defect_get(dev_id, chnl_id, dir, defect_id, &defect_status);

    return defect_status; 
}/* adpt_cvt_fc800_defect_get */


/*******************************************************************************
* adpt_cvt_otn_cm_status_get
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
PUBLIC PMC_ERROR adpt_cvt_otn_cm_status_get(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL cm_out_limit_i = FALSE;
    UINT32 cm_value = 0;
    PMC_ENTRY();

    rc = adpt_otn_cm_status_get(dev_id, chnl_id, &cm_out_limit_i, &cm_value);
    ADPT_PRINT(" dev_id[%d] chnl_id[%d] cm_out_limit_i[%d] cm_value[%d]\r\n", dev_id, 
               chnl_id, cm_out_limit_i, cm_value);

    PMC_RETURN(rc);
}/* adpt_cvt_otn_cm_status_get */


/*******************************************************************************
* adpt_cvt_ghao_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables G.HAO configuration before performing any G.HAO operations.
*
*   For increase scenerio follow these steps:
*     -- adpt_ghao_en_set
*     -- adpt_ghao_lcr_size
*     -- [call sifd knife edge sequence here if SIFD is in the path]
*     -- adpt_ghao_lcr_knife_edge
*     -- adpt_ghao_bwr_setup
*     -- adpt_ghao_bwr_ramp_start
*     -- adpt_ghao_bwr_ramp_end
*
*   For decrease scenerio follow these steps:
*     -- adpt_ghao_en_set
*     -- adpt_ghao_bwr_setup
*     -- adpt_ghao_bwr_ramp_start
*     -- adpt_ghao_lcr_knife_edge
*     -- [call sifd knife edge sequence here if SIFD is in the path]
*     -- adpt_ghao_lcr_size
*     -- adpt_ghao_bwr_ramp_end
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   new_rx_ts_mask0    - new Rx tribslot mask: TS0 ~ TS31.
*   new_rx_ts_mask1    - new Rx tribslot mask: TS32 ~ TS63.
*   new_rx_ts_mask2    - new Rx tribslot mask: TS64 ~ TS79.
*   new_tx_ts_mask0    - new Tx tribslot mask: TS0 ~ TS31.
*   new_tx_ts_mask1    - new Tx tribslot mask: TS32 ~ TS63.
*   new_tx_ts_mask2    - new Tx tribslot mask: TS64 ~ TS79.
*   enable             - TRUE: enable G.HAO feature.
*                        FALSE: disable G.HAO feature.
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
PUBLIC PMC_ERROR adpt_cvt_ghao_en_set(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   UINT32 new_rx_ts_mask0,
                                   UINT32 new_rx_ts_mask1,
                                   UINT32 new_rx_ts_mask2,
                                   UINT32 new_tx_ts_mask0,
                                   UINT32 new_tx_ts_mask1,
                                   UINT32 new_tx_ts_mask2,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 new_rx_ts_mask[3];
    UINT32 new_tx_ts_mask[3];
    PMC_ENTRY();

    new_rx_ts_mask[0] = new_rx_ts_mask0;
    new_rx_ts_mask[1] = new_rx_ts_mask1;
    new_rx_ts_mask[2] = new_rx_ts_mask2;
    new_tx_ts_mask[0] = new_tx_ts_mask0;
    new_tx_ts_mask[1] = new_tx_ts_mask1;
    new_tx_ts_mask[2] = new_tx_ts_mask2;

    rc = adpt_ghao_en_set(dev_id, chnl_id, new_rx_ts_mask, new_tx_ts_mask, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_ghao_en_set */


/*******************************************************************************
* adpt_cvt_enet_mgmt_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of a specific type of packets 
*   from ENET LINE or ENET SYS input interface to management FEGE output interface.
*   Packet could be also filtered out by their VLAN tag.
*
*   Incoming packets VLAN ID is replaced by a destination VLAN ID when 
*   replace_vlan_id is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pkt_type           - packet type.
*   is_vlan_tagged     - TRUE:  packet is VLAN-tagged and its VLAN will be replaced.
*                        FALSE: packet isn't VLAN-tagged. 
*   vlan_type          - VLAN-tagged packet configuration: VLAN type.
*   vlan_pcp           - VLAN-tagged packet configuration: VLAN priority code point.
*   vlan_cfi           - VLAN-tagged packet configuration: VLAN drop eligible.
*   vlan_id            - VLAN-tagged packet configuration: VLAN ID.
*   vlan_id_repl       - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl_en    - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_enet_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_port_dir_t dir,
                                            util_global_pkt_type_t pkt_type,
                                            BOOL is_vlan_tagged,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            UINT16 vlan_id_repl,
                                            BOOL vlan_id_repl_en,
                                            BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    util_global_vlan_cfg_t vlan_src_cfg;
    PMC_ENTRY();

    vlan_src_cfg.vlan_type = vlan_type;
    vlan_src_cfg.vlan_pcp  = vlan_pcp;
    vlan_src_cfg.vlan_cfi  = vlan_cfi;
    vlan_src_cfg.vlan_id   = vlan_id;

    rc = adpt_enet_mgmt_pkt_fwd_cfg(dev_id, chnl_id, dir, pkt_type, is_vlan_tagged,
                                    &vlan_src_cfg, vlan_id_repl, vlan_id_repl_en, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_enet_mgmt_pkt_fwd_cfg */


/*******************************************************************************
* adpt_cvt_mgmt_enet_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to ENET LINE or ENET_SYS output interface. Packets are
*   filtered in by their VLAN ID.  
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   vlan_type          - VLAN-tagged packet configuration: VLAN type.
*   vlan_pcp           - VLAN-tagged packet configuration: VLAN priority code point.
*   vlan_cfi           - VLAN-tagged packet configuration: VLAN drop eligible.
*   vlan_id            - VLAN-tagged packet configuration: VLAN ID.
*   vlan_pri_ow        - TRUE:  priority bit in VLAN TCI is overwriten by zeroes.
*                        FALSE: keep priority bit in VLAN TCI unchange.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_mgmt_enet_pkt_fwd_cfg(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_port_dir_t dir,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            BOOL vlan_pri_ow,
                                            BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    util_global_vlan_cfg_t vlan_src_cfg;
    PMC_ENTRY();

    vlan_src_cfg.vlan_type = vlan_type;
    vlan_src_cfg.vlan_pcp  = vlan_pcp;
    vlan_src_cfg.vlan_cfi  = vlan_cfi;
    vlan_src_cfg.vlan_id   = vlan_id;

    rc = adpt_mgmt_enet_pkt_fwd_cfg(dev_id, chnl_id, dir, &vlan_src_cfg, 
                                    vlan_pri_ow, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_mgmt_enet_pkt_fwd_cfg */


/*******************************************************************************
* adpt_cvt_cpu_mgmt_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of packets from CPU
*   to management FEGE output interface.
*
*   Packets VLAN ID is replaced by a destination VLAN ID when replace_vlan_id
*   is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*   vlan_type          - VLAN-tagged packet configuration: VLAN type.
*   vlan_pcp           - VLAN-tagged packet configuration: VLAN priority code point.
*   vlan_cfi           - VLAN-tagged packet configuration: VLAN drop eligible.
*   vlan_id            - VLAN-tagged packet configuration: VLAN ID.
*   vlan_id_repl       - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl_en    - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_cpu_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            UINT16 vlan_id_repl,
                                            BOOL vlan_id_repl_en,
                                            BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    util_global_vlan_cfg_t vlan_src_cfg;
    PMC_ENTRY();

    vlan_src_cfg.vlan_type = vlan_type;
    vlan_src_cfg.vlan_pcp  = vlan_pcp;
    vlan_src_cfg.vlan_cfi  = vlan_cfi;
    vlan_src_cfg.vlan_id   = vlan_id;

    rc = adpt_cpu_mgmt_pkt_fwd_cfg(dev_id, &vlan_src_cfg, vlan_id_repl, 
                                   vlan_id_repl_en, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_cpu_mgmt_pkt_fwd_cfg */


/*******************************************************************************
* adpt_cvt_mgmt_cpu_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to CPU output interface. Packets are
*   filtered in by their VLAN ID.  
*
* INPUTS:
*   dev_id             - device identity.
*   vlan_type          - VLAN-tagged packet configuration: VLAN type.
*   vlan_pcp           - VLAN-tagged packet configuration: VLAN priority code point.
*   vlan_cfi           - VLAN-tagged packet configuration: VLAN drop eligible.
*   vlan_id            - VLAN-tagged packet configuration: VLAN ID.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_mgmt_cpu_pkt_fwd_cfg(UINT32 dev_id,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    util_global_vlan_cfg_t vlan_src_cfg;
    PMC_ENTRY();

    vlan_src_cfg.vlan_type = vlan_type;
    vlan_src_cfg.vlan_pcp  = vlan_pcp;
    vlan_src_cfg.vlan_cfi  = vlan_cfi;
    vlan_src_cfg.vlan_id   = vlan_id;

    rc = adpt_mgmt_cpu_pkt_fwd_cfg(dev_id, &vlan_src_cfg, enable);

    PMC_RETURN(rc);
} /* adpt_cvt_mgmt_cpu_pkt_fwd_cfg */


/*******************************************************************************
* adpt_cvt_oduksw_latency_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the latency from backplane in ODUk SW MPMO block. Units 3.215 
*   nsecond ticks.
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
PUBLIC PMC_ERROR adpt_cvt_oduksw_latency_get(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 dbg_latency = 0;
    UINT32 min_latency = 0;
    UINT32 max_latency = 0; 
    PMC_ENTRY();

    rc = adpt_oduksw_latency_get(dev_id, chnl_id, &dbg_latency, &min_latency, &max_latency);
    dbg_latency = (UINT32)(3.215 * dbg_latency);
    min_latency = (UINT32)(3.215 * min_latency);
    max_latency = (UINT32)(3.215 * max_latency);

    ADPT_PRINT("gdb_latency[%d] min_latency[%d] max_latency[%d] \r\n", dbg_latency, 
               min_latency, max_latency);

    PMC_RETURN(rc);
}


/*******************************************************************************
* adpt_cvt_enet_eclass_filter_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function applies a filter config to the eclass subsystem.
*   The function can be called mulitple times to keep adding more filters
*   to the eclass, provided there are adequate resources. The eclass filter is
*   applied per enet link.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mode               - filter config mode for ENET eclass.
*   target             - eclass filter routing destination.
*   encap              - eclass encapsulation.
*   dir                - eclass datapath direction.
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
PUBLIC PMC_ERROR adpt_cvt_enet_eclass_filter_set(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_eclass_mode_t mode,
                                          digi_enet_eclass_target_t target,
                                          digi_enet_eclass_encap_t encap,
                                          digi_enet_eclass_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_eclass_filter_config_t eclass_cfg;
    PMC_ENTRY();

    eclass_cfg.mode      = mode;
    eclass_cfg.target    = target;
    eclass_cfg.encap     = encap;
    eclass_cfg.direction = dir;
    eclass_cfg.param_1   = 0;
    rc = adpt_enet_eclass_filter_set(dev_id, chnl_id, eclass_cfg);

    PMC_RETURN(rc);
} /* adpt_cvt_enet_eclass_filter_set */


#ifdef PMC_COMEXPRESS
/*******************************************************************************
* adpt_cvt_thread_cb_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function allows for users to call personal APIs in the callback created
*   through the adpt_cvt_thread_create().
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   thread_type        - thread type to identify different threads.
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
PUBLIC PMC_ERROR adpt_cvt_thread_cb_handle(UINT32 dev_id, 
                                           UINT32 chnl_id, 
                                           UINT32 thread_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* indentify different threads */
    switch (thread_type)
    {
        case 0:
            break;

        default:
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cvt_thread_cb_handle */


/*******************************************************************************
* adpt_cvt_thread_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a polling thread to poll either user defined callback or 
*   input callback.
*
* INPUTS:
*   *arg               - thread argument pointer.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   void *
*
* NOTES:
*
*******************************************************************************/
PUBLIC void* adpt_cvt_thread_poll(void *arg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    polling_para_t *para_p = (polling_para_t *)arg;
    polling_para_t para;
    PMC_ERROR (*func)(UINT32, UINT32);
    UINT32 thread_type;
    PMC_ENTRY();

    if (NULL == arg)
    {
        ADPT_PRINT("Invalid parameter.\r\n");
        PMC_RETURN(NULL);
    }

    para.func_p   = para_p->func_p;
    para.dev_id   = para_p->dev_id;
    para.chnl_id  = para_p->chnl_id;
    para.usleep_t = para_p->usleep_t;

    /* user self defined callback */
    if (para.func_p < ADPT_MAX_USER_DEFINED_THREADS)
    {
        thread_type = (UINT32)(para.func_p);
        while (1)
        {
            rc = adpt_cvt_thread_cb_handle(para.dev_id, para.chnl_id, thread_type);
            usleep(para.usleep_t);
        }
    }
    /* input callback */
    else
    {
        /* use memcpy since UINT64 can't be set to a pointer directly */
        PMC_MEMCPY((void*)&func, &(para.func_p), sizeof(func));

        while (1)
        {
            rc = (*func)(para.dev_id, para.chnl_id);
            usleep(para.usleep_t);
        }
    }

    PMC_RETURN(NULL);
} /* adpt_cvt_thread_poll */


/*******************************************************************************
* adpt_cvt_thread_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is to create a thread for polling a specified function or user
*   defined function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   func_p             - (func_p < 100): thread type to identify different threads.
*                        (func_p >= 100): function address retrieved through 
*                        "p <function_name>" in the GDB environment.
*   usleep_t           - polling task's sleep time, in count of microsecond(us)
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
*   There are two kinds of methods to create a thread to poll an expected function.
*
*   1. User defined functions: create a thread to poll user defined functions
*   written in the adpt_cvt_thread_cb_handle().
*
*   Procedure:
*   A. Add expected APIs in the adpt_cvt_thread_cb_handle() with a specific thread
*   type(thread_type) to identify different threads.
*   B. After compiling completely, call the adpt_cvt_thread_create() to create 
*   a thread with necessary arguments. Please note that func_p should be in line 
*   with thread_type defined in the adpt_cvt_thread_cb_handle(). 
*
*   2. Input function: create a thread to poll an input function. Please
*   note only the adpt_xxx(dev_id, chnl_id) prototype can be called. This special
*   case is usually used for alarm or PMON polling, for example, adpt_dump_otn_alm().
*
*   Procedure:
*   A. Enter GDB and type "p adpt_xxx" to get the logical address of this API.
*   B. In the tkcon, call the adpt_cvt_thread_create() to create a thread with 
*   necessary arguments. Please note that func_p should be the API address. 
*   Otherwise, a crash will likely happen.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cvt_thread_create(UINT32 dev_id, 
                                        UINT32 chnl_id,
                                        UINT64 func_p,
                                        UINT32 usleep_t)
{
    pthread_t polling_thread;
    pthread_attr_t polling_thread_attr;
    polling_para_t polling_thread_para;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    polling_thread_para.func_p    = func_p;
    polling_thread_para.dev_id    = dev_id;
    polling_thread_para.chnl_id   = chnl_id;
    polling_thread_para.usleep_t  = usleep_t;

    /* 10ms at minimal */
    if (polling_thread_para.usleep_t < 10000)
    {
        /* 100ms by default */
        polling_thread_para.usleep_t = 100000;
    }

    pthread_attr_init(&polling_thread_attr);
    pthread_attr_setdetachstate(&polling_thread_attr, PTHREAD_CREATE_DETACHED);

    /* create thread */
    rc = pthread_create(&polling_thread, 
                        &polling_thread_attr, 
                        adpt_cvt_thread_poll, 
                        &(polling_thread_para));
    if (PMC_SUCCESS != rc)
    {
         ADPT_PRINT("ERROR: return code from pthread_create() is %d\r\n", rc);
    }
    pthread_attr_destroy(&polling_thread_attr);

    /* wait 1 second for thread parameter copy */
    sleep(1);

    PMC_RETURN(rc);
} /* adpt_cvt_thread_create */
#endif


/*******************************************************************************
* adpt_cvt_serdes_port_recfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the serdes port again to initialize a 10G/40G/100G serdes port. This 
*   API is usually used for a dynamic switch of 10G/40G/100G serdes ports. For 
*   example, it should be called to initialize the serdes port when switching 
*   a 100G port to a 40G port. Before that, the adpt_traffic_mode_cfg() should be
*   called to disable the traffic mode of this port.
*
* INPUTS:
*   dev_id             - device identity.
*   serdes_id          - serdes identity.
*   serdes_type        - serdes type: DIGI_SERDES_LINE_INTF and DIGI_SERDES_SYSTEM_INTF.
*   los_pin            - los pin position.
*   op_mode            - operation mode to delete, add or overwrite. For example: 
*                        As to 1x100G <> 1x40G, the operation mode to overwrite is 
*                        suitable. As to 4x10G <> 1x40G, the operation mode to delete 
*                        should be used for all SERDES ports and then the operation 
*                        mode to add is required. 
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
PUBLIC PMC_ERROR adpt_cvt_serdes_port_recfg(UINT32 dev_id,
                                        UINT32 serdes_id,
                                        digi_serdes_intf_t serdes_type,
                                        UINT8 los_pin,
                                        digi_op_mode_t op_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lane_tx[DIGI_SERDES_XFI_LANES_MAX];
    UINT32 lane_rx[DIGI_SERDES_XFI_LANES_MAX];
    UINT32 index = 0;
    PMC_ENTRY();

    for (index = 0; index < DIGI_SERDES_XFI_LANES_MAX; index++)
    {
        lane_tx[index] = (UINT32)adpt_buffer[index];
    }

    for (index = 0; index < DIGI_SERDES_XFI_LANES_MAX; index++)
    {
        lane_rx[index] = (UINT32)adpt_buffer[index+DIGI_SERDES_XFI_LANES_MAX];
    }

    rc = adpt_serdes_port_recfg(dev_id, serdes_id, serdes_type, lane_tx, lane_rx, los_pin, op_mode);

    PMC_RETURN(rc);
} /* adpt_cvt_serdes_port_recfg */


/*
** End of file
*/
