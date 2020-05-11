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

#ifndef _OTU_FRM_H
#define _OTU_FRM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "oduksc.h"
#include "oduksc_api.h"
#include "otu_frm_api.h"

/*
** Enumerated Types
*/


/*******************************************************************************
* ENUM: otu_frm_sw_pwr_mode_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for pre-defined OTU_FRM power modes in LINEOTN
*
* ELEMENTS:
*       SW_LOWPWR_LS        - low power: RAM in light sleep mode,clocks are working,
*                             not in reset
*       SW_ACTIVE          - fully active mode: RAM active,clocks are working,
*                             not in reset
*
*******************************************************************************/
typedef enum
{
    SW_LOWPWR_LS = 0,
    SW_ACTIVE,

    LAST_SW_PWR_MODE        /* this is for out of range values error handling */

} otu_frm_sw_pwr_mode_cfg_t;


/*******************************************************************************
* ENUM: otu_frm_clk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying different clock types.
*
* ELEMENTS:
*           SW_RX_LINE_CLK0     - rx line clock for stream #0
*           SW_RX_LINE_CLK1     - rx line clock for stream #1
*           SW_RX_LINE_CLK2     - rx line clock for stream #2
*           SW_RX_LINE_CLK3     - rx line clock for stream #3
*           SW_RX_LINE_CLK4     - rx line clock for stream #4
*           SW_RX_LINE_CLK5     - rx line clock for stream #5
*           SW_RX_LINE_CLK6     - rx line clock for stream #6
*           SW_RX_LINE_CLK7     - rx line clock for stream #7
*           SW_RX_LINE_CLK8     - rx line clock for stream #8
*           SW_RX_LINE_CLK9     - rx line clock for stream #9
*           SW_RX_LINE_CLK10     - rx line clock for stream #10
*           SW_RX_LINE_CLK11    - rx line clock for stream #11
*           SW_SYSCLK           - system clock
*           SW_CLK_355M         - 355MHz clock
*
*******************************************************************************/

/*******************************************************************************
* ENUM: otu_frm_ctl_field_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify SK/SO control fields.
*
* ELEMENTS:
*   SK Control fields:
*   OTU_SK_BDI_DIS_FROM_TTI     -  the backwards action disable bit for the OTUk_TT action aBDI
*                                  for the SM channel resulting from a dTIM.
*   OTU_SK_BDI_DIS_FROM_SSF     -  the backwards action disable bit for the OTUk_TT action aBDI
*                                  for the SM channel resulting from an aSSF.
*   OTU_SK_BEI_DIS              -  the backwards action disable bit for the OTUk_TT action aBEI
*                                  for the SM channel resulting from an nBIPV.
*   OTU_SK_BIAE_DIS             -  the backwards action disable bit for the OTUk_TT action aBIAE
*                                  for the SM channel resulting from a dIAE.
*   OTU_SK_TSD_DIS              -  the disable bit for the OTUk_TT action aTSD for the SM channel
*                                  resulting from a dDEG.
*   OTU_SK_TSF_DIS_FROM_SSF     -  the disable bit for the OTUk_TT action aTSF for the SM channel
*                                  resulting from an aSSF.
*   OTU_SK_TIM_ACT_DIS          -  Enable/Disable dTIM defect contribution to the aTSF and the aAIS
*                                  consequential action declarations.
*   OTU_SK_ADAPT_TSD_DIS        -  Enable/Disable the aTSD consequential action is detection
*                                  and processing.
*   OTU_SK_ADAPT_SSD_DIS        -  the backwards action disable bit for the OTUk/ODUk_A action aSSD
*                                  for the SM channel.
*   OTU_SK_ADAPT_SSF_DIS        -  the backwards action disable bit for the OTUk/ODUk_A action aSSF
*                                  for the SM channel.
*   OTU_SK_ADAPT_AIS_DIS        -  the backwards action disable bit for the OTUk/ODUk_A action aAIS
*                                  for the SM channel.
*   OTU_SK_MI_LCK               -  controls the insertion of LCK when MI_AdminState is enabled
*                                  for the SM channel.
*   OTU_SK_SSF_MI_DIS           -  the aSSF disable for the MI_AdminState function and disables the
*                                  propagation of aSSF for the SM channel.
*   OTU_SK_AIS_MI_DIS           -  the aAIS disable for the MI_AdminState function and disables the
*                                  propagation of aAIS for the SM channel.
*   OTU_SK_SSF_MI_DIS           -  the aSSD disable for the MI_AdminState function and disables the
*                                  propagation of aSSD for the SM channel.
*   OTU_SK_TSD_MI_DIS           -  the aTSD disable for the MI_AdminState function and disables the
*                                  propagation of aTSD for the SM channel.
*   OTU_SK_TIM_DET_MO           -  the trail trace identifier mismatch detection mode for the SM channel.
*   OTU_SK_MAINT_SIG            -  the Maintenance Signal bits control the forced insertion of ODUk
*                                  maintenance signals.
*   OTU_SK_PASSTHRU             -  enables OTUk pass-through on the SM channel. \n\n
* 
*   OTU_SO_FAS_INS_CTRL         -  controls the insertion of the OTUk_So Frame Alignment FAS field.
*   OTU_SO_MFAS_INS_CTRL        -  controls the insertion of the OTUk_So Frame Alignment MFAS field.
*   OTU_SO_SM_TTI_INS_CTRL      -  controls the insertion of the OTUk_So SM TTI SAPI/DAPI/OP fields.
*   OTU_SO_SM_BIP8_INS_CTRL     -  controls the insertion of the OTUk_So SM BIP-8 field.
*   OTU_SO_SM_BEI_BIAE_INS_CTRL -  controls the insertion of the OTUk_So SM BEI/BIAE field.
*   OTU_SO_SM_BDI_INS_CTRL      -  controls the insertion of the OTUk_So SM BDI field.
*   OTU_SO_SM_IAE_INS_CTRL      -  controls the insertion of the OTUk_So SM IAE field.
*   OTU_SO_SM_RES7_INS_CTRL     -  controls the register insertion of the OTUk_So SM RES bit position 7 field.
*   OTU_SO_SM_RES8_INS_CTRL     -  controls the register insertion of the OTUk_So SM RES bit position 8 field.
*   OTU_SO_GCC0_INS_CTRL        -  controls the register insertion of the OTUk_So GCC0 field.
*   OTU_SO_RES13_INS_CTRL       -  controls the register insertion of the OTUk_So RES Column 13 field.
*   OTU_SO_RES14_INS_CTRL       -  controls the register insertion of the OTUk_So RES Column 14 field.
*   OTU_SO_MAINT_SIG            -  controls the force insertion of OTUk_SO ODUk maintenance signals.
*   OTU_SO_MI_ADMINSTATE        -  activates OTUk_So MI_AdminState and forces the insertion of OTUk_So ODUk-LCK.
*   OTU_SO_PASSTHRU             -  enables OTUk pass-through.
*
*******************************************************************************/
typedef enum
{
    OTU_SK_BDI_DIS_FROM_TTI = 0,
    OTU_SK_BDI_DIS_FROM_SSF,
    OTU_SK_BEI_DIS,
    OTU_SK_BIAE_DIS,
    OTU_SK_TSD_DIS,
    OTU_SK_TSF_DIS_FROM_SSF,
    OTU_SK_TIM_ACT_DIS,
    OTU_SK_ADAPT_SSD_DIS,
    OTU_SK_ADAPT_SSF_DIS,
    OTU_SK_ADAPT_AIS_DIS,
    OTU_SK_MI_LCK,
    OTU_SK_SSF_MI_DIS,
    OTU_SK_AIS_MI_DIS,
    OTU_SK_SSD_MI_DIS,
    OTU_SK_TIM_DET_MO,
    OTU_SK_MAINT_SIG,
    OTU_SK_PASSTHRU,

    OTU_SO_FAS_INS_CTRL,
    OTU_SO_MFAS_INS_CTRL,
    OTU_SO_SM_TTI_INS_CTRL,
    OTU_SO_SM_BIP8_INS_CTRL,
    OTU_SO_SM_BEI_BIAE_INS_CTRL,
    OTU_SO_SM_BDI_INS_CTRL,
    OTU_SO_SM_IAE_INS_CTRL,
    OTU_SO_SM_RES7_INS_CTRL,
    OTU_SO_SM_RES8_INS_CTRL,
    OTU_SO_GCC0_INS_CTRL,
    OTU_SO_RES13_INS_CTRL,
    OTU_SO_RES14_INS_CTRL,
    OTU_SO_MAINT_SIG,
    OTU_SO_MI_ADMINSTATE,
    OTU_SO_PASSTHRU,

    LAST_CTL    /* this is for out of range values error handling */

}otu_frm_ctl_field_t;


/*******************************************************************************
* ENUM: otu_frm_sm_oh_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify SK/SO OH fields.
*
* ELEMENTS:
*   OTU_SK_SM_TTI       -  the extracted SM TTI overhead field for a single OTUk frame.
*   OTU_SK_SM_BIP8      -  the extracted SM BIP-8 overhead field.
*   OTU_SK_SM_BEI_BIAE  -  the extracted SM-BEI/BIAE overhead field.
*   OTU_SK_SM_BDI       -  the extracted SM-BDI overhead field.
*   OTU_SK_SM_IAE       -  the extracted SM-IAE overhead field.
*   OTU_SK_SM_RES       -  the extracted SM-RES overhead field.
*   OTU_SK_RES          -  the extracted OTUk RES overhead field.
*   OTU_SK_GCC0         -  the extracted GCC0 overhead field.\n\n
*
*   OTU_SO_FAS_F6       -  the first 3 bytes of the FAS field
*   OTU_SO_FAS_28       -  the last 3 bytes of the FAS field
*   OTU_SO_MFAS         -  the OTUk_So MFAS Column 7 overhead field.
*   OTU_SO_SM_BIP8      -  the OTUk_So BIP8 overhead field.
*   OTU_SO_SM_BEI_BIAE  -  the OTUk_So BEI/BIAE overhead field.
*   OTU_SO_SM_BDI       -  the OTUk_So BDI overhead field.
*   OTU_SO_SM_IAE       -  the OTUk_So IAE overhead field.
*   OTU_SO_SM_RES7      -  the OTUk_So SM RES bit position 7 overhead field.
*   OTU_SO_SM_RES8      -  the OTUk_So SM RES bit position 8 overhead field.
*   OTU_SO_GCC0         -  the OTUk_So GCC0 overhead field.
*   OTU_SO_RES13        -  the OTUk_So RES Column 13 overhead field.
*   OTU_SO_RES14        -  the OTUk_So RES Column 14 overhead field.
*
*******************************************************************************/
typedef enum
{
    OTU_SK_SM_TTI = 0,
    OTU_SK_SM_BIP8,
    OTU_SK_SM_BEI_BIAE,
    OTU_SK_SM_BDI,
    OTU_SK_SM_IAE,
    OTU_SK_SM_RES,
    OTU_SK_RES,
    OTU_SK_GCC0,

    OTU_SO_FAS_F6,
    OTU_SO_FAS_28,
    OTU_SO_MFAS,
    OTU_SO_SM_BIP8,
    OTU_SO_SM_BEI_BIAE,
    OTU_SO_SM_BDI,
    OTU_SO_SM_IAE,
    OTU_SO_SM_RES7,
    OTU_SO_SM_RES8,
    OTU_SO_GCC0,
    OTU_SO_RES13,
    OTU_SO_RES14,

    LAST_OH /* this is for out of range values error handling */
}otu_frm_sm_oh_t;


/*******************************************************************************
* ENUM: otu_frm_stream_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying different supported data rates
*   Per data stream.
*
* ELEMENTS:
*       RATE_10G    -   10Gbps
*       RATE_40G    -   40Gbps
*       RATE_100G   -   100Gbps
*
*******************************************************************************/
typedef enum
{
    RATE_10G=0,
    RATE_40G,
    RATE_100G,

    LAST_RATE       /* this is for out of range values error handling */

}otu_frm_stream_rate_t;



/*******************************************************************************
* ENUM: otu_frm_bit_value_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify logical 1 and logical 0 per configuration
*   bit.
*
* ELEMENTS:
*       RESET_BIT       - Logical 0
*       SET_BIT         - Logical 1
*
*
*******************************************************************************/
typedef enum
{
    RESET_BIT   = 0,
    SET_BIT     = 1,

}otu_frm_bit_value_t;


/*******************************************************************************
* ENUM: otu_frm_sm_sk_gc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify OTUK_SK_CFG register fields.
*
* ELEMENTS:
*           OTU_SK_AC_TTI_FIELD -   bit 2
*           OTU_SK_TTI_LOCK     -   bit 1
*           OTU_SK_LOCK         -   bit 0
*
*
*******************************************************************************/
typedef enum
{
    OTU_SK_AC_TTI_FIELD = 0,
    OTU_SK_TTI_LOCK,
    OTU_SK_LOCK,

    LAST_SK_GC  /* this is for out of range values error handling */
}otu_frm_sm_sk_gc_t;



/*******************************************************************************
* ENUM: otu_frm_str_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify stream provisioning states.
*
* ELEMENTS:
*           STREAM_STATE_START          -   Initial stream state
*           STREAM_STATE_EQUIPPED       -   stream is provisioned
*           STREAM_STATE_OPERATIONAL    -   stream is active and operational
*
*******************************************************************************/
typedef enum
{
    STREAM_STATE_START = 0,
    STREAM_STATE_EQUIPPED,
    STREAM_STATE_OPERATIONAL,

    LAST_STR_STATE /* this is for out of range values error handling */
}otu_frm_str_state_t;

/*******************************************************************************
* ENUM: otu_frm_direction_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify SK or SO direction.
*
* ELEMENTS:
*       SK_DIR      - SK direction (Rx)
*       SO_DIR      - SO direction (Tx)
*
*
*******************************************************************************/
typedef enum
{
    SK_DIR=0,
    SO_DIR,

    LAST_DIR    /* this is for out of range values error handling */
}otu_frm_direction_t;


/*******************************************************************************
* ENUM: otu_frm_sk_sm_deg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify OTUK_SK_SM_DEG register fields.
*
* ELEMENTS:
*  OTU_SK_SM_DEG_M   - the degraded defect consecutive one second monitoring intervals 
*                      for the SM channel.
*  OTU_SK_SM_DEG_THR - the degraded defect one second errored block count for the 
*                      SM channel
*
*******************************************************************************/
typedef enum
{
    OTU_SK_SM_DEG_M = 0,
    OTU_SK_SM_DEG_THR,

    LAST_SM_DEG /* this is for out of range values error handling */
}otu_frm_sk_sm_deg_t;


/*******************************************************************************
* ENUM: otu_frm_state_param_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying different state counters.
*
* ELEMENTS:
*             OOM_CNT_LIMIT         - Out Of MultiFrame counter limit type
*             IM_CNT_LIMT           - In MultiFrame counter limit type
*             DLOM_CNT_LIMIT        - defect Loss Of Multiframe counter limit type
*             SF_SQULCH_CNT_LIMIT   - Signal Fail Squelch counter limit type
*
*******************************************************************************/
typedef enum
{
    OOM_CNT_LIMIT = 0,
    IM_CNT_LIMT,
    DLOM_CNT_LIMIT,
    SF_SQULCH_CNT_LIMIT,

    LAST_STATE      /* this is for out of range values error handling */

}otu_frm_state_param_t;

/*******************************************************************************
* ENUM: otu_frm_sm_cnt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify SK far/near end error counters.
*
* ELEMENTS:
*   OTU_SK_SM_P_NE_IAE  - IAE performance monitor (pIAE) for the SM channel
*   OTU_SK_SM_P_NE_DS   - Near End Defect Second performance monitor (pN_DS) for the SM channel
*   OTU_SK_SM_P_NE_BC   - Near End Errored Block Counter (N_EBC) for the SM channel
*   OTU_SK_SM_P_FE_BIAE - BIAE performance monitor (pBIAE) for the SM channel
*   OTU_SK_SM_P_FE_DS   - Far End Defect Second performance monitor (pF_DS) for the SM channel
*   OTU_SK_SM_P_FE_BC   - Far End Errored Block Counter (F_EBC) for the SM channel
*
*******************************************************************************/
typedef enum
{
    OTU_SK_SM_P_NE_IAE = 0,
    OTU_SK_SM_P_NE_DS,
    OTU_SK_SM_P_NE_BC,
    OTU_SK_SM_P_FE_BIAE,
    OTU_SK_SM_P_FE_DS,
    OTU_SK_SM_P_FE_BC,

    LAST_CNT    /* this is for out of range values error handling */

}otu_frm_sm_cnt_t;

/*******************************************************************************
* ENUM: otu_frm_sk_tim_det_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   enumerated structure to specify the trail trace identifier mismatch detection mode 
*   for the SM channel
*
* ELEMENTS:
*    OTU_SK_SM_DTIM_DET_DISABLED    - dTIM defect detection is disabled 
*    OTU_SK_SM_DTIM_DET_SAPI        - The expected SAPI field is used when determining the dTIM defect 
*    OTU_SK_SM_DTIM_DET_DAPI        - The expected DAPI field is used when determining the dTIM defect 
*    OTU_SK_SM_DTIM_DET_BOTH        - Both the expected SAPI field and the expected DAPI field are 
*                                     used when determining the dTIM defect
*    LAST_SK_SM_DTIM_DET            - this is for out of range values error handling 
 *******************************************************************************/
typedef enum
{
    OTU_SK_SM_DTIM_DET_DISABLED = 0,  
    OTU_SK_SM_DTIM_DET_SAPI, 
    OTU_SK_SM_DTIM_DET_DAPI, 
    OTU_SK_SM_DTIM_DET_BOTH, 
    LAST_SK_SM_DTIM_DET  
}otu_frm_sk_tim_det_mode_t;


/*
** Constants
*/

/*
 * Define the number of rows per TTI field
 */
#define SAPI_ROW_NUM            (4)
#define DAPI_ROW_NUM            (4)
#define OP_ROW_NUM              (8)

/*
 * Define the of elements per row of a TTI field
 */
#define TTI_ITEMS_PER_ROW       (4)


/*
 * Define the maximum number of RAM power states
 */
#define NUM_RAM_PWR_MODES       (RAM_ACTIVE)


/*
 * Define the maximum number of frames per OOM/IM counter
 */
#define MAX_OOM_IM_FRAME_NUM    (15)


/*
 * Define the maximum number of dci channels/streams per supported data rate
 */
#define MAX_10G_STREAM_NUM      (12)
#define MAX_40G_STREAM_NUM      (3)
#define MAX_100G_STREAM_NUM     (1)

#define MAX_DCI_CHANNEL_NUM     (12)


/*
 * Defines a free channel
 */
#define FREE_CHANNEL            (0x7F)

/*
 * Defines a defect error
 */
#define OTU_FRM_DEFECT_ERROR    (0xFFFFFFFF)
/*
 * Defines a free stream lane in the stream descriptor
 */
#define FREE_STREAM             (0xFF)

/*
 * Defines a free cbr int/dint calendar channel
 */
#define INT_DINT_CAL_NULL_ID    (0x7F)

/*
 * Defines CBR_INT/DINT the maximum number of lanes per supported data rate
 */
#define MAX_10G_LANE_NUM        (12)
#define MAX_40G_LANE_NUM        (3)
#define MAX_100G_LANE_NUM       (1)

/*
 * Defines CBR_INT/DINT free channel lane in the channel descriptor
 */
#define FREE_LANE               (0xFF)


/*
 * Defines a multilane bits location for setting the
 * int_dint_multilane_cfg_t bit fields
 */

#define MULTILANE_100G          (0x00000001) /* allocated 1 bit for 100G lanes*/

#define MULTILANE_40G_GRP0      (0x00000001)/* allocated 3 bits for 40G lanes */
#define MULTILANE_40G_GRP1      (0x00000002)
#define MULTILANE_40G_GRP2      (0x00000004)


#define MULTILANE_10G_40G_GRP0  (0x0000000f)
#define MULTILANE_10G_40G_GRP1  (0x000000f0)
#define MULTILANE_10G_40G_GRP2  (0x00000f00)

#define MULTILANE_10G_100G      (0x00000fff)

/*
 * Defines fec degap default fifo full/empty thersholds
 */
#define OTUDG_FIFO_EMPTY_THRS   (108)
#define OTUDG_FIFO_FULL_THRS    (204)
#define OTUDG_FIFO_SIZE         (240)

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include the following code */

/*
** Macro Definitions
*/
/* OTU_FRM_INT(handle, tsb_name, tsb_field, struct_field) */
#define OTU_FRM_INT_TABLE_DEFINE(dsi_master_lane_num, dci_channel_num)\
    link1 = dsi_master_lane_num; \
    link2 = dsi_master_lane_num; \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_INTLV_WOVR , otu_frm_intlv_wovr_i  ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_INTLV_RUDR , otu_frm_intlv_rudr_i  ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_DINTLV_WOVR, otu_frm_dintlv_wovr_i ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_DINTLV_RUDR, otu_frm_dintlv_rudr_i ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_D_LOM, otu_frm_d_lom_i      ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm,    field_range, OTU_FRM_OOM  , otu_frm_oom_i        ); \
    OTU_FRM_INT(otu_frm_handle, otudg,      field_range, V_FIFO_OVR  ,  v_fifo_ovr_i ); \
    OTU_FRM_INT(otu_frm_handle, otudg,      field_range, V_FIFO_UDR  ,  v_fifo_udr_i ); \
    link1 = dci_channel_num; \
    link2 = dci_channel_num; \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_AIS , otu_sk_a_ais_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_SSD , otu_sk_a_ssd_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_SSF , otu_sk_a_ssf_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_TSF , otu_sk_a_tsf_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_TSD , otu_sk_a_tsd_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BIAE, otu_sk_a_biae_i      ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BEI , otu_sk_a_bei_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BDI , otu_sk_a_bdi_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_IAE , otu_sk_d_iae_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_TIM , otu_sk_d_tim_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_DEG , otu_sk_d_deg_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_BDI , otu_sk_d_bdi_i       ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_BIAE, otu_sk_d_biae_i      ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_CI_SSF, otu_sk_ci_ssf_i      ); \
    OTU_FRM_INT(otu_frm_handle, otu_frm_tx, field_range, OTU_SO_A_IAE , otu_so_a_iae_i       ); \
    OTU_FRM_INT_NOOP(otu_frm_handle, otu_frm_tx, NULL, otu_sk_d_deg_enh_i); \
    OTU_FRM_INT_NOOP(otu_frm_handle, otu_frm_tx, NULL, otu_sk_d_deg_bursty_i); \

#define OTU_FRM_STATUS_TABLE_DEFINE(dsi_master_lane_num, dci_channel_num)                                           \
    link1 = dsi_master_lane_num; \
    link2 = dsi_master_lane_num; \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_INTLV_WOVR_V , otu_frm_intlv_wovr_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_INTLV_RUDR_V , otu_frm_intlv_rudr_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_DINTLV_WOVR_V, otu_frm_dintlv_wovr_v ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_DINTLV_RUDR_V, otu_frm_dintlv_rudr_v ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_D_LOM_V, otu_frm_d_lom_v ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm,    field_range, OTU_FRM_OOM_V  , otu_frm_oom_v   ); \
    link1 = dci_channel_num; \
    link2 = dci_channel_num; \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_AIS_V , otu_sk_a_ais_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_SSD_V , otu_sk_a_ssd_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_SSF_V , otu_sk_a_ssf_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_TSF_V , otu_sk_a_tsf_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_TSD_V , otu_sk_a_tsd_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BIAE_V, otu_sk_a_biae_v ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BEI_V , otu_sk_a_bei_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_A_BDI_V , otu_sk_a_bdi_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_IAE_V , otu_sk_d_iae_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_TIM_V , otu_sk_d_tim_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_DEG_V , otu_sk_d_deg_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_BDI_V , otu_sk_d_bdi_v  ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_D_BIAE_V, otu_sk_d_biae_v ); \
    OTU_FRM_STATUS(otu_frm_handle, otu_frm_rx, field_range, OTU_SK_CI_SSF_V, otu_sk_ci_ssf_v ); \
    OTU_FRM_STATUS_NOOP(otu_frm_handle, otu_frm_tx, NULL, otu_sk_d_deg_enh_v); \
    OTU_FRM_STATUS_NOOP(otu_frm_handle, otu_frm_tx, NULL, otu_sk_d_deg_bursty_v); \


#endif /* DOXYGEN_PUBLIC_ONLY */

/*
 * Defines a multilane macrosfor setting/reseting the
 * int_dint_multilane_cfg_t bit fields
 */
#define SET_MULTILANE(multilane_storage,multilane)      ( multilane_storage |= multilane )
#define RESET_MULTILANE(multilane_storage,multilane)    ( multilane_storage &= ~multilane )

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: int_dint_multilane_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Multilane bit map for tracking which lanes are allocated.
*   each allocated lane has 1 bit assigned to it.
*
* ELEMENTS:
*         multilane_100G - 1 bit value: 0: 100G lane not allocated,
*                                       1: 100G lane allocated\n
*                          Note: when 100G is allocated, all 12 10G
*                                bits will be set to 1.
*         multilane_40G  - 3 bit value: \n
*                                        000: No 40G lanes are allocated \n
*                                        001: 40G Group 0 lanes allocated \n
*                                        010: 40G Group 1 lanes allocated \n
*                                        100: 40G Group 2 lanes allocated\n
*                          Note: when 40G is allocated, relevant 10G
*                                bits that map to it will be set to 1.
*         multilane_10G  - 12 bit value: \n
*                                        000 000 000 000: 10G lanes not allocated \n
*                                        000 000 000 001: 10G lane 0 allocated \n
*                                        000 000 000 010: 10G lane 1 allocated \n
*                                        000 000 000 100: 10G lane 2 allocated \n
*                                        etc \n
*                                        010 000 000 000: 10G lane 10 allocated \n
*                                        100 000 000 000: 10G lane 11 allocated
*         unused_bits    - 16 bit value; unused.
*
*******************************************************************************/
typedef struct {
    UINT32 multilane_100G   : 1;
    UINT32 multilane_40G    : 3;
    UINT32 multilane_10G    : 12;
    UINT32 unused_bits      : 16;
}int_dint_multilane_cfg_t;



/*******************************************************************************
* STRUCTURE: otu_frm_stream_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   stream descriptor enable/disable structure.
*
* ELEMENTS:
*         fec_col_rm_enable         - 1 bit value, 0: disable, 1: enable
*         descrambling_enable       - 1 bit value, 0: disable, 1: enable
*         dlom_enable               - 1 bit value, 0: disable, 1: enable
*         sf_squelch_enable         - 1 bit value, 0: disable, 1: enable
*         fec_col_ins_enable        - 1 bit value, 0: disable, 1: enable
*         scarmbling_enable         - 1 bit value, 0: disable, 1: enable
*         dlom_to_sf_enable         - 1 bit value, 0: disable, 1: enable
*         unused_bits               - unused field
* 
*******************************************************************************/
typedef struct {
    UINT32 fec_col_rm_enable        : 1;
    UINT32 descrambling_enable      : 1;
    UINT32 dlom_enable              : 1;
    UINT32 sf_squelch_enable        : 1;
    UINT32 fec_col_ins_enable       : 1;
    UINT32 scarmbling_enable        : 1;
    UINT32 dlom_to_sf_enable        : 1;
    UINT32 unused_bits              :25;

}otu_frm_stream_cfg_t;

/*******************************************************************************
* STRUCTURE: otu_frm_stream_cfg_dsc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for stream descriptor.
*
* ELEMENTS:
*       stream_lane     - stream_lane: (UINT8) 10G (0..11),40G (0..2),100G (0)
*       rx_stream_state - RX Stream State
*       tx_stream_state - TX Stream State
*       stream_dsc      - stream descriptor enable/disable structure:
*
*******************************************************************************/
typedef struct {
    UINT8                           stream_lane;
    otu_frm_str_state_t             rx_stream_state;
    otu_frm_str_state_t             tx_stream_state;
    otu_frm_stream_cfg_t            stream_dsc;
} otu_frm_stream_cfg_dsc_t;


/*******************************************************************************
* STRUCTURE: otu_frm_power_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold the current sw and RAM power configuration.
*
* ELEMENTS:
*       sw_power_mode       - Configured SW power state
*
*******************************************************************************/
typedef struct {
    otu_frm_sw_pwr_mode_cfg_t       sw_power_mode;
} otu_frm_power_cfg_t;


/*******************************************************************************
* STRUCTURE: int_dint_channel_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deinterleaver channel descriptor enable/disable structure
*
* ELEMENTS:
*            ch_enable          - 1 bit value, 1:enable,0:disable the channel
*            ch_id              - 7 bits value, assigned channel id
*            ch_id_lane         - 8 bits value, channel id register lane
*            unused_bits        - unused field
*
*******************************************************************************/
typedef struct {
    UINT32 ch_enable        : 1;
    UINT32 ch_id            : 7;
    UINT32 ch_id_lane       : 8;
    UINT32 unused_bits      : 16;

}int_dint_channel_cfg_t;

/*******************************************************************************
* STRUCTURE: int_dint_channel_cfg_dsc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for Deinterleaver channel descriptor.
*
* ELEMENTS:
*           channel_lane - For 10G channels valid values are 0..11
*           channel_dsc  - Enable/Disable structure
*
*******************************************************************************/
typedef struct {
    UINT8                           channel_lane;
    int_dint_channel_cfg_t          channel_dsc;
} int_dint_channel_cfg_dsc_t;


/*******************************************************************************
* STRUCTURE: otu_frm_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the config struct for dynamic variables.
*
* ELEMENTS:
*       otu_frm_pwr_cfg             - Holds the current SW and RAM power configuration
*       otu_frm_10G_stream_cfg      - Channel descriptors for 10G streams
*       otu_frm_40G_stream_cfg      - Channel descriptors for 40G streams
*       otu_frm_100G_stream_cfg     - Channel descriptors for 100G streams
*       dci_to_dsi_chan_ids         - Look-up table to store dsi stream numbers
*       dsi_oduk_type               - Stores the ODUk rate associated for each channel
*                                               
*       cbr_dint_channel_id_cfg     - Deinterleaver Channel configuration
*       cbr_dint_multilane_cfg      - Tracks which lanes are allocated in the deinterleaver                
*                                               
*       cbr_int_channel_id_cfg      - Interleaver Channel configuration 
*       cbr_int_multilane_cfg       - Tracks which lanes are allocated in the interleaver                
*       data_smoothing_enabled      - Indicates whether the OTU degapper is enabled which removes
*                                     large gaps from where the FEC columns were removed
*       lineotn_db_handle_rx        - Pointer for the RX LINEOTN Database                
*       lineotn_db_handle_tx        - Pointer for the TX LINEOTN Database
*       lineotn_db_handle_sysotn_mode - Pointer for the SYSOTN Database
*       otudg_sfi51_lanes_bm        -  Used for the clock reference for the OTU Degapper
*                                      to know whether the SFI51 clock is used as a reference
* 
*******************************************************************************/
typedef struct {

    otu_frm_power_cfg_t         otu_frm_pwr_cfg;

    otu_frm_stream_cfg_dsc_t    otu_frm_10G_stream_cfg[MAX_10G_STREAM_NUM];
    otu_frm_stream_cfg_dsc_t    otu_frm_40G_stream_cfg[MAX_40G_STREAM_NUM];
    otu_frm_stream_cfg_dsc_t    otu_frm_100G_stream_cfg[MAX_100G_STREAM_NUM];
    UINT8                       dci_to_dsi_chan_ids[MAX_DCI_CHANNEL_NUM];
    util_global_odukp_type_t    dsi_oduk_type  [MAX_10G_STREAM_NUM];

    int_dint_channel_cfg_dsc_t  cbr_dint_channel_id_cfg[MAX_10G_LANE_NUM];
    int_dint_multilane_cfg_t    cbr_dint_multilane_cfg;

    int_dint_channel_cfg_dsc_t  cbr_int_channel_id_cfg[MAX_10G_LANE_NUM];
    int_dint_multilane_cfg_t    cbr_int_multilane_cfg;
    BOOL                        data_smoothing_enabled[MAX_10G_STREAM_NUM];
    UINT32                      lineotn_db_handle_rx;
    UINT32                      lineotn_db_handle_tx;
    UINT32                      lineotn_db_handle_sysotn_mode;
    UINT32                      otudg_sfi51_lanes_bm;

} otu_frm_var_t;


/*******************************************************************************
* STRUCTURE: otu_frm_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handle structure for the OTU_FRM block
*
* ELEMENTS:
*        base       - Base address
*        cfg        - Contains static configuration variables for this block
*        var        - Contains dynamic configuration variables for this block
*        oduksc_handle - Pointer for the ODUKSC sub-block
*        pop_ho_db      - Indicates whether the HO database is populated
*
*******************************************************************************/

typedef struct otu_frm_handle_struct_t {
    pmc_handle_t base;

    otu_frm_var_t var;

    oduksc_handle_t             *oduksc_handle;
    BOOL8                       pop_ho_db;
} otu_frm_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

/* Init functions */

PUBLIC otu_frm_handle_t *otu_frm_ctxt_create      (pmc_handle_t     *parent,
                                                   UINT32           base_address,
                                                   pmc_sys_handle_t *sys_handle,
                                                   const char       *tsb_name);

PUBLIC void             otu_frm_handle_init       (otu_frm_handle_t *otu_frm_handle);
PUBLIC void             otu_frm_blk_init          (otu_frm_handle_t *otu_frm_handle);
PUBLIC void             otu_frm_ctxt_destroy      (otu_frm_handle_t *otu_frm_handle);


/* Clocks and power */
PUBLIC PMC_ERROR        otu_frm_pwr_mode_config_set(otu_frm_handle_t          *otu_frm_handle,
                                                    otu_frm_sw_pwr_mode_cfg_t  sw_pwr_cfg_mode);

/* Provisioning functions */
PUBLIC PMC_ERROR otu_frm_blk_prov_rx_stream     (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type,
                                                 BOOL                       scrambling_enable,
                                                 BOOL                       data_smooth_enable,
                                                 UINT32                     *dci_channel);

PUBLIC PMC_ERROR otu_frm_blk_deprov_rx_stream   (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

PUBLIC PMC_ERROR otu_frm_blk_act_rx_stream      (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

PUBLIC PMC_ERROR otu_frm_blk_deact_rx_stream    (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

PUBLIC PMC_ERROR otu_frm_blk_prov_tx_stream     (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type,
                                                 BOOL                       descrambling_enable,
                                                 UINT32                     *dci_channel);

PUBLIC PMC_ERROR otu_frm_blk_deprov_tx_stream   (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

PUBLIC PMC_ERROR otu_frm_blk_act_tx_stream      (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

PUBLIC PMC_ERROR otu_frm_blk_deact_tx_stream    (otu_frm_handle_t           *otu_frm_handle,
                                                 UINT8                      dsi_stream,
                                                 util_global_odukp_type_t   oduk_type);

/* Misc  - higher resolution specific configuration and fine tuning functions */
PUBLIC PMC_ERROR otu_frm_sm_oh_set              (otu_frm_handle_t   *otu_frm_handle,
                                                 UINT8              dci_channel,
                                                 otu_frm_sm_oh_t    oh_field,
                                                 UINT32             value);

PUBLIC UINT32  otu_frm_sm_oh_get(otu_frm_handle_t    *otu_frm_handle,
                                 UINT8               dci_channel,
                                 otu_frm_sm_oh_t     oh_field);


PUBLIC PMC_ERROR otu_frm_sm_ctl_set             (otu_frm_handle_t   *otu_frm_handle,
                                                 UINT8              dci_channel,
                                                 otu_frm_ctl_field_t ctl_field,
                                                 UINT8              value);

PUBLIC UINT32 otu_frm_sm_ctl_get                ( otu_frm_handle_t    *otu_frm_handle,
                                                  UINT8               dci_channel,
                                                  otu_frm_ctl_field_t ctl_field );

PUBLIC PMC_ERROR otu_frm_sm_oh_ext_get          (otu_frm_handle_t  *otu_frm_handle,
                                                 UINT8             dci_channel,
                                                 otu_frm_sm_oh_t   sk_field, 
                                                 UINT32            *extracted_oh_val);

PUBLIC void otu_frm_passthru_set (otu_frm_handle_t   *otu_frm_handle,
                                  UINT8              dci_channel,
                                  otu_frm_ctl_field_t sk_so_passtru,
                                  UINT8              value);


/* otu_frm sub block functions */

PUBLIC PMC_ERROR otu_frm_stream_cfg_set(otu_frm_handle_t       *otu_frm_handle,
                                        otu_frm_stream_rate_t  stream_rate,
                                        otu_frm_stream_cfg_dsc_t *stream_dscptr,
                                        otu_frm_direction_t        rx_tx_dir );

PUBLIC PMC_ERROR otu_frm_dlom_to_sf_set(otu_frm_handle_t        *otu_frm_handle,
                                        otu_frm_stream_rate_t   stream_rate,
                                        UINT8                   stream_lane,
                                        otu_frm_bit_value_t     enable_dlom_to_sf);

PUBLIC UINT32 otu_frm_dlom_to_sf_get( otu_frm_handle_t         *otu_frm_handle,
                                      otu_frm_stream_rate_t    stream_rate,
                                      UINT8                    stream_lane );



PUBLIC PMC_ERROR otu_frm_reset_config_set( otu_frm_handle_t     *otu_frm_handle,
                                           BOOL                 reset_otu_frm );

/* otu_frm_rx,otu_frm_tx (SK/SO) sub block funtions */

PUBLIC PMC_ERROR otu_frm_stream_scrambling_set(otu_frm_handle_t            *otu_frm_handle,
                                               UINT8                       dsi_stream,
                                               otu_frm_stream_rate_t       stream_rate,
                                               BOOL                        scarmble_in_otu );

PUBLIC PMC_ERROR otu_frm_mi_active_set( otu_frm_handle_t            *otu_frm_handle,
                                        otu_frm_stream_rate_t       stream_rate,
                                        UINT8                       stream_lane,
                                        otu_frm_bit_value_t         enable_mi_active );

/* fec degap functions */
PUBLIC PMC_ERROR otu_frm_sm_deg_set             ( otu_frm_handle_t    *otu_frm_handle,      
                                                  UINT8               dci_channel,
                                                  otu_frm_sk_sm_deg_t deg_field,
                                                  UINT32              value );

PUBLIC PMC_ERROR otu_frm_sm_deg_get             ( otu_frm_handle_t       *otu_frm_handle,
                                                  UINT8               dci_channel,
                                                  otu_frm_sk_sm_deg_t deg_field,
                                                  UINT32              *value );

PUBLIC PMC_ERROR otu_frm_sm_tti_set             ( otu_frm_handle_t   *otu_frm_handle,
                                                  otu_frm_direction_t sk_so,
                                                  UINT8              dci_channel,
                                                  UINT32             tti_data[16] );

PUBLIC PMC_ERROR otu_frm_sm_tti_get             ( otu_frm_handle_t       *otu_frm_handle,
                                                 otu_frm_direction_t sk_so,
                                                 UINT8               dci_channel,
                                                 UINT32              tti_data[16] );


PUBLIC void otu_frm_sm_accepted_tti_get ( otu_frm_handle_t   *otu_frm_handle,
                                          UINT8              dci_channel,
                                          UINT32             tti_data_buf[16] );

PUBLIC void otu_frm_exp_tti_tim_det_mode_set( otu_frm_handle_t           *otu_frm_handle,
                                              UINT8                      dci_channel,
                                              otu_frm_sk_tim_det_mode_t  exp_tti_tim_det_mode );

PUBLIC void otu_frm_exp_tti_tim_det_mode_get( otu_frm_handle_t           *otu_frm_handle,
                                              UINT8                      dci_channel,
                                              otu_frm_sk_tim_det_mode_t  *exp_tti_tim_det_mode );

PUBLIC void otu_frm_ebc_get( otu_frm_handle_t           *otu_frm_handle,
                             UINT8                      dci_channel,
                             otu_frm_counters_t  *ebc );

PUBLIC BOOL otu_frm_start_state_test ( otu_frm_handle_t    *otu_frm_handle ); 

PUBLIC void otu_frm_init ( otu_frm_handle_t     *otu_frm_handle,
                           pmc_energy_state_t   energy_state );


PUBLIC PMC_ERROR otu_frm_int_enable(otu_frm_handle_t   *otu_frm_handle,
                                    otu_frm_int_t      *int_table_ptr,
                                    otu_frm_int_t      *int_en_table_ptr,
                                    BOOL                enable);

PUBLIC PMC_ERROR otu_frm_int_clear(otu_frm_handle_t    *otu_frm_handle,
                                   otu_frm_int_t       *int_table_ptr,
                                   otu_frm_int_t       *int_en_table_ptr);

PUBLIC PMC_ERROR otu_frm_int_retrieve(otu_frm_handle_t    *otu_frm_handle,
                                      otu_frm_int_t       *filt_table_ptr,
                                      otu_frm_int_t       *int_table_ptr);

PUBLIC PMC_ERROR otu_frm_int_enabled_check(otu_frm_handle_t   *otu_frm_handle,
                                           otu_frm_int_t      *int_en_table_ptr,
                                           BOOL               *int_found_ptr);

PUBLIC PMC_ERROR otu_frm_int_chnl_enable(otu_frm_handle_t   *otu_frm_handle,
                                        UINT32               dsi_stream,
                                        otu_frm_int_chnl_t  *int_table_ptr,
                                        otu_frm_int_chnl_t  *int_en_table_ptr,
                                        BOOL                 enable);

PUBLIC PMC_ERROR otu_frm_int_chnl_clear(otu_frm_handle_t    *otu_frm_handle,
                                        UINT32               dsi_stream,
                                        otu_frm_int_chnl_t  *int_en_table_ptr,
                                        otu_frm_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR otu_frm_int_chnl_retrieve(otu_frm_handle_t    *otu_frm_handle,
                                           UINT32               dsi_stream,
                                           otu_frm_int_chnl_t  *filt_table_ptr,
                                           otu_frm_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR otu_frm_int_chnl_enabled_check(otu_frm_handle_t   *otu_frm_handle,
                                                UINT32              dsi_stream,
                                                otu_frm_int_chnl_t *int_en_table_ptr,
                                                BOOL               *int_found_ptr);

PUBLIC PMC_ERROR otu_frm_channel_dsc_get( otu_frm_handle_t             *otu_frm_handle,
                                          BOOL8                        *rx_act,
                                          BOOL8                        *tx_act,
                                          otu_frm_stream_rate_t        *lane_rate,
                                          UINT8                        *dci_channel);

PUBLIC void otu_frm_client_rate_get(otu_frm_handle_t           *otu_frm_handle,
                                    UINT32                       dsi_stream,
                                    util_global_odukp_type_t    *client_rate);


PUBLIC void otu_frm_reg_cleanup(otu_frm_handle_t    *otu_frm_handle,
                                UINT32               num_dsi_streams, 
                                UINT32              *dsi_streams);

PUBLIC void otu_frm_alloc_init(otu_frm_handle_t  *otu_frm_handle);

PUBLIC PMC_ERROR otu_frm_handle_restart_init(otu_frm_handle_t    *otu_frm_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PUBLIC PMC_ERROR otu_frm_db_chnl_entry_audit(otu_frm_handle_t        *otu_frm_handle,  
                                             scbs3_page_t            *scbs3_page_ptr,
                                             UINT32                   chnl);


PUBLIC PMC_ERROR otu_frm_db_dci_chnl_num_entries_get(otu_frm_handle_t    *otu_frm_handle, 
                                                     UINT32               dci_chnl,         
                                                     BOOL8                is_rx_chnl,    
                                                     UINT32              *num_of_recs_ptr);   


PUBLIC void otu_frm_ssf_assertion_on_intlv_fifo_empty_set (otu_frm_handle_t *otu_frm_handle, 
                                                           BOOL8 enable);

PUBLIC PMC_ERROR otu_frm_dintlv_xoff_thres_set(otu_frm_handle_t  *otu_frm_handle,
                                               UINT32            dci_channel,
                                               UINT32            dintlv_xoff);

#ifdef __cplusplus
}
#endif

#endif /* _OTU_FRM_H */

/*
** end of file
*/
