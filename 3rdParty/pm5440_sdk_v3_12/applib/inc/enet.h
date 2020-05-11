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

#ifndef _ENET_H
#define _ENET_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "enet_api.h"
#include "emac.h"
#include "epmm.h"
#include "empei_e.h"
#include "empei_i.h"
#include "etrans.h"
#include "cfc.h"
#include "cfc_api.h"
#include "util_global.h"
#include "util_opsa.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: enet_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ENET channel state definition.
*
* ELEMENTS:
*   ENET_LNK_START         - start state
*   ENET_LNK_EQUIPPED      - provisioned state
*   ENET_LNK_OPERATIONAL   - activated and operational state
*           
*******************************************************************************/
typedef enum {
    ENET_LNK_START        = 0,
    ENET_LNK_EQUIPPED     = 1,
    ENET_LNK_OPERATIONAL  = 2,
    LAST_ENET_LNK_STATE
} enet_lnk_state_t;

/*******************************************************************************
* ENUM: enet_type_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ENET instance
*
* ELEMENTS:
*   ENET_LINE - enet line subsystem
*   ENET_SYS  - enet system  subsystem
*
*******************************************************************************/
typedef enum {
    ENET_LINE = 0x00,
    ENET_SYS  = 0x01
} enet_type_id_t;

/*******************************************************************************
* ENUM: enet_block_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ENET sub-blocks
*
* ELEMENTS:
*   ENET_<blk>_BLK  - when blk = ETRANS, 
*                                EPMM, 
*                                EMPEI_E, 
*                                EMPEI_E_ECLASS, 
*                                EMPEI_I, 
*                                EMPEI_I_ECLASS, 
*                                EMAC, 
*                                ETIME, 
*
*******************************************************************************/
typedef enum {
    ENET_ETRANS_BLK         = 0x0,
    ENET_EPMM_BLK           = 0x1,
    ENET_EMPEI_E_BLK        = 0x2,
    ENET_EMPEI_E_ECLASS_BLK = 0x3,
    ENET_EMPEI_I_BLK        = 0x4,
    ENET_EMPEI_I_ECLASS_BLK = 0x5,
    ENET_EMAC_BLK           = 0x6,
    ENET_ETIME_BLK          = 0x7,

    LAST_ENET_BLOCK    /* For out of range value handling */
} enet_block_t;

/*******************************************************************************
* ENUM: enet_datapath_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET_LINE major processing modes.  The
*   element terms have a syntax:
*      ENET_<line interface>_<application>
*
* ELEMENTS:
*   ENET_SERDES_100GE_MAC                     - Serdes 100GE MAC Terminated
*   ENET_SERDES_40GE_MAC                      - Serdes 40GE MAC Terminated
*   ENET_SERDES_10GE_MAC                      - Serdes 10GE MAC Terminated
*   ENET_SERDES_100GE_TRANSPARENT             - Serdes 100GE CBR
*   ENET_SERDES_40GE_TRANSPARENT              - Serdes 40GE CBR
*   ENET_SERDES_10GE_TRANSPARENT              - Serdes 10GE CBR
*   ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT   - Serdes 10GE Gsup43 7.3
*
*   ENET_SERDES_100GE_TRANSPARENT_RX_PMON            - Serdes 100GE CBR with PMON using Rx MAC
*   ENET_SERDES_40GE_TRANSPARENT_RX_PMON             - Serdes 40GE CBR with PMON using Rx MAC
*   ENET_SERDES_10GE_TRANSPARENT_RX_PMON             - Serdes 10GE CBR with PMON using Rx MAC
*   ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON  - Serdes 10GE Gsup43 7.3 with PMON using Rx MAC
*
*   ENET_SLAVE_100GE_TRANSPARENT_TX_PMON            - 100GE Tx EPMM traffic looped back to Rx MAC for PMON only
*   ENET_SLAVE_40GE_TRANSPARENT_TX_PMON             - 40GE Tx EPMM traffic looped back to Rx MAC for PMON only
*   ENET_SLAVE_10GE_TRANSPARENT_TX_PMON             - 10GE Tx EPMM traffic looped back to Rx MAC for PMON only
*   ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON  - 10GE Gsup43 7.3 Tx EPMM traffic looped back to Rx MAC for PMON only
*
*   ENET_SLAVE_100GE_MAC_TX_PMON              - 100GE Tx MAC traffic looped back to Rx MAC for PMON only
*   ENET_SLAVE_40GE_MAC_TX_PMON               - 40GE Tx MAC traffic looped back to Rx MAC for PMON only
*   ENET_SLAVE_10GE_MAC_TX_PMON               - 10GE Tx MAC traffic looped back to Rx MAC for PMON only

*   ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC      - MAPOTN 100GE GFP mapped Gsup 43 6.2 MAC Terminated
*   ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC       - MAPOTN 40GE GFP mapped Gsup 43 6.2 MAC Terminated
*   ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC       - MAPOTN 10GE Gsup43 6.2 GFP mapped MAC Terminated
*   ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC       - MAPOTN 10GE Gsup43 7.3 GFP mapped MAC Terminated
*   ENET_MAPOTN_GMP_G709_100GE_MAC            - MAPOTN 100GE GMP mapped MAC Terminated
*   ENET_MAPOTN_GMP_G709_40GE_MAC             - MAPOTN 40GE GMP mapped MAC Terminated
*   ENET_MAPOTN_BMP_GSUP43_10GE_MAC           - MAPOTN 10GE Gsup43 7.1/7.2 BMP mapped MAC Terminated
*
*   ENET_MAPOTN_GMP_G709_100GE_MAC_PMON       - MAPOTN 100GE GMP mapped MAC Terminated for PMON only
*   ENET_MAPOTN_GMP_G709_40GE_MAC_PMON        - MAPOTN 40GE GMP mapped MAC Terminated for PMON only 
*   ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON      - MAPOTN 10GE Gsup43 7.1/7.2 BMP mapped MAC Terminated for PMON only
*   ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON  - MAPOTN 10GE Gsup43 7.3 GFP mapped MAC Terminated for PMON only
*   ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON - MAPOTN 100GE GFP mapped Gsup 43 6.2 MAC Terminated for PMON only
*   ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON  - MAPOTN 40GE GFP mapped Gsup 43 6.2 MAC Terminated for PMON only
*   ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON  - MAPOTN 10GE Gsup43 6.2 GFP mapped MAC Terminated for PMON only
*
*   ENET_UNPROV_DATAPATH - Unprovision channel. Put channel in reset/low power state
*
*******************************************************************************/
typedef enum
{
    /* SERDES Datapath configuration. */
    ENET_SERDES_100GE_MAC,
    ENET_SERDES_40GE_MAC,
    ENET_SERDES_10GE_MAC,

    ENET_SERDES_100GE_TRANSPARENT,
    ENET_SERDES_40GE_TRANSPARENT,
    ENET_SERDES_10GE_TRANSPARENT,
    ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT,

    /* Datapath with Ingress also routed to the Rx MAC for PMON */
    ENET_SERDES_100GE_TRANSPARENT_RX_PMON,
    ENET_SERDES_40GE_TRANSPARENT_RX_PMON,
    ENET_SERDES_10GE_TRANSPARENT_RX_PMON,
    ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON,

    /* The configurations below are used for PMON only */
    /* CPB -> Tx EPMM -> loopback -> Rx MAC for PMON */
    ENET_SLAVE_100GE_TRANSPARENT_TX_PMON,
    ENET_SLAVE_40GE_TRANSPARENT_TX_PMON,
    ENET_SLAVE_10GE_TRANSPARENT_TX_PMON,
    ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON,

    /* CPB -> Tx MAC -> loopback -> Rx MAC for PMON */
    ENET_SLAVE_100GE_MAC_TX_PMON,
    ENET_SLAVE_40GE_MAC_TX_PMON,
    ENET_SLAVE_10GE_MAC_TX_PMON,

    /* For out of range value handling */
    ENET_DATAPATH_LAST_SERDES_OR_SLAVE,

    /* MAPOTN Datapath configuration. Only valid for ENET_LINE. Not valid for ENET_SYS. */
    ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC,
    ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC,
    ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC,
    ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC,
    ENET_MAPOTN_GMP_G709_100GE_MAC,
    ENET_MAPOTN_GMP_G709_40GE_MAC,
    ENET_MAPOTN_BMP_GSUP43_10GE_MAC,

    /* MAPOTN -> Rx ETRANS -> Rx MAC for PMON */
    ENET_MAPOTN_GMP_G709_100GE_MAC_PMON,
    ENET_MAPOTN_GMP_G709_40GE_MAC_PMON,
    ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON,
    ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON,
    ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON,
    ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON,
    ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON,

    /* Unprovision channel. Put channel in reset/low power state. */
    ENET_UNPROV_DATAPATH,  

    /* For out of range value handling */
    LAST_ENET_DATAPATH    
} enet_datapath_t;

/*******************************************************************************
* ENUM: enet_schd_populate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the ENET scheduler 
*    content source generation mode.
*
* ELEMENTS:
*           ENET_SCHD_POPULATE_LOCAL  - Use local scheduler content generation
*                                      method
*           ENET_SCHD_POPULATE_DB     - Use database contents to generate
*                                      scheduler contents
*
*******************************************************************************/
typedef enum
{
    ENET_SCHD_POPULATE_LOCAL      = 0,
    ENET_SCHD_POPULATE_DB,

    LAST_ENET_SCHD_POPULATE
} enet_schd_populate_t;


/*
** Constants
*/
#define ENET_NUM_LINK_MAX   12
#define ENET_NUM_CHNK_MAX   96
#define ENET_NUM_LINK_10GE  1
#define ENET_NUM_LINK_40GE  4
#define ENET_NUM_LINK_100GE 10 

#define ENET_40GE_LINK_1    0
#define ENET_40GE_LINK_2    4
#define ENET_40GE_LINK_3    8

#define ENET_NUM_SCHD     1

#define ENET_MAPOTN_CHANNEL_NONE 0

#define ENET_PCS_POLL_COUNT_MASK    0xFF000000
#define ENET_PCS_POLL_COUNT_OFFSET  24

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: enet_schd_use_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for schedule context 
*
* ELEMENTS:
*    alloc_mode                - scheduler allocation mode
*    db_id                     - database ID
*    db_client_handle          - database handle
* 
*******************************************************************************/
typedef struct {        
    util_schd_alloc_method_t alloc_mode;
    util_gen_db_id_t db_id;
    UINT32 db_client_handle;    
} enet_schd_use_ctxt_t;
   
/*******************************************************************************
* STRUCTURE: enet_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing basic configuration information for any ENET
*   instance.
*
* ELEMENTS:
*    enet_type_id             - ENET LINE or SYS
*    schd_populate_mode       - scheuler mode
*    schd_use_ctxt            - scheduler context 
* 
*******************************************************************************/
typedef struct enet_cfg_struct_t{          
    enet_type_id_t enet_type_id;
    enet_schd_populate_t schd_populate_mode;
    enet_schd_use_ctxt_t schd_use_ctxt;    
} enet_cfg_t;

/*******************************************************************************
* STRUCTURE: enet_lnk_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining link context information.
*
* ELEMENTS:
*    state             - defining the state of the link
*   dpi_sch_slot       - DPI slot number 
*   empei_e_eclass_en  - enable egress EMPEI 
*   empei_i_eclass_en  - enable ingress EMPEI 
*   ing_ts_en          - when TRUE ETIME ingress datatpath is enabled,
*                        otherwise it is disabled.
*   egr_ts_en          -  when TRUE ETIME ingress datatpath is enabled,
*                        otherwise it is disabled.
*
*******************************************************************************/
typedef struct enet_lnk_ctxt_struct_t {    
    enet_lnk_state_t               rx_state; 
    enet_lnk_state_t               tx_state; 
    BOOL8                          empei_e_eclass_en;
    BOOL8                          empei_i_eclass_en;
    BOOL8                          ing_ts_en;
    BOOL8                          egr_ts_en;

} enet_lnk_ctxt_t;  

/*******************************************************************************
* STRUCTURE: enet_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*    lnks_prov_cnt            - how many links  are provisionned
*    ing_ts_en                - timestamp, global enable for ingress RX direction
*    dpi_sch_slot             - DPI scheduler config
*
*******************************************************************************/
typedef struct enet_var_struct_t {
    UINT32          lnks_prov_cnt;
    enet_lnk_ctxt_t lnk_ctxt[ENET_NUM_LINK_MAX];
    UINT32          dpi_sch_slot[ENET_NUM_LINK_MAX];
    enet_datapath_t ch_datapath[ENET_NUM_LINK_MAX];
    BOOL8           enet_start_state;
    BOOL8           not_first_reset;
} enet_var_t;


/*******************************************************************************
* STRUCTURE: enet_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*
*******************************************************************************/
typedef struct enet_recover_var_struct_t {
    enet_var_t      var;
    BOOL8           has_emac_link;
    BOOL8           has_etrans_link;
    BOOL8           has_epmm_link;
    BOOL8           has_empei_i_link;
    BOOL8           has_empei_e_link;
    emac_data_rate_t emac_link_rate[ENET_NUM_LINK_MAX];
    etrans_datapath_t etrans_rx_link_datapath[ENET_NUM_LINK_MAX];
    etrans_datapath_t etrans_tx_link_datapath[ENET_NUM_LINK_MAX];
    epmm_datapath_t epmm_rx_link_datapath[ENET_NUM_LINK_MAX];
    epmm_datapath_t epmm_tx_link_datapath[ENET_NUM_LINK_MAX];
    scbs3_page_t epmm_mpma_scbs3_page;
    scbs3_page_t epmm_mpmo_scbs3_page;
} enet_recover_var_t;

/*******************************************************************************
* STRUCTURE: enet_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining ENET  handle information.
*
* ELEMENTS: 
*  
*
*******************************************************************************/
struct enet_handle_struct_t {
    pmc_handle_t base;
    
    enet_cfg_t    cfg;
    enet_var_t    var;
    
    /* Child handles needed for ENET */ 
    emac_handle_t    *emac_handle;
    epmm_handle_t    *epmm_handle;
    empei_e_handle_t *empei_e_handle;
    eclass_handle_t  *empei_e_eclass_handle;
    empei_i_handle_t *empei_i_handle;
    eclass_handle_t  *empei_i_eclass_handle;
    etrans_handle_t  *etrans_handle;
    
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC enet_handle_t *enet_ctxt_create(pmc_handle_t *parent, 
                                       UINT32 base_address, 
                                       pmc_sys_handle_t *sys_handle, 
                                       const char *tsb_name);
PUBLIC void enet_ctxt_destroy(enet_handle_t *enet_handle);
PUBLIC void enet_handle_init(enet_handle_t *enet_handle, 
                             enet_type_id_t enet_type_id, 
                             enet_schd_populate_t schd_populate_mode);
PUBLIC PMC_ERROR enet_db_entry_set( enet_handle_t *enet_handle_ptr,
                                    UINT32 schd_addr,
                                    UINT32 link );
PUBLIC PMC_ERROR enet_db_entry_clear( enet_handle_t *enet_handle_ptr,
                                      UINT32 link );
PUBLIC PMC_ERROR enet_db_entry_get( enet_handle_t *enet_handle_ptr,
                                    UINT32 schd_addr,
                                    UINT32 *link_ptr);
PUBLIC PMC_ERROR enet_db_entry_all_get( enet_handle_t *enet_handle_ptr,
                                        UINT32 link_ptr[96] );
PUBLIC PMC_ERROR enet_db_link_num_entries_get(enet_handle_t *enet_handle_ptr, 
                                              UINT32         link,              
                                              UINT32        *num_of_recs_ptr);
PUBLIC UINT32 enet_base_address_get (enet_handle_t *enet_handle);
PUBLIC PMC_ERROR enet_sw_reset      (enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath);
PUBLIC PMC_ERROR enet_lowpwr        (enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, BOOL8 value);
PUBLIC PMC_ERROR enet_tx_lane_info_get(enet_handle_t *hndl,
                                            UINT32 link,
                                            UINT8 txLaneId[20],
                                            UINT8 txLaneSkewFifoAddr[20]);
PUBLIC PMC_ERROR enet_emac_lf_rf_cfg(enet_handle_t *hndl, UINT32 link, 
                                     BOOL8 auto_fault_handling, 
                                     BOOL8 send_lf, 
                                     BOOL8 send_rf);
PUBLIC PMC_ERROR enet_epmm_lf_cfg   (enet_handle_t *hndl,
                                     UINT32 link,
                                     enet_dir_t   dir,
                                     BOOL8 enable);
PUBLIC PMC_ERROR enet_epmm_lf_cfg_get(enet_handle_t *hndl,
                                     UINT32 link,
                                     enet_dir_t   dir,
                                     BOOL8 *enable);
PUBLIC PMC_ERROR enet_epmm_rf_cfg   (enet_handle_t *hndl,
                                     UINT32 link,
                                     enet_dir_t   dir,
                                     BOOL8 enable);
PUBLIC PMC_ERROR enet_epmm_rf_cfg_get(enet_handle_t *hndl, 
                                     UINT32 link, 
                                     enet_dir_t   dir,
                                     BOOL8 *enable);
PUBLIC PMC_ERROR enet_epmm_lf_set_auto_insert_enable(enet_handle_t *hndl, 
                                                     UINT32 link, 
                                                     enet_dir_t   dir,
                                                     BOOL8 enable);
PUBLIC PMC_ERROR enet_epmm_lf_set_auto_insert_enable_get(enet_handle_t *hndl,
                                                     UINT32 link,
                                                     enet_dir_t   dir,
                                                     BOOL8 *enable);
PUBLIC PMC_ERROR enet_epmm_rf_set_auto_insert_enable(enet_handle_t *hndl, 
                                                     UINT32         link, 
                                                     enet_dir_t     dir,
                                                     BOOL8           enable);

PUBLIC PMC_ERROR enet_epmm_lf_is_force_enable(enet_handle_t *hndl, 
                                              UINT32 link, 
                                              enet_dir_t   dir,
                                              BOOL8 *force_enable);

PUBLIC PMC_ERROR enet_epmm_lf_set_force_enable(enet_handle_t *hndl, 
                                               UINT32 link, 
                                               enet_dir_t dir,
                                               BOOL8 enable);

PUBLIC PMC_ERROR enet_epmm_los_to_rf_consq_action_cfg(enet_handle_t *hndl, 
                                  UINT32 link, 
                                  BOOL8 enable);

PUBLIC PMC_ERROR enet_mpmo_cfg_get(enet_handle_t *hndl,
                                   UINT32 chnl,
                                   BOOL8 *mpmo_cfg_req);
PUBLIC PMC_ERROR enet_mpma_data_get(enet_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr,
                                    DOUBLE *epsilon_ptr);
PUBLIC PMC_ERROR enet_mpmo_prov(enet_handle_t *hndl,
                                UINT32 link,
                                UINT32 pkt_size,
                                UINT32 pkt_per_int,
                                UINT32 pkt_per_n,
                                UINT32 pkt_per_d);    

PUBLIC PMC_ERROR enet_mpmo_deprov(enet_handle_t *hndl,
                                  UINT32 link); 
                                  
PUBLIC PMC_ERROR enet_epmm_mpmo_ckctl_cfg(enet_handle_t *hndl,
                                          UINT32 ckctl_port, 
                                          UINT32 chnl,
                                          BOOL8 enable);

PUBLIC PMC_ERROR enet_mpmo_data_get(enet_handle_t *hndl,
                                    UINT32 link,
                                    UINT32 *pkt_size_ptr,
                                    UINT32 *pkt_per_int_ptr,
                                    UINT32 *pkt_per_n_ptr,
                                    UINT32 *pkt_per_d_ptr);

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */


PUBLIC PMC_ERROR enet_init(enet_handle_t *hndl, UINT32 link,  enet_datapath_t datapath, pmc_energy_state_request_t energy_state);
PUBLIC BOOL8 enet_start_state_test(enet_handle_t *hndl);
PUBLIC PMC_ERROR enet_uninit(enet_handle_t *hndl);
PUBLIC PMC_ERROR enet_datapath_cfg(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath, UINT32 chan, BOOL8 ts_en, BOOL8 use_page_swap);
PUBLIC PMC_ERROR enet_datapath_uncfg(enet_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR enet_tx_activate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath);
PUBLIC PMC_ERROR enet_tx_deactivate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath);
PUBLIC PMC_ERROR enet_rx_activate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath);
PUBLIC PMC_ERROR enet_rx_deactivate(enet_handle_t *hndl, UINT32 link, enet_datapath_t datapath);
PUBLIC BOOL8 enet_is_link_mapotn_test(enet_handle_t *hndl, 
                                     UINT32 link, 
                                     UINT32* mapotn_chnl_ptr);
PUBLIC void enet_convert_link_to_datapath_type(enet_handle_t *hndl, UINT32 link, enet_datapath_t* datapath);
PUBLIC void enet_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void enet_etrans_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void enet_epmm_energy_state_reg_set ( enet_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void enet_empei_e_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void enet_empei_i_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void enet_emac_energy_state_reg_set( enet_handle_t *hndl, pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR enet_flow_control_enable(enet_handle_t             *hndl, 
                                          UINT32                     link, 
                                          util_global_direction_t    dir,
                                          BOOL8                       enabled,
                                          UINT32                     pause_quanta,
                                          UINT32                     pause_quanta_thresh,
                                          UINT32                     cpb_sys_zone_thresh);
PUBLIC PMC_ERROR enet_flow_control_get(enet_handle_t *hndl, 
                                       UINT32                     link, 
                                       util_global_direction_t    dir,
                                       BOOL8                      *enabled,
                                       UINT32                     *pause_quanta,
                                       UINT32                     *pause_quanta_thresh);


PUBLIC PMC_ERROR enet_int_chnl_enable(enet_handle_t    *enet_handle,
                                      UINT32            link,
                                      enet_int_chnl_t  *int_table_ptr,
                                      enet_int_chnl_t  *int_en_table_ptr,
                                      BOOL8             enable,
                                      BOOL8             is_multichannels);

PUBLIC PMC_ERROR enet_int_chnl_clear(enet_handle_t    *enet_handle,
                                     UINT32            link,
                                     enet_int_chnl_t  *int_table_ptr,
                                     enet_int_chnl_t  *int_en_table_ptr,
                                     BOOL8             is_multichannels);

PUBLIC PMC_ERROR enet_int_chnl_retrieve(enet_handle_t    *enet_handle,
                                        UINT32            link,
                                        enet_int_chnl_t  *filt_table_ptr,
                                        enet_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR enet_int_chnl_enabled_check(enet_handle_t     *enet_handle,
                                             UINT32             link,
                                             enet_int_chnl_t   *int_en_table_ptr,
                                             BOOL8              *int_found_ptr,
                                             BOOL8              is_multichannels);


PUBLIC PMC_ERROR enet_max_frame_length_set(enet_handle_t *hndl, 
                                           UINT32 link, 
                                           UINT32 frame_len);
PUBLIC PMC_ERROR enet_max_frame_length_get(enet_handle_t *hndl, 
                                           UINT32 link, 
                                           UINT32 *p_frame_len);
PUBLIC PMC_ERROR enet_interrupt_enable(enet_handle_t *hndl,
                                       UINT32 link,
                                       BOOL8 enabled);
PUBLIC PMC_ERROR enet_extract_packet(enet_handle_t *hndl,                                      
                                     UINT32 link, 
                                     enet_dir_t direction,
                                     enet_extract_pkt_t extract_pkt_type);
PUBLIC PMC_ERROR enet_phy_status_get(enet_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 *p_is_enabled);
PUBLIC PMC_ERROR enet_send_test_frame_enable(enet_handle_t *hndl,
                                             UINT32 link,
                                             BOOL8 enabled);
PUBLIC PMC_ERROR enet_pkt_gen_cfg(enet_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8  enable,
                                  UINT32 pkt_size,
                                  UINT32 pyld_val,
                                  UINT32 pkt_interval,
                                  UINT32 da_hi,
                                  UINT32 da_low,
                                  UINT32 sa_hi,
                                  UINT32 sa_low,
                                  UINT32 len_etype,
                                  UINT32 vlan_out,
                                  UINT32 vlan_in);

PUBLIC void enet_alloc_init(enet_handle_t *enet_handle);

PUBLIC PMC_ERROR enet_handle_restart_init(enet_handle_t       *enet_handle,
                                          util_global_restart_init_cfg_t                *restart_init_cfg_ptr, 
                                          pmc_energy_state_t   top_energy_state_reg,
                                          enet_type_id_t       enet_type);

PUBLIC BOOL8 enet_prov_get(enet_handle_t             *hndl,
                           UINT32                    link,
                           BOOL8                     rx_chnl,
                           util_global_chnl_state_t  state);

PUBLIC UINT32 enet_enhanced_pmon_num_links_set_up_gsup43_pmon(enet_handle_t *hndl);

PUBLIC PMC_ERROR enet_tx_upi_set(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8          pkt_upi_val,
                                 UINT8          os_upi_val,
                                 BOOL8          enable_drop);

PUBLIC PMC_ERROR enet_tx_upi_get(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8         *pkt_upi_val_ptr,
                                 UINT8         *os_upi_val_ptr,
                                 BOOL8         *enable_drop_ptr);
PUBLIC PMC_ERROR enet_rx_upi_set(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8          pkt_upi_val,
                                 UINT8          os_upi_val);

PUBLIC PMC_ERROR enet_rx_upi_get(enet_handle_t *hndl, 
                                 UINT32         link, 
                                 UINT8         *pkt_upi_val_ptr,
                                 UINT8         *os_upi_val_ptr);

PUBLIC PMC_ERROR enet_emac_pause_frame_fwd_enable(enet_handle_t             *hndl,
                                                  UINT32                     link,
                                                  BOOL8                      enabled);

PUBLIC void enet_emac_pause_frame_fwd_get(enet_handle_t             *hndl,
                                          UINT32                     link,
                                          BOOL8                     *enabled_ptr);

PUBLIC BOOL8 enet_enhanced_pmon_prov_get(enet_handle_t             *hndl,
                                         UINT32                    link,
                                         BOOL8                     etrans_path,
                                         enet_data_rate_t          *data_rate);

PUBLIC PMC_ERROR enet_eclass_pkt_extract_cfg(enet_handle_t              *hndl, 
                                             UINT32                      link, 
                                             enet_dir_t                  enet_datapath,
                                             BOOL8                       enable,
                                             eclass_pkt_extract_cfg_t   *extract_cfg_ptr);


PUBLIC PMC_ERROR enet_eclass_pkt_extract_cleanup(enet_handle_t              *hndl, 
                                                 UINT32                      link, 
                                                 enet_dir_t                  enet_datapath);

PUBLIC BOOL8 enet_eclass_pkt_extract_status_get(enet_handle_t *hndl, 
                                                UINT32 link, 
                                                enet_dir_t   dir,
                                                eclass_pkt_extract_cfg_t *extract_cfg_ptr);

PUBLIC void enet_eclass_pkt_extract_cfg_get(enet_handle_t            *hndl, 
                                            UINT32                    link, 
                                            enet_dir_t                dir,
                                            UINT32                   *array_size_ptr,
                                            UINT32                   *array_used_size_ptr,
                                            eclass_pkt_extract_cfg_t *extract_cfg_ptr);

PUBLIC BOOL8 enet_eclass_pkt_extract_all_cfg_disabled_get(enet_handle_t *hndl,
                                                          enet_dir_t     dir);

PUBLIC BOOL8 enet_eclass_pkt_extract_link_cfg_disabled_get(enet_handle_t *hndl,
                                                           UINT32         link,
                                                           enet_dir_t     dir);

PUBLIC BOOL enet_100G_check_align_status_v_get(enet_handle_t *hndl);

PUBLIC void enet_100G_disable_align_status_loc_fault(enet_handle_t *hndl);

PUBLIC void enet_100G_enable_align_status_loc_fault(enet_handle_t *hndl);

PUBLIC void enet_100G_clear_align_status_loc_fault(enet_handle_t *hndl);

PUBLIC void enet_pcs_poll_count_set(enet_handle_t *hndl, UINT8 poll_count);

PUBLIC void enet_pcs_poll_count_get(enet_handle_t *hndl, UINT8 *poll_count);

PUBLIC void enet_emac_pcs_reset(enet_handle_t *hndl,
                                UINT32        link);

#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _ENET_H */

/* 
** end of file 
*/
