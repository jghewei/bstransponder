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
#ifndef _DIGI_LOC_H
#define _DIGI_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "digi.h"
#include "coreotn.h"
#include "lineotn.h"
#include "cpuif.h"
#include "digi_dcsu_loc.h"
#include "digi_sifd_loc.h"
#include "digi_enet_loc.h"
#include "digi_reset_loc.h"
#include "digi_opsa_loc.h"
#include "digi_pgmrclk_loc.h"
#include "digi_ghao_loc.h"
#include "hostmsg.h"
#include "tl_ic_io.h"
#include "digi_m1_io.h"
#include "digi_pgmrclk_io.h"
#include "dcsu_io.h"
#include "pcbi_io.h"
#include "digi_m1_io.h"
#include "digi120_mtsb_map.h"
#include "lifd.h"
#include "sifd.h"
#include "mgmt_fege.h"
#include "pmc_ctxt_mgmt.h"
#include "pmc_bitarray.h"
#include "acb_ft_io.h"

/* Used to define recent dependancies on SW/FW version */

#ifndef SW_FW_RECENT_DEPENDANCY_SVN_REVISION
#define SW_FW_RECENT_DEPENDANCY_SVN_REVISION (0x00069568) /* FW revision 69568 */
#endif

/* Interrupt Aggregator Outbound Interrupt Line Status Register. If you change
   this register address here, you must change it in pmc_emulation.c, too. */
#define PCIE_OB_IRQ_STAT_REG     (0x01809364)
#define PCIE_OB_IRQ_RSTAT_REG    (0x01809368)
#define PCIE_OB_IRQ_MASK         (0x000037ff)  /* IPI + SS_INT[1] */

/*
 * Number of ILKN Instances which is 2 and the number of 32 bit values
 * to represent the 254 ILKN channels that is 254/32 = 8. That is
 */
#define DIGI_NUM_ILKN_INSTANCES 2
#define DIGI_BCW_CTXT_WORDS 8
#define DIGI_WORD_BIT_SIZE 32
#define DIGI_M1_PMM_DSM_TYPE                   1
#define DIGI_M1_PMM_PACKET_PERIOD              100
#define DIGI_M1_PMM_FC                         300
#define DIGI_M1_PTP_CLK_MIN                    (DOUBLE) 125E6
#define DIGI_M1_PTP_CLK_MAX                    (DOUBLE) 155.52E6
#define round(x)   ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define SFI51_D8_TX_2X_TX_SWING_LEVEL_DEFAULT_VALUE  D8_TX_2X_TX_SWING_LEVEL_603


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated state of DIGI handle and device.
*
* ELEMENTS:
    DIGI_STATE_NOT_INIT     - DIGI handle and device are not initialized
    DIGI_STATE_HANDLE_INIT  - only DIGI handle is initialized
    DIGI_STATE_DEVICE_INIT  - DIGI handle and device are initialized
*
*******************************************************************************/
typedef enum
{
    DIGI_STATE_NOT_INIT = 0,
    DIGI_STATE_HANDLE_INIT,
    DIGI_STATE_DEVICE_INIT,

    LAST_DIGI_STATE

} digi_state_t;

/*******************************************************************************
* ENUM: cbrc_serdes_intf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated the serdes interface that is used by the CBRC
*   subsystem.
*
* ELEMENTS:
*   CBRC_SERDES_INTF_LINE - Line Serdes Interface
*   CBRC_SERDES_INTF_SYS  - System Serdes Interface
*
*******************************************************************************/
typedef enum
{
    CBRC_SERDES_INTF_LINE = 0,
    CBRC_SERDES_INTF_SYS,

    LAST_CBRC_SERDES_INTF       /* this is for out of range values error handling                                   */

} cbrc_serdes_intf_t;

/*******************************************************************************
* ENUM: line_serdes_post_mux_dsi_format_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated the set of Serdes modules that are connected to
*   the DIGI device.  Based on the set of Serdes modules used, the SW driver
*   will use a fixed relationship for the Post Mux DSI Lane assignment.
*   Regardless of which physical pins a module is connected to, they will be
*   limited to transfer to LINE_OTN, CBRC or ENET_LINE on fixed Post DSI Lane
*   resources.
*
*
* ELEMENTS:
*   See typedef.  'Post Mux DSI Lane Assignment' uses the notation:
*   - 'A' to describe LINE_SERDES_DSI_LANES_2G5
*   - 'B' to describe LINE_SERDES_DSI_LANES_10G
*   'Post Mux DSI Lane Assignment' presents the sets of base post mux DSI Lane
*   addresses for each 'HW interface presented'.
*
*******************************************************************************/
typedef enum
{                                     /* Max data rate    Rate breakdown              HW interfaces                       Base Post Mux DSI Lane Assignment*/

    LINE_SERDES_DSI_FORMAT_0 = 0,     /* 100GB            1x100GB                     1xOTL4.10                           {B 0}                            */
    LINE_SERDES_DSI_FORMAT_1,         /* 100GB            1x100GB                     1xSFI-S(n=10+1)                     {B 0}                            */
    LINE_SERDES_DSI_FORMAT_2,         /* 100GB            1x100GB                     2xSFI-S(n=5+1)                      {B 0}                            */

    LINE_SERDES_DSI_FORMAT_3,         /* 120GB            12x10GB                     12xOTU2                             {B 0 to 11}                      */
    LINE_SERDES_DSI_FORMAT_4,         /* 120GB            3x40GB                      3xOTL3.4                            {B 0,4,8}                        */
    LINE_SERDES_DSI_FORMAT_5,         /* 120GB            1x40GB + 8x10GB             1xOTL3.4 + 8xOTU2                   {B 0}, {B 4 to 11}               */
    LINE_SERDES_DSI_FORMAT_6,         /* 120GB            2x40GB + 4x10GB             2xOTL3.4 + 4xOTU2                   {B 0,4}, {B 8 to 11}             */

    LINE_SERDES_DSI_FORMAT_7,         /* 110GB            1x40GB + 1x40GB + 3x10GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  {B 8}, {B 0}, {B5 to 7}          */
    LINE_SERDES_DSI_FORMAT_8,         /* 110GB            1x40GB + 7x10GB             1xSFI-S(n=4+1) + 7xOTU2             {B 0}, {B 5 to 11}               */
    LINE_SERDES_DSI_FORMAT_9,         /* 100GB            2x40GB + 2x10GB             2xSFI-S(n=4+1) + 2xOTU2             {B 0}, {B 5}, {B 10 to 11}       */

    LINE_SERDES_DSI_FORMAT_10,        /* 120GB            1x40GB + 8x10GB             1xSFI5.1 + 8xOTU2                   {A 0},{B 4 to 11}                */
    LINE_SERDES_DSI_FORMAT_11,        /* 120GB            1x40GB + 1x40GB + 4x10GB    1xOTL3.4 + 1xSFI5.1 + 4xOTU2        {B 4}, {A 0}, {B 8 to 11}        */
    LINE_SERDES_DSI_FORMAT_12,        /* 120GB            2x40GB + 1x40GB             2xOTL3.4 + 1xSFI5.1                 {B 4,8}, {A 0}                   */
    LINE_SERDES_DSI_FORMAT_13,        /* 120GB            1x40GB + 1x40GB + 4x10GB    1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  {B 5}, {A 0}, {B 0 to 3}         */
    LINE_SERDES_DSI_FORMAT_14,        /* 120GB            1x40GB + 1x40GB + 1x40GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1{B 0}, {B 5}, {A 0}              */
    LINE_SERDES_DSI_FORMAT_15,        /* 120GB            2x40GB + 1x40GB             2xSFI-S(n=4+1) + 1xSFI5.1           {B 0,5}, {A 0}                   */
    LINE_SERDES_DSI_FORMAT_16,        /* 120GB            2x40GB + 4x10GB             2xSFI5.1 + 4xOTU2                   {A 0}, {A 17}, {B 0 to 3}        */
    LINE_SERDES_DSI_FORMAT_17,        /* 120GB            2x40GB + 1x40GB             2xSFI5.1 + 1xOTL3.4                 {A 0}, {A 17}, {B 0}             */
    LINE_SERDES_DSI_FORMAT_18,        /* 120GB            2x40GB + 1x40GB             2xSFI5.1 + 1xSFI-S(n=4+1)           {A 0}, {A 17}, {B 0}             */


    LAST_LINE_SERDES_DSI_FORMAT       /* this is for out of range values error handling                                   */

} line_serdes_post_mux_dsi_format_t;

/*******************************************************************************
* ENUM: line_serdes_post_mux_dsi_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   To be completed.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    LINE_SERDES_POST_MUX_DSI_LANE_10G_XFI = 0,
    LINE_SERDES_POST_MUX_DSI_LANE_10G_STL,
    LINE_SERDES_POST_MUX_DSI_LANE_10G_SFIS,
    LINE_SERDES_POST_MUX_DSI_LANE_2_5G,
    LAST_LINE_SERDES_POST_MUX_DSI_LANES
} line_serdes_post_mux_dsi_port_t;

/*******************************************************************************
* ENUM: digi_serdes_acb_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining serdes ACB mode of operation\n\n
*
*   Terms take on the following scheme: \n
*   DIGI_ALL_SERDES_INTF_[serdes_type]_[intf_name] \n
*   Where:\n
*   [serdes_type]: specifies the serdes instatiation type\n
*   [intf_name]:   specifies the interface name as per device HW specification
*
*
*
*
* ELEMENTS:
*    DIGI_SERDES_ACB_MODE_NOMINAL     - ACB does not influence CSU offset
*    DIGI_SERDES_ACB_MODE_B_ON_C      - ACB B input used to adjust CSU offset
*    DIGI_SERDES_ACB_MODE_B_AND_C     - ACB B and C input used to adjust CSU
*                                       offset
*    DIGI_SERDES_ACB_MODE_C           - ACB C input used to adjust CSU offset
*    DIGI_SERDES_ACB_MODE_LOOP_TIMING - ACB uses internal PMM LITE to ajust 
*                                       CSU offset
*
*******************************************************************************/
typedef enum {
    DIGI_SERDES_ACB_MODE_NOMINAL = 0,
    DIGI_SERDES_ACB_MODE_B_ON_C,
    DIGI_SERDES_ACB_MODE_B_AND_C,
    DIGI_SERDES_ACB_MODE_C,
    DIGI_SERDES_ACB_MODE_LOOP_TIMING,
    LAST_DIGI_SERDES_ACB_MODE      /* this is for out of range values error handling */

} digi_serdes_acb_mode_t;

/*******************************************************************************
* ENUM: digi_serdes_cfg_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining serdes configuration action\n\n
*   Configuration include the ACB and the multi-lane aligner
*
* ELEMENTS:
*    DIGI_SERDES_PROV_ALIGNER_ONLY       - Provision lane aligner only 
*    DIGI_SERDES_PROV                    - Provision SERDES
*    DIGI_SERDES_DEPROV                  - Deprovison SERDES
*    DIGI_SERDES_ACTIVATE                - Activate 
*    DIGI_SERDES_DEACTIVATE              - Deactivate
*    DIGI_SERDES_ENET_OTN_MAX_PPM_SET    - Configure serdes ACB FT maximum pmm
*                                          such as PPM MAX = ENET PPM + OTN PPM
*                                          
*
*
*******************************************************************************/
typedef enum {
    DIGI_SERDES_PROV_ALIGNER_ONLY,
    DIGI_SERDES_PROV,
    DIGI_SERDES_PROV_WITH_CLK_AS_MASTER,
    DIGI_SERDES_DEPROV,                                          
    DIGI_SERDES_ACTIVATE,
    DIGI_SERDES_DEACTIVATE,   
    DIGI_SERDES_ENET_OTN_MAX_PPM_SET,
    LAST_DIGI_SERDES_CFG_ACTION      /* this is for out of range values error handling */

} digi_serdes_cfg_action_t;

/*******************************************************************************
* ENUM: digi_group_size_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for post MLD secondary group size. 
*
*
* ELEMENTS:
*   DIGI_GRP_SIZE_10G   - 10G
*   DIGI_GRP_SIZE_40G   - 40G
*   DIGI_GRP_SIZE_100G  - 100G
*                           
*
*******************************************************************************/
typedef enum
{
    DIGI_GRP_SIZE_10G = 0,
    DIGI_GRP_SIZE_40G,
    DIGI_GRP_SIZE_100G,

    LAST_DIGI_GRP_SIZE    
} digi_group_size_t;



/*******************************************************************************
* ENUM: digi_enet_enhanced_pmon_req_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for returning the possibility of ENET PMON. 
*
* ELEMENTS:
*   DIGI_ENHANCED_PMON_NO_PMON_REQ                      - PMON not required.
*   DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP  - PMON is required and 
*                                                         realizable from MAPOTN.
*   DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN - PMON on ENET_SYS is 
*                                                         required and 
*                                                         realizable from ILKN1 
*                                                         or ILKN2.
*   DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN- PMON on ENET_LINE is
*                                                         required and 
*                                                         realizable from ILKN1 
*                                                         or ILKN2.
*   DIGI_ENHANCED_ENET_PMON_REQ_AND_NOT_REALIZABLE  - PMON is required and not 
*                                                       realizable.
*                           
*
*******************************************************************************/
typedef enum
{
    DIGI_ENHANCED_PMON_NO_PMON_REQ = 0,
    DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP,
    DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN,
    DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN,
    DIGI_ENHANCED_ENET_PMON_REQ_AND_NOT_REALIZABLE,

    LAST_DIGI_ENHANCED_ENET_PMON_REQ   /* this is for out of range values error handling */
} digi_enet_enhanced_pmon_req_t;

/*******************************************************************************
* ENUM: digi_enet_gsup43_pmon_clk_src_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for returning the possibility of the special initialization of
*   GSUP43_7_73 TX provisioning.
*
*   This indicates where the clock signal has come from. This is necessary when
*   deprovisioning is required.
*
* ELEMENTS:
*   DIGI_ENET_GSUP43_PMON_CLK_SRC_NONE      - No GSUP43_7_3 PMON mode has been
*                                             provisioned.
*   DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN1     - At least one GSUP43_7_3_TX PMON
*                                             mode has been provisioned using 
*                                             an ILKN1 clock src.
*   DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN2     - At least one GSUP43_7_3_TX PMON
*                                             mode has been provisioned using 
*                                             an ILKN2 clock src.
*   DIGI_ENET_GSUP43_PMON_CLK_SRC_ENET_SYS  - At least one GSUP43_7_3_TX PMON
*                                             mode has been provisioned using 
*                                             an ENET_SYS clock src. This
*                                             indicates that removing the last
*                                             GSUP43_7_3_TX PMON mode must also
*                                             deprovision the SERDES.
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_GSUP43_PMON_CLK_SRC_NONE = 0,
    DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN1,
    DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN2,
    DIGI_ENET_GSUP43_PMON_CLK_SRC_ENET_SYS, 

    DIGI_ENET_GSUP43_PMON_CLK_SRC_LAST /* this is for out of range values error handling */

} digi_enet_gsup43_pmon_clk_src_type_t;

/*******************************************************************************
* ENUM: digi_work_order_lclk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Indicates to which LCLK source to connect work order to.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_work_order_lclk
{
    WORK_ORDER_LCLK_NONE = 0,
    WORK_ORDER_LCLK_FEGE,
    WORK_ORDER_LCLK_MCPB,
    WORK_ORDER_LCLK_DCPB,
    WORK_ORDER_LCLK_SIFD,
    WORK_ORDER_LCLK_MAPOTN,
    WORK_ORDER_LCLK_SYSOTN_0,
    WORK_ORDER_LCLK_SYSOTN_1,
    WORK_ORDER_LCLK_ENETLINE,
    WORK_ORDER_LCLK_ENETSYS,
    WORK_ORDER_LCLK_CBRC,
    WORK_ORDER_LCLK_LINEOTN_0,
    WORK_ORDER_LCLK_LINEOTN_1,
    WORK_ORDER_LCLK_LINEOTN_2,
    WORK_ORDER_LCLK_COREOTN_0,
    WORK_ORDER_LCLK_COREOTN_1,
    WORK_ORDER_LCLK_COREOTN_2,
    WORK_ORDER_LCLK_CPUK_0,
    WORK_ORDER_LCLK_CPUK_1,
    WORK_ORDER_LCLK_MAX = WORK_ORDER_LCLK_CPUK_1,
} digi_work_order_lclk_t;


/*******************************************************************************
* ENUM: digi_lclk_trig_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI LCLK trigger. Trigger may be manual or automatic.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_lclk_trig
{
    DIGI_LCLK_TRIG_MANUAL = 0,
    DIGI_LCLK_TRIG_AUTO,
    DIGI_LCLK_TRIG_MAX = DIGI_LCLK_TRIG_AUTO,
} digi_lclk_trig_t;

/*******************************************************************************
* ENUM: digi_lclk_subsys_en_lclk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI LCLK enable/disable. Enable or disable subsystem lclk.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_lclk_subsys_en_lclk
{
    DIGI_LCLK_SUBSYS_LCLK_DISABLE = 0,
    DIGI_LCLK_SUBSYS_LCLK_ENABLE,
    DIGI_LCLK_SUBSYS_LCLK_EN_LCLK_MAX = DIGI_LCLK_SUBSYS_LCLK_ENABLE,
} digi_lclk_subsys_en_lclk_t;

/*******************************************************************************
* ENUM: digi_pmon_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PMON mode chooser.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_pmon_mode
{
    DIGI_PMON_MODE_DMA_ASSIST = 0,
    DIGI_PMON_MODE_SPRAM,
    DIGI_PMON_MODE_DIRECT_ACCESS,
    DIGI_PMON_MODE_MAX = DIGI_PMON_MODE_DIRECT_ACCESS,
} digi_pmon_mode_t;

/*******************************************************************************
* ENUM: digi_sifd_used_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an enum to keep track which SIFD/ILKN instances are configured and
*   in use. If both are in use, we cannot use ENET_SYS for PMON.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_sifd_used
{
    DIGI_SIFD_NOT_IN_USE = 0,
    DIGI_SIFD_1_USED,
    DIGI_SIFD_2_USED,
    DIGI_SIFD_1_AND_2_USED,

    LAST_DIGI_SIFD_USED
    
} digi_sifd_used_t;


/*******************************************************************************
* ENUM: digi_serdes_type_in_bp_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a enum to list of possible serdes types in backplane/SIFD interface.
*
* ELEMENTS:
*   DIGI_SERDES_TYPE_IN_BP_C8   - C8 serdes
*   DIGI_SERDES_TYPE_IN_BP_T8   - T8 serdes
*   DIGI_SERDES_TYPE_IN_BP_S16  - S16 serdes
*
*******************************************************************************/
typedef enum digi_serdes_type_in_bp_enum
{
    DIGI_SERDES_TYPE_IN_BP_C8,
    DIGI_SERDES_TYPE_IN_BP_T8,
    DIGI_SERDES_TYPE_IN_BP_S16,
    LAST_DIGI_SERDES_TYPE_IN_BP
}digi_serdes_type_in_bp_t;

/*******************************************************************************
* ENUM: digi_serdes_port_rate_prov_state_t;
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a enum to list serdes rate port provisioning state.
*
* ELEMENTS:
*  DIGI_SERDES_PORT_RATE_STATE_PROV               - serdes rate is provsioned
*  DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK - serdes rate is provisionned (w/ facility loopback)
*  DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK     - serdes rate is provisionned (w/ diagnostic loopback)
*  DIGI_SERDES_PORT_RATE_STATE_DEPROV             - serdes rate is deprovisioned
*
*******************************************************************************/
typedef enum digi_serdes_port_rate_prov_state_enum
{
    DIGI_SERDES_PORT_RATE_STATE_PROV = 0,
    DIGI_SERDES_PORT_RATE_STATE_PROV_RESTART_PENDING,
    DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK,
    DIGI_SERDES_PORT_RATE_STATE_PROV_FACILITY_LPBK_RESTART_PENDING,
    DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK,
    DIGI_SERDES_PORT_RATE_STATE_PROV_DIAG_LPBK_RESTART_PENDING,
    DIGI_SERDES_PORT_RATE_STATE_DEPROV,
    DIGI_SERDES_PORT_RATE_STATE_RESTART_PENDING,
    LAST_DIGI_SERDES_PORT_RATE_STATE,
    
}digi_serdes_port_rate_prov_state_t;

/*******************************************************************************
* ENUM: digi_dp_intf_serdes_operation_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a enum to list serdes rate port provisioning state.
*
* ELEMENTS:
*  DIGI_DP_INTF_SERDES_CFG              - serdes rate is configured
*  DIGI_DP_INTF_SERDES_LPBK_CFG         - serdes loopback is configured
*  DIGI_DP_INTF_SERDES_POL_CFG          - serdes polarity is configured
*  DIGI_BP_INTF_SERDES_LPBK_STATUS_GET  - Retrieve the loopback status
*
*******************************************************************************/
typedef enum
{
    DIGI_BP_INTF_SERDES_CFG = 0,
    DIGI_BP_INTF_SERDES_LPBK_CFG,
    DIGI_BP_INTF_SERDES_POL_CFG,
    DIGI_BP_INTF_SERDES_LPBK_STATUS_GET,
    LAST_DIGI_BP_INTF_SERDES_OPERATION,
    
} digi_bp_intf_serdes_operation_t;

/*******************************************************************************
* ENUM: digi_enet_pkt_fwd_dst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure enumerates the possible forwarding destination of a pkt from
*   ENET_LINE or ENET_SYS SS.
*
* ELEMENTS:
*  DIGI_ENET_PKT_FWD_DST_DCPB              - to DCPB
*  DIGI_ENET_PKT_FWD_DST_CPU               - to CPU (embedded processor)
*  DIGI_ENET_PKT_FWD_DST_MGMT_FEGE         - to MGMT_FEGE
*
*******************************************************************************/
typedef enum 
{
    DIGI_ENET_PKT_FWD_DST_DCPB = 0,
    DIGI_ENET_PKT_FWD_DST_CPU = 1,
    DIGI_ENET_PKT_FWD_DST_MGMT_FEGE = 2,

    LAST_DIGI_ENET_PKT_FWD_DST    
} digi_enet_pkt_fwd_dst_t;

/*******************************************************************************
* ENUM: digi_intr_ipi_monitor_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   HW interrups only trigger on the rising edge of the I-bit of the specified 
*   interrupts. The following IPI interrupts are used instead to indicate both
*   rising and falling edges of the firmware-based I-bits. 
*
* ELEMENTS:
*  See typedef
*******************************************************************************/
typedef enum
{
    INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL = 0,
    INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL,
    INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL,
    INTR_IPI_CBRC_FC1200_PCS_RX_RF,
    INTR_IPI_CBRC_FC1200_PCS_RX_LF,
    INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
    INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
    INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
    INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
    INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
    INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
    INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
    INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
    INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,
    LAST_INTR_IPI_WORKAROUND
}digi_intr_ipi_monitor_t;

/*******************************************************************************
* ENUM: digi_intr_ipi_monitor_running_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   HW interrups only trigger on the rising edge of the I-bit of the specified 
*   interrupts. The following enums indicate the monitors that are running in
*   FW to handle the rising and falling edges of the firmware-based I-bits.
*
* ELEMENTS:
*  See typedef
*******************************************************************************/
typedef enum
{
    INTR_IPI_ENET_LINE = 0,
    INTR_IPI_ENET_SYS,
    INTR_IPI_CBRC_FC1200,
    LAST_INTR_IPI_WORKAROUND_RUNNING
}digi_intr_ipi_monitor_running_t;

/*******************************************************************************
* ENUM: otn_maint_ins_point_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to describe the different maintenance signal insertion points
*
* ELEMENTS:
*  SYSOTN_TX    - OTU_FRM in SYSOTN TX
*  LINEOTN_TX   - OTU_FRM in LINEOTN TX
*  COREOTN_TX_HO_LINE - HO TFRM towards lineotn
*  COREOTN_TX_MO_LINE - MO TFRM towards lineotn
*  COREOTN_TX_LO_LINE - LO TFRM towards lineotn
*  COREOTN_TX_HO_SYS - HO TFRM towards SYSOTN
*  COREOTN_TX_MO_SYS - MO TFRM towards SYSOTN
*  COREOTN_RX_LINE - RFRM attached to ODUKSW towards LINEOTN
*  COREOTN_RX_SYS - RFRM attached to ODUKSW towards SYSOTN
*  
*******************************************************************************/
typedef enum
{
    SYSOTN_TX,
    LINEOTN_TX,
    COREOTN_TX_HO_LINE,
    COREOTN_TX_MO_LINE,
    COREOTN_TX_LO_LINE,
    COREOTN_TX_HO_SYS,
    COREOTN_TX_MO_SYS,
    COREOTN_RX_LINE,
    COREOTN_RX_SYS
} otn_maint_ins_point_t;




/*
** Constants
*/
#define DIGI_MIN_SERDES_LANES_FOR_40G       4
#define DIGI_MIN_SERDES_LANES_FOR_100G      10
#define DIGI_INT_COREOTN_LCLK1_TRIGGER      16


#ifdef PMC_DEVICE
#define CPU_SPRAM_OFFSET            0x01D00000
#else
#define CPU_SPRAM_OFFSET            0x01D00000
#endif


#define DIGI_SCHD_96_ENTRY                   96
#define DIGI_SCHD_10G_ENTRY_NUM              8
#define DIGI_SCHD_ENTRY_SPREAD               12
#define DIGI_SCHD_ALLOC_BITMASK              0x1  
#define DIGI_SCHD_ENTRY_UNASSGINED           0xffffffff
#define DIGI_SCHD_MAPPER_DCS_INSTANCE        3
#define DIGI_SCHD_ENET_LINE_DCS_INSTANCE     2

#define DIGI_MAX_NUM_ODU0_IN_10G             8
#define DIGI_MAX_NUM_ODU0_IN_40G             32
#define DIGI_MAX_NUM_ODU0_IN_100G            80

#define DIGI_POST_MLD_DSI_LANE_MAX           12
#define DIGI_POST_MLD_DSI_120G_BITMASK       0xFFF
#define DIGI_POST_MLD_DSI_100G_BITMASK       0x3FF
#define DIGI_POST_MLD_DSI_40G_BITMASK        0xF
#define DIGI_POST_MLD_DSI_10G_BITMASK        0x1

#define DIGI_SERDES_DSI_LANES_MAX            12
#define DIGI_SERDES_XFI_LANES_MAX            12
#define DIGI_SERDES_SYS_LANES_MAX            12
#define DIGI_SERDES_DSIS_LANES_MAX           20
#define DIGI_SERDES_GE_PORTS_MAX             6

#define DIGI_NUM_LINE_PORTS_MAX              12
#define DIGI_NUM_SYS_PORTS_MAX               12

#define DIGI_NUM_MAX_TS                      80
#define DIGI_NUM_HO_ODU_CHNL_MAX             12
#define DIGI_NUM_MO_ODU_CHNL_MAX             96
#define DIGI_NUM_LO_ODU_CHNL_MAX             96
#define DIGI_NUM_ODU_CHNL_TOT                DIGI_NUM_HO_ODU_CHNL_MAX + DIGI_NUM_MO_ODU_CHNL_MAX + DIGI_NUM_LO_ODU_CHNL_MAX            

#define DIGI_PORT_BANDWIDTH_NONE             0
#define DIGI_PORT_BANDWIDTH_10G              10
#define DIGI_PORT_BANDWIDTH_40G              40
#define DIGI_PORT_BANDWIDTH_100G             100
#define DIGI_PORT_BANDWIDTH_MAX              120
#define DIGI_CFP_BANDWIDTH_MAX               100

#define DIGI_SERDES_LINE_LANES_NONE          0
#define DIGI_SERDES_LINE_LANES_FOR_10G       1
#define DIGI_SERDES_LINE_LANES_FOR_40G       4
#define DIGI_SERDES_LINE_LANES_FOR_40G_SFIS  5
#define DIGI_SERDES_LINE_LANES_FOR_100G      10
#define DIGI_SERDES_LINE_LANES_FOR_100G_SFIS 11
#define DIGI_SERDES_LINE_LANES_FOR_100G_2X_SFIS_5 12

#define DIGI_SERDES_SYS_LANES_NONE           0
#define DIGI_SERDES_SYS_LANES_FOR_10G        1
#define DIGI_SERDES_SYS_LANES_FOR_40G        4
#define DIGI_SERDES_SYS_LANES_FOR_40G_SFIS   5
#define DIGI_SERDES_SYS_LANES_FOR_100G       10

#define DIGI_SERDES_ENET_MAX_PPM             100
#define DIGI_SERDES_OTN_MAX_PPM              20
#define DIGI_OVRCLOCKED_SERDES_OTN_MAX_PPM   60

#define DIGI_VALID_CAGE_PIN_CASE_1           1
#define DIGI_VALID_CAGE_PIN_CASE_2           4
#define DIGI_VALID_CAGE_PIN_CASE_3           5
#define DIGI_VALID_CAGE_PIN_CASE_4           10
#define DIGI_VALID_CAGE_PIN_CASE_5           11
#define DIGI_VALID_CAGE_PIN_CASE_6           12

#define DIGI_CPB_TYPE_HEADER_LENGTH          4
#define DIGI_DCPB_NUM_DPI_PORTS              6
#define DIGI_OCPB_NUM_DPI_PORTS              4
#define DIGI_CPB_PMON_COUNTERS_PER_PORT      4  

/* The number of SIFD/ILKN  instances */
#define DIGI_SIFD_BCW_CHANNEL_MAX            253
#define DIGI_SIFD_BCW_CHANNEL_LINK           254
#define DIGI_SIFD_BCW_CHANNEL_NULL           255




/* These are hardcoded values for dsi lane allocation  */
/* algorithm.                                          */
#define DSI_ALLOC_SFI_BUS_B_BITMASK       (0x3UL)
#define DSI_ALLOC_SFI_BUS_A_BITMASK       (0x2UL)
#define DSI_ALLOC_SFIS_BITMASK            (0x1UL)
#define DSI_ALLOC_OTL_BITMASK             (0x0UL)
#define DSI_ALLOC_2BIT_BITMASK            (0x3UL)

#define DSI_ALLOC_40G_BITMASK             (0x1UL)
#define DSI_ALLOC_10G_BITMASK             (0x1UL)

#define DSI_ALLOC_40G_DSI_LANE_NUM        (4)
#define DSI_ALLOC_10G_DSI_LANE_NUM        (12)
#define DSI_ALLOC_MAX_PORT_NUM            (12)
#define DSI_ALLOC_100G_MIN_DSI_LANE_NUM   (10)
#define DSI_ALLOC_SFI51_BUS_NUM           (2)


#define DSI_ALLOC_SYS_LANES_IDX           (0)
#define DSI_ALLOC_XFI_LANES_IDX           (1)

#define DSI_ALLOC_MAX_CFG_NUM             (20)
#define DSI_ALLOC_NO_CFG_SET              (0xff)

/* This is the order in which the schedule should
   be populated */
#define DIGI_SCHED_MASK { \
    {0, 12, 24, 36, 48, 60, 72, 84}, \
    {3, 15, 27, 39, 51, 63, 75, 87}, \
    {6, 18, 30, 42, 54, 66, 78, 90}, \
    {9, 21, 33, 45, 57, 69, 81, 93}, \
    {1, 13, 25, 37, 49, 61, 73, 85}, \
    {4, 16, 28, 40, 52, 64, 76, 88}, \
    {7, 19, 31, 43, 55, 67, 79, 91}, \
    {10, 22, 34, 46, 58, 70, 82, 94}, \
    {2, 14, 26, 38, 50, 62, 74, 86}, \
    {5, 17, 29, 41, 53, 65, 77, 89}, \
    {8, 20, 32, 44, 56, 68, 80, 92}, \
    {11, 23, 35, 47, 59, 71, 83, 95}, \
}

#define DIGI_FW_DOWNLOAD_NULL "NULL"
#define DIGI_FW_JMP_OFFSET                       0xBBD01200
#define DIGI_FW_STATUS_POLL_TRY_MAX              0x05
#define DIGI_FW_STATUS_POLL_INTERVAL_IN_US       500000
/* According to FW documentation, need to wait for 200ms for PCIe initialization to complete */
#define DIGI_FW_PRE_DOWNLOAD_WAIT_PERIOD_IN_US   200000
/* According to FW documentation, need to sleep for 1000us * 10 (1ms * 10) */
#define DIGI_FW_POST_DOWNLOAD_WAIT_PERIOD_IN_US  10000
#define DIGI_FW_COALESCE_WAIT_PERIOD_IN_US       10000
#define DIGI_FW_NIC_SW_RST_WAIT_IN_US            20000 /* 20 milliseconds */
#define DIGI_FW_MIP_SW_RST_WAIT_IN_US            1000
#define DIGI_FW_DOWNLOAD_BUFFER_SIZE_IN_WORDS    32

/*
 * Set the FW file names. Used in Emulation, and customer builds.
 * NULL value is used in SW SIMULATION and VERIFICATION. The default FW name
 * represents REV A FW and designed to work with REV A boards. The second
 * one is for REV B installations.
 */
#define DIGI_FW_DOWNLOAD_DEFAULT_FILE_NAME     "./digi_fw_app.mem"
#define DIGI_FW_DOWNLOAD_REVB_FILE_NAME        "./digi_fw_app_revb.mem"

/* Top of Calendar Enables */
#define DIGI_TOC_SYSOTN     0x0200
#define DIGI_TOC_ENET_LINE  0x0100
#define DIGI_TOC_ENET_SYS   0x0080
#define DIGI_TOC_CBRC       0x0040
#define DIGI_TOC_MAPOTN     0x0020
#define DIGI_TOC_SIFD       0x0010
#define DIGI_TOC_LINEOTN    0x0008
#define DIGI_TOC_COREOTN    0x0004
#define DIGI_TOC_DCPB       0x0002
#define DIGI_TOC_MCPB       0x0001



/* FW BOOT_INFO */
#define DIGI_FW_POLL_BOOT_ENABLE           0x1003
#define DIGI_FW_BOOT_SUCCESS               0x5000
#define DIGI_FW_DETECT_SOFT_RESET          0x0001
#define DIGI_FW_HOSTMSG_MAILBOX_SIZE       0x0040

#define DIGI_SERDES_MASK_NONE                0x0
#define DIGI_SERDES_S16_MASK_ALL             0xFFFF   /* 16 bits, 12 lanes */
#define DIGI_SERDES_SFI51_MASK_ALL           0xFF     /* 8 bits, 2 busses */

/* MAX number of D8 2X serdes in GE interface */
#define DIGI_GE_SERDES_D8_2X_MAX     3

/* SFI51 RX Serdes mapping for SFI51[0],SFI51[1] RX_DATA[0..16] */
#ifndef DIGI_SFI51_RX_SERDES_PAIR_LIST
#define DIGI_SFI51_RX_SERDES_PAIR_LIST {2, 6, 11, 17, 5, 9, 12, 15, 4, 7, 10, 14, 3, 8, 13, 16, 0, 0xFF}
#endif

/* SFI51 TX Serdes mapping for SFI51[0],SFI51[1] TX_DATA[0..16] */
#ifndef DIGI_SFI51_TX_SERDES_PAIR_LIST
#define DIGI_SFI51_TX_SERDES_PAIR_LIST {11, 8, 5, 2, 15, 10, 7, 1, 13, 12, 4, 3, 14, 9, 6, 0, 17, 16}
#endif

/* Maximum number of GE serdes */
#define DIGI_GE_S                 6

/* Maximum number of DCS in MCPB SS */
#define DIGI_MCPB_DCS_MAX         4
#define DIGI_MCPB_NUM_ENET_CHNL   24

/* repurposed index identifiers for mcpb_dcs_num_registered_chnl[] element */
#define DIGI_MCPB_DPI_2_3_CPU_CHNLS     3
#define DIGI_MCPB_DPI_2_3_MGMT_CHNLS    2
#define DIGI_MCPB_DPI_0_1_CPU_CHNLS     1
#define DIGI_MCPB_DPI_0_1_MGMT_CHNLS    0

/* Maximum number of MPMA/MPMO instances in a subsystem */
#define DIGI_MPMA_MPMO_NUM_MAX    96
    
#define DIGI_LATENCY_S_MAX_VALUE  38880
     
/*! Internal function pointer for interrupt processing. */
typedef PMC_ERROR (*digi_int_enable_fcn) (digi_handle_t     *digi_handle,
                                          void              *chnl_handle_ptr,   
                                          UINT32             int_type,    
                                          void              *int_table_ptr);

/*! Internal function pointer for interrupt processing. */
typedef PMC_ERROR (*digi_int_disable_fcn) (digi_handle_t     *digi_handle,
                                           void              *chnl_handle_ptr,   
                                           UINT32             int_type,    
                                           void              *int_table_ptr);

/*! Internal function pointer for interrupt processing. */
typedef PMC_ERROR (*digi_int_clear_fcn) (digi_handle_t     *digi_handle,
                                         void              *chnl_handle_ptr,   
                                         UINT32             int_type,    
                                         void              *int_table_ptr);
  
/*! Internal function pointer for interrupt processing. */
typedef PMC_ERROR (*digi_int_retrieve_fcn) (digi_handle_t     *digi_handle,
                                            void              *chnl_handle_ptr,   
                                            UINT32             int_type,    
                                            void             **int_table_pptr);

/*! Internal function pointer for interrupt processing. */
typedef PMC_ERROR (*digi_int_enabled_check_fcn) (digi_handle_t     *digi_handle,
                                                 UINT32             int_type);
  

/*
** Macro Definitions
*/
#define DIGI_CPB_PORT_TYPE_IS_MAPOTN(chnl_handle) \
    (UTIL_GLOBAL_CPB_PORT_MAPOTN == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type)) 

#define DIGI_ODUK_PORT_TYPE_IS_MAPOTN(chnl_handle) \
    (UTIL_GLOBAL_ODUK_PORT_MAPOTN == (((util_global_switch_data_def_t *)chnl_handle)->oduk_data.port_type)) 

#define DIGI_CPB_PORT_TYPE_IS_ILKN(chnl_handle) \
    ((UTIL_GLOBAL_CPB_PORT_ILKN1 == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type)) || \
     (UTIL_GLOBAL_CPB_PORT_ILKN2 == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type)))

#define DIGI_CPB_PORT_TYPE_IS_CBRC(chnl_handle) \
    (UTIL_GLOBAL_CPB_PORT_CBRC == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type))

#define DIGI_CPB_PORT_TYPE_IS_ENET_LINE(chnl_handle) \
    (UTIL_GLOBAL_CPB_PORT_ENET_LINE == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type))

#define DIGI_CPB_PORT_TYPE_IS_ENET_SYS(chnl_handle) \
    (UTIL_GLOBAL_CPB_PORT_ENET_SYS == (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type))

#define DIGI_ENH_PMON_PTR_GET(chnl_handle) \
    (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) ?  ((digi_mapper_chnl_def_t*)chnl_handle)->enet_pmon_data : (((((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) || (((util_global_switch_data_def_t *)chnl_handle)->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) ?  ((digi_ilkn_chnl_def_t*)chnl_handle)->enet_pmon_data : NULL)

#define DIGI_OTN_SERVER_IS_LINE(chnl_handle) \
    (((digi_otn_server_chnl_def_t *) chnl_handle)->sys_line_src == FALSE) 

#define DIGI_OTN_SERVER_IS_SYS(chnl_handle) \
    (((digi_otn_server_chnl_def_t *) chnl_handle)->sys_line_src == TRUE) 

#define DIGI_CBRC_IS_LINE_INTF(digi_handle)  (CBRC_SERDES_INTF_LINE == digi_handle->var.cbrc_intf)
#define DIGI_CBRC_IS_SYS_INTF(digi_handle)   (CBRC_SERDES_INTF_SYS == digi_handle->var.cbrc_intf)

#define DIGI_SERDES_MAX_PPM_COMPUTE(a, b)   ((a + b) * 1.5) 

#define DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result) \
    {                                                                   \
        /* check that mapper handle belongs to digi context */          \
        result = digi_mapper_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
        if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result) \
        {                                                               \
            PMC_ATOMIC_RETURN(digi_handle, result);                     \
        }                                                               \
    }                                                                   \

#define DIGI_ENET_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, rc)\
        /* check enet_line channel handle to make sure it belongs to digi handle pool */ \
        rc = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle); \
        if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != rc)                                          \
        {                                                               \
            rc = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle); \
        }                                                               \
        if (PMC_SUCCESS != rc) {PMC_ATOMIC_RETURN(digi_handle, rc);}    \


#define DIGI_CBR_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result) \
    /* check that cbr client handle belongs to digi context */              \
    result = digi_cbr_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
    if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result)                                          \
    {                                                                   \
        PMC_RETURN(result);                                             \
    }                                                                   \

#define DIGI_SERDES_PORT_CTXT_HANDLE_CHECK(digi_handle, port_ctxt_ptr, result)  \
    result = digi_serdes_port_ctxt_validate(digi_handle, port_ctxt_ptr); \
    if (PMC_SUCCESS != result)                                          \
    {                                                                   \
        PMC_ATOMIC_RETURN(digi_handle, result);                          \
    }                                                                   \

#define DIGI_ILKN_LINK_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result) \
    /* check that ilkn link handle belongs to digi context */              \
    result = digi_ilkn_link_handle_validate(digi_handle, chnl_ctxt_ptr); \
    if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result)   \
    {                                                                   \
        PMC_ATOMIC_RETURN(digi_handle, result);                         \
    }                                                                   \

#define DIGI_ILKN_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result) \
    /* check that ilkn channels handle belongs to digi context */              \
    result = digi_ilkn_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
    if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result)   \
    {                                                                   \
        PMC_ATOMIC_RETURN(digi_handle, result);                         \
    }                                                                   \

#define DIGI_OTN_ODU_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result) \
    /* check that odu channel handle belongs to digi context */         \
    result = NULL !=  chnl_ctxt_ptr? digi_otn_odu_chnl_handle_validate(digi_handle, chnl_ctxt_ptr)  : DIGI_ERR_INVALID_ARG; \
    if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result) \
    { \
        PMC_ATOMIC_RETURN(digi_handle, result); \
    } \

#define DIGI_OTN_SERVER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result, atomic_return) \
    /* check that (SYS)OTN server channel handle belongs to digi context */ \
    result = NULL !=  chnl_ctxt_ptr? digi_otn_server_chnl_handle_validate(digi_handle, chnl_ctxt_ptr)  : DIGI_ERR_INVALID_ARG; \
    if (DIGI_INT_ALL_CHNLS != chnl_ctxt_ptr && PMC_SUCCESS != result) \
    {                                                                    \
        result = digi_sysotn_server_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
        if (PMC_SUCCESS != result && TRUE == atomic_return)              \
        {                                                                \
            PMC_ATOMIC_RETURN(digi_handle, result);                      \
        }                                                                \
    }                                                                    \

#define DIGI_OTN_SERVER_CHNL_HANDLE_PROV_CHECK(digi_handle, otn_chnl_ctxt_ptr) \
    if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != (otn_chnl_ctxt_ptr)->header.prov_state) \
    {                                                                   \
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_NOT_PROVISONED);         \
    }                                                                   \

#define DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, chnl_ctxt_ptr)                     \
    do                                                                                     \
    {                                                                                      \
        if(digi_enet_line_chnl_handle_validate(digi_handle, chnl_ctxt_ptr) == PMC_SUCCESS) \
        {                                                                                  \
            if(digi_handle->var.digi_enet_pmon_line_usage & (1 << ((util_global_switch_data_def_t*)chnl_ctxt_ptr)->cpb_data.channel))          \
            {                                                                              \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
        if(digi_enet_sys_chnl_handle_validate(digi_handle, chnl_ctxt_ptr) == PMC_SUCCESS)  \
        {                                                                                  \
            if(digi_handle->var.digi_enet_pmon_sys_usage & (1 << ((util_global_switch_data_def_t*)chnl_ctxt_ptr)->cpb_data.channel))           \
            {                                                                              \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
        if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != (chnl_ctxt_ptr)->header.prov_state) \
        {                                                                                  \
            PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_NOT_PROVISONED);                        \
        }                                                                                  \
    } while(0); \

#define DIGI_SWITCH_DATA_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result)  \
    /* check that user switch data handle belongs to digi context */ \
    result = digi_cpb_chnl_validate(digi_handle, chnl_ctxt_ptr);        \
    if (PMC_SUCCESS != result)                                          \
    {                                                                   \
        result = digi_oduksw_chnl_validate(digi_handle, chnl_ctxt_ptr); \
    }                                                                   \
    if (PMC_SUCCESS != result)                                          \
    {                                                                   \
        result = digi_otn_server_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
    }                                                               \
    if (PMC_SUCCESS != result)                                          \
    {                                                                   \
        result = digi_sysotn_server_chnl_handle_validate(digi_handle, chnl_ctxt_ptr); \
    }                                                                   \


#define DIGI_IS_ODU_CHNL_HO_UNCHANNELIZED(digi_handle, ho_odu_ptr) \
                                 ((ho_odu_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_HO_ODU && \
                                 ho_odu_ptr->mem_ptr->payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED) || \
                                 (ho_odu_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_LO_ODU && \
                                 DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))) \

#define DIGI_IS_ODU_CHNL_MO_LO_UNCHANNELIZED(digi_handle, odu_ptr) \
                                  ((odu_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_LO_ODU && \
                                     DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) == FALSE) || \
                                    (odu_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_MO_ODU && \
                                     DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle) == FALSE && \
                                     odu_ptr->mem_ptr->payload_format == UTIL_GLOBAL_ODU_UNCHANNELIZED) || \
                                    (odu_ptr->mem_ptr->odu_level == ODU_STRUCT_LEVEL_MO_ODU && \
                                     DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle))) \



/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: digi_cfg_tbl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold all possible 10/40G DIGI configurations. each record (line)
*   in the table holds information about specific configuration
*
* ELEMENTS:
*       otu2_10g_num       - number of 10G OTU2 lanes
*       otl_40g_num        - number of 40G OT3.4 lanes
*       sfis_40g_num       - number of 40G SFIS lanes
*       sfi51_40g_num      - number of 40G SFI 5.1 lanes
*       cfg_signature_bm   - configuration signature bit map
*       
*******************************************************************************/
typedef struct
{
    UINT8   otu2_10g_num;
    UINT8   otl_40g_num;
    UINT8   sfis_40g_num;
    UINT8   sfi51_40g_num;
    UINT32  cfg_signature_bm;
    UINT8   cfg_num; 
}digi_cfg_tbl_t;

/*******************************************************************************
* STRUCTURE: digi_s16_serdes_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure that holds s16 serdes status used to determie S16 configuration
*   operation status.
*
* ELEMENTS:
*       See typedef.
*       
*******************************************************************************/
typedef struct {
    UINT8  csu0_vcoc_done_v;
    UINT8  all_calib_done_v;
    UINT8  csu0_lock_gain_v;
} digi_s16_serdes_status_t;

/*******************************************************************************
* STRUCTURE: digi_s16_serdes_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure that holds d8 serdes status used to determine D8 configuration
*   operation status.
*
* ELEMENTS:
*       See typedef.
*       
*******************************************************************************/
typedef struct {
    UINT8 vcoc_done_i2;
} digi_d8_serdes_status_t;

/*******************************************************************************
* STRUCTURE: digi_s16_serdes_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure that holds SFI51 serdes status used to determine SFI51 configuration
*   operation status.
*
* ELEMENTS:
*       See typedef.
*       
*******************************************************************************/
typedef struct digi_sfi51_serdes_status_t {
    UINT8 rx_vcoc_done_i2;
    UINT8 tx_vcoc_done_i2;    
    UINT8 loc_oc_pass;
} digi_sfi51_serdes_status_t;

/*******************************************************************************
* STRUCTURE: digi_start_adapt_serdes_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure that holds start adapotserdes status used to determine if serdes 
*   adapation has completed 
*
* ELEMENTS:
*       See typedef.
*       
*******************************************************************************/
typedef struct digi_start_adapt_serdes_status_t {
    UINT8 step0_done;    
    UINT8 step1_done;    
} digi_start_adapt_serdes_status_t;

/*******************************************************************************
* STRUCTURE: digi_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure that holds configuration parameters.
*
* ELEMENTS:
*       See typedef.
*       
*******************************************************************************/
typedef struct {    
    digi_cfg_tbl_t  digi_cfg_tbl[DSI_ALLOC_MAX_CFG_NUM];

} digi_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_int_info_ary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining interrupt information

*
* ELEMENTS:
*   int_info[LAST_DIGI_INT_TYPE] - array of info tables for all the aggregation 
*                                  types. 
*   ddeg_cache                   - cache of ddeg interrupts from FW
*   ddeg_server                  - Structure organizing ddeg interrupts by lineotn
*   ddeg_sys_server              -  Structure organizing ddeg interrupts by sysotn
*   ddeg_stage                   - Structure organizing ddeg interrupts by stage.
*                                  The ddeg cache coming from FW is organized by
*                                  i-bits and v-bits, with all the stages 
*                                  being defined as explicit fields within those
*                                  i/v bit structures. This makes it difficult
*                                  to cleanly access into the stages in a 
*                                  generic manner. The ddeg_stage structure
*                                  contains pointers to the correct i/v bit arrays,
*                                  but now organized in a way that can be indexed
*                                  by odu stage.
*   ddeg_cache_valid             - TRUE to indicate ddeg cache contains valid data
*******************************************************************************/
typedef struct digi_int_info_ary_t 
{
    digi_int_info_t             int_info[LAST_DIGI_INT_TYPE];
    digi_pmon_ddeg_t            ddeg_cache[DIGI_PMON_DDEG_MODE_MAX_NUM];
    digi_pmon_ddeg_stage_t      ddeg_server[DIGI_PMON_DDEG_MODE_MAX_NUM];
    digi_pmon_ddeg_stage_t      ddeg_sys_server[DIGI_PMON_DDEG_MODE_MAX_NUM];
    digi_pmon_ddeg_stage_t      ddeg_stage[LAST_ODU_STRUCT_LEVEL][DIGI_PMON_DDEG_MODE_MAX_NUM];
    BOOL8                       ddeg_cache_valid;
} digi_int_info_ary_t ;

/*******************************************************************************
* STRUCTURE: digi_serdes_rate_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining serdes port rate status.

*
* ELEMENTS:
*       port_mode               - serdes port rate.
*       rate_prov_state         - serdes port rate state.
*       port_prov_state         - serdes port state in post mld datapath to
*                                 know if serdes is associated to a datapath
*******************************************************************************/
typedef struct digi_serdes_rate_state_t
{
    digi_serdes_port_mode_t port_mode;  
    digi_serdes_port_rate_prov_state_t rate_prov_state;
    digi_serdes_prov_state_t port_prov_state;
} digi_serdes_rate_state_t;

/*******************************************************************************
* STRUCTURE: digi_xfi_pin_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the relationship between the cage, port
*   and the SERDES module lane used for an XFI pin.
*
* ELEMENTS:
*       cage_id                    - An identifier to indicate which cage this port
*                                    belongs to.
*       port_uid                   - Unique user assigned value for identifying
*                                    this port with the specified pin and lane.
*       module_lane_tx_index       - Zero-indexed logical Serdes module lane for the 
*                                    set of pins making up tx lanes of a Serdes port.
*       module_lane_rx_index       - Zero-indexed logical Serdes module lane for the 
*                                    set of pins making up rx lanes of a Serdes port.
*       los_pin                    - The pin index on which the LOS is carried.
*                                    Valid values are are values within the module 
*                                    lanes.
*       los_inv                    - Set to TRUE if the LOS is to be inverted, FALSE 
*                                    otherwise.             
*       post_mux_dsi_lane          - The post LIFD mux lane used by this XFI pin.
*       serdes_rate_state          - Rate state of associated serdes.
*
*******************************************************************************/
typedef struct digi_xfi_pin_ctxt_t
{
    UINT8  cage_id;
    UINT32 port_uid;
    UINT8  module_lane_tx_index;
    UINT8  module_lane_rx_index;
    UINT8  los_pin;
    BOOL8   los_inv;
    UINT8  post_mux_dsi_lane;
    digi_serdes_rate_state_t serdes_rate_state;
} digi_xfi_pin_ctxt_t;


/*******************************************************************************
* STRUCTURE: digi_m1_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the DIGI_M1 context 
*
* ELEMENTS:
*  is_pmm_lite_configured - TRUE when it is configured
*  pmm_lite_ref_clk_src - the pmm clock source 
*  pmm_lite_pkt_size - the packet size 
*  pmm_lite_pkt_period  - the packet period
*  pmm_lite_dsm_denom - PMM DSM denom
*  pmm_lite_ref_clk_freq - the ref clock frequency

*******************************************************************************/
typedef struct digi_m1_ctxt_t
{
    BOOL8  is_pmm_lite_configured;
    digi_m1_pmm_lite_clk_ref_t pmm_lite_ref_clk_src;
    UINT32 pmm_lite_pkt_size;
    UINT32 pmm_lite_pkt_period;
    UINT32 pmm_lite_dsm_denom;
    DOUBLE pmm_lite_ref_clk_freq;    
} digi_m1_ctxt_t;


/*******************************************************************************
* STRUCTURE: digi_sfi51_bus_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the post mld bus used for this port.
*
* ELEMENTS:
*     cage_id;                    - An identifier to indicate which cage this 
*                                   port belongs to.
*     port_uid                    - Unique user assigned value for identifying
*                                   this port with the specified bus.
*     post_mux_2G5_bus_allocation - The post LIFD mux 2G5 bus used by this port.
*     lane_swap_tx                - Tx Lane Order requirement
*     lane_swap_rx                - Rx Lane Order requirement
*     los_inv                     - Set to TRUE if the LOS is to be inverted, 
*                                   FALSE otherwise.      
*     serdes_rate_state           - Rate state of associated serdes..       
*******************************************************************************/
typedef struct digi_sfi51_bus_ctxt_t
{
    UINT32 port_uid;
    digi_post_mux_sfi51_bus_t post_mux_2G5_bus_allocation;
    digi_serdes_sfi51_swap_t lane_swap_tx;
    digi_serdes_sfi51_swap_t lane_swap_rx;
    BOOL8   los_inv;
    digi_serdes_rate_state_t serdes_rate_state;
} digi_sfi51_bus_ctxt_t;


/*******************************************************************************
* STRUCTURE: digi_ge_serdes_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining GE serdes port context.
*
* ELEMENTS:
*   port_id          - serdes port interface properties
*   enabled          - serdes port status: enabled(TRUE) or disabled (FALSE)
*   loopback_enabled - serdes loopback is enabled
*   prbs_mon_enabled - serdes PRBS monitor is enabled
*   prbs_gen_enabled - serdes PRBS generator is enabled
*   *csu_ctxt_ptr    - pointer to context of associated CSU
*******************************************************************************/
typedef struct digi_ge_serdes_ctxt_struct_t
{
    digi_ge_serdes_id_t port_id;  
    BOOL8 enabled;
    BOOL8 loopback_enabled;
    BOOL8 prbs_mon_enabled;
    BOOL8 prbs_gen_enabled;
} digi_ge_serdes_ctxt_t;

/*******************************************************************************
* STRUCTURE: digi_post_mld_dsi_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the post MLD dsi lane and the assigned port that is 
*   using it.
*
* ELEMENTS:
*       lane_index      - DSI lane index.
*       cage_uid        - An identifier to indicate which cage this port
*                         belongs to.
*       port_uid        - Unique user assigned value for identifying
*                         this port with the post MLD DSI lane.
*                         The value 0xFFFFFFFF DIGI_SERDES_UNASSIGNED is reserved 
*                         as the default unassigned setting.
*       prov_state      - Describes the current provisioned state against this
*                         resource.
*       map_type        - Describes the current mapper usage of this dsi lane.
*       num_odu0        - Number of ODU0s provisioned against this resource.
*******************************************************************************/
typedef struct digi_post_mld_dsi_ctxt_t
{
    UINT8  lane_index;
    UINT8  cage_uid;
    UINT32 port_uid;
    digi_serdes_prov_state_t prov_state;
    digi_mapper_alloc_t map_type;
    UINT8  num_odu0;
} digi_post_mld_dsi_ctxt_t;

/*******************************************************************************
* STRUCTURE: digi_cage_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A set of BOOLs indicating if the cage(s) for a particular interface has
*   been defined.
*
*   This is used to ensure that cage(s) per interface type are only defined
*   once per the life of the device.
*
* ELEMENTS:
*       is_line_cfp         - TRUE if this line cage is a 10 pin cage.
*       is_line_cage_cfg    - TRUE if the line cage(s) have been configured.
*       is_sys_cfp          - TRUE if this sys cage is a 10 pin cage.
*       is_sys_cage_cfg     - TRUE if the sys cage(s) have been configured.
*
*******************************************************************************/
typedef struct digi_cage_cfg_t
{
    BOOL8 is_line_cfp;
    BOOL8 is_line_cage_cfg;
    BOOL8 is_sys_cfp;
    BOOL8 is_sys_cage_cfg;

} digi_cage_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the handle data for a SERDES port. The actual port data is tied
*   to the port_uid.
*
* ELEMENTS:
* *** internally generated data ***
*       init            - A boolean to indicate if the following are valid and
*                         have been populated by the algorithm
*       master_lane     - The master lane to be used for this port
* *** user provided data below ***
*       port_uid        - Unique user assigned value for identifying
*                         serdes port.  The value 0xFFFFFFFF
*                         DIGI_SERDES_UNASSIGNED is reserved as the
*                         default unassigned setting.
*       intf            - enum, defining the serdes interface the port
*                         is associated with
*       s16_clk_cfg     - This struct defines the configuration of spare S16 
*                         serdes used a clock. s16_clk_cfg is valid when 
*                         s16_clk_cfg.pin_index is not set to 
*                         DIGI_SERDES_PIN_NOT_USED\n
*                         Nore that this field if only valid for
*                         DIGI_SERDES_LINE_INTF and DIGI_SERDES_SYSTEM_INTF 
*                         interface 
*       los_pin         - LOS pin mask
*
*******************************************************************************/
typedef struct digi_serdes_port_def_t
{
    BOOL8 init;
    UINT32 port_uid;
    digi_serdes_intf_t intf;
    digi_s16_clk_cfg_t s16_clk_cfg;
    UINT8  master_lane;
    UINT8 los_pin;
} digi_serdes_port_def_t;

/*******************************************************************************
* STRUCTURE: digi_port_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The set of handles defining all the SERDES ports in the system
*
* ELEMENTS:
*       line_port_ctxt  - An array of digi_serdes_port_t structures, the index
*                         being the the SERDES pin number. This describes all
*                         the provisioned and unprovisioned LINE ports on the
*                         device. Note that the digi_serdes_intf_t will always
*                         be set to DIGI_SERDES_LINE_INTF.
*       sfi51_port_ctxt - An array of digi_serdes_port_t structures, the index
*                         being the the SERDES pin number. This describes all
*                         the provisioned and unprovisioned SFI ports on the
*                         device. Note that the digi_serdes_intf_t will always
*                         be set to DIGI_SERDES_SFI_51_INTF.
*       sys_port_ctxt   - An array of digi_serdes_port_t structures, the index
*                         being the the SERDES pin number. This describes all
*                         the provisioned and unprovisioned SYS ports on the
*                         device. Note that the digi_serdes_intf_t will always
*                         be set to DIGI_SERDES_SYSTEM_INTF.
*
*******************************************************************************/
typedef struct digi_port_ctxt_t
{
    /*! line */
    digi_serdes_port_def_t line_port_ctxt[DIGI_NUM_LINE_PORTS_MAX];   
    /*! line */
    digi_serdes_port_def_t sfi51_port_ctxt[DIGI_NUM_SFI51_PORTS_MAX]; 
    /*! sys  */
    digi_serdes_port_def_t sys_port_ctxt[DIGI_NUM_SYS_PORTS_MAX];     
} digi_port_ctxt_t;

/*******************************************************************************
* STRUCTURE: digi_pin_bus_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the pin and bus context on which a physical port resides.
*
* ELEMENTS:
*       xfi_line_pin_ctxt    - An array of digi_xfi_line_pin_ctxt_t structures, the index
*                              being the the logical SERDES pin number. The physical
*                              SERDES pin is specified within the digi_xfi_line_pin_ctxt_t
*                              structure.
*       sfi51_bus_ctxt       - An array of digi_sfi51_bus_ctxt_t structures, the 
*                              index being the the logical SERDES port number. The 
*                              choice of physical bus is specified within the 
*                              digi_sfi51_bus_ctxt_t structure.
*       xfi_sys_pin_ctxt     - An array of digi_xfi_sys_pin_ctxt_t structures, the index
*                              being the the logical SERDES pin number. The physical
*                              SERDES pin is specified within the digi_xfi_sys_pin_ctxt_t
*                              structure.
*       ge_ctxt              - An array of digi_ge_serdes_ctxt_t structures, the index
*                              being a serdes index from 0 to DIGI_SERDES_GE_PORTS_MAX.
*                                serdes id = 0 -> OHP_0
*                                serdes id = 1 -> OHP_1
*                                serdes id = 2 -> OHP_2
*                                serdes id = 3 -> ENENT_MGMT
*                                serdes id = 4 -> RI_RCP
*                                serdes id = 5 -> DI_RCP
*
*******************************************************************************/
typedef struct digi_pin_bus_ctxt_t
{
    /*! line */
    digi_xfi_pin_ctxt_t xfi_line_pin_ctxt[DIGI_SERDES_XFI_LANES_MAX];
    /*! line */ 
    digi_sfi51_bus_ctxt_t sfi51_bus_ctxt[DIGI_NUM_SFI51_PORTS_MAX]; 
    /*! sys */       
    digi_xfi_pin_ctxt_t xfi_sys_pin_ctxt[DIGI_SERDES_SYS_LANES_MAX]; 
    /*! OHP, RCP, ENET MGMT */  
    digi_ge_serdes_ctxt_t ge_ctxt[DIGI_SERDES_GE_PORTS_MAX];                  
} digi_pin_bus_ctxt_t;


/*******************************************************************************
* STRUCTURE: digi_post_mld_dsi_line_sys_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the post MLD DSI lanes on which a pin was routed to.
*
* ELEMENTS:
*       post_mld_dsi_line_ctxt - An array of digi_post_mld_dsi_ctxt_t 
*                                structures, the index being the the post MUX
*                                DSI lane number. This value is obtained from 
*                                xfi_line_pin_ctxt in digi_pin_ctxt. 
*       post_mld_dsi_sys_ctxt  - An array of digi_post_mld_dsi_ctxt_t 
*                                structures, the index being the the post MUX
*                                DSI lane number. This value is obtained from 
*                                xfi_sys_pin_ctxt in digi_pin_ctxt. 
*
*******************************************************************************/
typedef struct digi_post_mld_dsi_line_sys_ctxt_t
{
    digi_post_mld_dsi_ctxt_t post_mld_dsi_line_ctxt[DIGI_SERDES_DSI_LANES_MAX];
    digi_post_mld_dsi_ctxt_t post_mld_dsi_sys_ctxt[DIGI_SERDES_DSI_LANES_MAX];
} digi_post_mld_dsi_line_sys_ctxt_t;


/*******************************************************************************
* STRUCTURE: digi_utilized_bandwidth_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining device bandwidth utilization.
*
* ELEMENTS:
*   is_port_cfp         - boolean to indicate if a 100G CFP port is used.
*   total_port_bw_cfgd  - total bandwidth provisioned on line ports in 10G increments
*
*******************************************************************************/
typedef struct digi_utilized_bandwidth_t
{
    BOOL8  is_port_cfp;         
    UINT8 total_port_bw_cfgd;  
} digi_utilized_bandwidth_t;

/*******************************************************************************
* STRUCTURE: digi_fw_enet_client_source_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Specifies enet client source used by FW.
*
* ELEMENTS:
*    valid          - when TRUE value of enet_cpb_port can be used.
*                     when FALSE value of enet_cpb_port should be ignored.
*    is_enet_line   - when TRUE FW enet client source is ENET_LINE,
*                     otherwise it is ENET_SYS.
*
*******************************************************************************/
typedef struct digi_fw_enet_client_source_t
{
    BOOL8 valid;
    BOOL8 is_enet_line;
}digi_fw_enet_client_source_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_mode_profile_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Contains information about pmon configuration mode.
*   and now also contains pointers to firmware statistics.
*
* ELEMENTS:
*  pmon_trig_mode - PMON Trigger Mode\n
*                   0 -- manual trigger\n
*                   1 -- automatic trigger\n
*  pmon_sync_en - Enable or disable sync of automatic PMON trigger\n
*                 0 -- Disable\n
*                 1 -- Enable\n
*  trig_period - Trigger period in miliseconds
*  data_store_vir_start_addr - Virtual address of the PMON data store.
*  data_store_phys_start_addr - Physical address of the PMON data store.
*  data_buf_size - single data buffer size.
*  num_data_buf - number of data buffers.
*  digi_fw_stats - handles to firmware statistics
*  fw_enet_client_source - ENET client source configured in FW. 
*
*******************************************************************************/
typedef struct digi_pmon_mode_profile
{
    UINT32 pmon_trig_mode;
    UINT32 pmon_sync_en;
    UINT32 trig_period;
    void*  data_store_vir_start_addr;
    UINT64 data_store_phys_start_addr;
    UINT32 data_buf_size;
    UINT32 num_data_buf;
#ifndef DIGI_FW_STATS
#define DIGI_FW_STATS
#endif
#ifdef DIGI_FW_STATS
    digi_fw_stats_t digi_fw_stats;
#endif
    digi_fw_enet_client_source_t fw_enet_client_source;
} digi_pmon_mode_profile_t;




/*******************************************************************************
* STRUCTURE: digi_mcpb_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains context configuration for a MCPB.\n\n
*
*  The element mcpb_dcs_num_register_chnl is used to track the provisioning of
*  alternate queue inputs, in order to ensure the deprovisioning of one does
*  not impact the other.\n\n
*
*  The lowest 12 bits 11:0 of the UINT32 field are used with the bit index
*  identifying the DPI master port channel ID.  The assignment of the 4 UINT32
*  elements is as follows:\n\n
*
*  mcpb_dcs_num_register_chnl[3] DPI master port 2/3 CPU channels\n
*  mcpb_dcs_num_register_chnl[2] DPI master port 2/3 MGMT channels\n
*  mcpb_dcs_num_register_chnl[1] DPI master port 0/1 CPU channels\n
*  mcpb_dcs_num_register_chnl[0] DPI master port 0/1 MGMT channels\n\n
*
* ELEMENTS:
*  mcpb_mux_mode_init           - initialization status of mcpb 
*  mcpb_dcs_num_registered_chnl - repurposed, see the description
*  mcpb_mgmt_fege_port_cfg      - mgmt_fege MCPB port configuration status
*  mcpb_cpu_port_cfg            - CPU MCPB port configuration status
*   
*******************************************************************************/
typedef struct {    
    BOOL8                      mcpb_mux_mode_init; 
    UINT32                     mcpb_dcs_num_registered_chnl[DIGI_MCPB_DCS_MAX];
    BOOL8                      mcpb_mgmt_fege_port_cfg;
    BOOL8                      mcpb_cpu_port_cfg;
} digi_mcpb_ctxt_t;

/*******************************************************************************
* STRUCTURE: digi_cpb_pmon_counter_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains context configuration for a CPB PMON counter
*
* ELEMENTS:
*  See description of digi_dcpb_pmon_cfg
*
*******************************************************************************/
typedef struct {
    cpb_pmon_mode pmon_mode;
    BOOL8 count_drop_packets;
    BOOL8 count_err_packets;
    BOOL8 count_oversize_packets;
    BOOL8 count_good_packets;
} digi_cpb_pmon_counter_t;



/*******************************************************************************
* STRUCTURE: digi_int_otn_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains enabled OTN framer, TCM PID, PRBS interrupt information
*  per HO/MO/LO/stg3b/stg4 odu level
*
* ELEMENTS:
*  See structure.
*
*******************************************************************************/
typedef struct {
    coreotn_int_framer_chnl_t    framer_ho[DIGI_OTN_SERVER_HO_CHNL_MAX];
    coreotn_int_framer_chnl_t    framer_mo[DIGI_OTN_SERVER_MO_CHNL_MAX];
    coreotn_int_framer_chnl_t    framer_lo[DIGI_OTN_SERVER_LO_CHNL_MAX];
    coreotn_int_framer_chnl_t    framer_stg3b[DIGI_OTN_SERVER_LO_CHNL_MAX];
    coreotn_int_framer_chnl_t    framer_stg4[DIGI_OTN_SERVER_LO_CHNL_MAX];
    coreotn_int_framer_chnl_t    framer_sysotn_line[DIGI_OTN_SERVER_HO_CHNL_MAX];
     
    odu_rfrm_int_pid_chnl_t      pid_ho[TCM_POOL_SIZE_12];
    odu_rfrm_int_pid_chnl_t      pid_mo[TCM_POOL_SIZE_96];
    odu_rfrm_int_pid_chnl_t      pid_lo[TCM_POOL_SIZE_96];
    odu_rfrm_int_pid_chnl_t      pid_stg3b[TCM_POOL_SIZE_96];
    odu_rfrm_int_pid_chnl_t      pid_stg4[TCM_POOL_SIZE_96];
    odu_rfrm_int_pid_chnl_t      pid_sysotn_line[TCM_POOL_SIZE_12];

    coreotn_int_prbs_chnl_t      prbs[LAST_COREOTN_PRBS_INST][COREOTN_PRBS_NUM_CHNL];

    odu_ohp_int_chnl_t           ohp_1[DIGI_OTN_SERVER_HO_CHNL_MAX];
    odu_ohp_int_chnl_t           ohp_2[DIGI_OTN_SERVER_MO_CHNL_MAX];
    odu_ohp_int_chnl_t           ohp_3[DIGI_OTN_SERVER_LO_CHNL_MAX];


    odu_ri_rcp_int_chnl_t        ri_rcp[DIGI_OTN_SERVER_LO_CHNL_MAX];
} digi_int_otn_t;




/*******************************************************************************
* STRUCTURE: digi_cpb_port_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains instances of CPB PMON counters
*
* ELEMENTS:
*  Array of CPB PMON counter instances
*
*******************************************************************************/
typedef struct {
    digi_cpb_pmon_counter_t pmon_counter[DIGI_CPB_PMON_COUNTERS_PER_PORT];
} digi_cpb_port_pmon_t;


/*******************************************************************************
* STRUCTURE: digi_serdes_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DIGI SERDES context.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct { 
    digi_serdes_port_mode_t s16_line_lanes_rate[DIGI_SERDES_XFI_LANES_MAX];
    digi_serdes_port_mode_t s16_sys_lanes_rate[DIGI_SERDES_MAX_S16_IN_BP];
    digi_serdes_port_mode_t t8_lanes_rate[DIGI_SERDES_MAX_T8_E1_IN_BP_LR];
    digi_serdes_port_mode_t c8_lanes_rate[DIGI_SERDES_MAX_C8_IN_BP_SR];
    digi_serdes_port_mode_t sfi51_lanes_rate[DIGI_NUM_SFI51_PORTS_MAX];
    digi_serdes_cfg_t       *serdes_cfg;
} digi_serdes_ctxt_t;


/* This struct is to illustrate the dynamically changing context. */
/*******************************************************************************
* STRUCTURE: digi_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing DIGI context.
*
* ELEMENTS:
*   digi_state      - DIGI handle and device state
*   is_ilkn_enabled - boolean to indicate which ILKN interface is enabled
*   sifd_ilkn_1_cfg - structure describing ILKN1 configuration data
*   sifd_ilkn_2_cfg - structure describing ILKN2 configuration data
*******************************************************************************/ 
/*******************************************************************************
*   sifd_dcpb_in_use    - tracks which SIFD/ILKN instances are used
*   sifd_1_major_mode   - enum, SIFD1 major ILKN paths
*   sifd_2_major_mode   - enum, SIFD2 major ILKN paths
*   ilkn1_tx_fc         - ILKN1 TX Flow Control data
*   ilkn1_rx_fc         - ILKN1 RX Flow Control data
*   ilkn2_tx_fc         - ILKN2 TX Flow Control data
*   ilkn2_rx_fc         - ILKN2 RX Flow Control data
*   tx_ilkn_1_bcw_dpi_map   - array for ILKN1 TX DPI channel ID to BCW channel ID mapping
*   rx_ilkn_1_bcw_dpi_map   - array for ILKN1 RX DPI channel ID to BCW channel ID mapping
*   tx_ilkn_2_bcw_dpi_map   - array for ILKN2 TX DPI channel ID to BCW channel ID mapping
*   rx_ilkn_2_bcw_dpi_map   - array for ILKN2 RX DPI channel ID to BCW channel ID mapping
*   cage_cfg    - structure describing which cage has been defined for a particular interface
*   port_ctxt   - structure defining all the SERDES ports
*   pin_bus_ctxt    - Structure defining the pin and bus context.
*   post_mld_dsi_ctxt - Structure defining the post MLD DSI lanes
*   digi_m1_ctxt - DIGI M1 context
*   digi_enet_pmon_line_usage   - ENET_LINE PMON usage
*   digi_enet_pmon_sys_usage    - ENET_SYS PMON usage
*   digi_enet_pmon_GSUP43_7_3_sys_usage - ENET PMON usage in GSup 7.3 mode
*   digi_enet_pmon_GSUP43_7_3_sys_clk_type - 
*   line_port_bw    - LINE side port bandwidth
*   sys_port_bw     - SYSTEM side port bandwidth
*   cbrc_intf       - SERDES interface used by CBRC subsystem
*   prod_app        - enum, product applications supported by the device
*   otn_cfg_mode    - enum, OTN configuration operation mode
*   mux_stages      - enum, maximum number of multiplexing stages supported
*   lineotn_cfg_num - indicates which LINE side interface is used
*   sysotn_cfg_num  - indicates which SYSTEM side interface is used
*   odu_chnl_payload    - array of payload formats of ODU channels
*   mo_prov_state_rx    - array of booleans to keep track of provisioning state
*                         of MO channel in receive direction
*   mo_prov_state_tx    - array of booleans to keep track of provisioning state
*                         of MO channel in transmit direction
*******************************************************************************/
/******************************************************************************* 
*   fw_pathname             - FW pathname on disc
*   fw_filename             - FW file name on disc
*   dcsu_init_state         - boolean to indicate which DCSU has been initialized
*   pgmrclk_ctxt            - DIGI PGMRCLK context
*   rx_bcw_chnl_id_assigned - tracks which BCW channel ID has been assigned in ILKN interfaces
*   cpb_port_pmon           - CPB PMON counter
*   oduksw_port_pmon        - ODUKSW PMON counter
*   mcpb_ctxt               - MCPB configuration context
*   serdes_ctxt             - SERDES context
*   linesys_ddeg_mode       - enum, LINEOTN or SYSOTN dDEG monitoring mode
*   ghao_cfg                - G.HAO configuration data
*   etrans_dcpb_use_model   - DCPB use model by ETRANS
*   nominal_chnl_ctxt       - array to keep track of nominal maintenance signal
*                             at different points in the TX path
*   cbr_nominal_fault       - Used to flag whether CBR pattern/loopback 
*                             FW OPSA has been added/dropped.
*   fw_intr_mon_running - indicates which interrupt monitors are running in FW
*   enabled_int         - DIGI level aggregation table for interrupt and status
*   enabled_otn_int     - OTN related interrupt information
*   int_info_ary        - interrupt callback configuration
*   digi_pmon_mode_profile  - PMON configuration mode data
*   event_cfg               - Event configuration data
*******************************************************************************/
typedef struct 
{

    digi_state_t digi_state;
    

    /* SIFD / ILKN section */
    BOOL8 is_ilkn_enabled;
    digi_sifd_ilkn_1_cfg_t sifd_ilkn_1_cfg;
    digi_sifd_ilkn_2_cfg_t sifd_ilkn_2_cfg;



    digi_sifd_used_t sifd_dcpb_in_use;
    digi_sifd_path_t sifd_1_major_mode;
    digi_sifd_path_t sifd_2_major_mode;
    sifd_ilkn_calendar_t ilkn1_tx_fc;
    sifd_ilkn_calendar_t ilkn1_rx_fc;
    sifd_ilkn_calendar_t ilkn2_tx_fc;
    sifd_ilkn_calendar_t ilkn2_rx_fc;


    UINT8 tx_ilkn_1_bcw_dpi_map[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX];
    UINT8 rx_ilkn_1_bcw_dpi_map[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX];
    UINT8 tx_ilkn_2_bcw_dpi_map[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX];
    UINT8 rx_ilkn_2_bcw_dpi_map[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX];

    /* Cage, port, pin, lane section */
    digi_cage_cfg_t cage_cfg;
    digi_port_ctxt_t port_ctxt;
    digi_pin_bus_ctxt_t pin_bus_ctxt;
    digi_post_mld_dsi_line_sys_ctxt_t post_mld_dsi_ctxt;

    /* DIGI m1 cfg */
    digi_m1_ctxt_t digi_m1_ctxt;
    
    /* ENET Enhanced PMON usage */
    UINT16 digi_enet_pmon_line_usage;
    UINT16 digi_enet_pmon_sys_usage;
    UINT8  digi_enet_pmon_GSUP43_7_3_sys_usage;
    digi_enet_gsup43_pmon_clk_src_type_t digi_enet_pmon_GSUP43_7_3_sys_clk_type;

    /* Bandwidth configuration */
    digi_utilized_bandwidth_t line_port_bw;
    digi_utilized_bandwidth_t sys_port_bw;

    cbrc_serdes_intf_t cbrc_intf;
    digi_prod_app_t prod_app;
    digi_otn_cfg_mode_t otn_cfg_mode;
    digi_otn_mux_stages_t mux_stages;
    UINT8   lineotn_cfg_num;
    UINT8   sysotn_cfg_num;
    /* SS Operational states */
    digi_odu_line_payload_t odu_chnl_payload[DIGI_NUM_ODU_CHNL_TOT];
    BOOL8 mo_prov_state_rx[DIGI_NUM_MO_ODU_CHNL_MAX];
    BOOL8 mo_prov_state_tx[DIGI_NUM_MO_ODU_CHNL_MAX];

    
    /* Firmware section */
    /*! FW path and filename on disc */
    char *fw_pathname;
    char* fw_filename; 
    BOOL8 dcsu_init_state[LAST_DIGI_DCSU_INST];

    digi_pgmrclk_ctxt_t      pgmrclk_ctxt;

    UINT32 rx_bcw_chnl_id_assigned[DIGI_NUM_ILKN_INSTANCES][DIGI_BCW_CTXT_WORDS];
    digi_cpb_port_pmon_t cpb_port_pmon[DIGI_DCPB_NUM_DPI_PORTS];
    digi_cpb_port_pmon_t oduksw_port_pmon[DIGI_OCPB_NUM_DPI_PORTS];

    /* mcpb context */
    digi_mcpb_ctxt_t mcpb_ctxt;

    /* serdes adaptation parameters context */
    digi_serdes_ctxt_t  serdes_ctxt;
    /* LINEOTN/SYSOTN ddeg mode */
    digi_otn_ddeg_mode_t linesys_ddeg_mode;
    /* G.HAO configuration and control struct */
    digi_ghao_cfg_t ghao_cfg;
    BOOL8 etrans_dcpb_use_model;

    BOOL8 nominal_chnl_ctxt[DIGI_NUM_LO_ODU_CHNL_MAX];
    BOOL8 cbr_nominal_fault[DIGI_NUM_HO_ODU_CHNL_MAX];

    /* Indicates which interrupt monitors are running in FW */
    BOOL8 fw_intr_mon_running[LAST_INTR_IPI_WORKAROUND_RUNNING];


    /* Enabled Interrupts, PMON and interrupt config*/
    digi_int_t                  *enabled_int;
    digi_int_otn_t              *enabled_otn_int;
    digi_int_info_ary_t         *int_info_ary;
    digi_pmon_mode_profile_t    *digi_pmon_mode_profile;
    digi_event_cfg_t            *event_cfg;

} digi_var_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_patt_gen_bus_width_map_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining mapping between serdes rate and corresponding 
*   pattern generator bus width
*
* ELEMENTS:
*   rate                  - serdes rate
*   patt_gen_bus_width    - value of pattern generator bus width associated to serdes rate
*******************************************************************************/
typedef struct digi_serdes_patt_gen_bus_width_map_t
{
    digi_serdes_port_mode_t rate;
    UINT8                   patt_gen_bus_width;
} digi_serdes_patt_gen_bus_width_map_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_link_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the specific ILKN configuration
*
* ELEMENTS:
*       ilkn_inst       - digi_sifd_ilkn_inst_enum
*                         0 - DIGI_SIFD_ILKN1  Interlaken 1 Instance
*                         1 - DIGI_SIFD_ILKN2  Interlaken 2 Instance
*
*******************************************************************************/
typedef struct digi_ilkn_link_def_t
{
    /* First element should be the ILKN instance */
    digi_sifd_ilkn_inst_t ilkn_inst;
} digi_ilkn_link_def_t;


/*******************************************************************************
* STRUCTURE: digi_cbr_client_chnl_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common cbr output parameters
*
* ELEMENTS:
* 
*       switch_data     - a util_global_switch_data_def_t structure.
*       serdes_mode     - Configuration mode for serdes port
*       mode            - Mode used when channel is provisioned and from
*                         which the serdes_mode is derived.
*       mux_dsi_chnl    - pointer to base post MUX DSI channel that is
*                         processing the data for user specified dsi_chnl
*       port_uid        - Serdes port associated with this handle.
*
*******************************************************************************/
typedef struct digi_cbr_client_chnl_def_t
{
    util_global_switch_data_def_t switch_data;
    digi_serdes_port_mode_t serdes_mode;
    digi_cbr_client_mode_t mode;
    UINT8  mux_dsi_chnl;
    UINT32 port_uid;
} digi_cbr_client_chnl_def_t;

/*******************************************************************************
* STRUCTURE: digi_mapper_chnl_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common MAPOTN Channel properties.  A MAPOTN channel
*   handle composes COREOTN, MAPOTN and possibly ENET_LINE resources forming a
*   single channel between ODUKSW and CPB Subsysetem
*
* ELEMENTS:
* 
*       switch_data     - a util_global_switch_data_def_t structure.
*       mode            - mapper adaptaion function enum 
*       enet_pmon       - 10GE/40GE/100GE passive monitoring of CBR Ethernet
*                         signals
*       mapotn_chnl     - MAPOTN channel
*       odu_data        - ODU container data for the channel.
*       odu_flex_data   - Number of ODU0 tributary slots making up the
*                         ODUFlex container.
*       enet_pmon_data  - a digi_enet_client_chnl_def_t structure pointing 
*                         to the enet_line/enet_sys structure if not NULL 
*                         so it can be demapped when it is no longer required
*
*******************************************************************************/
typedef struct digi_mapper_chnl_def_t
{
    util_global_switch_data_def_t switch_data;
    digi_mapping_mode_t mode;
    digi_enet_pmon_type_t enet_pmon;
    UINT16 mapotn_chnl;
    digi_mapper_primary_group_t primary_group;
    digi_mapper_secondary_group_t secondary_group;
    digi_oduk_t odu_data;
    UINT32 odu_flex_data;
    digi_enet_client_chnl_def_t* enet_pmon_data;
    UINT8 post_mld_index;
} digi_mapper_chnl_def_t;

/*******************************************************************************
* STRUCTURE: digi_otn_server_chnl_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the OTN server Channel properties
*
* ELEMENTS:
*       header              - First element of all channel handles
*       dsi_base_chnl       - pointer to dsi chnl base lane used to transfer signal
*                             from MUX to LINE OTN subsystem
*       dsi_serdes_type     - dsi serdes interface type (OTUk,OTL,SFIS)
*       dci_chnl            - dci_chnl associated with LINE OTN framer
*                             processing OTN signal
*       port_uid            - user port id associated with this dsi lane
*       serdes_mode         - Configuration mode for serdes port
*       loopback_enabled    - Indicate if a loopback has been enabled 
*       odu_struct_ptr      - pointer to HO ODU struct that contains configuration
*                             of the HO OTN framer
*       oduk_type           - ODUk frame type (UTIL_GLOBAL_ODU0,UTIL_GLOBAL_ODU1 ... )
*       sys_line_src        - FALSE -> LINEOTN is the source
*                             TRUE  -> COREOTN is the source
*       
*******************************************************************************/
typedef struct digi_otn_server_chnl_def_t
{
    util_global_chnl_header_t header;

    /* lineotn parameters */
    UINT8  dsi_base_chnl;           
    UINT8  dsi_serdes_type;
    UINT32 dci_chnl;
    UINT32 port_uid; 
    digi_serdes_port_mode_t serdes_mode;
    BOOL8  loopback_enabled;

    /*! coreotn parameters */
    struct odu_struct_t* odu_struct_ptr;
    UINT8 oduk_type;

    /*SYSOTN or LINEOTN context*/
    BOOL8 sys_line_src;
} digi_otn_server_chnl_def_t;

/*******************************************************************************
* STRUCTURE: digi_handle_pool_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Contains information about all the digi_handles that would be required.
*
* ELEMENTS:
*  ilkn_link            - The two interlaken contexts indexed on the ILKN instance.
*  ilkn_1_chnl          - The ILKN 1 channel handles.
*  ilkn_2_chnl          - The ILKN 2 channel handles.
*  enet_line_chnl       - The ENET Line channel handles.
*  enet_sys_chnl        - The ENET System channel handles.
*  cbr_chnl             - The CBRC channel handles.
*  mapper_chnl          - The MAPOTN channel handles.
*  otn_server_chnl      - The LINE/SYS OTN channel handles.
*  sysotn_server_chnl   - The SYSOTN channel handles
*  ho_odu_struct        - The HO ODU struct channel handles.
*  mo_odu_struct        - The MO ODU struct channel handles.
*  lo_odu_struct        - The LO ODU struct channel handles.
*  s3b_odu_struct       - The COREOTN Stage 3B channel handles.
*  s4_odu_struct        - The COREOTN Stage 4 channel handles.
*  sysotn_line_struct   - The COREOTN LINE HO channel handles in SYSOTN application
*  ho_odu_chnl          - The HO ODU channel handles.
*  mo_odu_chnl          - The MO ODU channel handles.
*  lo_odu_chnl          - The LO ODU channel handles.
*  s3b_odu_chnl         - The COREOTN Stage 3B channel handles.
*  dummy_odu_chnl_stg4  - The COREOTN Stage 4 channel handle.
*  dummy_odu_chnl_stg2_sysotn - The COREOTN Stage 2 channel handle in SYSOTN application
*
*******************************************************************************/
typedef struct digi_handle_pool_t {

    /*! Indexed on ILKN instance */
    digi_ilkn_link_def_t ilkn_link[DIGI_SIFD_ILKN_NUM];  
    /*! Indexed on DPI channel number. First set is CPB, followed by ODUK */
    digi_ilkn_chnl_def_t ilkn_1_chnl[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX]; 
    /*! Indexed on DPI channel number. First set is CPB, followed by ODUK */
    digi_ilkn_chnl_def_t ilkn_2_chnl[DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX]; 

    digi_enet_client_chnl_def_t enet_line_chnl[DIGI_ENET_LINE_CHNL_MAX];
    digi_enet_client_chnl_def_t enet_sys_chnl[DIGI_ENET_SYS_CHNL_MAX];
    digi_cbr_client_chnl_def_t  cbr_chnl[DIGI_CBRC_CHNL_MAX];
    digi_mapper_chnl_def_t      mapper_chnl[DIGI_MAPOTN_CHNL_MAX];
    digi_otn_server_chnl_def_t  otn_server_chnl[DIGI_OTN_SERVER_CHNL_MAX];
    digi_otn_server_chnl_def_t  sysotn_server_chnl; 
    odu_struct_t                ho_odu_struct[DIGI_OTN_SERVER_HO_CHNL_MAX];
    odu_struct_t                mo_odu_struct[DIGI_OTN_SERVER_MO_CHNL_MAX];
    odu_struct_t                lo_odu_struct[DIGI_OTN_SERVER_LO_CHNL_MAX];
    odu_struct_t                s3b_odu_struct[DIGI_OTN_SERVER_LO_CHNL_MAX];
    odu_struct_t                s4_odu_struct[DIGI_OTN_SERVER_LO_CHNL_MAX];
    
    odu_struct_t                sysotn_line_struct[DIGI_OTN_SERVER_HO_CHNL_MAX]; 
    odu_chnl_t                  ho_odu_chnl[DIGI_OTN_SERVER_HO_CHNL_MAX];
    odu_chnl_t                  mo_odu_chnl[DIGI_OTN_SERVER_MO_CHNL_MAX];
    odu_chnl_t                  lo_odu_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    odu_chnl_t                  s3b_odu_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    odu_chnl_t                  dummy_odu_chnl_stg4; 
    odu_chnl_t                  dummy_odu_chnl_stg2_sysotn; 
} digi_handle_pool_t;

/*******************************************************************************
* STRUCTURE: digi_ge_serdes_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining GE serdes information.
*
* ELEMENTS:
*   ge_port_ctxt                - pointer to GE serdes portd context
*   d8_tx_2x_handle             - D8 TX 2 slice block handles
*   d8_rx_2x_handle             - D8 RX 2 slice block handles
*   csu_enabled                 - state of the CSU shared by all serdes
*******************************************************************************/
typedef struct digi_ge_serdes_handle_t
{
    digi_ge_serdes_ctxt_t *ge_port_ctxt[DIGI_GE_SERDES_D8_2X_MAX*2];
    d8_tx_2x_handle_t *d8_tx_2x_handle[DIGI_GE_SERDES_D8_2X_MAX];
    d8_rx_2x_handle_t *d8_rx_2x_handle[DIGI_GE_SERDES_D8_2X_MAX];    
    BOOL8 csu_enabled;
} digi_ge_serdes_handle_t;

/*******************************************************************************
* STRUCTURE: digi_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DIGI handle information.
*
* ELEMENTS:
*   base                        - System handle reference \n
*   cfg                         - Structure defining static configuration data \n
*   var                         - Structure defining dynamic configuration data \n
*   handle_pool                 - Structure defining channel handle resources \n
*   digi_sw_status              - SW status data \n
*   digi_fw_status              - FW status data \n
*   *dcpb_handle                - CPB SS submodule reference \n
*   *mcpb_handle                - MCPB SS submodule reference \n
*   *enet_sys_handle            - ENET_SYS submodule reference \n
*   *enet_line_handle           - ENET_LINE submodule reference \n
*   *sifd_handle                - SIFD submodule reference \n
*   *lifd_handle                - LIFD submodule reference \n
*   *cbrc_handle                - CBRC submodule reference \n
*   *mapotn_handle              - MAPOTN submodule reference \n
*   *coreotn_handle             - COREOTN submodule reference \n
*   *lineotn_handle             - LINEOTN submodule reference \n
*   *sysotn_handle              - SYSOTN submodule reference \n
*   *s16_line_handle            - S16 LINE SERDES submodule reference \n
*   *s16_sys_handle             - S16 SYSTEM SERDES submodule reference 
*   *sfi51_d8_tx_18x_handle     - SFI51 D8 SERDES TX 18 SLICES submodule reference \n
*   *sfi51_d8_rx_18x_handle     - SFI51 D8 SERDES RX 18 SLICES submodule reference \n
*   *bp_lr_t8_handle            - T8 SERDES submodule reference \n
*   *bp_sr_c8_handle            - C8 SERDES submodule reference \n
*   *ge_intf_handle             - GE SERDES submodule reference \n
*   *mgmt_fege_handle           - MGMT_FEGE SERDES submodule reference \n
*
*******************************************************************************/
struct digi_handle_struct_t { 
    pmc_handle_t base;

    digi_cfg_t cfg;
    digi_var_t var;
    digi_handle_pool_t handle_pool;
    digi_sw_status_t digi_sw_status;
    digi_fw_status_t digi_fw_status;

    cpb_handle_t                     *dcpb_handle;
    cpb_handle_t                     *mcpb_handle;
    enet_handle_t                    *enet_sys_handle;
    enet_handle_t                    *enet_line_handle;
    sifd_handle_t                    *sifd_handle;
    lifd_handle_t                    *lifd_handle;
    cbrc_handle_t                    *cbrc_handle;
    mapotn_handle_t                  *mapotn_handle;
    coreotn_handle_t                 *coreotn_handle;
    lineotn_handle_t                 *lineotn_handle;
    lineotn_handle_t                 *sysotn_handle; 
    s16_handle_t                     *s16_line_handle;
    s16_handle_t                     *s16_sys_handle;
    sfi51_d8_tx_18x_handle_t        **sfi51_d8_tx_18x_handle;
    sfi51_d8_rx_18x_handle_t        **sfi51_d8_rx_18x_handle;
    t8_handle_t                     **bp_lr_t8_handle;
    c8_handle_t                      *bp_sr_c8_handle; 
    digi_ge_serdes_handle_t          *ge_intf_handle;
    mgmt_fege_handle_t               *mgmt_fege_handle;
    
};

/*
** Global variables
*/
extern BOOL8 digi_handle_init_uses_fw_q_flag;
/*
** Public functions
*/
/* Firmware related functions */
PUBLIC PMC_ERROR digi_fw_reload_internal(digi_handle_t *digi_handle,BOOL load_fw_context);
PUBLIC PMC_ERROR digi_fw_shutdown_and_put_mips_in_reset(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_download_and_out_of_reset(digi_handle_t *digi_handle,BOOL load_fw_context);
PUBLIC PMC_ERROR digi_fw_download_from_hard_reset(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_stats_init(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_poll_for_alive(digi_handle_t *digi_handle);
PUBLIC BOOL digi_fw_is_in_reset(digi_handle_t *digi_handle);
PUBLIC BOOL digi_fw_is_running(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_version_get(digi_handle_t *digi_handle,
                                     hostmsg_info_req_ver_t info_req,
                                     UINT32 *fw_revision_number_ptr,
                                     UINT32 *fw_build_number_ptr,
                                     UINT32 *bootloader_revision_number_ptr,
                                     UINT32 *bootloader_build_number_ptr);
PUBLIC BOOL digi_fw_is_ver_3_05(digi_handle_t *digi_handle);

/* SERDES Configuration */
PUBLIC PMC_ERROR digi_serdes_c8_cfg(digi_handle_t *digi_handle, UINT8 enable_msk,
                                    digi_serdes_port_mode_t serdes_port_mode);
PUBLIC PMC_ERROR digi_serdes_t8_cfg(digi_handle_t *digi_handle, UINT32 lane_0_19_enable_msk,
                                    UINT32 lane_20_39_enable_msk,
                                    digi_serdes_port_mode_t lane_0_19_port_mode, 
                                    digi_serdes_port_mode_t lane_20_39_port_mode);
PUBLIC PMC_ERROR digi_serdes_s16_cfg(digi_handle_t *digi_handle, 
                                     hostmsg_s16_intf_type_t s16_intf_type, 
                                     UINT16 enable_msk, 
                                     digi_serdes_port_mode_t serdes_rate, 
                                     UINT16 disable_msk);
PUBLIC PMC_ERROR digi_serdes_sfi51_cfg(digi_handle_t *digi_handle, 
                                       UINT8 enable_msk, digi_serdes_port_mode_t serdes_port_mode, 
                                       UINT8 disable_msk);
PUBLIC PMC_ERROR digi_serdes_d8_cfg(digi_handle_t *digi_handle, 
                                     hostmsg_d8_intf_type_enum d8_intf_type,
                                     BOOL8 cfg_csu, 
                                     BOOL8 enable);
PUBLIC PMC_ERROR digi_serdes_loopback_c8_cfg(digi_handle_t *digi_handle, 
                                             UINT8 enable_msk,
                                             digi_serdes_port_mode_t serdes_port_mode,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_serdes_loopback_c8_status_get(digi_handle_t *digi_handle, 
                                             UINT8 enable_msk,
                                             digi_serdes_port_mode_t serdes_port_mode,
                                             BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR digi_serdes_loopback_t8_cfg(digi_handle_t *digi_handle, 
                                             UINT32 lane_0_19_enable_msk,
                                             UINT32 lane_20_39_enable_msk,
                                             digi_serdes_port_mode_t lane_0_19_port_mode, 
                                             digi_serdes_port_mode_t lane_20_39_port_mode,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_serdes_loopback_t8_status_get(digi_handle_t *digi_handle, 
                                             UINT32 lane_0_19_enable_msk,
                                             UINT32 lane_20_39_enable_msk,
                                             digi_serdes_port_mode_t lane_0_19_port_mode, 
                                             digi_serdes_port_mode_t lane_20_39_port_mode,
                                             BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR digi_serdes_loopback_s16_cfg(digi_handle_t *digi_handle, 
                                              hostmsg_s16_intf_type_t s16_intf_type, 
                                              UINT16 enable_msk, 
                                              digi_serdes_port_mode_t serdes_rate, 
                                              BOOL8 enable);                                             
PUBLIC PMC_ERROR digi_serdes_loopback_s16_status_get(digi_handle_t *digi_handle, 
                                              hostmsg_s16_intf_type_t s16_intf_type, 
                                              UINT16 enable_msk,
                                              digi_serdes_port_mode_t serdes_rate, 
                                              BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR digi_serdes_polarity_c8_cfg(digi_handle_t                *digi_handle, 
                                             UINT8                         pin_id, 
                                             util_global_direction_t       dir,
                                             BOOL8                         enable);

PUBLIC PMC_ERROR digi_serdes_polarity_t8_cfg(digi_handle_t                *digi_handle, 
                                             UINT8                         pin_id, 
                                             util_global_direction_t       dir,
                                             BOOL8                         enable);

PUBLIC PMC_ERROR digi_serdes_polarity_bp_s16_cfg(digi_handle_t           *digi_handle, 
                                                 UINT8                    pin_id, 
                                                 util_global_direction_t  dir,
                                                 BOOL8                    enable);    

PUBLIC PMC_ERROR digi_post_mld_prov_state_set(digi_handle_t *digi_handle,
                                              UINT32 port_uid,
                                              digi_serdes_prov_state_t prov_state);

PUBLIC PMC_ERROR digi_by_port_uid_post_mld_prov_state_get(digi_handle_t *digi_handle,
                                                          UINT32 port_uid,
                                                          digi_serdes_prov_state_t* prov_state);

PUBLIC void digi_ilkn_serdes_pin_use_get(digi_handle_t *digi_handle,
                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                         digi_ilkn_serdes_rate_t *ilkn_1_rate_ptr,
                                         digi_ilkn_serdes_rate_t *ilkn_2_rate_ptr,
                                         UINT32 *ilkn1_e1_39_20_msk_ptr,
                                         UINT32 *ilkn1_e1_19_00_msk_ptr,
                                         UINT32 *ilkn1_n1_msk_ptr,
                                         UINT32 *ilkn1_n2_msk_ptr,
                                         UINT32 *ilkn2_e1_39_20_msk_ptr,
                                         UINT32 *ilkn2_e1_19_00_msk_ptr,
                                         UINT32 *ilkn2_n1_msk_ptr,
                                         UINT32 *ilkn2_n2_msk_ptr);

PUBLIC PMC_ERROR digi_serdes_port_rate_state_set(digi_handle_t *digi_handle,
                                                 digi_serdes_port_t *port_ctxt_ptr,
                                                 digi_serdes_port_mode_t port_mode,
                                                 digi_serdes_prov_state_t prov_state,
                                                 digi_serdes_port_rate_prov_state_t rate_prov_state);

PUBLIC PMC_ERROR digi_serdes_s16_rate_convert(digi_serdes_port_mode_t serdes_rate, 
                                              hostmsg_s16_serdes_rate_t *hostmsg_s16_serdes_rate_ptr,
                                              DOUBLE *s16_serdes_rate,
                                              BOOL8 *s16_otu_client,
                                              s16_port_mode_t           *s16_serdes_port_mode_ptr);
PUBLIC PMC_ERROR digi_serdes_t8_rate_convert(digi_serdes_port_mode_t    serdes_rate, 
                                             hostmsg_t8_serdes_rate_t   *hostmsg_t8_serdes_rate_ptr);

PUBLIC PMC_ERROR digi_serdes_c8_rate_convert(digi_serdes_port_mode_t     serdes_rate, 
                                              hostmsg_c8_serdes_rate_t   *hostmsg_c8_serdes_rate_ptr);

PUBLIC PMC_ERROR digi_serdes_sfi51_d8_rate_convert(digi_serdes_port_mode_t       serdes_rate, 
                                                   hostmsg_sfi51_serdes_rate_t  *hostmsg_sfi51_d8_serdes_rate_ptr);

PUBLIC digi_serdes_port_rate_prov_state_t digi_serdes_port_prov_status_get(digi_handle_t *digi_handle,
                                                                           UINT32 port_uid);

PUBLIC void digi_serdes_port_prov_state_get(digi_handle_t *digi_handle,
                                            digi_serdes_intf_t intf,
                                            UINT8 port_num,
                                            digi_serdes_prov_state_t *serdes_port_prov_state);

PUBLIC UINT32 digi_serdes_line_and_sys_port_lane_index_for_module_lane_get(digi_handle_t *digi_handle,
                                                                           digi_serdes_intf_t intf,
                                                                           UINT8 module_lane);

PUBLIC digi_sfi51_bus_ctxt_t* digi_sfi51_bus_ctxt_find(digi_handle_t *digi_handle,
                                                       UINT32 port_uid);

PUBLIC PMC_ERROR digi_ge_serdes_ctxt_get(digi_handle_t *digi_handle,
                                         digi_ge_serdes_id_t ge_intf,
                                         digi_ge_serdes_ctxt_t **ge_serdes_ctxt);

PUBLIC PMC_ERROR digi_ge_d8_rx_2x_handle_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             d8_rx_2x_handle_t**d8_rx_2x_handle,
                                             UINT8 *serdes_id);

PUBLIC PMC_ERROR digi_ge_d8_tx_2x_handle_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             d8_tx_2x_handle_t**d8_tx_2x_handle,
                                             UINT8 *serdes_id);

PUBLIC PMC_ERROR digi_bp_intf_serdes_cfg(digi_handle_t *digi_handle,
                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                         digi_bp_intf_serdes_operation_t bp_intf_serdes_operation,
                                         BOOL8 enable);

PUBLIC PMC_ERROR digi_lineotn_loopback_cfg          ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_dir_t             loopback_dir,
                                                      lineotn_loopback_state_t  loopback_state );

PUBLIC PMC_ERROR digi_bp_intf_serdes_loopback_status_get(digi_handle_t *digi_handle,
                                         digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                         digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                         BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR digi_port_ctxt_from_channel_number_get(digi_handle_t *digi_handle,
                                                         digi_serdes_prov_state_t prov_state,
                                                         UINT32 chnl,
                                                         digi_serdes_port_t **port_ctxt_pptr);
PUBLIC PMC_ERROR digi_loopback_serdes_port_acb_only_cfg(digi_handle_t *digi_handle,
                                                        digi_serdes_port_t *port_ctxt_ptr, 
                                                        BOOL8 enable);

PUBLIC PMC_ERROR digi_serdes_ilkn_enum_conversion(digi_handle_t *digi_handle, 
                                                  digi_ilkn_serdes_rate_t ilkn_rate, 
                                                  digi_serdes_port_mode_t *serdes_e1_port_mode_ptr,
                                                  digi_serdes_port_mode_t *serdes_n1_port_mode_ptr,
                                                  digi_serdes_port_mode_t *serdes_n2_port_mode_ptr);
PUBLIC BOOL digi_sys_serdes_pin_use_collision_test(digi_handle_t *digi_handle, 
                                                   digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                   digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);

PUBLIC PMC_ERROR digi_bp_serdes_mask_get(digi_handle_t               *digi_handle,
                                         digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                         digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                         UINT32                      *e1_19_00_msk,
                                         UINT32                      *e1_39_20_msk,
                                         UINT32                      *n1_msk,
                                         UINT32                      *n2_msk);

PUBLIC UINT8 digi_serdes_map_t8_serdes(UINT8 pin_id);

PUBLIC void digi_pgmrclk_div_all_disable(digi_handle_t *digi_handle); 
 
PUBLIC PMC_ERROR digi_dsi_lane_allocation(digi_handle_t *digi_handle, BOOL8 line_interface_only);

PUBLIC PMC_ERROR digi_otn_cfg_mode_set(digi_handle_t *digi_handle);

PUBLIC void digi_serdes_all_shutdown(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_serdes_ge_all_ports_init(digi_handle_t *digi_handle);
PUBLIC void digi_enet_serdes_mode_get(digi_enet_mode_t enet_mode,
                                       digi_serdes_port_mode_t* serdes_mode);
PUBLIC UINT32 digi_cbr_channel_num_get(digi_handle_t            *digi_handle,
                                       digi_cbr_client_chnl_t   *chnl_handle);
PUBLIC UINT32 digi_lineotn_channel_num_get(digi_handle_t *digi_handle,
                                           digi_otn_server_chnl_t *chnl_handle);

PUBLIC PMC_ERROR digi_mapper_channel_num_get(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t *chnl_ctxt_pptr, 
                                             UINT32 *mapotn_chnl_ptr, 
                                             UINT32 *fo2_chnl_ptr, 
                                             UINT32 *enet_sys_link_ptr, 
                                             UINT32 *enet_line_link_ptr);

PUBLIC void digi_mapper_src_dest_get(digi_handle_t *digi_handle,
                                     digi_mapping_mode_t mode,
                                     BOOL enet_pmon,
                                     mapotn_src_dest_t *mapper_src,
                                     mapotn_src_dest_t *mapper_dest);

PUBLIC PMC_ERROR digi_mapper_db_deprov_cfg(digi_handle_t *digi_handle,
                                           util_global_cpb_port_t cpb_port_type,
                                           UINT32 mapper_chnl,
                                           UINT32 enet_link,
                                           BOOL8 is_umbilical_case);

PUBLIC digi_sifd_ilkn_inst_t digi_sifd_inst_get(digi_handle_t      *digi_handle,
                                                digi_ilkn_link_t   *inst_handle);
PUBLIC void digi_cbrc_serdes_mode_get(digi_cbr_client_mode_t cbrc_mode,
                                       digi_serdes_intf_t lane_type,
                                       digi_serdes_port_mode_t* serdes_mode);
PUBLIC void digi_otn_serdes_mode_get(digi_otn_server_otuk_t otuk_mode,
                                      digi_serdes_intf_t lane_type,
                                      digi_serdes_port_mode_t* serdes_mode);

PUBLIC void digi_mapotn_serdes_mode_get(digi_mapping_mode_t map_mode,
                                         digi_serdes_intf_t lane_type,
                                         digi_serdes_port_mode_t* serdes_mode);
PUBLIC UINT32 digi_serdes_num_10g_data_lane_get(util_global_odukp_type_t odu_type);
PUBLIC PMC_ERROR digi_serdes_port_prov(digi_handle_t *digi_handle,
                                     digi_serdes_port_t *port_ctxt_ptr,
                                     digi_serdes_port_mode_t mode);

PUBLIC PMC_ERROR digi_serdes_port_deprov(digi_handle_t *digi_handle,
                                       digi_serdes_port_t *port_ctxt_ptr);
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_prov(digi_handle_t *digi_handle,
                                                   digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_deprov(digi_handle_t *digi_handle,
                                                    digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_port_internal_prov_get(digi_handle_t *digi_handle,
                                                       digi_ge_serdes_id_t ge_intf,
                                                       BOOL8 *enable);
PUBLIC digi_serdes_port_def_t* digi_port_ctxt_find(digi_handle_t *digi_handle,
                                                    UINT32 port_uid,
                                                    digi_serdes_intf_t intf);
PUBLIC digi_serdes_port_t* digi_port_ctxt_no_type_find(digi_handle_t *digi_handle,
                                                        UINT32 port_uid);


PUBLIC digi_serdes_port_def_t* digi_port_ctxt_and_intf_find(digi_handle_t *digi_handle,
                                                             UINT32 port_uid,
                                                             digi_serdes_intf_t* intf);

PUBLIC void digi_port_ctxt_all_reset(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_serdes_enet_line_port_validate(digi_serdes_port_def_t *port_def_ptr);

PUBLIC PMC_ERROR digi_serdes_enet_sys_port_validate(digi_serdes_port_def_t *port_def_ptr);

PUBLIC PMC_ERROR digi_serdes_lifd_sfi51_configure(digi_handle_t *digi_handle,
                                                   digi_serdes_port_def_t *port_def_ptr,
                                                   xifd_dsis_type_t subsystem_type);
PUBLIC PMC_ERROR digi_serdes_lifd_xfi_configure(digi_handle_t *digi_handle,
                                                 digi_serdes_port_def_t *port_def_ptr,
                                                 xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR digi_serdes_lifd_xfi_align_tx_rx_configure(digi_handle_t *digi_handle,
                                                            digi_serdes_port_def_t *port_def_ptr,
                                                            xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR digi_serdes_port_lifd_mux_align_rx_tx_cfg(digi_handle_t *digi_handle,
                                                           digi_serdes_port_def_t *port_def_ptr,
                                                           xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR digi_serdes_sifd_configure(digi_handle_t *digi_handle,
                                             digi_serdes_port_def_t *port_def_ptr,
                                             xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR digi_serdes_lifd_xfi_pin_los_cfg(digi_handle_t           *digi_handle,
                                                  digi_serdes_port_def_t  *port_def_ptr,
                                                  BOOL8                    enable);

PUBLIC PMC_ERROR digi_serdes_sifd_pin_los_cfg(digi_handle_t            *digi_handle,
                                              digi_serdes_port_def_t   *port_def_ptr,
                                              BOOL8                     enable);


PUBLIC PMC_ERROR digi_serdes_lifd_sfi51_pin_los_cfg(digi_handle_t *digi_handle,
                                                    digi_serdes_port_def_t *port_def_ptr,
                                                    BOOL8 enable);

PUBLIC BOOL digi_sys_serdes_bp_csu_multi_rate_test(digi_handle_t *digi_handle,
                                                    digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                                    digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);             
PUBLIC PMC_ERROR digi_non_otn_acb_ckctl_cfg(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *input_pc_ptr, 
                                            util_global_switch_data_t *output_pc_ptr,
                                            digi_serdes_cfg_action_t serdes_cfg_action,
                                            digi_enet_tx_timing_mode_t force_enet_timing_mode);

PUBLIC PMC_ERROR digi_serdes_non_otn_acb_cfg(digi_handle_t *digi_handle,
                                             UINT32 port,
                                             UINT32 master_port,
                                             digi_serdes_intf_t intf,
                                             util_global_switch_data_t *input_pc_ptr, 
                                             util_global_switch_data_t *output_pc_ptr,
                                             digi_serdes_acb_mode_t acb_timing_mode,
                                             UINT32 num_10g_data_lane,
                                             digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr); 

PUBLIC PMC_ERROR digi_serdes_otn_acb_cfg(digi_handle_t *digi_handle,
                                         UINT32 port,
                                         UINT32 master_port,
                                         digi_serdes_intf_t intf,
                                         digi_serdes_port_mode_t serdes_mode,
                                         coreotn_mpmo_inst_t mpmo_instance,
                                         UINT32 chnl,
                                         UINT32 num_10g_data_lane,
                                         acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                         BOOL8  prov);  

PUBLIC PMC_ERROR digi_serdes_otn_acb_reprov_pkt_size_configs(digi_handle_t *digi_handle,
                                                             UINT32 port,
                                                             UINT32 master_port,
                                                             digi_serdes_intf_t intf,
                                                             digi_serdes_port_mode_t serdes_mode,
                                                             coreotn_mpmo_inst_t mpmo_instance,
                                                             UINT32 chnl,
                                                             UINT32 num_10g_data_lane,
                                                             acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr);

PUBLIC PMC_ERROR digi_serdes_non_otn_acb_reprov_pkt_size_configs(digi_handle_t *digi_handle,
                                                                 UINT32 port,
                                                                 UINT32 master_port,
                                                                 digi_serdes_intf_t intf,
                                                                 util_global_switch_data_t *input_pc_ptr, 
                                                                 util_global_switch_data_t *output_pc_ptr,
                                                                 digi_serdes_acb_mode_t acb_timing_mode,
                                                                 UINT32 num_10g_data_lane,
                                                                 digi_enet_tx_timing_mode_t force_enet_timing_mode,
                                                                 acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr);

PUBLIC void digi_serdes_s16_clk_patt_gen_cfg_get(digi_handle_t *digi_handle, 
                                                 BOOL8 enable, 
                                                 digi_s16_clk_divider_t divider,
                                                 util_serdes_patt_gen_cfg_t *patt_cfg_ptr);

PUBLIC PMC_ERROR digi_s16_clk_acb_cfg(digi_handle_t *digi_handle, 
                                      digi_serdes_intf_t intf, 
                                      UINT32 port_uid, 
                                      UINT32 master_lane, 
                                      digi_serdes_cfg_action_t serdes_cfg_action,
                                      acb_ft_prov_cfg_t *acb_ft_prov_cfg_ptr,
                                      BOOL8              cfg_clk_as_master);

PUBLIC PMC_ERROR digi_serdes_port_lifd_sifd_subsytem_disable(digi_handle_t *digi_handle,
                                                              UINT32 port_uid);
PUBLIC PMC_ERROR digi_ocpb_pmon_internal_cfg(digi_handle_t *digi_handle,
                                             cpb_ocpb_dpi_port_type_t port,
                                             UINT32 counter_num,
                                             cpb_pmon_mode pmon_mode,
                                             BOOL count_drop_packets,
                                             BOOL count_err_packets,
                                             BOOL count_oversize_packets,
                                             BOOL count_good_packets);
PUBLIC PMC_ERROR digi_cpb_pmon_chnl_add_drop_internal_cfg(digi_handle_t *digi_handle,
                                                          util_global_switch_data_t * input_pc_ptr,
                                                          UINT32 counter_num,
                                                          BOOL is_dcpb,
                                                          BOOL add_channel);
PUBLIC PMC_ERROR digi_pmon_param_check(digi_handle_t    *digi_handle,
                                       void             *pmon_table_ptr);

PUBLIC PMC_ERROR digi_pmon_chnl_param_check(digi_handle_t    *digi_handle,
                                            void             *chnl_handle,
                                            void             *pmon_table_ptr);

PUBLIC PMC_ERROR digi_int_param_check(digi_handle_t    *digi_handle,
                                       void             *chnl_handle,
                                       digi_int_type_t   int_type,
                                       void             *int_table_ptr);

PUBLIC BOOL digi_int_is_cb_registered(digi_handle_t    *digi_handle,
                                      digi_int_type_t   int_type);

PUBLIC PMC_ERROR digi_int_callback_set(digi_handle_t        *digi_handle,
                                       digi_int_type_t       int_type,
                                       void                 *token,
                                       UINT32                token_size,
                                       digi_int_cb_fcn       user_cb);

PUBLIC PMC_ERROR digi_int_info_get(digi_handle_t    *digi_handle,
                                   digi_int_type_t   int_type,
                                   digi_int_info_t **int_info_pptr);

PUBLIC PMC_ERROR digi_int_link_range_get(digi_handle_t      *digi_handle,
                                         void               *chnl_handle,
                                         digi_int_type_t     int_type,
                                         UINT32             *start_link,
                                         UINT32             *end_link);

PUBLIC PMC_ERROR digi_int_en_table_get(digi_handle_t        *digi_handle,
                                       digi_int_type_t       int_type,
                                       void                **int_en_table_pptr);

PUBLIC PMC_ERROR digi_int_callback_run(digi_handle_t    *digi_handle,
                                       void*             chnl_handle,
                                       digi_int_type_t   int_type);

PUBLIC PMC_ERROR digi_cbr_int_enabled_check_internal(digi_handle_t          *digi_handle);
PUBLIC PMC_ERROR digi_enet_line_int_enabled_check_internal(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_enet_sys_int_enabled_check_internal(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_mapper_int_enabled_check_internal(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_otn_int_enabled_check_internal(digi_handle_t  *digi_handle);
PUBLIC PMC_ERROR digi_pmon_int_enabled_check_internal(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_sifd_int_enabled_check_internal(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_otn_odu_blk_filter_set(digi_handle_t       *digi_handle,
                                             coreotn_int_chnl_t  *filt_table_ptr,
                                             UINT32              *blk_filter_ptr);

PUBLIC PMC_ERROR digi_pmon_shared_mem_get(digi_handle_t    *digi_handle,
                                          digi_pmon_t     **pmon_pptr);

PUBLIC PMC_ERROR digi_otn_odu_pmon_chnl_retrieve_internal(digi_handle_t                  *digi_handle,
                                                          digi_otn_odu_chnl_t            *chnl_handle,
                                                          digi_otn_odu_pmon_chnl_data_t  *pmon_table_ptr);


PUBLIC BOOL8 digi_serdes_s16_rate_enet_mapping_get(digi_serdes_port_mode_t    serdes_rate);


PUBLIC void digi_enet_post_mld_dsi_get(digi_handle_t *digi_handle, 
                                        UINT32 link, 
                                        digi_group_size_t group_size,
                                        UINT16 *dsi_mask_ptr);
PUBLIC digi_post_mld_dsi_ctxt_t* digi_first_post_mld_dsi_find(digi_handle_t *digi_handle,
                                                               UINT32 port_uid);

PUBLIC PMC_ERROR digi_by_mld_index_post_mld_map_type_get(digi_handle_t *digi_handle,
                                                         UINT8 post_mld_index,
                                                         digi_mapper_alloc_t *map_type);

PUBLIC PMC_ERROR digi_by_mld_index_post_mld_map_type_set(digi_handle_t *digi_handle,
                                                         UINT8 post_mld_index,
                                                         util_global_cpb_port_t port_type,
                                                         digi_mapper_alloc_t map_type);

PUBLIC PMC_ERROR digi_serdes_port_lifd_sifd_mux_cfg(digi_handle_t *digi_handle,
                                                     digi_serdes_port_def_t *port_def_ptr,
                                                     xifd_dsis_type_t subsystem_type);

PUBLIC PMC_ERROR digi_serdes_port_lifd_sifd_mux_pin_los_cfg(digi_handle_t            *digi_handle,
                                                            digi_serdes_port_def_t   *port_def_ptr,
                                                            BOOL8                    enable) ;

PUBLIC void digi_chnl_ptr_initialize(util_global_switch_data_def_t* switch_data_ptr);

PUBLIC PMC_ERROR digi_cpb_pop_port_get(digi_handle_t *digi_handle,
                                        util_global_cpb_port_t output_port,
                                        UINT32 data_dest,
                                        UINT32 *egress_mux_input_port,
                                        UINT32 *num_pop_ports);                                       
PUBLIC void digi_post_mld_dsi_schd_entry_get(digi_handle_t *digi_handle,
                                              UINT16 dsi_mask,
                                              BOOL *schd_addr_alloc);

PUBLIC UINT8 digi_num_xfi_pins_in_use(digi_handle_t *digi_handle,
                                       UINT32 port_uid);

PUBLIC UINT8 digi_num_sys_pins_in_use(digi_handle_t *digi_handle,
                                       UINT32 port_uid);

PUBLIC PMC_ERROR digi_cpb_internal_chnl_map(digi_handle_t *digi_handle,
                                             util_global_switch_data_t*input_pc_ptr,
                                             util_global_switch_data_t*output_pc_ptr,
                                             digi_cpb_stream_t rate_data,
                                             UINT32 client_num_odu0,
                                             UINT32 halt_buffer);

PUBLIC PMC_ERROR digi_cpb_internal_chnl_demap(digi_handle_t* digi_handle,
                                              cpb_handle_t* cpb_handle,
                                              UINT32 cpb_output_port,
                                              util_global_switch_data_t* input_pc_ptr,
                                              util_global_switch_data_t* output_pc_ptr);

PUBLIC PMC_ERROR digi_cpb_internal_chnl_activate(digi_handle_t* digi_handle,
                                                 util_global_switch_data_t* output_pc_ptr);

PUBLIC PMC_ERROR digi_cpb_internal_chnl_deactivate(digi_handle_t* digi_handle,
                                                   util_global_switch_data_t* output_pc_ptr);

PUBLIC util_global_switch_data_t* digi_cpb_input_port_get(digi_handle_t* digi_handle,
                                                           util_global_switch_data_t *output_pc_ptr);

PUBLIC util_global_switch_data_t* digi_oduksw_input_port_get(digi_handle_t* digi_handle,
                                                             UINT32 ingress_mux_input_port,
                                                             UINT32 ingress_mux_input_port_chnl,
                                                             digi_ilkn_chnl_target_t target);

PUBLIC PMC_ERROR digi_oduk_rate_param_convert( digi_oduk_t oduk_type,
                                                util_global_odukp_type_t *odu_type );  


PUBLIC PMC_ERROR digi_oduk_rate_num_cal_slot_get( util_global_odukp_type_t client_oduk_type,
                                                   util_global_odukp_type_t server_oduk_type,
                                                   UINT32 *num_cal_slots );

PUBLIC PMC_ERROR digi_serdes_line_cage_cfg(digi_handle_t *digi_handle,
                                            UINT8 cage_id_per_pin[DIGI_SERDES_XFI_LANES_MAX]);
PUBLIC PMC_ERROR digi_serdes_sys_cage_cfg(digi_handle_t *digi_handle,
                                           UINT8 cage_id_per_pin[DIGI_SERDES_SYS_LANES_MAX]);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_map(digi_handle_t* digi_handle,
                                             util_global_switch_data_t* input_pc_ptr, 
                                             util_global_switch_data_t* output_pc_ptr,
                                             digi_cpb_stream_t rate_data,
                                             UINT32 client_num_odu0,
                                             UINT32 halt_buffer,
                                             digi_enet_enhanced_pmon_req_t pmon_req_type,
                                             digi_mapper_chnl_def_t* mapper_chnl_ptr,
                                             digi_ilkn_chnl_def_t* ilkn_chnl_ptr,
                                             digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                             enet_datapath_t enet_mode, 
                                             digi_enet_mode_t digi_enet_mode);

PUBLIC PMC_ERROR digi_enet_is_enhanced_pmon_required(digi_handle_t* digi_handle,
                                                     util_global_switch_data_t* input_pc_ptr, 
                                                     util_global_switch_data_t* output_pc_ptr,
                                                     digi_mapper_chnl_def_t** mapper_chnl_ptr,
                                                     digi_ilkn_chnl_def_t** ilkn_chnl_ptr,
                                                     digi_enet_client_chnl_def_t** enet_chnl_ptr,
                                                     enet_datapath_t* enet_mode_ptr, 
                                                     digi_enet_mode_t* digi_enet_mode_ptr,
                                                     digi_enet_enhanced_pmon_req_t* pmon_req_type);

PUBLIC PMC_ERROR digi_enet_is_link_in_use(digi_handle_t* digi_handle,
                                          UINT32 enet_link, 
                                          digi_group_size_t group_size,
                                          util_global_cpb_port_t port_type);

PUBLIC BOOL8 digi_enet_is_link_mac_block(enet_handle_t *enet_handle,
                                         UINT32 link);

PUBLIC void digi_mapotn_mode_params_convert(digi_mapping_mode_t mode,
                                            util_global_map_adapt_func_t* map_mode,
                                            util_global_map_adapt_func_t* demap_mode);
                                             
PUBLIC void digi_enet_pmon_prov_params_convert(enet_datapath_t mode, 
                                               digi_group_size_t* group_size);             

PUBLIC PMC_ERROR digi_enet_line_prov_params_additional_validate(digi_handle_t *digi_handle,
                                                                digi_enet_mode_t mode);

PUBLIC PMC_ERROR digi_enet_line_prov_params_validate(digi_handle_t *h,
                                                      digi_serdes_port_def_t *port_def_ptr,
                                                      digi_enet_mode_t mode,
                                                      digi_enet_client_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_enet_sys_prov_params_validate(digi_handle_t *h,
                                                     digi_serdes_port_def_t *port_def_ptr,
                                                     digi_enet_mode_t mode,
                                                     digi_enet_client_chnl_t **chnl_ctxt_pptr);

PUBLIC void digi_enet_line_prov_params_convert(digi_enet_mode_t mode,
                                                enet_datapath_t* e_mode, 
                                                digi_group_size_t* group_size);

PUBLIC void digi_enet_sys_prov_params_convert(digi_enet_mode_t mode,
                                               enet_datapath_t* e_mode, 
                                               digi_group_size_t* group_size);
PUBLIC PMC_ERROR digi_enet_prov_new_mapper_state_get(digi_mapper_alloc_t in_map_type,
                                                      digi_mapper_alloc_t *out_map_type);

PUBLIC PMC_ERROR digi_enet_line_db_prov_cfg(digi_handle_t *digi_handle,  
                                             digi_group_size_t group_size,
                                             UINT32 enet_link, 
                                             BOOL mirror_port);

PUBLIC PMC_ERROR digi_enet_line_db_deprov_cfg(digi_handle_t *digi_handle,
                                              UINT32 enet_link,
                                              BOOL mirror_mode);

PUBLIC PMC_ERROR digi_enet_deprov_new_mapper_state_get(digi_mapper_alloc_t in_map_type,
                                                        digi_mapper_alloc_t *out_map_type);

PUBLIC PMC_ERROR digi_enet_sys_db_prov_cfg(digi_handle_t *digi_handle,  
                                            digi_group_size_t group_size,
                                            UINT32 enet_link);    

PUBLIC digi_post_mld_dsi_ctxt_t* digi_first_sys_post_mld_dsi_find(digi_handle_t *digi_handle,
                                                                   UINT32 port_uid);

PUBLIC PMC_ERROR digi_serdes_port_rate_prov_status_internal_get(digi_handle_t *digi_handle,
                                                                digi_serdes_port_t *port_ctxt_ptr,
                                                                BOOL8               skip_mld_check,
                                                                BOOL8 *prov_status,
                                                                digi_serdes_port_mode_t *port_mode,
                                                                digi_serdes_prov_state_t *prov_state);

PUBLIC PMC_ERROR digi_serdes_line_sys_polarity_get(digi_handle_t          *digi_handle,
                                                   digi_serdes_port_t     *port_ctxt_ptr,
                                                   UINT8                   logical_lane,
                                                   util_global_direction_t dir,
                                                   BOOL8                  *polarity_inv);

PUBLIC PMC_ERROR digi_fw_enet_client_source_cfg(digi_handle_t *digi_handle,  util_global_cpb_port_t cpb_port);
PUBLIC PMC_ERROR digi_fw_enet_client_source_get(digi_handle_t          *digi_handle, BOOL8 *is_enet_line);
PUBLIC PMC_ERROR digi_fw_enet_eclass_init(digi_handle_t *digi_handle,
                                          enet_dir_t    datapath);

PUBLIC PMC_ERROR digi_fw_eclass_hwire_cfg(digi_handle_t           *digi_handle,
                                          digi_enet_pkt_fwd_dst_t pkt_fwd_dst,
                                          enet_dir_t              datapath,
                                          UINT32                  link,
                                          BOOL8                   link_enable,
                                          BOOL8                   extract_enable,
                                          util_global_pkt_type_t  pkt_type,
                                          BOOL8                   include_ts,
                                          BOOL8                   is_vlan_tagged,
                                          UINT16                  vlan_tpid,
                                          UINT16                  vlan_tci);

PUBLIC PMC_ERROR digi_fw_enet_intr_monitor_cfg(digi_handle_t *digi_handdle,
                                               digi_intr_ipi_monitor_t int_id,
                                               BOOL8 enable);

PUBLIC PMC_ERROR digi_fw_enet_intr_monitor_status_get(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      BOOL8  clr_status,
                                                      UINT32 *fw_i_bit_status_ptr);

PUBLIC PMC_ERROR digi_fw_cbrc_intr_monitor_cfg(digi_handle_t *digi_handle,
                                               digi_intr_ipi_monitor_t int_id,
                                               BOOL8 enable);

PUBLIC PMC_ERROR digi_fw_cbrc_intr_monitor_status_get(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      UINT32 *fw_i_bit_status_ptr);

PUBLIC PMC_ERROR digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle_t *digi_handle,
                                                                  digi_intr_ipi_monitor_t int_id,
                                                                  UINT32 ch_id,
                                                                  UINT32 *fw_i_bit_status_ptr);

PUBLIC PMC_ERROR digi_fw_cbrc_intr_channelized_clear(digi_handle_t *digi_handle,
                                                      digi_intr_ipi_monitor_t int_id,
                                                      UINT32 ch_id);


PUBLIC PMC_ERROR digi_mcpb_enet_mux_mode_init(digi_handle_t                    *digi_handle,
                                              util_global_mcpb_port_t            cpb_enet_port_type,
                                              BOOL8                             enable);

PUBLIC PMC_ERROR digi_mcpb_port_init(digi_handle_t                    *digi_handle,
                                     cpb_mcpb_egr_output_port_type_t   egr_port,
                                     cpb_mcpb_ingr_input_port_type_t   ingr_port,
                                     UINT32                            rate);

PUBLIC PMC_ERROR digi_mcpb_port_uninit(digi_handle_t                    *digi_handle,
                                       cpb_mcpb_egr_output_port_type_t   egr_port,
                                       cpb_mcpb_ingr_input_port_type_t   ingr_port);

PUBLIC PMC_ERROR digi_mcpb_chnl_prov(digi_handle_t *digi_handle,
                                     cpb_mcpb_egr_output_port_type_t egr_port,
                                     UINT32 egr_port_chnl,
                                     cpb_mcpb_ingr_input_port_type_t ingr_port,
                                     UINT32 ingr_port_chnl,
                                     UINT32 rate);

PUBLIC PMC_ERROR digi_mcpb_chnl_deprov(digi_handle_t *digi_handle,
                                       cpb_mcpb_egr_output_port_type_t egr_port,
                                       UINT32 egr_port_chnl,
                                       cpb_mcpb_ingr_input_port_type_t ingr_port,
                                       UINT32 ingr_port_chnl);
                   
PUBLIC util_global_mcpb_port_t digi_mcpb_enet_port_type_get(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_ins_chnl_cfg(digi_handle_t                     *digi_handle,
                                                  UINT32                             chnl,
                                                  cpb_mcpb_egr_output_port_type_t    mcpb_egr_port,
                                                  util_global_vlan_cfg_t            *vlan_cfg_ptr,
                                                  BOOL8                              enable,
                                                  BOOL8                              ovrd_vlan_prio,
                                                  digi_cpb_stream_t                  rate_data,
                                                  BOOL8                              fcs_enable);
PUBLIC PMC_ERROR digi_enet_mgmt_fege_ins_chnl_cfg_get(digi_handle_t                  *digi_handle,
                                                      UINT32                          chnl,
                                                      cpb_mcpb_egr_output_port_type_t mcpb_egr_port,
                                                      util_global_vlan_cfg_t         *vlan_cfg_ptr,
                                                      BOOL8                          *enable,
                                                      BOOL8                          *priority_en);
 PUBLIC PMC_ERROR digi_enet_mgmt_fege_ext_chnl_cfg(digi_handle_t                     *digi_handle,
                                                   UINT32                             chnl,
                                                   cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port,
                                                   util_global_vlan_cfg_t            *vlan_cfg_ptr,
                                                   BOOL8                              enable,
                                                   BOOL8                              vlan_id_repl_en,
                                                   UINT16                             vlan_id_repl,
                                                   digi_cpb_stream_t                  rate_data);
 PUBLIC PMC_ERROR digi_enet_mgmt_fege_ext_chnl_cfg_get(digi_handle_t                  *digi_handle,
                                                      UINT32                           chnl,
                                                      cpb_mcpb_ingr_input_port_type_t  mcpb_ing_port,
                                                      BOOL8                           *vlan_id_repl_en,
                                                      UINT16                          *vlan_id_repl);
PUBLIC BOOL digi_enet_gsup43_7_3_legacy_mode_chk(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_ptr,
                                                  UINT8 *data_upi_ptr,
                                                  UINT8 *os_upi_ptr);

PUBLIC PMC_ERROR digi_dcpb_mapotn_head_hole_match(digi_handle_t *digi_handle, 
                                                  util_global_switch_data_t *output_pc_ptr);  

PUBLIC BOOL8 digi_handle_init_status_get(digi_handle_t    *digi_handle);

PUBLIC BOOL8 digi_device_init_status_get(digi_handle_t    *digi_handle);

PUBLIC PMC_ERROR digi_port_uid_from_enet_link_get(digi_handle_t *digi_handle,
                                                  UINT32 link,
                                                  BOOL8 is_line,
                                                  UINT32* port_uid);

PUBLIC PMC_ERROR digi_enet_sys_db_deprov_cfg(digi_handle_t *digi_handle,
                                             UINT32 enet_link);

PUBLIC PMC_ERROR digi_otn_odu_int_link_range_get(digi_handle_t             *digi_handle,
                                                 void                      *chnl_handle,
                                                 odu_struct_odu_level_t     odu_level,
                                                 UINT32                    *start_link,
                                                 UINT32                    *end_link);
PUBLIC PMC_ERROR digi_otn_odu_int_odu_get(digi_handle_t            *digi_handle,
                                          odu_struct_odu_level_t    odu_level,
                                          digi_otn_int_t           *int_table_ptr,
                                          digi_otn_odu_chnl_t    ***odu_handle_ppptr,
                                          digi_otn_odu_int_chnl_t **odu_pptr);
PUBLIC PMC_ERROR digi_otn_pool_id_get(digi_handle_t              *digi_handle,
                                      digi_otn_odu_chnl_t        *chnl_handle,
                                      UINT32                     *pool_id_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_en_framer_get(digi_handle_t              *digi_handle,
                                                odu_struct_odu_level_t      odu_level,
                                                coreotn_int_framer_chnl_t **framer_en_pptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_stage_enabled_get_helper(digi_handle_t              *digi_handle,
                                                                digi_otn_odu_chnl_t        *chnl_handle,
                                                                odu_struct_odu_level_t      odu_level,
                                                                coreotn_int_chnl_t         *int_table_ptr);

PUBLIC BOOL8 digi_enet_is_enhanced_pmon_umbilical_case(digi_handle_t* digi_handle,
                                                        digi_mapper_chnl_def_t* mapper_chnl,
                                                        digi_enet_pmon_type_t enet_pmon);

PUBLIC PMC_ERROR digi_otn_odu_int_en_framer_get(digi_handle_t              *digi_handle,
                                                odu_struct_odu_level_t      odu_level,
                                                coreotn_int_framer_chnl_t **framer_en_pptr);

PUBLIC UINT32 digi_otn_odu_chnl_num_internal_get(digi_handle_t              *digi_handle,
                                                 digi_otn_odu_chnl_t        *chnl_handle);
PUBLIC void digi_mem_write_internal(digi_handle_t *digi_handle, UINT32 vaddr, UINT32 len, UINT8 *data_ptr);

PUBLIC PMC_ERROR digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle_t *digi_handle,
                                                             digi_otn_odu_chnl_t *chnl_in_ptr,
                                                             digi_otn_odu_chnl_t **chnl_out_ptr);

PUBLIC void digi_enet_pmon_line_usage_set(digi_handle_t* digi_handle,
                                          UINT32 enet_link, 
                                          digi_enet_mode_t mode,
                                          BOOL in_use);

PUBLIC PMC_ERROR digi_mapotn_data_src_acb_mode_find(digi_handle_t *digi_handle,
                                                    digi_mapping_mode_t mode,
                                                    digi_serdes_acb_mode_t *acb_timing_mode); 
                                                     
PUBLIC PMC_ERROR digi_enet_data_src_acb_mode_find(digi_handle_t *digi_handle,
                                                  digi_enet_mode_t mode,
                                                  digi_serdes_acb_mode_t *acb_timing_mode); 

PUBLIC PMC_ERROR digi_cpb_push_pop_port_get(digi_handle_t *digi_handle,
                                            util_global_cpb_port_t input_port,
                                            util_global_cpb_port_t output_port,
                                            UINT32 data_source,
                                            UINT32 data_dest,
                                            UINT32 *ingress_mux_output_port,
                                            UINT32 *num_push_ports,
                                            UINT32 *egress_mux_input_port,
                                            UINT32 *num_pop_ports);
                                                 
PUBLIC void digi_num_odus_and_map_type_deprov_set(digi_handle_t *digi_handle,
                                                  util_global_cpb_port_t port_type,
                                                  UINT32 map_chnl); 

PUBLIC void digi_num_odus_and_map_type_etrans_deprov_set(digi_handle_t *digi_handle,
                                                         util_global_cpb_port_t port_type,
                                                         UINT32 enet_chnl);

PUBLIC PMC_ERROR digi_otn_acb_ckctl_cfg(digi_handle_t *digi_handle,
                                        digi_otn_server_chnl_def_t *otn_server_ptr,
                                        digi_serdes_cfg_action_t serdes_cfg_action);


PUBLIC void digi_otn_acb_ckctl_get(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *pc_ptr,
                                   digi_serdes_intf_t *intf_ptr,
                                   UINT8 *serdes_pin_ptr,
                                   UINT8 *ckctl_port_addr_ptr);

PUBLIC void digi_non_otn_acb_ckctl_get(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *pc_ptr,
                                       digi_serdes_intf_t *intf_ptr,
                                       UINT8 *serdes_pin_ptr,
                                       UINT8 *ckctl_port_addr_ptr,
                                       UINT8 *fo2_ckctl_port_addr_ptr);

PUBLIC digi_otn_server_chnl_def_t* digi_otn_server_chnl_get(digi_handle_t* digi_handle, 
                                                             UINT32 dci_chnl);

PUBLIC digi_otn_server_chnl_def_t* digi_sysotn_server_chnl_get(digi_handle_t* digi_handle);

PUBLIC UINT32 digi_lineotn_dci_chnl_get(digi_handle_t          *digi_handle,
                                        digi_otn_server_chnl_t *chnl_handle);

PUBLIC PMC_ERROR digi_serdes_sfi51_loop_timing_cfg(digi_handle_t            *digi_handle, 
                                                   digi_serdes_port_t       *port_ctxt_ptr, 
                                                   BOOL                      enable);

PUBLIC PMC_ERROR digi_serdes_one_t8_params_wr(digi_handle_t            *digi_handle, 
                                              UINT32                    lane);

PUBLIC PMC_ERROR digi_serdes_t8_params_wr(digi_handle_t            *digi_handle, 
                                          UINT32                   lane_0_19_msk, 
                                          UINT32                   lane_20_39_msk);

PUBLIC PMC_ERROR digi_serdes_s16_line_params_wr(digi_handle_t            *digi_handle, 
                                                UINT32                   lanes_msk);

PUBLIC PMC_ERROR digi_serdes_s16_sys_params_wr(digi_handle_t            *digi_handle, 
                                               UINT32                    lanes_msk);

PUBLIC PMC_ERROR digi_serdes_c8_params_wr(digi_handle_t            *digi_handle, 
                                          UINT32                   lanes_msk);

PUBLIC PMC_ERROR digi_serdes_sfi51_params_wr(digi_handle_t            *digi_handle, 
                                             UINT32                    enable_msk,
                                             UINT32                    disable_msk,
                                             digi_serdes_port_mode_t   serdes_port_mode);

PUBLIC PMC_ERROR digi_serdes_ge_params_wr(digi_handle_t               *digi_handle, 
                                          hostmsg_d8_intf_type_enum    d8_intf_type,
                                          BOOL8                        enable);

PUBLIC PMC_ERROR digi_oduksw_sifd_db_clear(digi_handle_t *digi_handle,
                                           digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                           UINT32 chnl,
                                           BOOL8 sifd_db);

PUBLIC BOOL8 digi_mapper_is_mapotn_mode_gfp_enet(digi_mapper_chnl_def_t* mapper_chnl_handle_ptr);

PUBLIC void digi_otn_mpmo_get(digi_handle_t          *digi_handle,
                              digi_serdes_intf_t      intf,
                              lifd_sig_src_type_t    *acb_if_sel_clk_source,
                              coreotn_mpmo_inst_t    *coreotn_mpmo_inst);

PUBLIC PMC_ERROR digi_otn_mpmo_ckctl_port_offset_get(digi_handle_t             *digi_handle, 
                                                     digi_serdes_intf_t         intf,
                                                     UINT32                    *ckctl_port_offset_ptr);

PUBLIC PMC_ERROR digi_cbrc_db_deprov_cfg(digi_handle_t *digi_handle,
                                         UINT32 cbrc_port); 

PUBLIC PMC_ERROR digi_cbr_info_get(digi_handle_t          *digi_handle,
                                   digi_serdes_intf_t      intf_type,
                                   UINT32                  port_uid,
                                   cbrc_serdes_type_t     *cbrc_serdes,
                                   UINT32                 *cbrc_link,
                                   UINT32                 *mux_link);

PUBLIC void digi_cbrc_prov_params_convert(digi_cbr_client_mode_t mode,
                                          digi_group_size_t* group_size);

PUBLIC PMC_ERROR digi_cbrc_db_prov_cfg(digi_handle_t *digi_handle,
                                       digi_group_size_t group_size,
                                       UINT32 cbrc_port,
                                       BOOL8 no_cpb_cfg);

PUBLIC PMC_ERROR digi_cpb_input_port_init(digi_handle_t *digi_handle,
                                          UINT32 cpb_emul_type,
                                          util_global_cpb_port_t input_port,
                                          cpb_xfer_mode_t cpb_xfer_mode,
                                          UINT32 hdr_removal_size);

PUBLIC PMC_ERROR digi_ilkn_link_handle_validate(digi_handle_t   *digi_handle,
                                                void            *handle_ptr);

PUBLIC PMC_ERROR digi_ilkn_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                void            *handle_ptr);

PUBLIC PMC_ERROR digi_enet_line_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                     void            *handle_ptr);

PUBLIC PMC_ERROR digi_enet_sys_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_enet_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_cbr_chnl_handle_validate(digi_handle_t   * digi_handle,
                                               void            *handle_ptr);

PUBLIC PMC_ERROR digi_mapper_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                  void            *handle_ptr);

PUBLIC PMC_ERROR digi_otn_server_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                      void            *handle_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                         void            *handle_ptr);

PUBLIC PMC_ERROR digi_ho_odu_struct_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_mo_odu_struct_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_lo_odu_struct_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_s3b_odu_struct_handle_validate(digi_handle_t   * digi_handle,
                                                     void            *handle_ptr);

PUBLIC PMC_ERROR digi_s4_odu_struct_handle_validate(digi_handle_t   * digi_handle,
                                                    void            *handle_ptr);

PUBLIC PMC_ERROR digi_sysotn_line_struct_handle_validate(digi_handle_t   * digi_handle,
                                                         void            *handle_ptr);

PUBLIC PMC_ERROR digi_otn_odu_chnl_handle_validate(digi_handle_t   * digi_handle,
                                                   void            *handle_ptr);

PUBLIC PMC_ERROR digi_serdes_port_ctxt_validate(digi_handle_t      *digi_handle,
                                                digi_serdes_port_t *port_ctxt_ptr);

PUBLIC PMC_ERROR digi_oduksw_chnl_validate(digi_handle_t   *digi_handle,
                                           void            *handle_ptr);

PUBLIC PMC_ERROR digi_cpb_chnl_validate(digi_handle_t   *digi_handle,
                                        void            *handle_ptr);

PUBLIC PMC_ERROR digi_cbr_validate_params(digi_handle_t             *digi_handle,
                                          digi_cbr_client_chnl_t    *chnl_handle,
                                          cbrc_handle_t            **cbrc_handle_pptr,
                                          UINT32                    *link_ptr);

PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_mode_internal_get(digi_handle_t      *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_pptr,
                                                          UINT32             *fcs_mode);

PUBLIC BOOL digi_cpb_enet_mirror_chk(digi_handle_t *digi_handle,
                                     util_global_switch_data_t *output_pc_data_ptr);

PUBLIC PMC_ERROR digi_otn_remove_nominal_replacement(digi_handle_t           *digi_handle,
                                                      odu_struct_t *ho_odu_ptr);

PUBLIC PMC_ERROR digi_cpb_update_nominal_replacement(digi_handle_t           *digi_handle,
                                                     UINT32 port_uid,
                                                     util_global_switch_data_def_t *switch_data_ptr,
                                                     digi_opsa_cfg_action_t action,
                                                     BOOL8 is_loopback);

PUBLIC  digi_otn_server_chnl_def_t* digi_otn_server_chnl_port_uid_get(digi_handle_t* digi_handle, 
                                                                       UINT32 port_uid);
PUBLIC  BOOL8 digi_disable_fw_acb_proc(digi_handle_t       * digi_handle, 
                                       digi_serdes_port_t   *port_ctxt_ptr,
                                       odu_struct_t                 **odu_ptr,
                                       util_global_switch_data_def_t **switch_data_ptr,
                                       hostmsg_opsa_subsystem_cfg_t *subsystem);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_internal_enable(digi_handle_t             *digi_handle,  
                                                         mgmt_fege_int_t           *int_table_ptr,
                                                         mgmt_fege_int_t           *int_en_table_ptr,
                                                         BOOL                       enable);
PUBLIC PMC_ERROR digi_otn_odu_int_chnl_stage_clear(digi_handle_t             *digi_handle,
                                                   digi_otn_odu_chnl_t       *chnl_handle,
                                                   odu_struct_odu_level_t     odu_level,
                                                   digi_otn_odu_int_chnl_t   *int_table_ptr,
                                                   BOOL8 clear_rfrm);
PUBLIC PMC_ERROR digi_otn_odu_int_trigger_transfer(digi_handle_t    *digi_handle, 
                                                   coreotn_latch_defect_t latch_defect);

PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle_t *digi_handle,
                                               UINT32 enet_link,
                                               digi_client2trunk_conseq_action_t *client2trunk_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle_t *digi_handle,
                                                                       UINT32 enet_link,
                                                                       UINT32 client_type,
                                                                       UINT32 mapotn_chnl,
                                                                       UINT32 mapper, 
                                                                       UINT32 epmm_legacy_mode,
                                                                       UINT32 enable_monitoring,
                                                                       UINT32 is_send_dci,
                                                                       UINT32 is_gen_keep_alive_cmf,
                                                                       UINT32 upi_for_keep_alive_cmf,
                                                                       UINT32 is_gen_payload_fcs,
                                                                       UINT32 is_use_payload_csf,
                                                                       UINT8 *digi_cmf_payload_ptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle_t *digi_handle,
                                                                        UINT32 mapotn_chnl,
                                                                        UINT32 mapper, 
                                                                        UINT32 enet_link,
                                                                        UINT32 opu_csf_persistance_time,
                                                                        UINT32 client_type );

PUBLIC PMC_ERROR digi_cmf_opu_csf_monitor_cfg(digi_handle_t                   *digi_handle,
                                              digi_mapper_chnl_t              *chnl_ctxt_ptr,
                                              UINT32                           output_chnl,
                                              digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         cmf_trunk2client_conseqAction_t  *trunk2client_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 enet_cbrc_link,
                                                                         digi_client2trunk_conseq_action_t  *client2trunk_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         cmf_trunk2client_portMapping_t  *trunk2client_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 enet_cbrc_link,
                                                                         digi_client2trunk_port_action_table_cmf_t  *client2trunk_tables_ptr );

PUBLIC PMC_ERROR digi_cmf_clienttotrunk_tables_update_internal(digi_handle_t *digi_handle,
                                                               UINT32 mapper_chnl_id);

PUBLIC PMC_ERROR digi_cmf_enet_emac_lf_rf_cfg(digi_handle_t *digi_handle,
                                              emac_handle_t *hndl,
                                              UINT32 link, 
                                              BOOL8 flt_hdl_dis, 
                                              BOOL8 send_rf, 
                                              BOOL8 send_lf,
                                              BOOL8 etrans_fault_on, 
                                              BOOL8 etrans_legacy_mode);

PUBLIC PMC_ERROR digi_enet_validate_parms(digi_handle_t               *digi_handle,
                                          util_global_switch_data_t   *chnl_handle,
                                          enet_handle_t              **enet_pptr,
                                          UINT32                      *link_ptr);

PUBLIC PMC_ERROR digi_enet_channel_mcpb_clean_test(digi_handle_t               *digi_handle,
                                                   util_global_switch_data_t   *chnl_handle);

PUBLIC PMC_ERROR digi_enet_channel_mcpb_clean_filtered_test(digi_handle_t               *digi_handle,
                                                            util_global_switch_data_t   *chnl_handle,
                                                            UINT32 dcs_mask,
                                                            UINT32 target_mask,
                                                            BOOL8  source_mask);

PUBLIC PMC_ERROR digi_serdes_restart_init(digi_handle_t *digi_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PUBLIC void digi_serdes_cleanup(digi_handle_t *digi_handle);
PUBLIC void digi_int_disable_all(digi_handle_t   *digi_handle);
PUBLIC PMC_ERROR digi_dcsu_restart_init(digi_handle_t *digi_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PUBLIC void digi_enet_pmon_sys_usage_set(digi_handle_t* digi_handle,
                                          UINT32 enet_link, 
                                          digi_enet_mode_t mode,
                                         BOOL8 in_use);

PUBLIC PMC_ERROR digi_otn_fec_type_validate( digi_handle_t             *digi_handle,
                                             digi_otn_server_fec_t     fec_type );

PUBLIC util_global_switch_data_t* digi_cpb_retrieve_input_port(digi_handle_t* digi_handle,
                                                               UINT32 ingress_mux_input_port,
                                                               UINT32 ingress_mux_input_chnl);

PUBLIC PMC_ERROR digi_cpb_switch_data_slot_get(digi_handle_t *digi_handle,
                                               util_global_switch_data_def_t *switch_data_ptr,
                                               UINT32 *num_slot);

PUBLIC PMC_ERROR digi_map_opu_csf_int_default_cfg_get(digi_handle_t                  *digi_handle,
                                                      digi_mapper_chnl_def_t         *mapper_chnl_ctxt_ptr,
                                                      BOOL8                           enable,
                                                      digi_trunk2client_tables_cmf_t *trunk2client_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_cfg_internal(digi_handle_t                   *digi_handle,
                                                                   digi_mapper_chnl_t              *chnl_ctxt_ptr,
                                                                   digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr,
                                                                   BOOL8                            mapper_chnl_activate);



PUBLIC PMC_ERROR digi_map_opu_csf_int_cfg(digi_handle_t          *digi_handle,
                                          digi_mapper_chnl_def_t *mapper_chnl_ctxt_ptr,
                                          UINT32                  output_chnl,
                                          BOOL8                   enable);
                                                 
PUBLIC void digi_oduk_params_convert(digi_oduk_t odu_data,
                                     UINT32 odu_flex_data,
                                     util_global_odukp_type_t* oduk_data,
                                     digi_group_size_t* group_size);

PUBLIC void digi_mapper_odu_flex_data_get(digi_mapping_mode_t mode, 
                                          UINT32* odu_flex_data_ptr,
                                          util_global_odukp_type_t* oduk_data_ptr,
                                          UINT32* oduflex_cbr_rate_ptr);

PUBLIC BOOL8 digi_cpb_retrieve_mpma_data(digi_handle_t *digi_handle,
                                         UINT32 port_uid,
                                         util_global_switch_data_def_t *switch_data_ptr,
                                         digi_opsa_cfg_action_t action,
                                         hostmsg_opsa_subsystem_cfg_t *input_mpma_ptr,
                                         UINT32 *input_chnl_ptr,
                                         UINT8 *qs_ch_en_ptr);


PUBLIC BOOL8 digi_cpb_retrieve_mpmo_data(digi_handle_t *digi_handle,
                                         UINT32 port_uid,
                                         util_global_switch_data_def_t *switch_data_ptr,
                                         digi_opsa_cfg_action_t action,
                                         BOOL8 is_loopback,
                                         hostmsg_opsa_subsystem_cfg_t *output_mpmo_ptr,
                                         BOOL8 *cbrc_monitor_ptr,
                                         hostmsg_opsa_subsystem_cfg_t *input_fo2_ptr,
                                         UINT32 *input_fo2_chnl_ptr,
                                         BOOL8 *is_bmp_ptr);

PUBLIC void digi_otn_rfrm_maint_sig_set(digi_handle_t *digi_handle,
                                        odu_struct_t *odu_ptr,
                                        BOOL8 is_sysotn,
                                        coreotn_maint_sig_type_t *odu_rx_maint_sig_ptr);



PUBLIC BOOL digi_otn_remove_dummy_maintenance_signal_get(digi_handle_t       *digi_handle,
                                                         odu_struct_t      *odu_ptr);

PUBLIC BOOL digi_otn_nominal_tx_unchannelized_get(digi_handle_t       *digi_handle,
                                                  odu_struct_t      *odu_ptr);                                                         

PUBLIC void digi_otn_channel_nominal_set(digi_handle_t         *digi_handle,
                                         otn_maint_ins_point_t ins_point,
                                         UINT32                channel,
                                         BOOL                  nominal);

PUBLIC BOOL digi_otn_channel_nominal_get(digi_handle_t          *digi_handle,
                                         otn_maint_ins_point_t  ins_point,
                                         UINT32                 channel);

PUBLIC void digi_cbr_nominal_fault_init(digi_handle_t *digi_handle);

PUBLIC void digi_cbr_nominal_fault_set(digi_handle_t *digi_handle,
                                       UINT32 chnl_id,
                                       BOOL8 value);

PUBLIC BOOL8 digi_cbr_nominal_fault_get(digi_handle_t *digi_handle,
                                        UINT32 chnl_id);

PUBLIC UINT32 digi_port_pin_bit_mask_get(digi_handle_t* digi_handle,
                                          digi_serdes_port_def_t* port_def_ptr,
                                          UINT16* bit_mask);
/* 
** Deprecated Functions 
*/


#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR digi_by_mld_index_post_mld_prov_state_get(digi_handle_t *digi_handle,
                                                            UINT8 post_mld_index,
                                                            digi_serdes_prov_state_t* prov_state);

PUBLIC PMC_ERROR  digi_serdes_all_zeros_or_ones_det_cfg(digi_handle_t              *digi_handle,
                                                             util_global_switch_data_t  *chnl_handle,
                                                             BOOL8                       enable);

PUBLIC PMC_ERROR digi_serdes_all_zeros_or_ones_det_status_get(digi_handle_t              *digi_handle,
                                                              util_global_switch_data_t  *chnl_handle,
                                                              util_patt_status_t         *status);

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_LOC_H */

/* 
** end of file 
*/
