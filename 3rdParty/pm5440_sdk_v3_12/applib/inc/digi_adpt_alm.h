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
*     This file describes DIGI defects detection function. 
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_ALM_H
#define _DIGI_ADPT_ALM_H
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
* ENUM: digi_otn_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN defects. The
*   element terms have a syntax:
*      DIGI_OTN_<application>
*
* ELEMENTS:
*   DIGI_OTN_LOS         - dLOS(Loss of Signal) defect
*   DIGI_OTN_LOL         - dLOL(Loss of Lane Alignment) defect
*   DIGI_OTN_OTUAIS      - dOTUAIS(OTUk dAIS) defect
*   DIGI_OTN_LOF         - dLOF(Loss Of Frame) defect
*   DIGI_OTN_OOF         - dOOF(Out Of Frame) defect
*   DIGI_OTN_LOM         - dLOM(Loss of Multiframe) defect
*   DIGI_OTN_OOM         - dOOM(Out of Multiframe) defect
*   DIGI_OTN_LOFLOM      - dLOFLOM(Loss Of Frame and Loss Of Multiframe) defect
*   DIGI_OTN_IAE         - dIAE(Incoming Alignment Error) defect
*   DIGI_OTN_BIAE        - dBIAE(Backward Incoming Alignment Error) defect
*   DIGI_OTN_TIM         - dTIM(Trail trace Identifier Mismatch) defect
*   DIGI_OTN_BDI         - dBDI(Backward Defect Indication) defect
*   DIGI_OTN_DEG         - dDEG(Degraded defect) defect
*   DIGI_OTN_EDEG        - enhancement dDEG(Degraded defect) defect
*   DIGI_OTN_LTC         - dLTC(Loss of Tandem Connection) defect
*   DIGI_OTN_LCK         - dLCK(Locked defect) defect
*   DIGI_OTN_OCI         - dOCI(Open Connection Indication) defect
*   DIGI_OTN_AIS         - dAIS(Alarm Indication Signal) defect
*   DIGI_OTN_PLM         - dPLM(Payload Mismatch) defect
*   DIGI_OTN_MSIM        - dMSIM(Multiplex Structure Identifier Mismatch) defect
*   DIGI_OTN_CSF         - dCSF(Client Signal Fail) defect
*   DIGI_OTN_LOOMFI      - dLOOMFI(Loss of OPU Multi-Frame Identifier) defect
*   DIGI_OTN_LOFLANE     - dLOFLANE(Loss of Frame and Loss of Multiframe) defect
*   DIGI_OTN_OOR         - dOOR(Out of Recovery) defect
*   DIGI_OTN_LOR         - dLOR(Loss of Recovery) defect
*   DIGI_OTN_TSF         - aTSF(Trail Signal Fail)
*   DIGI_OTN_TSD         - aTSD(Trail Signal Degraded)
*   DIGI_OTN_SSF         - CI_SSF(Server Signal Fail)
*   DIGI_OTN_CND_MAX     - cnd_max_i interrupt
*   DIGI_OTN_JC_SYNCLOSS - jc_nomtch_lossync_i/jc_err_syncloss_i interrupt
*
* Note:
*   dLOFLANE defect, not listed here, is provided with the specific API.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_LOS   = 0x0,
    DIGI_OTN_LOL,
    DIGI_OTN_OTUAIS,
    DIGI_OTN_LOF,
    DIGI_OTN_OOF,
    DIGI_OTN_LOM,
    DIGI_OTN_LOFLOM,
    DIGI_OTN_OOM,
    DIGI_OTN_IAE,
    DIGI_OTN_BIAE,
    DIGI_OTN_TIM,
    DIGI_OTN_BDI,
    DIGI_OTN_DEG,
    DIGI_OTN_EDEG,
    DIGI_OTN_LTC,
    DIGI_OTN_LCK,
    DIGI_OTN_OCI,
    DIGI_OTN_AIS,
    DIGI_OTN_TSF,
    DIGI_OTN_TSD,
    DIGI_OTN_SSF,
    DIGI_OTN_PLM,
    DIGI_OTN_MSIM,
    DIGI_OTN_CSF,
    DIGI_OTN_LOOMFI,
    DIGI_OTN_LOFLANE,
    DIGI_OTN_OOR,
    DIGI_OTN_LOR,
    DIGI_OTN_CND_MAX,
    DIGI_OTN_JC_SYNCLOSS,

    LAST_DIGI_OTN_DEFECT, /* this is for out of range values error handling */
} digi_otn_defect_t;

/*******************************************************************************
* ENUM: digi_enet_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet defect status. The
*   element terms have a syntax:
*      DIGI_ENET_<application>
*
* ELEMENTS:
*   DIGI_ENET_LOS          - physical LOS
*   DIGI_ENET_LF           - local fault
*   DIGI_ENET_RF           - remote fault
*   DIGI_ENET_HI_BER       - high ber 
*   DIGI_ENET_BLK_LCK      - loss of block lock
*   DIGI_ENET_ALIGN_STATUS - Not aligned in PCS block
*   DIGI_ENET_LINK_DOWN    - link down
*   DIGI_ENET_LOAM         - loss of alignment marker
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_LOS = 0,
    DIGI_ENET_LF,
    DIGI_ENET_RF,
    DIGI_ENET_HI_BER,
    DIGI_ENET_BLK_LCK,
    DIGI_ENET_ALIGN_STATUS,
    DIGI_ENET_LINK_DOWN,
    DIGI_ENET_LOAM,

    LAST_DIGI_ENET_DEFECT, /* this is for out of range values error handling */
} digi_enet_defect_t;

/*******************************************************************************
* ENUM: digi_sdh_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SONET/SDH defects. The
*   element terms have a syntax:
*      DIGI_SDH_<application>
*
* ELEMENTS:
*   DIGI_SDH_LOS         - dLOS(Loss Of Signal) defect
*   DIGI_SDH_GAIS        - general dAIS(PN11 sequence) defect
*   DIGI_SDH_LOF         - dLOF(Loss Of Frame) defect
*   DIGI_SDH_OOF         - dLOF(Out Of Frame) defect
*   DIGI_SDH_RS_TIM      - RSdTIM(Trail trace Identifier Mismatch) defect
*   DIGI_SDH_RS_TIU      - RSdTIU(Trail trace Identifier Unstable) defect
*   DIGI_SDH_RS_DEG      - RSdDEG(Degraded defect) defect
*   DIGI_SDH_RS_EXC      - RSdEXC(Excessive Error defect) defect
*   DIGI_SDH_MS_AIS      - MSdAIS(Alarm Indication Signal) defect
*   DIGI_SDH_MS_RDI      - MSdRDI(Remote Defect Indication) defect
*   DIGI_SDH_MS_DEG      - MSdDEG(Degraded defect) defect
*   DIGI_SDH_MS_EXC      - MSdEXC(Excessive Error defect) defect
*
*******************************************************************************/
typedef enum
{
    DIGI_SDH_LOS  = 0,
    DIGI_SDH_GAIS,
    DIGI_SDH_LOF,
    DIGI_SDH_OOF,
    DIGI_SDH_RS_TIM,
    DIGI_SDH_RS_TIU,
    DIGI_SDH_RS_DEG,
    DIGI_SDH_RS_EXC,
    DIGI_SDH_MS_AIS,
    DIGI_SDH_MS_RDI,
    DIGI_SDH_MS_DEG,
    DIGI_SDH_MS_EXC,
    
    LAST_DIGI_SDH, /* this is for out of range values error handling */
} digi_sdh_defect_t;

/*******************************************************************************
* ENUM: digi_gfp_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for GFP defects. The
*   element terms have a syntax:
*      DIGI_GFP_<application>
*
* ELEMENTS:
*   DIGI_GFP_LFD            - dLFD(Loss of Frame Delineation) defect.
*   DIGI_GFP_EXM            - dEXM(Extension Header Mismatch) defect.
*   DIGI_GFP_UPM            - dUPM(User Payload Mismatch) defect.
*   DIGI_GFP_CSF            - dCSF-LOS(Loss Of client Signal) defect
*                             or dCSF-LOCS(Loss Of Character Synchronization) defect                      
*                             or dCSF-FDI(Forward Defect Indication) defect
*                             or dCSF-RDI(Reverse Defect Indication) defect.
*   DIGI_GFP_IDLE           - dIDLE(idle frame) defect.
*
*******************************************************************************/
typedef enum
{
    DIGI_GFP_LFD      = 0,
    DIGI_GFP_EXM,
    DIGI_GFP_UPM,
    DIGI_GFP_CSF,
    DIGI_GFP_IDLE,
    
    LAST_DIGI_GFP, /* this is for out of range values error handling */
} digi_gfp_defect_t;

/*******************************************************************************
* ENUM: digi_fc1200_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FC1200 defects. The
*   element terms have a syntax:
*      DIGI_FC_<application>
*
* ELEMENTS:
*   DIGI_FC_RX_RF              - remote fault defect.
*   DIGI_FC_RX_LF              - local fault defect.
*   DIGI_FC_RX_HIGH_BER        - high-BER defect.
*   DIGI_FC_RX_LOSS_SYNC       - loss of block synchronization state defect.
*   DIGI_FC_RX_LOS             - loss-of-signal defect.
*   DIGI_FC_TX_LF              - local fault defect.
*
*******************************************************************************/
typedef enum
{
    DIGI_FC_RX_RF  = 0,
    DIGI_FC_RX_LF,
    DIGI_FC_RX_HIGH_BER,
    DIGI_FC_RX_LOSS_SYNC,
    DIGI_FC_RX_LOS,
    DIGI_FC_TX_LF,

    LAST_DIGI_FC_1200, /* this is for out of range values error handling */
} digi_fc1200_defect_t;

/*******************************************************************************
* ENUM: digi_fc800_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FC800 defects. The
*   element terms have a syntax:
*      DIGI_FC_<application>
*
* ELEMENTS:
*   DIGI_FC_DISP_INV           - the state of the receive disparity inversion logic.
*   DIGI_FC_XDET               - the transaction detect state.
*   DIGI_FC_DLOLB              - the state of the receive DLOLB.
*   DIGI_FC_ASD                - the state of the analog signal detect.
*   DIGI_FC_SYNC               - the state of the receive word synchronizer.
*   DIGI_FC_LCV                - the state of line code violations .
*
*******************************************************************************/
typedef enum
{
    DIGI_FC_DISP_INV  = 0,
    DIGI_FC_XDET,
    DIGI_FC_DLOLB,
    DIGI_FC_ASD,
    DIGI_FC_SYNC,
    DIGI_FC_LCV,

    LAST_DIGI_FC_800, /* this is for out of range values error handling */
} digi_fc800_defect_t;

/*******************************************************************************
* ENUM: digi_blk_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI block. The
*   element terms have a syntax:
*      DIGI_BLK_<application>
*
* ELEMENTS:
*   DIGI_BLK_ENET          - ENET(line) and ENET(sys)
*   DIGI_BLK_OTN           - optimized COREOTN and LINEOTN
*   DIGI_BLK_MAP           - MAPOTN
*   DIGI_BLK_SIFD          - SIFD 
*   DIGI_BLK_CBR           - CBR
*   DIGI_BLK_ALL           - all blocks
*   DIGI_BLK_OTN_ALL       - full COREOTN and LINEOTN
*   DIGI_BLK_FULL          - all blocks with full COREOTN and LINEOTN
*
*******************************************************************************/
typedef enum
{
    DIGI_BLK_ENET     = 0x0001,
    DIGI_BLK_OTN      = 0x0002,
    DIGI_BLK_MAP      = 0x0004,
    DIGI_BLK_SIFD     = 0x0008,
    DIGI_BLK_CBR      = 0x0010,
    DIGI_BLK_ALL      = 0x001F,

    DIGI_BLK_OTN_ALL  = 0x0100,
    DIGI_BLK_FULL     = 0x011F,

    LAST_DIGI_BLK, /* this is for out of range values error handling */
} digi_blk_type_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_defect_trigger(UINT32 dev_id, digi_blk_type_t blk_msk);
PUBLIC PMC_ERROR adpt_otn_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_defect_t defect_id,
                                  BOOL *defect_status);
PUBLIC PMC_ERROR adpt_otn_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_defect_t defect_id,
                                   BOOL *defect_i);
PUBLIC PMC_ERROR adpt_otn_mld_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 lane_id,
                                digi_otn_defect_t defect_id,
                                BOOL *defect_status);
PUBLIC PMC_ERROR adpt_otn_mld_i_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 lane_id,
                                 digi_otn_defect_t defect_id,
                                 BOOL *defect_i);
PUBLIC PMC_ERROR adpt_enet_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_defect_t defect_id,
                                  BOOL *defect_status);
PUBLIC PMC_ERROR adpt_enet_defect_i_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_enet_defect_t defect_id,
                                    BOOL *defect_i);
PUBLIC PMC_ERROR adpt_enet_pcs_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                UINT32 lane_id,
                                digi_enet_defect_t defect_id,
                                BOOL *defect_status);
PUBLIC PMC_ERROR adpt_enet_pcs_i_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 lane_id,
                                  digi_enet_defect_t defect_id,
                                  BOOL *defect_i);
PUBLIC PMC_ERROR adpt_sdh_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_defect_t defect_id,
                                  BOOL *defect_status);
PUBLIC PMC_ERROR adpt_sdh_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_sdh_defect_t defect_id,
                                   BOOL *defect_i);
PUBLIC PMC_ERROR adpt_gfp_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gfp_defect_t defect_id,
                                  BOOL *defect_status);
PUBLIC PMC_ERROR adpt_gfp_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_gfp_defect_t defect_id,
                                   BOOL *defect_i);
PUBLIC PMC_ERROR adpt_fc1200_defect_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_defect_t defect_id,
                                    BOOL *defect_status);
PUBLIC PMC_ERROR adpt_fc1200_defect_i_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_fc1200_defect_t defect_id,
                                      BOOL *defect_i);
PUBLIC PMC_ERROR adpt_fc800_defect_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_defect_t defect_id,
                                   BOOL *defect_status);
PUBLIC PMC_ERROR adpt_fc800_defect_i_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_fc800_defect_t defect_id,
                                     BOOL *defect_i);
PUBLIC PMC_ERROR adpt_device_int_get(UINT32 dev_id, digi_device_int_t *int_table);
PUBLIC PMC_ERROR adpt_otn_odu_int_start(UINT32 dev_id, coreotn_latch_defect_t latch_defect);
PUBLIC PMC_ERROR adpt_otn_odu_int_end(UINT32 dev_id, coreotn_latch_defect_t latch_defect);
PUBLIC PMC_ERROR adpt_otn_ddeg_int_start(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_otn_ddeg_int_end(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_otn_server_int_chnl_filt_retrieve(UINT32 dev_id,
                                               UINT32 chnl_id,
                                               lineotn_int_chnl_t *filt_table,
                                               lineotn_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_otn_server_int_chnl_clear(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          lineotn_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_otn_odu_int_chnl_filt_retrieve(UINT32 dev_id,
                                             UINT32 chnl_id,
                                             digi_otn_loc_t location,
                                             coreotn_int_framer_chnl_t *framer_filt_table,
                                             odu_rfrm_int_pid_chnl_t *pid_filt_table,
                                             digi_otn_odu_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_otn_odu_int_chnl_clear(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_loc_t location,
                                        digi_otn_odu_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_otn_int_filt_retrieve(UINT32 dev_id,
                                     digi_otn_int_filter_t *int_filter,
                                     digi_otn_int_t *int_table);
PUBLIC PMC_ERROR adpt_otn_int_clear(UINT32 dev_id, digi_otn_int_t *int_table);
PUBLIC PMC_ERROR adpt_mapper_int_chnl_filt_retrieve(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_mapper_int_chnl_t *filt_table,
                                            digi_mapper_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_mapper_int_chnl_clear(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_mapper_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_mapper_int_filt_retrieve(UINT32 dev_id,
                                        digi_mapper_int_chnl_t *filt_table,   
                                        digi_mapper_int_t *int_table);
PUBLIC PMC_ERROR adpt_mapper_int_clear(UINT32 dev_id, digi_mapper_int_t *int_table);
PUBLIC PMC_ERROR adpt_enet_int_chnl_filt_retrieve(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_int_chnl_t *filt_table,   
                                          digi_enet_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_enet_line_int_chnl_clear(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_enet_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_enet_sys_int_chnl_clear(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_enet_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_enet_line_int_filt_retrieve(UINT32 dev_id,
                                         digi_enet_int_chnl_t *filt_table,   
                                         digi_enet_int_t *int_table);
PUBLIC PMC_ERROR adpt_enet_line_int_clear(UINT32 dev_id, digi_enet_int_t *int_table);
PUBLIC PMC_ERROR adpt_enet_sys_int_filt_retrieve(UINT32 dev_id,
                                         digi_enet_int_chnl_t *filt_table,   
                                         digi_enet_int_t *int_table);
PUBLIC PMC_ERROR adpt_enet_sys_int_clear(UINT32 dev_id, digi_enet_int_t *int_table);
PUBLIC PMC_ERROR adpt_enet_int_chnl_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_enet_int_t *int_table,
                                    digi_enet_int_chnl_t **int_chnl_table);
PUBLIC PMC_ERROR adpt_cbr_int_chnl_filt_retrieve(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_cbr_int_chnl_t *filt_table,   
                                          digi_cbr_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_cbr_int_chnl_clear(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_cbr_int_chnl_t *int_table);
PUBLIC PMC_ERROR adpt_cbr_int_filt_retrieve(UINT32 dev_id,
                                     digi_cbr_int_chnl_t *filt_table,   
                                     digi_cbr_int_t *int_table);
PUBLIC PMC_ERROR adpt_cbr_int_clear(UINT32 dev_id, digi_cbr_int_t *int_table);
PUBLIC PMC_ERROR adpt_cbr_int_chnl_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_cbr_int_t *int_table,
                                   digi_cbr_int_chnl_t **int_chnl_table);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_ALM_H */

/* 
** end of file 
*/

