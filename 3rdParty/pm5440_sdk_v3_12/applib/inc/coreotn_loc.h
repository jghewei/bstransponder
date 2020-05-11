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
#ifndef _COREOTN_LOC_H
#define _COREOTN_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_log.h"
#include "pmc_bitarray.h"
#include "odu_struct.h"
#include "coreotn.h"
#include "coreotn_fo_io.h"
#include "coreotn_ctl_io.h"
#include "coreotn_fmf_io.h"
#include "coreotn120_mtsb_map.h"
#include "odtu_mux.h"
#include "expand_ibuf.h"
#include "expand_ibuf_io.h"
#include "cpact_obuf_io.h"


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: coreotn_dci_mux1_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the source of MUX1 control.
*
*   The element terms have a syntax:
*      COREOTN_DCI_MUX1_<dci_source>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    COREOTN_DCI_MUX1_ODU_RFRM_12    = 0,
    COREOTN_DCI_MUX1_HO_DCI_INTF    = 1,
    LAST_COREOTN_DCI_MUX1
} coreotn_dci_mux1_src_t;

/*******************************************************************************
* ENUM: coreotn_dci_mux2_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the source of MUX2 control.
*
*   The element terms have a syntax:
*      COREOTN_DCI_MUX2_<dci_source>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    COREOTN_DCI_MUX2_ODTU_DMX       = 0,
    COREOTN_DCI_MUX2_COREOTN_CTL    = 1,
    LAST_COREOTN_DCI_MUX2
} coreotn_dci_mux2_src_t;

/*******************************************************************************
* ENUM: coreotn_dci_mux5_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the source of MUX5 control.
*
*   The element terms have a syntax:
*      COREOTN_DCI_MUX5_<dci_source>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    COREOTN_DCI_MUX5_LO_DCI_INTF    = 0,
    COREOTN_DCI_MUX5_MUX3_CTRL      = 1,
    LAST_COREOTN_DCI_MUX5
} coreotn_dci_mux5_src_t;  

/*
** Constants
*/

#define COREOTN_MPMO_TOC    0x00
#define COREOTN_MPMA_TOC    0x00
#define COREOTN_NUM_CHNL    96
#define COREOTN_MAX_NUM_HO_CHNL 12
#define COREOTN_MAX_NUM_MO_CHNL 96
#define COREOTN_MAX_NUM_LO_CHNL 96
#define COREOTN_MAX_NUM_TS      80
#define COREOTN_DB_ENTRY_UNKNOWN 0xFFFFFFFF

#define COREOTN_ODUKSC_USERBITS_DEL              0xB
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_MPMA_COREOTN              7
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_MPMA_COREOTN_FROM_MAPOTN  8
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_MPMA_MPMO_COREOTN_CTL     9
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_MPMO_COREOTN_MUX_FRAMER   10
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_MPMO_COREOTN_TO_MAPOTN    11
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_COREOTN_ODUKSC1           12
#define COREOTN_UTIL_SCHL_CAL_DIST_METHOD_COREOTN_ODUKSC2           13
#define COREOTN_ODUKSW_NUM_OF_DCS                                   4

#define COREOTN_TCM_PID_VALID_BITARRAY_SIZE  PMC_BITARRAY_SIZE(COREOTN_MAX_NUM_LO_CHNL*ODU_STRUCT_PIDS_PER_CHNL)
/* double to uint conversion limit */
#define COREOTN_UINT32_LIMIT 4200E6
 
/* temporary definitions until codeGen is fixed */
#define COREOTN_FMF_OFFSET 0x100000
#define COREOTN_FO_OFFSET 0x40000

#define COREOTN_ODUKSW_BLOCK_SIZE   768

/* Timer values */

/* wait for 3*1280 frame period */
#define COREOTN_ODU4_FRAME_CNT_WAIT_MAX 4486
/* wait for 3*256 frame period */
#define COREOTN_OTHER_FRAME_CNT_WAIT_MAX 2331
#define COREOTN_120US_SLEEP_TIME 120
#define COREOTN_2CAL_CYCLES_SLEEP_TIME 1


/* Polling count for page swap */
#ifdef PMC_SW_SIMULATION
#define COREOTN_POLL_COUNT_MAX 1
#else /* else */
#define COREOTN_POLL_COUNT_MAX 20000
#endif /* #ifdef PMC_SW_SIMULATION */

/*#define COREOTN_800US_SLEEP_TIME 800*/
#define COREOTN_TRIB_WRD_CNT 3

/*
** Macro Definitions
*/
#define COREOTN_PRBS_MON_DISABLED    0x7F
#define COREOTN_OCPB_NUM_PORT        5


#define COREOTN_PRBS_MON_CHNL_GET(handle, prbs_inst, prbs_chnl)                 \
    (handle->var.prbs_chnl_prov[prbs_inst][prbs_chnl])

#define COREOTN_PRBS_MON_CHNL_SET(handle, prbs_inst, prbs_chnl, dci_chnl)       \
do {                                                                            \
    handle->var.prbs_chnl_prov[prbs_inst][prbs_chnl] = dci_chnl;                \
} while (0)

#define COREOTN_PRBS_MON_IS_ENABLED(handle, prbs_inst, prbs_chnl)               \
    (COREOTN_PRBS_MON_CHNL_GET(handle, prbs_inst, prbs_chnl) != COREOTN_PRBS_MON_DISABLED)

#define COREOTN_PRBS_MON_ENABLE(handle, prbs_inst, prbs_chnl, dci_chnl, enable)           \
do {                                                                            \
    if (enable) {                                                               \
        COREOTN_PRBS_MON_CHNL_SET(handle, prbs_inst, prbs_chnl, dci_chnl); \
    } else {                                                                    \
        COREOTN_PRBS_MON_CHNL_SET(handle, prbs_inst, prbs_chnl, COREOTN_PRBS_MON_DISABLED); \
    }                                                                           \
} while (0)


#define S_OLD_RATIO 35


/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: coreotn_oduksc_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining ODUKSC configuration parameters for each instantiation.
*
* ELEMENTS:
*       max_chnl         - defining maximum number of channels in the context of
*                          a parent
*       userbits_del     - defining userbits 
*
*******************************************************************************/
typedef struct coreotn_oduksc_cfg_struct_t {   
    UINT32 max_chnl;   
    UINT32 userbits_del;  
}  coreotn_oduksc_cfg_struct_t;

/* coreotn FMF energy structure */
/*******************************************************************************
* STRUCTURE: coreotn_fmf_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the energy state of COREOTN FMF1 macro.
*
* ELEMENTS:
*       prbs        - energy state of PRBS block
*       oduksc      - energy state of ODUKSC block
*       odujat      - energy state of ODUJAT block
*       frm_rcp96   - energy state of FRM_RCP96 block
*       frm_rcp12   - energy state of FRM_RCP12 block
*       rfrm12      - energy state of ODU_RFRM12 block
*       tfrm12      - energy state of ODU_TFRM12 block
*       rfrm96      - energy state of ODU_RFRM96 block
*       tfrm96      - energy state of ODU_TFRM96 block
*       odtu_dmx    - energy state of ODTU_DMX block
*       odtu_mux    - energy state of ODTU_MUX block
*
*******************************************************************************/
typedef struct coreotn_fmf_energy_state_struct_t { 
    pmc_energy_state_t prbs;
    pmc_energy_state_t oduksc;
    pmc_energy_state_t odujat;
    pmc_energy_state_t frm_rcp96;
    pmc_energy_state_t frm_rcp12;    
    pmc_energy_state_t rfrm12;
    pmc_energy_state_t tfrm12;
    pmc_energy_state_t rfrm96;
    pmc_energy_state_t tfrm96;
    pmc_energy_state_t odtu_dmx;   
    pmc_energy_state_t odtu_mux;   
} coreotn_fmf_energy_state_t;

/* coreotn FO energy structure */
/*******************************************************************************
* STRUCTURE: coreotn_fo_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the energy state of COREOTN FO macro.
*
* ELEMENTS:
*       ohfs_remove - energy state of OHFS_REMOVE block
*       frm_rcp     - energy state of FRM_RCP block
*       prbs        - energy state of PRBS block
*       mpmo        - energy state of MPMO block
*       mpma        - energy state of MPMA block
*       tfrm        - energy state of ODU_TFRM96 block
*       rfrm        - energy state of ODU_RFRM96 block
*
*******************************************************************************/
typedef struct coreotn_fo_energy_state_struct_t { 
    pmc_energy_state_t ohfs_remove;
    pmc_energy_state_t frm_rcp;
    pmc_energy_state_t prbs;
    pmc_energy_state_t mpmo;
    pmc_energy_state_t mpma;
    pmc_energy_state_t tfrm;
    pmc_energy_state_t rfrm;
} coreotn_fo_energy_state_t;

/* coreotn FO energy structure */

/*******************************************************************************
* STRUCTURE: coreotn_ctl_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the energy state of COREOTN FO macro.
*
* ELEMENTS: 
*       mpmo        - energy state of MPMO block
*       mpma        - energy state of MPMA block
*       ohp3        - energy state of ODU_OHP3 block
*       ohp2        - energy state of ODU_OHP2 block 
*       ohp1        - energy state of ODU_OHP1 block
*
*******************************************************************************/
typedef struct coreotn_ctl_energy_state_struct_t 
{ 
    pmc_energy_state_t mpmo;
    pmc_energy_state_t mpma;
    pmc_energy_state_t ohp3;
    pmc_energy_state_t ohp2;
    pmc_energy_state_t ohp1;
} coreotn_ctl_energy_state_t;

/* coreotn COREOTN energy structure */
/*******************************************************************************
* STRUCTURE: coreotn_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the energy state of COREOTN macros.
*
* ELEMENTS: 
*       fmf1        - energy state of FMF1 macro
*       fmf2        - energy state of FMF2 macro
*       fo1         - energy state of FO1 macro
*       fo2         - energy state of FO2 macro 
*       coreotn_ctl - energy state of COREOTN_CTRL macro
*
*******************************************************************************/
typedef struct coreotn_energy_state_struct_t { 
    coreotn_fmf_energy_state_t fmf1;
    coreotn_fmf_energy_state_t fmf2;
    coreotn_fo_energy_state_t fo1;
    coreotn_fo_energy_state_t fo2;
    coreotn_ctl_energy_state_t coreotn_ctl; 
} coreotn_energy_state_t;

/* This struct is to illustrate the fixed context. */
/*******************************************************************************
* STRUCTURE: coreotn_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the fixed properties of COREOTN.
*
* ELEMENTS: 
*       num_coreotn_chnl        - maximum number of COREOTN channels
*       schdpopulate_mode       - mode of ODUKSW DCS scheduler population method
*       rx_db_handle            - handle to COREOTN RX database 
*       tx_db_handle            - handle to COREOTN TX database
*       rx_db_id                - COREOTN RX database id
*       tx_db_id                - COREOTN TX database id
*       rx_mapotn_db_handle     - handle to MAPOTN RX database
*       tx_mapotn_db_handle     - handle to MAPOTN TX database
*       rx_mapotn_db_id         - MAPOTN RX database id
*       tx_mapotn_db_id         - MAPOTN TX database id
*       lineotn_sysotn_card_db_handle   - handle to LINEOTN database in SYSOTN application
*       lineotn_sysotn_card_db_id       - LINEOTN database id in SYSOTN application
*
*******************************************************************************/
typedef struct {
    
    /* To add static COREOTN config context */
    UINT32                          num_coreotn_chnl;
    cpb_schd_populate_t             schdpopulate_mode[COREOTN_ODUKSW_NUM_OF_DCS];
    
    UINT32                          rx_db_handle;
    UINT32                          tx_db_handle;
    util_gen_db_id_t                rx_db_id;    
    util_gen_db_id_t                tx_db_id;

    UINT32                          rx_mapotn_db_handle;
    UINT32                          tx_mapotn_db_handle;
    util_gen_db_id_t                rx_mapotn_db_id;    
    util_gen_db_id_t                tx_mapotn_db_id;

    UINT32                          lineotn_sysotn_card_db_handle;
    util_gen_db_id_t                lineotn_sysotn_card_db_id;

    
} coreotn_cfg_t;

/*******************************************************************************
* STRUCTURE: coreotn_chnl_ho_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the HO ODU channel context.
*
* ELEMENTS: 
*       chnl_state          - enum, HO ODU channel state 
*       odu_container       - enum, HO ODU container size
*       payload_format      - payload format of the HO ODU channel
*       chnls_prov_cnt      - number of provisioned channel in the current ODU layer
*       sub_chnls_prov_cnt  - number of channels in the sub ODU layer
*
*******************************************************************************/
typedef struct {
    coreotn_chnl_state_t chnl_state;
    util_global_odukp_type_t odu_container;
    util_global_odu_line_payload_t payload_format;
    UINT8 chnls_prov_cnt;   
    UINT8 sub_chnls_prov_cnt;
} coreotn_chnl_ho_ctxt_t;

/*******************************************************************************
* STRUCTURE: coreotn_chnl_molo_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the MO or LO ODU channel context.
*
* ELEMENTS: 
*       chnl_state          - enum, ODU channel state 
*       odu_container       - enum, ODU container size
*       lo_odu_rate         - enum, sub layer ODU container size
*       odu_flex_rate       - GFP rate in the ODU in case of ODUFlex container
*       cbr_flex_rate       - CBR rate in the ODU in case of ODUFlex container 
*       payload_format      - payload format of the ODU channel
*       mapping_mode        - mapping mode to map LO ODU to a higher order ODU channel
*       tribslot_mask       - mask descriting all the tributary slots assigned
*                             for this channel \n
*       trib_port           - tributary port number assigned for the channel
*       chnls_prov_cnt      - number of channels provisioned in the current layer 
*       sub_chnls_prov_cnt  - number of channels in the sub ODU layer
*       chnl_cfg_state
*******************************************************************************/
typedef struct {
    coreotn_chnl_state_t chnl_state;
    util_global_odukp_type_t odu_container; 
    util_global_odukp_type_t lo_odu_rate;   
    UINT32 odu_flex_rate;
    DOUBLE cbr_flex_rate;
    util_global_odu_line_payload_t payload_format;
    coreotn_mapping_type_t mapping_mode;
    UINT32 tribslot_mask[3];
    UINT32 trib_port;
    UINT8 chnls_prov_cnt;   
    UINT8 sub_chnls_prov_cnt;
    coreotn_lo_odu_chnl_cfg_state_t chnl_cfg_state;
} coreotn_chnl_molo_ctxt_t;

/*******************************************************************************
* STRUCTURE: coreotn_oduksw_chnl_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the channel context across ODUKSW
*
* ELEMENTS: 
*       state               - enum, ODUKSW connection state
*       input_map_count     - number of primary channel mapped 
*       dest_active_count   - number of destination channels activated
*
*******************************************************************************/
typedef struct {
    coreotn_chnl_state_t state;
    UINT8 input_map_count;
    UINT8 dest_active_count;
} coreotn_oduksw_chnl_ctxt_t;

/*******************************************************************************
* STRUCTURE: coreotn_odtu_mux_mo_odu3e2_3e1_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the context used for MO ODU3e1 or MO ODU3e2
*
* ELEMENTS: 
*       chnl_id         - channel ID of the MO ODU3e2 or MO ODU3e1
*       cycle           - an extra cycle that was used in MO ODU3e2 or MO ODU3e1
*       entry_state     - boolean value to indicate the state of the extra cycle
*
*******************************************************************************/
typedef struct {
    UINT8 chnl_id;
    UINT8 cycle;
    BOOL8 entry_state;
} coreotn_odtu_mux_mo_odu3e2_3e1_ctxt_t;

/*******************************************************************************
* STRUCTURE: coreotn_mux_profile_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the multiflexing profile of the datapath
*   This structure is only used when bulk provisioning/deprovisioning is
*   enabled and when the total number of ODU channels to be provisioned is known
*   at the start of bulk provisioning.
*
* ELEMENTS: 
*       num_mo_chnls        - number of MO ODU channels to be provisioned in bulk
*       num_lo_chnls        - number of LO ODU channels to be provisioned in bulk
*
*******************************************************************************/
typedef struct {
    UINT8 num_mo_chnls; 
    UINT8 num_lo_chnls; 
} coreotn_mux_profile_t;


/*******************************************************************************
* STRUCTURE: coreotn_ddeg_mon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the context used in dDEG monitoring feature
*
* ELEMENTS: 
*       ddeg_mode       - enum, dDEG monitoring mode
*       mon_status      - boolean value to indicate if monitoring is enabled or
*                         disabled
*
*******************************************************************************/
typedef struct{
    coreotn_ddeg_mode_t ddeg_mode;
    BOOL8               mon_status;
} coreotn_ddeg_mon_t;


/* This struct is to illustrate the dynamically changing context. */
/*******************************************************************************
* STRUCTURE: coreotn_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the dynamically changing properties of COREOTN.
*
* ELEMENTS: 
*       init_operation      - enum, COREOTN subsystem power initialization mode
*       prov_mode           - provisioning mode of ODU channels
*       mux_profile         - structure to indicate multiplexing profile of the datapath
*       ilkn_if_enable      - boolean value to indicate if the Interlaken interface
*                             is enabled or not
*       coreotn_start_state - boolean value to indicate if COREOTN has been
*                             intialized already.
*       rx_ho_chnl_ctxt     - array of HO channel context in RX direction
*       tx_ho_chnl_ctxt     - array of HO channel context in TX direction           
*       rx_mo_chnl_ctxt     - array of MO channel context in RX direction
*       tx_mo_chnl_ctxt     - array of MO channel context in TX direction
*       rx_lo_chnl_ctxt     - array of LO channel context in RX direction
*       tx_lo_chnl_ctxt     - array of LO channel context in TX direction
*       rx_oduksw_chnl_ctxt - array of ODUKSW channel context in RX direction
*       tx_oduksw_chnl_ctxt - array of ODUKSW channel context in RX direction
*       tx_stg4_chnl_prov   - array of boolean value to indicate if STG4 channel
*                             is provisioned in TX direction
*       rx_stg4_chnl_prov   - array of boolean value to indicate if STG4 channel
*                             is provisioned in RX direction
*       sysapp_line_state   - array of boolean value to indicate if COREOTN to
*                             LINEOTN connection has been provisioned in SYSOTN application.
*       ddeg_mon            - enum, dDEG monitoring mode
*       ddeg_mon_seg        - enum, dDEG monitoring segment
*       extra_entry_ctxt    - an array of extra entries marked for MO ODU3e2/ODU3e1
*       stg3b_odu_level     - an array to track the ODU layer attached to STG3B ODU framers
*       tcm_pid_valid       - bit array to track if a TCM Pool ID is used or not
*       prbs_chnl_prov      - 2D array to track PRBS channel provisioning state
*       prev_pm_mode        - 2D array to store previous PM provisioning mode
*       prev_fo1_rfrm_maint_sig - an array to track previously inserted maintenance signal
*       dummy_fo1_maint_sig     - an array of boolean to indicate if a maintenance
*                                 signal is inserted to support maintenace signal
*                                 insertion test in open ODUKSW connection
*       prev_fmf1_sysotn_rfrm_maint_sig - an array to track previously inserted
*                                         maintenance signal
*       dummy_fmf1_sysotn_maint_sig - an array of boolean to indicate if a maintenance
*                                     signal is inserted to support maintenace signal
*                                     insertion test in open ODUKSW connection 
*       dcs_to_mapotn_pinch_entry   - an array of boolean to indicate if DCS to
*                                     MAPOTN pinching has been applied or not.
*
*******************************************************************************/
typedef struct {       
    coreotn_init_operation_t init_operation;
    coreotn_odu_chnl_prov_mode_t prov_mode;
    coreotn_mux_profile_t mux_profile[COREOTN_MAX_NUM_HO_CHNL];
    BOOL8 ilkn_if_enable;
    BOOL8 coreotn_start_state;
    
    coreotn_chnl_ho_ctxt_t rx_ho_chnl_ctxt[COREOTN_MAX_NUM_HO_CHNL];
    coreotn_chnl_ho_ctxt_t tx_ho_chnl_ctxt[COREOTN_MAX_NUM_HO_CHNL];
    coreotn_chnl_molo_ctxt_t rx_mo_chnl_ctxt[COREOTN_MAX_NUM_MO_CHNL];
    coreotn_chnl_molo_ctxt_t tx_mo_chnl_ctxt[COREOTN_MAX_NUM_MO_CHNL];
    coreotn_chnl_molo_ctxt_t rx_lo_chnl_ctxt[COREOTN_MAX_NUM_LO_CHNL];
    coreotn_chnl_molo_ctxt_t tx_lo_chnl_ctxt[COREOTN_MAX_NUM_LO_CHNL];
    coreotn_oduksw_chnl_ctxt_t rx_oduksw_chnl_ctxt[COREOTN_OCPB_NUM_PORT][COREOTN_MAX_NUM_LO_CHNL];
    coreotn_oduksw_chnl_ctxt_t tx_oduksw_chnl_ctxt[COREOTN_OCPB_NUM_PORT][COREOTN_MAX_NUM_LO_CHNL];
    BOOL8                tx_stg4_chnl_prov[COREOTN_MAX_NUM_LO_CHNL];
    BOOL8                rx_stg4_chnl_prov[COREOTN_MAX_NUM_LO_CHNL];
    coreotn_chnl_state_t sysapp_line_state[COREOTN_MAX_NUM_HO_CHNL];
    coreotn_ddeg_mon_t   ddeg_mon[COREOTN_NUM_DDEG_MOD];  
    coreotn_ddeg_mon_seg_t ddeg_mon_seg;
    coreotn_odtu_mux_mo_odu3e2_3e1_ctxt_t extra_entry_ctxt[UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD];

    /* Tracks which ODU level the stg3b framer is attached to */
    odu_struct_odu_level_t  stg3b_odu_level[COREOTN_MAX_NUM_LO_CHNL];

    /* Tracks if the tcm_pids are in use or not */
    pmc_bitarray_t  tcm_pid_valid[LAST_ODU_STRUCT_LEVEL][COREOTN_TCM_PID_VALID_BITARRAY_SIZE];

    /* Tracks which Framer channel (0-95) each PRBS (FMF1/2, FO1/2, 0-11) is attached to */
    UINT8 prbs_chnl_prov[LAST_COREOTN_PRBS_INST][COREOTN_PRBS_NUM_CHNL];
    odu_tfrm_pm_tcm_mode_t prev_pm_mode[LAST_COREOTN_PRBS_INST][COREOTN_MAX_NUM_LO_CHNL];

    UINT8 prev_fo1_rfrm_maint_sig[COREOTN_MAX_NUM_LO_CHNL];
    BOOL8 dummy_fo1_maint_sig[COREOTN_MAX_NUM_LO_CHNL];

    UINT8 prev_fmf1_sysotn_rfrm_maint_sig[COREOTN_MAX_NUM_HO_CHNL];
    BOOL8 dummy_fmf1_sysotn_maint_sig[COREOTN_MAX_NUM_HO_CHNL];
    
    BOOL8 dcs_to_mapotn_pinch_entry[COREOTN_MAX_NUM_LO_CHNL];

} coreotn_var_t;

/*******************************************************************************
* STRUCTURE: coreotn_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the context used in COREOTN context recovery
*
* ELEMENTS: 
*       var             - enum, dDEG monitoring mode
*       ocpb_connect    - structure to describe ODUKSW connection data
*       dmx1            - structure to describe ODTU_DMX1 block context
*       dmx2            - structure to describe ODTU_DMX1 block context       
*       mux1            - structure to describe ODTU_DMX1 block context
*       mux2            - structure to describe ODTU_DMX1 block context
*       energy_state_reg    - structure to describe the energy state of COREOTN 
*       stg4_energy         - structure to describe the energy state of COREOTN STG4 blocks
*       odukp_energy        - structure to describe blocks related to ODUK layer
*
*******************************************************************************/
typedef struct {
    coreotn_var_t           var;    
    util_global_cpb_connect_t ocpb_connect;
    odtu_dmx_recover_var_t  dmx1;
    odtu_dmx_recover_var_t  dmx2;
    odtu_mux_recover_var_t  mux1; 
    odtu_mux_recover_var_t  mux2; 
    coreotn_energy_state_t  energy_state_reg;
    pmc_energy_state_t      stg4_energy;
    pmc_energy_state_t      odukp_energy;    
} coreotn_recover_var_t;

/*******************************************************************************
* STRUCTURE: coreotn_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the context of COREOTN handle
*
* ELEMENTS:
*       base                    - base address of the COREOTN handle
*       odu_rfrm_stg1_handle    - handle to STG1 ODU_RFRM block
*       odu_rfrm_stg2_handle    - handle to STG2 ODU_RFRM block
*       odu_rfrm_stg3a_handle   - handle to STG3a ODU_RFRM block
*       odu_rfrm_stg3b_handle   - handle to STG3b ODU_RFRM block
*       odu_rfrm_stg4_handle    - handle to STG4 ODU_RFRM block
*       odu_rfrm_stgN_handle    - handle to STGN ODU_RFRM block
*       odu_tfrm_stg1_handle    - handle to STG1 ODU_TFRM block
*       odu_tfrm_stg2_handle    - handle to STG2 ODU_TFRM block
*       odu_tfrm_stg3a_handle   - handle to STG3a ODU_TFRM block
*       odu_tfrm_stg3b_handle   - handle to STG3b ODU_TFRM block
*       odu_tfrm_stg4_handle    - handle to STG4 ODU_TFRM block
*       odu_tfrm_stgN_handle    - handle to STGN ODU_TFRM block
*       odtu_dmx1_handle        - handle to ODTU_DMX1 block
*       odtu_dmx2_handle        - handle to ODTU_DMX2 block
*       odtu_mux1_handle        - handle to ODTU_MUX1 block
*       odtu_mux2_handle        - handle to ODTU_MUX2 block
*       mpma_fo1_handle         - handle to FO1::MPMA block
*       mpma_fo2_handle         - handle to FO2::MPMA block
*       mpma_core_ctl_handle    - handle to CTRL::MPMA block
*       mpmo_fo1_handle         - handle to FO1::MPMO block
*       mpmo_fo2_handle         - handle to FO2::MPMO block
*       mpmo_core_ctl_handle    - handle to CTRL::MPMO block
*       oduksc1_handle          - handle to ODUKSC1 block
*       oduksc2_handle          - handle to ODUKSC2 block
*       odujat1_handle          - handle to ODUJAT1 block
*       odujat2_handle          - handle to ODUJAT2 block
*       oduksw_handle           - handle to ODUKSW block
*       odu_rcp_handle          - handle to ODU_RCP block
*       odu_ohp_handle          - handle to ODU_OHP block
*       cfg - structure to describe fixed context of COREOTN
*       var - structure to describe the dynamically changing context of COREOTN
*******************************************************************************/
typedef struct coreotn_handle_struct_t 
{
    pmc_handle_t base;
    
    /* ODU RFRM */
    odu_rfrm_handle_t *odu_rfrm_stg1_handle;
    odu_rfrm_handle_t *odu_rfrm_stg2_handle;
    odu_rfrm_handle_t *odu_rfrm_stg3a_handle;
    odu_rfrm_handle_t *odu_rfrm_stg3b_handle;
    odu_rfrm_handle_t *odu_rfrm_stg4_handle;
    odu_rfrm_handle_t *odu_rfrm_stgN_handle;
    
    /* ODU TFRM */
    odu_tfrm_handle_t *odu_tfrm_stg1_handle;
    odu_tfrm_handle_t *odu_tfrm_stg2_handle;
    odu_tfrm_handle_t *odu_tfrm_stg3a_handle;
    odu_tfrm_handle_t *odu_tfrm_stg3b_handle;
    odu_tfrm_handle_t *odu_tfrm_stg4_handle;
    odu_tfrm_handle_t *odu_tfrm_stgN_handle;
    
    /* ODTU_DMX */
    odtu_dmx_handle_t *odtu_dmx1_handle;
    odtu_dmx_handle_t *odtu_dmx2_handle;
    
    /* ODTU_MUX */
    odtu_mux_handle_t *odtu_mux1_handle;
    odtu_mux_handle_t *odtu_mux2_handle;
    
    /* MPMA */
    mpma_handle_t *mpma_fo1_handle;
    mpma_handle_t *mpma_fo2_handle;
    mpma_handle_t *mpma_core_ctl_handle;
    
    /* MPMO */
    mpmo_handle_t *mpmo_fo1_handle;    
    mpmo_handle_t *mpmo_fo2_handle;    
    mpmo_handle_t *mpmo_core_ctl_handle;
    
    /* ODUKSC */
    oduksc_handle_t *oduksc1_handle;
    oduksc_handle_t *oduksc2_handle;
    
    /* ODUJAT */
    odujat_handle_t *odujat1_handle;
    odujat_handle_t *odujat2_handle;
    
    /* ODUk Switch */
    cpb_handle_t *oduksw_handle;

    /* ODU RCP */
    odu_rcp_handle_t *odu_rcp_handle;
    
    /* ODU OHP */
    odu_ohp_handle_t *odu_ohp_handle[COREOTN_NUM_ODU_OHP_MAX];
    
    coreotn_cfg_t cfg;
    coreotn_var_t var;
     
} coreotn_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

                                                   
#ifdef __cplusplus
}
#endif

#endif /* _COREOTN_LOC_H */

/* 
** end of file 
*/
