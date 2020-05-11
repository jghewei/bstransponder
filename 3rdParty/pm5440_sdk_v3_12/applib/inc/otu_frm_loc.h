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
#ifndef _OTU_FRM_LOC_H
#define _OTU_FRM_LOC_H
/*GB: deterimine if we need extern C tags for *.h files in the src directory or only inc directory*/
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "otu_frm.h"
#include "pm55_82_20_otu_frm_mtsb_map.h"
#include "otu_frm_io.h"
#include "otu_frm_tx_io.h"
#include "otu_frm_rx_io.h"
#include "otudg_io.h"


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: otu_frm_sw_pwr_cfg_bit_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to specify different bits inside the 
*   OTU_FRM_MASTER_CFG register.
*
* ELEMENTS:
*           SW_RESET         - bit 31
*           SW_LOW_POWER        - bit 0
*           SW_RAM_LOW_POWER    - bit 1
*******************************************************************************/
typedef enum
{
    SW_RESET        = 0,
    SW_LOW_POWER,
    SW_RAM_LOW_POWER,

    LAST_SW_PWR_BIT        /* this is for out of range values error handling */
    
} otu_frm_sw_pwr_cfg_bit_t;



/*
** Constants
*/


/*
** Macro Definitions
*/






/*
** Structures and Unions
*/



/*
** Global variables
*/

/*
** Function Prototypes
*/
/* fec degap functions */
PUBLIC void otu_frm_fec_degap_init_set(otu_frm_handle_t    *otu_frm_handle,
                                       UINT8             fifo_full_thresh,
                                       UINT8             fifo_empty_thresh);

PUBLIC PMC_ERROR otu_frm_stream_fec_degap_set(otu_frm_handle_t      *otu_frm_handle,
                                              UINT8                 dsi_stream,
                                              otu_frm_stream_rate_t lane_rate,
                                              otu_frm_bit_value_t   enable_smoothing);

PUBLIC PMC_ERROR otu_frm_fec_degap_enable(otu_frm_handle_t       *otu_frm_handle,
                                          UINT8                  dsi_stream,
                                          otu_frm_stream_rate_t  lane_rate,
                                          BOOL                   enable_fec_degap);

PUBLIC PMC_ERROR otu_frm_rx_lineclk_mux_cfg(otu_frm_handle_t     *otu_frm_handle,
                                            UINT8                rx_lineclk_sel,
                                            UINT8                rx_lineclk_src);

PUBLIC PMC_ERROR otu_frm_rx_lineclk_mux_enable(otu_frm_handle_t *otu_frm_handle,
                                               UINT8 rx_lineclk_sel,
                                               UINT8 rx_lineclk_src,
                                               otu_frm_bit_value_t enable_source);

/* CBR_INT sub block functions */
PUBLIC PMC_ERROR cbr_int_channel_cfg_set(otu_frm_handle_t           *otu_frm_handle,
                                         otu_frm_stream_rate_t      lane_rate,
                                         int_dint_channel_cfg_dsc_t *channel_dscptr,
                                         otu_frm_bit_value_t        provision_ch);

PUBLIC PMC_ERROR cbr_int_multilane_cfg_set(otu_frm_handle_t      *otu_frm_handle,
                                           otu_frm_stream_rate_t lane_rate,
                                           UINT8                 lane_group_select,
                                           otu_frm_bit_value_t   enable_lane );

PUBLIC PMC_ERROR cbr_int_channel_dsc_set(otu_frm_handle_t           *otu_frm_handle,
                                         int_dint_channel_cfg_dsc_t *channel_dscptr,
                                         otu_frm_stream_rate_t      lane_rate);

PUBLIC PMC_ERROR cbr_int_lanes_cfg_set(otu_frm_handle_t           *otu_frm_handle,
                                       otu_frm_stream_rate_t      lane_rate,
                                       int_dint_channel_cfg_dsc_t *channel_dscptr);

PUBLIC PMC_ERROR cbr_int_free_lanes(otu_frm_handle_t      *otu_frm_handle,
                                    UINT8                 dsi_stream,
                                    otu_frm_stream_rate_t lane_rate );

PUBLIC void cbr_int_channel_reg_cleanup( otu_frm_handle_t *otu_frm_handle,
                                         UINT32            dsi_stream);

/* CBR_DINT sub block functions */
PUBLIC PMC_ERROR cbr_dint_channel_cfg_set(otu_frm_handle_t           *otu_frm_handle,
                                          otu_frm_stream_rate_t      lane_rate,
                                          int_dint_channel_cfg_dsc_t *channel_dscptr,
                                          otu_frm_bit_value_t        provision_ch);

PUBLIC PMC_ERROR cbr_dint_multilane_cfg_set(otu_frm_handle_t        *otu_frm_handle,
                                            otu_frm_stream_rate_t   lane_rate,
                                            UINT8                   lane_group_select,
                                            otu_frm_bit_value_t     enable_lane);

PUBLIC PMC_ERROR cbr_dint_channel_dsc_set(otu_frm_handle_t           *otu_frm_handle,
                                          int_dint_channel_cfg_dsc_t *channel_dscptr,
                                          otu_frm_stream_rate_t      lane_rate);

PUBLIC PMC_ERROR cbr_dint_lanes_cfg_set(otu_frm_handle_t             *otu_frm_handle,
                                        otu_frm_stream_rate_t        lane_rate,
                                        int_dint_channel_cfg_dsc_t   *channel_dscptr);

PUBLIC PMC_ERROR cbr_dint_free_lanes(otu_frm_handle_t       *otu_frm_handle,
                                     UINT8                  dsi_stream,
                                     otu_frm_stream_rate_t  lane_rate);


PUBLIC void cbr_dint_channel_reg_cleanup( otu_frm_handle_t *otu_frm_handle,
                                          UINT32            dsi_stream);
#ifdef __cplusplus
}
#endif

#endif /* _OTU_FRM_LOC_H */

/* 
** end of file 
*/

