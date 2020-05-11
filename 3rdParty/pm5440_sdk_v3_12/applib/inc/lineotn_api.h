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
*   Lineotn_api.h contains the Lineotn sub system api declaration for the
*   Lineotn runtime configuration and operation, maintenance and performance
*   monitoring functions and their related enumerations,constant definitions
*   and structure definitions.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _LINEOTN_API_H
#define _LINEOTN_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "otu_frm_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: lineotn_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN direction.
*
* ELEMENTS:
*   LINE_OTN_RX_DIR   - Rx (SK) direction.
*   LINE_OTN_TX_DIR   - Tx (SO) direction.
*   LAST_LINE_OTN_DIR -   this is for out of range values error handling 
*******************************************************************************/
typedef enum
{
    LINE_OTN_RX_DIR = 0,
    LINE_OTN_TX_DIR,

    LAST_LINE_OTN_DIR     /* this is for out of range values error handling */

} lineotn_dir_t;

/*******************************************************************************
* ENUM: lineotn_frame_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN odu/otu frame rate.  The element
*   terms have a syntax: 
*      - LINE_OTN_%frame_type%.
*      - %frame_type% is the implicit frame rate as specified in the standards.
*
* ELEMENTS:
*       LINE_OTN_RATE_OTU4      - OTU4   frame rate
*       LINE_OTN_RATE_OTU3E2    - OTU3e2 frame rate
*       LINE_OTN_RATE_OTU3E1    - OTU3e1 frame rate
*       LINE_OTN_RATE_OTU3      - OTU3   frame rate      
*       LINE_OTN_RATE_OTU2F     - OTU2f  frame rate
*       LINE_OTN_RATE_OTU2E     - OTU2e  frame rate
*       LINE_OTN_RATE_OTU2      - OTU2   frame rate      
*       LINE_OTN_RATE_OTU1F     - OTU1f  frame rate     
*       LINE_OTN_RATE_OTU1E     - OTU1e  frame rate                                     
*       LAST_LINE_OTN_FRAME_RATE - place holder for error checking
* 
*
*******************************************************************************/

typedef enum
{
    LINE_OTN_RATE_OTU4          = 0,


    LINE_OTN_RATE_OTU3E2,


    LINE_OTN_RATE_OTU3E1,


    LINE_OTN_RATE_OTU3,
    LINE_OTN_RATE_OTU2F,
    LINE_OTN_RATE_OTU2E,
    LINE_OTN_RATE_OTU2,
    LINE_OTN_RATE_OTU1F,
    LINE_OTN_RATE_OTU1E,

    LAST_LINE_OTN_FRAME_RATE        /* this is for out of range values error handling */
} lineotn_frame_rate_t;


/*******************************************************************************
* ENUM: lineotn_fec_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN fec type.  
*
* ELEMENTS:
*    LINE_OTN_FEC_NONE          - No FEC configured
*    LINE_OTN_FEC_I4            - FEC type I4
*    LINE_OTN_FEC_I7            - FEC type I7
*    LINE_OTN_FEC_G709          - FEC type RSFEC (G7.09)
*    LINE_OTN_FEC_SWIZZLE_100G  - FEC type Swizzle 100G 
*    LINE_OTN_FEC_SWIZZLE_40G   - FEC type Swizzle 40G 
*    LAST_LINE_OTN_FEC          - place holder for error checking
* 
*******************************************************************************/

typedef enum
{
    LINE_OTN_FEC_NONE         = 0,
    LINE_OTN_FEC_I4,
    LINE_OTN_FEC_I7,
    LINE_OTN_FEC_G709,
    LINE_OTN_FEC_SWIZZLE_100G,
    LINE_OTN_FEC_SWIZZLE_40G,

    LAST_LINE_OTN_FEC   /* this is for out of range values error handling */
} lineotn_fec_t;


/*******************************************************************************
* ENUM: lineotn_fec_bypass_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN fec type.  
*
* ELEMENTS:
*    LINE_OTN_FEC_BYPASS        - FEC in bypass mode    
*    LINE_OTN_FEC_NO_BYPASS_SET - FEC out of bypass mode
*     LAST_LINE_OTN_FEC_BYPASS  - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_FEC_BYPASS = 0,
    LINE_OTN_FEC_NO_BYPASS_SET,

    LAST_LINE_OTN_FEC_BYPASS  
} lineotn_fec_bypass_t;


/*******************************************************************************
* ENUM: lineotn_fec_dec_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type Rx FEC processing mode.
*
* ELEMENTS:
*    LINE_OTN_FEC_RX_ENABLED              - Rx FEC Processing
*    LINE_OTN_FEC_RX_CORRECTION_DISABLED  - No Rx FEC Processing, maintaining FEC 
*                                           datapath latency (NO FEC correction)
*    LINE_OTN_FEC_RX_BYPASSED             - No Rx FEC Processing, FEC bypassed, 
*                                           lowest latency
*
*******************************************************************************/
typedef enum {
    LINE_OTN_FEC_RX_ENABLED              = 0,
    LINE_OTN_FEC_RX_CORRECTION_DISABLED  = 1,
    LINE_OTN_FEC_RX_BYPASSED             = 2,
    LAST_LINE_OTN_FEC_RX

} lineotn_fec_dec_mode_t;


/*******************************************************************************
* ENUM: lineotn_oh_field_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type OH control insertion fields.
*
* ELEMENTS:
*   LINE_OTN_SO_FAS_F6       - frame alignment sync tx OH field (first 3 bytes)
*   LINE_OTN_SO_FAS_28       - frame alignment sync tx OH field (last 3 bytes)
*   LINE_OTN_SO_TTI         - trail trace identifier tx OH field  
*   LINE_OTN_SO_RES7        - resreved 7 tx OH field 
*   LINE_OTN_SO_RES8        - resreved 8 tx OH field 
*   LINE_OTN_SO_GCC0        - general communication channel 0 tx OH field
*   LINE_OTN_SO_RES13       - resreved 13 tx OH field 
*   LINE_OTN_SO_RES14       - resreved 14 tx OH field 
*   LAST_LINE_OTN_OH_FIELD  - this is for out of range values error handling
*******************************************************************************/
typedef enum
{
    LINE_OTN_SO_FAS_F6 = 0,
    LINE_OTN_SO_FAS_28,
    LINE_OTN_SO_TTI,
    LINE_OTN_SO_RES7,
    LINE_OTN_SO_RES8,
    LINE_OTN_SO_GCC0,
    LINE_OTN_SO_RES13,
    LINE_OTN_SO_RES14,

    LAST_LINE_OTN_OH_FIELD    /* this is for out of range values error handling */

} lineotn_oh_field_t;


/*******************************************************************************
* ENUM: lineotn_oh_extract_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type contains the extracted SM XXX overhead field
*   for a single OTUk frame in the SK (Rx) direction.
*
* ELEMENTS:
*    LINE_OTN_EXT_SK_SM_TTI         - trail trace identifier SM rx extracted field  
*    LINE_OTN_EXT_SK_SM_BIP8        - BIP8 SM rx extracted field 
*    LINE_OTN_EXT_SK_SM_BEI_BIAE    - BEI/BIAE SM rx extracted field 
*    LINE_OTN_EXT_SK_SM_BDI         - BDI SM rx extracted field          
*    LINE_OTN_EXT_SK_SM_IAE         - IAE SM rx extracted field 
*    LINE_OTN_EXT_SK_SM_RES         - RES SM rx extracted field 
*    LINE_OTN_EXT_SK_GCC0           - GCC0 SM rx extracted field 
*    LINE_OTN_EXT_SK_RES            - RES rx extracted field 
*    LAST_LINE_OTN_OH_EXT_FIELD  - this is for out of range values error handling
*******************************************************************************/
typedef enum
{
    LINE_OTN_EXT_SK_SM_TTI = 0,
    LINE_OTN_EXT_SK_SM_BIP8,
    LINE_OTN_EXT_SK_SM_BEI_BIAE,
    LINE_OTN_EXT_SK_SM_BDI,
    LINE_OTN_EXT_SK_SM_IAE,
    LINE_OTN_EXT_SK_SM_RES,
    LINE_OTN_EXT_SK_RES,
    LINE_OTN_EXT_SK_GCC0,

    LAST_LINE_OTN_OH_EXT_FIELD    /* this is for out of range values error handling */

}lineotn_oh_extract_t;


/*******************************************************************************
* ENUM: lineotn_oh_ins_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type OH control insertion methods.
*
* ELEMENTS:
*  LINE_OTN_INS_TYPE_UNMODIFIED - field passes through unmodified from the upstream
*  LINE_OTN_INS_TYPE_USER_VALUE - user value is inserted
*  LAST_LINE_OTN_OH_INS_TYPE    - this is for out of range values error handling 
*******************************************************************************/
typedef enum
{
    LINE_OTN_INS_TYPE_UNMODIFIED = 0,
    LINE_OTN_INS_TYPE_USER_VALUE,

    LAST_LINE_OTN_OH_INS_TYPE    /* this is for out of range values error handling */

} lineotn_oh_ins_type_t;


/*******************************************************************************
* ENUM: lineotn_bkwd_oh_field_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type sm OH backwards error indications
*   insertion control fields.
*
* ELEMENTS:
*  LINE_OTN_SO_BEI_BIAE         - BEI/BIAE tx  sm OH backwards error indications
*  LINE_OTN_SO_BDI              - BDI tx  sm OH backwards error indication
*   LAST_LINE_OTN_BKWD_OH_FIELD - this is for out of range values error handling 
*******************************************************************************/
typedef enum
{
    LINE_OTN_SO_BEI_BIAE,
    LINE_OTN_SO_BDI,

    LAST_LINE_OTN_BKWD_OH_FIELD    /* this is for out of range values error handling */

} lineotn_bkwd_oh_field_t;


/*******************************************************************************
* ENUM: lineotn_bkwd_ins_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type sm OH backwards error indications
*   insertion methods.
*
* ELEMENTS:
*   LINE_OTN_BKWD_INS_TYPE_UNMODIFIED - BDI/BEI/BIAE field passes through
*                                       unmodified from the upstream
*
*   LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE - BDI/BEI/BIAE field contains the backwards
*                                            consequential action value from the  OTUk_Sk
*   LINE_OTN_BKWD_INS_TYPE_USER_VALUE - user value BDI/BEI/BIAE is inserted
*   LAST_LINE_OTN_OH_BKWD_INS_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_BKWD_INS_TYPE_UNMODIFIED = 0,
    LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE,
    LINE_OTN_BKWD_INS_TYPE_USER_VALUE,

    LAST_LINE_OTN_OH_BKWD_INS_TYPE    /* this is for out of range values error handling */

} lineotn_bkwd_ins_type_t;


/*******************************************************************************
* ENUM: lineotn_mfas_ins_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type sm OH mfas
*   insertion methods.
*
* ELEMENTS:
*   LINE_OTN_MFAS_INS_TYPE_UNMODIFIED - mfas will not be inserted
*
*   LINE_OTN_MFAS_INS_TYPE_USER_VALUE - user value mfas will be inserted
*
*   LINE_OTN_MFAS_INS_TYPE_UPSTREAM_VALUE - mfas field is inserted
*   via a freewheeling counter based on the upstream MFAS.
*
*   LAST_LINE_OTN_OH_MFAS_INS_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_MFAS_INS_TYPE_UNMODIFIED = 0,
    LINE_OTN_MFAS_INS_TYPE_UPSTREAM_VALUE,
    LINE_OTN_MFAS_INS_TYPE_USER_VALUE,

    LAST_LINE_OTN_OH_MFAS_INS_TYPE    /* this is for out of range values error handling */

} lineotn_mfas_ins_type_t;


/*******************************************************************************
* ENUM: lineotn_bip8_ins_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type sm OH bip8
*   insertion methods.
*
* ELEMENTS:
*   LINE_OTN_BIP8_INS_DISABLED - bip8 will not be inserted
*
*   LINE_OTN_BIP8_INS_TYPE_ENABLED -  calculated
*   bip8 value is inserted
*
*   LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE - calculated
*   value insertion is disabled and the upstream SM bip8 value
*   in the frame is XOR'd with the bip8 user value
*
*   LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE -
*   calculated SM bip8 value is XOR'd with the bip8 user value
*
*   LINE_OTN_BIP8_INS_TYPE_USER_VALUE - bip8 constant user value insertion
*
*   LAST_LINE_OTN_OH_BIP8_INS_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_BIP8_INS_DISABLED = 0,
    LINE_OTN_BIP8_INS_TYPE_ENABLED,
    LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE,
    LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE,
    LINE_OTN_BIP8_INS_TYPE_USER_VALUE,

    LAST_LINE_OTN_OH_BIP8_INS_TYPE    /* this is for out of range values error handling */

} lineotn_bip8_ins_type_t;


/*******************************************************************************
* ENUM: lineotn_iae_ins_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type sm OH iae
*   insertion methods.
*
* ELEMENTS:
*   LINE_OTN_IAE_INS_TYPE_UNMODIFIED -  value insertion is
*   disabled and the iae field passes through unmodified from the upstream
*
*   LINE_OTN_IAE_INS_TYPE_USER_VALUE - user value iae will be inserted
*
*   LINE_OTN_IAE_INS_TYPE_DETECTED_VALUE - iae field contains
*   the value detected by the OTUk/ODUk_A So function
*
*   LAST_LINE_OTN_OH_IAE_INS_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_IAE_INS_TYPE_UNMODIFIED = 0,
    LINE_OTN_IAE_INS_TYPE_DETECTED_VALUE,
    LINE_OTN_IAE_INS_TYPE_USER_VALUE,

    LAST_LINE_OTN_OH_IAE_INS_TYPE    /* this is for out of range values error handling */

} lineotn_iae_ins_type_t;


/*******************************************************************************
* ENUM: lineotn_action_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type consequential actions types.
*
* ELEMENTS:
*  LINE_OTN_A_TSF         - TSF consequential actions
*  LINE_OTN_A_SSF         - SSF consequential actions
*  LINE_OTN_A_TSD         - TSD consequential actions
*  LINE_OTN_A_IAE         - IAE consequential actions
*  LINE_OTN_A_SSD         - SSD consequential actions
*  LINE_OTN_A_AIS         - AIS consequential actions
*  LINE_OTN_A_BEI         - BEI consequential actions
*  LINE_OTN_A_BIAE        - BIAE consequential actions
*  LINE_OTN_A_BDI         - BDI consequential actions
*  LINE_OTN_A_FFAIL       - FFAIL consequential actions
*  LAST_LINE_OTN_ACT_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_A_TSF = 0,
    LINE_OTN_A_SSF,
    LINE_OTN_A_TSD,
    LINE_OTN_A_IAE,
    LINE_OTN_A_SSD,
    LINE_OTN_A_AIS,
    LINE_OTN_A_BEI,
    LINE_OTN_A_BIAE,
    LINE_OTN_A_BDI,
    LINE_OTN_A_FFAIL,

    LAST_LINE_OTN_ACT_TYPE    /* this is for out of range values error handling */

} lineotn_action_type_t;


/*******************************************************************************
* ENUM: lineotn_maint_sig_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated maintenance signal types.
*
* ELEMENTS:
*  LINE_OTN_NO_MAINT_SIG    - no maintenace signal
*  LINE_OTN_ODUK_AIS        - oduk AIS maintenace signal
*  LINE_OTN_ODUK_LCK        - oduk lock maintenace signal
*  LINE_OTN_ODUK_OCI        - oduk OCI maintenace signal (always performed
*                             at nominal rate)
*  LINE_OTN_ODUK_AIS_NOMINAL - oduk AIS maintenace signal at nominal rate
*  LINE_OTN_ODUK_LCK_NOMINAL - oduk LCK maintenace signal at nominal rate
*  LAST_LINE_OTN_MAINT_SIG - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_NO_MAINT_SIG = 0,
    LINE_OTN_ODUK_AIS,
    LINE_OTN_ODUK_LCK,
    LINE_OTN_ODUK_OCI,
    LINE_OTN_ODUK_AIS_NOMINAL,
    LINE_OTN_ODUK_LCK_NOMINAL,

    LAST_LINE_OTN_MAINT_SIG    /* this is for out of range values error handling */

}lineotn_maint_sig_type_t;


/*******************************************************************************
* ENUM: lineotn_mi_active_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated mi active types.
*
* ELEMENTS:
*  LINE_OTN_MI_ACTIVE_DISABLED  - no mi active 
*  LINE_OTN_MI_ACTIVE_ENABLED   - mi active on OTUk TT (OTU_FRM)
*  LAST_LINE_OTN_MI_ACTIVE_TYPE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_MI_ACTIVE_DISABLED = 0,
    LINE_OTN_MI_ACTIVE_ENABLED,

    LAST_LINE_OTN_MI_ACTIVE_TYPE    /* this is for out of range values error handling */
}lineotn_mi_active_type_t;


/*******************************************************************************
* ENUM: lineotn_sk_sm_deg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify OTUK_SK_SM_DEG register fields.
*
* ELEMENTS:
*  LINE_OTN_OTU_SK_SM_DEG_M   - the degraded defect consecutive one second monitoring
*                               intervals for the SM channel.
*  LINE_OTN_OTU_SK_SM_DEG_THR - the degraded defect one second error block count
*                               for the SM channel
*
*  LAST_LINE_OTN_SM_DEG       - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_OTU_SK_SM_DEG_M = 0,
    LINE_OTN_OTU_SK_SM_DEG_THR,

    LAST_LINE_OTN_SM_DEG /* this is for out of range values error handling */
}lineotn_sk_sm_deg_t;


/*******************************************************************************
* ENUM: lineotn_loopback_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying loopback state.
*
* ELEMENTS:
*    LINE_OTN_LOOPBACK_DISABLED   - loopback disabled
*    LINE_OTN_LOOPBACK_ENABLED    - loopback enabled
*    LAST_LINE_OTN_LOOPBACK_STATE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_LOOPBACK_DISABLED = 0,
    LINE_OTN_LOOPBACK_ENABLED,

    LAST_LINE_OTN_LOOPBACK_STATE      /* this is for out of range values error handling */

}lineotn_loopback_state_t;


/*******************************************************************************
* ENUM: lineotn_sm_cnt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated specify SK far/near end error counters.
*
* ELEMENTS:
*   LINE_OTN_OTU_SK_SM_P_NE_IAE  - IAE performance monitor (pIAE) for the SM channel
*   LINE_OTN_OTU_SK_SM_P_NE_DS   - Near End Defect Second performance monitor (pN_DS) for the SM channel
*   LINE_OTN_OTU_SK_SM_P_NE_BC   - Near End Error Block Counter (N_EBC) for the SM channel
*   LINE_OTN_OTU_SK_SM_P_FE_BIAE - BIAE performance monitor (pBIAE) for the SM channel
*   LINE_OTN_OTU_SK_SM_P_FE_DS   - Far End Defect Second performance monitor (pF_DS) for the SM channel
*   LINE_OTN_OTU_SK_SM_P_FE_BC   - Far End Error Block Counter (F_EBC) for the SM channel
*   LAST_LINE_OTN_FE_NE_CNT      - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_OTU_SK_SM_P_NE_IAE = 0,
    LINE_OTN_OTU_SK_SM_P_NE_DS,
    LINE_OTN_OTU_SK_SM_P_NE_BC,
    LINE_OTN_OTU_SK_SM_P_FE_BIAE,
    LINE_OTN_OTU_SK_SM_P_FE_DS,
    LINE_OTN_OTU_SK_SM_P_FE_BC,

    LAST_LINE_OTN_FE_NE_CNT    /* this is for out of range values error handling */

}lineotn_sm_cnt_t;


/******************************************************************************
* ENUM: lineotn_deskew_type_t
*______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated deskew configuration type.
*   LineOTN has 19 supported modes as specified above.
*   Some configurations can have either a HW resources routing via SFIS or OTL 
*   HW modules. In the case of SFIS routing user need to configure the type of
*   dsi lane deskew required. for OTL3.4/OTU2 configurations routing no deskew is required.
*
* ELEMENTS:
*   LINE_OTN_NO_DESKEW              - no deskew required, OTL3.4/OTU2 configurations  
*   LINE_OTN_OIF_SFIS_DESKEW        - OIF SFI-S deskew  
*   LINE_OTN_ENHANCED_SFIS_DESKEW   - OTL over SFI deskew mode 
*   LAST_LINE_OTN_DESKEW_TYPE       -   this is for out of range values error handling           
*******************************************************************************/
typedef enum
{
    LINE_OTN_NO_DESKEW = 0,
    LINE_OTN_OIF_SFIS_DESKEW , 
    LINE_OTN_ENHANCED_SFIS_DESKEW ,
    LAST_LINE_OTN_DESKEW_TYPE   /* this is for out of range values error handling           */

}lineotn_deskew_type_t;


/******************************************************************************
*
* ENUM: lineotn_dsi_type_t
*
______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated lineotn dsi stream type. 
*   dsi stream type specifies the dsi hw/serdes interface type for each dsi stream.
*
* ELEMENTS:
*   LINE_OTN_DSI_TYPE_SFI_51_A - marks SFI5.1 dsi lanes 0-16
*   LINE_OTN_DSI_TYPE_SFI_51_B - marks SFI5.1 dsi lanes 17-33
*   LINE_OTN_DSI_TYPE_SFIS     - marks SFIS(n) dsi lanes 
*   LINE_OTN_DSI_TYPE_OTL,     - marks OTL 3.4/4.10 dsi lanes   
*   LINE_OTN_DSI_TYPE_OTUK     - marks 10G OTUk lanes
*   LAST_LINE_OTN_DSI_TYPE     - this is for out of range values error handling  
 
*******************************************************************************/
typedef enum
{
    LINE_OTN_DSI_TYPE_SFI_51_A = 0,
    LINE_OTN_DSI_TYPE_SFI_51_B,
    LINE_OTN_DSI_TYPE_SFIS,
    LINE_OTN_DSI_TYPE_OTL,
    LINE_OTN_DSI_TYPE_OTUK ,

    LAST_LINE_OTN_DSI_TYPE, 
}lineotn_dsi_type_t;


/******************************************************************************
*
* ENUM: lineotn_mi_admin_state_t
*
______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated lineotn mi admin state.
*
* ELEMENTS:
*   LINE_OTN_MI_ADMIN_STATE_ENABLED  - mi admin state is active
*   LINE_OTN_MI_ADMIN_STATE_DISABLED - mi admin state is disabled
*   LAST_LINE_OTN_MI_ADMIN_STATE,    - this is for out of range values error handling
*******************************************************************************/
typedef enum
{
    LINE_OTN_MI_ADMIN_STATE_ENABLED = 0,
    LINE_OTN_MI_ADMIN_STATE_DISABLED,

    LAST_LINE_OTN_MI_ADMIN_STATE

}lineotn_mi_admin_state_t;


/*******************************************************************************
* ENUM: lineotn_bergen_err_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Bergen bit error generator modes.
*
* ELEMENTS:
*    LINE_OTN_BERGEN_NORMAL - Inverts the input bit in order to insert an erroneous bit.
*    LINE_OTN_BERGEN_0_INSERTION -  Inverts the input bit only if the input bit is 1 
*                                   in order to insert an erroneous 0.
*    LINE_OTN_BERGEN_1_INSERTION -  Inverts the input bit only if the input bit is 0 
*                                   in order to insert an erroneous 1.
*    LAST_LINE_OTN_BERGEN_ERROR_MODE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_BERGEN_NORMAL= 0,
    LINE_OTN_BERGEN_0_INSERTION,
    LINE_OTN_BERGEN_1_INSERTION,

    LAST_LINE_OTN_BERGEN_ERROR_MODE      

}lineotn_bergen_err_mode;


/*******************************************************************************
* STRUCTURE: lineotn_fec_stats_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for returning fec statistics per one or more operational dsi streams.
*
* ELEMENTS:
*   records_num     - UINT32, number or records returned
*   statistics_recs - UINT8 pointer, pointer to allocated memory with containing
*                     fec statistics information.
*
*******************************************************************************/
typedef struct lineotn_fec_stats_t
{
    UINT32  records_num;
    UINT8   *statistics_recs;
} lineotn_fec_stats_t;

/*******************************************************************************
* ENUM: lineotn_sk_tim_det_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   enumerated tructure to specify the trail trace identifier mismatch detection mode 
*   for the SM channel
*
* ELEMENTS:
*    OTU_SK_SM_DTIM_DET_DISABLED    - dTIM defect detection is disabled 
*    OTU_SK_SM_DTIM_DET_SAPI        - The expected SAPI field is used when determining the dTIM defect 
*    OTU_SK_SM_DTIM_DET_DAPI        - The expected DAPI field is used when determining the dTIM defect 
*    OTU_SK_SM_DTIM_DET_BOTH        - Both the expected SAPI field and the expected DAPI field are 
*                                     used when determining the dTIM defect
*    LAST_LINE_OTN_SK_SM_DTIM_DET   - this is for out of range values error handling 
* 
 *******************************************************************************/
typedef enum
{
    LINE_OTN_SK_SM_DTIM_DET_DISABLED = 0,  
    LINE_OTN_SM_DTIM_DET_SAPI, 
    LINE_OTN_SM_DTIM_DET_DAPI, 
    LINE_OTN_SK_SM_DTIM_DET_BOTH, 
    LAST_LINE_OTN_SK_SM_DTIM_DET    
}lineotn_sk_tim_det_mode_t;

/*******************************************************************************
* STRUCTURE: lineotn_otu_frm_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LINEOTN otu framer far/near end counters for a single channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    otu_frm_sk_sm_p_n_ds            - OTU_SK_SM_P_N_DS [24:24]
*    otu_frm_sk_sm_p_n_ebc           - OTU_SK_SM_P_N_EBC [23:0]
*    otu_frm_sk_sm_p_iae             - OTU_SK_SM_P_IAE [25:25]
*    otu_frm_sk_sm_p_f_ds            - OTU_SK_SM_P_F_DS [24:24]
*    otu_frm_sk_sm_p_f_ebc           - OTU_SK_SM_P_F_EBC [23:0]
*    otu_frm_sk_sm_p_biae            - OTU_SK_SM_P_BIAE [25:25]
*
*******************************************************************************/
typedef struct lineotn_otu_frm_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;

    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
#else
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */

    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
#endif
 } lineotn_otu_frm_counters_t;


/*
** Constants
*/
#ifndef DOXYGEN_PUBLIC_ONLY 
/*
 * This macro is a logical 'all dsi streams' notation. when used
 * in relevant lineOTN functions it specifies an operation to be taken
 * on all already configured dsi streams per the current configuration.
 */
#define LINE_OTN_CFG_ALL_DSI_STREAMS            (0x00000fff)

/*
 * This list of macros specifies defect bit masks that can be used
 * in the activation of consequential actions.
 */
#define LINE_OTN_D_LOS_P                        (0x00000001)
#define LINE_OTN_D_LOL_P                        (0x00000002)
#define LINE_OTN_D_LOFLANE                      (0x00000004)
#define LINE_OTN_D_AIS                          (0x00000008)
#define LINE_OTN_D_LOF                          (0x00000010)
#define LINE_OTN_D_LOM                          (0x00000020)
#define LINE_OTN_D_TIM                          (0x00000040)
#define LINE_OTN_D_IAE                          (0x00000080)
#define LINE_OTN_D_DEG                          (0x00000100)
#define LINE_OTN_N_BIPV                         (0x00000200)
#define LINE_OTN_IAE                            (0x00000400)
#define LINE_OTN_D_BDI_FROM_SSF                 (0x00000800)
#define LINE_OTN_D_TSF_FROM_SSF                 (0x00001000)
#define LINE_OTN_D_SSF_FROM_TSF                 (0x00004000)
#define LINE_OTN_D_AIS_FROM_TSF                 (0x00008000)
#define LINE_OTN_D_SSD_FROM_TSD                 (0x00010000)


/*
 * This list of macros specifies consequential actions activation
 * bit masks.
 */

/* Following defines is for the OPSM/OTUk-a_A_Sk process */
#define LINE_OTN_A_TSF_OPSM_TT                  (LINE_OTN_D_LOS_P | LINE_OTN_D_LOL_P | LINE_OTN_D_LOFLANE )
#define LINE_OTN_A_SSF_OPSM_A                   (LINE_OTN_A_TSF_OPSM_TT| LINE_OTN_D_AIS | LINE_OTN_D_LOF | LINE_OTN_D_LOM)

/* Following define is for the OCh/OTUk-a_A_Sk process */
#define LINE_OTN_A_SSF_OTUK_A                   (LINE_OTN_D_LOS_P | LINE_OTN_D_AIS | LINE_OTN_D_LOF | LINE_OTN_D_LOM)

/* Following defines are for the OTUk_TT_Sk process */
#define LINE_OTN_A_TSF_OTUK_TT                  (LINE_OTN_A_SSF_OPSM_A | LINE_OTN_D_TSF_FROM_SSF | LINE_OTN_D_TIM)
#define LINE_OTN_A_SSF_ODUK_A                   (LINE_OTN_A_TSF_OTUK_TT | LINE_OTN_D_SSF_FROM_TSF )

#define LINE_OTN_A_SSF_ACT                      (LINE_OTN_A_SSF_ODUK_A)
#define LINE_OTN_A_AIS_ACT                      (LINE_OTN_A_TSF_OTUK_TT | LINE_OTN_D_AIS_FROM_TSF)
#define LINE_OTN_A_BDI_ACT                      (LINE_OTN_A_SSF_OPSM_A | LINE_OTN_D_TIM | LINE_OTN_D_BDI_FROM_SSF)
#define LINE_OTN_A_TSF_ACT                      (LINE_OTN_A_TSF_OTUK_TT)
#define LINE_OTN_A_BEI_ACT                      (LINE_OTN_N_BIPV)
#define LINE_OTN_A_BIAE_ACT                     (LINE_OTN_D_IAE)
#define LINE_OTN_A_TSD_ACT                      (LINE_OTN_D_DEG)
#define LINE_OTN_A_SSD_ACT                      (LINE_OTN_A_TSD_ACT|LINE_OTN_D_SSD_FROM_TSD)
#define LINE_OTN_A_IAE_ACT                      (LINE_OTN_IAE)
#define LINE_OTN_A_FFAIL_ACT                    (LINE_OTN_A_TSF_OPSM_TT | LINE_OTN_D_AIS | LINE_OTN_D_LOF | LINE_OTN_D_LOM )

#define LINE_OTN_DEFECT_ERROR                   (0xffffffff)
/*
 * Lineotn max number of dsi streams
 */
#define LINE_OTN_MAX_DSI_STREAMS                (12)
#define LINE_OTN_40G_DSI_STREAMS                (4)

#define LINEOTN_MAX_LANES_PER_OPSM23_SLICE      4
#define LINEOTN_MAX_LANES_PER_OPSM4_SLICE       20

#define LINEOTN_SFIS_NUM_LANE                   10
#define LINEOTN_SFI51_NUM_LANE                  16
#endif /* DOXYGEN_PUBLIC_ONLY */



/*
** Macro Definitions
*/


/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: lineotn_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct lineotn_handle_struct_t lineotn_handle_t; 

/*******************************************************************************
* STRUCTURE:  digi_fec_stat_intf_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining FEC statistic interface configuration.
*
* ELEMENTS:
*   collection_period - Define the period at which Lane Statistics Accumulator
*                       (LLSA) performance counters are latched and pushed on 
*                       the 4-wire OTN FEC statistics interface. This value is
*                       in milliseconds and shall be  valid values are from 
*                       between 1 to 1000 ms.
*   inv_polarity - Invert the polarity of the 4-wire interface. 
*                   - TRUE: outputs (valid, data and frame pulse) are inverted
*                   - FALSE: outputs (valid, data and frame pulse) are not inverted
*
*******************************************************************************/
typedef struct digi_fec_stat_intf_cfg_t  
{    
    UINT32 collection_period;
    BOOL8 inv_polarity;
} digi_fec_stat_intf_cfg_t;


/*******************************************************************************
* STRUCTURE: lineotn_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN level table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct lineotn_int_t
{
    otu_frm_int_t  otu_frm;

    BOOL8   fecstat_air_err_int_i;
} lineotn_int_t;

/*******************************************************************************
* STRUCTURE: lineotn_sfis_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN SFIS level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct lineotn_sfis_int_chnl_t
{

    /* Interrupts */
    BOOL8   rxlos_dsc_i;
    BOOL8   rxoof_i;
    BOOL8   rxudr_i[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rxudr_dsc_i;
    BOOL8   rxovr_i[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rxovr_dsc_i;
    BOOL8   rx_biterr_dsc_i;
    BOOL8   txovr_i;
    BOOL8   txudr_i;
    BOOL8   rxlos_i[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rxooa_i[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rx_biterr_i[LINEOTN_SFIS_NUM_LANE];
    
    /* Status */
    BOOL8   rxlos_dsc_v;
    BOOL8   rxoof_v;
    BOOL8   rxudr_dsc_v;
    BOOL8   rxovr_dsc_v;
    BOOL8   rx_biterr_dsc_v;
    BOOL8   rxlos_v[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rxooa_v[LINEOTN_SFIS_NUM_LANE];
    BOOL8   rx_biterr_v;

} lineotn_sfis_int_chnl_t;


/*******************************************************************************
* STRUCTURE: lineotn_sfi51_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN SFI 5.1 level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct lineotn_sfi51_int_chnl_t
{

    /* Interrupts */
    BOOL8   rx_deskew_los_i;
    BOOL8   rx_deskew_oof_i;
    BOOL8   rx_deskew_lof_i;
    BOOL8   rx_deskew_fifo_unfl_i;
    BOOL8   rx_deskew_fifo_ovfl_i;
    BOOL8   rx_deskew_ooa_i[LINEOTN_SFI51_NUM_LANE];
    BOOL8   rx_chn_bit_err_i[LINEOTN_SFI51_NUM_LANE];
    BOOL8   tx_fifo_unfl_i;
    BOOL8   tx_fifo_ovfl_i;
    
    /* Status */
    BOOL8   rx_deskew_los_v;
    BOOL8   rx_deskew_oof_v;
    BOOL8   rx_deskew_lof_v;
    BOOL8   rx_deskew_fifo_unfl_v;
    BOOL8   rx_deskew_fifo_ovfl_v;
    BOOL8   rx_deskew_ooa_v[LINEOTN_SFI51_NUM_LANE];
    BOOL8   rx_chn_bit_err_v[LINEOTN_SFI51_NUM_LANE];
    BOOL8   tx_fifo_unfl_v;
    BOOL8   tx_fifo_ovfl_v;

} lineotn_sfi51_int_chnl_t;


/*******************************************************************************
* STRUCTURE: lineotn_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct lineotn_int_chnl_t
{
    otu_frm_int_chnl_t       otu_frm;
    lineotn_sfis_int_chnl_t  sfis;
    lineotn_sfi51_int_chnl_t sfi51;

    BOOL8                   sfis_valid; 
    BOOL8                   opsm23_valid; 
    BOOL8                   opsm4_valid; 

    /* Interrupts */
    BOOL8   opsm23_tx_sfi_fifo_overflow_int_i;
    BOOL8   opsm23_tx_sfi_fifo_underflow_int_i;
    BOOL8   opsm23_tx_sys_fifo_overflow_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_sys_fifo_underflow_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_lane_fifo_overflow_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_lane_fifo_underflow_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_sfi_los_int_i;
    BOOL8   opsm23_rx_sfi_oof_int_i;
    BOOL8   opsm23_rx_sfi_lof_int_i;
    BOOL8   opsm23_rx_otu_oof_int_i;
    BOOL8   opsm23_rx_otu_lof_int_i;
    BOOL8   opsm23_rx_sfi_ais_int_i;
    BOOL8   opsm23_rx_lol_int_i;
    BOOL8   opsm23_rx_skew_range_violation_int_i;
    BOOL8   opsm23_rx_align_change_int_i;
    BOOL8   opsm23_rx_deskew_change_int_i;
    BOOL8   opsm23_rx_sfi_fifo_overflow_int_i;
    BOOL8   opsm23_rx_sfi_fifo_underflow_int_i;
    BOOL8   opsm23_rx_lane_los_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_oof_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_lof_loflane_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_otuk_ais_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_otl_ais_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_align_change_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_fifo_error_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_oor_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_lor_int_i[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    
    BOOL8   opsm4_tx_sfi_fifo_overflow_int_i;
    BOOL8   opsm4_tx_sfi_fifo_underflow_int_i;
    BOOL8   opsm4_tx_sys_fifo_overflow_int_i;
    BOOL8   opsm4_tx_sys_fifo_underflow_int_i;
    BOOL8   opsm4_tx_lane_fas_iae_int_i;
    BOOL8   opsm4_tx_lane_mfas_iae_int_i;
    BOOL8   opsm4_tx_lane_fifo_overflow_int_i;
    BOOL8   opsm4_tx_lane_fifo_underflow_int_i;
    BOOL8   opsm4_tx_otuk_ais_int_i;
    BOOL8   opsm4_rx_sfi_los_int_i;
    BOOL8   opsm4_rx_sfi_oof_int_i;
    BOOL8   opsm4_rx_sfi_lof_int_i;
    BOOL8   opsm4_rx_otu_oof_int_i;
    BOOL8   opsm4_rx_otu_lof_int_i;
    BOOL8   opsm4_rx_sfi_ais_int_i;
    BOOL8   opsm4_rx_lol_int_i;
    BOOL8   opsm4_rx_skew_range_violation_int_i;
    BOOL8   opsm4_rx_sfi_align_change_int_i;
    BOOL8   opsm4_rx_deskew_change_int_i;
    BOOL8   opsm4_rx_sfi_fifo_overflow_int_i;
    BOOL8   opsm4_rx_sfi_fifo_underflow_int_i;
    BOOL8   opsm4_rx_lane_los_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oof_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_lof_loflane_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oom_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_otuk_ais_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_otl_ais_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_align_change_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_fifo_error_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oor_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_lor_int_i[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];

    /* Status */
    BOOL8   opsm23_tx_sfi_fifo_overflow_int_v;
    BOOL8   opsm23_tx_sfi_fifo_underflow_int_v;
    BOOL8   opsm23_tx_sys_fifo_overflow_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_sys_fifo_underflow_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_lane_fifo_overflow_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_tx_lane_fifo_underflow_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_sfi_los_int_v;
    BOOL8   opsm23_rx_sfi_oof_int_v;
    BOOL8   opsm23_rx_sfi_lof_int_v;
    BOOL8   opsm23_rx_otu_oof_int_v;
    BOOL8   opsm23_rx_otu_lof_int_v;
    BOOL8   opsm23_rx_sfi_ais_int_v;
    BOOL8   opsm23_rx_lol_int_v;
    BOOL8   opsm23_rx_skew_range_violation_int_v;
    BOOL8   opsm23_rx_align_change_int_v;
    BOOL8   opsm23_rx_deskew_change_int_v;
    BOOL8   opsm23_rx_sfi_fifo_overflow_int_v;
    BOOL8   opsm23_rx_sfi_fifo_underflow_int_v;
    BOOL8   opsm23_rx_lane_los_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_oof_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_lof_loflane_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_otuk_ais_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_otl_ais_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_align_change_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_fifo_error_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_oor_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    BOOL8   opsm23_rx_lane_lor_int_v[LINEOTN_MAX_LANES_PER_OPSM23_SLICE];
    
    BOOL8   opsm4_tx_sfi_fifo_overflow_int_v;
    BOOL8   opsm4_tx_sfi_fifo_underflow_int_v;
    BOOL8   opsm4_tx_sys_fifo_overflow_int_v;
    BOOL8   opsm4_tx_sys_fifo_underflow_int_v;
    BOOL8   opsm4_tx_lane_fas_iae_int_v;
    BOOL8   opsm4_tx_lane_mfas_iae_int_v;
    BOOL8   opsm4_tx_lane_fifo_overflow_int_v;
    BOOL8   opsm4_tx_lane_fifo_underflow_int_v;
    BOOL8   opsm4_tx_otuk_ais_int_v;
    BOOL8   opsm4_rx_sfi_los_int_v;
    BOOL8   opsm4_rx_sfi_oof_int_v;
    BOOL8   opsm4_rx_sfi_lof_int_v;
    BOOL8   opsm4_rx_otu_oof_int_v;
    BOOL8   opsm4_rx_otu_lof_int_v;
    BOOL8   opsm4_rx_sfi_ais_int_v;
    BOOL8   opsm4_rx_lol_int_v;
    BOOL8   opsm4_rx_skew_range_violation_int_v;
    BOOL8   opsm4_rx_sfi_align_change_int_v;
    BOOL8   opsm4_rx_deskew_change_int_v;
    BOOL8   opsm4_rx_sfi_fifo_overflow_int_v;
    BOOL8   opsm4_rx_sfi_fifo_underflow_int_v;
    BOOL8   opsm4_rx_lane_los_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oof_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_lof_loflane_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oom_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_otuk_ais_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_otl_ais_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_align_change_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_fifo_error_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_oor_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];
    BOOL8   opsm4_rx_lane_lor_int_v[LINEOTN_MAX_LANES_PER_OPSM4_SLICE];

} lineotn_int_chnl_t;




/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*
** Log Messaging
*/

/*
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums
 */
#define LINEOTN_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_LOG_CODE_DEBUG,    "LINEOTN: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_CODE_ASSERT,   "LINEOTN: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_INVALID_PARAMETERS,"LINEOTN:Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_OTHER_FEC_ALREADY_PROV,"LINEOTN: Err Another FEC is already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FAIL,"LINEOTN: Err Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_INVALID_PTR,"LINEOTN: Err Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_REPROVISION_ERR,"LINEOTN:Err reprovision failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_DEPROVISION_FEC_STAT_ERR,"LINEOTN:Err deprovision failed since it is registred in FEC statistic block" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_DEPROVISION_BERGEN_ERR,"LINEOTN:Err deprovision failed since BERGEN is activated" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_PROVISION_ERR,"LINEOTN:Err provision failed due to incorrect parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_NOT_PROV,"LINEOTN: Stream not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_NOT_ACT,"LINEOTN: Stream not activated" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_OH_INS_FAILED,"LINEOTN:Err OH inseration failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_OH_EXT_FAILED,"LINEOTN:Err OH extraction failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_SET_FAILED,"LINEOTN:Err FEC configuration failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_SM_OH_INS_FAILED,"LINEOTN:Err overhead insertion failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_UNEXPECTED_PARAM_FOR_OTU4_CLK_CFG,"LINEOTN:Err OTU4 clock config failed due to unexpected dsi_type" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_ALREADY_INIT,"LINEOTN:Err FEC STAT already initialized" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_NOT_INIT,"LINEOTN:Err FEC STAT not initialized" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_BAD_COLLECTION_PERIOD,"LINEOTN:Err FEC STAT Invalid collection period" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_INVALID_FEC,   "LINEOTN:Err FEC STAT provision failed due to an invalid FEC" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_FEC_BYPASS,   "LINEOTN:Err FEC STAT provision failed since FEC is bypassed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_STREAM_NOT_PROV,   "LINEOTN:Err FEC STAT provision failed due to an unprovision stream" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_INVALID_INTF,  "LINEOTN:Err FEC STAT provision failed due to an unsupported interface (SFIS or SFI5.1)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_ALREADY_SET,   "LINEOTN:Err FEC STAT provision failed since it is already provsionned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_LANE_DLOF,     "LINEOTN:Err FEC STAT provision failed since 1 or more RX lanes are in LANE dLOF" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_NOT_SET,       "LINEOTN:Err FEC STAT deprovision failed since it was not provisionned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_SET_FSL_TBL_ERROR,  "LINEOTN:Err FEC STAT error in FEC STAT logic table" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_STAT_TABLE_NOT_EMPTY,  "LINEOTN:Err FEC STAT logic table not empty" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_LOG_CODE_LOG,      "LINEOTN: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_LOG_HOSTMSG,      "LINEOTN: Host message log" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_DB_ENTRY_NOT_FOUND,      "LINEOTN: Database entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_CONTEXT_INCOHERENT,                   "LINEOTN: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_CONTEXT_REG_INCOHERENT,               "LINEOTN: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_CONTEXT_CLEANUP,                      "LINEOTN: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT, "LINEOTN: Communal ressource energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_INVALID_REVISION, "LINEOTN: Invalid for none Revision A implementation" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_STRONG_FEC_MIX_NOT_SUPPORTED, "LINEOTN: Mixing 10G strong FEC streams within a 40G group is not supported in hardware" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_DEC_CORR_DISABLE_NOT_SUPPORTED, "LINEOTN: Disabling FEC decoder correction is not supported for this mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_DEC_BYPASS_NOT_SUPPORTED, "LINEOTN: Bypassing FEC decoder is not supported for this mode " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_NOT_CONFIGURED, "LINEOTN: No FEC configured " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_INCORRECT_DECODER_BYPASS_CFG_DETECTED, "LINEOTN: Incorrect FEC decoder bypass configuration is detected ")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LINEOTN_ERR_FEC_DEC_CFG_STREAM_NOT_PROV, "LINEOTN: Err FEC decoder configuration failed due to an unprovision stream")\
    

/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum LINEOTN_LOG_ERR_INDEX
{
    LINEOTN_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_LINEOTN << 16),
    LINEOTN_LOG_ERR_TABLE
    LINEOTN_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char LINEOTN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char LINEOTN_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _LINEOTN_API_H */

/*
** end of file
*/
