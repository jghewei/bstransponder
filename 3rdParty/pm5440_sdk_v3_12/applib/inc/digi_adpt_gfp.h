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
*     The head file describes the GFP Run-Time functionality of CBRC subsystem.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_GFP_H
#define _DIGI_ADPT_GFP_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_map_api.h"

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
* ENUM: digi_gfp_fcs_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FCS mode in Tx GFP. The
*   element terms have a syntax:
*      DIGI_GFP_<application>
*
* ELEMENTS:
*   DIGI_GFP_NO_FCS            - have no any FCS.
*   DIGI_GFP_PFCS              - Insert pFCS for GFP Frame.
*   DIGI_GFP_EFCS_4_BYTES      - Calculate eFCS for special Ethernet traffic
*                                received from the CPB excluding preamble bytes
*                                after skipping 4-bytes of the packet.
*   DIGI_GFP_EFCS_8_BYTES      - Calculate eFCS for special Ethernet traffic 
*                                received from the CPB excluding preamble bytes
*                                after skipping 8-bytes of the packet.
*   DIGI_GFP_EFCS_12_BYTES     - Calculate eFCS for special Ethernet traffic 
*                                received from the CPB excluding preamble bytes
*                                after skipping 12-bytes of the packet.
*   DIGI_GFP_EFCS_16_BYTES     - Calculate eFCS for special Ethernet traffic 
*                                received from the CPB excluding preamble bytes
*                                after skipping 16-bytes of the packet
*   DIGI_GFP_EFCS              - Calculate eFCS for special Ethernet traffic 
*                                received from the CPB including the preamble 
*                                byte
*
*******************************************************************************/
typedef enum
{
    DIGI_GFP_NO_FCS        = MAPOTN_NO_FCS,
    DIGI_GFP_PFCS          = MAPOTN_PFCS,
    DIGI_GFP_EFCS_4_BYTES  = MAPOTN_EFCS_4_BYTES,
    DIGI_GFP_EFCS_8_BYTES  = MAPOTN_EFCS_8_BYTES,
    DIGI_GFP_EFCS_12_BYTES = MAPOTN_EFCS_12_BYTES,
    DIGI_GFP_EFCS_16_BYTES = MAPOTN_EFCS_16_BYTES,
    DIGI_GFP_EFCS          = MAPOTN_EFCS,

    LAST_DIGI_GFP_FCS, /* this is for out of range values error handling */
} digi_gfp_fcs_mode_t;

/*******************************************************************************
* ENUM: digi_gfp_rx_fcs_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FCS check mode in Rx GFP. The
*   element terms have a syntax:
*      DIGI_GFP_RX_<application>
*
* ELEMENTS:
*   DIGI_GFP_RX_NO_FCS         - No FCS check.
*   DIGI_GFP_RX_PFCS           - pFCS check.
*   DIGI_GFP_RX_EFCS_N_BYTES   - eFCS check after skipping programmed number of
*                                preambles.
*   DIGI_GFP_RX_EFCS           - eFCS check from 1st byte onwards (no preamble
*                                skipping).
*
*******************************************************************************/
typedef enum
{
    DIGI_GFP_RX_NO_FCS        = 0x0,
    DIGI_GFP_RX_PFCS          = 0x1,
    DIGI_GFP_RX_EFCS_N_BYTES  = 0x2,
    DIGI_GFP_RX_EFCS          = 0x3,

    LAST_DIGI_GFP_RX_FCS, /* this is for out of range values error handling */
} digi_gfp_rx_fcs_mode_t;

/*******************************************************************************
* ENUM: digi_gfp_cmf_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for CMF extract mode in GFP. The
*   element terms have a syntax:
*      DIGI_GFP_CMF_<application>
*
* ELEMENTS:
*   DIGI_GFP_CMF_CPU            - Extract CMF from CPU FIFO.
*   DIGI_GFP_CMF_INT            - Extract CMF from last stored memory through 
*                                 interrupt.
*
*******************************************************************************/
typedef enum
{
    DIGI_GFP_CMF_CPU       = 0,
    DIGI_GFP_CMF_INT       = 1,

    LAST_DIGI_GFP_CMF, /* this is for out of range values error handling */
} digi_gfp_cmf_mode_t;

/*******************************************************************************
* ENUM: digi_alm_act_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN OPU-CSF or GFP defect mask. The
*   element terms have a syntax:
*      DIGI_ACT_<application>
*
* ELEMENTS:
*   DIGI_ACT_ENET_PHY_LOS         - ENET Link Down (PHY LOS) defect
*   DIGI_ACT_ENET_BLK_LCK         - ENET PCS loss of block sync defect
*   DIGI_ACT_ENET_HI_BER          - ENET Excessive error ratio (HiBER) defect
*   DIGI_ACT_ENET_RX_LF           - ENET ingress LF defect
*   DIGI_ACT_ENET_RX_RF           - ENET ingress RF defect
*   DIGI_ACT_CBRC_LOS_SYNC        - CBRC 8B10B RX_SYNC defect
*   DIGI_ACT_CBRC_LOS_SIG         - CBRC 8B10B LOS defect
*   DIGI_ACT_FC1200_LOS_SYNC      - CBRC FC1200 LOSS_SYNC defect
*   DIGI_ACT_FC1200_LOS_SIG       - CBRC FC1200 LOS defect
*   DIGI_ACT_SDH_LOS_SIG          - SDH/SONET LOS_S defect
*   DIGI_ACT_SDH_LOS_FRM          - SDH/SONET LOF defect
*   DIGI_ACT_EPMM_LOS             - EPMM LOS defect for G.sup43
*   DIGI_ACT_EPMM_LF              - EPMM LF(Local Fault) defect for G.sup43
*   DIGI_ACT_EPMM_RF              - EPMM RF(Remote Fault) defect for G.sup43
*   DIGI_ACT_SW_FORCE             - software force/manual trigger
*   DIGI_ACT_ENET_LOAM            - ENET MAC 40/100G LOAM defect
*   DIGI_ACT_TX_ALL               - all defects
*
*******************************************************************************/
typedef enum
{
    DIGI_ACT_ENET_PHY_LOS    = 0x1,
    DIGI_ACT_ENET_BLK_LCK    = 0x2,
    DIGI_ACT_ENET_HI_BER     = 0x4,
    DIGI_ACT_ENET_RX_LF      = 0x8,
    DIGI_ACT_ENET_RX_RF      = 0x10,
    DIGI_ACT_CBRC_LOS_SYNC   = 0x20,
    DIGI_ACT_CBRC_LOS_SIG    = 0x40,
    DIGI_ACT_FC1200_LOS_SYNC = 0x80,
    DIGI_ACT_FC1200_LOS_SIG  = 0x100,
    DIGI_ACT_SDH_LOS_SIG     = 0x200,
    DIGI_ACT_SDH_LOS_FRM     = 0x400,
    DIGI_ACT_EPMM_LOS        = 0x800,
    DIGI_ACT_EPMM_LF         = 0x1000,
    DIGI_ACT_EPMM_RF         = 0x2000,
    DIGI_ACT_SW_FORCE        = 0x4000,
    DIGI_ACT_ENET_LOAM       = 0x8000,
    DIGI_ACT_TX_ALL          = 0xFFFF,

    LAST_DIGI_ACT_BLK, /* this is for out of range values error handling */
} digi_alm_act_mask_t;

/*******************************************************************************
* ENUM: digi_gfp_tx_act_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the action for Tx direction.
*
* ELEMENTS:
*
*   DIGI_NO_ACTION                  - Nothing                               
*   DIGI_GFP_CMF_CSF_LOS            - Insert GFP-F CMF-CSF (loss client)    
*   DIGI_GFP_CMF_CSF_LOCS           - Insert GFP-F CMF-CSF (loss char)      
*   DIGI_GFP_CMF_DCI                - Insert GFP-F CMF-DCI                  
*   DIGI_GFP_CMF_FDI                - Insert GFP-F CMF-FDI                  
*   DIGI_GFP_CMF_RDI                - Insert GFP-F CMF-RDI                  
*   DIGI_OPU_CSF                    - Insert OPU-CSF                        
*   DIGI_CBRC_FORCE_FAULT           - Insert CBRC ingress replacement (NOS) 
*                                     CPRI(BMP), GDPS(BMP), FC800(BMP), FC1200(BMP)
*   DIGI_GSUP43_C73_LF              - Insert enet epmm ingress local fault
*   DIGI_GFP_ORDERED_SET            - Insert epmm gfp ordered set
*   DIGI_LF_LEGACY_MODE             - Insert enet epmm ingress local fault for legacy 
*                                     mode under G.sup43 7.3 mapping
*   DIGI_LF_HIGH_FREQ_MODE          - Insert enet epmm ingress local fault with high 
*                                     freq ordered sets under G.sup43 7.3 mapping
*   DIGI_GFP_CSF_LOS_WITH_SQCH      - Insert GFP-F CMF-CSF (loss client) with packet 
*                                     squelching for GSUP6.2
*   DIGI_GFP_CSF_LOS_WITH_SQCH_C73  - Insert GFP-F CMF-CSF (loss client) with packet 
*                                     squelching for GSUP7.3
*   DIGI_GFP_FDI_WITH_SQCH          - Insert GFP-F CMF-FCI with packet squelching for 
*                                     GSUP6.2
*   DIGI_ENET_EPMM_40G_LF           - Insert enet epmm ingress local fault (40G)
*
*******************************************************************************/
typedef enum
{
    DIGI_NO_ACTION                 = NO_C2T_ACTION,
    DIGI_GFP_CMF_CSF_LOS           = INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL,
    DIGI_GFP_CMF_CSF_LOCS          = INSERT_GFP_CMF_CSF_LOSS_CHAR_SYNC,
    DIGI_GFP_CMF_DCI               = INSERT_GFP_CMF_DCI,
    DIGI_GFP_CMF_FDI               = INSERT_GFP_CMF_FDI,
    DIGI_GFP_CMF_RDI               = INSERT_GFP_CMF_RDI,
    DIGI_OPU_CSF                   = INSERT_OPU_CSF,
    DIGI_CBRC_FORCE_FAULT          = INSERT_CBRC_FORCE_FAULT,
    DIGI_GSUP43_C73_LF             = INSERT_ENET_ING_GSUP43_C73_LF,
    DIGI_GFP_ORDERED_SET           = INSERT_ENET_EPMM_GFP_ORDERED_SET,
    DIGI_LF_LEGACY_MODE            = INSERT_ENET_ING_GSUP43_C73_LF_LEGACY_MODE,
    DIGI_LF_HIGH_FREQ_MODE         = INSERT_ENET_ING_GSUP43_C73_LF_HIGH_FREQ_MODE,
    DIGI_GFP_CSF_LOS_WITH_SQCH     = INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL_WITH_PACKET_SQUELCH,
    DIGI_GFP_CSF_LOS_WITH_SQCH_C73 = INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL_WITH_PACKET_SQUELCH_GSUP73,
    DIGI_GFP_FDI_WITH_SQCH         = INSERT_GFP_CMF_FDI_WITH_PACKET_SQUELCH,
    DIGI_ENET_EPMM_40G_LF          = INSERT_ENET_EPMM_40G_LF,   
} digi_gfp_tx_act_t;

/*******************************************************************************
* ENUM: digi_rx_alm_act_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for client traffic defect mask. The
*   element terms have a syntax:
*      DIGI_ACT_<application>
*
* ELEMENTS:
*   DIGI_ACT_SSF_SF               - SSF_SF defect
*   DIGI_ACT_PLM                  - dPLM defect
*   DIGI_ACT_OPU_CSF              - OPUdCSF defect
*   DIGI_ACT_GFP_LFD              - GFPdLFD defect
*   DIGI_ACT_GFP_UPM              - GFPdUPM defect
*   DIGI_ACT_GFP_CSF_LOS          - GFPdCSF_LOS(Loss Of client Signal) defect
*   DIGI_ACT_GFP_CSF_LOCS         - GFPdCSF_LOCS(Loss Of Character Sync) defect
*   DIGI_ACT_GFP_FDI              - GFPdFDI defect
*   DIGI_ACT_GFP_RDI              - GFPdRDI defect
*   DIGI_ACT_SDH_SFIS_LOF         - SDHdLOF defect
*   DIGI_ACT_GFP_EXM              - GFPdEXM defect
*   DIGI_ACT_RX_ALL               - all defects
*
*******************************************************************************/
typedef enum
{
    DIGI_ACT_SSF_SF       = 0x1,
    DIGI_ACT_PLM          = 0x2,
    DIGI_ACT_OPU_CSF      = 0x4,
    DIGI_ACT_GFP_LFD      = 0x8,
    DIGI_ACT_GFP_UPM      = 0x10,
    DIGI_ACT_GFP_CSF_LOS  = 0x20,
    DIGI_ACT_GFP_CSF_LOCS = 0x40,
    DIGI_ACT_GFP_FDI      = 0x80,
    DIGI_ACT_GFP_RDI      = 0x100,
    DIGI_ACT_SDH_SFIS_LOF = 0x200,
    DIGI_ACT_GFP_EXM      = 0x400,
    DIGI_ACT_RX_ALL       = 0x7FF,

    LAST_DIGI_RX_ACT_BLK, /* this is for out of range values error handling */
} digi_rx_alm_act_mask_t;

/*******************************************************************************
* ENUM: digi_gfp_rx_act_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the action for Rx direction.
*
* ELEMENTS:
*
*   DIGI_RX_NO_ACTION            -  Do Nothing                
*   DIGI_FORCE_C62_LF            -  Force GE C62 LF 
*   DIGI_FORCE_C73_LF            -  Force GE C73 LF           
*   DIGI_FORCE_C71_C72_LF        -  Force GE C71 72 LF        
*   DIGI_FORCE_40G_LF            -  Force 40G GMP LF          
*   DIGI_FORCE_100G_LF           -  Force GE 100G GMP LF
*   DIGI_FORCE_C62_RF            -  Force GE C62 RF           
*   DIGI_FORCE_C73_RF            -  Force GE C73 RF           
*   DIGI_FORCE_C71_C72_RF        -  Force GE C71 72 RF        
*   DIGI_FORCE_40G_RF            -  Force 40G GMP RF          
*   DIGI_FORCE_100G_RF           -  Force 100G GMP RF         
*   DIGI_FORCE_TX_IDLE_C62_GFP   -  Force Egress Idle (C6.2, 40G GFP, 100G GFP)  
*   DIGI_T2T_GFP_CMF_RDI         -  Insert GFP RDI into trunk 
*   DIGI_FORCE_TX_IDLE_C73       -  Force Egress Idle (enet mode C7.3) 
*   DIGI_SQUELCH_PACKETS_ETRANS  -  Squelch packets (datapaths going through ENET(ETRANS->EMAC)
*   DIGI_FORCE_CBRC_FORCE_FAULT  -  Insert CBRC force fault  
*   DIGI_FORCE_ILKN_PIF_SQUELCH  -  FORCE_squelch on direct gfp to ilkn(pif) - into the trunk
*   DIGI_SDH_TX_FORCE_FAULT      -  Insert SONET force fault pattern for sfis mode
*   DIGI_FORCE_C62_LF_NFAC       -  Force LF NO FAULT AUTO CLEAR (Gsup43 6.2, G7041 7.1)
*   DIGI_FORCE_C62_LF_ENET_SYS   -  Force GE LF on ENET SYS (Gsup43 6.2, G7041 7.1)
*   DIGI_FORCE_T2T_OPU_CSF       -  Insert an OPU CSF - into the trunk
*   DIGI_FORCE_T2T_GFP_CSF_LOS   -  Insert GFP CMF CSF Loss of Signal - into the trunk
*   DIGI_FORCE_C73_LF_NULL_FRM   -  Force GE LF (Gsup43 7.3) with NULL framer
*   DIGI_FORCE_C62_LF_NULL_FRM   -  Force GE LF (Gsup43 6.2) with NULL framer
*   DIGI_T2T_GFP_CMF_RDI_NO_SQUL -  Add GFP CMF RDI with no data squelching
*   DIGI_T2T_GFP_CMF_RDI_TXENA   -  Add GFP CMF RDI action that doesn't use SEND_IDLE
*   DIGI_T2T_RX_EMAC_SEND_RF     -  Add an EMAC SEND_RF action
*
*******************************************************************************/
typedef enum
{
    DIGI_RX_NO_ACTION            = NO_T2C_ACTION,
    DIGI_FORCE_C62_LF            = FORCE_C62_LF,
    DIGI_FORCE_C73_LF            = FORCE_C73_LF,
    DIGI_FORCE_C71_C72_LF        = FORCE_C71_C72_LF,
    DIGI_FORCE_40G_LF            = FORCE_40G_LF,
    DIGI_FORCE_100G_LF           = FORCE_100G_LF,
    DIGI_FORCE_C62_RF            = FORCE_C62_RF,
    DIGI_FORCE_C73_RF            = FORCE_C73_RF,
    DIGI_FORCE_C71_C72_RF        = FORCE_C71_C72_RF,
    DIGI_FORCE_40G_RF            = FORCE_40G_RF,
    DIGI_FORCE_100G_RF           = FORCE_100G_RF,
    DIGI_FORCE_TX_IDLE_C62_GFP   = FORCE_EGRESS_IDLE_C62_GFP,
    DIGI_T2T_GFP_CMF_RDI         = INSERT_T2T_GFP_CMF_RDI,
    DIGI_FORCE_TX_IDLE_C73       = FORCE_EGRESS_IDLE_C73,
    DIGI_SQUELCH_PACKETS_ETRANS  = SQUELCH_PACKETS_ETRANS,
    DIGI_FORCE_CBRC_FORCE_FAULT  = FORCE_CBRC_FORCE_FAULT,
    DIGI_FORCE_ILKN_PIF_SQUELCH  = FORCE_DIRECT_ILKN_PIF_SQUELCH,
    DIGI_SDH_TX_FORCE_FAULT      = INSERT_SONET_EGRESS_FORCE_FAULT,
    DIGI_FORCE_C62_LF_NFAC       = FORCE_C62_LF_NO_FAULT_AUTO_CLEAR,
    DIGI_FORCE_C62_LF_ENET_SYS   = FORCE_C62_LF_ENET_SYS,
    DIGI_FORCE_T2T_OPU_CSF       = FORCE_T2T_OPU_CSF,
    DIGI_FORCE_T2T_GFP_CSF_LOS   = FORCE_T2T_GFP_CMF_CSF_LOS,
    DIGI_FORCE_C73_LF_NULL_FRM   = FORCE_C73_LF_WITH_NULL_FRAME,
    DIGI_FORCE_C62_LF_NULL_FRM   = FORCE_C62_LF_WITH_NULL_FRAME,
    DIGI_T2T_GFP_CMF_RDI_NO_SQUL = INSERT_T2T_GFP_CMF_RDI_NO_SQUELCH,
    DIGI_T2T_GFP_CMF_RDI_TXENA   = INSERT_T2T_GFP_CMF_RDI_TXENA,
    DIGI_T2T_RX_EMAC_SEND_RF     = INSERT_T2T_RX_EMAC_SEND_RF,
} digi_gfp_rx_act_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_gfp_tx_cmf_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT32 payload_len_type,
                                  UINT32 upi_value,
                                  BOOL   fcs_en,
                                  digi_cmf_one_shot_cmf_t *cmf_data);
PUBLIC PMC_ERROR adpt_gfp_rx_cmf_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gfp_cmf_mode_t cmf_mode,
                                  digi_cmf_extract_cmf_t *cmf_data);
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_fcs_mode_t fcs_mode);
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_fcs_mode_t *fcs_mode);
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_corrupt_set(UINT32 dev_id,
                                        UINT32 chnl_id, 
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_gfp_tx_fcs_corrupt_get(UINT32 dev_id,
                                        UINT32 chnl_id, 
                                        BOOL *enable);
PUBLIC PMC_ERROR adpt_gfp_rx_fcs_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_rx_fcs_mode_t fcs_mode,
                                       UINT32 preamble_size);
PUBLIC PMC_ERROR adpt_gfp_rx_fcs_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_gfp_rx_fcs_mode_t *fcs_mode);
PUBLIC PMC_ERROR adpt_gfp_tx_ehec_en_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 ehec_len,
                                      UINT32 ehec_buffer[16],
                                      BOOL ehec_enable);
PUBLIC PMC_ERROR adpt_gfp_tx_ehec_en_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *ehec_len,
                                      BOOL   *ehec_enable);
PUBLIC PMC_ERROR adpt_gfp_rx_ehec_en_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 ehec_err_cor,
                                      UINT32 exi_len,
                                      BOOL   ehec_enable);
PUBLIC PMC_ERROR adpt_gfp_rx_ehec_en_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *ehec_err_cor,
                                      UINT32 *exi_len,
                                      BOOL   *ehec_enable);
PUBLIC PMC_ERROR adpt_gfp_tx_exi_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 exi_value);
PUBLIC PMC_ERROR adpt_gfp_tx_exi_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 *exi_value);
PUBLIC PMC_ERROR adpt_gfp_exp_exi_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 exi_value1,
                                   UINT8 exi_value2);
PUBLIC PMC_ERROR adpt_gfp_exp_exi_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *exi_value1,
                                   UINT8 *exi_value2);
PUBLIC PMC_ERROR adpt_gfp_tx_upi_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 upi_value);
PUBLIC PMC_ERROR adpt_gfp_tx_upi_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 *upi_value);
PUBLIC PMC_ERROR adpt_gfp_exp_upi_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 upi_value);
PUBLIC PMC_ERROR adpt_gfp_exp_upi_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *upi_value);
PUBLIC PMC_ERROR adpt_gfp_rx_upi_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  UINT8 *upi_value);
PUBLIC PMC_ERROR adpt_gfp_legacy_mode_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT8  data_frm_upi,
                                       UINT8  ordered_set_upi);
PUBLIC PMC_ERROR adpt_gfp_legacy_mode_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT8  *data_frm_upi,
                                       UINT8  *ordered_set_upi);
PUBLIC PMC_ERROR adpt_gfp_exp_upi_ext_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT8 upi_value1, 
                                      UINT8 upi_value2);
PUBLIC PMC_ERROR adpt_gfp_exp_upi_ext_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT8 *upi_value1,
                                      UINT8 *upi_value2);
PUBLIC PMC_ERROR adpt_gfp_non_std_upi_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT32 los_upi,
                                       UINT32 loss_sync_upi,
                                       UINT32 dci_upi,
                                       UINT32 fdi_upi,
                                       UINT32 rdi_upi);
PUBLIC PMC_ERROR adpt_gfp_tx_pti_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 pti_value);
PUBLIC PMC_ERROR adpt_gfp_tx_pti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT8 *pti_value);
PUBLIC PMC_ERROR adpt_gfp_exp_pti_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 pti_value);
PUBLIC PMC_ERROR adpt_gfp_exp_pti_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT8 *pti_value);
PUBLIC PMC_ERROR adpt_gfp_frm_len_set(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  UINT32 min_frm_len,
                                  UINT32 max_frm_len);
PUBLIC PMC_ERROR adpt_gfp_frm_len_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   UINT32 *min_frm_len,
                                   UINT32 *max_frm_len);
PUBLIC PMC_ERROR adpt_gfp_tx_conseq_act_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        UINT32 alm_msk,
                                        digi_gfp_tx_act_t alm_act1,
                                        digi_gfp_tx_act_t alm_act2,
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_gfp_tx_conseq_act_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_alm_act_mask_t alm_item,
                                        digi_gfp_tx_act_t *alm_act1,
                                        digi_gfp_tx_act_t *alm_act2,
                                        BOOL *enable);
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_set(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        UINT32 alm_msk,
                                        digi_gfp_rx_act_t alm_act,
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_rx_alm_act_mask_t alm_item,
                                        digi_gfp_rx_act_t *alm_act,
                                        BOOL *enable);
PUBLIC PMC_ERROR adpt_gfp_os_rate_inc_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      BOOL   enable);
PUBLIC PMC_ERROR adpt_gfp_os_rate_inc_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      BOOL   *enable);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_GFP_H */

/* 
** end of file 
*/

