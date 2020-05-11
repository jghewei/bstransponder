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
*     This head file describes the OTN overhead configuration Run-Time function.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_OTN_H
#define _DIGI_ADPT_OTN_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_otn_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_otn_err_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Bergen bit error generator modes. The
*   element terms have a syntax:
*      DIGI_OTN_ERR_<application>
*
* ELEMENTS:
*   DIGI_OTN_ERR_NORMAL             - Inverts the input bit in order to insert 
                                      an erroneous bit.
*   DIGI_OTN_ERR_0_INV              - Inverts the input bit only if the input 
*                                     bit is 1 in order to insert an erroneous 0.
*   DIGI_OTN_ERR_1_INV              - Inverts the input bit only if the input 
*                                     bit is 0 in order to insert an erroneous 1.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_ERR_NORMAL  = LINE_OTN_BERGEN_NORMAL,
    DIGI_OTN_ERR_0_INV   = LINE_OTN_BERGEN_0_INSERTION,
    DIGI_OTN_ERR_1_INV   = LINE_OTN_BERGEN_1_INSERTION,

    LAST_DIGI_OTN_ERR, /* this is for out of range values error handling */
} digi_otn_err_mode_t;

/*******************************************************************************
* ENUM: digi_otn_seg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN segment type. The
*   element terms have a syntax:
*      DIGI_OTN_SEG_<application>
*
* ELEMENTS:
*   DIGI_OTN_SEG_PM                - PM segment
*   DIGI_OTN_SEG_TCM1              - TCM1 segment
*   DIGI_OTN_SEG_TCM2              - TCM2 segment
*   DIGI_OTN_SEG_TCM3              - TCM3 segment
*   DIGI_OTN_SEG_TCM4              - TCM4 segment
*   DIGI_OTN_SEG_TCM5              - TCM5 segment
*   DIGI_OTN_SEG_TCM6              - TCM6 segment
*   DIGI_OTN_SEG_PM_NIM            - PM NIM segment with TCM pool ID
*                                    (only valid for sink)
*   DIGI_OTN_SEG_DCI               - PARENT is DCI
*   DIGI_OTN_SEG_SM                - SM segment
*
*******************************************************************************/
typedef enum 
{
    DIGI_OTN_SEG_PM     = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM,
    DIGI_OTN_SEG_TCM1   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM1,
    DIGI_OTN_SEG_TCM2   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM2, 
    DIGI_OTN_SEG_TCM3   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM3,
    DIGI_OTN_SEG_TCM4   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM4,
    DIGI_OTN_SEG_TCM5   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM5,
    DIGI_OTN_SEG_TCM6   = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM6,
    DIGI_OTN_SEG_PM_NIM = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM,
    DIGI_OTN_SEG_DCI    = ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DCI,
    DIGI_OTN_SEG_SM,

    LAST_DIGI_OTN_SEG, /* this is for out of range values error handling */
} digi_otn_seg_t;

/*******************************************************************************
* ENUM: digi_otn_tim_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN dTIM detection mode. The
*   element terms have a syntax:
*      DIGI_TIM_MODE_<application>
*
* ELEMENTS:
*   DIGI_TIM_MODE_OFF               - dTIM detection disable
*   DIGI_TIM_MODE_SAPI              - dTIM detection based on SAPI field
*   DIGI_TIM_MODE_DAPI              - dTIM detection based on DAPI field
*   DIGI_TIM_MODE_SAPI_DAPI         - dTIM detection based on SAPI + DAPI field
*
*******************************************************************************/
typedef enum
{
    DIGI_TIM_MODE_OFF       = 0x0,
    DIGI_TIM_MODE_SAPI,
    DIGI_TIM_MODE_DAPI,
    DIGI_TIM_MODE_SAPI_DAPI,

    LAST_DIGI_TIM_MODE, /* this is for out of range values error handling */
} digi_otn_tim_mode_t;

/*******************************************************************************
* ENUM: digi_gcc_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN GCC overhead type. The
*   element terms have a syntax:
*      DIGI_OTN_GCC_<application>
*
* ELEMENTS:
*   DIGI_OTN_GCC_0              - GCC0 overhead
*   DIGI_OTN_GCC_1              - GCC1 overhead
*   DIGI_OTN_GCC_2              - GCC2 overhead
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_GCC_0    = 0x0,
    DIGI_OTN_GCC_1,
    DIGI_OTN_GCC_2,
    
    LAST_DIGI_OTN_GCC, /* this is for out of range values error handling */
} digi_gcc_type_t;

/*******************************************************************************
* ENUM: digi_res_oh_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated RES overhead position for OTN. The
*   element terms have a syntax:
*      DIGI_RES_ROW_COLUMN
*
* ELEMENTS:
*   DIGI_RES_1_10             - RES row:1, column:10 BIT7 ~ BIT8
*   DIGI_RES_1_13             - RES row:1, column:13
*   DIGI_RES_1_14             - RES row:1, column:14
*   DIGI_RES_2_1              - RES row:2, column:1
*   DIGI_RES_2_2              - RES row:2, column:2
*   DIGI_RES_2_3              - RES row:2, column:3 BIT8
*   DIGI_RES_4_9              - RES row:4, column:9
*   DIGI_RES_4_10             - RES row:4, column:10
*   DIGI_RES_4_11             - RES row:4, column:11
*   DIGI_RES_4_12             - RES row:4, column:12
*   DIGI_RES_4_13             - RES row:4, column:13
*   DIGI_RES_4_14             - RES row:4, column:14
*
*******************************************************************************/
typedef enum
{
    DIGI_RES_1_10   = 0x0,
    DIGI_RES_1_13,
    DIGI_RES_1_14,
    DIGI_RES_2_1,
    DIGI_RES_2_2,
    DIGI_RES_2_3,
    DIGI_RES_4_9,
    DIGI_RES_4_10,
    DIGI_RES_4_11,
    DIGI_RES_4_12,
    DIGI_RES_4_13,
    DIGI_RES_4_14,
    
    LAST_DIGI_RES, /* this is for out of range values error handling */
} digi_res_oh_t;

/*******************************************************************************
* ENUM: digi_otn_loc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI traffic location type. The
*   element terms have a syntax:
*      DIGI_OTN_<application>
*
* ELEMENTS:
*   DIGI_OTN_LINE_SIDE           - line side location (stage 1/2/3A/N/4)
*   DIGI_OTN_SYS_SIDE            - system side location(stage 3B)
*
* Note:
*   As to stage4, either DIGI_OTN_LINE_SIDE or DIGI_OTN_SYS_SIDE can be selected.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_LINE_SIDE   = 0x0,
    DIGI_OTN_SYS_SIDE,
    
    LAST_DIGI_OTN_SIDE, /* this is for out of range values error handling */
} digi_otn_loc_t;

/*******************************************************************************
* ENUM: digi_otn_pos_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN position type. The
*   element terms have a syntax:
*      DIGI_OTN_<application>
*
* ELEMENTS:
*   DIGI_OTN_SINK_POS            - sink position
*   DIGI_OTN_SOURCE_POS          - source position
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_SINK_POS   = 0x0,
    DIGI_OTN_SOURCE_POS,
    
    LAST_DIGI_OTN_POS, /* this is for out of range values error handling */
} digi_otn_pos_t;

/*******************************************************************************
* ENUM: digi_tcm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN TCM mode. The
*   element terms have a syntax:
*      DIGI_TCM_<application>
*
* ELEMENTS:
*   DIGI_TCM_OPERATIONAL            - TCM OPERATIONAL mode
*   DIGI_TCM_TRANSPARENT            - TCM TRANSPARENT mode
*   DIGI_TCM_MONITOR                - TCM MONITOR mode
*   DIGI_TCM_NON_INTRUSIVE_MOINTOR  - TCM non-intrusive MONITOR mode
*   DIGI_TCM_OPERATIONAL_OHNZ       - TCM OPERATIONAL mode (The TCM OH is not 
*                                     zeroed out. It is only available at the 
*                                     sink postion.)
*   DIGI_TCM_ERASE                  - Zeros out the TCM overhead (G.798
*                                     MI_TCM_Extension - erase).
*
*******************************************************************************/
typedef enum
{
    DIGI_TCM_OPERATIONAL             = ODU_STRUCT_PM_TCM_TT_OPERATIONAL,
    DIGI_TCM_TRANSPARENT             = ODU_STRUCT_PM_TCM_TT_TRANSPARENT,
    DIGI_TCM_MONITOR                 = ODU_STRUCT_PM_TCM_TT_MONITOR,
    DIGI_TCM_NON_INTRUSIVE_MONITOR   = ODU_STRUCT_PM_TCM_NON_INTRUSIVE_MONITOR,
    DIGI_TCM_OPERATIONAL_OHNZ        = ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED,
    DIGI_TCM_ERASE                   = ODU_STRUCT_PM_TCM_TT_ERASE,

    LAST_DIGI_TCM, /* this is for out of range values error handling */
} digi_tcm_mode_t;

/*******************************************************************************
* ENUM: digi_tcm_pri_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN TCM process priority. The
*   element terms have a syntax:
*      DIGI_TCM_PRI_<application>
*
* ELEMENTS:
*   DIGI_TCM_PRI_0            - The highest priority
*   DIGI_TCM_PRI_1            - The second priority 
*   DIGI_TCM_PRI_2            - The third priority
*   DIGI_TCM_PRI_3            - The fourth priority
*   DIGI_TCM_PRI_4            - The fifth priority
*   DIGI_TCM_PRI_5            - The sixth priority
*   DIGI_TCM_PRI_6            - The lowest priority
*
*******************************************************************************/
typedef enum
{
    DIGI_TCM_PRI_0 = 0,
    DIGI_TCM_PRI_1 = 1,
    DIGI_TCM_PRI_2 = 2,
    DIGI_TCM_PRI_3 = 3,
    DIGI_TCM_PRI_4 = 4,
    DIGI_TCM_PRI_5 = 5,
    DIGI_TCM_PRI_6 = 6,

    LAST_DIGI_TCM_PRI, /* this is for out of range values error handling */
} digi_tcm_pri_t;

/*******************************************************************************
* ENUM: digi_dm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN DM mode. The
*   element terms have a syntax:
*      DIGI_DM_<application>
*
* ELEMENTS:
*   DIGI_DM_INITIATOR    - DMti/DMp overhead can be inserted by user.
*   DIGI_DM_LOOPBACK     - DMti/DMp overhead will be loopbacked from sink 
*                          position to source position.
*
*******************************************************************************/
typedef enum
{
    DIGI_DM_INITIATOR = 0x0,
    DIGI_DM_LOOPBACK,
    
    LAST_DIGI_DM, /* this is for out of range values error handling */
} digi_dm_mode_t;

/*******************************************************************************
* ENUM: digi_mt_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN maintenance signal type. The
*   element terms have a syntax:
*      DIGI_MT_<application>
*
* ELEMENTS:
*   DIGI_MT_NORMAL      - Normal signal.
*   DIGI_MT_AIS         - AIS signal.
*   DIGI_MT_LCK         - LCK signal. 
*   DIGI_MT_OCI         - OCI signal.
*   DIGI_MT_AIS_NOMINAL - AIS signal with the nominal clock.
*   DIGI_MT_LCK_NOMINAL - LCK signal with the nominal clock.
*
*******************************************************************************/
typedef enum
{
    DIGI_MT_NORMAL    = 0x0,
    DIGI_MT_AIS,
    DIGI_MT_LCK,
    DIGI_MT_OCI,
    DIGI_MT_AIS_NOMINAL,
    DIGI_MT_LCK_NOMINAL,

    LAST_DIGI_MT, /* this is for out of range values error handling */
} digi_mt_type_t;

/*******************************************************************************
* ENUM: digi_defect_act_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN defect consequent action. The
*   element terms have a syntax:
*      DIGI_<application>_ACT
*
* ELEMENTS:
*   DIGI_LTC_ACT         - dLTC consequent action.
*   DIGI_AIS_ACT         - dAIS consequent action.
*   DIGI_OCI_ACT         - dOCI consequent action.
*   DIGI_LCK_ACT         - dLCK consequent action.
*   DIGI_LOFLOM_ACT      - dLOFLOM consequent action.
*   DIGI_PLM_TO_SSF      - dPLM --> SSF.
*   DIGI_LOOMFI_TO_SSF   - dLOOMFI --> SSF.
*   DIGI_MSIM_TO_SSF     - dMSIM --> SSF.
*   DIGI_CSF_TO_SSF      - dCSF --> SSF.
*
*******************************************************************************/
typedef enum
{
    DIGI_LTC_ACT    = 0x0,
    DIGI_AIS_ACT,
    DIGI_OCI_ACT,
    DIGI_LCK_ACT,
    DIGI_LOFLOM_ACT,
    DIGI_PLM_TO_SSF,
    DIGI_LOOMFI_TO_SSF,
    DIGI_MSIM_TO_SSF,
    DIGI_CSF_TO_SSF,
    
    LAST_DIGI_ACT, /* this is for out of range values error handling */
} digi_defect_act_t;

/*******************************************************************************
* ENUM: digi_deg_interval_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN DEG performance collection interval. 
*   The element terms have a syntax:
*      DIGI_DEG_<application>
*
* ELEMENTS:
*   DIGI_DEG_10MS        - 10ms interval for performance collection.
*   DIGI_DEG_100MS       - 100ms interval for performance collection.
*   DIGI_DEG_1000MS      - 1000ms interval for performance collection.
*
*******************************************************************************/
typedef enum
{
    DIGI_DEG_10MS  = 0x0,
    DIGI_DEG_100MS,
    DIGI_DEG_1000MS,
    
    LAST_DIGI_DEG, /* this is for out of range values error handling */
} digi_deg_interval_t;

/*******************************************************************************
* ENUM: digi_oh_inst_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTU overhead insertion type. The
*   element terms have a syntax:
*      DIGI_OH_INST_<application>
*
* ELEMENTS:
*   DIGI_OH_INST_DISABLE     - overhead will not be inserted.
*   DIGI_OH_INST_ENABLE      - calculated BIP8 or OTU_Sk consequent action 
*                              value is inserted.
*
*                              Note: For MFAS field,  it is inserted via a 
*                              freewheeling counter based on the upstream MFAS.
*
*   DIGI_OH_INST_USER_VALUE  - constant user value insertion.
*
*******************************************************************************/
typedef enum
{
    DIGI_OH_INST_DISABLE = 0,
    DIGI_OH_INST_ENABLE,
    DIGI_OH_INST_USER_VALUE,

    LAST_DIGI_OH_INST, /* this is for out of range values error handling */
} digi_oh_inst_type_t;

/*******************************************************************************
* ENUM: digi_oh_field_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTU overhead field. The
*   element terms have a syntax:
*      DIGI_OTU_OH_<application>
*
* ELEMENTS:
*   DIGI_OTU_OH_BIP8        - BIP8 field overhead.
*   DIGI_OTU_OH_BDI         - BDI field overhead. 
*   DIGI_OTU_OH_BEI_BIAE    - BEI/BIAE field overhead.
*   DIGI_OTU_OH_IAE         - IAE field overhead.
*   DIGI_OTU_OH_MFAS        - MFAS field overhead.
*   DIGI_OTU_OH_FAS_F6      - FAS field overhead(first 3 FAS bytes).
*   DIGI_OTU_OH_FAS_28      - FAS field overhead(last 3 FAS bytes).
*
*******************************************************************************/
typedef enum
{
    DIGI_OTU_OH_BIP8 = 0,
    DIGI_OTU_OH_BDI,
    DIGI_OTU_OH_BEI_BIAE,
    DIGI_OTU_OH_IAE,
    DIGI_OTU_OH_MFAS,
    DIGI_OTU_OH_FAS_F6,
    DIGI_OTU_OH_FAS_28,

    LAST_DIGI_OTU_OH_INST, /* this is for out of range values error handling */
} digi_oh_field_t;

/*******************************************************************************
* ENUM: digi_source_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN source selection type. The
*   element terms have a syntax:
*      DIGI_SRC_<application>
*
* ELEMENTS:
*   DIGI_SRC_RX_FRAMER          - Local Rx Framer
*   DIGI_SRC_RI_RCP             - RI_RCP interface
*   DIGI_SRC_TX_FRAMER          - Force values at Tx Framer
*
*******************************************************************************/
typedef enum
{
    DIGI_SRC_RX_FRAMER    = 0x0,
    DIGI_SRC_RI_RCP,
    DIGI_SRC_TX_FRAMER,
    
    LAST_DIGI_OTN_SRC, /* this is for out of range values error handling */
} digi_source_type_t;

/*
** Structures and Unions
*/
typedef struct
{
    UINT32 bip;
    UINT32 nds;
    UINT32 piae;
    UINT32 bei;
    UINT32 fds;
    UINT32 pbiae;
}digi_otn_cnt_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_otn_fec_type_set(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   digi_otn_server_fec_t fec_type);
PUBLIC PMC_ERROR adpt_otn_fec_type_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   digi_otn_server_fec_t *fec_type);
PUBLIC PMC_ERROR adpt_otn_fec_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  BOOL fec_enable);
PUBLIC PMC_ERROR adpt_otn_fec_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  BOOL *fec_enable);
PUBLIC PMC_ERROR adpt_otn_fec_err_inst_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 err_rate,
                                      digi_otn_err_mode_t err_mode,
                                      BOOL   err_enable);
PUBLIC PMC_ERROR adpt_otn_fec_err_inst_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *err_rate,
                                      digi_otn_err_mode_t *err_mode,
                                      BOOL   *err_enable);
PUBLIC PMC_ERROR adpt_otn_tx_tti_set(UINT32 dev_id, 
                                UINT32 chnl_id, 
                                digi_otn_seg_t seg_id,
                                digi_otn_loc_t location,
                                UINT8 tti_data[64]);
PUBLIC PMC_ERROR adpt_otn_tx_tti_get(UINT32 dev_id, 
                                UINT32 chnl_id, 
                                digi_otn_seg_t seg_id,
                                digi_otn_loc_t location,
                                UINT8 *tti_data);
PUBLIC PMC_ERROR adpt_otn_rx_tti_lock(UINT32 dev_id, BOOL tti_lock);
PUBLIC PMC_ERROR adpt_otn_rx_tti_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_seg_t seg_id,
                                 digi_otn_loc_t location,
                                 UINT8 *tti_data);
PUBLIC PMC_ERROR adpt_otn_exp_tti_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 tti_data[64]);
PUBLIC PMC_ERROR adpt_otn_exp_tti_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 *tti_data);
PUBLIC PMC_ERROR adpt_otn_tim_det_mode_set(UINT32 dev_id, 
                                        UINT32 chnl_id, 
                                        digi_otn_seg_t seg_id,
                                        digi_otn_loc_t location,
                                        digi_otn_tim_mode_t tim_mode);
PUBLIC PMC_ERROR adpt_otn_tim_det_mode_get(UINT32 dev_id, 
                                        UINT32 chnl_id, 
                                        digi_otn_seg_t seg_id, 
                                        digi_otn_loc_t location,
                                        digi_otn_tim_mode_t *tim_mode);
PUBLIC PMC_ERROR adpt_otn_tim_act_dis_set(UINT32 dev_id, 
                                      UINT32 chnl_id, 
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL tim_act_dis);
PUBLIC PMC_ERROR adpt_otn_tim_act_dis_get(UINT32 dev_id, 
                                      UINT32 chnl_id, 
                                      digi_otn_seg_t seg_id, 
                                      digi_otn_loc_t location,
                                      BOOL *tim_act_dis);
PUBLIC PMC_ERROR adpt_otn_tx_pt_set(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 UINT8 pt_value);
PUBLIC PMC_ERROR adpt_otn_tx_pt_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 UINT8 *pt_value);
PUBLIC PMC_ERROR adpt_otn_rx_pt_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 UINT8 *pt_value);
PUBLIC PMC_ERROR adpt_otn_exp_pt_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT8 pt_value);
PUBLIC PMC_ERROR adpt_otn_exp_pt_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT8 *pt_value);
PUBLIC PMC_ERROR adpt_otn_tx_msi_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT8  trib_slot,
                                  UINT8 msi_value);
PUBLIC PMC_ERROR adpt_otn_tx_msi_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT8 *msi_value);
PUBLIC PMC_ERROR adpt_otn_rx_msi_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT8 *msi_value);
PUBLIC PMC_ERROR adpt_otn_exp_msi_set(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   UINT8  trib_slot,
                                   UINT8 msi_value);
PUBLIC PMC_ERROR adpt_otn_exp_msi_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   UINT8 *msi_value);
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_set(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_loc_t location,
                                 UINT8 ftfl_value[20]);
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_loc_t location,
                                 UINT8 *ftfl_value);
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_src_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location,
                                    BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_ftfl_src_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location,
                                    BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_ftfl_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_loc_t location,
                                 UINT8 *ftfl_value);
PUBLIC PMC_ERROR adpt_otn_tx_gcc_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_gcc_type_t gcc_type, 
                                  UINT16 gcc_value);
PUBLIC PMC_ERROR adpt_otn_tx_gcc_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_gcc_type_t gcc_type, 
                                  UINT16 *gcc_value);
PUBLIC PMC_ERROR adpt_otn_tx_gcc_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_gcc_type_t gcc_type,
                                     BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_gcc_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_gcc_type_t gcc_type,
                                     BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_gcc_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_gcc_type_t gcc_type, 
                                  UINT16 *gcc_value);
PUBLIC PMC_ERROR adpt_otn_tx_exp_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT16 exp_value);
PUBLIC PMC_ERROR adpt_otn_tx_exp_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT16 *exp_value);
PUBLIC PMC_ERROR adpt_otn_tx_exp_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_exp_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_exp_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT16 *exp_value);
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_set(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     UINT8 tcm_act_value);
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_get(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     UINT8 *tcm_act_value);
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_src_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_tcm_act_src_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_tcm_act_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     UINT8 *tcm_act_value);
PUBLIC PMC_ERROR adpt_otn_tx_res_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_res_oh_t res_type, 
                                  UINT8 res_value);
PUBLIC PMC_ERROR adpt_otn_tx_res_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_res_oh_t res_type, 
                                  UINT8 *res_value);
PUBLIC PMC_ERROR adpt_otn_tx_res_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_res_oh_t res_type,
                                     BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_res_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_res_oh_t res_type,
                                     BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_res_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_res_oh_t res_type, 
                                  UINT8 *res_value);
PUBLIC PMC_ERROR adpt_otn_tx_aps_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 aps_value);
PUBLIC PMC_ERROR adpt_otn_tx_aps_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 *aps_value);
PUBLIC PMC_ERROR adpt_otn_tx_aps_src_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     BOOL oh_inst);
PUBLIC PMC_ERROR adpt_otn_tx_aps_src_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     BOOL *oh_inst);
PUBLIC PMC_ERROR adpt_otn_rx_aps_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  UINT32 *aps_value);
PUBLIC PMC_ERROR adpt_otn_tcm_mode_set(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_otn_pos_t position, 
                                     digi_tcm_mode_t tcm_mode);
PUBLIC PMC_ERROR adpt_otn_tcm_mode_get(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     digi_otn_pos_t position, 
                                     digi_tcm_mode_t *tcm_mode);
PUBLIC PMC_ERROR adpt_otn_tcm_pri_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_tcm_pri_t tcm_pri[7]);
PUBLIC PMC_ERROR adpt_otn_tcm_pri_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position,
                                  digi_tcm_pri_t *tcm_pri);
PUBLIC PMC_ERROR adpt_otn_tcm_parent_set(UINT32 dev_id, 
                                      UINT32 chnl_id, 
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      digi_otn_seg_t p_seg_id);
PUBLIC PMC_ERROR adpt_otn_ltc_act_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  BOOL ltc_act);
PUBLIC PMC_ERROR adpt_otn_ltc_act_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  BOOL *ltc_act);
PUBLIC PMC_ERROR adpt_otn_ais_act_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL ais_act);
PUBLIC PMC_ERROR adpt_otn_ais_act_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  BOOL *ais_act);
PUBLIC PMC_ERROR adpt_otn_tsd_tsf_act_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL tsd_act,
                                     BOOL tsf_act);
PUBLIC PMC_ERROR adpt_otn_tsd_tsf_act_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL *tsd_act,
                                     BOOL *tsf_act);
PUBLIC PMC_ERROR adpt_otn_dm_mode_set(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_otn_seg_t seg_id, 
                                    digi_otn_loc_t location,
                                    digi_dm_mode_t dm_mode);
PUBLIC PMC_ERROR adpt_otn_dm_mode_get(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_otn_seg_t seg_id, 
                                    digi_otn_loc_t location,
                                    digi_dm_mode_t *dm_mode);
PUBLIC PMC_ERROR adpt_otn_deg_para_set(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_otn_seg_t seg_id, 
                                    digi_otn_loc_t location,
                                    UINT32 deg_th, 
                                    UINT32 deg_m);
PUBLIC PMC_ERROR adpt_otn_deg_para_get(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_otn_seg_t seg_id, 
                                    digi_otn_loc_t location,
                                    UINT32 *deg_th, 
                                    UINT32 *deg_m);
PUBLIC PMC_ERROR adpt_otn_deg_mode_set(UINT32 dev_id,
                                     digi_otn_ddeg_mon_seg_t odu_seg,
                                     digi_otn_ddeg_mode_t deg_mode);
PUBLIC PMC_ERROR adpt_otn_deg_mode_get(UINT32 dev_id,
                                     digi_otn_ddeg_mon_seg_t *odu_seg,
                                     digi_otn_ddeg_mode_t *deg_mode);
PUBLIC PMC_ERROR adpt_otn_enhanced_deg_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        UINT32 deg_th,
                                        digi_deg_interval_t interval);
PUBLIC PMC_ERROR adpt_otn_enhanced_deg_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_seg_t seg_id,
                                        UINT32 *deg_th,
                                        digi_deg_interval_t *interval);
PUBLIC PMC_ERROR adpt_otn_deg_mon_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    BOOL mon_en);
PUBLIC PMC_ERROR adpt_otn_mt_sig_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position, 
                                  digi_mt_type_t mt_type);
PUBLIC PMC_ERROR adpt_otn_mt_sig_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  digi_otn_pos_t position, 
                                  digi_mt_type_t *mt_type);
PUBLIC PMC_ERROR adpt_otn_bw_inst_set(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location,
                                   digi_source_type_t inst_src);
PUBLIC PMC_ERROR adpt_otn_tx_bdi_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  BOOL bdi_insert);
PUBLIC PMC_ERROR adpt_otn_tx_bdi_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  BOOL *bdi_insert);
PUBLIC PMC_ERROR adpt_otn_tx_bei_biae_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      UINT8 bei_biae);
PUBLIC PMC_ERROR adpt_otn_tx_bei_biae_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      UINT8 *bei_biae);
PUBLIC PMC_ERROR adpt_otn_tx_stat_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  UINT8 stat_value);
PUBLIC PMC_ERROR adpt_otn_tx_stat_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  UINT8 *stat_value);
PUBLIC PMC_ERROR adpt_otn_rx_stat_get(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_otn_seg_t seg_id, 
                                  digi_otn_loc_t location,
                                  UINT8 *stat_value);
PUBLIC PMC_ERROR adpt_otn_tx_iae_force(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_otn_conseq_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_defect_act_t defect_id,
                                      BOOL defect_act);
PUBLIC PMC_ERROR adpt_otn_conseq_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_defect_act_t defect_id,
                                      BOOL *defect_act);
PUBLIC PMC_ERROR adpt_otn_tcm_bw_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL bw_act);
PUBLIC PMC_ERROR adpt_otn_tcm_bw_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_seg_t seg_id,
                                      digi_otn_loc_t location,
                                      BOOL *bw_act);
PUBLIC PMC_ERROR adpt_otn_tx_otu_oh_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_oh_field_t oh_field,
                                     UINT32 oh_value,
                                     digi_oh_inst_type_t oh_type);
PUBLIC PMC_ERROR adpt_otn_tx_otu_oh_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_oh_field_t oh_field,
                                     UINT32 *oh_value,
                                     digi_oh_inst_type_t *oh_type);
PUBLIC PMC_ERROR adpt_otu_conseq_act_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      lineotn_action_type_t act_type,
                                      UINT32 defect_msk,
                                      BOOL defect_act);
PUBLIC PMC_ERROR adpt_otu_conseq_act_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      lineotn_action_type_t act_type,
                                      UINT32 defect_msk,
                                      UINT32 *defect_act);
PUBLIC PMC_ERROR adpt_otn_prbs_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 prbs_chid,
                                   digi_otn_prbs_pattern_t prbs_mode,
                                   BOOL so_prbs_en,
                                   BOOL sk_prbs_en);
PUBLIC PMC_ERROR adpt_otn_prbs_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 *prbs_chid,
                                   digi_otn_prbs_pattern_t *prbs_mode,
                                   BOOL *so_prbs_en,
                                   BOOL *sk_prbs_en);
PUBLIC PMC_ERROR adpt_otn_prbs_err_inst_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL err_insert);
PUBLIC PMC_ERROR adpt_otn_prbs_err_inst_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *err_insert);
PUBLIC PMC_ERROR adpt_otn_prbs_thresh_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT64 threshold,
                                      UINT16 reseed_thresh,
                                      UINT16 reseed_interval);
PUBLIC PMC_ERROR adpt_otn_prbs_thresh_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT64 *threshold);
PUBLIC PMC_ERROR adpt_otn_prbs_dlss_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL   *dlss_state);
PUBLIC PMC_ERROR adpt_otn_null_sig_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL null_enable);
PUBLIC PMC_ERROR adpt_otn_null_sig_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL *null_enable);
PUBLIC PMC_ERROR adpt_otn_opu_csf_force_inst_set(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            BOOL enable);
PUBLIC PMC_ERROR adpt_otn_opu_csf_force_inst_get(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            BOOL *enable);
PUBLIC PMC_ERROR adpt_otn_cm_status_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL *cm_out_limit_i,
                                     UINT32 *cm_value);
PUBLIC PMC_ERROR adpt_odu_ts_type_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_odu_line_payload_t ts_type);
PUBLIC PMC_ERROR adpt_odu_ts_type_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_odu_line_payload_t *ts_type);
PUBLIC PMC_ERROR adpt_otn_pm_reg_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_cnt_t *otn_cnt);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_OTN_H */

/* 
** end of file 
*/

