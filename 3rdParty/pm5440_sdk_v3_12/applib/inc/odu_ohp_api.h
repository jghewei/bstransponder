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
*       OHP configuration
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_OHP_API_H
#define _ODU_OHP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfc_api.h"
#include "enet_fege_api.h"

/*
** Enumerated Types
*/
  
/*******************************************************************************
* ENUM: odu_ohp_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of the three OHPs.
*
* ELEMENTS:
*   ODU_OHP_1      - OHP 1
*   ODU_OHP_2      - OHP 2
*   ODU_OHP_3      - OHP 3
*   ODU_OHP_LAST   - This is for out of range checking.
*
*******************************************************************************/
typedef enum 
{
    ODU_OHP_1 = 0,
    ODU_OHP_2,
    ODU_OHP_3,
    ODU_OHP_LAST
} odu_ohp_inst_t;

/*******************************************************************************
* ENUM: odu_ohp_port_framer_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of the different framers that an OHP port can be connected to.
*
* ELEMENTS:
*   ODU_OHP_PORT_UNUSED        - OHP Port is not connected to a framer
*   ODU_OHP_PORT_FRAMER_1      - Stage 1 framer (OH)
*   ODU_OHP_PORT_FRAMER_2      - Stage 2 framer (MO)
*   ODU_OHP_PORT_FRAMER_3A     - Stage 3A framer (LO)
*   ODU_OHP_PORT_FRAMER_3B     - Stage 3B framer (LO)
*   ODU_OHP_PORT_FRAMER_4      - Stage 4 framer (SNC)
*   ODU_OHP_PORT_FRAMER_N      - Stage N framer (SysOTN)
*   ODU_OHP_PORT_FRAMER_LAST   - This is for out of range checking.
******************************************************************************/
typedef enum 
{
    ODU_OHP_PORT_UNUSED = 0,
    ODU_OHP_PORT_FRAMER_1,
    ODU_OHP_PORT_FRAMER_2,
    ODU_OHP_PORT_FRAMER_3A,
    ODU_OHP_PORT_FRAMER_3B,
    ODU_OHP_PORT_FRAMER_4,
    ODU_OHP_PORT_FRAMER_N,
    ODU_OHP_PORT_FRAMER_LAST
} odu_ohp_port_framer_t;

/*******************************************************************************
* ENUM: odu_ohp_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of the different OHP Ports.
*
* ELEMENTS:
*   ODU_OHP_PORT_A        - OHP Port A
*   ODU_OHP_PORT_B        - OHP Port B
*   ODU_ODP_PORT_LAST     - This is for out of range checking.
*
*******************************************************************************/
typedef enum 
{
    ODU_OHP_PORT_A = 0,
    ODU_OHP_PORT_B,
    ODU_OHP_PORT_LAST,
} odu_ohp_port_t;


/*******************************************************************************
* ENUM: odu_ohp_cfc_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of ODU OHP CFC instances.
*
* ELEMENTS:
*    ODU_OHP_CFC_EX0 - CFC EX0
*    ODU_OHP_CFC_EX1 - CFC EX1
*    ODU_OHP_CFC_IN0 - CFC IN0
*    ODU_OHP_CFC_IN1 - CFC IN1
*******************************************************************************/
typedef enum
{
    ODU_OHP_CFC_EX0 = 0,
    ODU_OHP_CFC_EX1 = 1,
    ODU_OHP_CFC_IN0 = 2,
    ODU_OHP_CFC_IN1 = 3,
    LAST_ODU_OHP_CFC
} odu_ohp_cfc_inst_t;

/*
** Constants
*/
/* Define maximum number of channels associated to ODU OHP instance */
#define ODU_OHP_3_MAX_NUM_CHNLS      96
#define ODU_OHP_2_MAX_NUM_CHNLS      96
#define ODU_OHP_1_MAX_NUM_CHNLS      12
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odu_ohp_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct odu_ohp_handle_struct_t odu_ohp_handle_t;

/*******************************************************************************
* STRUCTURE: odu_ohp_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHP configuration structure.
*
* ELEMENTS:
*       ohp              - OHP (ODU_OHP_1, ODU_OHP_2,ODU_OHP_3)
*       port_a_framer    - OHP port A framer (see digi_ohp_port_framer_t)
*       port_a_src_addr  - OHP Port A src address
*       port_a_dest_addr - OHP Port A dest address
*       port_b_framer    - OHP port B framer (see digi_ohp_port_framer_t)
*       port_b_src_addr  - OHP Port B src address
*       port_b_dest_addr - OHP Port b dest address
*******************************************************************************/
typedef struct odu_ohp_cfg_t 
{
    odu_ohp_inst_t  ohp;
    odu_ohp_port_framer_t port_a_framer;
    UINT32 port_a_src_addr;
    UINT32 port_a_dest_addr;
    odu_ohp_port_framer_t port_b_framer;
    UINT32 port_b_src_addr;
    UINT32 port_b_dest_addr;
} odu_ohp_cfg_t;

/*******************************************************************************
* STRUCTURE: odu_ohp_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU OHP table for unchannelized interrupts and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct 
{
    enet_fege_int_t enet_fege;

    /* interrupts */
    BOOL8 rxmac_uid_fail_i;
    BOOL8 rxmac_frmrid_fail_i;
    BOOL8 rxmac_chid_fail_i;
    BOOL8 rxmac_pkterr_i;
    BOOL8 rxmac_fifo_err_i;
    
} odu_ohp_int_t;

/*******************************************************************************
* STRUCTURE: odu_ohp_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU OHP table for channelized interrupts and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct 
{
    /* interrupts */
    BOOL8 rxfa_cfc_full_i;
    BOOL8 rxfb_cfc_full_i;
    BOOL8 rxfa_txdly_i;
    BOOL8 rxfb_txdly_i;
    BOOL8 cfc_ex0_ipt_err_i;
    BOOL8 cfc_ex1_ipt_err_i;
    BOOL8 cfc_in0_ipt_err_i;
    BOOL8 cfc_in1_ipt_err_i;
} odu_ohp_int_chnl_t;


/*
** Global variables
*/

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
#define ODU_OHP_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_LOG_CODE_DEBUG,                           "ODU_OHP: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_CODE_ASSERT,                          "ODU_OHP: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_NULL_HANDLE,                          "ODU_OHP: Invalid Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_INVALID_ARGS,                         "ODU_OHP: Invalid Arguement" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_LOG_CODE_LOG,                             "ODU_OHP: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_DIV_BY_ZERO,                          "ODU_OHP: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_STATE_NOT_OPERATIONAL,                "ODU_OHP: OHP not initialized" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_STATE_ALREADY_START,                  "ODU_OHP: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_STATE_ALREADY_PROVISIONED,            "ODU_OHP: Instance is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_STATE_UNKNOWN,                        "ODU_OHP: Unkown state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_DB_ENTRY_NOT_FOUND,                   "ODU_OHP: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_INVALID_OHP,                          "ODU_OHP: Invalid OHP" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_INVALID_FRAMER,                       "ODU_OHP: Invalid Framer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_INVALID_SCHED,                        "ODU_OHP: Invalid Schedule" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_PORT_NOT_CONFIG,                      "ODU_OHP: Port not configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_CH_NOT_ENABLED,                       "ODU_OHP: Ch not enable" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_CH_ALREADY_ENABLED,                   "ODU_OHP: Ch already enabled" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_SCH_REQ,                              "ODU_OHP: sch Req Poll" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_MISMATCHED_RATE,                      "ODU_OHP: TX and RX rates are mismatched" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_OHP_ERR_SCHEDULE_FULL,                        "ODU_OHP: Schedule full" ) 

   
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODU_OHP_LOG_ERR_INDEX
{
    ODU_OHP_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODU_OHP << 16),
    ODU_OHP_LOG_ERR_TABLE
    ODU_OHP_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODU_OHP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODU_OHP_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _ODU_OHP_API_H */

/*
** end of file
*/
