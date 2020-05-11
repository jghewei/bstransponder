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
*      To be added.
* 
*   NOTES:
*
*******************************************************************************/

#ifndef _UTIL_GLOBAL_API_H
#define _UTIL_GLOBAL_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_types.h"
#include "pmc_mid.h"
/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: util_global_odukp_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enum defines the ODUKP rates.  The rates UTIL_GLOBAL_ODU1E_ETRANS and
*   UTIL_GLOBAL_ODU2E_ETRANS are for internal usage only.
*   
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum 
{
    UTIL_GLOBAL_ODU0 = 34,
    UTIL_GLOBAL_ODU1,
    UTIL_GLOBAL_ODU1E,
    UTIL_GLOBAL_ODU1F,
    UTIL_GLOBAL_ODU2,
    UTIL_GLOBAL_ODU2E,
    UTIL_GLOBAL_ODU2F,
    UTIL_GLOBAL_ODU3,
    UTIL_GLOBAL_ODU3E1,
    UTIL_GLOBAL_ODU3E2,
    UTIL_GLOBAL_ODU4,

    UTIL_GLOBAL_ODUFLEX_CBR,
    UTIL_GLOBAL_ODUFLEX_GFP,
    UTIL_GLOBAL_ODU1E_ETRANS,
    UTIL_GLOBAL_ODU2E_ETRANS,
    UTIL_GLOBAL_ODUK_DONT_CARE,
    UTIL_GLOBAL_LAST_ODUK
} util_global_odukp_type_t;

/*******************************************************************************
* ENUM: util_global_mapping_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MAPPING_MODE (definition of all the supported mapping types)
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum util_global_mapping_mode_t 
{
    UTIL_GLOBAL_BMP     = 0,
    UTIL_GLOBAL_AMP     = 1,
    UTIL_GLOBAL_GMP     = 2,
    UTIL_GLOBAL_RESERVED  = 3,
    UTIL_GLOBAL_GFP     = 4,
    UTIL_GLOBAL_NO_MAP  = 5,
} util_global_mapping_mode_t;

/*******************************************************************************
* ENUM: util_global_odu_line_payload_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store HO Tributary slot types.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_ODU_UNCHANNELIZED  = 0,
    UTIL_GLOBAL_ODU_TS_1G25        = 1,
    UTIL_GLOBAL_ODU_TS_2G5         = 2,
    UTIL_GLOBAL_ODU_NULL           = 3,
   
    LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD,
} util_global_odu_line_payload_t;

/** 
* The following typdef is used as a handle to util_global_switch_data_def_t. 
* This allows for this type to be passed into many different functions which 
* can unwrap and process the port data they require.
*/
typedef void util_global_switch_data_t;

/*******************************************************************************
* ENUM: util_global_opu_payload_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store payload types processed in COREOTN.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_OPU_PAYLOAD_NULL    = 0,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_1    = 0x1,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_2    = 0x2,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_3    = 0x3,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_5    = 0x5,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_7    = 0x7,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_8    = 0x8,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_9    = 0x9,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_F    = 0xF,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_20   = 0x20,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_21   = 0x21,
    UTIL_GLOBAL_OPU_PAYLOAD_PT_80   = 0x80,
    UTIL_GLOBAL_OPU_PAYLOAD_PRBS    ,
    
    LAST_OPU_PAYLOAD_TYPE
} util_global_opu_payload_type_t;


/*******************************************************************************
* ENUM: util_patt_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors status.
*
* ELEMENTS:
*   UTIL_PATT_STATUS_LOCKED              - Pattern monitor or generator is locked.
*   UTIL_PATT_STATUS_TRYING_TO_LOCK      - Pattern monitor or generator is trying to lock.
*   UTIL_PATT_STATUS_ZERO_DET            - Pattern monitor is receiving all logic 0.
*   UTIL_PATT_STATUS_ONES_DET            - Pattern monitor is receiving all logic 1.
*   UTIL_PATT_STATUS_DISABLED            - Pattern monitor or generator is disabled.
*
*******************************************************************************/
typedef enum 
{
    UTIL_PATT_STATUS_LOCKED             = 0,
    UTIL_PATT_STATUS_TRYING_TO_LOCK     ,
    UTIL_PATT_STATUS_ZERO_DET           ,
    UTIL_PATT_STATUS_ONES_DET           ,
    UTIL_PATT_STATUS_DISABLED           ,
    LAST_UTIL_PATT_STATUS
} util_patt_status_t;

/*******************************************************************************
* ENUM: util_patt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors mode
*
* ELEMENTS:
*   UTIL_PATT_MODE_PRBS_5   - PRBS mode 5
*   UTIL_PATT_MODE_PRBS_7   - PRBS mode 7
*   UTIL_PATT_MODE_PRBS_11  - PRBS mode 11
*   UTIL_PATT_MODE_PRBS_15  - PRBS mode 15
*   UTIL_PATT_MODE_PRBS_20  - PRBS mode 20
*   UTIL_PATT_MODE_PRBS_23  - PRBS mode 23
*   UTIL_PATT_MODE_PRBS_31  - PRBS mode 31
*   UTIL_PATT_MODE_PRBS_9   - PRBS mode 9
*   UTIL_PATT_MODE_CJAT     - CJPAT
*   UTIL_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*
*******************************************************************************/
typedef enum 
{
    UTIL_PATT_MODE_PRBS_5   = 0,
    UTIL_PATT_MODE_PRBS_7   = 1,
    UTIL_PATT_MODE_PRBS_11  = 2,
    UTIL_PATT_MODE_PRBS_15  = 3,
    UTIL_PATT_MODE_PRBS_20  = 4,
    UTIL_PATT_MODE_PRBS_23  = 5,
    UTIL_PATT_MODE_PRBS_31  = 6,
    UTIL_PATT_MODE_PRBS_9   = 7,
    UTIL_PATT_MODE_CJAT     = 8,
    UTIL_PATT_MODE_USER_DEF = 15,
    UTIL_PATT_MODE_PRBS_29       ,
    LAST_UTIL_PATT_MODE

} util_patt_mode_t;


/*******************************************************************************
* ENUM: util_global_vlan_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating VLAN type range of values.
*
* ELEMENTS:
*   UTIL_GLOBAL_VLAN_TYPE_IEEE_8021P_SINGLE_TAG      - Single VLAN tag (IEEE_8021P)
*   UTIL_GLOBAL_VLAN_TYPE_IEEE_8021AD_DOUBLE_TAG     - Double BLAN tag (IEEE_8021AD)
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_VLAN_TYPE_IEEE_8021P_SINGLE_TAG = 0x8100,
    UTIL_GLOBAL_VLAN_TYPE_IEEE_8021AD_DOUBLE_TAG =  0x88A8,
    LAST_UTIL_GLOBAL_VLAN_TYPE
}util_global_vlan_type_t;


/*******************************************************************************
* ENUM: util_global_pkt_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating supported packet type that can be classified by DIGI ENET.
* 
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_PKT_TYPE_SYNCE                                     = 1,
    UTIL_GLOBAL_PKT_TYPE_LACP                                      = 2,
    UTIL_GLOBAL_PKT_TYPE_MARKER                                    = 3,
    UTIL_GLOBAL_PKT_TYPE_VLAN_MGMT                                 = 4,
    UTIL_GLOBAL_PKT_TYPE_PTP_ALL                                   = 5, /* Intended for test only */

    LAST_UTIL_GLOBAL_PKT_TYPE
}util_global_pkt_type_t;

/*******************************************************************************
* ENUM: util_global_direction_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating flow direction: RX, TX or both.
*
* ELEMENTS:
*   UTIL_GLOBAL_DIR_RX      - RX direction.
*   UTIL_GLOBAL_DIR_TX      - TX direction.
*   UTIL_GLOBAL_DIR_BOTH    - RX and TX directions.
*   LAST_UTIL_GLOBAL_DIR    - NONE.
*******************************************************************************/
typedef enum util_global_direction_t
{
    UTIL_GLOBAL_DIR_RX = 0x1,
    UTIL_GLOBAL_DIR_TX = 0x2,
    UTIL_GLOBAL_DIR_BOTH = 0x3,
    LAST_UTIL_GLOBAL_DIR = 0x0
}util_global_direction_t;

/*******************************************************************************
* ENUM: util_global_sfi51_refclk_source_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating SFI51 CSU refrence clocks.
*
* ELEMENTS:
*   UTIL_GLOBAL_SFI51_REFCLK_SRC_0      - Reference clock source 0.
*   UTIL_GLOBAL_SFI51_REFCLK_SRC_1      - Reference clock source 1.
*   UTIL_GLOBAL_SFI51_REFCLK_SRC_2      - Reference clock source 2.
*******************************************************************************/
typedef enum util_global_sfi51_refclk_source_t
{
    UTIL_GLOBAL_SFI51_REFCLK_SRC_0 = 0x0,
    UTIL_GLOBAL_SFI51_REFCLK_SRC_1 = 0x1,
    UTIL_GLOBAL_SFI51_REFCLK_SRC_2 = 0x2,

    LAST_UTIL_GLOBAL_SFI51_REFCLK_SRC
}util_global_sfi51_refclk_source_t;

/*******************************************************************************
* ENUM: hostmsg_opsa_subsystem_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_OPSA message subsytem field types. For more information 
*   please look at the DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_opsa_subsystem_cfg_t
{
    OPSA_HOSTMSG_CPB_SS_CBRC             = 0x00, 
    OPSA_HOSTMSG_CPB_SS_MAPOTN           = 0x01, 
    OPSA_HOSTMSG_CPB_SS_ENET_LINE        = 0x02, 
    OPSA_HOSTMSG_CPB_SS_ENET_SYS         = 0x03, 
    OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO1  = 0x04, 
    OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_FO2  = 0x05, 
    OPSA_HOSTMSG_ODUK_SW_SS_COREOTN_CTL  = 0x06, 
    LAST_OPSA_HOSTMSG_SS
} hostmsg_opsa_subsystem_cfg_t;

/*******************************************************************************
* ENUM: util_vhist_test_length_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the number of samples to capture
*
* ELEMENTS:
*   UTIL_VHIST_DEBUG    -  
*   UTIL_VHIST_SHORT    - 
*   UTIL_VHIST_LONG     - 
*   UTIL_VHIST_USERDEF  - User defined
*
*******************************************************************************/
typedef enum {
    UTIL_VHIST_DEBUG    = 0,    /*   0x0FFFFFFF */
    UTIL_VHIST_SHORT    = 1,    /*   0xFFFFFFFF */
    UTIL_VHIST_LONG     = 2,    /* 0x0FFFFFFFFF */
    UTIL_VHIST_USERDEF  = 3     /* User defined */
} util_vhist_test_length_t;

/*******************************************************************************
* ENUM: util_global_coreotn_sgmii_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration identifying the FEGE ports within COREOTN subsystem
*
* ELEMENTS:
*   UTIL_GLOBAL_COREOTN_SGMII_INST_DIRCP  - Defect Information Ring Control Port
*   UTIL_GLOBAL_COREOTN_SGMII_INST_RIRCP  - Remove Information Ring Control Port
*   UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_1  - Overhead Port 1
*   UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_2  - Overhead Port 2
*   UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_3  - Overhead Port 3
*   LAST_UTIL_GLOBAL_COREOTN_SGMII_INST   - Used for out of range error checking
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_COREOTN_SGMII_INST_DIRCP    = 0,
    UTIL_GLOBAL_COREOTN_SGMII_INST_RIRCP,
    UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_1,
    UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_2,
    UTIL_GLOBAL_COREOTN_SGMII_INST_OHP_3,
    LAST_UTIL_GLOBAL_COREOTN_SGMII_INST
} util_global_coreotn_sgmii_inst_t;

/*******************************************************************************
* ENUM: util_global_counter_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration identifying the FEGE ports within COREOTN subsystem
*
* ELEMENTS:
*   UTIL_GLOBAL_COUNTER_ACTION_TRIGGER   - Trigger transfer of counters to holding
*                                          registers
*   UTIL_GLOBAL_COUNTER_ACTION_RETRIEVE  - Read counters to local data structure
*   LAST_UTIL_GLOBAL_COREOTN_SGMII_INST   - Used for out of range error checking
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_COUNTER_ACTION_TRIGGER    = 0,
    UTIL_GLOBAL_COUNTER_ACTION_RETRIEVE,
    LAST_UTIL_GLOBAL_COUNTER_ACTION
} util_global_counter_action_t;

/*******************************************************************************
* STRUCTURE: fw_opsa_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OPSA startup configuration and state variables
*
* ELEMENTS:
*   mpma_enable         - Enable/disable procedure for MPMA channel                     
*   mpma_qs_ch_en       - QSn_CH_EN bit setting to use for the MPMA channel        
*   mpma_state          - MPMA procedure state                               
*   mpma_tick_cnt       - Tick count used to wait for clean window            
*   mpmo_enable         - Enable/disable procedure for MPMO                  
*   mpmo_ch_data_target - Downstream data target 
*   mpmo_acb_instance   - ACB instance for Tx SERDES                          
*   mpmo_state          - MPMO procedure state                               
*   mpmo_tick_cnt       - Tick count used to wait for clean window            
*******************************************************************************/
typedef struct {
    UINT32 mpma_enable;         
    UINT32 mpma_qs_ch_en;       
    UINT32 mpma_state;          
    UINT32 mpma_tick_cnt;       
    UINT32 mpmo_enable;         
    UINT32 mpmo_ch_data_target; 
    UINT32 mpmo_acb_instance;   
    UINT32 mpmo_state;          
    UINT32 mpmo_tick_cnt;       
} fw_opsa_status_t;

/*******************************************************************************
* STRUCTURE: fw_opsa_mpmo_debug_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO debug variables
*   
* ELEMENTS:
*   proc1_counts    - number of times each state has been seen for that procedure
*   proc235_counts  - number of times each state has been seen for that procedure      
*   proc4_counts    - number of times each state has been seen for that procedure
*   proc6_counts    - number of times each state has been seen for that procedure          
*   proc6_ssf_gen_i_set_count        - count of ssf_gen_i == 1                  
*   proc6_ssf_gen_i_clear_count      - count of ssf_gen_i == 0
*   proc6_buf_underrun_i_set_count   - count of buf_underrun_i == 1                      
*   proc6_buf_underrun_i_clear_count - count of buf_underrun_i == 0                              
*   cookie                           - 0x1235678
*******************************************************************************/
typedef struct {
     UINT32 proc1_counts[7];
     UINT32 proc235_counts[7];
     UINT32 proc4_counts[7];
     UINT32 proc6_counts[7];
     UINT32 proc6_ssf_gen_i_set_count;
     UINT32 proc6_ssf_gen_i_clear_count;
     UINT32 proc6_buf_underrun_i_set_count;
     UINT32 proc6_buf_underrun_i_clear_count;
     UINT32 cookie;  
} fw_opsa_mpmo_debug_t;

/*******************************************************************************
* STRUCTURE: digi_fw_opsa_mpmo_times_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OPSA mpmo times
*
* ELEMENTS:
*   see elements            
*******************************************************************************/
typedef struct {
    UINT32 proc1_clear_window_in_us;                /*!< default 250 microseconds */
    UINT32 proc1_zone_center_duration_in_us;        /*!< default 1 second         */
    UINT32 proc235_clear_window_in_us;              /*!< default 250 microseconds */      
    UINT32 proc235_zone_center_duration_in_us;      /*!< default 100 microseconds */
    UINT32 proc4_clear_window_in_us;                /*!< default 250 microseconds */      
    UINT32 proc4_zone_center_duration_in_us;        /*!< default 100 microseconds */      
    UINT32 proc6_fc1200_clear_window_in_us;         /*!< default 0 microseconds */      
    UINT32 proc6_fc1200_zone_center_duration_in_us; /*!< default 1 second         */      
    UINT32 proc6_other_clear_window_in_us;          /*!< default 0 microseconds */      
    UINT32 proc6_other_zone_center_duration_in_us;  /*!< default 1 second         */      
} digi_fw_opsa_mpmo_times_t;

/*******************************************************************************
* STRUCTURE: digi_fw_opsa_mpmo_fifo_trigger_enables_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OPSA mpmo fifo trigger enables
*
* ELEMENTS:
*   see elements            
*******************************************************************************/
typedef struct {
    UINT32 proc1_fifo_trigger_enabled;                 /*!< default TRUE */
    UINT32 proc235_fifo_trigger_enabled;               /*!< default TRUE */
    UINT32 proc4_fifo_trigger_enabled;                 /*!< default TRUE */
    UINT32 proc6_fc1200_fifo_trigger_enabled;          /*!< default TRUE */
    UINT32 proc6_other_fifo_trigger_enabled;           /*!< default TRUE */
} digi_fw_opsa_mpmo_fifo_trigger_enables_t;

/*
** Constants
*/
/* DIGI user-defined patterns maximum size for S16 serdes in bits */
#define UTIL_PATTERN_GEN_MON_USER_DATA_MAX_BIT_SIZE 320
/* DIGI user-defined patterns maximum size for S16 serdes in words of 32-bit */
#define UTIL_PATTERN_GEN_MON_USER_DATA_S16_WORD_SIZE 10
/* DIGI user-defined patterns maximum size for T8 serdes in bits */
#define UTIL_PATTERN_GEN_MON_USER_DATA_T8_BIT_SIZE  40
/* DIGI user-defined patterns maximum size for T8 serdes in words of 32-bit */
#define UTIL_PATTERN_GEN_MON_USER_DATA_T8_WORD_SIZE  2

/* VLAN TPID for single vlan-tagged paket */
#define UTIL_GLOBAL_IEEE_8021Q_VLAN_TPID             0x8100

/* VLAN TPID for double vlan-tagged paket */
#define UTIL_GLOBAL_IEEE_8021AD_VLAN_TPID            0x88a8

/*
 * Matches the number of odu_rfrm_int_defect_t enumerands. As well redefine
 * other constants to ensure the arrays defined in 
 * util_global_otn_pm_status_t structure match those for retrieving values
 * in odu_rfrm.
 */
#define UTIL_GLOBAL_NUM_ODU_RFRM_DEFECTS 16
#define UTIL_GLOBAL_NUM_ODU_RFRM_CHNL    96
#define UTIL_GLOBAL_TCM_POOL_SIZE_96        256
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: util_serdes_patt_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining serdes patt generators or monitor configuration.
*
* ELEMENTS:
*  enable            - pattern generator or monitor state ON(TRUE) or OFF(FALSE).
*  mode              - pattern type (UTIL_PATT_MODE_PRBS_5 or UTIL_PATT_MODE_PRBS_7 or ..).
*  inv_cfg           - pattern monitor inverter configuration ON(TRUE) or OFF(FALSE).
*  usr_patt_size     - size of user-defined pattern (40 or 320 bits depending on serdes type).
*  usr_patt          - user-defined pattern.
*
*******************************************************************************/
typedef struct util_serdes_patt_cfg_t
{
    BOOL8 enable;
    util_patt_mode_t mode;
    BOOL8 inv_cfg;
    UINT32 usr_patt_size;
    UINT32 usr_patt[UTIL_PATTERN_GEN_MON_USER_DATA_MAX_BIT_SIZE];
} util_serdes_patt_cfg_t;
 
/*******************************************************************************
* STRUCTURE: util_serdes_patt_mon_cfg_t
* ______________________________________________________________________________
* 
* DESCRIPTION:
*   Structure defining common patt generators/monitor configuration.
*
* ELEMENTS:
*   patt_cfg   - pattern monitor basic configuration.
*
*******************************************************************************/
typedef struct util_serdes_patt_mon_cfg_t
{
    util_serdes_patt_cfg_t patt_cfg;
} util_serdes_patt_mon_cfg_t;

/*******************************************************************************
* STRUCTURE: util_patt_gen_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining serdes pattern generator configuration.
*
* ELEMENTS:
*   patt_cfg   - pattern generitor basic configuration.
*******************************************************************************/
typedef struct util_serdes_patt_gen_cfg_t
{
    util_serdes_patt_cfg_t patt_cfg;
} util_serdes_patt_gen_cfg_t;

/*******************************************************************************
* STRUCTURE: util_global_vlan_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining vlan replace configuration. The sturcture is used to
*   specify incoming or outgoing vlan packets that transit through ENET ss,
*   CPU and MGMT_FEGE.
*
* ELEMENTS:
*    vlan_type       - VLAN type.
*    vlan_pcp        - VLAN priority code point.
*    vlan_cfi        - VLAN drop eligible.
*    vlan_id         - vlan ID that replaced ingress source packets' VLAN_ID.
*                      Egress packets VLAN_ID used to determine packet destination.
*
*******************************************************************************/
typedef struct util_global_vlan_cfg_t
{    
    util_global_vlan_type_t vlan_type;
    UINT8                   vlan_pcp;
    UINT8                   vlan_cfi; 
    UINT16                  vlan_id;
} util_global_vlan_cfg_t;

/*******************************************************************************
* STRUCTURE: util_global_otn_pm_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Holds the pm/tcm status interrupts. These values are either set directly
*  as interrupts or are used for setting other interrupts. Table is used to
*  retrieve the values currently set in the system.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct 
{
   BOOL8  int_chnl_en[UTIL_GLOBAL_NUM_ODU_RFRM_CHNL];
   BOOL8  int_pid_en[UTIL_GLOBAL_TCM_POOL_SIZE_96];
   BOOL8  int_comb_en[UTIL_GLOBAL_NUM_ODU_RFRM_DEFECTS];

} util_global_otn_pm_status_t;


/*
** Global variables
*/


/* The source supports 7 TCM/PM layers
TCM1, TCM2, TCM3, TCM4, TCM5, TCM6, PM*/
#define ODU_SOURCE_MAX_PM_TCM_LAYERS 7

/* The sink supports up to 9 TCM/PM layers
TCM1, TCM2, TCM3, TCM4, TCM5, TCM6, PM, floating TCM, PM-NIM*/
#define ODU_SINK_MAX_PM_TCM_LAYERS 9



/*
** Function Prototypes
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
#define UTIL_GLOBAL_LOG_ERR_TABLE                                       \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GLOBAL_LOG_CODE_DEBUG,               "UTIL_GLOBAL: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GLOBAL_ERR_CODE_ASSERT,              "UTIL_GLOBAL: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GLOBAL_LOG_CODE_LOG,                 "UTIL_GLOBAL: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS,   "UTIL_GLOBAL: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_GLOBAL_LOG_ERR_DIV_BY_ZERO,          "UTIL_GLOBAL: Division by zero" )
    

/* 
* The following creates the ERR/LOG indexes from above by pulling out the enum
* portion of the LOG_ERR_TABLE above  
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum UTIL_GLOBAL_LOG_ERR_INDEX
{
    UTIL_GLOBAL_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_UTIL_GLOBAL << 16),
    UTIL_GLOBAL_LOG_ERR_TABLE
    UTIL_GLOBAL_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char UTIL_GLOBAL_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char UTIL_GLOBAL_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _UTIL_GLOBAL_API_H */

/*
** end of file
*/
