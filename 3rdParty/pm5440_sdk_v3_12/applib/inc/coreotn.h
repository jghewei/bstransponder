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
#ifndef _COREOTN_H
#define _COREOTN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "coreotn_api.h"
#include "util_global.h"
#include "cpb.h"
#include "mpma.h"
#include "mpmo.h"
#include "oduksc.h"
#include "odu_tfrm.h"
#include "odu_rfrm.h"
#include "util_schd.h"
#include "pmc_handle.h"
#include "pmc_log.h"
#include "scbs3.h"
#include "cfc.h"
#include "rxjc_proc.h"
#include "ohfs_remove.h"
#include "mp_mgen.h"
#include "cpact_obuf.h"
#include "expand_ibuf.h"
#include "txjc_proc.h"
#include "ohfs_insert.h"
#include "sts_sw.h"
#include "tgmp.h"
#include "rgmp.h"
#include "odtu_mux.h"
#include "odtu_dmx.h"
#include "odujat.h"
#include "odu_rcp.h"
#include "odu_ohp.h"
#include "odu_struct_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: coreotn_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify COREOTN channel state definition.
*
* ELEMENTS:
*   COREOTN_CHNL_START         - start state \n
*   COREOTN_CHNL_EQUIPPED      - provisioned state \n
*   COREOTN_CHNL_OPERATIONAL   - activated and operational state \n
*   COREOTN_CHNL_IN_PASSTHRU   - channel is in passthru \n
*   COREOTN_CHNL_PROV_PENDING  - channel is provisioned on the current offline 
*                                page. Page swap and updating the offline page 
*                                has not occurred on these channels. 
*                                This state is used in bulk-provisioning of LO
*                                ODUk channels.\n
*   COREOTN_CHNL_DEPROV_PENDING- channel is deprovisioned on the current offline 
*                                page. Page swap and updating the offline page 
*                                has not occurred on these channels.   
*                                This state is used in bulk-deprovisioning of LO
*                                ODUk channels. \n
*
*******************************************************************************/
typedef enum {
    COREOTN_CHNL_START = 0,
    COREOTN_CHNL_EQUIPPED,
    COREOTN_CHNL_OPERATIONAL,
    COREOTN_CHNL_IN_PASSTHRU,
    COREOTN_CHNL_PROV_PENDING,
    COREOTN_CHNL_DEPROV_PENDING,
} coreotn_chnl_state_t;

/*******************************************************************************
* ENUM: coreotn_fo_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify COREOTN fo instance 
*
* ELEMENTS:
*   COREOTN_FO_INST_1 - COREOTN FO1 \n
*   COREOTN_FO_INST_2 - COREOTN FO2
*
*******************************************************************************/
typedef enum {
    COREOTN_FO_INST_1 = 0,
    COREOTN_FO_INST_2,
} coreotn_fo_inst_t;

/*******************************************************************************
* ENUM: coreotn_power_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated the power "zone" in COREOTN 
*
* ELEMENTS:
*   COREOTN_STG4   - coreotn stage 4 \n 
*   COREOTN_ODUKP  - coreotn ODUKP data paths (from line to ODUKSW) \n
*   COREOTN_ODUKSW  - ODUkSW \n
*   COREOTN_ODU_IF - COREOTN supporting interface 
*
*******************************************************************************/
typedef enum
{
    COREOTN_STG4 = 0,
    COREOTN_ODUKP,
    COREOTN_ODUKSW,    
    COREOTN_ODU_IF,
    LAST_COREOTN_POWER,
} coreotn_power_t;

/*******************************************************************************
* ENUM: coreotn_fmf_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify COREOTN fmf instance 
*
* ELEMENTS:
*   COREOTN_FMF1_INST          - The FMF1 Instance. \n
*   COREOTN_FMF2_INST          - The FMF2 Instance.
*
*******************************************************************************/
typedef enum {
    COREOTN_FMF_INST_1 = 0,
    COREOTN_FMF_INST_2,
    LAST_COREOTN_FMF_INST
} coreotn_fmf_inst_t;

/*******************************************************************************
* ENUM: coreotn_init_operation_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying destination interface.
*
* ELEMENTS:
*   COREOTN_SS_OPERATIONAL_MODE         - COREOTN subsystem is enabled and all
*                                         resources are operational.\n
*   COREOTN_SS_ONE_STAGE_MUXING_MODE    - COREOTN subsystem is enabled but is
*                                         only used for one stage muxing at the
*                                         device level.\n
*   COREOTN_SS_SYSOTN_MODE              - COREOTN subsystem is enabled to
*                                         operate in SYSOTN mode (with muxing)\n
*   COREOTN_SS_SYSOTN_BYPASS_MODE       - COREOTN subsystem is enabled to
*                                         operate in SYSOTN mode (without muxing)\n
*   COREOTN_SS_BYPASS_MODE              - COREOTN subsystem is enabled to
*                                         operate in full bypass mode 
*
*******************************************************************************/
typedef enum
{
    COREOTN_SS_OPERATIONAL_MODE         = 0,
    COREOTN_SS_ONE_STAGE_MUXING_MODE    ,
    COREOTN_SS_SYSOTN_MODE              ,
    COREOTN_SS_BYPASS_MODE              ,
    COREOTN_SS_SYSOTN_BYPASS_MODE       ,
    LAST_COREOTN_INIT_OPERATION                       /* this is for out of range values error handling */
} coreotn_init_operation_t;

/*******************************************************************************
* ENUM: coreotn_oduksw_input_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying input port of ODUKSW.
*
* ELEMENTS:
*   COREOTN_ODUKSW_INPUT_SIFD1              - input port from SIFD1\n
*    COREOTN_ODUKSW_INPUT_SIFD2              - input port from SIFD2\n
*   COREOTN_ODUKSW_INPUT_COREOTN            - input port from COREOTN(FO1)\n
*   COREOTN_ODUKSW_INPUT_MAPOTN             - input port from MAPOTN\n
*   COREOTN_ODUKSW_INPUT_LINESIDE_COREOTN   - input from lineside COREOTN(FMF1)\n
*       NOTE: Only for single-chip mux-ponder and transponder application
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODUKSW_INPUT_SIFD1              = 0,
    COREOTN_ODUKSW_INPUT_SIFD2              = 1,
    COREOTN_ODUKSW_INPUT_COREOTN            = 2,
    COREOTN_ODUKSW_INPUT_MAPOTN             = 3,
    COREOTN_ODUKSW_INPUT_LINESIDE_COREOTN   = 4,
    COREOTN_ODUKSW_INPUT_NOT_USED,              /* Added to match the util_global_oduk_port_t */
    LAST_COREOTN_ODUKSW_INPUT,                  /* this is for out of range values error handling */
} coreotn_oduksw_input_port_type_t;

/*******************************************************************************
* ENUM: coreotn_oduksw_output_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying destination interface.
*
* ELEMENTS:
*   COREOTN_ODUKSW_OUTPUT_SIFD1             - output port to SIFD1\n
*    COREOTN_ODUKSW_OUTPUT_SIFD2             - output port to SIFD2\n
*   COREOTN_ODUKSW_OUTPUT_COREOTN           - output port to COREOTN(FO1)\n
*   COREOTN_ODUKSW_OUTPUT_MAPOTN            - output port to MAPOTN\n
*   COREOTN_ODUKSW_OUTPUT_LINESIDE_COREOTN  - output to lineside COREOTN(FMF1)\n
*       NOTE: Only for single-chip mux-ponder and transponder application
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODUKSW_OUTPUT_SIFD1             = 0,
    COREOTN_ODUKSW_OUTPUT_SIFD2             = 1,
    COREOTN_ODUKSW_OUTPUT_COREOTN           = 2,
    COREOTN_ODUKSW_OUTPUT_MAPOTN            = 3,
    COREOTN_ODUKSW_OUTPUT_LINESIDE_COREOTN  = 4,

    LAST_COREOTN_ODUKSW_OUTPUT,                   /* this is for out of range values error handling */
} coreotn_oduksw_output_port_type_t;

/*******************************************************************************
* ENUM: coreotn_oduksw_xfer_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying burst transfer size.
*
* ELEMENTS:
*   COREOTN_ODUKSW_SEGMENT            - Word Transfers\n
*   COREOTN_ODUKSW_PACKET             - Packet Transfers\n
*   COREOTN_ODUKSW_SEGMENT_BURST_128  - Segment Burst 128 Byte Transfers\n
*   COREOTN_ODUKSW_SEGMENT_BURST_192  - Segment Burst 192 Byte Transfers\n
*   COREOTN_ODUKSW_SEGMENT_BURST_256  - Segment Burst 256 Byte Transfers\n
*   COREOTN_ODUKSW_PACKET_BURST_128   - Packet Burst 128 Byte Transfers\n
*   COREOTN_ODUKSW_PACKET_BURST_192   - Packet Burst 192 Byte Transfers\n
*   COREOTN_ODUKSW_PACKET_BURST_256   - Packet Burst 256 Byte Transfers
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODUKSW_SEGMENT = 0,
    COREOTN_ODUKSW_PACKET,
    COREOTN_ODUKSW_SEGMENT_BURST_128,
    COREOTN_ODUKSW_SEGMENT_BURST_192,
    COREOTN_ODUKSW_SEGMENT_BURST_256,
    COREOTN_ODUKSW_PACKET_BURST_128,
    COREOTN_ODUKSW_PACKET_BURST_192,
    COREOTN_ODUKSW_PACKET_BURST_256,

    LAST_COREOTN_ODUKSW_BURST_XFER     /* this is for out of range values error handling */

} coreotn_oduksw_xfer_mode_t;


/*******************************************************************************
* ENUM: coreotn_mux_stage_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify number of stages involved in
*    multiplexing the payload data.
*
* ELEMENTS:
*   COREOTN_MUX_STAGE_NONE   - No multiplexing with unchannelized payload \n
*   COREOTN_MUX_STAGE_ONE    - One stage muxing or demuxing \n
*   COREOTN_MUX_STAGE_TWO    - Two stage muxing or demuxing
*           
*******************************************************************************/
typedef enum
{
    COREOTN_MUX_STAGE_NONE     = 0,
    COREOTN_MUX_STAGE_ONE      = 1,
    COREOTN_MUX_STAGE_TWO      = 2,

    LAST_COREOTN_MUX_STAGE                    /* this is for out of range values error handling */
} coreotn_mux_stage_t;

/*******************************************************************************
* ENUM: coreotn_mapping_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store mapping mode supported in COREOTN
*   subsystem.
*
* ELEMENTS:
*   COREOTN_MAP_AMP         - channel is AMP mapped \n
*   COREOTN_MAP_GMP         - channel is GMP mapped \n
*   COREOTN_MAP_RESERVED    - reserved mapping mode
*
*******************************************************************************/
typedef enum {
    COREOTN_MAP_AMP        = 1,
    COREOTN_MAP_GMP        = 2,
    COREOTN_MAP_RESERVED   = 3,

    LAST_COREOTN_MAP                /* this is for out of range values error handling */
} coreotn_mapping_type_t;

/*******************************************************************************
* ENUM: coreotn_oduksw_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client port using the ODUKSW.
*
*   The element terms have a syntax:
*      COREOTN_ODUKSW_PORT_<subsystem_block>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    COREOTN_ODUKSW_PORT_ILKN1 = 0,
    COREOTN_ODUKSW_PORT_ILKN2,
    COREOTN_ODUKSW_PORT_COREOTN,
    COREOTN_ODUKSW_PORT_MAPOTN,

    LAST_COREOTN_ODUKSW_PORT        /* this is for out of range values error handling */
} coreotn_oduksw_port_t;

/*******************************************************************************
* ENUM: coreotn_odu_rfrm_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odu_rfrm
*
* ELEMENTS:
*   COREOTN_ODU_RFRM_STG1      - Stage 1 ODU_RFRM \n
*   COREOTN_ODU_RFRM_STG2      - Stage 2 ODU_RFRM \n
*   COREOTN_ODU_RFRM_STG3A     - Stage 3a ODU_RFRM \n
*   COREOTN_ODU_RFRM_STG3B     - Stage 3b ODU_RFRM \n
*   COREOTN_ODU_RFRM_STG4      - Stage 4 ODU_RFRM \n
*   COREOTN_ODU_RFRM_STGN      - Stage N ODU_RFRM
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODU_RFRM_STG1   = 0,
    COREOTN_ODU_RFRM_STG2   = 1,
    COREOTN_ODU_RFRM_STG3A  = 2,
    COREOTN_ODU_RFRM_STG3B  = 3,
    COREOTN_ODU_RFRM_STG4   = 4,
    COREOTN_ODU_RFRM_STGN   = 5,
} coreotn_odu_rfrm_inst_t;

/*******************************************************************************
* ENUM: coreotn_odu_tfrm_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odu_tfrm
*
* ELEMENTS:
*   COREOTN_ODU_TFRM_STG1      - Stage 1 ODU_TFRM \n
*   COREOTN_ODU_TFRM_STG2      - Stage 2 ODU_TFRM \n
*   COREOTN_ODU_TFRM_STG3A     - Stage 3a ODU_TFRM \n
*   COREOTN_ODU_TFRM_STG3B     - Stage 3b ODU_TFRM \n
*   COREOTN_ODU_TFRM_STG4      - Stage 4 ODU_TFRM \n
*   COREOTN_ODU_TFRM_STGN      - Stage N ODU_TFRM
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODU_TFRM_STG1   = 0,
    COREOTN_ODU_TFRM_STG2   = 1,
    COREOTN_ODU_TFRM_STG3A  = 2,
    COREOTN_ODU_TFRM_STG3B  = 3,
    COREOTN_ODU_TFRM_STG4   = 4,
    COREOTN_ODU_TFRM_STGN   = 5,
} coreotn_odu_tfrm_inst_t;

/*******************************************************************************
* ENUM: coreotn_odtu_dmx_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odtu_dmx
*
* ELEMENTS:
*   COREOTN_ODTU_DMX_STG1        - First ODTU_DMX \n
*   COREOTN_ODTU_DMX_STG2        - Second ODTU_DMX
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODTU_DMX_STG1   = 0,
    COREOTN_ODTU_DMX_STG2   = 1,
} coreotn_odtu_dmx_inst_t;

/*******************************************************************************
* ENUM: coreotn_odtu_mux_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odtu_dmx
*
* ELEMENTS:
*   COREOTN_ODTU_MUX_STG1     - First ODTU_MUX \n
*   COREOTN_ODTU_MUX_STG2     - Second ODTU_MUX
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODTU_MUX_STG1   = 0,
    COREOTN_ODTU_MUX_STG2    = 1,
} coreotn_odtu_mux_inst_t;

/*******************************************************************************
* ENUM: coreotn_oduksc_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an oduksc
*
* ELEMENTS:
*   COREOTN_ODUKSC1      - ODUKSC connected to ODU_RFRM_STG2 \n
*   COREOTN_ODUKSC2      - ODUKSC connected to ODU_RFRM_STG3A
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODUKSC1   = 0,
    COREOTN_ODUKSC2   = 1,
} coreotn_oduksc_inst_t;

/*******************************************************************************
* ENUM: coreotn_odujat_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odujat
*
* ELEMENTS:
*   COREOTN_ODUJAT1      - ODUJAT after ODTU_DMX1 \n
*   COREOTN_ODUJAT2      - ODUJAT after ODTU_DMX2
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODUJAT1   = 0,
    COREOTN_ODUJAT2   = 1,
} coreotn_odujat_inst_t;

/*******************************************************************************
* ENUM: coreotn_frm_rcp_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odujat
*
* ELEMENTS:
*   COREOTN_FMF1_FRM_RCP12       - FRM_RCP12 in FMF1 (STG 1 framers) \n
*   COREOTN_FMF1_FRM_RCP96       - FRM_RCP96 in FMF1 (STG 2 framers) \n
*   COREOTN_FMF2_FRM_RCP12       - FRM_RCP12 in FMF1 (STG N framers) \n
*   COREOTN_FMF2_FRM_RCP96       - FRM_RCP96 in FMF1 (STG 3A framers) \n
*   COREOTN_FO1_FRM_RCP          - FRM_RCP in FO1    (STG 3B framers) \n
*   COREOTN_FO2_FRM_RCP          - FRM_RCP in FO2    (STG 4 framers)
*
*******************************************************************************/
typedef enum
{
    COREOTN_FMF1_FRM_RCP12   = 0,
    COREOTN_FMF1_FRM_RCP96,
    COREOTN_FMF2_FRM_RCP12,
    COREOTN_FMF2_FRM_RCP96,
    COREOTN_FO1_FRM_RCP,
    COREOTN_FO2_FRM_RCP,
    LAST_COREOTN_FRM_RCP,
} coreotn_frm_rcp_inst_t;

/*******************************************************************************
* ENUM: coreotn_mpmo_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of a mpmo
*
* ELEMENTS:
*   COREOTN_MPMO_TO_COREOTN  - sourced from ODUk pushing to MUX/Framer \n
*   COREOTN_MPMO_TO_MAPOTN   - sourced from ODUk pushing to MAPOTN \n
*   COREOTN_MPMO_CORECTL     - source from ODUk pushing to COREOTN_CTL
*
*******************************************************************************/
typedef enum
{
    COREOTN_MPMO_TO_COREOTN     = 0,
    COREOTN_MPMO_TO_MAPOTN      = 1,
    COREOTN_MPMO_CORECTL        = 2,
    LAST_COREOTN_MPMO_INST
} coreotn_mpmo_inst_t;

/*******************************************************************************
* ENUM: coreotn_mpma_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of a mpmo
*
* ELEMENTS:
*   COREOTN_MPMA_TO_COREOTN  - sourced from Rx Framer/Demux pushing to ODUk \n
*   COREOTN_MPMA_FROM_MAPOTN - sourced from MAPOTN pushing to ODUk \n
*   COREOTN_MPMA_CORECTL     - sourced from COREOTN_CTL pushing to ODUk
*
*******************************************************************************/
typedef enum
{
    COREOTN_MPMA_TO_COREOTN     = 0,
    COREOTN_MPMA_FROM_MAPOTN    = 1,
    COREOTN_MPMA_CORECTL        = 2,
    LAST_COREOTN_MPMA_INST
} coreotn_mpma_inst_t;

/*******************************************************************************
* ENUM: coreotn_prbs_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of PRBS
*
* ELEMENTS:
*   COREOTN_FMF1_PRBS    - PRBS Generator and monitor block in FMF1 \n
*   COREOTN_FMF2_PRBS    - PRBS Generator and monitor block in FMF2 \n
*   COREOTN_FO1_PRBS     - PRBS Generator and monitor block in FO1 \n
*   COREOTN_FO2_PRBS     - PRBS Generator and monitor block in FO2
*
******************************************************************************
typedef enum
{
    COREOTN_FMF1_PRBS   = 0,
    COREOTN_FMF2_PRBS   = 1,
    COREOTN_FO1_PRBS    = 2,
    COREOTN_FO2_PRBS    = 3,
} coreotn_prbs_inst_t;*/

/*******************************************************************************
* ENUM: coreotn_odu_rcp_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odu_rcp
*
* ELEMENTS:
*   COREOTN_ODU_RCP_RI      - RI ODU RCP \n
*   COREOTN_ODU_RCP_DI      - DI ODU RCP
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODU_RCP_RI   = 0,
    COREOTN_ODU_RCP_DI   = 1,
    LAST_COREOTN_ODU_RCP
} coreotn_odu_rcp_inst_t;

/*******************************************************************************
* ENUM: coreotn_ohfs_remove_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of an odtu_dmx
*
* ELEMENTS:
*   COREOTN_FO1_OHFS_REMOVE        - OHFS_REMOVE block in FO1 \n
*   COREOTN_FO2_OHFS_REMOVE        - OHFS_REMOVE block in FO2
*
*******************************************************************************/
typedef enum
{
    COREOTN_FO1_OHFS_REMOVE   = 0,
    COREOTN_FO2_OHFS_REMOVE   = 1,
} coreotn_ohfs_remove_inst_t;

/*******************************************************************************
* ENUM: coreotn_chnl_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the operational mode of an
*    ODU channel in COREOTN.
*
* ELEMENTS:
*   COREOTN_CHNL_PASSTHRU            - channel is in passthru \n
*    COREOTN_CHNL_NORMAL_OPERATION    - channel is in operation
*
*
*******************************************************************************/
typedef enum
{
    COREOTN_CHNL_PASSTHRU           = 0,
    COREOTN_CHNL_NORMAL_OPERATION   = 1,

    LAST_COREOTN_CHNL_MODE                      /* this is for out of range values error handling */
} coreotn_chnl_mode_t;

/*******************************************************************************
* ENUM: coreotn_prbs_pattern_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store PRBS pattern types.
*
* ELEMENTS:
*   COREOTN_INVERTED_PRBS_11    - Inverted PRBS-11 \n
*   COREOTN_INVERTED_PRBS_31    - Inverted PRBS-31 \n
*   COREOTN_PRBS_11             - PRBS-11 \n
*   COREOTN_PRBS_31             - PRBS-31
*
*******************************************************************************/
typedef enum
{
    COREOTN_INVERTED_PRBS_11    = 0,
    COREOTN_INVERTED_PRBS_31    = 1,
    COREOTN_PRBS_11             = 2,
    COREOTN_PRBS_31             = 3,
} coreotn_prbs_pattern_t;

/*******************************************************************************
* ENUM: coreotn_chnl_order_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the channel order type.
*
* ELEMENTS:
*   COREOTN_CHNL_ORDER_LO           - channel is of the low order flavour \n
*   COREOTN_CHNL_ORDER_MO           - channel is of the mid order flavour \n
*   COREOTN_CHNL_ORDER_HO           - channel is of the high order flavour
*
*
*******************************************************************************/
typedef enum
{
    COREOTN_CHNL_ORDER_LO = 0,
    COREOTN_CHNL_ORDER_MO = 1,
    COREOTN_CHNL_ORDER_HO = 2,

    LAST_COREOTN_CHNL_ORDER  /* this is for out of range values error handling */
} coreotn_chnl_order_t;

/*******************************************************************************
* ENUM: coreotn_odu_ohp_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated ODU_OHP instances.
*
* ELEMENTS:
*   COREOTN_ODU_OHP_1            - ODU OHP 1 \n
*   COREOTN_ODU_OHP_2            - ODU OHP 2 \n
*   COREOTN_ODU_OHP_3            - ODU OHP 3
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODU_OHP_1 = 0,
    COREOTN_ODU_OHP_2 = 1,
    COREOTN_ODU_OHP_3 = 2,

    LAST_COREOTN_ODU_OHP  /* this is for out of range values error handling */
} coreotn_odu_ohp_inst_t;


/*******************************************************************************
* ENUM: coreotn_int_source_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to indicate LINE or SYSTEM source
*
* ELEMENTS:
*   COREOTN_INT_SOURCE_LINE     - LINEOTN \n
*   COREOTN_INT_SOURCE_SYS      - SYSOTN
*
*******************************************************************************/
typedef enum
{
    COREOTN_INT_SOURCE_LINE = 0,
    COREOTN_INT_SOURCE_SYS,

    LAST_COREOTN_INT_SOURCE,    /* this is for out of range values error handling */
} coreotn_int_source_t;


/*******************************************************************************
* ENUM: coreotn_int_type_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Type of coreotn interrupts
*
* ELEMENTS:
*   COREOTN_INT_TYPE_STG4   - coreotn stage 4  \n
*   COREOTN_INT_TYPE_ODUKP  - coreotn ODUKP data paths (from line to ODUKSW)
*
*******************************************************************************/
typedef enum
{
    COREOTN_INT_TYPE_ODUKP = 0,
    COREOTN_INT_TYPE_STG4,

    LAST_COREOTN_INT_TYPE,
} coreotn_int_type_t;


/*******************************************************************************
* ENUM: coreotn_ddeg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for dDEG monitoring mode.
*
* ELEMENTS:
*   COREOTN_HARDWARE_DDEG      - Hardware dDEG will issue dDEG report using
*                                 1 second monitoring interval as defined
*                                 in G.806. \n
*   COREOTN_ENHANCED_DDEG      - dDEG report will be issued by firmware. The
*                                 firmware will issue both the G.806 dDEG report
*                                 AND an "enhanced dDEG report".
*                                 The enhanced dDEG report allows for a 
*                                 configurable monitoring interval (10ms, or 100ms) \n
*   COREOTN_ENH_BURSTY_DDEG    - dDEG report will be issued by firmware. The
*                                 firmware will issue the G.806 dDEG report. 
*   
*******************************************************************************/
typedef enum
{
    COREOTN_HARDWARE_DDEG = 0,
    COREOTN_ENHANCED_DDEG, 
    COREOTN_ENH_BURSTY_DDEG,
    
    LAST_COREOTN_DDEG_MODE,        /* this is for error checking */
} coreotn_ddeg_mode_t;

/*******************************************************************************
* ENUM: coreotn_ddeg_mon_seg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for dDEG monitoring segment.
*
* ELEMENTS:
*   COREOTN_DDEG_MON_LINE      - dDEG monitoring is performed on COREOTN 
*                                Stage 1/2/N/3A framers \n
*   COREOTN_DDEG_MON_CLIENT    - dDEG monitoring is performed on COREOTN STG4
*                                framer. 
*******************************************************************************/
typedef enum
{
    COREOTN_DDEG_MON_LINE      = 0,
    COREOTN_DDEG_MON_CLIENT    = 1,  
    
    LAST_COREOTN_DDEG_MON_SEG,        /* this is for error checking */
} coreotn_ddeg_mon_seg_t;

/*******************************************************************************
* ENUM: coreotn_dci_mux3_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the source of MUX3 control.
*
*   The element terms have a syntax:
*      COREOTN_DCI_MUX3_<dci_source>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    COREOTN_DCI_MUX3_ODU_RFRM_96    = 0,
    COREOTN_DCI_MUX3_MUX1_CTRL      = 1,
    LAST_COREOTN_DCI_MUX3
} coreotn_dci_mux3_src_t;
     
/*******************************************************************************
* ENUM: coreotn_dci_mux4_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the source of MUX4 control.
*
*   The element terms have a syntax:
*      COREOTN_DCI_MUX4_<dci_source>
*
* ELEMENTS:
*       COREOTN_DCI_MUX4_ODTU_MUX - for 1 stage or 2 stage muxing \n
*       COREOTN_DCI_MUX4_ODU_RFRM_96 - from LO ODU_TFRM \n
*       COREOTN_DCI_MUX4_LO_DCI_INTF - for no stage muxing
*
*******************************************************************************/
typedef enum {
    COREOTN_DCI_MUX4_ODTU_MUX       = 0,
    COREOTN_DCI_MUX4_ODU_RFRM_96    = 1,
    COREOTN_DCI_MUX4_LO_DCI_INTF    = 2,
    LAST_COREOTN_DCI_MUX4
} coreotn_dci_mux4_src_t;  

/*******************************************************************************
* ENUM: coreotn_xoff_sel_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration describing which XOFF selection to select.
*
* ELEMENTS:
    COREOTN_XOFF_SEL_ZERO      - XOFF MUX Select position zero. \n
    COREOTN_XOFF_SEL_ONE       - XOFF MUX Select position one.
*  
*******************************************************************************/
typedef enum
{
    COREOTN_XOFF_SEL_ZERO = 0,
    COREOTN_XOFF_SEL_ONE  = 1,

    LAST_COREOTN_XOFF_SEL
} coreotn_xoff_sel_t;

/*******************************************************************************
* ENUM: coreotn_odu_chnl_prov_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the provisioning mode of 
*    LO ODU channels in a HO channel
*
* ELEMENTS:
*   COREOTN_ODU_CHNL_PROV_IN_INCREMENT - default setting. This will provision
*                                        one LO ODU channel into a HO at a time \n
*   COREOTN_ODU_CHNL_PROV_ALL          - This will provision all LO ODU channels
*                                        into a HO at once. This mode will
*                                        reduce provisioning configuration time. 
*   COREOTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK,
*                                      - Should be only used in 1+1 SNCP case,
*                                        where switching happens inside oduksw.
*
*******************************************************************************/
typedef enum
{
    COREOTN_ODU_CHNL_PROV_IN_INCREMENT = 0,
    COREOTN_ODU_CHNL_PROV_ALL,
    COREOTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK,

    LAST_COREOTN_CHNL_PROV_MODE,
} coreotn_odu_chnl_prov_mode_t;

/*******************************************************************************
* ENUM: coreotn_lo_odu_chnl_cfg_state_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration describing the configuration status of each LO channel.
*
* ELEMENTS:
*   COREOTN_LO_ODU_CHNL_CFG_START   - The first LO channel being added to a HO \n
*   COREOTN_LO_ODU_CHNL_CFG_APPLY   - LO channel being added to a HO. Not the
*                                     first nor the last LO channel \n
*   COREOTN_LO_ODU_CHNL_CFG_END     - The last LO channel being added to a HO
*  
*******************************************************************************/
typedef enum
{
    COREOTN_LO_ODU_CHNL_CFG_START = 0,
    COREOTN_LO_ODU_CHNL_CFG_APPLY,
    COREOTN_LO_ODU_CHNL_CFG_END,
    
    LAST_COREOTN_LO_ODU_CHNL_CFG_MODE,

} coreotn_lo_odu_chnl_cfg_state_t;

/*
** Constants
*/
#define COREOTN_PRBS_NUM_CHNL        12


/* maximum number of OHP instances */
#define COREOTN_NUM_ODU_OHP_MAX         3
#define COREOTN_UNUSED_TCM_POOL_ID      0xFFFF

/* number of DDEG modules */
#define COREOTN_NUM_DDEG_MOD            3

#define COREOTN_ODUKSW_PORT_TYPE_DPI_SLAVE      0
#define COREOTN_ODUKSW_PORT_TYPE_DPI_MASTER     1
#define COREOTN_ODUKSW_PORT_TYPE_DCS            2
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: coreotn_int_chnl_info_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing a specific channel and odu level for COREOTN 
*   interrupt operations
*
* ELEMENTS:
*   chnl_id             - Channel to access
*   rx_pool_id          - TCM PID(s) to access
*   odu_level           - HO/MO/LO odu level
*   source              - Line or System LINEOTN
*   rfrm_int_summary    - OTN ODU interrupt summary information
*   blk_filter          - Block filtering bitmask
*   defect              - defect type
*
*******************************************************************************/
typedef struct {
    UINT32                       chnl_id;
    UINT32                       rx_pool_id[ODU_RFRM_PIDS_PER_CHNL];
    odu_struct_odu_level_t       odu_level;
    coreotn_int_source_t         source;
    odu_rfrm_int_summary_t       rfrm_int_summary;
    UINT32                       blk_filter;
    odu_rfrm_int_defect_t        defect;
} coreotn_int_chnl_info_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

/*
 * Exported Interface
 */
PUBLIC coreotn_handle_t *coreotn_ctxt_create(pmc_handle_t *parent, UINT32 base_address, 
                                             pmc_sys_handle_t *sys_handle, 
                                             const char *tsb_name);
PUBLIC void coreotn_ctxt_destroy(coreotn_handle_t *coreotn_handle);
PUBLIC void coreotn_handle_init(coreotn_handle_t *coreotn_handle);
PUBLIC void coreotn_lo_chnl_get(coreotn_handle_t *coreotn_handle,
                                     BOOL8  line_side,
                                     coreotn_chnl_order_t ho_odu_level,
                                     UINT32 ho_chnl_id,
                                     UINT32 *ch_ptr,
                                     UINT32 *num_ch);

PUBLIC void coreotn_ilkn_interface_enable(coreotn_handle_t *coreotn_handle, BOOL8 enable);
PUBLIC void coreotn_alloc_init(coreotn_handle_t *coreotn_handle);
PUBLIC PMC_ERROR coreotn_handle_restart_init(coreotn_handle_t *coreotn_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                             pmc_energy_state_t energy_state);
PUBLIC void coreotn_operation_init_set(coreotn_handle_t *coreotn_handle,
                                       coreotn_init_operation_t init_operation);
PUBLIC BOOL coreotn_start_state_test(coreotn_handle_t *coreotn_handle);
PUBLIC BOOL coreotn_stg4_start_state_test(coreotn_handle_t *coreotn_handle);
PUBLIC BOOL coreotn_odukp_start_state_test(coreotn_handle_t *coreotn_handle);
PUBLIC BOOL coreotn_oduksw_start_state_test(coreotn_handle_t *coreotn_handle);
PUBLIC void coreotn_init(coreotn_handle_t *coreotn_handle, 
                         pmc_energy_state_request_t energy_state_request,
                         coreotn_power_t power);
PUBLIC PMC_ERROR coreotn_odu_chnl_prov_mode_set(coreotn_handle_t *coreotn_handle,
                                                coreotn_odu_chnl_prov_mode_t prov_mode); 
PUBLIC PMC_ERROR coreotn_odu_chnl_prov_mode_get(coreotn_handle_t *coreotn_handle,
                                                coreotn_odu_chnl_prov_mode_t *prov_mode);                                                                             
PUBLIC BOOL8 coreotn_sysapp_line_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id,
                                               util_global_chnl_state_t state);
PUBLIC PMC_ERROR coreotn_stg4_rx_chnl_prov(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl_id, UINT32 mode);      
PUBLIC PMC_ERROR coreotn_stg4_tx_chnl_prov(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl_id, UINT32 mode);      
PUBLIC BOOL8 coreotn_stg4_rx_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl_id);      
PUBLIC BOOL8 coreotn_stg4_tx_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl_id);  
PUBLIC PMC_ERROR coreotn_sysapp_line_chnl_prov(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id);
PUBLIC PMC_ERROR coreotn_sysapp_line_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                                 UINT32 chnl_id);
PUBLIC PMC_ERROR coreotn_super_prov_lo_prov_num_chnl_set(coreotn_handle_t *coreotn_handle,
                                                         UINT8 ho_chnl_id,
                                                         UINT8 num_mo_chnls,
                                                         UINT8 num_lo_chnls); 
PUBLIC PMC_ERROR coreotn_super_prov_lo_prov_num_chnl_get(coreotn_handle_t *coreotn_handle,
                                                         util_global_odu_level_t odu_level,
                                                         UINT8 ho_chnl_id,
                                                         UINT8 *num_mo_chnls,
                                                         UINT8 *num_lo_chnls); 
PUBLIC PMC_ERROR coreotn_lo_odu_chnl_cfg_state_set(coreotn_handle_t *coreotn_handle,
                                                   util_global_odu_level_t odu_level,
                                                   UINT32 chnl_id,
                                                   coreotn_lo_odu_chnl_cfg_state_t cfg_state,
                                                   BOOL coreotn_rx_prov,
                                                   BOOL coreotn_tx_prov);

PUBLIC PMC_ERROR coreotn_rx_lo_chnl_offline_update_all(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT32 ho_chnl_id,
                                                       UINT32 *new_lo_chnl_ids,
                                                       UINT32 num_new_lo);    
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_offline_update_all(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT32 ho_chnl_id,
                                                       UINT32 *new_lo_chnl_ids,
                                                       UINT32 num_new_lo);                                                                                                                                                                                                                                                                                                         
PUBLIC PMC_ERROR coreotn_ho_chnl_prov(coreotn_handle_t *coreotn_handle,
                                      UINT32 chnl_id,
                                      coreotn_mux_stage_t mux_stage,
                                      util_global_odukp_type_t odu_container,
                                      util_global_odu_line_payload_t payload_format,
                                      BOOL prov_rfrm_tfrm_cfg,
                                      BOOL prov_dummy_schd);
PUBLIC BOOL8 coreotn_ho_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                      UINT32 chnl_id,
                                      BOOL8  is_rx_side,
                                      coreotn_mux_stage_t mux_stage,
                                      util_global_odu_line_payload_t payload_format,
                                      util_global_chnl_state_t state);
PUBLIC BOOL8 coreotn_lo_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                      UINT32 chnl_id,
                                      BOOL8  is_rx_side,
                                      coreotn_mux_stage_t mux_stage,
                                      util_global_odu_line_payload_t payload_format,
                                      UINT32 tx_trib_slot_mask[3],
                                      UINT32 rx_trib_slot_mask[3],
                                      util_global_chnl_state_t state);

PUBLIC PMC_ERROR coreotn_ho_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                        UINT32 chnl_id,
                                        coreotn_mux_stage_t mux_stage,
                                        BOOL deprov_rfrm_tfrm_cfg,
                                        BOOL deprov_dummy_schd);                                 

PUBLIC PMC_ERROR coreotn_oduksw_chnl_prov(coreotn_handle_t *coreotn_handle,
                                          coreotn_oduksw_input_port_type_t input_port,
                                          UINT32 input_chnl_id,
                                          coreotn_oduksw_output_port_type_t output_port,
                                          UINT32 output_chnl_id,
                                          util_global_odukp_type_t odu_type,
                                          UINT32 num_odu0,
                                          DOUBLE client_bitrate,
                                          UINT32 pkt_size,
                                          UINT32 pkt_per_int, UINT32 pkt_per_n,
                                          UINT32 pkt_per_d,
                                          coreotn_oduksw_xfer_mode_t xfer_mode,
                                          BOOL   hys_buffer_en,
                                          UINT32 hys_buffer,
                                          UINT32 halt_buffer,
                                          UINT32 hdr_removal_size,
                                          UINT32 latency_s,
                                          BOOL8 bmp_map_mode);

PUBLIC BOOL8 coreotn_oduksw_chnl_prov_get(coreotn_handle_t *coreotn_handle,
                                          coreotn_oduksw_input_port_type_t input_port,
                                          UINT32 input_chnl_id,
                                          coreotn_oduksw_output_port_type_t output_port,
                                          UINT32 output_chnl_id,
                                          util_global_chnl_state_t state);

    
PUBLIC PMC_ERROR coreotn_oduksw_chnl_activate(coreotn_handle_t *coreotn_handle,
                                              coreotn_oduksw_input_port_type_t input_port,
                                              UINT32 input_chnl_id,
                                              coreotn_oduksw_output_port_type_t output_port,
                                              UINT32 output_chnl_id);

PUBLIC PMC_ERROR coreotn_oduksw_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                            coreotn_oduksw_input_port_type_t input_port,
                                            UINT32 input_chnl_id,
                                            coreotn_oduksw_output_port_type_t output_port,
                                            UINT32 output_chnl_id);

PUBLIC PMC_ERROR coreotn_oduksw_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                                coreotn_oduksw_input_port_type_t input_port,
                                                UINT32 input_chnl_id,
                                                coreotn_oduksw_output_port_type_t output_port,
                                                UINT32 output_chnl_id);

PUBLIC PMC_ERROR coreotn_oduksw_chnl_switch(coreotn_handle_t *coreotn_handle,
                                            coreotn_oduksw_output_port_type_t output_port,
                                            UINT32 output_chnl_id);

PUBLIC PMC_ERROR coreotn_oduksw_fifo_var_data_and_chnl_state_reset( coreotn_handle_t *coreotn_handle,
                                                                    UINT32 ingress_mux_input_port,
                                                                    UINT32 ingress_mux_input_port_chnl,
                                                                    UINT32 egress_mux_output_port,
                                                                    UINT32 egress_mux_output_port_chnl );   
                                                                                                                 
PUBLIC PMC_ERROR coreotn_ho_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                            UINT32 chnl_id,
                                            coreotn_mux_stage_t mux_stage);
PUBLIC PMC_ERROR coreotn_ho_chnl_activate(coreotn_handle_t *coreotn_handle,
                                          UINT32 chnl_id,
                                          coreotn_mux_stage_t mux_stage);
PUBLIC BOOL coreotn_oduksw_port_init_state_get(coreotn_handle_t *coreotn_handle,
                                               UINT32 port_type,
                                               UINT32 port_id);
PUBLIC PMC_ERROR coreotn_oduksw_input_port_init(coreotn_handle_t *coreotn_handle,
                                                util_global_oduk_port_t input_port,
                                                coreotn_oduksw_xfer_mode_t oduksw_xfer_mode,
                                                UINT32 hdr_removal_size);
PUBLIC PMC_ERROR coreotn_oduksw_output_port_init(coreotn_handle_t *coreotn_handle,
                                                 util_global_oduk_port_t output_port,
                                                 UINT32 oduksw_xfer_mode,
                                                 UINT32 mode);
PUBLIC PMC_ERROR coreotn_oduksw_chnl_source_and_type_test(coreotn_handle_t *coreotn_handle,
                                                          UINT32 egress_mux_output_port,
                                                          UINT32 egress_mux_output_port_chnl,
                                                          UINT32 *ingress_mux_input_port,
                                                          UINT32 *ingress_mux_input_port_chnl,
                                                          BOOL *is_broadcast,
                                                          BOOL *is_multicast);

PUBLIC void coreotn_oduksw_connect_data_state_get(coreotn_handle_t *coreotn_handle,
                                                  util_global_cpb_connect_t **connect_data_pptr);

PUBLIC PMC_ERROR coreotn_oduksw_chnl_type_get(coreotn_handle_t *coreotn_handle,
                                              UINT32 egress_mux_output_port,
                                              UINT32 egress_mux_output_port_chnl,
                                              BOOL*   is_primary,
                                              BOOL*   is_zone_master,
                                              BOOL*   is_broadcast,
                                              BOOL*   is_multicast );

PUBLIC PMC_ERROR coreotn_oduksw_egress_mux_input_init(coreotn_handle_t *coreotn_handle,
                                                      util_global_oduk_port_t output_port,
                                                      UINT32 oduksw_xfer_mode,
                                                      UINT32 mode);                                             

PUBLIC PMC_ERROR coreotn_oduksw_queue_resize_wrapper(coreotn_handle_t *coreotn_handle,
                                                     UINT32 port,
                                                     UINT32 chnl,
                                                     UINT32 num_blocks);

PUBLIC PMC_ERROR coreotn_oduksw_sifd_path_cfg(coreotn_handle_t *coreotn_handle,
                                              coreotn_init_operation_t init_operation);  
                                  
PUBLIC PMC_ERROR coreotn_db_full_cal_entries_get(coreotn_handle_t * coreotn_handle,
                                                 BOOL rx_db,
                                                 odu_struct_frm_id_t frm_id,
                                                 UINT32 *cal_entry_ptr,
                                                 UINT32 *num_cal_entries);
            
PUBLIC PMC_ERROR coreotn_oduksw_db_entry_get(coreotn_handle_t *coreotn_handle,
                                             coreotn_oduksw_output_port_type_t output_port,
                                             UINT32 schd_addr,
                                             UINT32 *egress_mux_output_port_ptr,
                                             UINT32 *egress_mux_output_chnl_ptr,
                                             UINT32 *rd_channel_id_ptr);

PUBLIC PMC_ERROR coreotn_oduksw_dcs_db_num_entry_get(coreotn_handle_t *coreotn_handle,
                                                     coreotn_oduksw_output_port_type_t output_port,
                                                     UINT32 egress_mux_output_chnl,
                                                     UINT32 *num_entries);                                                                              
PUBLIC PMC_ERROR coreotn_chnl_id_and_order_get(coreotn_handle_t* coreotn_handle,
                                               UINT32 output_chnl_id,
                                               UINT32 db_inst,
                                               UINT32* chnl_id,
                                               UINT32* ho_chnl_id,
                                               coreotn_chnl_order_t* order_type,
                                               UINT32* num_of_recs,
                                               line_core_otn_db_query_res** q_result);
                                               
PUBLIC PMC_ERROR coreotn_fo2_mpmo_ch_activate(coreotn_handle_t *coreotn_handle,
                                              UINT32 output_chnl_id);  
                                              
PUBLIC PMC_ERROR coreotn_fo2_mpmo_ch_deactivate(coreotn_handle_t *coreotn_handle,
                                                UINT32 output_chnl_id);                                              
                                              
PUBLIC PMC_ERROR coreotn_oduksw_chnl_hole_inst_head_cfg(coreotn_handle_t *coreotn_handle,
                                                        UINT32 egress_mux_output_port,
                                                        UINT32 egress_mux_output_port_chnl,
                                                        UINT32 hole_inst_sop_size);                                                                                           

PUBLIC PMC_ERROR coreotn_odtu_mux_lo_chnl_activate_only(coreotn_handle_t *coreotn_handle,
                                                        UINT32 chnl_id);
                                                        
PUBLIC PMC_ERROR coreotn_odtu_mux_lo_chnl_deactivate_only(coreotn_handle_t *coreotn_handle, 
                                                          UINT32 chnl_id);                                                        

PUBLIC PMC_ERROR coreotn_odtu_dmx_dummy_schd_prov_num_get(coreotn_handle_t *coreotn_handle,
                                                          UINT32 *num_dummy_schd);  
                                                          
PUBLIC PMC_ERROR coreotn_fo1_mpmo_scbs3_dummy_cycle_remove(coreotn_handle_t *coreotn_handle,
                                                           UINT32 chnl_id);                                    
                                                  
PUBLIC PMC_ERROR coreotn_odtu_mux_empty_ho_unlock(coreotn_handle_t *coreotn_handle,
                                                  util_global_odu_level_t chnl_level,
                                                  UINT32 lo_chnl_id);                                           

PUBLIC PMC_ERROR coreotn_odtu_dmx_dummy_schd_get(coreotn_handle_t *coreotn_handle,
                                                 UINT32 dmx_schd_idx,
                                                 UINT8 *mo_chnl_id,
                                                 UINT8 *calendar_entry);                                                                                                                                     
PUBLIC PMC_ERROR coreotn_msi_cfg_mode_set(coreotn_handle_t *coreotn_handle,
                                          util_global_otn_msi_cfg_mode_t msi_mode);  
                                          
PUBLIC PMC_ERROR coreotn_msi_cfg_mode_get(coreotn_handle_t *coreotn_handle,
                                          util_global_otn_msi_cfg_mode_t *msi_mode);  

PUBLIC PMC_ERROR coreotn_ts_port_ctxt_update(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id,
                                             UINT32 tx_trib_port,
                                             UINT32 rx_trib_port,
                                             coreotn_mux_stage_t mux_stage);                                                                                  
                                          
PUBLIC PMC_ERROR coreotn_rx_lo_chnl_offline_activate_all(coreotn_handle_t *coreotn_handle,
                                                         UINT32 ho_chnl_id,
                                                         coreotn_mux_stage_t mux_stage);
                                                         
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_offline_activate_all(coreotn_handle_t *coreotn_handle,
                                                         UINT32 ho_chnl_id,
                                                         coreotn_mux_stage_t mux_stage);  

PUBLIC PMC_ERROR coreotn_mux_dmx_ho_rst_cfg(coreotn_handle_t* coreotn_handle,
                                            coreotn_fmf_inst_t fmf_inst,
                                            UINT32 chnl_id,
                                            BOOL8 enable); 
                                            
PUBLIC PMC_ERROR coreotn_mpmo_scbs3_all_chnls_activate(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mpmo_inst_t mpmo_inst);  
                                                       
PUBLIC PMC_ERROR coreotn_mpma_scbs3_all_chnls_activate(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mpma_inst_t mpma_inst);
                                                       
PUBLIC PMC_ERROR coreotn_mpmo_scbs3_all_chnls_deactivate(coreotn_handle_t *coreotn_handle,
                                                         coreotn_mpmo_inst_t mpmo_inst);   
                                                         
PUBLIC PMC_ERROR coreotn_mpma_scbs3_all_chnls_deactivate(coreotn_handle_t *coreotn_handle,
                                                         coreotn_mpma_inst_t mpma_inst);                                                                                                             
                                                       
PUBLIC PMC_ERROR coreotn_rx_lo_chnl_offline_deprov_all(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT32 ho_chnl_id,
                                                       UINT32 *local_lo_chnl_ids,
                                                       UINT32 num_lo_deprov);
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_offline_deprov_all(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT32 ho_chnl_id,
                                                       UINT32 *local_lo_chnl_ids,
                                                       UINT32 num_lo_deprov); 

PUBLIC PMC_ERROR coreotn_oduksw_dcs_swap_and_update_all(coreotn_handle_t *coreotn_handle,
                                                        UINT32 dcs_inst,
                                                        BOOL8* is_dcs_updated_ptr,
                                                        BOOL8 is_dcs_entry_updated_arr[]);

PUBLIC PMC_ERROR coreotn_tx_lo_chnl_offline_deactivate_all(coreotn_handle_t *coreotn_handle,
                                                           UINT32 ho_chnl_id,
                                                           coreotn_mux_stage_t mux_stage);
                                                           
PUBLIC PMC_ERROR coreotn_rx_lo_chnl_offline_deactivate_all(coreotn_handle_t *coreotn_handle,
                                                           UINT32 ho_chnl_id,
                                                           coreotn_mux_stage_t mux_stage);   
                                                                                                                                                                                                                                                                            
PUBLIC PMC_ERROR coreotn_rx_mo_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id);          
                                                                                                                                                                                                                                                                  
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_ids_get(coreotn_handle_t *coreotn_handle,
                                            coreotn_mux_stage_t mux_stage,
                                            UINT32 ho_chnl_id,
                                            UINT32 *new_lo_chnl_ids,
                                            UINT32 *num_new_lo);                                                                                                                                                                                                                                                                  
                                                                                                              
/*
** COREOETN PRBS APIs
*/ 
PUBLIC PMC_ERROR coreotn_prbs_chnl_init(coreotn_handle_t *coreotn_handle,
                                        coreotn_prbs_inst_t prbs_inst, 
                                        UINT32 chnl, 
                                        UINT32 dci_chnl,
                                        UINT32 prbs_operation,
                                        UINT32 mode);
PUBLIC PMC_ERROR coreotn_prbs_pattern_cfg(coreotn_handle_t *coreotn_handle,
                                          coreotn_prbs_inst_t prbs_inst,
                                          UINT32 chnl, UINT32 dci_chnl, 
                                          coreotn_prbs_pattern_t prbs_pattern);
PUBLIC PMC_ERROR coreotn_prbs_ber_thres_cfg(coreotn_handle_t *coreotn_handle,
                                            coreotn_prbs_inst_t prbs_inst,
                                            UINT32 prbs_ber_lo_thres, 
                                            UINT32 prbs_ber_hi_thres,
                                            UINT16 prbs_reseed_thr,
                                            UINT16 prbs_reseed_interval);
PUBLIC PMC_ERROR coreotn_prbs_chnl_bit_err_inject(coreotn_handle_t *coreotn_handle,
                                                  coreotn_prbs_inst_t prbs_inst,
                                                  UINT32 chnl, UINT32 insert_err);
PUBLIC PMC_ERROR coreotn_prbs_chnl_defect_cnt_get(coreotn_handle_t *coreotn_handle,
                                                  coreotn_prbs_inst_t prbs_inst, 
                                                  UINT32 chnl, UINT32 *defect_cnt);
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR coreotn_prbs_dlss_status_get(coreotn_handle_t* coreotn_handle,
                                              coreotn_prbs_inst_t prbs_inst, 
                                              UINT32 prbs_chnl, 
                                              UINT8 *dlss_stat);     
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/
PUBLIC PMC_ERROR coreotn_prbs_pattern_get(coreotn_handle_t *coreotn_handle,
                                          coreotn_prbs_inst_t prbs_inst,
                                          UINT32 chnl, UINT32 dci_chnl, 
                                          coreotn_prbs_pattern_t *prbs_pattern);                                                                                           
/*
** COREOTN channel state accessor functions
*/

PUBLIC void coreotn_payload_format_get(coreotn_handle_t        *coreotn_handle,
                                       util_global_odu_level_t chnl_level,
                                       BOOL direction,
                                       UINT32 chnl_id,
                                       util_global_odu_line_payload_t *payload_format);

PUBLIC PMC_ERROR coreotn_lo_chnl_state_get(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl, 
                                           UINT32 direction,
                                           coreotn_chnl_state_t *chnl_state);

PUBLIC PMC_ERROR coreotn_mo_chnl_state_get(coreotn_handle_t *coreotn_handle,
                                           UINT32 mo_channel,
                                           UINT32 direction, 
                                           coreotn_chnl_state_t *chnl_state); 

PUBLIC util_global_chnl_state_t coreotn_odtu_chnl_state_get(coreotn_handle_t *coreotn_handle,
                                                            coreotn_chnl_order_t ho_odu_level,
                                                            UINT32 chnl_id,
                                                            BOOL8 rx_dir);
                                           

PUBLIC PMC_ERROR coreotn_lo_chnl_state_set(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl, UINT32 direction,
                                           coreotn_chnl_state_t chnl_state);
                                           
PUBLIC PMC_ERROR coreotn_mo_chnl_state_set(coreotn_handle_t *coreotn_handle,
                                           UINT32 chnl, UINT32 direction,
                                           coreotn_chnl_state_t chnl_state);                                                                                      
                                              
PUBLIC PMC_ERROR coreotn_rx_ho_chnl_prov(coreotn_handle_t *coreotn_handle,
                                         UINT32 chnl_id,
                                         coreotn_mux_stage_t mux_stage,
                                         util_global_odukp_type_t odu_container,
                                         util_global_odu_line_payload_t payload_format,
                                         BOOL prov_rfrm_tfrm_cfg);

PUBLIC PMC_ERROR coreotn_rx_lo_chnl_prov(coreotn_handle_t *coreotn_handle,
                                         UINT32 chnl_id,
                                         coreotn_mux_stage_t mux_stage,
                                         util_global_odukp_type_t lo_odu_type,
                                         UINT32 odu_flex_rate,
                                         DOUBLE client_rate,
                                         coreotn_mapping_type_t mapping_mode,
                                         UINT32 tribslot_mask[3],
                                         UINT32 trib_port,
                                         util_global_odu_line_payload_t payload_format,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 ho_chnl_id,
                                         BOOL prov_rfrm);
                                              
PUBLIC PMC_ERROR coreotn_tx_ho_chnl_prov(coreotn_handle_t *coreotn_handle,
                                         UINT32 chnl_id,
                                         coreotn_mux_stage_t mux_stage,
                                         util_global_odukp_type_t odu_container,
                                         util_global_odu_line_payload_t payload_format,
                                         BOOL prov_rfrm_tfrm_cfg,
                                         BOOL prov_dummy_schd);

PUBLIC PMC_ERROR coreotn_tx_dummy_schd_prov(coreotn_handle_t *coreotn_handle,
                                            UINT32 chnl_id);

PUBLIC PMC_ERROR coreotn_tx_lo_chnl_prov(coreotn_handle_t *coreotn_handle,
                                         UINT32 chnl_id,
                                         coreotn_mux_stage_t mux_stage,
                                         util_global_odukp_type_t lo_odu_type,
                                         UINT32 odu_flex_rate,
                                         coreotn_mapping_type_t mapping_mode,
                                         UINT32 tribslot_mask[3],
                                         UINT32 trib_port,
                                         util_global_odu_line_payload_t payload_format,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 ho_chnl_id,
                                         DOUBLE client_rate,
                                         BOOL prov_tfrm);
PUBLIC PMC_ERROR coreotn_rx_ho_chnl_deprov( coreotn_handle_t *coreotn_handle,
                                            UINT32 chnl_id,
                                            coreotn_mux_stage_t mux_stage,
                                            BOOL deprov_rfrm_tfrm_cfg);

PUBLIC PMC_ERROR coreotn_tx_ho_chnl_deprov( coreotn_handle_t *coreotn_handle,
                                            UINT32 chnl_id,
                                            coreotn_mux_stage_t mux_stage,
                                            BOOL deprov_rfrm_tfrm_cfg,
                                            BOOL deprov_dummy_schd);

PUBLIC PMC_ERROR coreotn_tx_dummy_schd_deprov(coreotn_handle_t *coreotn_handle,
                                              UINT32 chnl_id);

PUBLIC PMC_ERROR coreotn_rx_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                           coreotn_mux_stage_t mux_stage,
                                           UINT32 chnl_id,
                                           UINT32 ho_chnl_id,
                                           BOOL deprov_rfrm);                                                                                       
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                           coreotn_mux_stage_t mux_stage,
                                           UINT32 chnl_id,
                                           UINT32 ho_chnl_id,
                                           BOOL deprov_ho_cfg);
PUBLIC PMC_ERROR coreotn_rx_ho_chnl_activate(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id,
                                             coreotn_mux_stage_t mux_stage);

PUBLIC PMC_ERROR coreotn_tx_ho_chnl_activate(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id,
                                             coreotn_mux_stage_t mux_stage);

PUBLIC PMC_ERROR coreotn_tx_ho_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id,
                                             coreotn_mux_stage_t mux_stage);
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_activate(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id, UINT32 ho_chnl_id,
                                             coreotn_mux_stage_t mux_stage);
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id, UINT32 ho_chnl_id,
                                               coreotn_mux_stage_t mux_stage);

PUBLIC PMC_ERROR coreotn_rx_lo_chnl_activate(coreotn_handle_t *coreotn_handle,
                                             UINT32 chnl_id, UINT32 ho_chnl_id,
                                             coreotn_mux_stage_t mux_stage,
                                             BOOL activate_rfrm);
PUBLIC PMC_ERROR coreotn_rx_lo_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id, UINT32 ho_chnl_id,
                                               coreotn_mux_stage_t mux_stage);
PUBLIC PMC_ERROR coreotn_rx_ho_chnl_deactivate(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl_id,
                                               coreotn_mux_stage_t mux_stage);
                                               
PUBLIC PMC_ERROR coreotn_tx_lo_chnl_offline_update(coreotn_handle_t *coreotn_handle,
                                                   UINT32 chnl_id,
                                                   coreotn_mux_stage_t mux_stage,
                                                   util_global_odukp_type_t lo_odu_type,
                                                   UINT32 odu_flex_rate,
                                                   coreotn_mapping_type_t mapping_mode,
                                                   UINT32 tribslot_mask[3],
                                                   UINT32 trib_port,
                                                   util_global_odu_line_payload_t payload_format,
                                                   util_global_odukp_type_t ho_odu_type,
                                                   UINT32 ho_chnl_id,
                                                   UINT32 client_rate);

PUBLIC PMC_ERROR coreotn_tx_ho_chnl_offline_update(coreotn_handle_t *coreotn_handle,
                                                   UINT32 chnl_id,
                                                   coreotn_mux_stage_t mux_stage,
                                                   util_global_odukp_type_t odu_container,
                                                   util_global_odu_line_payload_t payload_format);

PUBLIC PMC_ERROR coreotn_rx_lo_chnl_offline_update(coreotn_handle_t *coreotn_handle,
                                                   UINT32 chnl_id,
                                                   coreotn_mux_stage_t mux_stage,
                                                   util_global_odukp_type_t lo_odu_type,
                                                   UINT32 odu_flex_rate,
                                                   DOUBLE client_rate,
                                                   coreotn_mapping_type_t mapping_mode,
                                                   UINT32 tribslot_mask[3],
                                                   UINT32 trib_port,
                                                   util_global_odu_line_payload_t payload_format,
                                                   util_global_odukp_type_t ho_odu_type,
                                                   UINT32 ho_chnl_id);

PUBLIC PMC_ERROR coreotn_rx_ho_chnl_offline_update(coreotn_handle_t *coreotn_handle,
                                                   UINT32 chnl_id,
                                                   coreotn_mux_stage_t mux_stage,
                                                   util_global_odukp_type_t odu_container,
                                                   util_global_odu_line_payload_t payload_format);

PUBLIC PMC_ERROR coreotn_oif_pkt_period_calc(coreotn_handle_t *coreotn_handle,
                                             DOUBLE odu_bitrate,
                                             UINT32 pkt_size,
                                             UINT32 header_size,
                                             UINT32 *pkt_size_ptr,
                                             UINT32 *pkt_per_int_ptr,
                                             UINT32 *pkt_per_n_ptr,
                                             UINT32 *pkt_per_d_ptr,
                                             DOUBLE *eps_ptr);
PUBLIC PMC_ERROR coreotn_pkt_period_calc(coreotn_handle_t *coreotn_handle,
                                         DOUBLE odu_bitrate,
                                         UINT32 pkt_size,
                                         UINT32 pkt_size_range,
                                         util_opsa_cn_frame_period_t cn_frame_period,
                                         util_opsa_pkt_period_calc_t pkt_period_calc,
                                         UINT32 *pkt_per_int_ptr,
                                         UINT32 *pkt_per_n_ptr,
                                         UINT32 *pkt_per_d_ptr,
                                         UINT32 *pkt_size_ptr,
                                         DOUBLE *epsilon_ptr);

PUBLIC PMC_ERROR coreotn_min_eps_pkt_period_calc(coreotn_handle_t *coreotn_handle,
                                                 DOUBLE odu_bitrate,
                                                 UINT32 *pkt_per_int_ptr,
                                                 UINT32 *pkt_per_n_ptr,
                                                 UINT32 *pkt_per_d_ptr,
                                                 UINT32 *pkt_size_ptr,
                                                 DOUBLE *epsilon_ptr);

PUBLIC PMC_ERROR coreotn_mpmo_ckctl_cfg(coreotn_handle_t *coreotn_handle,
                                        coreotn_mpmo_inst_t mpmo_instance,
                                        UINT32 ckctl_port, 
                                        UINT32 chnl,
                                        BOOL enable);   

PUBLIC PMC_ERROR coreotn_mpmo_data_get(coreotn_handle_t *coreotn_handle,
                                       coreotn_mpmo_inst_t mpmo_instance,
                                       UINT32 chnl,
                                       UINT32 *pkt_size_ptr,
                                       UINT32 *pkt_per_int_ptr,
                                       UINT32 *pkt_per_n_ptr,
                                       UINT32 *pkt_per_d_ptr); 
                                       
PUBLIC PMC_ERROR coreotn_dci_mux3_pub_cfg(coreotn_handle_t *coreotn_handle,
                                          coreotn_fmf_inst_t fmf_inst,
                                          coreotn_dci_mux3_src_t dci_mux);
                                         
PUBLIC PMC_ERROR coreotn_dci_mux4_xoff_cfg(coreotn_handle_t *coreotn_handle,
                                           coreotn_fmf_inst_t fmf_inst,
                                           coreotn_dci_mux4_src_t dci_mux, 
                                           coreotn_xoff_sel_t xoff);
                                           
PUBLIC PMC_ERROR coreotn_oduksw_odu_payload_format_validate(coreotn_handle_t* coreotn_handle,
                                                            util_global_oduk_port_data_t *local_output_pc_ptr,
                                                            BOOL8 *valid_payload);                                           
                                        
/* DB accessors */
PUBLIC PMC_ERROR coreotn_oduksw_db_entry_set(coreotn_handle_t *coreotn_handle,
                                             coreotn_oduksw_output_port_type_t output_port,
                                             UINT32 schd_addr,
                                             UINT32 output_chnl_id);

#if defined PMC_EMULATION | defined PMC_SW_SIMULATION
PUBLIC PMC_ERROR coreotn_db_entry_set(coreotn_handle_t *coreotn_handle,
                                      UINT32 chnl_id,
                                      UINT32 cal_slot, UINT32 trib_slot);

PUBLIC PMC_ERROR coreotn_db_ho_chnl_cal_schd_add(coreotn_handle_t *coreotn_handle,
                                                 UINT32 chnl_id,
                                                 UINT32 cal_slot_mask[3]);
#endif /* PMC_EMULATION | PMC_SW_SIMULATION */

PUBLIC PMC_ERROR coreotn_db_first_cal_entry_get(coreotn_handle_t *coreotn_handle,
                                                coreotn_chnl_order_t chnl_level,
                                                UINT32 direction,
                                                UINT32 chnl_id,
                                                UINT32 *cal_entry);
PUBLIC PMC_ERROR coreotn_db_ho_chnl_entry_get(coreotn_handle_t *coreotn_handle,
                                              UINT32 cal_slot,
                                              UINT32 *chnl_id);
PUBLIC PMC_ERROR coreotn_db_ho_cal_entry_all_get(coreotn_handle_t *coreotn_handle,
                                                 UINT32 chnl_id,
                                                 UINT32 *cal_entry_ptr,
                                                 UINT32 *num_cal_entries);
PUBLIC PMC_ERROR coreotn_db_ho_entry_clear(coreotn_handle_t *coreotn_handle,
                                           util_gen_db_id_t db_id, UINT32 chnl_id);
PUBLIC PMC_ERROR coreotn_db_mo_entry_clear(coreotn_handle_t *coreotn_handle,
                                           util_gen_db_id_t db_id, UINT32 chnl_id,
                                           UINT32 ho_chnl_id);                                           
PUBLIC PMC_ERROR coreotn_db_lo_entry_clear(coreotn_handle_t *coreotn_handle,
                                           util_gen_db_id_t db_id, UINT32 chnl_id,
                                           UINT32 ho_chnl_id);   
PUBLIC PMC_ERROR coreotn_oduksw_local_db_entry_set(coreotn_handle_t *coreotn_handle,
                                                   UINT32 input_chnl_id,
                                                   coreotn_oduksw_output_port_type_t output_port,
                                                   UINT32 output_chnl_id);     
PUBLIC PMC_ERROR coreotn_db_ho_entry_update(coreotn_handle_t *coreotn_handle,
                                            util_gen_db_id_t db_id, UINT32 chnl_id);
PUBLIC PMC_ERROR coreotn_db_mo_entry_update(coreotn_handle_t *coreotn_handle,
                                            util_gen_db_id_t db_id, UINT32 chnl_id);
PUBLIC PMC_ERROR coreotn_db_lo_entry_update(coreotn_handle_t *coreotn_handle,
                                            util_gen_db_id_t db_id, UINT32 chnl_id);  
PUBLIC PMC_ERROR coreotn_oduksw_db_entry_clear(coreotn_handle_t *coreotn_handle,
                                               coreotn_oduksw_output_port_type_t output_port,
                                               UINT32 output_chnl_id);
PUBLIC PMC_ERROR coreotn_db_mo_cal_entry_all_get(coreotn_handle_t *coreotn_handle,
                                                 UINT32 chnl_id,
                                                 UINT8 db_inst,
                                                 UINT32 *cal_entry_ptr,
                                                 UINT32 *num_cal_entries); 
PUBLIC PMC_ERROR coreotn_db_lo_cal_entry_all_get(coreotn_handle_t *coreotn_handle,
                                                 UINT32 chnl_id,
                                                 UINT8  db_inst,
                                                 UINT32 *cal_entry_ptr,
                                                 UINT32 *num_cal_entries);
PUBLIC PMC_ERROR coreotn_db_ts_port_cal_entry_get(coreotn_handle_t *coreotn_handle,
                                                  UINT8 db_inst,
                                                  odu_struct_odu_level_t odu_level,
                                                  UINT32 chnl_id,
                                                  UINT32 ts_port,
                                                  UINT32 *cal_entry_ptr,
                                                  UINT32 *num_cal_entries);
PUBLIC PMC_ERROR coreotn_db_ts_port_to_lo_id_get(coreotn_handle_t *coreotn_handle,
                                                 UINT8 db_inst,
                                                 odu_struct_odu_level_t odu_level,
                                                 UINT32 chnl_id,
                                                 UINT32 ts_port,
                                                 UINT32 *cal_entry_ptr,
                                                 UINT32 *num_cal_entries,
                                                 UINT32 *lo_chnl_id);                                                  
PUBLIC PMC_ERROR coreotn_db_cal_entry_to_ts_port_get(coreotn_handle_t *coreotn_handle,
                                                     UINT8 db_inst,
                                                     odu_struct_odu_level_t odu_level,
                                                     UINT32 chnl_id,
                                                     UINT32 *cal_entry_ptr,
                                                     UINT32 *ts_port_ptr,
                                                     UINT32 *num_cal_entries,
                                                     UINT32 *num_active_ts_port);  
PUBLIC PMC_ERROR coreotn_db_ho_chnl_id_get(coreotn_handle_t *coreotn_handle,
                                           UINT32 direction,
                                           util_global_odu_level_t chnl_level,
                                           UINT32 lo_chnl_id,
                                           UINT32 *ho_chnl_id);

PUBLIC PMC_ERROR coreotn_db_ho_cal_entries_mo_lo_chnl_get(coreotn_handle_t *coreotn_handle,
                                                          UINT32 chnl_id,
                                                          BOOL   db_id,
                                                          UINT32 *cal_entry_ptr,
                                                          UINT32 *num_cal_entries,
                                                          UINT32 *ho_chnl_id,
                                                          UINT32 *mo_chnl_id,
                                                          UINT32 *lo_chnl_id); 

PUBLIC PMC_ERROR coreotn_db_mo_cal_entries_lo_chnl_get(coreotn_handle_t *coreotn_handle,
                                                          UINT32 chnl_id,
                                                          BOOL   db_inst,
                                                          UINT32 *cal_entry_ptr,
                                                          UINT32 *num_cal_entries,
                                                          UINT32 *mo_chnl_id,
                                                          UINT32 *lo_chnl_id);
                                                  
PUBLIC PMC_ERROR coreotn_db_ret_sched_populate(coreotn_handle_t* coreotn_handle,
                                               coreotn_oduksw_output_port_type_t output_port,
                                               UINT32 output_chnl_id,
                                               UINT32* ho_chnl_id,
                                               coreotn_chnl_order_t* order_type);                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
PUBLIC PMC_ERROR coreotn_oduksw_num_blocks_get(coreotn_handle_t *coreotn_handle,
                                               UINT32 output_port,
                                               UINT32 output_chnl_id,
                                               UINT32 *num_current_blocks);
                                               
PUBLIC coreotn_chnl_state_t coreotn_oduksw_port_chnl_state_get(coreotn_handle_t *coreotn_handle,
                                                                UINT32 chnl_id,
                                                                UINT32 direction,
                                                                util_global_oduk_port_t port_type);                                               


PUBLIC PMC_ERROR coreotn_mpmo_debug_data_get(coreotn_handle_t *coreotn_handle,
                                             mpmo_type_id_t mpmo_type,
                                             UINT32 chnl,
                                             UINT32 *debug_latency_ptr,
                                             UINT32 *debug_ramp_state_ptr,
                                             UINT32 *debug_curr_csi_ptr);

PUBLIC PMC_ERROR coreotn_mpmo_raw_latency_get(coreotn_handle_t *coreotn_handle,
                                              mpmo_type_id_t mpmo_type,
                                              UINT32 chnl,
                                              UINT32 *minimum_raw_latency,
                                              UINT32 *maximum_raw_latency);

PUBLIC PMC_ERROR coreotn_validate_tribslot_mask(coreotn_handle_t* coreotn_handle,
                                                 util_global_odukp_type_t lo_odu_type,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 UINT32 tribslot_mask[3],
                                                 UINT32 ho_chnl_id,
                                                 coreotn_mux_stage_t mux_stage,
                                                 BOOL rx_segment);
                                         
PUBLIC PMC_ERROR odu_struct_deg_m_set(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                      UINT8 deg_m,
                                      odu_struct_target_t target);

PUBLIC PMC_ERROR odu_struct_deg_th_set(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       UINT32 deg_th,
                                       odu_struct_target_t target);
PUBLIC BOOL8 coreotn_is_ddeg_hw_mode(coreotn_handle_t  *coreotn_handle);
PUBLIC PMC_ERROR odu_struct_ddeg_mode_set(coreotn_handle_t    *coreotn_handle,
                                          coreotn_ddeg_mode_t  ddeg_mode,
                                          BOOL8                enable,
                                          UINT8                odu_segment);

PUBLIC PMC_ERROR coreotn_ddeg_mode_get(coreotn_handle_t    *coreotn_handle,
                                       coreotn_ddeg_mode_t  ddeg_mode,
                                       BOOL8               *ddeg_enabled);   

PUBLIC PMC_ERROR coreotn_ddeg_status_set(coreotn_handle_t     *coreotn_handle,
                                         coreotn_ddeg_mode_t   ddeg_mode,
                                         BOOL8                 activate);

PUBLIC PMC_ERROR coreotn_ddeg_mon_segment_set(coreotn_handle_t *coreotn_handle,
                                              coreotn_ddeg_mon_seg_t mon_seg);
                                              
PUBLIC PMC_ERROR coreotn_ddeg_mon_segment_get(coreotn_handle_t *coreotn_handle,
                                              coreotn_ddeg_mon_seg_t *mon_seg);

PUBLIC cpb_handle_t * coreotn_oduksw_handle_get(coreotn_handle_t* coreotn_handle);

PUBLIC enet_fege_handle_t * coreotn_enet_fege_handle_get(coreotn_handle_t* coreotn_handle,
                                                         util_global_coreotn_sgmii_inst_t inst);
/*
 * DI/RI RCP 
 */
PUBLIC PMC_ERROR coreotn_odu_rcp_init(coreotn_handle_t *coreotn_handle,
                                      odu_rcp_inst_t odu_rcp_id,
                                      odu_rcp_init_cfg_t* prov_cfg_ptr);

PUBLIC PMC_ERROR coreotn_odu_rcp_uninit(coreotn_handle_t *coreotn_handle,
                                        odu_rcp_inst_t odu_rcp_id);
PUBLIC PMC_ERROR coreotn_odu_rcp_init_get(coreotn_handle_t *coreotn_handle,
                                          odu_rcp_inst_t odu_rcp_id,
                                          odu_rcp_init_cfg_t* prov_cfg_ptr,
                                          BOOL *enable);

/*
 * OHP
 */
PUBLIC PMC_ERROR coreotn_ohp_init(coreotn_handle_t *hndl, 
                                  odu_ohp_cfg_t *cfg);
PUBLIC PMC_ERROR coreotn_ohp_init_get(coreotn_handle_t *coreotn_handle,
                                      odu_ohp_inst_t ohp,
                                      odu_ohp_cfg_t *cfg_ptr,
                                      BOOL *enable);

PUBLIC PMC_ERROR coreotn_ohp_cfg(coreotn_handle_t *coreotn_handle, 
                                 BOOL enable);


PUBLIC PMC_ERROR coreotn_ohp_uninit(coreotn_handle_t *hndl, 
                                    odu_ohp_inst_t ohp);

PUBLIC PMC_ERROR coreotn_ohp_cfg(coreotn_handle_t *coreotn_handle, BOOL enable);

PUBLIC PMC_ERROR coreotn_ohp_ch_cfg(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_ptr, 
                                    UINT16 extract_mask,
                                    UINT16 insert_mask,
                                    UINT32 num_cal_entries,
                                    UINT32 *cal_entries,
                                    BOOL enable);
PUBLIC PMC_ERROR coreotn_ohp_ch_cfg_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_ptr, 
                                        UINT16 *extract_mask,
                                        UINT16 *insert_mask,
                                        BOOL *enable);

PUBLIC PMC_ERROR coreotn_ohp_ch_unidir_cfg(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_ptr, 
                                           UINT16 mask,
                                           UINT32 num_cal_entries,
                                           UINT32 *cal_entries,
                                           BOOL insertion_mode,
                                           BOOL enable);
PUBLIC PMC_ERROR coreotn_ohp_ch_unidir_cfg_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_ptr, 
                                               BOOL insertion_mode,
                                               UINT16 *mask,
                                               BOOL *enable);
PUBLIC PMC_ERROR coreotn_ohp_ch_resize(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_ptr, 
                                       INT32 num_cal_entries);

PUBLIC PMC_ERROR coreotn_db_cal_entries_get(coreotn_handle_t * coreotn_handle,
                                            BOOL sysotn,
                                            BOOL rx_db,
                                            UINT32 ch_id,
                                            odu_struct_frm_id_t frm_id,
                                            UINT32 *cal_entry_ptr,
                                            UINT32 *num_cal_entries);


PUBLIC PMC_ERROR coreotn_stg3b_odu_level_set(coreotn_handle_t       *coreotn_handle,
                                             UINT32                  chnl_id,
                                             odu_struct_odu_level_t  odu_level);

PUBLIC PMC_ERROR coreotn_stg3b_odu_level_get(coreotn_handle_t       *coreotn_handle,
                                             UINT32                  chnl_id,
                                             odu_struct_odu_level_t *odu_level);
                                    
PUBLIC PMC_ERROR coreotn_int_framer_chnl_enable(coreotn_handle_t           *coreotn_handle,
                                         coreotn_int_chnl_info_t    *chnl_info,
                                         coreotn_int_framer_chnl_t  *int_table_ptr,
                                         coreotn_int_framer_chnl_t  *int_en_table_ptr,
                                         BOOL                        enable);

PUBLIC PMC_ERROR coreotn_int_pm_chnl_enable(coreotn_handle_t        *coreotn_handle,
                                            coreotn_int_chnl_info_t *chnl_info_ptr,
                                            BOOL                     enable);

PUBLIC PMC_ERROR coreotn_int_pid_chnl_enable(coreotn_handle_t           *coreotn_handle,
                                             coreotn_int_chnl_info_t    *chnl_info_ptr,
                                             BOOL                        enable);

PUBLIC PMC_ERROR coreotn_int_pid_chnl_tbl_enable(coreotn_handle_t           *coreotn_handle,
                                                 coreotn_int_chnl_info_t    *chnl_info,
                                                 odu_rfrm_int_pid_chnl_t    *int_table_ptr,
                                                 odu_rfrm_int_pid_chnl_t    *int_en_table_ptr,
                                                 BOOL                        enable);

PUBLIC PMC_ERROR coreotn_int_defect_enable(coreotn_handle_t           *coreotn_handle,
                                           coreotn_int_chnl_info_t    *chnl_info_ptr,
                                           BOOL                        enable);

PUBLIC PMC_ERROR coreotn_int_prbs_chnl_enable(coreotn_handle_t          *coreotn_handle,
                                              UINT32                     prbs_chnl,
                                              coreotn_prbs_inst_t        prbs_inst, 
                                              coreotn_int_prbs_chnl_t   *int_table_ptr,
                                              coreotn_int_prbs_chnl_t   *int_en_table_ptr,
                                              BOOL                       enable);

PUBLIC PMC_ERROR coreotn_int_ohp_chnl_enable(coreotn_handle_t          *coreotn_handle,
                                             odu_ohp_inst_t             ohp,
                                             coreotn_int_chnl_info_t   *chnl_info,
                                             odu_ohp_int_chnl_t        *int_table_ptr,
                                             odu_ohp_int_chnl_t        *int_en_table_ptr,
                                             BOOL                       enable);

PUBLIC PMC_ERROR coreotn_int_ohp_chnl_retrieve(coreotn_handle_t           *coreotn_handle,
                                               odu_ohp_inst_t              ohp,
                                               coreotn_int_chnl_info_t    *chnl_info,
                                               odu_ohp_int_chnl_t         *filt_table_ptr,
                                               odu_ohp_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR coreotn_int_ohp_retrieve(coreotn_handle_t           *coreotn_handle,
                                          odu_ohp_inst_t              ohp,
                                          odu_ohp_int_t              *int_table_ptr,
                                          odu_ohp_int_t              *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_int_ohp_enable(coreotn_handle_t     *coreotn_handle,
                                        odu_ohp_inst_t        ohp,
                                        odu_ohp_int_t        *int_table_ptr,
                                        odu_ohp_int_t        *int_en_table_ptr,
                                        BOOL                       enable);

PUBLIC PMC_ERROR coreotn_int_ohp_clear(coreotn_handle_t           *coreotn_handle,
                                       odu_ohp_inst_t              ohp,
                                       odu_ohp_int_t              *int_table_ptr,
                                       odu_ohp_int_t              *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_int_ri_rcp_chnl_enable(coreotn_handle_t           *coreotn_handle,
                                                coreotn_int_chnl_info_t    *chnl_info,
                                                odu_ri_rcp_int_chnl_t      *int_table_ptr,
                                                odu_ri_rcp_int_chnl_t      *int_en_table_ptr,
                                                BOOL                        enable);

PUBLIC PMC_ERROR coreotn_int_ri_rcp_chnl_retrieve(coreotn_handle_t           *coreotn_handle,
                                                  coreotn_int_chnl_info_t    *chnl_info,
                                                  odu_ri_rcp_int_chnl_t      *filt_table_ptr,
                                                  odu_ri_rcp_int_chnl_t      *int_table_ptr);

PUBLIC PMC_ERROR coreotn_int_rcp_retrieve(coreotn_handle_t        *coreotn_handle,
                                          odu_rcp_inst_t           rcp_inst,
                                          odu_rcp_int_t           *int_table_ptr,
                                          odu_rcp_int_t           *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_int_rcp_enable(coreotn_handle_t     *coreotn_handle,
                                        odu_rcp_inst_t        rcp_inst,
                                        odu_rcp_int_t        *int_table_ptr,
                                        odu_rcp_int_t        *int_en_table_ptr,
                                        BOOL                     enable);

PUBLIC PMC_ERROR coreotn_int_rcp_clear(coreotn_handle_t     *coreotn_handle,
                                       odu_rcp_inst_t        rcp_inst,
                                       odu_rcp_int_t        *int_table_ptr,
                                       odu_rcp_int_t        *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_int_summary_get(coreotn_handle_t              *coreotn_handle,
                                         coreotn_int_chnl_info_t       *chnl_info,
                                         odu_rfrm_int_summary_type_t    summary_mask,
                                         UINT32                        *bitmask_ptr);

PUBLIC PMC_ERROR coreotn_int_enable(coreotn_handle_t      *coreotn_handle,
                                    coreotn_int_t         *int_table_ptr,
                                    coreotn_int_t         *int_en_table_ptr,
                                    BOOL                   enable);

PUBLIC PMC_ERROR coreotn_int_retrieve(coreotn_handle_t      *coreotn_handle,
                                      coreotn_int_t         *filt_table_ptr,
                                      coreotn_int_t         *int_table_ptr);

PUBLIC PMC_ERROR coreotn_int_clear(coreotn_handle_t      *coreotn_handle,
                                   coreotn_int_t         *int_table_ptr,
                                   coreotn_int_t         *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_int_enabled_check(coreotn_handle_t      *coreotn_handle,
                                           coreotn_int_t         *int_en_table_ptr,
                                           BOOL                  *int_found_ptr);

PUBLIC PMC_ERROR coreotn_fo2_int_enable(coreotn_handle_t      *coreotn_handle,
                                        coreotn_int_framer_t  *int_table_ptr,
                                        coreotn_int_framer_t  *int_en_table_ptr,
                                        BOOL                   enable);

PUBLIC PMC_ERROR coreotn_fo2_int_retrieve(coreotn_handle_t      *coreotn_handle,
                                          coreotn_int_framer_t  *filt_table_ptr,
                                          coreotn_int_framer_t  *int_table_ptr);

PUBLIC PMC_ERROR coreotn_fo2_int_clear(coreotn_handle_t      *coreotn_handle,
                                       coreotn_int_framer_t  *int_table_ptr,
                                       coreotn_int_framer_t  *int_en_table_ptr);

PUBLIC PMC_ERROR coreotn_fo2_int_enabled_check(coreotn_handle_t      *coreotn_handle,
                                               coreotn_int_framer_t  *int_en_table_ptr,
                                               BOOL                  *int_found_ptr);

PUBLIC PMC_ERROR coreotn_int_chnl_retrieve(coreotn_handle_t           *coreotn_handle,
                                           coreotn_int_chnl_info_t    *chnl_info,
                                           coreotn_int_chnl_t         *filt_table_ptr,
                                           coreotn_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR coreotn_int_chnl_enabled_check(coreotn_handle_t           *coreotn_handle,
                                                coreotn_int_chnl_info_t    *chnl_info,
                                                coreotn_int_framer_chnl_t  *int_en_table_ptr,
                                                coreotn_int_pid_chnl_t     *pid_int_en_table_ptr,
                                                coreotn_int_prbs_chnl_t    *prbs_en_table_ptr,
                                                odu_ohp_int_chnl_t        **ohp_en_table_pptr,
                                                odu_ri_rcp_int_chnl_t      *ri_rcp_en_table_ptr,
                                                BOOL                       *int_found_ptr);

PUBLIC PMC_ERROR coreotn_int_transfer_done_poll(coreotn_handle_t    *coreotn_handle, 
                                                coreotn_latch_defect_t latch_defect);

PUBLIC PMC_ERROR coreotn_int_transfer_done_clr(coreotn_handle_t    *coreotn_handle, 
                                               coreotn_latch_defect_t latch_defect);

PUBLIC PMC_ERROR coreotn_int_start(coreotn_handle_t          *coreotn_handle, 
                                   coreotn_latch_defect_t     latch_defect,
                                   BOOL                       start,
                                   BOOL8                      clear);


PUBLIC PMC_ERROR coreotn_int_chnl_clear(coreotn_handle_t           *coreotn_handle,
                                        coreotn_int_chnl_info_t    *chnl_info,
                                        coreotn_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR coreotn_rfrm_chnl_clear(coreotn_handle_t           *coreotn_handle,
                                         coreotn_int_chnl_info_t    *chnl_info);

PUBLIC BOOL coreotn_int_filt_check(UINT32                          filter_mask,
                                   coreotn_int_filter_blocks_t     block);

PUBLIC PMC_ERROR coreotn_int_top_enabled_check(coreotn_handle_t        *coreotn_handle,
                                               coreotn_int_type_t       int_type,
                                               BOOL                    *int_found_ptr);

PUBLIC PMC_ERROR coreotn_tcm_pid_valid_set(coreotn_handle_t        *coreotn_handle,
                                           odu_struct_odu_level_t   odu_level,
                                           UINT32                   chnl_id,
                                           UINT32                   tcm_pid_slot,
                                           BOOL                     valid);

PUBLIC BOOL coreotn_is_tcm_pid_valid(coreotn_handle_t        *coreotn_handle,
                                     odu_struct_odu_level_t   odu_level,
                                     UINT32                   chnl_id,
                                     UINT32                   tcm_pid_slot);

PUBLIC PMC_ERROR coreotn_adjust_dci_clock(coreotn_handle_t *coreotn_handle,
                                          UINT32 num_mult,
                                          UINT32 denom_mult);

PUBLIC PMC_ERROR coreotn_prbs_chnl_get(coreotn_handle_t    *coreotn_handle,
                                       coreotn_prbs_inst_t  prbs_inst,
                                       UINT32               dci_chnl,
                                       UINT32              *prbs_chnl,
                                       BOOL                *prbs_chnl_valid);

PUBLIC PMC_ERROR coreotn_prbs_mon_or_gen_chnl_get(coreotn_handle_t    *coreotn_handle,
                                                  coreotn_prbs_inst_t  prbs_inst,
                                                  UINT32               dci_chnl,
                                                  UINT32              *prbs_chnl,
                                                  BOOL                *prbs_chnl_valid);

                                              
PUBLIC PMC_ERROR coreotn_prbs_chnl_prov_get(coreotn_handle_t    *coreotn_handle,
                                            coreotn_prbs_inst_t  prbs_inst,
                                            UINT32               prbs_chnl,
                                            BOOL8                *prbs_gen_enbl);                                                                                        
/*
 * Exported PMC Internal Testing Interface
 */



/*
 * Register Access Operations
 */


PUBLIC PMC_ERROR coreotn_ghao_db_chnl_rec_update(coreotn_handle_t *coreotn_handle,
                                                 util_gen_db_id_t db_id,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 ho_chnl_id,
                                                 UINT32 lo_chnl_id,
                                                 BOOL8  new_ts[ODTU_MUX_NUM_CHANL],
                                                 UINT32 old_cal_mask[3],
                                                 UINT32 new_cal_mask[3],
                                                 UINT32 *cal_slots_to_add_remove_num,
                                                 UINT32 cal_slots_to_add_remove[79]);

PUBLIC PMC_ERROR coreotn_ghao_db_chnl_get_masks(coreotn_handle_t *coreotn_handle,
                                                util_gen_db_id_t db_id,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT32 ho_chnl_id,
                                                UINT32 lo_chnl_id,
                                                BOOL8  new_ts[ODTU_MUX_NUM_CHANL],
                                                UINT32 old_cal_mask[3],
                                                UINT32 new_cal_mask[3],
                                                UINT32 *cal_slots_to_add_remove_num,
                                                UINT32 cal_slots_to_add_remove[79]);

PUBLIC PMC_ERROR coreotn_tpid_get(coreotn_handle_t *coreotn_handle,
                                  coreotn_mux_stage_t mux_stage,
                                  UINT32 lo_chnl_id,
                                  UINT8 *tx_tpid,
                                  UINT8 *rx_tpid);

PUBLIC PMC_ERROR coreotn_fo1_bwr_ind_sel_set(coreotn_handle_t *coreotn_handle,
                                             coreotn_mux_stage_t mux_stage);


/* ODUKSW */
PUBLIC PMC_ERROR coreotn_oduksw_ghao_queue_resize_zone_update(coreotn_handle_t *coreotn_handle,
                                                              UINT32 egress_mux_output_port,
                                                              UINT32 egress_mux_output_port_chnl,
                                                              UINT32 halt_buffer,
                                                              UINT32 oduflex_num_ts,
                                                              BOOL increase);   


/* ODTU_MUX */
PUBLIC PMC_ERROR coreotn_mux_ghao_init(coreotn_handle_t *coreotn_handle,
                                       coreotn_mux_stage_t mux_stage,
                                       UINT8 ghao_id,
                                       UINT8 ho_chnl_id,
                                       UINT8 lo_chnl_id,
                                       UINT8 tpid,
                                       BOOL8 cal_slots_add_rem[ODTU_MUX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_add_nack(coreotn_handle_t *coreotn_handle,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_add_ack(coreotn_handle_t *coreotn_handle,
                                               coreotn_mux_stage_t mux_stage,
                                               UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_remove_nack(coreotn_handle_t *coreotn_handle,
                                                   coreotn_mux_stage_t mux_stage,
                                                   UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_remove_ack(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mux_stage_t mux_stage,
                                                  UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_tscc(coreotn_handle_t *coreotn_handle,
                                            coreotn_mux_stage_t mux_stage,
                                            UINT8 ghao_id,
                                            BOOL8 tscc);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_rp(coreotn_handle_t *coreotn_handle,
                                          coreotn_mux_stage_t mux_stage,
                                          UINT8 ghao_id,
                                          BOOL8 rp);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_norm(coreotn_handle_t *coreotn_handle,
                                            coreotn_mux_stage_t mux_stage,
                                            UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_send_idle_nack(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_update_stdby_config(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_wait_config_update_done(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 ho_chnl_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_resize_fifo(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_odtu_mux_xoff_thresh_change(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_instance,
                                                     UINT32 chnl_id,
                                                     UINT32 xoff_lvl);

PUBLIC PMC_ERROR coreotn_odtu_mux_xoff_thresh_update(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_instance,
                                                     UINT32 chnl_id,
                                                     BOOL increase,
                                                     UINT32 *xoff_lvl);

PUBLIC PMC_ERROR coreotn_mux_ghao_recfg_passthru(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ghao_recfg_exp_lo_calen(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_stage,
                                                     UINT8 ghao_id,
                                                     UINT8 cal_slot);

PUBLIC PMC_ERROR coreotn_mux_ghao_end(coreotn_handle_t *coreotn_handle,
                                      coreotn_mux_stage_t mux_stage,
                                      UINT8 ghao_id);

PUBLIC PMC_ERROR coreotn_mux_ohfs_insert_lo_ipt_clear_cfg(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 old_highest_cs,
                                                          UINT8 chnl_id);


PUBLIC PMC_ERROR coreotn_mux_ghao_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 lo_chnl_id,
                                                 UINT32 ho_chnl_id);



PUBLIC PMC_ERROR coreotn_mux_ghao_clear_old_last_cal_slot(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t ghao_mux_stage,
                                                          UINT32 coreotn_lo_chnl_id,
                                                          BOOL ghao_increase,
                                                          UINT8 coreotn_tx_old_trib_slots[96],
                                                          UINT8 coreotn_tx_new_trib_slots[96],
                                                          UINT32 coreotn_tx_old_cal_mask[3]);


/* ODTU_DEMUX */
PUBLIC PMC_ERROR coreotn_dmx_ghao_init(coreotn_handle_t *coreotn_handle,
                                       coreotn_mux_stage_t mux_stage,
                                       UINT8 lo_chnl_id,
                                       UINT8 tpid,
                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_add_nack(coreotn_handle_t *coreotn_handle,
                                                    coreotn_mux_stage_t mux_stage,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_update_stdby_config(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      BOOL8 add,
                                                      UINT32 old_trib_slot_mask[3],
                                                      UINT8 trib_slots_add_rem[ODTU_DMX_NUM_CHANL],
                                                      UINT8 msi_data);

PUBLIC PMC_ERROR coreotn_dmx_ghao_update_stdby_config_after_knife_edge(coreotn_handle_t *coreotn_handle,
                                                                       coreotn_mux_stage_t mux_stage,
                                                                       UINT8 ho_chnl_id,
                                                                       UINT8 lo_chnl_id,
                                                                       UINT32 num_ts,
                                                                       UINT32 new_trib_slot_mask[3],
                                                                       UINT8 msi_data,
                                                                       BOOL8 add,
                                                                       UINT32 cal_to_add_remove_num,
                                                                       UINT32 cal_slots_add_rem[79]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_config_update_done(coreotn_handle_t *coreotn_handle,
                                                          coreotn_mux_stage_t mux_stage,
                                                          UINT8 ho_chnl_id);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_add_ack(coreotn_handle_t *coreotn_handle,
                                                   coreotn_mux_stage_t mux_stage,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_norm_ack(coreotn_handle_t *coreotn_handle,
                                                    coreotn_mux_stage_t mux_stage,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_idle_nack(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mux_stage_t mux_stage,
                                                     UINT8 *ho_chnl_id,
                                                     UINT8 *lo_chnl_id,
                                                     UINT8 *tpid,
                                                     BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_tscc(coreotn_handle_t *coreotn_handle,
                                                coreotn_mux_stage_t mux_stage,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                UINT8 *tpid,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                BOOL8 tscc);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_rp(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT8 *ho_chnl_id,
                                              UINT8 *lo_chnl_id,
                                              UINT8 *tpid,
                                              BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_remove_nack(coreotn_handle_t *coreotn_handle,
                                                       coreotn_mux_stage_t mux_stage,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_wait_for_remove_ack(coreotn_handle_t *coreotn_handle,
                                                      coreotn_mux_stage_t mux_stage,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      UINT8 *tpid,
                                                      BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR coreotn_dmx_ghao_lo_chnl_deprov(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_stage,
                                                 UINT32 lo_chnl_id,
                                                 UINT32 ho_chnl_id);

/* MPMA */
PUBLIC PMC_ERROR coreotn_mpma_ghao_fifo_resize(coreotn_handle_t *coreotn_handle,
                                               coreotn_mpma_inst_t mpma_inst,
                                               UINT32 chnl, 
                                               UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR coreotn_mpma_ghao_scbs3_reprov(coreotn_handle_t *coreotn_handle,
                                                coreotn_mpma_inst_t mpma_inst,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate );

PUBLIC PMC_ERROR coreotn_ghao_mpma_dsp_enable_set(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mpma_inst_t mpma_inst,
                                                  UINT32 chnl,
                                                  BOOL8 enable,
                                                  UINT32 target_ts);

PUBLIC PMC_ERROR coreotn_ghao_mpma_bwr_end_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpma_inst_t mpma_inst,
                                                     UINT32 chnl,
                                                     DOUBLE odu_bitrate,
                                                     UINT32 oif_pkt_size,
                                                     UINT32 oif_header_size,
                                                     BOOL   use_min_eps,
                                                     UINT32 *pkt_size_ptr,
                                                     UINT32 *pkt_per_int_ptr,
                                                     UINT32 *pkt_per_n_ptr,
                                                     UINT32 *pkt_per_d_ptr,
                                                     DOUBLE *eps_ptr);


PUBLIC PMC_ERROR coreotn_ghao_mpma_bwr_ind_set(coreotn_handle_t *coreotn_handle,
                                               UINT32 chnl,
                                               UINT32 bwr_ind);

/* MPMO */
PUBLIC PMC_ERROR coreotn_mpmo_ghao_fifo_resize(coreotn_handle_t *coreotn_handle,
                                               coreotn_mpmo_inst_t mpmo_inst,
                                               UINT32 chnl,
                                               UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR coreotn_mpmo_ghao_fifo_resize_no_thresh_update(coreotn_handle_t *coreotn_handle,
                                                                coreotn_mpmo_inst_t mpmo_inst,
                                                                UINT32 chnl, 
                                                                UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR coreotn_mpmo_ghao_scbs3_reprov(coreotn_handle_t *coreotn_handle,
                                                coreotn_mpmo_inst_t mpmo_inst,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate );

PUBLIC PMC_ERROR coreotn_mpmo_ghao_scbs3_reprov_force_userbits(coreotn_handle_t *coreotn_handle,
                                                               coreotn_mpmo_inst_t mpmo_inst,
                                                               UINT32 chnl,
                                                               UINT32 oduflex_gfp_rate,
                                                               UINT32 set_mask[3],
                                                               UINT32 userbit_mask[3]);

PUBLIC PMC_ERROR coreotn_mpmo_bwr_ind_get(coreotn_handle_t *coreotn_handle,
                                          coreotn_mpmo_inst_t mpmo_instance,
                                          UINT32 bwr_ind[3]);


PUBLIC PMC_ERROR coreotn_mpmo_xoff_thresh_change(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mpmo_inst_t mpmo_instance,
                                                 UINT32 chnl_id,
                                                 BOOL8 increase);


PUBLIC PMC_ERROR coreotn_mpmo_set_custom_userbit_mask(coreotn_handle_t *coreotn_handle,
                                                      UINT32 chnl_id,
                                                      coreotn_mpmo_inst_t mpmo_instance,
                                                      BOOL enable,
                                                      UINT32 set_mask[3],
                                                      UINT32 userbit_mask[3],
                                                      BOOL8 config_update);

PUBLIC PMC_ERROR coreotn_mpmo_update_userbit_mask(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mpmo_inst_t mpmo_instance,
                                                  UINT32 chnl_id,
                                                  UINT32 cal_slot_to_add_remove[96],
                                                  UINT32 num_to_add_remove,
                                                  UINT32 cal_slot_mask[3],
                                                  UINT32 userbit_mask[3]);

PUBLIC PMC_ERROR coreotn_mpmo_adjust_age_gain(coreotn_handle_t *coreotn_handle,
                                              coreotn_mpmo_inst_t mpmo_instance,
                                              UINT32 chnl_id,
                                              UINT32 num_cal_entries);

PUBLIC PMC_ERROR coreotn_ghao_mpmo_bwr_rate_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpmo_inst_t mpmo_inst,
                                                      UINT32 chnl,
                                                      UINT32 oduflex_gfp_rate,
                                                      BOOL8 increase);

PUBLIC PMC_ERROR coreotn_ghao_mpmo_bwr_rate_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpmo_inst_t mpmo_inst,
                                                      UINT32 chnl,
                                                      UINT32 oduflex_gfp_rate,
                                                      BOOL8 increase);

PUBLIC PMC_ERROR coreotn_ghao_mpmo_bwr_end_param_set(coreotn_handle_t *coreotn_handle,
                                                     coreotn_mpmo_inst_t mpmo_inst,
                                                     UINT32 chnl,
                                                     DOUBLE odu_bitrate,
                                                     UINT32 pkt_size,
                                                     UINT32 pkt_per_int,
                                                     UINT32 pkt_per_n,
                                                     UINT32 pkt_per_d,
                                                     DOUBLE eps_ptr);


PUBLIC PMC_ERROR coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(coreotn_handle_t *coreotn_handle,
                                                             UINT32 chnl,
                                                             DOUBLE client_bitrate,
                                                             UINT32 pkt_size,
                                                             UINT32 pkt_per_int,
                                                             UINT32 pkt_per_n,
                                                             UINT32 pkt_per_d,
                                                             UINT32 *cn_base_int,
                                                             UINT32 *cn_base_n,
                                                             UINT32 *cn_base_d,
                                                             BOOL *program_cn_n_first);

PUBLIC PMC_ERROR coreotn_ghao_mpmo_new_cn_set(coreotn_handle_t *coreotn_handle,
                                              UINT32 chnl,
                                              UINT32 cn_base_int,
                                              UINT32 cn_base_n,
                                              UINT32 cn_base_d,
                                              BOOL program_cn_n_first);


/* ODUKSC */
PUBLIC PMC_ERROR coreotn_oduksc_scbs3_cgen_reprov(coreotn_handle_t *coreotn_handle,
                                                  coreotn_mux_stage_t mux_stage,
                                                  UINT32 chnl,
                                                  UINT32 oduflex_gfp_rate);

PUBLIC PMC_ERROR coreotn_oduksc_ramp_prov(coreotn_handle_t *coreotn_handle,
                                          coreotn_mux_stage_t mux_stage,
                                          UINT32 chnl,
                                          UINT32 oduflex_gfp_rate);

PUBLIC PMC_ERROR coreotn_oduksc_ramp_start(coreotn_handle_t *coreotn_handle,
                                           coreotn_mux_stage_t mux_stage,
                                           UINT32 chnl);

PUBLIC PMC_ERROR coreotn_oduksc_ramp_done_isr(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT32 chnl);

PUBLIC PMC_ERROR coreotn_oduksc_ramp_done_get(coreotn_handle_t *coreotn_handle,
                                              coreotn_mux_stage_t mux_stage,
                                              UINT32 chnl,
                                              UINT32 *ramp_done);


/* ODUJAT */
PUBLIC PMC_ERROR coreotn_odujat_ghao_resize_fifo(coreotn_handle_t *coreotn_handle,
                                                 coreotn_mux_stage_t mux_instance,
                                                 UINT32 chnl_id,
                                                 UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR coreotn_mux_ghao_passthru_cal_enable_diable(coreotn_handle_t *coreotn_handle,
                                                             UINT32 cycle,
                                                             BOOL8 enable);

PUBLIC void odu_struct_ri_rcp_cfc_int_sel_get(coreotn_handle_t        *coreotn_handle,
                                              odu_rcp_frm_id_t        *framer_id_ptr,
                                              util_global_direction_t *src_dir_ptr);

PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_sel_set(coreotn_handle_t        *coreotn_handle,
                                                   odu_rcp_frm_id_t         framer_id,
                                                   util_global_direction_t  src_dir);

PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_type_sel_set(coreotn_handle_t          *coreotn_handle,
                                                        odu_struct_t              *odu_struct,
                                                        odu_ri_rcp_cfc_int_type_t  int_type);

PUBLIC PMC_ERROR odu_struct_ri_rcp_cfc_int_type_sel_get(coreotn_handle_t           *coreotn_handle,
                                                        odu_struct_t               *odu_struct,
                                                        odu_ri_rcp_cfc_int_type_t  *int_type_ptr);

PUBLIC PMC_ERROR coreotn_odu4_calendar_pinch_set(coreotn_handle_t *coreotn_handle,
                                                 coreotn_oduksw_output_port_type_t output_port,
                                                 BOOL cal_pinch_enable);

PUBLIC PMC_ERROR coreotn_oduksw_reprov_pkt_size_configs(coreotn_handle_t *coreotn_handle,
                                                        coreotn_oduksw_output_port_type_t output_port,
                                                        util_global_odu_level_t odu_level,
                                                        UINT32 output_chnl_id,
                                                        UINT32 pkt_size,
                                                        UINT32 pkt_per_int,
                                                        UINT32 pkt_per_n,
                                                        UINT32 pkt_per_d,
                                                        BOOL   hys_buffer_en, 
                                                        UINT32 hys_buffer,
                                                        UINT32 latency_s);

PUBLIC PMC_ERROR coreotn_dcs_to_mapotn_pinch_entry_set(coreotn_handle_t        *coreotn_handle,
                                                       UINT32 calendar_entry,
                                                       BOOL8 pinch_enable);

PUBLIC PMC_ERROR coreotn_dcs_to_mapotn_pinch_entry_get(coreotn_handle_t        *coreotn_handle,
                                                       UINT32 calendar_entry,
                                                       BOOL8 *pinch_enable);
                                                        
PUBLIC PMC_ERROR coreotn_hys_latency_get(coreotn_handle_t *coreotn_handle,
                                         coreotn_oduksw_output_port_type_t output_port,
                                         UINT32 output_chnl_id,
                                         DOUBLE loc_client_rate,
                                         UINT32 *hys_latency); 

PUBLIC PMC_ERROR coreotn_hys_latency_range_chk(coreotn_handle_t *coreotn_handle,
                                               coreotn_oduksw_output_port_type_t output_port,
                                               UINT32 output_chnl_id,
                                               DOUBLE client_bitrate,
                                               UINT32 pkt_size,
                                               UINT32 hys_buffer);                                                                                               
PUBLIC BOOL8 coreotn_odu_rcp_di_enable_get(coreotn_handle_t *coreotn_handle);

PUBLIC PMC_ERROR coreotn_mpmo_csi_cfg(coreotn_handle_t *coreotn_handle,
                                      coreotn_mpmo_inst_t mpmo_inst,
                                      BOOL csi_squelch_en);
                                      
PUBLIC PMC_ERROR coreotn_mpma_csi_cfg(coreotn_handle_t *coreotn_handle,
                                      coreotn_mpma_inst_t mpma_inst,
                                      mpma_csi_type_t cfg_csi_type,
                                      UINT32 csi_val);

PUBLIC void coreotn_mapotn_null_frame_mode_ptr_get(coreotn_handle_t *coreotn_handle,
                                                   UINT32 **null_chan_en,
                                                   UINT32 **prbs_null_chan_en);

PUBLIC void coreotn_prev_rfrm_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                            UINT32 chnl_id,
                                            BOOL fo1_rfrm,
                                            coreotn_maint_sig_type_t *prev_maint_sig);

PUBLIC void coreotn_cm_status_get(coreotn_handle_t *coreotn_handle,
                                  coreotn_fmf_inst_t fmf_istance,
                                  UINT32 chnl_id,
                                  UINT32 *value);

PUBLIC PMC_ERROR coreotn_pm_status_get(coreotn_handle_t *coreotn_handle,
                                       coreotn_int_chnl_info_t *chnl_info_ptr,
                                       util_global_otn_pm_status_t *pm_table_ptr);

PUBLIC void coreotn_mpmo_fifo_clear(coreotn_handle_t *coreotn_handle,
                                        UINT32 clear_set,
                                        odu_struct_t     *odu_struct_ptr);

PUBLIC PMC_ERROR coreotn_reset_mux(coreotn_handle_t *coreotn_handle,
                                   UINT32 ho_chnl_id,
                                   BOOL8 is_sysotn);
PUBLIC void coreotn_oduksw_port_bulk_update_req_set(coreotn_handle_t *coreotn_handle,
                                                    util_global_oduk_port_t port_type,
                                                    util_global_datapath_dir_t port_direction,
                                                    BOOL8 update_required);

PUBLIC BOOL8 coreotn_oduksw_port_bulk_update_req_get(coreotn_handle_t *coreotn_handle,
                                                     util_global_oduk_port_t port_type,
                                                     util_global_datapath_dir_t port_direction);
PUBLIC void coreotn_oduksw_port_bulk_update_req_init(coreotn_handle_t *coreotn_handle);

#ifdef __cplusplus
}
#endif

#endif /* _COREOTN_H */

/*
** end of file
*/
