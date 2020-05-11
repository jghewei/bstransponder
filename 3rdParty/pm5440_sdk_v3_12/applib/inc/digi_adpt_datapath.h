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
*     The head file describes the data path related configuration.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_DATAPATH_H
#define _DIGI_ADPT_DATAPATH_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_adpt_customer.h"
#include "digi_adpt_com.h"

/*
** Constants
*/

/*
** Macro Definitions
*/
/* traffic location */
#define DIGI_LINE_SIDE  0
#define DIGI_SYS_SIDE   1
#define DIGI_SIDE_MAX   2

/* traffic type */
#define DIGI_TYPE_OTU   0
#define DIGI_TYPE_ETH   1
#define DIGI_TYPE_CBR   2
#define DIGI_TYPE_INLK  3
#define DIGI_TYPE_MAX   4

/* traffic mode information */
#define DIGI_TRAFFIC_TYPE_GET(traffic_mode)   ((traffic_mode) >> 24)
#define DIGI_TRAFFIC_POS_GET(traffic_mode)    (((traffic_mode) & 0xFF0000) >> 16)
#define DIGI_CLIENT_MODE_GET(traffic_mode)    ((traffic_mode) & 0xFFFF)

/*************************
* The maximum case: 12xOTU2 <> 48xODU1 <> 96xODU0 <> ODUk SW <> ILKN
* serdes:         12+12; 
* ODUk stage2:    48;
* ODUk stage3A:   96;
* ODUk stage3B/4: 96; (including interlaken)
*************************/
#define DIGI_NUM_CHNL_MAX                      264

/* channel ID information */
#define DIGI_TRIB_SLOT_GET(chnl_id)            ((chnl_id) & 0x00FFFFFF)
#define DIGI_SERDES_PORT_GET(chnl_id)          ((chnl_id) >> 24)
#define DIGI_CHNL_ID_SET(serdes_port, ts_port) (((serdes_port) & 0xFF) << 24 | ((ts_port) & 0x00FFFFFF))
/* default channel ID */
#define DIGI_DEFAULT_CHNL_ID                   0xFFFFFFFF
#define DIGI_SWITCH_CHNL_ID                    0xFFFFFFFE
#define DIGI_TEMP_CHNL_ID                      0xFFFFFF

/* TCM pool resource information */
#define DIGI_TCM_POOL_MAX               256
#define DIGI_TCM_ENABLE                 0xF
#define DIGI_TCM_EN_GET(tcm_pool_id)    (((tcm_pool_id) >> 12) & 0xF)  
#define DIGI_TCM_MODE_GET(tcm_pool_id)  (((tcm_pool_id) >> 8) & 0xF)  
#define DIGI_TCM_ID_GET(tcm_pool_id)    ((tcm_pool_id) & 0xFF) 
#define DIGI_TCM_SET(tcm_en, tcm_mode, tcm_id)    ((((tcm_en) & 0xF) << 12) | (((tcm_mode) & 0xF) << 8) | ((tcm_id) & 0xFF))

/* interlaken information */
#define DIGI_ILKN_PMON_GET(ilkn_info)   (((ilkn_info) >> 24) & 0xFF)
#define DIGI_ILKN_CHNL_GET(ilkn_info)   (((ilkn_info) >> 8) & 0xFFFF)
#define DIGI_ILKN_RATE_GET(ilkn_info)   ((ilkn_info) & 0xFF)
#define DIGI_ILKN_CHNL_MAX              384
#define DIGI_ILKN_TOTAL_RATE            96

/* switch information */
#define DIGI_SWT_EN_GET(src_inf)        (((src_inf) >> 15) & 0x1)
#define DIGI_SWT_ID_GET(src_inf)        ((src_inf) & 0x3FFF)
#define DIGI_SWT_FLAG_GET(src_inf)      (((src_inf) >> 14) & 0x1)
#define DIGI_SWT_FLAG_CLEAR(src_inf)    ((src_inf) & ~(0x1 << 14))
#define DIGI_SWT_SET(enable, flag, src_index) ((((enable) & 0x1) << 15) | (((flag) & 0x1) << 14) | ((src_index) & 0x7FFF))

/* CPB switch information for MAPOTN */
#define DIGI_SWT_CBR_SET(src_inf, enable, flag, dir)  (((src_inf) & 0xF0) | (((enable) & 0x1) << 3) | (((flag) & 0x1) << 2) | ((dir) & 0x3))
#define DIGI_SWT_MAP_SET(src_inf, enable, flag, dir)  (((src_inf) & 0xF) | (((((enable) & 0x1) << 3) | (((flag) & 0x1) << 2) | ((dir) & 0x3)) << 4))
#define DIGI_SWT_CBR_EN_GET(src_inf)                  (((src_inf) >> 3) & 0x1)
#define DIGI_SWT_CBR_DIR_GET(src_inf)                 ((src_inf) & 0x3)
#define DIGI_SWT_CBR_FLAG_GET(src_inf)                (((src_inf) >> 2) & 0x1)
#define DIGI_SWT_MAP_EN_GET(src_inf)                  (((src_inf) >> 7) & 0x1)
#define DIGI_SWT_MAP_DIR_GET(src_inf)                 (((src_inf) >> 4) & 0x3)
#define DIGI_SWT_MAP_FLAG_GET(src_inf)                (((src_inf) >> 6) & 0x1)
#define DIGI_CPB_FLAG_CLEAR(src_inf)                  ((src_inf) & ~((0x1 << 2) | (0x1 << 6)))

/* datapath flag information */
#define DIGI_RX_FLAG_GET(enable)        (((enable) >> 30) & 0x1)
#define DIGI_TX_FLAG_GET(enable)        (((enable) >> 31) & 0x1)
#define DIGI_RX_TX_FLAG_GET(enable)     (((enable) >> 30) & 0x3)
#define DIGI_FLAG_CLEAR(enable)         ((enable) & ~(0x3 << 30))
#define DIGI_EN_SET(enable, flag)       ((((flag) & 0x3) << 30) | ((enable) & 0xFFFF))

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_ilkn_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for interlaken processing modes.  The
*   element terms have a syntax:
*      DIGI_ILKN_<SW/CPB>_<application>
*
* ELEMENTS:
*   DIGI_ILKN_SW_COPI1                - COPI #1 block through ODUk SWITCH
*   DIGI_ILKN_CPB_COPI1               - COPI #1 block through CPB
*   DIGI_ILKN_SW_COPI2                - COPI #2 block through ODUk SWITCH
*   DIGI_ILKN_CPB_COPI2               - COPI #2 block through CPB
*
*******************************************************************************/
typedef enum
{
    DIGI_ILKN_SW_COPI1  = 0,
    DIGI_ILKN_CPB_COPI1,
    DIGI_ILKN_SW_COPI2,
    DIGI_ILKN_CPB_COPI2,

    LAST_DIGI_ILKN,      /* this is for out of range values error handling */
} digi_ilkn_mode_t;

/*******************************************************************************
* ENUM: digi_switch_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for fabric switch.  The
*   element terms have a syntax:
*      DIGI_SWITCH_<application>
*
* ELEMENTS:
*   DIGI_SWITCH_ODUK_CPB            - ODUk and CPB SWITCH
*   DIGI_SWITCH_CPB                 - CPB SWITCH
*   DIGI_SWITCH_ODUK                - ODUk SWITCH
*
*******************************************************************************/
typedef enum
{
    DIGI_SWITCH_ODUK_CPB = 0,
    DIGI_SWITCH_CPB,
    DIGI_SWITCH_ODUK,

    LAST_DIGI_SWITCH,      /* this is for out of range values error handling */
} digi_switch_mode_t;

/*******************************************************************************
* ENUM: digi_traffic_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET/OTU/CBR major processing modes.  The
*   element terms have a syntax:
*      DIGI_<line/sys interface>_<application>
*
* ELEMENTS:
*   DIGI_LINE_OTU4                          - OTU4                
*   DIGI_LINE_OTU3E2                        - OTU3E2
*   DIGI_LINE_OTU3E1                        - OTU3E1
*   DIGI_LINE_OTU3                          - OTU3
*   DIGI_LINE_OTU2F                         - OTU2F
*   DIGI_LINE_OTU1F                         - OTU1F
*   DIGI_LINE_OTU2E                         - OTU2E
*   DIGI_LINE_OTU1E                         - OTU1E
*   DIGI_LINE_OTU2                          - OTU2
*   DIGI_LINE_100GE_MAC                     - Serdes 100GE MAC Terminated
*   DIGI_LINE_100GE_TRANSPARENT             - Serdes 100GE CBR
*   DIGI_LINE_40GE_MAC                      - Serdes 40GE MAC Terminated 
*   DIGI_LINE_40GE_TRANSPARENT              - Serdes 40GE CBR           
*   DIGI_LINE_10GE_MAC                      - Serdes 10GE MAC Terminated
*   DIGI_LINE_10GE_GSUP43_7_3_TRANSPARENT   - Serdes 10GE Gsup43 7.3
*   DIGI_LINE_10GE_TRANSPARENT              - Serdes 10GE CBR       
*   DIGI_LINE_GSUP43_6_2_100GE_MAC          - MAPOTN 100GE GFP mapped Gsup 43 6.2 MAC Terminated
*   DIGI_LINE_G709_100GE_MAC                - MAPOTN 100GE GMP mapped MAC Terminated
*   DIGI_LINE_GSUP43_6_2_40GE_MAC           - MAPOTN 40GE GFP mapped Gsup 43 6.2 MAC Terminated 
*   DIGI_LINE_G709_40GE_MAC                 - MAPOTN 40GE GMP mapped MAC Terminated             
*   DIGI_LINE_GSUP43_7_3_10GE_MAC           - MAPOTN 10GE Gsup43 7.3 GFP mapped MAC Terminated     
*   DIGI_LINE_GSUP43_6_2_10GE_MAC           - MAPOTN 10GE Gsup43 6.2 GFP mapped MAC Terminated     
*   DIGI_LINE_GSUP43_10GE_MAC               - MAPOTN 10GE Gsup43 7.1/7.2 BMP mapped MAC Terminated 
*   DIGI_LINE_STS768                        - STS768
*   DIGI_LINE_STM256                        - STM256
*   DIGI_LINE_STS192                        - STS192
*   DIGI_LINE_STM64                         - STM64           
*   DIGI_LINE_FC1200_ODU1F                  - FC1200 into ODU1F    
*   DIGI_LINE_FC1200_ODU2F                  - FC1200 into ODU2F    
*   DIGI_LINE_FC1200_GFP_ODU2E              - FC1200 into ODU2E with GFP
*   DIGI_LINE_FC800                         - FC800           
*   DIGI_LINE_CPRI_5G                       - CPRI 5G         
*   DIGI_LINE_CPRI_6G                       - CPRI 6G         
*   DIGI_LINE_CPRI_10G                      - CPRI 10G        
*   DIGI_LINE_GDPS_5G                       - GDPS 5G         
*   DIGI_LINE_GDPS_10G                      - GDPS 10G        
*   DIGI_LINE_STS192_PRBS                   - STS192 PRBS     
*   DIGI_LINE_STM64_PRBS                    - STM64 PRBS      
*
*******************************************************************************/
typedef enum
{
    /* line OTU */
    DIGI_LINE_OTU4                          = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU4),
    DIGI_LINE_OTU3E2                        = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU3E2),
    DIGI_LINE_OTU3E1                        = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU3E1),
    DIGI_LINE_OTU3                          = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU3),
    DIGI_LINE_OTU2F                         = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU2F),
    DIGI_LINE_OTU1F                         = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU1F),
    DIGI_LINE_OTU2E                         = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU2E),
    DIGI_LINE_OTU1E                         = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU1E),
    DIGI_LINE_OTU2                          = ((DIGI_TYPE_OTU << 24) | (DIGI_LINE_SIDE << 16) | DIGI_OTN_SERVER_OTU2),
    /* line Ethernet */
    DIGI_LINE_100GE_MAC                     = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_100GE_MAC),
    DIGI_LINE_100GE_TRANSPARENT             = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_100GE_TRANSPARENT),
    DIGI_LINE_100GE_TRANSPARENT_RX_PMON     = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON),
    DIGI_LINE_40GE_MAC                      = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_40GE_MAC),
    DIGI_LINE_40GE_TRANSPARENT              = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_40GE_TRANSPARENT),
    DIGI_LINE_40GE_TRANSPARENT_RX_PMON      = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON),
    DIGI_LINE_10GE_MAC                      = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_10GE_MAC),
    DIGI_LINE_10GE_GSUP43_7_3               = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT),
    DIGI_LINE_10GE_GSUP43_7_3_RX_PMON       = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON),
    DIGI_LINE_10GE_TRANSPARENT              = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_10GE_TRANSPARENT),
    DIGI_LINE_10GE_TRANSPARENT_RX_PMON      = ((DIGI_TYPE_ETH << 24) | (DIGI_LINE_SIDE << 16) | DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON),
    /* line CBR */
    DIGI_LINE_STS768                        = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_40G_STS768),
    DIGI_LINE_STM256                        = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_40G_STM256),
    DIGI_LINE_STS192                        = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_STS192),
    DIGI_LINE_STM64                         = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_STM64),
    DIGI_LINE_FC1200_ODU1F                  = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_ODU1F),
    DIGI_LINE_FC1200_ODU2F                  = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_ODU2F),
    DIGI_LINE_FC1200_GFP_ODU2E              = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E),
    DIGI_LINE_FC800                         = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC800),
    DIGI_LINE_CPRI_5G                       = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_5G),
    DIGI_LINE_CPRI_6G                       = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_6G),
    DIGI_LINE_CPRI_10G                      = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_10G),
    DIGI_LINE_GDPS_5G                       = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_GDPS_5G),
    DIGI_LINE_GDPS_10G                      = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_GDPS_10G),
    DIGI_LINE_STS192_PRBS                   = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_STS192_PRBS),
    DIGI_LINE_STM64_PRBS                    = ((DIGI_TYPE_CBR << 24) | (DIGI_LINE_SIDE << 16) | DIGI_CBR_CLIENT_10G_STM64_PRBS),
    /* system OTU */
    DIGI_SYS_OTU4                           = ((DIGI_TYPE_OTU << 24) | (DIGI_SYS_SIDE << 16) | DIGI_OTN_SERVER_OTU4),
    DIGI_SYS_OTU3E2                         = ((DIGI_TYPE_OTU << 24) | (DIGI_SYS_SIDE << 16) | DIGI_OTN_SERVER_OTU3E2),
    DIGI_SYS_OTU3E1                         = ((DIGI_TYPE_OTU << 24) | (DIGI_SYS_SIDE << 16) | DIGI_OTN_SERVER_OTU3E1),
    DIGI_SYS_OTU3                           = ((DIGI_TYPE_OTU << 24) | (DIGI_SYS_SIDE << 16) | DIGI_OTN_SERVER_OTU3),
    /* system Ethernet */ 
    DIGI_SYS_100GE_MAC                      = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_100GE_MAC),
    DIGI_SYS_100GE_TRANSPARENT              = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_100GE_TRANSPARENT),
    DIGI_SYS_100GE_TRANSPARENT_RX_PMON      = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON),
    DIGI_SYS_40GE_MAC                       = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_40GE_MAC),
    DIGI_SYS_40GE_TRANSPARENT               = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_40GE_TRANSPARENT),
    DIGI_SYS_40GE_TRANSPARENT_RX_PMON       = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON),
    DIGI_SYS_10GE_MAC                       = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_10GE_MAC),
    DIGI_SYS_10GE_GSUP43_7_3                = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT),
    DIGI_SYS_10GE_GSUP43_7_3_RX_PMON        = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON),
    DIGI_SYS_10GE_TRANSPARENT               = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_10GE_TRANSPARENT),
    DIGI_SYS_10GE_TRANSPARENT_RX_PMON       = ((DIGI_TYPE_ETH << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON),
    /* system CBR */
    DIGI_SYS_STS768                         = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_40G_STS768),
    DIGI_SYS_STM256                         = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_40G_STM256),
    DIGI_SYS_STS192                         = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_STS192),
    DIGI_SYS_STM64                          = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_STM64),
    DIGI_SYS_FC1200_ODU1F                   = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_ODU1F),
    DIGI_SYS_FC1200_ODU2F                   = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_ODU2F),
    DIGI_SYS_FC1200_GFP_ODU2E               = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E),
    DIGI_SYS_FC800                          = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_FC800),
    DIGI_SYS_CPRI_5G                        = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_5G),
    DIGI_SYS_CPRI_6G                        = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_6G),
    DIGI_SYS_CPRI_10G                       = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_CPRI_10G),
    DIGI_SYS_GDPS_5G                        = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_GDPS_5G),
    DIGI_SYS_GDPS_10G                       = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_GDPS_10G),
    DIGI_SYS_STS192_PRBS                    = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_STS192_PRBS),
    DIGI_SYS_STM64_PRBS                     = ((DIGI_TYPE_CBR << 24) | (DIGI_SYS_SIDE << 16) | DIGI_CBR_CLIENT_10G_STM64_PRBS),
    /* system interlaken */
    DIGI_SYS_ILKN_SW_COPI1                  = ((DIGI_TYPE_INLK << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ILKN_SW_COPI1),
    DIGI_SYS_ILKN_CPB_COPI1                 = ((DIGI_TYPE_INLK << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ILKN_CPB_COPI1),
    DIGI_SYS_ILKN_SW_COPI2                  = ((DIGI_TYPE_INLK << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ILKN_SW_COPI2),
    DIGI_SYS_ILKN_CPB_COPI2                 = ((DIGI_TYPE_INLK << 24) | (DIGI_SYS_SIDE << 16) | DIGI_ILKN_CPB_COPI2),

    LAST_DIGI_TRAFFIC,      /* this is for out of range values error handling */
}digi_traffic_mode_t;

/*******************************************************************************
* ENUM: digi_mapotn_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn payload mapping modes.  The element
*   terms have a syntax:
*      DIGI_MAPOTN_<odu container>_<client_data_type>_<mapping_mode>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_MAPOTN_ODU4P_100_GE_GFP             = 1,
    DIGI_MAPOTN_ODU4P_100_GE_GFP_ENET        = 2,
    DIGI_MAPOTN_ODU3P_40_GE_GFP              = 3,
    DIGI_MAPOTN_ODU3P_40_GE_GFP_ENET         = 4,
    DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP      = 5,
    DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET = 6,
    DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP      = 7,
    DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP          = 8,
    DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP_ENET     = 9,
    DIGI_MAPOTN_ODU2EP_FC_1200_GFP           = 10,
    DIGI_MAPOTN_ODU2P_FC_800_GMP             = 11,
    DIGI_MAPOTN_ODU2EP_10G_GDPS_GMP          = 12,
    DIGI_MAPOTN_ODU2P_5G_GDPS_GMP            = 13,
    DIGI_MAPOTN_ODU2P_CPRI_5_GMP             = 14,
    DIGI_MAPOTN_ODU2P_CPRI_10_GMP            = 15,
    DIGI_MAPOTN_ODU2P_CPRI_6_GMP             = 16,
    DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP          = 17,
    DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP          = 18,
    DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP         = 19,
    DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP         = 20,
    DIGI_MAPOTN_ODUFLEX_5G_GDPS_BMP          = 21,
    DIGI_MAPOTN_ODUFLEX_10G_GDPS_BMP         = 22,
    DIGI_MAPOTN_ODUFLEX_CPRI_5_BMP           = 23,
    DIGI_MAPOTN_ODUFLEX_CPRI_10_BMP          = 24,
    DIGI_MAPOTN_ODUFLEX_CPRI_6_BMP           = 25,
    DIGI_MAPOTN_ODUFLEX_FC_800_BMP           = 26,
    DIGI_MAPOTN_ODU2FP_FC_1200_BMP           = 27,
    DIGI_MAPOTN_ODU1FP_FC_1200_BMP           = 28,
    DIGI_MAPOTN_ODU1EP_CBR_10G3_7_2_BMP      = 29,
    DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP      = 30,
    DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP      = 31,
    DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP      = 32,
    DIGI_MAPOTN_ODU3P_OC_768_CBR_BMP         = 33,
    DIGI_MAPOTN_ODU2P_OC_192_CBR_BMP         = 34,
    DIGI_MAPOTN_ODU3P_OC_768_CBR_AMP         = 35,
    DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP         = 36,
    DIGI_MAPOTN_ODU0_PACKET_GFP              = 37,
    DIGI_MAPOTN_ODU1_PACKET_GFP              = 38,
    DIGI_MAPOTN_ODU1E_PACKET_GFP             = 39,
    DIGI_MAPOTN_ODU1F_PACKET_GFP             = 40,
    DIGI_MAPOTN_ODU2_PACKET_GFP              = 41,
    DIGI_MAPOTN_ODU2E_PACKET_GFP             = 42,
    DIGI_MAPOTN_ODU2F_PACKET_GFP             = 43,
    DIGI_MAPOTN_ODU3_PACKET_GFP              = 44,
    DIGI_MAPOTN_ODU3E1_PACKET_GFP            = 45,
    DIGI_MAPOTN_ODU3E2_PACKET_GFP            = 46,
    DIGI_MAPOTN_ODU4_PACKET_GFP              = 47,
    DIGI_MAPOTN_ODUFLEX_PACKET_GFP           = 48,

    LAST_DIGI_MAPOTN,       /* this is for out of range values error handling */

} digi_mapotn_mode_t;

/*******************************************************************************
* ENUM: digi_otn_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN direction type on OTN mux/demux.  The 
*   element terms have a syntax:
*      DIGI_OTN_DIR_<dir>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_DIR_RX     = 0x1,
    DIGI_OTN_DIR_TX     = 0x2,
    DIGI_OTN_DIR_RX_TX  = 0x3,

    LAST_DIGI_OTN_DIR,       /* this is for out of range values error handling */
} digi_otn_dir_t;

/*******************************************************************************
* ENUM: digi_otn_bulk_stage_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN provision/deprovision end stage. The 
*   element terms have a syntax:
*      DIGI_OTN_BULK_<prov_end_type>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_BULK_PROV_END   = 0,
    DIGI_OTN_BULK_DEPROV_END = 1,

    LAST_DIGI_OTN_BULK,  /* this is for out of range values error handling */
}digi_otn_bulk_stage_t;

/*******************************************************************************
* ENUM: digi_init_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Adapter intialization state. The 
*   element terms have a syntax:
*      DIGI_INIT_<application>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_INIT_RESET    = 0x0,
    DIGI_INIT_DONE     = 0x1,
    DIGI_INIT_RESTART  = 0x2,
    DIGI_INIT_RESTORE  = 0x3,

    LAST_DIGI_INIT_STATE,  /* this is for out of range values error handling */
}digi_init_state_t;

/*******************************************************************************
* ENUM: digi_dp_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the datapath operation mode. The 
*   element terms have a syntax:
*      DIGI_DP_<application>
*
* ELEMENTS:
*   DIGI_DP_NORMAL      - Normal operation with necessary sequences.
*   DIGI_DP_IDEMPOTENT  - Itempotent operation with the following features:
*                         1. Have the same behavior for the same input parameters.
*                         2. Those conflicted resources can be deleted dynamically.
*
*******************************************************************************/
typedef enum
{
    DIGI_DP_NORMAL      = 0x0,
    DIGI_DP_IDEMPOTENT  = 0x1,

    LAST_DIGI_DP_OP,  /* this is for out of range values error handling */
}digi_dp_mode_t;

/*******************************************************************************
* ENUM: digi_restore_stage_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for datapath restoration after crash/warm 
*   restart. The element terms have a syntax:
*      DIGI_CLEAR_<application>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_RESTORE_START   = 0x0,
    DIGI_RESTORE_END     = 0x1,

    LAST_DIGI_RESTORE,  /* this is for out of range values error handling */
}digi_dp_stage_t;

/*******************************************************************************
* ENUM: digi_dp_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the direction of a given datapath. The 
*   element terms have a syntax:
*      DIGI_DP_<application>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_DP_NONE   = 0x0,
    DIGI_DP_RX     = DIGI_OTN_DIR_RX,
    DIGI_DP_TX     = DIGI_OTN_DIR_TX,
    DIGI_DP_RX_TX  = DIGI_OTN_DIR_RX_TX,

    LAST_DIGI_DP_DIR,       /* this is for out of range values error handling */
} digi_dp_dir_t;

/*******************************************************************************
* ENUM: digi_cpb_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for CPB switch direction. The
*   element terms have a syntax:
*      DIGI_CPB_<application>
*
* ELEMENTS:
*   DIGI_CPB_MAP_TO_CBR          - CPB switch from MAPOTN to ENET/CBR/INLK
*   DIGI_CPB_CBR_TO_MAP          - CPB switch from ENET/CBR/INLK to MAPOTN
*   DIGI_CPB_MAP_TO_MAP          - CPB switch from MAPOTN to MAPOTN
*
*******************************************************************************/
typedef enum
{
    DIGI_CPB_MAP_TO_CBR   = 0x0,
    DIGI_CPB_CBR_TO_MAP,
    DIGI_CPB_MAP_TO_MAP,
    
    LAST_DIGI_CPB_DIR, /* this is for out of range values error handling */
} digi_cpb_dir_t;

/*
** Structures and Unions
*/
typedef struct
{
    UINT32                      chnl_id;

    /* general information */
    digi_traffic_mode_t         traffic_mode;
    UINT32                      ho_chnl_id;
    /* ODUk information */
    digi_otn_mapping_type_t     mapping_type;
    digi_oduk_t                 oduk_type;
    digi_odu_line_payload_t     payload_type;
    digi_otn_dir_t              otn_dir;
    UINT32                      tx_ts_mask[3];
    UINT32                      rx_ts_mask[3];
    DOUBLE                      oduflex_rate;
    UINT16                      tcm_pool_id[4][7];
    UINT8                       tcm_pri[4][7];
    UINT8                       prbs_chid;
    UINT8                       prbs_enable;
    digi_odu_struct_odu_level_t odu_stage;
    digi_otn_odu_chnl_t         *odu_handle;
    /* OTUk information */
    digi_otn_server_fec_t       fec_type;
    digi_otn_server_chnl_t      *otu_handle;
    /* Ethernet information */
    digi_enet_client_chnl_t     *enet_handle;
    digi_enet_prbs_t            prbs_mode;
    /* CBR information */
    digi_cbr_client_chnl_t      *cbr_handle;
    /* MAPOTN information */
    digi_mapotn_mode_t          mapping_mode;
    UINT32                      ts_number;
    UINT32                      map_enable;
    digi_mapper_chnl_t          *map_handle;
    /* interlaken information */
    digi_ilkn_chnl_t            *ilkn_handle;
    UINT16                      ilkn_info;
    /* switch information */
    UINT16                      cpb_src_index;
    UINT16                      oduk_src_index;
    UINT8                       map_sw_info;
    /* G.HAO information */
    UINT32                      ghao_id;
    UINT32                      ghao_enable;

    /* enable */
    UINT32                      enable;
}digi_traffic_info_t;

typedef struct
{
    digi_oduk_t              oduk_type;
    digi_otn_mapping_type_t  mapping_type;
    digi_odu_line_payload_t  payload_type;
    digi_otn_dir_t           otn_dir;
    UINT32                   tx_ts_mask[3];
    UINT32                   rx_ts_mask[3];
    BOOL                     tp_enable;
    /* only available for ODUflex(CBR) */
    /* oduflex_rate(bps) = CBR rate(bps) * 239/238 */
    DOUBLE                   oduflex_rate;
    /* self tributary slot type for bulk provision */
    digi_odu_line_payload_t  self_ts_type;
}digi_odu_mux_t;

typedef struct
{
    digi_enet_pmon_t       line_enet_pmon;
    digi_enet_pmon_t       sys_enet_pmon;
    digi_otn_pmon_t        otn_pmon;
    digi_mapper_pmon_t     mapper_pmon;
    digi_mapper_pmon_fo2_t mapper_otn_pmon;
    digi_sifd_pmon_t       sifd_pmon;
    digi_cbr_pmon_t        cbr_pmon;

    digi_enet_int_t        line_enet_defect;
    digi_enet_int_t        sys_enet_defect;
    digi_otn_int_t         otn_defect;
    digi_otn_int_t         otn_defect_bk;
    digi_mapper_int_t      mapper_defect;
    digi_sifd_int_t        sifd_defect;
    digi_cbr_int_t         cbr_defect;
}digi_pmon_data_t;

/*----------------------------------------------------------------------------*/
/* Handle Container Definition                                                */
/*----------------------------------------------------------------------------*/
typedef struct 
{
    /* Device level configuration structures */
    digi_init_state_t    init_state;
    digi_handle_t        *digi_handle;
    pmc_sys_handle_t     *sys_handle;
    digi_dev_info_t      *dev_info;
    digi_serdes_port_t   *line_serdes[DIGI_SERDES_XFI_LANES_MAX];
    digi_serdes_port_t   *sys_serdes[DIGI_SERDES_SYS_LANES_MAX];
    digi_ilkn_link_t     *ilkn_1_ctxt;
    digi_ilkn_link_t     *ilkn_2_ctxt;
    pmc_mem_pool_handle  pmon_pool;
    PMC_UINTPTR          pmon_buf_addr;

    /* datapath operation mode */
    digi_dp_mode_t       dp_mode;
    /* PMON */
    digi_pmon_data_t     *pmon_data;
    digi_pmon_data_t     *pmon_backup;
    /* traffic configuration for datapath restoration */
    digi_traffic_info_t  *restart_info[DIGI_NUM_CHNL_MAX];
    /* interlaken table information */
    digi_ilkn_chnl_t     *ilkn_chnl[DIGI_ILKN_CHNL_MAX-DIGI_NUM_CHNL_MAX];
    /* MAPOTN resource */
    UINT32               map_lane[DIGI_MAPPER_NUM_SECONDARY_GROUPS];
    /* RCP */
    BOOL                 di_rcp_en;
    BOOL                 ri_rcp_en;

    /* traffic configuration structures */
    digi_traffic_info_t  traffic_info[DIGI_NUM_CHNL_MAX];
} digi_cfg_container_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_traffic_mode_cfg(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t traffic_mode,
                                   digi_otn_server_fec_t fec_type,
                                   UINT32 ilkn_info,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_traffic_mode_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t *traffic_mode,
                                   BOOL *enable);
PUBLIC PMC_ERROR adpt_odu_mux_cfg(UINT32 dev_id,
                                 UINT32 ho_chnl_id,
                                 UINT32 lo_chnl_id,
                                 digi_odu_mux_t odu_mux);
PUBLIC PMC_ERROR adpt_odu_mux_del(UINT32 dev_id, 
                                 UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_odu_mux_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_odu_mux_t *odu_mux);
PUBLIC PMC_ERROR adpt_client_map_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_mapotn_mode_t mapping_mode,
                                  UINT32 ts_number,
                                  BOOL enable);
PUBLIC PMC_ERROR adpt_client_map_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_mapotn_mode_t *mapping_mode,
                                  UINT32 *ts_number,
                                  BOOL *enable);
PUBLIC PMC_ERROR adpt_packet_switch(UINT32 dev_id,
                                 digi_switch_mode_t switch_mode,
                                 UINT32 src_chnl_id,
                                 UINT32 dest_chnl_id,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_packet_switch_get(UINT32 dev_id,
                                     digi_switch_mode_t switch_mode,
                                     UINT32 dest_chnl_id,
                                     UINT32 *src_chnl_id,
                                     BOOL *enable);
PUBLIC PMC_ERROR adpt_cpb_chnl_enable(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   digi_cpb_dir_t dir,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_cpb_chnl_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_cpb_dir_t dir,
                                   BOOL *enable);
PUBLIC PMC_ERROR adpt_prov_mode_set(UINT32 dev_id,
                                  digi_otn_odu_chnl_prov_mode_t mode);
PUBLIC PMC_ERROR adpt_prov_mode_get(UINT32 dev_id,
                                  digi_otn_odu_chnl_prov_mode_t *mode);
PUBLIC PMC_ERROR adpt_bulk_cfg_end(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_bulk_stage_t stage);
PUBLIC PMC_ERROR adpt_dp_restore(UINT32 dev_id, digi_dp_stage_t stage);
#ifdef PMC_COMEXPRESS
PUBLIC PMC_ERROR adpt_dp_mode_set(UINT32 dev_id, digi_dp_mode_t mode);
PUBLIC PMC_ERROR adpt_dp_mode_get(UINT32 dev_id, digi_dp_mode_t *mode);
PUBLIC PMC_ERROR adpt_dp_traffic_mode_cfg(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_traffic_mode_t traffic_mode,
                                      digi_otn_server_fec_t fec_type,
                                      UINT32 ilkn_info,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_dp_odu_mux_cfg(UINT32 dev_id,
                                    UINT32 ho_chnl_id,
                                    UINT32 lo_chnl_id,
                                    digi_odu_mux_t odu_mux);
PUBLIC PMC_ERROR adpt_dp_client_map_cfg(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_mapotn_mode_t mapping_mode,
                                     UINT32 ts_number,
                                     BOOL enable);
PUBLIC PMC_ERROR adpt_dp_packet_switch(UINT32 dev_id,
                                    digi_switch_mode_t switch_mode,
                                    UINT32 src_chnl_id,
                                    UINT32 dest_chnl_id,
                                    BOOL enable);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_DATAPATH_H */

/* 
** end of file 
*/

